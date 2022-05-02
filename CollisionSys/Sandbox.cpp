#include "Sandbox.h"

#include <iostream>
#include <sstream>
#include <filesystem>

#include "GJK.h"
#include "general/string.h"
#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	void readHeader() {
		std::ifstream file("header.txt");

		if (!file.is_open()) {
			cStyle::error() <<
				"+------------------------------+" << std::endl <<
				"| A header file nem talalhato. |" << std::endl <<
				"+------------------------------+" << cStyle::endl;
		}

		glib::string line;
		while (file) {
			std::getline(file, line);
			std::cout << line << std::endl;
		}
	}

	int csMain(int argc, char** argv) {
		readHeader();
		Sandbox sandbox;
		sandboxInit(sandbox);
		sandbox.run();
		return 0;
	}

	Sandbox::Sandbox() :
		cmd_registry(),
		s_registry(),
		shapes(),
		cdisp(*this, true),
		wdisp(*this),
		is_running(true),
		myfile()
	{
		this->s_registry.add("polygon", lambdaMaker<Polygon>());
		this->s_registry.add("regular", lambdaMaker<RegularPolygon>());
		this->s_registry.add("circle", lambdaMaker<Circle>());
		this->s_registry.add("ellipse", lambdaMaker<Ellipse>());
		this->s_registry.add("point", lambdaMaker<Point>());
		this->s_registry.add("bezier", lambdaMaker<BezierCurve>());

		this->cmd_registry.add("help", new Commands::Help(*this));
		this->cmd_registry.add("openwin", new Commands::Openwin(*this));
		this->cmd_registry.add("shapetypes", new Commands::ListShapeTypes(*this));
		this->cmd_registry.add("shapes", new Commands::ListShapes(*this));
		this->cmd_registry.add("create", new Commands::Create(*this));
		this->cmd_registry.add("destroy", new Commands::Destroy(*this));
		this->cmd_registry.add("move", new Commands::Move(*this));
		this->cmd_registry.add("rotate", new Commands::Rotate(*this));
		this->cmd_registry.add("scale", new Commands::Scale(*this));
		this->cmd_registry.add("contacts", new Commands::CheckContacts(*this));
		this->cmd_registry.add("contact", new Commands::CheckContact(*this));
		this->cmd_registry.add("saveas", new Commands::SaveAs(*this));
		this->cmd_registry.add("save", new Commands::Save(*this));
		this->cmd_registry.add("merge", new Commands::Merge(*this));
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
		this->is_running = true;
		while (this->is_running) {
			this->cdisp.update();
			this->wdisp.update();
		}
	}

	void Sandbox::stop() {
		this->is_running = false;
		this->cdisp.makeActive(false);
		this->wdisp.makeActive(false);
	}

	void Sandbox::openWindow() {
		this->wdisp.makeActive(true);
		this->cdisp.makeActive(false);
	}

	void Sandbox::closeWindow() {
		this->cdisp.makeActive(true);
		this->wdisp.makeActive(false);
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

	const glib::string& Sandbox::getMyFile() const {
		return this->myfile;
	}

	void Sandbox::setMyFile(const glib::string& path) {
		bool exists = std::filesystem::exists(std::filesystem::path(path.c_str()));
		if (exists) {
			this->myfile = path;
		}
	}
	
#ifndef COLLSYS_LIB
	void sandboxInit(Sandbox& sandbox) {}
#endif
}
