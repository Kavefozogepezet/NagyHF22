#include "Point.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	Point::Point(const glib::string& type, double X, double Y) :
		AbstractShape(type)
	{
		this->name = "Point";
		this->shape.resize(1);
		this->shape[0] = { X, Y };
	}

	Point::Point(const glib::string& type, const glib::vec2d& point) :
		AbstractShape(type)
	{
		this->name = "Point";
		this->shape.resize(1);
		this->shape[0] = point;
	}

	bool Point::fromConsole(std::stringstream& buf) {
		if (!AbstractShape::fromConsole(buf)) {
			return false;
		}
		glib::vec2d p;
		if (buf >> this->shape[0]) {
			return true;
		}
		else {
			cStyle::error() << "Rossz parameterezes" << cStyle::endl;
			return false;
		}
	}

	glib::vec2d Point::objSpaceSupport(const glib::vec2d& direction) const {
		return this->shape[0];
	}

	void Point::write(std::ostream& stream) const {
		AbstractShape::write(stream);
		stream << ' ' << this->shape[0].x << ' ' << this->shape[0].y;
	}

	void Point::read(std::istream& stream) {
		AbstractShape::read(stream);
		stream >> this->shape[0].x >> this->shape[0].y;
	}

	void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::VertexArrayCast(temp, this->shape, this->displayColor, sf::Points);
		target.draw(temp, states);
	}
}
