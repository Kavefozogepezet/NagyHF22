#include "Command.h"

#include "debug/memtrace.h"

#include "general/string.h"
#include "general/consoleStyle.h"
#include "Sandbox.h"

using cStyle = glib::consoleStyle;

namespace CollSys {
	Command::Command(Sandbox& sandbox) :
		reciever(sandbox),
		desc()
	{}

	std::ostream& operator << (std::ostream& stream, const Command& cmd) {
		stream << cmd.desc;
		return stream;
	}

	TransformCommand::TransformCommand(Sandbox& sandbox) :
		Command(sandbox)
	{}

	AbstractShape* TransformCommand::readShape(glib::linebuffer & input) {
		glib::string name;
		input >> name;

		if (input.eol()) {
			std::cout << cStyle::error << "Nem adta meg a sikidom nevet." << cStyle::none << std::endl;
			return nullptr;
		}

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
}
