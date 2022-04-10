﻿#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#include "general/vec2.h"
#include "Transformable.h"
#include "general/string.h"
#include "iocapable.h"

namespace CollSys {
	/*
	* @brief Ez az absztrakt osztály deklarálja azt az alap funkcionalitást amely szükséges a GJK algoritmushoz.
	*/
	class AbstractShape : public sf::Drawable, public Transformable
	{
	public:
		AbstractShape(const glib::string& type);

		virtual ~AbstractShape();

		/*
		* @brief A support függvény a keresés irányának vektorán elvégzi a síkidomon végzett transzformációk inverzét,
		* majd az így keletkező vektorral meghívja az objSpaceSupport tagfüggvényt. Így a gyerekosztályoknak csak az
		* objSpaceSupport függvényt szükséges felülírniuk, amiben a transzformációkat figyelmen kívül hagyják.
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - Az objSpaceSupport által visszaadott vektor, amin elvégzi a síkidom transzformációit
		*/
		glib::vec2d support(const glib::vec2d& direction) const;

		/*
		* @brief Beállítja a síkidom renderelési színét.
		* @param color - Ilyen színű lesz a síkidom.
		*/
		void setColor(sf::Color color);

		const glib::string& getType();

		virtual bool fromConsole(std::stringstream& buff);

		/** @returns Az objektum neve */
		const glib::string& getName();
	protected:
		/** @brief Első sorban a változó a síkidom megjelenítésére szolgál, közelíti a síkidom körvonalait.*/
		glib::VertexArray shape;

		/** @brief A síkidom ezzel a színnel lesz a képernyőre rajzolva.*/
		sf::Color displayColor;

		/** @brief A síkidom neve */
		glib::string name;
		
		/** @brief A síkidom típusának az a string tekintendő, amivel regisztrálva van.*/
		glib::string my_type;

		/*
		* @brief Megkeresi a síkidom legtávolabbi pontját egy adott irányban. Nem veszi figyelembe a síkidomon végzett transzformációkat
		* (nagyítás, forgatás). A gyerek osztályok maguk implementálják.
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - A legmesszebb pont koordinátája
		*/
		virtual glib::vec2d objSpaceSupport(const glib::vec2d& direction) const = 0;

		virtual void write(std::ostream& stream) const override;

		virtual void read(std::istream& stream) override;

		/*
		* @brief grafikus megjelenítéshez szükséges, csak akkor override-olandó ha a leszármazott síkidom
		* speciális megjelenítést igényel
		*/
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
