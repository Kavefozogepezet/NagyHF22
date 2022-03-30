#pragma once

#include "general/string.h"
#include "general/linebuffer.h"

namespace CollSys {
	class Sandbox;
	class AbstractShape;

	class Command
	{
	public:
		Command(Sandbox& sandbox);

		virtual ~Command() = default;

		virtual bool execute(glib::linebuffer& input) const = 0;

		friend std::ostream& operator << (std::ostream& stream, const Command& cmd);
	protected:
		bool postInputCheck(glib::linebuffer& input) const;

		Sandbox& reciever;
		glib::string desc, params;
	};

	class TransformCommand : public Command
	{
	public:
		TransformCommand(Sandbox& sandbox);

		virtual ~TransformCommand() = default;
	protected:
		AbstractShape* getShape(glib::string& name) const;
	};
}
