#pragma once

#include "../Bases/AbstractShape.h"

namespace CollSys {
	class Point : public AbstractShape
	{
	public:
		Point(double X = 0.0, double Y = 0.0);

		Point(const glib::vec2d& point);
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
