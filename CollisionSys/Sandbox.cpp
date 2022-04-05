#include "Sandbox.h"

#include <iostream>
#include <sstream>

#include "GJK.h"
#include "general/string.h"
#include "general/consoleStyle.h"

#include "debug/memtrace.h"

using cStyle = glib::consoleStyle;

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
		s_registry(),
		shapes(),
		selected(nullptr),
		is_running(true),
		is_win_open(false),
		window(nullptr)
	{
		this->s_registry.add("polygon", lambdaMaker<Polygon>());
		this->s_registry.add("circle", lambdaMaker<Circle>());
		this->s_registry.add("ellipse", lambdaMaker<Ellipse>());
		this->s_registry.add("point", lambdaMaker<Point>());

		this->cmd_registry.add("help", new Commands::Help(*this));
		this->cmd_registry.add("openwin", new Commands::Openwin(*this));
		this->cmd_registry.add("shapetypes", new Commands::ListShapeTypes(*this));
		this->cmd_registry.add("shapes", new Commands::ListShapes(*this));
		this->cmd_registry.add("create", new Commands::Create(*this));
		this->cmd_registry.add("move", new Commands::Move(*this));
		this->cmd_registry.add("rotate", new Commands::Rotate(*this));
		this->cmd_registry.add("scale", new Commands::Scale(*this));
		this->cmd_registry.add("contacts", new Commands::CheckContacts(*this));
		this->cmd_registry.add("saveas", new Commands::SaveAs(*this));
		this->cmd_registry.add("load", new Commands::Load(*this));
		this->cmd_registry.add("exit", new Commands::Exit(*this));
	}

	Sandbox::~Sandbox() {
		for (auto shape : this->shapes) {
			delete shape;
		}
		for (auto& cmd : this->cmd_registry) {
			delete cmd.second;
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

	void Sandbox::stop() {
		this->is_running = false;
		if (this->is_win_open) {
			this->closeWindow();
		}
	}

	void Sandbox::openWindow() {
		if (this->is_win_open) {
			return;
		}
		this->window = new sf::RenderWindow(sf::VideoMode(800, 800), "CollSys Sandbox");
		this->window->setFramerateLimit(30);
		sf::View view({ 0.0f, 0.0f }, { 2.0f, 2.0f });
		this->window->setView(view);
		this->is_win_open = true;
	}

	void Sandbox::closeWindow() {
		if (!this->is_win_open) {
			return;
		}
		this->window->close();
		delete this->window;
		this->is_win_open = false;
	}

	Sandbox::CmdReg& Sandbox::getCmdReg() {
		return this->cmd_registry;
	}

	Sandbox::ShapeReg& Sandbox::getShapeReg() {
		return this->s_registry;
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
				this->closeWindow();
				return;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i pixelPos = sf::Mouse::getPosition(*this->window);
					glib::vec2d worldPos = glib::VectorCast<double>(this->window->mapPixelToCoords(pixelPos));
					CollSys::Point point("click", worldPos);
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

		glib::string line;
		std::getline(std::cin, line);
		std::stringstream lbuff(line);

		glib::string cmd;
		if (!(lbuff >> cmd)) {
			std::cout << cStyle::warn << "Nem adott meg parancsot. A parancsok listajat megtekintheti a help parancsal." << cStyle::none << std::endl;
			return;
		}

		bool cmd_found = false;

		auto it = this->cmd_registry.get(cmd);
		if (it != this->cmd_registry.end()) {
			it->second->execute(lbuff);
			glib::string str;
			if (lbuff >> str) {
				std::cout << cStyle::warn << "A parancs nem olvasta vegig a sort, ezek a parameterek elvesztek :" << cStyle::none;
				do {
					std::cout << " " << str;
				} while (lbuff >> str);
				std::cout << std::endl;
			}
		}
		else {
			std::cout << cStyle::error << "A \"" << cmd <<
				"\" parancs nem letezik. A parancsok listajat megtekintheti a help parancsal." <<
				cStyle::none << std::endl;
		}
		std::cout << std::endl;
	}

	void Sandbox::contactCheck() {
		for (auto shape : this->shapes) {
			shape->setColor(sf::Color::White);
		}
		cStyle contact_style = cStyle().fg(cStyle::GREEN);
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
						std::cout << contact_style << "Ez a ket objektum erintkezik: " <<
							shape1->getName() << "; " << shape2->getName() << cStyle::none << std::endl << std::endl;
					}
				}
			}
		}
	}
}
