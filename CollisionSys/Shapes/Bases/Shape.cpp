#include "Shape.h"

#include "debug/memtrace.h"
#include "Commands/Base/Command.h"
#include "graphics/consoleStyle.h"
#include "graphics/Text.h"

namespace CollSys {
	Shape::Shape() :
		shape(),
		displayColor(sf::Color::White),
		name(),
		_position(),
		_scale({ 1.0, 1.0}),
		_rotation(0.0)
	{
		this->recalculateTransform();
	}

	Shape::~Shape() {}

	Shape& Shape::setPosition(const glib::vec2d& position) {
		this->_position = position;
		this->transform_update = true;
		return *this;
	}

	Shape& Shape::setPosition(double x, double y) {
		return this->setPosition({ x, y });
	}

	Shape& Shape::setScale(const glib::vec2d& scale) {
		this->_scale = scale;
		this->transform_update = true;
		return *this;
	}

	Shape& Shape::setScale(double x, double y) {
		return this->setScale({ x, y });
	}

	Shape& Shape::setRotation(double angle) {
		this->_rotation = angle;
		this->transform_update = true;
		return *this;
	}

	Shape& Shape::move(const glib::vec2d& position) {
		this->_position += position;
		this->transform_update = true;
		return *this;
	}

	Shape& Shape::move(double x, double y) {
		return this->move({ x, y });
	}

	Shape& Shape::scale(const glib::vec2d& scale) {
		this->_scale.scale(scale);
		this->transform_update = true;
		return *this;
	}

	Shape& Shape::scale(double x, double y) {
		return this->scale({ x, y });
	}

	Shape& Shape::rotate(double angle) {
		this->_rotation += angle;
		this->transform_update = true;
		return *this;
	}

	const glib::vec2d& Shape::getPosition() const {
		return this->_position;
	}

	const glib::vec2d& Shape::getScale() const {
		return this->_scale;
	}

	double Shape::getRotation() const {
		return this->_rotation;
	}

	const sf::Transform& Shape::getTransform() const {
		if (this->transform_update) { // ha a transform elavult, frissíteni kell
			this->recalculateTransform();
			this->transform_update = false;
		}
		return this->transform;
	}

	void Shape::setColor(sf::Color color) {
		this->displayColor = color;
	}

	const glib::string& Shape::getName() {
		return this->name;
	}

	void Shape::fromConsole(std::stringstream& buff) {
		if (!(buff >> this->name)) {
			throw Commands::Error("Nem adott nevet a sikidomnak.");
		}
	}

	void Shape::write(std::ostream& stream) const {
		stream <<
			this->_position << ' ' <<
			this->_rotation << ' ' <<
			this->_scale;
	}

	void Shape::read(std::istream& stream) {
		stream >>
			this->_position >>
			this->_rotation >>
			this->_scale;
		this->transform_update = true;
	}

	void Shape::recalculateTransform() const {
		this->transform = sf::Transform();
		this->transform
			.translate(glib::VectorCast(this->_position))
			.rotate(static_cast<float>(this->_rotation))
			.scale(glib::VectorCast(this->_scale));
	}

	void Shape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		sf::VertexArray temp;
		glib::OutlineCast(temp, this->shape, this->displayColor);

		// síkidom nevének előkészítése
		Text text(this->name);
		sf::FloatRect rect = text.getGlobalBounds();
		sf::Vector2f pos =
			glib::VectorCast(this->getPosition()) -
			sf::Vector2f(rect.width / 2.0f, rect.height / 2.0f);
		text.setPosition(pos);

		target.draw(temp, states);
		target.draw(text);
	}


	std::ostream& operator<<(std::ostream& stream, const Shape& obj) {
		obj.write(stream);
		return stream;
	}

	std::istream& operator>>(std::istream& stream, Shape& obj) {
		obj.read(stream);
		return stream;
	}
}
