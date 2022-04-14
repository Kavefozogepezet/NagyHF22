#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "Shapes/Shapes.h"
#include "Commands/Commands.h"
#include "general/list.h"
#include "general/registry.h"
#include "graphics/displays/ConsoleDisplay.h"
#include "graphics/displays/WindowDisplay.h"

namespace CollSys {
	class Sandbox
	{
	public:
		using LambdaType = std::function<AbstractShape* (const glib::string&)>;

		using CmdReg = glib::registry<glib::string, const Commands::Command* const>;
		using ShapeReg = glib::registry<glib::string, LambdaType>;
		using ShapeList = glib::list<AbstractShape*>;
	public:
		static double linear_speed;
		static double angular_speed;

		template<typename Shape>
		static LambdaType lambdaMaker() {
			return [](const glib::string& type) { return new Shape(type); };
		}

	public:
		Sandbox();
		~Sandbox();

		void run();
		void stop();

		void openWindow();
		void closeWindow();

		CmdReg& getCmdReg();
		ShapeReg& getShapeReg();

		ShapeList& getShapeList();
	private:
		CmdReg cmd_registry;
		ShapeReg s_registry;
		ShapeList shapes;

		ConsoleDisplay cdisp;
		WindowDisplay wdisp;

		AbstractShape* selected;
		
		bool is_running;
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
