#pragma once

#include "../Bases/AbstractShape.h"

namespace CollSys {
	class Polygon : public AbstractShape
	{
	public:
		Polygon(const glib::string& type);

		Polygon(const glib::string& type, std::initializer_list<glib::vec2d> points);
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		void build(const std::initializer_list<glib::vec2d>& points);
	};
}
