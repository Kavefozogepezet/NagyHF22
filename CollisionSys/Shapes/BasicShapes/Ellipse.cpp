#include "Ellipse.h"

#include "../../debug/memtrace.h"


namespace CollSys {
	Ellipse::Ellipse(double a, double b) :
		a(a), b(b)
	{
		this->build();
	}

	glib::vec2d Ellipse::objSpaceSupport(const glib::vec2d& dir) const {
		double a2 = this->a * this->a,
			sqr = (dir.y / dir.x) * (this->b / a2),
			x = 1.0f / std::sqrt(sqr * sqr + 1 / a2),
			y = this->b * std::sqrt(1 - (x * x) / a2);

		if (dir.x < 0) { x = -x; }
		if (dir.y < 0) { y = -y; }

		return glib::vec2d(x, y);
	}

	void Ellipse::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			double angle = 2.0f * M_PI * static_cast<double>(i) / 32.0f;
			glib::vec2d d(std::cos(angle), std::sin(angle));
			glib::vec2d p = this->support(d);
			this->shape[i] = p;
		}
	}
}
