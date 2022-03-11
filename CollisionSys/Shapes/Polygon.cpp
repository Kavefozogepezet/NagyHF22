#include "Polygon.h"
#include "../general/sfmlmath.h"

namespace CollSys {
	Polygon::Polygon() :
		points(sf::LinesStrip, 6)
	{
		this->points[0].position = sf::Vector2f(-1.0f, 1.0f);
		this->points[1].position = sf::Vector2f(1.0f, 1.0f);
		this->points[2].position = sf::Vector2f(1.53f, -0.4f);
		this->points[3].position = sf::Vector2f(0.8f, -1.74f);
		this->points[4].position = sf::Vector2f(-1.0f, -1.0f);
		this->points[5].position = sf::Vector2f(-1.0f, 1.0f);

		for (size_t i = 0; i < this->points.getVertexCount() - 1; i++) {
			const sf::Vector2f& p = this->points[i].position;
			float distance = sfmath::length(p);

			if (distance > this->inf_rng) {
				this->inf_rng = distance;
			}
		}
	}

	sf::Vector2f Polygon::support(const sf::Vector2f& dir) const {
		sf::Vector2f tdir = sfmath::rotate(dir, -this->getRotation());
		float max_distance = sfmath::dot(this->points[0].position, tdir);
		sf::Vector2f point = this->points[0].position;

		for (size_t i = 1; i < this->points.getVertexCount() - 1; i++) {
			const sf::Vector2f& p = this->points[i].position;
			float dot_product = sfmath::dot(p, tdir);

			if (dot_product > max_distance) {
				max_distance = dot_product;
				point = p;
			}
		}
		return this->getTransform() * point;
	}

	double Polygon::influenceRange() const { return this->inf_rng; }

	void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		target.draw(this->points, states);
	}
}
