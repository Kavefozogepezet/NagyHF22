#include "Commands.h"

#include "debug/memtrace.h"

#include "Sandbox.h"

namespace CollSys {
	// -------------------- HELP --------------------

	Help::Help(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc =
			"Kilistazza a vegrehajthato parancsokat.\n"
			"A help parancs utan egy masik letezo parancsot irva hivjuk meg, "
			"kiirja a valasztott parancs reszletes leirasat.";
	}

	bool Help::execute(glib::linebuffer& input) {
		for (auto c : this->owner.getCmdReg()) {
			std::cout << "\033[30;107m -> " << c.first << ": \033[0m" << std::endl <<
				*c.second << std::endl << std::endl;
		}
		return true;
	}

	// -------------------- CREATE --------------------

	Create::Create(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc =
			"Letrehoz egy sikidomot.\n"
			"A create parancs utan be kell irni milyen sikidomot szeretnenk, "
			"majd a sikidom parametereit kell megadni.\n"
			"pelda: create circle 0.5";
	}

	bool Create::execute(glib::linebuffer& input)
	{
		glib::string shape_key;
		Sandbox::ShapeReg& sreg = this->owner.getShapeReg();

		input >> shape_key;
		auto it = sreg.get(shape_key);
		if (it != sreg.end()) {
			this->owner.getShapeList().push_back(it->second());
			std::cout << " A \"" << shape_key << "\" sikidom elkeszult." << std::endl << std::endl;
			return false;
		}
		else {
			std::cout << "Nem letezik \"" << shape_key << "\" sikidom." << std::endl << std::endl;
		}
		return true;
	}

	// -------------------- OPENWIN --------------------

	Openwin::Openwin(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megnyit egy uj ablakot, ahol a sikidomokat grafikusan szemlelteti.";
	}

	bool Openwin::execute(glib::linebuffer& input) {
		this->owner.openWindow();
		return true;
	}

	// -------------------- EXIT --------------------

	Exit::Exit(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilep a programbol.";
	}

	bool Exit::execute(glib::linebuffer& input) {
		this->owner.stop();
		return true;
	}
}