#include "Polygon.h"

#include "../debug/memtrace.h"
#include "../general/sfmlmath.h"

namespace CollSys {
	Polygon::Polygon() {
		this->build();
		this->refreshInfRng();
	}

	sf::Vector2f Polygon::support(const sf::Vector2f& dir) const {
		sf::Vector2f tdir = sfmath::rotate(dir, -this->getRotation());
		float max_distance = sfmath::dot(this->shape[0].position, tdir);
		sf::Vector2f point = this->shape[0].position;

		for (size_t i = 1; i < this->shape.getVertexCount() - 1; i++) {
			const sf::Vector2f& p = this->shape[i].position;
			float dot_product = sfmath::dot(p, tdir);

			if (dot_product > max_distance) {
				max_distance = dot_product;
				point = p;
			}
		}
		return this->getTransform() * point;
	}

	void Polygon::build() {
		this->shape.resize(6);

		this->shape[0].position = sf::Vector2f(-1.0f, 1.0f);
		this->shape[1].position = sf::Vector2f(1.0f, 1.0f);
		this->shape[2].position = sf::Vector2f(1.53f, -0.4f);
		this->shape[3].position = sf::Vector2f(0.8f, -1.74f);
		this->shape[4].position = sf::Vector2f(-1.0f, -1.0f);
		this->shape[5].position = sf::Vector2f(-1.0f, 1.0f);
	}

	float Polygon::refreshInfRng() {
		for (size_t i = 0; i < this->shape.getVertexCount() - 1; i++) {
			const sf::Vector2f& p = this->shape[i].position;
			float distance = sfmath::length(p);

			if (distance > this->inf_rng) {
				this->inf_rng = distance;
			}
		}
		return this->inf_rng;
	}
}
