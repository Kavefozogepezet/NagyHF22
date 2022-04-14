#include "Command.h"

#include "debug/memtrace.h"

#include "general/string.h"
#include "graphics/consoleStyle.h"
#include "Sandbox.h"

namespace CollSys::Commands {
	using cStyle = consoleStyle;

	Command::Command(Sandbox& sandbox) :
		reciever(sandbox),
		desc(),
		params()
	{}

	bool Command::postInputCheck(std::stringstream& input) const {
		if (!input) {
			cStyle::error() << "Nem megfelelo a parameterezes." << std::endl <<
				"A paramcs parameterlistaja: " << this->params << cStyle::endl;
			return false;
		}
		return true;
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

	AbstractShape* TransformCommand::getShape(glib::string& name) const {
		bool found_shape = false;
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == name) {
				found_shape = true;
				return s;
			}
		}
		cStyle::error() << "Nincs " << name << " nevu sikidom" << cStyle::endl;
		return nullptr;
	}

	CreatorCommand::CreatorCommand(Sandbox& sandbox) :
		Command(sandbox)
	{}

	AbstractShape* CreatorCommand::createShape(const glib::string & key) const {
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();
		auto it = sreg.get(key);
		if (it == sreg.end()) {
			cStyle::error() << "Nem letezik \"" << key << "\" sikidom." << cStyle::endl;
			return nullptr;
		}
		return it->second(key);
	}

	bool CreatorCommand::validateShape(AbstractShape* shape) const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == shape->getName()) {
				cStyle::error() << "Mar letezik \"" << shape->getName() << "\" nevu sikidom." << cStyle::endl;
				return false;
			}
		}
		return true;
	}
}
