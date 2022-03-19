#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Circle : public AbstractShape
	{
	public:
		Circle(double radius = 0.5f);

		glib::vec2d support(const glib::vec2d& direction) const override;
	protected:
		void build();
	private:
		double r;
	};
}
