#pragma once

#pragma once

#include "../Bases/AbstractShape.h"

namespace CollSys {
	class Ellipse : public AbstractShape
	{
	public:
		Ellipse(const glib::string& type, double a = 0.5f, double b = 0.25f);
	private:
		double a, b;

		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		void build();
	};
}
