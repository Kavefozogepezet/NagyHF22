#include "string.h"

#include "debug/memtrace.h"

namespace glib {
	string::string() : data(new char[1]{ 0 }) {}

	string::string(const char* str) {
		if (!str) {
			throw std::invalid_argument("Invalid argument. (const char *) str was nullptr.");
		}
		size_t size = strlen(str) + 1;
		data = new char[size];
		strcpy_s(this->data, size, str);
	}

	string::string(const string& other) {
		size_t size = other.length() + 1;
		this->data = new char[size];
		strcpy_s(this->data, size, other.data);
	}

	string::string(const std::string& stdstr) {
		size_t size = stdstr.length() + 1;
		this->data = new char[size];
		strcpy_s(this->data, size, stdstr.c_str());
	}

	string::operator std::string() const {
		return std::string(this->c_str());
	}

	string::~string() {
		delete[] this->data;
	}

	bool string::empty() const {
		return this->data[0] == 0;
	}

	string& string::operator = (const string& other) {
		size_t size = other.length() + 1;
		delete[] this->data;
		this->data = new char[size];
		strcpy_s(this->data, size, other.data);
		return *this;
	}

	char& string::operator [] (size_t idx) {
		const char& const_element = static_cast<const string*>(this)->operator[](idx);
		return const_cast<char&>(const_element);
	}

	const char& string::operator [] (size_t idx) const {
		if (idx > this->length()) {
			throw std::out_of_range("invalid string subscript.");
		}
		return this->data[idx];
	}

	string string::operator + (const string& other) const {
		size_t size = this->length() + other.length() + 1;
		char* str_ptr = new char[size];
		strcpy_s(str_ptr, size, this->data);
		strcat_s(str_ptr, size, other.data);
		string str(str_ptr);
		delete[] str_ptr;
		return str;
	}

	string string::operator + (const char* other) const {
		size_t size = this->length() + strlen(other) + 1;
		char* str_ptr = new char[size];
		strcpy_s(str_ptr, size, this->data);
		strcat_s(str_ptr, size, other);
		string str(str_ptr);
		delete[] str_ptr;
		return str;
	}

	string operator + (const char* str1, const string& str2) {
		size_t size = str2.length() + strlen(str1) + 1;
		char* str_ptr = new char[size];
		strcpy_s(str_ptr, size, str1);
		strcat_s(str_ptr, size, str2.data);
		string str(str_ptr);
		delete[] str_ptr;
		return str;
	}

	string string::operator + (char c) const {
		size_t size = this->length() + 2;
		char* str_ptr = new char[size];
		strcpy_s(str_ptr, size, this->data);
		str_ptr[size - 2] = c;
		str_ptr[size - 1] = 0;
		string str(str_ptr);
		delete[] str_ptr;
		return str;
	}

	string operator + (char c, const string& str2) {
		size_t size = str2.length() + 2;
		char* str_ptr = new char[size];
		str_ptr[0] = c;
		strcat_s(str_ptr + 1, size, str2.data);
		string str(str_ptr);
		delete[] str_ptr;
		return str;
	}

	string string::operator += (const string& other) {
		size_t size = this->length() + other.length() + 1;
		char* new_str = new char[size];
		strcpy_s(new_str, size, this->data);
		strcat_s(new_str, size, other.data);
		delete[] this->data;
		this->data = new_str;
		return *this;
	}

	string string::operator += (const char* other) {
		size_t size = this->length() + strlen(other) + 1;
		char* new_str = new char[size];
		strcpy_s(new_str, size, this->data);
		strcat_s(new_str, size, other);
		delete[] this->data;
		this->data = new_str;
		return *this;
	}

	string string::operator += (const char c) {
		size_t size = this->length() + 2;
		char* new_str = new char[size];
		strcpy_s(new_str, size, this->data);
		new_str[size - 2] = c;
		new_str[size - 1] = 0;
		delete[] this->data;
		this->data = new_str;
		return *this;
	}

	bool string::operator == (string other) const {
		return strcmp(this->data, other.data) == 0;
	}

	bool string::operator == (const char* other) const {
		return strcmp(this->data, other) == 0;
	}

	bool operator == (const char* str1, const string& str2) {
		return strcmp(str1, str2.data) == 0;
	}

	bool string::operator != (string other) const {
		return !(*this == other);
	}

	bool string::operator != (const char* other) const {
		return !(*this == other);
	}

	bool operator != (const char* str1, const string& str2) {
		return !(str1 == str2);
	}

	std::istream& operator >> (std::istream& stream, string& str) {
		if (stream) {
			unsigned char temp;
			bool ws_found = false;
			bool changed = false;
			std::istream::iostate state = stream.goodbit;
			str.clear();

			auto flags = stream.flags();
			stream.setf(std::ios_base::skipws);

			while (stream >> temp && !ws_found) {
				stream.unsetf(std::ios_base::skipws);
				if (isspace(temp)) {
					stream.putback(temp);
					ws_found = true;
				}
				else {
					str += temp;
					changed = true;
				}
			}
			if (stream.eof()) { state |= stream.eofbit; }
			if (!changed) { state |= stream.failbit; }
			stream.clear(state);
			stream.setf(flags);
		}
		return stream;
	}

	std::ostream& operator << (std::ostream& stream, const string& str) {
		return stream << str.data;
	}

	const char* string::c_str() const { return this->data; }

	inline size_t string::length() const { return strlen(this->data); }

	void string::clear() {
		delete[] this->data;
		this->data = new char[1]{ 0 };
	}
}

namespace std {
	istream& getline(istream& stream, glib::string& str) {
		unsigned char temp;
		bool changed = false;
		auto state = stream.rdstate();
		str.clear();

		auto flags = stream.flags();
		stream.unsetf(std::ios_base::skipws);

		while (stream >> temp && temp != '\n') {
			str += temp;
			changed = true;
		}
		if (stream.eof()) { state |= stream.eofbit; }
		if (!changed) { state |= stream.failbit; }
		stream.clear(state);
		stream.setf(flags);
		return stream;
	}
}
