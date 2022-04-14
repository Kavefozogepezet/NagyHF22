#include "Sandbox.h"

#include <iostream>
#include <sstream>

#include "GJK.h"
#include "general/string.h"
#include "graphics/consoleStyle.h"

#include "debug/memtrace.h"

namespace CollSys {
	using cStyle = consoleStyle;

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
		cdisp(*this, true),
		wdisp(*this)
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
	
#ifndef COLLSYS_LIB
	void sandboxInit(Sandbox& sandbox) {}
#endif
}
