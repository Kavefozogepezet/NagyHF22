#include "Command.h"

#include "debug/memtrace.h"

#include "general/string.h"
#include "graphics/consoleStyle.h"
#include "Sandbox.h"

namespace CollSys::Commands {
	using cStyle = consoleStyle;

	Error::Error(const glib::string& desc) : desc(desc) {}

	void Error::print() {
		cStyle::error() << this->desc << cStyle::endl;
	}

	Command::Command(Sandbox& sandbox) :
		reciever(sandbox),
		desc(),
		params()
	{}

	void Command::postInputCheck(std::stringstream& input) const {
		if (!input) {
			throw Error("Nem megfelelo a parameterezes.\nA paramcs parameterlistaja: " + this->params);
		}
	}

	std::ostream& operator << (std::ostream& stream, const Command& cmd) {
		stream << cmd.desc;
		if (!cmd.params.empty()) {
			stream << std::endl << "parameterek: " << cmd.params;
		}
		return stream;
	}

	TransformCommand::TransformCommand(Sandbox& sandbox) :
		Command(sandbox)
	{}

	ConvexShape* TransformCommand::getShape(glib::string& name) const {
		bool found_shape = false;
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == name) {
				found_shape = true;
				return s;
			}
		}
		throw Error("Nincs" + name + " nevu sikidom");
	}

	CreatorCommand::CreatorCommand(Sandbox& sandbox) :
		Command(sandbox)
	{}

	ConvexShape* CreatorCommand::createShape(const glib::string & key) const {
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();
		auto it = sreg.get(key);
		if (it == sreg.end()) {
			throw Error("Nem letezik \"" + key + "\" sikidom.");
		}
		return it->second(key);
	}

	void CreatorCommand::validateShape(ConvexShape* shape) const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == shape->getName()) {
				delete shape;
				throw Error("Mar letezik \"" + shape->getName() + "\" nevu sikidom.");
			}
		}
	}
}
