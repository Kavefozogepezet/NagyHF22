#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "Shapes/Shapes.h"
#include "general/list.h"
#include "general/registry.h"

namespace CollSys {
	class Sandbox
	{
	public:
		using LambdaType = std::function<AbstractShape* ()>;

		using ShapeReg = glib::registry<glib::string, LambdaType>;
		using ShapeList = glib::list<AbstractShape*>;
	public:
		static double linear_speed;
		static double angular_speed;

		template<typename Shape>
		static LambdaType lambdaMaker() {
			return []() -> AbstractShape* { return new Shape; };
		}

	public:
		Sandbox();

		~Sandbox();

		void run();

		ShapeReg& getShapeReg();

		ShapeList& getShapeList();
	private:
		ShapeReg registry;
		ShapeList shapes;

		AbstractShape* selected;

		bool is_running;

		bool is_win_open;
		sf::RenderWindow* window;

		void winUpdate();

		void consoleUpdate();

		void contactCheck();
	};

	int cs_main(int argc, char** argv);

	extern void sandboxInit(Sandbox& sandbox);
}

#ifdef COLLSYS_MAIN
int main(int argc, char** argv) {
	CollSys::cs_main(argc, argv);
	return 0;
}
#endif
