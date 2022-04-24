#pragma once

#include "Shapes/Bases/ConvexShape.h"

namespace CollSys {
	class Polygon : public ConvexShape
	{
	public:
		Polygon(const glib::string& type);

		Polygon(const glib::string& type, std::initializer_list<glib::vec2d> points);
	
		void fromConsole(std::stringstream& buf) override;
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;
	private:
		void build(const std::initializer_list<glib::vec2d>& points);
	};
}
