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

	bool Help::execute(std::stringstream& input) const {
		cStyle list_entry = cStyle().fg(cStyle::BLACK).bg(cStyle::LIGHT_GRAY);
		for (auto c : this->reciever.getCmdReg()) {
			list_entry() << ' ' << c.first << ' ' << cStyle::nostyle << ':' << std::endl <<
				*c.second << std::endl;
		}
		return true;
	}

	// -------------------- LIST SHAPE TYPES --------------------

	ListShapeTypes::ListShapeTypes(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a peldanyosithato sikidom tipusokat.";
	}

	bool ListShapeTypes::execute(std::stringstream& input) const {
		Sandbox::ShapeReg& shape_reg = this->reciever.getShapeReg();
		for (auto& sr : shape_reg) {
			cStyle::basic() << sr.first << std::endl;
		}
		return true;
	}

	// -------------------- LIST SHAPES --------------------

	ListShapes::ListShapes(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilistazza a letrehozott sikidomokat.";
	}

	bool ListShapes::execute(std::stringstream& input) const {
		Sandbox::ShapeList& shape_reg = this->reciever.getShapeList();
		for (auto shape : shape_reg) {
			cStyle::basic() << shape->getName() << " (" << shape->getType() << ") " << std::endl;
		}
		return true;
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

	bool Create::execute(std::stringstream& input) const {
		glib::string shape_key, shape_name;
		Sandbox::ShapeReg& sreg = this->reciever.getShapeReg();

		input >> shape_key;
		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->createShape(shape_key);
		if (!shape) { return false; }
		if (!shape->fromConsole(input)) {
			cStyle::error() << "Hiba a sikidom beolvasasakor." << cStyle::endl;
			delete shape;
			return false;
		}
		if (this->validateShape(shape)) {
			this->reciever.getShapeList().push_back(shape);
			cStyle::basic() << " A \"" << shape->getName() << "\" nevu " << shape_key << " tipusu sikidom elkeszult." << std::endl;
			return true;
		}
		else {
			return false;
		}
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

	bool Destroy::execute(std::stringstream& input) const {
		glib::string name;
		input >> name;
		
		if (!this->postInputCheck(input)) {
			return false;
		}
		Sandbox::ShapeList& shapes = this->reciever.getShapeList();
		for (auto it = shapes.begin(); it != shapes.end(); it++) {
			if ((*it)->getName() == name) {
				delete *it;
				shapes.erase(it);
				cStyle::basic() << "A(z) \"" << name << "\" nevu sikidom torolve." << std::endl;
				return true;
			}
		}
		cStyle::error() << "Nincs \"" << name << "\" nevu sikidom." << cStyle::endl;
		return false;
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

	bool Move::execute(std::stringstream& input) const {
		glib::vec2d vec;
		glib::string name;
		input >> name >> vec;

		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->getShape(name);
		if (shape == nullptr) {
			return false;
		}
		if (!vec.x && !vec.y) {
			cStyle::warn() << "A mozgatas vektora { 0, 0 }. Lehetseges, hogy helytelenul adta meg a komponenseket." << cStyle::endl;
			return false;
		}
		shape->move(vec);
		cStyle::basic() << "A " << shape->getName() << " sikidom elmozgatva a " << vec << " vektorral." << std::endl;
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

	bool Rotate::execute(std::stringstream& input) const {
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
			cStyle::warn() << "A forgatas szoge 0 fok. Lehetseges, hogy helytelenul adta meg a szoget." << cStyle::endl;
			return false;
		}
		shape->rotate(angle);
		cStyle::basic() << "A " << shape->getName() << " sikidom elforgatva " << angle << " fokkal." << std::endl;
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

	bool Scale::execute(std::stringstream& input) const {
		glib::string name;
		glib::vec2d vec;
		input >> name >> vec;

		if (!this->postInputCheck(input)) {
			return false;
		}
		AbstractShape* shape = this->getShape(name);
		if (shape == nullptr) {
			return false;
		}
		shape->scale(vec);
		cStyle::basic() << "A " << shape->getName() << " sikidom atmeretezve a " << vec << " vektorral." << std::endl;
		return true;
	}

	// -------------------- CHECK CONTACTS --------------------

	CheckContacts::CheckContacts(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Megkeresi es kiirja az erintkezo sikidomokat.";
	}

	bool CheckContacts::execute(std::stringstream& input) const {
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
					contact_style() << "Ez a ket objektum erintkezik: " <<
						shape1->getName() << "; " << shape2->getName() << cStyle::endl;
				}
			}
		}
		return true;
	}

	// -------------------- SAVE AS --------------------

	SaveAs::SaveAs(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "File-ba menti a letrehozott sikidomokat.";
		this->params = "<file neve>";
	}

	bool SaveAs::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;
		file_path += file_ext;
		unsigned int scount = 0;

		if (!this->postInputCheck(input)) {
			return false;
		}
		bool exists = std::filesystem::exists(std::filesystem::path(file_path.c_str()));
		if (exists) {
			cStyle::error() << "A(z) \"" << file_path << "\" nevu file mar letezik." << cStyle::endl;
			return false;
		}
		std::ofstream file(file_path);
		for (auto shape : this->reciever.getShapeList()) {
			file << "new " << shape->getType() << ' ' << *shape << std::endl;
			scount++;
		}
		cStyle::basic() << scount << " sikidom elmentve a " << file_path << " fajlba." << std::endl;
		return true;
	}

	// -------------------- SAVE --------------------
	
	Save::Save(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "File-ba menti a valtozasokat.";
		this->params = "<file neve>";
	}

	bool Save::execute(std::stringstream& input) const {
		const glib::string& file_path = this->reciever.getMyFile();
		if (file_path == "") {
			cStyle::error() << "Mentsen a saveas parancsal" << cStyle::endl;
			return false;
		}
		std::ofstream file(file_path);
		for (auto shape : this->reciever.getShapeList()) {
			file << "new " << shape->getType() << ' ' << *shape << std::endl;
		}
		cStyle::basic() << "Sikidomok elmentve." << cStyle::endl;
		return true;
	}
	
	// -------------------- MERGE --------------------

	Merge::Merge(Sandbox& sandbox) :
		CreatorCommand(sandbox)
	{
		this->desc = "Beolvasa a megadott fajlban tarolt sikidomokat. (.shps formatumu fajlbol).";
		this->params = "<file neve>";
	}

	bool Merge::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;

		if (!this->postInputCheck(input)) {
			return false;
		}
		std::ifstream file;
		if (this->openFile(file_path, file)) {
			this->readShapes(file);
			return true;
		}
		return false;
	}

	void Merge::readShapes(std::ifstream& file) const {
		glib::string temp, shape_key;
		unsigned int shape_count = 0, loaded_count = 0;
		while (file >> temp) {
			if (temp == "new") {
				file >> shape_key;
				AbstractShape* shape = this->createShape(shape_key);
				if (!shape) {
					break;
				}
				file >> *shape;
				if (this->validateShape(shape)) {
					this->reciever.getShapeList().push_back(shape);
					cStyle::basic() << "Beolvasva: " << shape->getName() << " (" << shape->getType() << ")" << std::endl;
					loaded_count++;
				}
				else {
					delete shape;
				}
				shape_count++;
			}
			else if (!temp.empty()) {
				cStyle::warn() << "Ervenytelen token: " << temp << cStyle::basic << std::endl;
			}
		}
		if (file.rdstate() == std::iostream::failbit) {
			cStyle::error() << "Hiba a beolvasas soran." << cStyle::endl;
		}
		cStyle::basic() << "Sikeresen beolvasva " << loaded_count << '/' << shape_count << " sikidom." << std::endl;
	}

	bool Merge::openFile(glib::string file_path, std::ifstream& stream) const {
		glib::string extension = glib::string(file_path.c_str() + file_path.length() - 5);
		if (extension != file_ext) {
			cStyle::error() << "A file kiterjesztese (" << extension << ") nem felismerheto." << cStyle::endl;
			return false;
		}
		bool exists = std::filesystem::exists(std::filesystem::path(file_path.c_str()));
		if (!exists) {
			cStyle::error() << "A(z) " << file_path << " file nem letezik." << cStyle::endl;
			return false;
		}
		stream.open(file_path);
		if (!stream.is_open()) {
			cStyle::error() << "A(z) " << file_path << " file-t nem lehet megnyitni." << cStyle::endl;
			return false;
		}
		return true;
	}

	// -------------------- LOAD --------------------

	Load::Load(Sandbox& sandbox) :
		Merge(sandbox)
	{
		this->desc = "Betolt egy file-t (.shps formatumu).";
		this->params = "<file neve>";
	}

	bool Load::execute(std::stringstream& input) const {
		glib::string file_path;
		input >> file_path;

		if (!this->postInputCheck(input)) {
			return false;
		}
		std::ifstream file;
		if (this->openFile(file_path, file)) {
			this->deleteExistingShapes();
			this->readShapes(file);
			this->reciever.setMyFile(file_path);
			return true;
		}
		return false;
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

	bool Openwin::execute(std::stringstream& input) const {
		this->reciever.openWindow();
		cStyle::basic() << "Amig az ablak nyitva van, ide nem tud parancsot beirni." << std::endl;
		return true;
	}

	// -------------------- EXIT --------------------

	Exit::Exit(Sandbox& sandbox) :
		Command(sandbox)
	{
		this->desc = "Kilep a programbol.";
	}

	bool Exit::execute(std::stringstream& input) const {
		this->reciever.stop();
		return true;
	}
}