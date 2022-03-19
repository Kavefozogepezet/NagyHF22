#include "Circle.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"

namespace CollSys {
	Circle::Circle(double r) {
		this->inf_rng = r; 
		this->build();
	}

	glib::vec2d Circle::support(const glib::vec2d& dir) const {
		glib::vec2d tdir = glib::rotate(glib::normalize(dir), -static_cast<double>(this->getRotation()));
		return this->getTransform() * (this->inf_rng * tdir);
	}

	void Circle::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			float angle = 2.0f * (float)M_PI * static_cast<float>(i) / 32.0f;
			glib::vec2d d(std::cos(angle), std::sin(angle));
			this->shape[i] = this->inf_rng * d;
		}
	}

	double Circle::refreshInfRng() { return this->inf_rng; }
}
