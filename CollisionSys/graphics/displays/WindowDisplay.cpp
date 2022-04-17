#include "WindowDisplay.h"

#include <SFML/Graphics.hpp>

#include "Sandbox.h"
#include "GJK.h"
#include "graphics/Text.h"

namespace CollSys {
	double
		WindowDisplay::linear_speed = 0.02,
		WindowDisplay::angular_speed = 2.0;

	WindowDisplay::WindowDisplay(Sandbox& parent, bool active) :
		display(parent, active),
		selected(parent.getShapeList().end()),
		window()
	{}

	void WindowDisplay::onActivation() {
		this->window.create(
			sf::VideoMode(800, 800),
			"CollSys Sandbox"
		);
		this->window.setFramerateLimit(30);
		this->setView();
		/*
		sf::View view({ 0.0f, 0.0f }, { 2.0f, 2.0f });
		this->window.setView(view);
		*/
	}

	void WindowDisplay::onDeactivation() {
		this->window.close();
	}

	void WindowDisplay::onUpdate() {
		this->handleEvents();
		this->render();
	}

	void WindowDisplay::setView() {
		Sandbox::ShapeList& shapes = this->m_parent.getShapeList();

		auto it = shapes.begin();
		AbstractShape& shape1 = **it;
		glib::vec2d
			tl = shape1.support(glib::vec2d(-1.0, 0.0)),
			br = tl;

		do {
			AbstractShape& shape = **it;

			glib::vec2d
				supx1 = shape.support(glib::vec2d(-1.0, 0.0)),
				supx2 = shape.support(glib::vec2d(1.0, 0.0)),
				supy1 = shape.support(glib::vec2d(0.0, -1.0)),
				supy2 = shape.support(glib::vec2d(0.0, 1.0));

			tl = glib::vec2d(
				std::min(tl.x, supx1.x),
				std::min(tl.y, supy1.y)
			);
			br = glib::vec2d(
				std::max(br.x, supx2.x),
				std::max(br.y, supy2.y)
			);
		} while (++it != shapes.end());

		sf::FloatRect viewRect(
			static_cast<float>(tl.x),
			static_cast<float>(tl.y),
			static_cast<float>(br.x - tl.x),
			static_cast<float>(br.y - tl.y)
		);
		this->window.setView(sf::View(viewRect));
	}

	void WindowDisplay::colorShapes() {
		Sandbox::ShapeList& shapes = this->m_parent.getShapeList();
		for (auto shape : shapes) {
			shape->setColor(sf::Color::White);
		}
		for (auto it1 = shapes.begin(); it1 != shapes.end(); ++it1) {
			AbstractShape& shape1 = *(*it1);
			auto it2 = it1; it2++;
			for (; it2 != shapes.end(); it2++) {
				AbstractShape& shape2 = *(*it2);
				CollSys::GJKSolver gjk_test(shape1, shape2);
				if (gjk_test.isContact()) {
					shape1.setColor(sf::Color::Red);
					shape2.setColor(sf::Color::Red);
				}
			}
		}
	}

	void WindowDisplay::handleEvents() {
		Sandbox::ShapeList& shapes = this->m_parent.getShapeList();
		sf::Event event;

		// event loop
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				this->makeActive(false);
				this->m_parent.closeWindow();
				return;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(this->window);
					glib::vec2d worldPos = glib::VectorCast<double>(this->window.mapPixelToCoords(pixelPos));
					CollSys::Point point("click", worldPos);
					this->selected = shapes.end();

					for (auto it = shapes.begin(); it != shapes.end(); it++) {
						AbstractShape& shape = *(*it);
						CollSys::GJKSolver point_test(shape, point);
						if (point_test.isContact()) {
							selected = it;
							break;
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					selected = shapes.end();
				}
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					this->makeActive(false);
					this->m_parent.closeWindow();
				}
				else if (event.key.code == sf::Keyboard::Tab) {
					if (this->selected != shapes.end()) {
						this->selected++;
					}
					else {
						this->selected = shapes.begin();
					}
				}
			}
		}
		// folyamatos user input
		if (this->selected != shapes.end()) {
			AbstractShape& shape = *(*this->selected);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { shape.rotate(angular_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { shape.rotate(-angular_speed); }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { shape.move(-linear_speed, 0.0f); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { shape.move(0.0f, -linear_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { shape.move(0.0f, linear_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { shape.move(linear_speed, 0.0f); }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { shape.scale(1.0, 0.96); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { shape.scale(1.0, 1.04); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { shape.scale(0.96, 1.0); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) { shape.scale(1.04, 1.0); }
		}
	}

	void WindowDisplay::render() {
		Sandbox::ShapeList& shapes = this->m_parent.getShapeList();

		// Contact checks
		this->colorShapes();

		if (this->selected != shapes.end()) {
			(*this->selected)->setColor(sf::Color::Yellow);
		}

		// Rendering
		this->window.clear();
		for (auto shape : shapes) {
			this->window.draw(*shape);
		}
		this->window.display();
	}
}
