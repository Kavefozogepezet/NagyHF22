#pragma once

#include "Base/Command.h"

namespace CollSys {
	// -------------------- general --------------------

	class Help : public Command
	{
	public:
		Help(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class ListShapes : public Command
	{
	public:
		ListShapes(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class Openwin : public Command
	{
	public:
		Openwin(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class Exit : public Command
	{
	public:
		Exit(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	// -------------------- transform --------------------

	class Create : public Command
	{
	public:
		Create(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class Move : public TransformCommand
	{
	public:
		Move(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class Rotate : public TransformCommand
	{
	public:
		Rotate(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	class Scale : public TransformCommand
	{
	public:
		Scale(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	// -------------------- contact --------------------

	class CheckContacts : public Command
	{
	public:
		CheckContacts(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

	// -------------------- fileio --------------------

	class SaveAs : public Command
	{
	public:
		SaveAs(Sandbox& sandbox);
		bool execute(glib::linebuffer& input) const;
	};

}
