#include "consoleStyle.h"

namespace glib {
	consoleStyle
		consoleStyle::none = consoleStyle("0"),
		consoleStyle::warn = consoleStyle("93"),
		consoleStyle::error = consoleStyle("91");

	consoleStyle::consoleStyle() : style() {}

	consoleStyle::consoleStyle(string args) : style(args) {}

	consoleStyle::consoleStyle(const consoleStyle& other) : style(other.style) {}

	consoleStyle& consoleStyle::add(string arg) {
		if (this->style.length() > 0) {
			this->style += ";";
		}
		this->style += arg;
		return *this;
	}

	consoleStyle& consoleStyle::fg(ColorCode color) {
		char col_str[4] { 0 };
		_itoa_s(static_cast<int>(color), col_str, 4, 10);
		return this->add(col_str);
	}

	consoleStyle& consoleStyle::fg(color8b color) {
		char col_str[4]{ 0 };
		_itoa_s(color, col_str, 4, 10);
		return this->add("38").add("5").add(col_str);
	}

	consoleStyle& consoleStyle::fg(color8b r, color8b g, color8b b) {
		char col_str[3][4];
		_itoa_s(r, col_str[0], 4, 10);
		_itoa_s(g, col_str[1], 4, 10);
		_itoa_s(b, col_str[2], 4, 10);
		return this->add("38").add("2").add(col_str[0]).add(col_str[1]).add(col_str[2]);
	}

	consoleStyle& consoleStyle::bg(ColorCode color) {
		int temp = static_cast<int>(color) + 10;
		char col_str[4]{ 0 };
		_itoa_s(temp, col_str, 4, 10);
		return this->add(col_str);
	}

	consoleStyle& consoleStyle::bg(color8b color) {
		char col_str[4]{ 0 };
		_itoa_s(color, col_str, 4, 10);
		return this->add("48").add("5").add(col_str);
	}

	consoleStyle& consoleStyle::bg(color8b r, color8b g, color8b b) {
		char col_str[3][4];
		_itoa_s(r, col_str[0], 4, 10);
		_itoa_s(g, col_str[1], 4, 10);
		_itoa_s(b, col_str[2], 4, 10);
		return this->add("48").add("2").add(col_str[0]).add(col_str[1]).add(col_str[2]);
	}

	std::ostream& operator<<(std::ostream& stream, const consoleStyle& cs) {
		stream << "\033[" + cs.style + "m";
		return stream;
	}

}
