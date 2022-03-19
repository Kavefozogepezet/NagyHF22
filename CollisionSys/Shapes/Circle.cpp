#include "Circle.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"

namespace CollSys {
	Circle::Circle(double radius) :
		r(radius)
	{ 
		this->build();
	}

	glib::vec2d Circle::support(const glib::vec2d& dir) const {
		glib::vec2d tdir = glib::rotate(glib::normalize(dir), -this->getRotation());
		return this->getTransform() * (this->r * tdir);
	}

	void Circle::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			double angle = 2.0f * M_PI * static_cast<double>(i) / 32.0;
			glib::vec2d d(std::cos(angle), std::sin(angle));
			this->shape[i] = this->r * d;
		}
	}
}
