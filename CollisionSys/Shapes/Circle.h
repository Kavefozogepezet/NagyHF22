#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Circle : public AbstractShape
	{
	public:
		Circle(float radius = 0.5f);

		sf::Vector2f support(const sf::Vector2f& direction) const override;
	protected:
		virtual void build() override;
		float refreshInfRng() override;
	};
}
