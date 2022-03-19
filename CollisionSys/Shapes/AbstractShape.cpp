#include "AbstractShape.h"

#include "../debug/memtrace.h"

namespace CollSys {
	AbstractShape::AbstractShape() :
		shape(),
		displayColor(sf::Color::White)
	{}

	void AbstractShape::setColor(sf::Color color) {
		this->displayColor = color;
	}

	void AbstractShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::OutlineCast(temp, this->shape, this->displayColor);
		target.draw(temp, states);
	}
}
