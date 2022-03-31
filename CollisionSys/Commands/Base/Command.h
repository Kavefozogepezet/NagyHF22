#pragma once

#include "general/string.h"
#include "general/linebuffer.h"

namespace CollSys {
	class Sandbox;
	class AbstractShape;
}

namespace CollSys::Commands {
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

	class CreatorCommand : public Command {
	public:
		CreatorCommand(Sandbox& sandbox);

		virtual ~CreatorCommand() = default;
	protected:
		AbstractShape* createShape(const glib::string& key) const;

		bool validateShape(AbstractShape* shape) const;
	};
}
