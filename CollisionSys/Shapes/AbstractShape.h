#pragma once

#include <SFML/Graphics.hpp>

#include "../general/vec2.h"
#include "Transformable.h"

namespace CollSys {
	// Contact osztály előre deklarálása.
	class Contact;

	/*
	* @brief Ez az absztrakt osztály deklarálja azt az alap funkcionalitást amely szükséges a GJK algoritmushoz.
	*/
	class AbstractShape : public sf::Drawable, public Transformable
	{
	public:
		AbstractShape();

		/*
		* @brief A support függvény megkeresi a síkidom legtávolabbi pontját egy adott irányban.
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - A legmesszebb pont koordinátája
		*/
		virtual glib::vec2d support(const glib::vec2d& direction) const = 0;

		void setColor(sf::Color color);
	protected:
		/** @brief Első sorban a változó a síkidom megjelenítésére szolgál, közelíti a síkidom körvonalait.*/
		glib::VertexArray shape;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Color displayColor;
	};
}
