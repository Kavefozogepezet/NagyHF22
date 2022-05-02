#pragma once

#include "SFML/Graphics.hpp"

#include "general/string.h"

namespace CollSys {
	class Text : public sf::Text
	{
	public:
		Text();
		Text(const glib::string& str);
	private:
		static bool font_init;
		static sf::Font font;

		static void fontInit();
	};
}
