#pragma once

#include <stdexcept>
#include <cctype>
#include <iostream>

namespace glib {
	class string {
	public:
		string();

		string(const char* str);

		string(const string& other);

		string(const std::string& stdstr);

		~string();

		operator std::string() const;

		bool empty() const;

		string& operator = (const string& other);

		char& operator [] (size_t idx);

		const char& operator [] (size_t idx) const;

		string operator + (const string& other) const;

		string operator + (const char* other) const;

		friend string operator + (const char* str1, const string& str2);

		string operator + (char c) const;

		friend string operator + (char c, const string& str2);

		string operator += (const string& other);

		string operator += (const char* other);

		string operator += (const char c);

		bool operator == (string other) const;

		bool operator == (const char* other) const;

		friend bool operator == (const char* str1, const string& str2);

		bool operator != (string other) const;

		bool operator != (const char* other) const;

		friend bool operator != (const char* str1, const string& str2);

		friend std::istream& operator >> (std::istream& stream, string& str);

		friend std::ostream& operator << (std::ostream& stream, const string& str);

		const char* c_str() const;

		inline size_t length() const;

		void clear();

		friend std::istream& getline(std::istream& stream, string& str);
	private:
		char* data;
	};
}

namespace std {
	istream& getline(istream& stream, glib::string& str);
}