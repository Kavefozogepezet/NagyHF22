#include "Point.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"
#include "Commands/Base/Command.h"

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

	void Point::fromConsole(std::stringstream& buf) {
		AbstractShape::fromConsole(buf);

		glib::vec2d p;
		if (!(buf >> this->shape[0])) {
			throw Commands::Error("Nem adta meg a pontot");
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
