#pragma once

#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Ellipse : public AbstractShape
	{
	public:
		Ellipse(double a = 0.5f, double b = 0.25f);

		glib::vec2d support(const glib::vec2d& direction) const override;
	protected:
		void build();
		double refreshInfRng() override;
	private:
		double a, b;
	};
}
