#include "Command.h"

namespace CollSys {
	Command::Command(Sandbox& sandbox) :
		owner(sandbox),
		desc()
	{}

	Command::~Command() {}

	std::ostream& operator << (std::ostream& stream, const Command& cmd) {
		stream << cmd.desc;
		return stream;
	}
}
