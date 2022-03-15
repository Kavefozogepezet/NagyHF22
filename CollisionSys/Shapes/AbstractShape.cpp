#include "AbstractShape.h"

#include "../debug/memtrace.h"

namespace CollSys {
	AbstractShape::AbstractShape() :
		inf_rng(0.0f),
		shape(sf::LineStrip, 0)
	{}

	double AbstractShape::influenceRange() const { return this->inf_rng; }

	void AbstractShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		target.draw(this->shape, states);
	}
}
