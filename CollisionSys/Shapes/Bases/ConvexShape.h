#pragma once

#include "general/vec2.h"
#include "Shape.h"

namespace CollSys {
	/*
	* @brief Ez az absztrakt osztály deklarálja azt az alap funkcionalitást amely szükséges a GJK algoritmushoz.
	*/
	class ConvexShape : public Shape
	{
	public:
		ConvexShape(const glib::string& type);
		virtual ~ConvexShape();

		/*
		* @brief A support függvény a keresés irányának vektorán elvégzi a síkidomon végzett transzformációk inverzét,
		* majd az így keletkező vektorral meghívja az objSpaceSupport tagfüggvényt. Így a gyerekosztályoknak csak az
		* objSpaceSupport függvényt szükséges felülírniuk, amiben a transzformációkat figyelmen kívül hagyják.
		* @param direction - Az irány, amelyben a legmesszebbi pontot keressük
		* @return - Az objSpaceSupport által visszaadott vektor, amin elvégzi a síkidom transzformációit
		*/
		glib::vec2d support(const glib::vec2d& direction) const;

		const glib::string& getType();
	protected:		
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
	};
}
