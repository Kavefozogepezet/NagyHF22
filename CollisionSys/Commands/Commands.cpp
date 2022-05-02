#include "Commands.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "debug/memtrace.h"

#include "Sandbox.h"
#include "graphics/consoleStyle.h"
#include "GJK.h"

namespace CollSys::Commands {
	using cStyle = consoleStyle;

	static const glib::string file_ext = ".shps";

	// -------------------- HELP --------------------

	Help::Help(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a vegrehajthato parancsokat.";
	}

	void Help::execute(std::stringstream& input) const {
		cStyle list_entry = cStyle().fg(cStyle::BLACK).bg(cStyle::LIGHT_GRAY);
		// Kilistázzuk az összes parancs leírását
		for (auto c : this->reciever.getCmdReg()) {
			list_entry() << ' ' << c.first << ' ' << cStyle::nostyle << ':' << std::endl <<
				*c.second << std::endl;
		}
	}

	// -------------------- LIST SHAPE TYPES --------------------

	ListShapeTypes::ListShapeTypes(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a peldanyosithato sikidom tipusokat.";
	}

	void ListShapeTypes::execute(std::stringstream& input) const {
		Sandbox::ShapeReg& shape_reg = this->reciever.getShapeReg();
		for (auto& sr : shape_reg) {
			cStyle::basic() << "\t- " << sr.first << std::endl;
		}
	}

	// -------------------- LIST SHAPES --------------------

	ListShapes::ListShapes(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a letrehozott sikidomokat.";
	}

	void ListShapes::execute(std::stringstream& input) const {
		Sandbox::ShapeList& shape_reg = this->reciever.getShapeList();
		for (auto shape : shape_reg) {
			cStyle::basic() << "\t- " << shape->getName() << " (" << shape->getType() << ") " << std::endl;
		}
	}

	// -------------------- CREATE --------------------

	Create::Create(Sandbox& sandbox) :
		CreatorCommand(sandbox)
	{
		this->desc =
			"Letrehoz egy sikidomot.\n"
			"A create parancs utan be kell irni milyen sikidomot szeretnenk, "
			"majd a sikidom parametereit kell megadni.";
		this->params =
			"<sikidom tipusa> <sikidom neve> <egyeb parameterek>";
	}

	void Create::execute(std::stringstream& input) const {
		glib::string shape_key, shape_name;
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();

		input >> shape_key;
		this->postInputCheck(input);

		ConvexShape* shape = this->createShape(shape_key);

		try { shape->fromConsole(input); }
		catch (Error err) { // nem sikerült beolvasni a síkidomot
			delete shape;
			throw err;
		}

		this->validateShape(shape);
		this->reciever.getShapeList().push_back(shape);
		cStyle::success() << " A \"" << shape->getName() << "\" nevu " << shape_key << " tipusu sikidom elkeszult." << cStyle::endl;
	}

	// -------------------- CREATE --------------------

	Destroy::Destroy(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc =
			"Torli a megadott nevu sikidomot.\n"
			"A destroy parancs utan irja a torolni kivant sikidom nevet.";
		this->params =
			"<sikidom neve>";
	}

	void Destroy::execute(std::stringstream& input) const {
		glib::string name;
		input >> name;
		this->postInputCheck(input);

		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto it = shapes.begin(); it != shapes.end(); it++) {
			if ((*it)->getName() == name) {
				delete *it;
				shapes.erase(it);
				cStyle::success() << "A(z) \"" << name << "\" nevu sikidom torolve." << cStyle::endl;
				return;
			}
		}
		throw Error("Nincs \"" + name + "\" nevu sikidom.");
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

	void Move::execute(std::stringstream& input) const {
		glib::vec2d vec;
		glib::string name;
		input >> name >> vec;
		this->postInputCheck(input);

		ConvexShape* shape = this->getShape(name);
		shape->move(vec);
		cStyle::success() << "A " << shape->getName() << " sikidom elmozgatva a " << vec << " vektorral." << cStyle::endl;
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

	void Rotate::execute(std::stringstream& input) const {
		glib::string name;
		double angle;
		input >> name >> angle;
		this->postInputCheck(input);

		ConvexShape* shape = this->getShape(name);
		shape->rotate(angle);
		cStyle::success() << "A " << shape->getName() << " sikidom elforgatva " << angle << " fokkal." << cStyle::endl;
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

	void Scale::execute(std::stringstream& input) const {
		glib::string name;
		glib::vec2d vec;
		input >> name >> vec;
		this->postInputCheck(input);

		ConvexShape* shape = this->getShape(name);
		shape->scale(vec);
		cStyle::success() << "A " << shape->getName() << " sikidom atmeretezve a " << vec << " vektorral." << cStyle::endl;
	}

	// -------------------- CHECK CONTACTS --------------------

	CheckContacts::CheckContacts(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megkeresi es kiirja az erintkezo sikidomokat.";
	}

	void CheckContacts::execute(std::stringstream& input) const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto it1 = shapes.begin(); it1 != shapes.end(); ++it1) {
			ConvexShape* shape1 = *it1;
			auto it2 = it1; it2++;
			for (; it2 != shapes.end(); it2++) {
				ConvexShape* shape2 = *it2;
				GJKSolver gjk_test(*shape1, *shape2);
				if (gjk_test.isContact()) {
					shape1->setColor(sf::Color::Red);
					shape2->setColor(sf::Color::Red);
					cStyle::success() << "Ez a ket objektum erintkezik: " <<
						shape1->getName() << "; " << shape2->getName() << cStyle::endl;
				}
			}
		}
	}

	// -------------------- CHECK CONTACT --------------------

	CheckContact::CheckContact(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kiirja, hogy a megadott ket sikidom erintkezik e.";
		this->params = "<egyik sikidom neve> <masik sikidom neve>";
	}

	void CheckContact::execute(std::stringstream& input) const {
		glib::string sname1, sname2;
		input >> sname1 >> sname2;
		this->postInputCheck(input);

		if (sname1 == sname2) {
			throw Error("Ket kulombozo sikidomot adjon meg.");
		}

		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		ConvexShape* s1 = nullptr, * s2 = nullptr;
		for (auto shape : shapes) {
			if (shape->getName() == sname1) {
				s1 = shape;
			}
			else if (shape->getName() == sname2) {
				s2 = shape;
			}
		}

		if (!(s1 && s2)) {
			throw Error("Letezo sikidomokat adjon meg.");
		}

		GJKSolver solver(*s1, *s2);
		glib::string result = solver.isContact() ? " " : " nem ";
		cStyle::success() << "A ket sikidom" << result << "erintkezik." << cStyle::endl;
	}

	// -------------------- SAVE AS --------------------

	SaveAs::SaveAs(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "File-ba menti a letrehozott sikidomokat.";
		this->params = "<file neve>";
	}

	void SaveAs::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;
		file_path += file_ext;
		unsigned int scount = 0;

		this->postInputCheck(input);

		bool exists = std::filesystem::exists(std::filesystem::path(file_path.c_str()));
		if (exists) {
			throw Error("A(z) \"" + file_path + "\" nevu file mar letezik.");
		}
		std::ofstream file(file_path);
		for (auto shape : this->reciever.getShapeList()) {
			file << "new " << shape->getType() << ' ' << *shape << std::endl;
			scount++;
		}
		cStyle::success() << scount << " sikidom elmentve a " << file_path << " fajlba." << cStyle::endl;
		this->reciever.setMyFile(file_path);
	}

	// -------------------- SAVE --------------------
	
	Save::Save(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "File-ba menti a valtozasokat.";
		this->params = "<file neve>";
	}

	void Save::execute(std::stringstream& input) const {
		const glib::string& file_path = this->reciever.getMyFile();
		if (file_path == "") {
			throw Error("Mentsen a saveas parancsal");
		}
		std::ofstream file(file_path);
		for (auto shape : this->reciever.getShapeList()) {
			file << "new " << shape->getType() << ' ' << *shape << std::endl;
		}
		cStyle::success() << "Sikidomok elmentve." << cStyle::endl;
	}
	
	// -------------------- MERGE --------------------

	Merge::Merge(Sandbox& sandbox) :
		CreatorCommand(sandbox)
	{
		this->desc = "Beolvasa a megadott fajlban tarolt sikidomokat. (.shps formatumu fajlbol).";
		this->params = "<file neve>";
	}

	void Merge::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;
		this->postInputCheck(input);

		std::ifstream file = this->openFile(file_path);
		this->readShapes(file);
	}

	void Merge::readShapes(std::ifstream& file) const {
		glib::string temp, shape_key;
		unsigned int
			shape_count = 0, // összes síkidom száma, egyenlő a "new" tokenek számával a fájlban
			loaded_count = 0; // sikeresen beolvasott síkidomok száma

		// az első "new" tokenig elvetünk mindent
		while (file >> temp && temp != "new");

		while (file) {
			shape_count++;
			// a következő "new" tokenig egy beolvassuk a paramétereket egy bufferbe
			std::stringstream buf;
			while (file >> temp && temp != "new") {
				buf << temp << ' ';
			}
			// a bufferből kiolvassuk a síkidomot
			buf >> shape_key;
			ConvexShape* shape;
			try { shape = this->createShape(shape_key); }
			catch (Error err) { err.print(); continue; } // nem létezik a típus

			if (buf >> *shape) { // folytatjuk, ha sikerül beolvasni
				try {
					this->validateShape(shape);
					this->reciever.getShapeList().push_back(shape);
					cStyle::success() << "Beolvasva: " << shape->getName() << " (" << shape->getType() << ")" << cStyle::endl;
					loaded_count++;
				}
				catch (Error err) {
					err.print();
				}
			}
			else {
				delete shape;
			}
		}
		cStyle::basic() << "Sikeresen beolvasva " << loaded_count << '/' << shape_count << " sikidom." << cStyle::endl;
	}

	std::ifstream Merge::openFile(glib::string file_path) const {
		std::ifstream stream;
		glib::string extension = glib::string(file_path.c_str() + file_path.length() - 5);
		if (extension != file_ext) {
			throw Error("A file kiterjesztese (" + extension + ") nem felismerheto.");
		}
		bool exists = std::filesystem::exists(std::filesystem::path(file_path.c_str()));
		if (!exists) {
			throw Error("A(z) " + file_path + " file nem letezik.");
		}
		stream.open(file_path);
		if (!stream.is_open()) {
			throw Error("A(z) " + file_path + " file-t nem lehet megnyitni.");
		}
		return stream;
	}

	// -------------------- LOAD --------------------

	Load::Load(Sandbox& sandbox) :
		Merge(sandbox)
	{
		this->desc = "Betolt egy file-t (.shps formatumu).";
		this->params = "<file neve>";
	}

	void Load::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;
		this->postInputCheck(input);

		std::ifstream file = this->openFile(file_path);
		this->deleteExistingShapes();
		this->readShapes(file);
		this->reciever.setMyFile(file_path);
	}

	void Load::deleteExistingShapes() const {
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto shape : shapes) {
			delete shape;
		}
		shapes.clear();
	}

	// -------------------- OPENWIN --------------------

	Openwin::Openwin(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megnyit egy uj ablakot, ahol a sikidomok grafikusan abrazolva jelennek meg.";
	}

	void Openwin::execute(std::stringstream& input) const {
		this->reciever.openWindow();
		cStyle::warn() << "Amig az ablak nyitva van, ide nem tud parancsot beirni." << cStyle::endl;
	}

	// -------------------- EXIT --------------------

	Exit::Exit(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilep a programbol.";
	}

	void Exit::execute(std::stringstream& input) const {
		this->reciever.stop();
	}
}