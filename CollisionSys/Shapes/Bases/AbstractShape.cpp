#include "AbstractShape.h"

#include "debug/memtrace.h"
#include "general/consoleStyle.h"

namespace CollSys {
	AbstractShape::AbstractShape(const glib::string& type) :
		shape(),
		displayColor(sf::Color::White),
		name(),
		my_type(type)
	{}

	AbstractShape::~AbstractShape() {}

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

	const glib::string& AbstractShape::getType() {
		return this->my_type;
	}

	void AbstractShape::write(std::ostream& stream) const {
		Transformable::write(stream);
		stream << ' ' << this->name;
	}

	void AbstractShape::read(std::istream& stream) {
		Transformable::read(stream);
		stream >> this->name;
	}

	bool AbstractShape::fromConsole(glib::linebuffer& buff) {
		buff >> name;
		if (buff.eol()) {
			std::cout << glib::consoleStyle::error <<
				"Nem adott nevet a sikidomnak." <<
				glib::consoleStyle::none << std::endl;
			return false;
		}
		return true;
	}

	const glib::string& AbstractShape::getName() {
		return this->name;
	}

	void AbstractShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::OutlineCast(temp, this->shape, this->displayColor);
		target.draw(temp, states);
	}
}
