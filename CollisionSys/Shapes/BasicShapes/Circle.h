#pragma once

#include "../Bases/AbstractShape.h"

namespace CollSys {
	class Circle : public AbstractShape
	{
	public:
		Circle(double radius = 0.5f);

		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;
	protected:
		void build();
	private:
		double r;
	};
}
