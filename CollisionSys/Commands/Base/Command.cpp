#include "Command.h"

#include "debug/memtrace.h"

#include "general/string.h"
#include "general/consoleStyle.h"
#include "Sandbox.h"

using cStyle = glib::consoleStyle;

namespace CollSys::Commands {
	Command::Command(Sandbox& sandbox) :
		reciever(sandbox),
		desc(),
		params()
	{}

	bool Command::postInputCheck(glib::linebuffer& input) const {
		if (input.eol()) {
			std::cout << cStyle::error << "Nem adott meg eleg parametert." << std::endl <<
				"A paramcs parameterlistaja: " << this->params << cStyle::none << std::endl;
			return false;
		}
		return true;
	}

	std::ostream& operator << (std::ostream& stream, const Command& cmd) {
		stream << cmd.desc;
		if (!cmd.params.empty()) {
			std::cout << std::endl << "parameterek: " << cmd.params;
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
		std::cout << cStyle::error << "Nincs " << name << " nevu sikidom" << cStyle::none << std::endl;
		return nullptr;
	}

	CreatorCommand::CreatorCommand(Sandbox& sandbox) :
		Command(sandbox)
	{}

	AbstractShape* CreatorCommand::createShape(const glib::string & key) const {
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();
		auto it = sreg.get(key);
		if (it == sreg.end()) {
			std::cout << cStyle::error << "Nem letezik \"" << key << "\" sikidom." << cStyle::none << std::endl;
			return nullptr;
		}
		return it->second(key);
	}

	bool CreatorCommand::validateShape(AbstractShape* shape) const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == shape->getName()) {
				std::cout << cStyle::error << "Mar letezik \"" << shape->getName() << "\" nevu sikidom." << cStyle::none << std::endl;
				return false;
			}
		}
		return true;
	}
}
