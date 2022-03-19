#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Polygon : public AbstractShape
	{
	public:
		Polygon();

		Polygon(std::initializer_list<glib::vec2d> points);
		

		glib::vec2d support(const glib::vec2d& direction) const override;
	protected:
		void build(const std::initializer_list<glib::vec2d>& points);
	};
}
