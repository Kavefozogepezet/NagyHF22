#pragma once

#include "Base/Command.h"

namespace CollSys::Commands {
	// -------------------- general --------------------

	class Help : public Command
	{
	public:
		Help(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class ListShapeTypes : public Command
	{
	public:
		ListShapeTypes(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class ListShapes : public Command
	{
	public:
		ListShapes(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Openwin : public Command
	{
	public:
		Openwin(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Exit : public Command
	{
	public:
		Exit(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Create : public CreatorCommand
	{
	public:
		Create(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	// -------------------- transform --------------------

	class Move : public TransformCommand
	{
	public:
		Move(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Rotate : public TransformCommand
	{
	public:
		Rotate(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Scale : public TransformCommand
	{
	public:
		Scale(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	// -------------------- contact --------------------

	class CheckContacts : public Command
	{
	public:
		CheckContacts(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	// -------------------- fileio --------------------

	class SaveAs : public Command
	{
	public:
		SaveAs(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	};

	class Load : public CreatorCommand {
	public:
		Load(Sandbox& sandbox);
		bool execute(std::stringstream& input) const;
	private:
		void readShapes(std::ifstream& file) const ;
		void deleteExistingShapes() const;
	};
}
