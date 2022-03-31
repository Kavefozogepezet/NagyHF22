#pragma once

#include "SFML/Graphics/Transform.hpp"

#include "general/vec2.h"
#include "iocapable.h"

namespace CollSys {
	class Transformable : public IOCapable
	{
	public:
		Transformable();

		virtual ~Transformable();

		Transformable& setPosition(const glib::vec2d& position);
		Transformable& setPosition(double x, double y);

		Transformable& setScale(const glib::vec2d& scale);
		Transformable& setScale(double x, double y);

		Transformable& setRotation(double angle);

		Transformable& move(const glib::vec2d& position);
		Transformable& move(double x, double y);

		Transformable& scale(const glib::vec2d& scale);
		Transformable& scale(double x, double y);

		Transformable& rotate(double angle);

		const glib::vec2d& getPosition() const;
		const glib::vec2d& getScale() const;
		double getRotation() const;

		const sf::Transform& getTransform() const;
	protected:
		virtual void write(std::ostream& stream) const override;

		virtual void read(std::istream& stream) override;
	private:
		glib::vec2d _position, _scale;
		double _rotation;

		mutable bool transform_update;
		mutable sf::Transform transform;

		void recalculateTransform() const;
	};
}
