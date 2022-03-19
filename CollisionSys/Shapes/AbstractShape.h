#pragma once

#include <SFML/Graphics.hpp>

#include "../general/vec2.h"

namespace CollSys {
	/*
	* @brief Ez az absztrakt osztály deklarálja azt az alap funkcionalitást amely szükséges a GJK algoritmushoz.
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
		virtual glib::vec2d support(const glib::vec2d& direction) const = 0;
		
		/*
		* @brief Megadja azt a legkisebb sugarú, síkidom középpontjával egyező középpontú
		* kör (befolyási kör) sugarát, amely az egész síkidomot tartalmazza.
		* @return - A sugár hossza.
		*/
		double influenceRange() const;
	protected:
		/** @brief Tárolja a befolyási kör sugarának hosszát. Az osztály gyerekei maguk kezelik az értékét.*/
		double inf_rng;
		/** @brief Első sorban a változó a síkidom megjelenítésére szolgál, közelíti a síkidom körvonalait.*/
		glib::VertexArray shape;

		/*
		* @brief Akkor hívandó meg, amikor a síkidom dimenziói változnak.
		* Frissítenie kell a befolyási kör sugarát tartalmazó inf_rng változót.
		* @return Az új befolyási kör sugarának hossza.
		*/
		virtual double refreshInfRng() = 0;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
