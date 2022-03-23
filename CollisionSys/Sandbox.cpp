#include "Sandbox.h"

#include "debug/memtrace.h"

#include "general/string.h"
#include "general/linebuffer.h"
#include "GJK.h"

namespace CollSys {
	int cs_main(int argc, char** argv) {
		Sandbox sandbox;
		sandboxInit(sandbox);
		sandbox.run();
		return 0;
	}

	double Sandbox::linear_speed = 0.02;
	double Sandbox::angular_speed = 2.0;

	Sandbox::Sandbox() :
		registry(),
		shapes(),
		selected(nullptr),
		is_running(true),
		is_win_open(false),
		window(nullptr)
	{
		this->registry.add("polygon", lambdaMaker<Polygon>());
		this->registry.add("circle", lambdaMaker<Circle>());
		this->registry.add("ellipse", lambdaMaker<Ellipse>());
		this->registry.add("point", lambdaMaker<Point>());
	}

	Sandbox::~Sandbox() {
		for (auto shape : shapes) {
			delete shape;
		}
	}

	void Sandbox::run() {
		while (this->is_running) {
			if (this->is_win_open) {
				this->winUpdate();
			}
			else {
				this->consoleUpdate();
			}
		}
	}

	Sandbox::ShapeReg& Sandbox::getShapeReg() {
		return this->registry;
	}

	Sandbox::ShapeList& Sandbox::getShapeList() {
		return this->shapes;
	}

	void Sandbox::winUpdate() {
		// Event loop
		sf::Event event;
		while (this->window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				this->window->close();
				delete this->window;
				this->is_win_open = false;
				return;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(*this->window);
					glib::vec2d worldPos = glib::VectorCast<double>(this->window->mapPixelToCoords(pixelPos));
					CollSys::Point point(worldPos);
					this->selected = nullptr;

					for (auto shape : this->shapes) {
						CollSys::GJKSolver point_test(*shape, point);
						if (point_test.isContact()) {
							selected = shape;
							break;
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					selected = nullptr;
				}
			}
		}
		// User input
		if (selected) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { selected->rotate(angular_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { selected->rotate(-angular_speed); }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { selected->move(-linear_speed, 0.0f); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { selected->move(0.0f, -linear_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { selected->move(0.0f, linear_speed); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { selected->move(linear_speed, 0.0f); }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { selected->scale(1.0, 0.96); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { selected->scale(1.0, 1.04); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { selected->scale(0.96, 1.0); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) { selected->scale(1.04, 1.0); }
		}
		// Contact checks
		this->contactCheck();

		if (selected) {
			selected->setColor(sf::Color::Yellow);
		}

		// Rendering
		this->window->clear();
		for (auto shape : shapes) {
			this->window->draw(*shape);
		}
		this->window->display();
	}

	void Sandbox::consoleUpdate() {
		std::cout << ">>> ";

		glib::linebuffer lbuff;
		std::cin >> lbuff;

		glib::string cmd;
		lbuff >> cmd;

		if (cmd == "showwin") {
			this->window = new sf::RenderWindow(sf::VideoMode(800, 800), "CollSys Sandbox");
			this->window->setFramerateLimit(30);
			sf::View view({ 0.0f, 0.0f }, { 2.0f, 2.0f });
			this->window->setView(view);
			this->is_win_open = true;
		}
		else if (cmd == "create") {
			glib::string shape_key;
			lbuff >> shape_key;
			auto it = this->registry.get(shape_key);
			if (it != this->registry.end()) {
				this->shapes.push_back(it->second());
				std::cout << " A \"" << shape_key << "\" sikidom elkeszult." << std::endl << std::endl;
			}
			else {
				std::cout << "Nem letezik \"" << shape_key << "\" sikidom." << std::endl << std::endl;
			}
		}
		else if (cmd == "exit") {
			this->is_running = false;
			return;
		}
		else {
			std::cout << "A \"" << cmd << 
				"\" parancs nem letezik. Hasznalja a help parancsot tobb informacioert." 
				<< std::endl << std::endl;
		}

		this->contactCheck();
	}

	void Sandbox::contactCheck() {
		for (auto shape : this->shapes) {
			shape->setColor(sf::Color::White);
		}
		for (auto it1 = this->shapes.begin(); it1 != this->shapes.end(); ++it1) {
			AbstractShape* shape1 = *it1;
			auto it2 = it1; it2++;
			for (; it2 != this->shapes.end(); it2++) {
				AbstractShape* shape2 = *it2;
				CollSys::GJKSolver gjk_test(*shape1, *shape2);
				if (gjk_test.isContact()) {
					shape1->setColor(sf::Color::Red);
					shape2->setColor(sf::Color::Red);
					if (!this->is_win_open) {
						std::cout << "Ez a ket objektum erintkezik: " <<
							shape1->getName() << "; " << shape2->getName() << std::endl;
					}
				}
			}
		}
	}
}
