#include "Transformable.h"

#include "../../debug/memtrace.h"

namespace CollSys {
	Transformable::Transformable() :
		_position(),
		_scale({ 1.0, 1.0}),
		_rotation(0.0)
	{
		this->recalculateTransform();
	}

	Transformable::~Transformable() {}

	Transformable& Transformable::setPosition(const glib::vec2d& position) {
		this->_position = position;
		this->transform_update = true;
		return *this;
	}

	Transformable& Transformable::setPosition(double x, double y) {
		return this->setPosition({ x, y });
	}

	Transformable& Transformable::setScale(const glib::vec2d& scale) {
		this->_scale = scale;
		this->transform_update = true;
		return *this;
	}

	Transformable& Transformable::setScale(double x, double y) {
		return this->setScale({ x, y });
	}

	Transformable& Transformable::setRotation(double angle) {
		this->_rotation = angle;
		this->transform_update = true;
		return *this;
	}

	Transformable& Transformable::move(const glib::vec2d& position) {
		this->_position += position;
		this->transform_update = true;
		return *this;
	}

	Transformable& Transformable::move(double x, double y) {
		return this->move({ x, y });
	}

	Transformable& Transformable::scale(const glib::vec2d& scale) {
		this->_scale.scale(scale);
		this->transform_update = true;
		return *this;
	}

	Transformable& Transformable::scale(double x, double y) {
		return this->scale({ x, y });
	}

	Transformable& Transformable::rotate(double angle) {
		this->_rotation += angle;
		this->transform_update = true;
		return *this;
	}

	const glib::vec2d& Transformable::getPosition() const {
		return this->_position;
	}

	const glib::vec2d& Transformable::getScale() const {
		return this->_scale;
	}

	double Transformable::getRotation() const {
		return this->_rotation;
	}

	const sf::Transform& Transformable::getTransform() const {
		if (this->transform_update) {
			this->recalculateTransform();
		}
		return this->transform;
	}

	void Transformable::write(std::ostream& stream) const {
		stream <<
			this->_position << ' ' <<
			this->_rotation << ' ' <<
			this->_scale;
	}

	void Transformable::read(std::istream& stream) {
		stream >>
			this->_position >>
			this->_rotation >>
			this->_scale;
		this->transform_update = true;
	}

	void Transformable::recalculateTransform() const {
		this->transform = sf::Transform();
		this->transform
			.translate(glib::VectorCast(this->_position))
			.rotate(static_cast<float>(this->_rotation))
			.scale(glib::VectorCast(this->_scale));
	}

}
