#pragma once

#include <iostream>

#include "string.h"

namespace glib {
	class consoleStyle
	{
	public:
		static consoleStyle none, warn, error;
	public:
		enum ColorCode {
			BLACK = 30,
			RED,
			GREEN,
			YELLOW,
			BLUE,
			MAGENTA,
			CYAN,
			LIGHT_GRAY,
			GRAY = 90,
			BRIGHT_RED,
			BRIGHT_GREEN,
			BRIGHT_YELLOW,
			BRIGHT_BLUE,
			BRIGHT_MAGENTA,
			BRIGHT_CYAN,
			WHITE,
		};

		typedef unsigned char color8b;
	public:
		consoleStyle();
		consoleStyle(string args);
		consoleStyle(const consoleStyle& other);

		consoleStyle& add(string arg);

		consoleStyle& fg(ColorCode color);
		consoleStyle& fg(color8b color);
		consoleStyle& fg(color8b r, color8b g, color8b b);

		consoleStyle& bg(ColorCode color);
		consoleStyle& bg(color8b color);
		consoleStyle& bg(color8b r, color8b g, color8b b);

		friend std::ostream& operator << (std::ostream& stream, const consoleStyle& cs);
	private:
		string style;
	};
}
