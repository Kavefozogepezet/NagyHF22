#include "Circle.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"

namespace CollSys {
	Circle::Circle(float r) {
		this->inf_rng = r; 
		this->build();
	}

	sf::Vector2f Circle::support(const sf::Vector2f& dir) const {
		sf::Vector2f tdir = sfmath::rotate(dir, -this->getRotation());
		return this->getTransform() * (this->inf_rng * tdir);
	}

	void Circle::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			float angle = 2.0f * (float)M_PI * static_cast<float>(i) / 32.0f;
			sf::Vector2f d(std::cosf(angle), std::sinf(angle));
			this->shape[i].position = this->inf_rng * d;
		}
	}

	float Circle::refreshInfRng() { return this->inf_rng; }
}
