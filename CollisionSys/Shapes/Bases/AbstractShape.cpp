
#include "AbstractShape.h"

#include "../../debug/memtrace.h"

namespace CollSys {
	AbstractShape::AbstractShape() :
		shape(),
		displayColor(sf::Color::White)
	{}

	glib::vec2d AbstractShape::support(const glib::vec2d& direction) const {
		if (this->getScale() == glib::vec2d(0.0, 0.0)) {
			return this->getPosition();
		}
		glib::vec2d objSpaceDir = direction;
		objSpaceDir.rotate(-this->getRotation()).scale(this->getScale());
		return this->getTransform() * this->objSpaceSupport(objSpaceDir);
	}

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
