#pragma once

#include "Base/Command.h"

namespace CollSys {
	class Help : public Command
	{
	public:
		Help(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Create : public Command
	{
	public:
		Create(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Openwin : public Command
	{
	public:
		Openwin(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Exit : public Command
	{
	public:
		Exit(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};
}
