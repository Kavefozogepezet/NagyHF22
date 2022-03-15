#pragma once

#pragma once

#include "AbstractShape.h"

namespace CollSys {
	class Ellipse : public AbstractShape
	{
	public:
		Ellipse(float a = 0.5f, float b = 0.25f);

		sf::Vector2f support(const sf::Vector2f& direction) const override;
	protected:
		virtual void build() override;
		float refreshInfRng() override;
	private:
		float a, b;
	};
}
