#include "Commands.h"

#include "debug/memtrace.h"

#include "Sandbox.h"
#include "general/consoleStyle.h"

using cStyle = glib::consoleStyle;

namespace CollSys {
	// -------------------- HELP --------------------

	Help::Help(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a vegrehajthato parancsokat.";
	}

	bool Help::execute(glib::linebuffer& input) {
		cStyle list_entry = cStyle().fg(cStyle::BLACK).bg(cStyle::LIGHT_GRAY);
		for (auto c : this->reciever.getCmdReg()) {
			std::cout << list_entry << ' ' << c.first << ' ' << cStyle::none << ':' << std::endl <<
				*c.second << std::endl;
		}
		return true;
	}

	// -------------------- LIST SHAPES --------------------

	ListShapes::ListShapes(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a lehetseges sikidom tipusokat.";
	}

	bool ListShapes::execute(glib::linebuffer& input) {
		Sandbox::ShapeReg& shape_reg = this->reciever.getShapeReg();
		for (auto& sr : shape_reg) {
			std::cout << sr.first << std::endl;
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
			"majd a sikidom parametereit kell megadni:\n"
			"create <sikidom tipusa> <sikidom neve> <egyeb parameterek>";
	}

	bool Create::execute(glib::linebuffer& input)
	{
		glib::string shape_key, shape_name;
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();

		input >> shape_key;
		auto it = sreg.get(shape_key);
		if (it == sreg.end()) {
			std::cout << cStyle::error << "Nem letezik \"" << shape_key << "\" sikidom." << cStyle::none << std::endl;
			return false;
		}
		AbstractShape* shape = it->second();
		if (!shape->read(input)) {
			std::cout << cStyle::error << "Hiba a sikidom beolvasasakor." << cStyle::none << std::endl;
			delete shape;
			return false;
		}
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto s : shapes) {
			if (s->getName() == shape->getName()) {
				std::cout << cStyle::error << "Mar letezik \"" << shape->getName() << "\" nevu sikidom." << cStyle::none << std::endl;
				delete shape;
				return false;
			}
		}
		shapes.push_back(shape);
		std::cout << " A \"" << shape->getName() << "\" nevu " << shape_key << " tipusu sikidom elkeszult." << std::endl;
		
		return true;
	}

	// -------------------- MOVE --------------------

	Move::Move(Sandbox& sandbox) :
		TransformCommand(sandbox)
	{
		this->desc =
			"Elmozgat egy sikidomot.\n"
			"A move parancs utan be kell irni a mozgatni kivant sikidom nevet, majd a mozgatas vektoranak x es y komponenset:\n"
			"move <sikidom neve> <x> <y>";
	}

	bool Move::execute(glib::linebuffer& input) {
		AbstractShape* shape = this->readShape(input);
		if (shape == nullptr) {
			return false;
		}
		glib::vec2d vec;
		input >> vec.x >> vec.y;

		if (input.eol()) {
			std::cout << cStyle::error << "Nem adott meg eleg parametert" << cStyle::none << std::endl;
			return false;
		}
		if (!vec.x && !vec.y) {
			std::cout << cStyle::warn << "A mozgatas vektora { 0, 0 }. Lehetseges, hogy helytelenul adta meg a komponenseket." << std::endl;
			return false;
		}
		shape->move(vec);
		std::cout << "A " << shape->getName() << " sikidom elmozgatva a " << vec << " vektorral." << std::endl;
		return true;
	}

	// -------------------- ROTATE --------------------

	Rotate::Rotate(Sandbox& sandbox) :
		TransformCommand(sandbox)
	{
		this->desc =
			"Elforgat egy sikidomot.\n"
			"A rotate parancs utan be kell irni a forgatni kivant sikidom nevet, majd a forgatas szoget:\n"
			"rotate <sikidom neve> <szog fokban>";
	}

	bool Rotate::execute(glib::linebuffer& input) {
		AbstractShape* shape = this->readShape(input);
		if (shape == nullptr) {
			return false;
		}
		double angle;
		input >> angle;

		if (input.eol()) {
			std::cout << cStyle::error << "Nem adott meg eleg parametert" << cStyle::none << std::endl;
			return false;
		}
		if (!angle) {
			std::cout << cStyle::warn << "A forgatas szoge 0 fok. Lehetseges, hogy helytelenul adta meg a szoget." << std::endl;
			return false;
		}
		shape->rotate(angle);
		std::cout << "A " << shape->getName() << " sikidom elforgatva " << angle << " fokkal." << std::endl;
		return true;
	}

	// -------------------- OPENWIN --------------------

	Openwin::Openwin(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megnyit egy uj ablakot, ahol a sikidomok grafikusan abrazolva jelennek meg.";
	}

	bool Openwin::execute(glib::linebuffer& input) {
		this->reciever.openWindow();
		return true;
	}

	// -------------------- EXIT --------------------

	Exit::Exit(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilep a programbol.";
	}

	bool Exit::execute(glib::linebuffer& input) {
		this->reciever.stop();
		return true;
	}
}