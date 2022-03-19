#include "AbstractShape.h"

#include "../debug/memtrace.h"

namespace CollSys {
	AbstractShape::AbstractShape() :
		inf_rng(0.0f),
		shape()
	{}

	double AbstractShape::influenceRange() const { return this->inf_rng; }

	void AbstractShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::VertexArrayCast(temp, this->shape);
		target.draw(temp, states);
	}
}
