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
		using LambdaType = std::function<ConvexShape* (const glib::string&)>;

		using CmdReg = glib::registry<glib::string, const Commands::Command* const>;
		using ShapeReg = glib::registry<glib::string, LambdaType>;
		using ShapeList = glib::list<ConvexShape*>;
	public:
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

		const glib::string& getMyFile() const;
		void setMyFile(const glib::string& path);
	private:
		CmdReg cmd_registry;
		ShapeReg s_registry;
		ShapeList shapes;

		ConsoleDisplay cdisp;
		WindowDisplay wdisp;
		
		bool is_running;

		glib::string myfile;
	};

	int csMain(int argc, char** argv);

	extern void sandboxInit(Sandbox& sandbox);
}

#ifdef COLLSYS_MAIN
int main(int argc, char** argv) {
	return CollSys::csMain(argc, argv);
}
#endif
