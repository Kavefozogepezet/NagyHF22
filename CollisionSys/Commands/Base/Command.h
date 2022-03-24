#pragma once

#include "general/string.h"
#include "general/linebuffer.h"

namespace CollSys {
	class Sandbox;

	class Command
	{
	public:
		Command(Sandbox& sandbox);

		virtual ~Command();

		virtual bool execute(glib::linebuffer& input) = 0;

		friend std::ostream& operator << (std::ostream& stream, const Command& cmd);
	protected:
		Sandbox& owner;
		glib::string desc;
	};
}
