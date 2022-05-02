#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#include "general/vec2.h"
#include "general/string.h"

namespace CollSys {
	/*
	* @brief Absztrakt osztály egy síkidomot modellezésére, mely grafikusan megjeleníthető,
	* mozgatható, nagyítható, forgatható. Leszármazottjai fájlba menthetők, fájlból és konzolról beolvashatók.
	*/
	class Shape : public sf::Drawable
	{
	public:
		Shape();
		virtual ~Shape();

		/*
		* @brief A síkidom pozícióját beállítja a megadot vektorra.
		* @param position - A síkidom új pozíciója
		* @returns Referencia saját magára
		*/
		Shape& setPosition(const glib::vec2d& position);
		/*
		* @brief A síkidom pozícióját beállítja a megadot x, y komponensre.
		* @param x - A síkidom új pozíciójának x komponense
		* @param y - A síkidom új pozíciójának y komponense
		* @returns Referencia saját magára
		*/
		Shape& setPosition(double x, double y);

		/*
		* @brief A síkidom nagyítását beállítja a megadot vektorra.
		* @param scale - A síkidom új nagyítása
		* @returns Referencia saját magára
		*/
		Shape& setScale(const glib::vec2d& scale);
		/*
		* @brief A síkidom nagyítását beállítja a megadot x, y komponensre.
		* @param x - A síkidom új nagyításának x komponense
		* @param y - A síkidom új nagyításának y komponense
		* @returns Referencia saját magára
		*/
		Shape& setScale(double x, double y);

		/*
		* @brief A síkidom forgatását beállítja a megadot értékre.
		* @param angle - A síkidom új forgatása fokban
		* @returns Referencia saját magára
		*/
		Shape& setRotation(double angle);

		/*
		* @brief A síkidomot elmozgatja a megadot vektorral.
		* @param delta - A síkidom mozgatásának vektora
		* @returns Referencia saját magára
		*/
		Shape& move(const glib::vec2d& delta);
		/*
		* @brief A síkidomot elmozgatja a megadot x, y komponensel.
		* @param x - A síkidom mozgatásának x komponense
		* @param y - A síkidom mozgatásának y komponense
		* @returns Referencia saját magára
		*/
		Shape& move(double dx, double dy);

		/*
		* @brief A síkidomot nagyítja a megadot vektorral.
		* @param scale - A síkidom nagyításának vektora
		* @returns Referencia saját magára
		*/
		Shape& scale(const glib::vec2d& scale);
		/*
		* @brief A síkidomot nagyítja a megadot x, y komponensel.
		* @param x - A síkidom nagyításának x komponense
		* @param y - A síkidom nagyításának y komponense
		* @returns Referencia saját magára
		*/
		Shape& scale(double x, double y);

		/*
		* @brief A síkidomot elforgatja a megadot értékkel.
		* @param angle - A síkidom forgatásának szöge fokban
		* @returns Referencia saját magára
		*/
		Shape& rotate(double angle);

		/** @returns A síkidom pozíciója */
		const glib::vec2d& getPosition() const;
		/** @returns A síkidom nagyítása */
		const glib::vec2d& getScale() const;
		/** @ returns A síkidom forgatása */
		double getRotation() const;
		/** @returns A síkidom transform-ja */
		const sf::Transform& getTransform() const;

		/*
		* @brief Beállítja a síkidom renderelési színét.
		* @param color - Ilyen színû lesz a síkidom.
		*/
		void setColor(sf::Color color);

		/** @returns Az objektum neve */
		const glib::string& getName();

		/*
		* @brief A kapott stringstream-ből kiolvassa a síkidom adatait.
		* Akkor hívódik meg, ha a síkidomot konzolos parancsal inicializálta a felhasználó.
		* @param buff - stringstream ami tartalmazza a síkidom konzolról beolvasott paramétereit.
		*/
		virtual void fromConsole(std::stringstream& buff);

		friend std::ostream& operator << (std::ostream& stream, const Shape& obj);
		friend std::istream& operator >> (std::istream& stream, Shape& obj);
	protected:
		/*
		* @brief Fájlba menti a síkidom adatait.
		* Ha egy leszármazott saját adatait elmentené, ezt az ősosztály függvényét fölülírva teheti meg.
		* A leszármazott függvénye köteles meghívni az ősosztály write függvényét.
		* @param stream - A stream, ahova a síkidom adatait mentjük
		*/
		virtual void write(std::ostream& stream) const;
		/*
		* @brief Fájlba menti a síkidom adatait.
		* Ha egy leszármazott saját adatait szeretné kiolvasni, ezt az ősosztály függvényét fölülírva teheti meg.
		* A leszármazott függvénye köteles meghívni az ősosztály write függvényét.
		* @params stream - A stream, ahonnan a síkidom adatait kiolvassuk
		*/
		virtual void read(std::istream& stream);

		/** @brief Elsõ sorban a változó a síkidom megjelenítésére szolgál, közelíti a síkidom körvonalait.*/
		glib::VertexArray shape;

		/** @brief A síkidom ezzel a színnel lesz a képernyõre rajzolva.*/
		sf::Color displayColor;

		/** @brief A síkidom neve */
		glib::string name;

		/*
		* @brief grafikus megjelenítéshez szükséges, csak akkor override-olandó ha a leszármazott síkidom
		* speciális megjelenítést igényel.
		*/
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		glib::vec2d _position, _scale;
		double _rotation;

		mutable bool transform_update;
		mutable sf::Transform transform;

		/** @brief Az aktuális pozícióból, nagyításból, és forgatásból frissíti a transform változót. */
		void recalculateTransform() const;
	};
}
