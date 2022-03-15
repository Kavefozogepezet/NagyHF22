#pragma once

#include <string.h>
#include <stdexcept>
#include <cctype>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

namespace glib {
	class string {
	public:
		string() : 
			data(new char[1] { 0 })
		{}

		string(const char* str) {
			if (!str) {
				throw std::invalid_argument("Invalid argument. (const char *) str was nullptr.");
			}
			size_t size = strlen(str) + 1;
			data = new char[size];
			strcpy_s(this->data, size, str);
		}

		string(const string& other):
			data(nullptr)
		{
			size_t size = other.length() + 1;
			this->data = new char[size];
			strcpy_s(this->data, size, other.data);
		}

		~string() {
			delete[] this->data;
		}

		string& operator = (const string& other) {
			size_t size = other.length() + 1;
			delete[] this->data;
			this->data = new char[size];
			strcpy_s(this->data, size, other.data);
			return *this;
		}

		char& operator [] (size_t idx) {
			const char& const_element = static_cast<const string*>(this)->operator[](idx);
			return const_cast<char&>(const_element);
		}

		const char& operator [] (size_t idx) const {
			if (idx >= this->length()) {
				throw std::out_of_range("invalid string subscript.");
			}
			return this->data[idx];
		}

		string operator + (const string& other) const {
			string str;
			size_t size = this->length() + other.length() + 1;
			str.data = new char[size];
			strcpy_s(str.data, size, this->data);
			strcat_s(str.data, size, other.data);
			return str;
		}

		string operator + (const char* other) const {
			string str;
			size_t size = this->length() + strlen(other) + 1;
			str.data = new char[size];
			strcpy_s(str.data, size, this->data);
			strcat_s(str.data, size, other);
			return str;
		}

		friend string operator + (const char* str1, const string& str2) {
			string str;
			size_t size = str2.length() + strlen(str1) + 1;
			str.data = new char[size];
			strcpy_s(str.data, size, str1);
			strcat_s(str.data, size, str2.data);
			return str;
		}

		string operator + (char c) const {
			string str;
			size_t size = this->length() + 2;
			str.data = new char[size];
			strcpy_s(str.data, size, this->data);
			str.data[size - 2] = c;
			str.data[size - 1] = 0;
			return str;
		}

		friend string operator + (char c, const string& str2) {
			string str;
			size_t size = str2.length() + 2;
			str.data = new char[size];
			str.data[0] = c;
			str.data[1] = 0;
			strcat_s(str.data, size, str2.data);
			return str;
		}

		string operator += (const string& other) {
			size_t size = this->length() + other.length() + 1;
			char* new_str = new char[size];
			strcpy_s(new_str, size, this->data);
			strcat_s(new_str, size, other.data);
			delete[] this->data;
			this->data = new_str;
			return *this;
		}

		string operator += (const char* other) {
			size_t size = this->length() + strlen(other) + 1;
			char* new_str = new char[size];
			strcpy_s(new_str, size, this->data);
			strcat_s(new_str, size, other);
			delete[] this->data;
			this->data = new_str;
			return *this;
		}

		string operator += (const char c) {
			size_t size = this->length() + 2;
			char* new_str = new char[size];
			strcpy_s(new_str, size, this->data);
			new_str[size - 2] = c;
			new_str[size - 1] = 0;
			delete[] this->data;
			this->data = new_str;
			return *this;
		}

		bool operator == (string other) const {
			return strcmp(this->data, other.data) == 0;
		}

		bool operator == (const char* other) const {
			return strcmp(this->data, other) == 0;
		}

		friend bool operator == (const char* str1, const string& str2) {
			return strcmp(str1, str2.data) == 0;
		}

		bool operator != (string other) const {
			return !(*this == other);
		}

		bool operator != (const char* other) const {
			return !(*this == other);
		}

		friend bool operator != (const char* str1, const string& str2) {
			return !(str1 == str2);
		}

		friend std::istream& operator >> (std::istream& stream, string& str) {
			char temp;
			bool nsf = true;
			str.clear();

			while (nsf) {
				stream.get(temp);
				if (std::isspace(temp)) {
					nsf = false;
				}
				else {
					str += temp;
				}
			}
			return stream;
		}

		friend std::ostream& operator << (std::ostream& stream, const string& str) {
			return stream << str.data;
		}

		const char* c_str() const { return this->data; }

		inline size_t length() const { return strlen(this->data); }

		void clear() {
			delete[] this->data;
			this->data = new char[1]{ 0 };
		}

		friend std::istream& getline(std::istream& stream, string& str);
	private:
		char* data;
	};

	std::istream& getline(std::istream& stream, string& str) {
		char temp;
		bool nelf = true;
		str.clear();

		while (nelf) {
			stream.get(temp);
			if (temp == '\n') {
				nelf = false;
			}
			else {
				str += temp;
			}
		}
		return stream;
	}
}