#pragma once

#include <SFML/Graphics.hpp>

namespace CollSys {
	/*
	* @brief Ez az absztrakt osztály deklarálja azt az alap funkcionalitást amely szükséges a GJK algoritmushoz.
	* Deklarál továbbá olyan segédfüggvényeket, melyek megkönnyítik egy síkidom karbantartását.
	*/
	class AbstractShape : public sf::Drawable, public sf::Transformable
	{
	public:
		AbstractShape();

		/*
		* @brief A support függvény megkeresi a síkidom legtávolabbi pontját egy adott irányban.
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - A legmesszebb pont koordinátája
		*/
		virtual sf::Vector2f support(const sf::Vector2f& direction) const = 0;
		
		/*
		* @brief Megadja azt a legkisebb sugarú, síkidom középpontjával egyező középpontú
		* kör (befolyási kör) sugarát, amely az egész síkidomot tartalmazza.
		* @return - A sugár hossza.
		*/
		double influenceRange() const;
	protected:
		/** @brief Tárolja a befolyási kör sugarának hosszát. A gyerek osztályok maguk frissítik.*/
		float inf_rng;
		/** @brief Első sorban a változó a síkidom megjelenítésére szolgál, közelíti a síkidom körvonalait.*/
		sf::VertexArray shape;

		/*
		* @brief A build tagfüggvényben ajánlott feltölteni a shape változót a síkidomot közelítő körvonallal. Használata opcionális.
		*/
		virtual void build() {};

		/*
		* @brief Akkor hívandó meg, amikor a síkidom dimenziói változnak.
		* Frissíti a befolyási kör sugarát tartalmazó inf_rng változót.
		* @return Az új befolyási kör sugarának hossza.
		*/
		virtual float refreshInfRng() = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
