#pragma once

#include "../Bases/AbstractShape.h"

namespace CollSys {
	class Circle : public AbstractShape
	{
	public:
		Circle(const glib::string& type, double radius = 0.5f);
	private:
		double r;

		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		void build();
	};
}
