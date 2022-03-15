#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Polygon : public AbstractShape
	{
	public:
		Polygon();

		sf::Vector2f support(const sf::Vector2f& direction) const override;
	protected:
		virtual void build() override;
		float refreshInfRng() override;
	};
}
