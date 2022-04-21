#pragma once

#include <sstream>

#include "general/string.h"

namespace CollSys {
	class Sandbox;
	class AbstractShape;
}

namespace CollSys::Commands {
	// -------------------- error during execution --------------------

	class Error
	{
	public:
		Error(const glib::string& desc);
		glib::string desc;
		void print();
	};

	// -------------------- alap osztályok --------------------

	class Command
	{
	public:
		Command(Sandbox& sandbox);

		virtual ~Command() = default;

		virtual void execute(std::stringstream& input) const = 0;

		friend std::ostream& operator << (std::ostream& stream, const Command& cmd);
	protected:
		void postInputCheck(std::stringstream& input) const;

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

		void validateShape(AbstractShape* shape) const;
	};
}
