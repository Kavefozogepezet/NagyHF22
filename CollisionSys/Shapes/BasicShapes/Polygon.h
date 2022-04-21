#pragma once

#include "Shapes/Bases/AbstractShape.h"

namespace CollSys {
	class Polygon : public AbstractShape
	{
	public:
		Polygon(const glib::string& type);

		Polygon(const glib::string& type, std::initializer_list<glib::vec2d> points);
	
		void fromConsole(std::stringstream& buf) override;
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		void build(const std::initializer_list<glib::vec2d>& points);
	};
}
