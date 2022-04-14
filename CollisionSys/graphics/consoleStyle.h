#pragma once

#include <iostream>

#include "general/string.h"

namespace CollSys {
	class consoleStyle
	{
	public:
		static consoleStyle basic, warn, error;

		static std::ostream& writeWith(const consoleStyle& style);

		static std::ostream& __CLRCALL_OR_CDECL endl(std::ostream& stream) {
			stream << consoleStyle::basic;
			stream << std::endl;
			return stream;
		}

		static std::ostream& __CLRCALL_OR_CDECL nostyle(std::ostream& stream) {
			stream << consoleStyle::basic;
			return stream;
		}
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
		consoleStyle(glib::string args);
		consoleStyle(const consoleStyle& other);

		consoleStyle& add(glib::string arg);

		consoleStyle& fg(ColorCode color);
		consoleStyle& fg(color8b color);
		consoleStyle& fg(color8b r, color8b g, color8b b);

		consoleStyle& bg(ColorCode color);
		consoleStyle& bg(color8b color);
		consoleStyle& bg(color8b r, color8b g, color8b b);

		std::ostream&  operator () () const;

		friend std::ostream& operator << (std::ostream& stream, const consoleStyle& cs);
	private:
		glib::string style;
	};
}
