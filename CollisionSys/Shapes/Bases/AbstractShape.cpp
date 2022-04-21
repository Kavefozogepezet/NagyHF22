#include "AbstractShape.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"
#include "graphics/Text.h"
#include "Commands/Base/Command.h"

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

	void AbstractShape::fromConsole(std::stringstream& buff) {
		if (!(buff >> this->name)) {
			throw Commands::Error("Nem adott nevet a sikidomnak.");
		}
	}

	const glib::string& AbstractShape::getName() {
		return this->name;
	}

	void AbstractShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::OutlineCast(temp, this->shape, this->displayColor);

		Text text(this->name);
		sf::FloatRect rect = text.getGlobalBounds();
		sf::Vector2f pos =
			glib::VectorCast(this->getPosition()) -
			sf::Vector2f(rect.width / 2.0f, rect.height / 2.0f);
		text.setPosition(pos);

		target.draw(temp, states);
		target.draw(text);
	}
}
