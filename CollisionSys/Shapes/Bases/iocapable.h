#pragma once

#include <iostream>

namespace CollSys {
	class IOCapable
	{
	public:
		IOCapable() = default;
		virtual ~IOCapable() = default;

		friend std::ostream& operator << (std::ostream& stream, const IOCapable& obj);
		friend std::istream& operator >> (std::istream& stream, IOCapable& obj);
	protected:
		virtual void write(std::ostream&) const = 0;
		virtual void read(std::istream&) = 0;	};
}