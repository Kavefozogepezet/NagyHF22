#pragma once

#include "Base/Command.h"

namespace CollSys {
	class Help : public Command
	{
	public:
		Help(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class ListShapes : public Command
	{
	public:
		ListShapes(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Create : public Command
	{
	public:
		Create(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Move : public TransformCommand
	{
	public:
		Move(Sandbox& sandbox);
		bool execute(glib::linebuffer& input);
	};

	class Rotate : public TransformCommand
	{
	public:
		Rotate(Sandbox& sandbox);
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
