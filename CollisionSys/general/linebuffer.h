#pragma once

#include "string.h"

namespace glib {
	class linebuffer
	{
	public:
		linebuffer();

		bool eol() const;

		operator bool() const;

		linebuffer& get(char& c);

		friend std::istream& operator >> (std::istream& stream, linebuffer& buff);
		friend std::ostream& operator << (std::ostream& stream, linebuffer& buff);

		friend linebuffer& operator >> (linebuffer& buff, string& str);
		friend linebuffer& operator >> (linebuffer& buff, char& c);
		friend linebuffer& operator >> (linebuffer& buff, unsigned char& uc);
		friend linebuffer& operator >> (linebuffer& buff, int& i);
		friend linebuffer& operator >> (linebuffer& buff, unsigned int& ui);
		friend linebuffer& operator >> (linebuffer& buff, short int& si);
		friend linebuffer& operator >> (linebuffer& buff, unsigned short int& usi);
		friend linebuffer& operator >> (linebuffer& buff, long int& li);
		friend linebuffer& operator >> (linebuffer& buff, unsigned long int& uli);
		friend linebuffer& operator >> (linebuffer& buff, long int& ll);
		friend linebuffer& operator >> (linebuffer& buff, unsigned long long& ull);
		friend linebuffer& operator >> (linebuffer& buff, float& f);
		friend linebuffer& operator >> (linebuffer& buff, double& d);
		friend linebuffer& operator >> (linebuffer& buff, long double& ld);
	private:
		bool end_of_line;

		size_t line_idx;
		glib::string line;
	};
}
