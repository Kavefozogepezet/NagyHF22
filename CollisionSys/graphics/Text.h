#pragma once

#include "sfmlinclude.h"

#include "general/string.h"

#ifndef CPORTA
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
#endif // !CPORTA
