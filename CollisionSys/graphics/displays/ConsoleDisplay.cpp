#include "ConsoleDisplay.h"

#include <iostream>

#include "Sandbox.h"
#include "general/string.h"
#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	void ConsoleDisplay::onUpdate() {
		std::cout << ">>> ";

		glib::string line;
		std::getline(std::cin, line);
		std::stringstream lbuff(line);

		glib::string cmd;
		if (!(lbuff >> cmd)) {
			cStyle::warn() << "Nem adott meg parancsot. A parancsok listajat megtekintheti a help parancsal." << cStyle::endl;
			return;
		}

		bool cmd_found = false;

		auto it = this->_parent.getCmdReg().get(cmd);
		if (it != this->_parent.getCmdReg().end()) {
			try {
				it->second->execute(lbuff);
			}
			catch (Commands::Error err) { // hiba volt a parancs vegrehajtasa soran
				err.print();
			}

			glib::string str;
			if (lbuff >> str) {
				cStyle::warn() << "A parancs nem olvasta vegig a sort, ezek a parameterek elvesztek :";
				do {
					std::cout << " " << str;
				} while (lbuff >> str);
				cStyle::endl(std::cout);
			}
		}
		else {
			cStyle::error() << "A \"" << cmd <<
				"\" parancs nem letezik. A parancsok listajat megtekintheti a help parancsal." << cStyle::endl;
		}
		std::endl(std::cout);
	}
}
