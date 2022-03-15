#include "Ellipse.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"


namespace CollSys {
	Ellipse::Ellipse(float a, float b) :
		a(a), b(b)
	{
		this->refreshInfRng();
		this->build();
	}

	sf::Vector2f Ellipse::support(const sf::Vector2f& dir) const {
		sf::Vector2f tdir = sfmath::rotate(dir, -this->getRotation());
		float a2 = this->a * this->a,
			sqr = (tdir.y / tdir.x) * (this->b / a2),
			x = 1.0f / std::sqrtf(sqr * sqr + 1 / a2),
			y = this->b * std::sqrtf(1 - (x * x) / a2);

		if (tdir.x < 0) { x = -x; }
		if (tdir.y < 0) { y = -y; }

		return this->getTransform() * sf::Vector2f(x, y);
	}

	void Ellipse::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			float angle = 2.0f * (float)M_PI * static_cast<float>(i) / 32.0f;
			sf::Vector2f d(std::cosf(angle), std::sinf(angle));
			sf::Vector2f p = this->support(d);
			this->shape[i].position = p;
		}
	}

	float Ellipse::refreshInfRng() {
		if (this->b > this->a) {
			this->inf_rng = this->b;
		}
		else {
			this->inf_rng = this->a;
		}
		return this->inf_rng;
	}
}
