#pragma once

#include <exception>
#include <iostream>

#include "Base/Command.h"
#include "general/string.h"

namespace CollSys::Commands {
	// -------------------- general --------------------

	class Help : public Command
	{
	public:
		Help(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class ListShapeTypes : public Command
	{
	public:
		ListShapeTypes(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class ListShapes : public Command
	{
	public:
		ListShapes(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Openwin : public Command
	{
	public:
		Openwin(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Exit : public Command
	{
	public:
		Exit(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Create : public CreatorCommand
	{
	public:
		Create(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Destroy : public Command
	{
	public:
		Destroy(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	// -------------------- transform --------------------

	class Move : public TransformCommand
	{
	public:
		Move(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Rotate : public TransformCommand
	{
	public:
		Rotate(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Scale : public TransformCommand
	{
	public:
		Scale(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	// -------------------- contact --------------------

	class CheckContacts : public Command
	{
	public:
		CheckContacts(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class CheckContact : public Command
	{
	public:
		CheckContact(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	// -------------------- fileio --------------------

	class SaveAs : public Command
	{
	public:
		SaveAs(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Save : public Command
	{
	public:
		Save(Sandbox& sandbox);
		void execute(std::stringstream& input) const;
	};

	class Merge : public CreatorCommand
	{
	public:
		Merge(Sandbox& sandbox);
		virtual void execute(std::stringstream& input) const;
	protected:
		void readShapes(std::ifstream& file) const;
		std::ifstream openFile(glib::string path) const;
	};

	class Load : public Merge {
	public:
		Load(Sandbox& sandbox);
		void execute(std::stringstream& input) const override;
	private:
		void deleteExistingShapes() const;
	};
}
