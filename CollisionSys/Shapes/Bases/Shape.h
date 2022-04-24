#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#include "general/vec2.h"
#include "general/string.h"

namespace CollSys {
	class Shape : public sf::Drawable
	{
	public:
		Shape();
		virtual ~Shape();

		Shape& setPosition(const glib::vec2d& position);
		Shape& setPosition(double x, double y);

		Shape& setScale(const glib::vec2d& scale);
		Shape& setScale(double x, double y);

		Shape& setRotation(double angle);

		Shape& move(const glib::vec2d& position);
		Shape& move(double x, double y);

		Shape& scale(const glib::vec2d& scale);
		Shape& scale(double x, double y);

		Shape& rotate(double angle);

		const glib::vec2d& getPosition() const;
		const glib::vec2d& getScale() const;
		double getRotation() const;
		const sf::Transform& getTransform() const;

		/*
		* @brief Be�ll�tja a s�kidom renderel�si sz�n�t.
		* @param color - Ilyen sz�n� lesz a s�kidom.
		*/
		void setColor(sf::Color color);

		/** @returns Az objektum neve */
		const glib::string& getName();

		virtual void fromConsole(std::stringstream& buff);

		friend std::ostream& operator << (std::ostream& stream, const Shape& obj);
		friend std::istream& operator >> (std::istream& stream, Shape& obj);
	protected:
		virtual void write(std::ostream& stream) const;
		virtual void read(std::istream& stream);

		/** @brief Els� sorban a v�ltoz� a s�kidom megjelen�t�s�re szolg�l, k�zel�ti a s�kidom k�rvonalait.*/
		glib::VertexArray shape;

		/** @brief A s�kidom ezzel a sz�nnel lesz a k�perny�re rajzolva.*/
		sf::Color displayColor;

		/** @brief A s�kidom neve */
		glib::string name;

		/*
		* @brief grafikus megjelen�t�shez sz�ks�ges, csak akkor override-oland� ha a lesz�rmazott s�kidom
		* speci�lis megjelen�t�st ig�nyel
		*/
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		glib::vec2d _position, _scale;
		double _rotation;

		mutable bool transform_update;
		mutable sf::Transform transform;

		void recalculateTransform() const;
	};
}
