#include "Text.h"

#include "graphics/consoleStyle.h"

#ifndef CPORTA
namespace CollSys {
	bool Text::font_init = false;
	sf::Font Text::font = sf::Font();

	Text::Text() :
		sf::Text()
	{
		this->fontInit();
		this->setFont(font);
		this->setScale(0.002f, 0.002f);
	}

	Text::Text(const glib::string& str) :
		sf::Text()
	{
		this->fontInit();
		this->setFont(font);
		this->setString(str.c_str());
		this->setScale(0.002f, 0.002f);
	}

	void Text::fontInit() {
		if (!font_init) {
			bool res = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
			if (!res) {
				consoleStyle::error() << "Font olvasasi hiba" << consoleStyle::endl;
			}
			font_init = true;
		}
	}
}
#endif
