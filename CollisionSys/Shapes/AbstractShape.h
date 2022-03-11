#pragma once

#include <SFML/Graphics.hpp>

namespace CollSys {
	class AbstractShape : public sf::Drawable, public sf::Transformable
	{
	public:
		AbstractShape() : inf_rng(0.0) {}

		/*
		* @brief A support függvény a GJK algoritmushoz.
		* Egy adott irányban megkeresi a síkidom legtávolabbi pontját
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - A legmesszebb pont koordinátája
		*/
		virtual sf::Vector2f support(const sf::Vector2f& direction) const = 0;
		
		/*
		* @brief Megadja azt a legkisebb sugarú, síkidom középpontjával egyező középpontú
		* kör (befolyási kör) sugarát, amely az egész síkidomot tartalmazza
		* @return - A sugár hossza.
		*/
		virtual double influenceRange() const = 0;
	protected:
		double inf_rng;
	private:
	};
}
