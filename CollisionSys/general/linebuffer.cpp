#include "linebuffer.h"

#include "../debug/memtrace.h"

namespace glib {
	glib::linebuffer::linebuffer() :
		end_of_line(true),
		line_idx(0),
		line()
	{}

	bool glib::linebuffer::eol() const { return this->end_of_line; }

	glib::linebuffer::operator bool() const { return !this->end_of_line; }

	linebuffer& linebuffer::get(char& c) {
		if (this->end_of_line) {
			return *this;
		}
		char temp = this->line[this->line_idx++];
		if (temp == 0) {
			this->end_of_line = true;
			return *this;
		}
		c = temp;
		return *this;
	}

	std::istream& operator >> (std::istream& stream, linebuffer& buff)
	{
		char temp;
		bool neol = true;
		buff.line.clear();
		buff.line_idx = 0;
		buff.end_of_line = false;

		while (neol) {
			if (stream.get(temp).eof()) {
				neol = false;
			}
			else {
				if (temp == '\n') {
					neol = false;
				}
				buff.line += temp;
			}
		}
		return stream;
	}

	std::ostream& operator << (std::ostream& stream, linebuffer& buff) {
		stream << buff.line;
		return stream;
	}

	linebuffer& operator >> (linebuffer& buff, string& str) {
		char temp;
		bool is_eol = buff.eol();
		str.clear();

		do {
			is_eol = buff.get(temp).eol();
		} while (!is_eol && std::isspace(temp));

		while (!is_eol && !std::isspace(temp)) {
			str += temp;
			is_eol = buff.get(temp).eol();
		}
		return buff;
	}

	linebuffer& glib::operator >> (linebuffer& buff, char& c) {
		do {
			buff.get(c);
		} while (buff && isspace(c));
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, int& i) {
		string temp;
		if (buff >> temp) {
			i = atoi(temp.c_str());
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, long long& ll) {
		string temp;
		if (buff >> temp) {
			ll = atoll(temp.c_str());
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, float& f) {
		string temp;
		if (buff >> temp) {
			f = atof(temp.c_str());
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, unsigned char& uc) {
		char temp;
		buff >> temp;
		uc = static_cast<unsigned char>(temp);
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, unsigned int& ui) {
		long long temp;
		if (buff >> temp) {
			ui = static_cast<unsigned int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, short int& si) {
		int temp;
		if (buff >> temp) {
			si = static_cast<short int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, unsigned short int& usi) {
		int temp;
		if (buff >> temp) {
			usi = static_cast<unsigned short int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, long int& li) {
		long long temp;
		if (buff >> temp) {
			li = static_cast<long int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, unsigned long long& ull) {
		long long temp;
		if (buff >> temp) {
			ull = static_cast<unsigned long int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, unsigned long int& uli) {
		long long temp;
		if (buff >> temp) {
			uli = static_cast<unsigned long int>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, double& d) {
		float temp;
		if (buff >> temp) {
			d = static_cast<double>(temp);
		}
		return buff;
	}

	linebuffer& operator >> (linebuffer& buff, long double& ld) {
		float temp;
		if (buff >> temp) {
			ld = static_cast<long double>(temp);
		}
		return buff;
	}
}