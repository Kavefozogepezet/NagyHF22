#include "Commands.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "debug/memtrace.h"

#include "Sandbox.h"
#include "general/consoleStyle.h"
#include "GJK.h"

using cStyle = glib::consoleStyle;

namespace CollSys {
	// -------------------- HELP --------------------

	Help::Help(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a vegrehajthato parancsokat.";
	}

	bool Help::execute(glib::linebuffer& input) const {
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

	bool ListShapes::execute(glib::linebuffer& input) const {
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
			"majd a sikidom parametereit kell megadni.";
		this->params =
			"<sikidom tipusa> <sikidom neve> <egyeb parameterek>";
	}

	bool Create::execute(glib::linebuffer& input) const {
		glib::string shape_key, shape_name;
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();

		input >> shape_key;
		if (!this->postInputCheck(input)) {
			return false;
		}
		auto it = sreg.get(shape_key);
		if (it == sreg.end()) {
			std::cout << cStyle::error << "Nem letezik \"" << shape_key << "\" sikidom." << cStyle::none << std::endl;
			return false;
		}
		AbstractShape* shape = it->second(shape_key);
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
			"A move parancs utan be kell irni a mozgatni kivant sikidom nevet, majd a mozgatas vektoranak x es y komponenset.";
		this->params =
			"<sikidom neve> <x elmozdulas> <y elmozdulas>";
	}

	bool Move::execute(glib::linebuffer& input) const {
		glib::vec2d vec;
		glib::string name;
		input >> name >> vec.x >> vec.y;

		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->getShape(name);
		if (shape == nullptr) {
			return false;
		}
		if (!vec.x && !vec.y) {
			std::cout << cStyle::warn << "A mozgatas vektora { 0, 0 }. Lehetseges, hogy helytelenul adta meg a komponenseket." << cStyle::none << std::endl;
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
			"A rotate parancs utan be kell irni a forgatni kivant sikidom nevet, majd a forgatas szoget.";
		this->params =
			"rotate <sikidom neve> <szog fokban>";
	}

	bool Rotate::execute(glib::linebuffer& input) const {
		glib::string name;
		double angle;
		input >> name >> angle;

		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->getShape(name);
		if (shape == nullptr) {
			return false;
		}
		if (!angle) {
			std::cout << cStyle::warn << "A forgatas szoge 0 fok. Lehetseges, hogy helytelenul adta meg a szoget." << cStyle::none << std::endl;
			return false;
		}
		shape->rotate(angle);
		std::cout << "A " << shape->getName() << " sikidom elforgatva " << angle << " fokkal." << std::endl;
		return true;
	}

	// -------------------- SCALE --------------------

	Scale::Scale(Sandbox& sandbox) :
		TransformCommand(sandbox)
	{
		this->desc =
			"Atmeretez egy sikidomot.\n"
			"A scale parancs utan be kell irni a sikidom nevet, és a meretenek x és y szorzójat.";
		this->params =
			"<sikidom neve> <x szorzo> <y szorzo>";
	}

	bool Scale::execute(glib::linebuffer& input) const {
		glib::string name;
		glib::vec2d vec;
		input >> name >> vec.x >> vec.y;

		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->getShape(name);
		if (shape == nullptr) {
			return false;
		}
		shape->scale(vec);
		std::cout << "A " << shape->getName() << " sikidom atmeretezve a " << vec << " vektorral." << std::endl;
		return true;
	}

	// -------------------- CHECK CONTACTS --------------------

	CheckContacts::CheckContacts(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megkeresi es kiirja az erintkezo sikidomokat.";
	}

	bool CheckContacts::execute(glib::linebuffer& input) const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		cStyle contact_style = cStyle().fg(cStyle::GREEN);
		for (auto it1 = shapes.begin(); it1 != shapes.end(); ++it1) {
			AbstractShape* shape1 = *it1;
			auto it2 = it1; it2++;
			for (; it2 != shapes.end(); it2++) {
				AbstractShape* shape2 = *it2;
				CollSys::GJKSolver gjk_test(*shape1, *shape2);
				if (gjk_test.isContact()) {
					shape1->setColor(sf::Color::Red);
					shape2->setColor(sf::Color::Red);
					std::cout << contact_style << "Ez a ket objektum erintkezik: " <<
						shape1->getName() << "; " << shape2->getName() << cStyle::none << std::endl << std::endl;
				}
			}
		}
		return true;
	}

	// -------------------- SAVE --------------------

	SaveAs::SaveAs(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Elmenti a letrehozott sikidomokat.";
		this->params = "<file neve>";
	}

	bool SaveAs::execute(glib::linebuffer& input) const {
		glib::string file_path;
		input >> file_path;
		file_path += ".shps";

		if (!this->postInputCheck(input)) {
			return false;
		}
		bool exists = std::filesystem::exists(std::filesystem::path(file_path.c_str()));
		if (exists) {
			std::cout << cStyle::error << "A(z) \"" << file_path << "\" nevu file mar letezik." << cStyle::none << std::endl;
			return false;
		}
		std::ofstream file(file_path.c_str());
		for (auto shape : this->reciever.getShapeList()) {
			file << "new ";
			shape->write(file);
			file << std::endl;
		}
		return true;
	}

	// -------------------- OPENWIN --------------------

	Openwin::Openwin(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megnyit egy uj ablakot, ahol a sikidomok grafikusan abrazolva jelennek meg.";
	}

	bool Openwin::execute(glib::linebuffer& input) const {
		this->reciever.openWindow();
		return true;
	}

	// -------------------- EXIT --------------------

	Exit::Exit(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilep a programbol.";
	}

	bool Exit::execute(glib::linebuffer& input) const {
		this->reciever.stop();
		return true;
	}
}