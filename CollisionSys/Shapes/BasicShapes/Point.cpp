#include "Point.h"

namespace CollSys {
	Point::Point(double X, double Y) {
		this->name = "Point";
		this->shape.resize(1);
		this->shape[0] = { X, Y };
	}

	Point::Point(const glib::vec2d& point) {
		this->name = "Point";
		this->shape.resize(1);
		this->shape[0] = point;
	}

	glib::vec2d Point::objSpaceSupport(const glib::vec2d& direction) const {
		return this->shape[0];
	}

	void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::VertexArrayCast(temp, this->shape, this->displayColor, sf::Points);
		target.draw(temp, states);
	}
}
