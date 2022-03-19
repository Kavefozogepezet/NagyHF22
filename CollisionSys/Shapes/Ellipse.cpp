#include "Ellipse.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"


namespace CollSys {
	Ellipse::Ellipse(double a, double b) :
		a(a), b(b)
	{
		this->build();
	}

	glib::vec2d Ellipse::support(const glib::vec2d& dir) const {
		glib::vec2d tdir = glib::rotate(dir, -this->getRotation());
		double a2 = this->a * this->a,
			sqr = (tdir.y / tdir.x) * (this->b / a2),
			x = 1.0f / std::sqrt(sqr * sqr + 1 / a2),
			y = this->b * std::sqrt(1 - (x * x) / a2);

		if (tdir.x < 0) { x = -x; }
		if (tdir.y < 0) { y = -y; }

		return this->getTransform() * glib::vec2d(x, y);
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
