#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <SFML/Graphics/VertexArray.hpp>

#include "../debug/gtest_lite.h"
#include "array.h"

namespace glib {
	/*
	* @brief Két dimenziós vektor sablonja.
	* @param Base - Primitív, valós számokat ábrázoló típus
	*/
	template <typename Base>
	struct vec2
	{
		using _MyVec2 = vec2<Base>;

		Base x, y;

		vec2() :
			x(static_cast<Base>(0)),
			y(static_cast<Base>(0))
		{}

		vec2(Base X, Base Y) :
			x(X), y(Y)
		{}

		vec2(const _MyVec2& other) :
			x(other.x), y(other.y)
		{}

		template <typename Alt>
		vec2(const vec2<Alt>& other) :
			x(static_cast<Base>(other.x)),
			y(static_cast<Base>(other.y))
		{}

		_MyVec2& operator = (const _MyVec2& other) {
			this->x = other.x;
			this->y = other.y;
			return *this;
		}

		bool operator == (const _MyVec2& other) {
			return
				gtest_lite::almostEQ(this->x, other.x) &&
				gtest_lite::almostEQ(this->y, other.y);
		}

		bool operator != (const _MyVec2& other) {
			return !(*this == other);
		}

		_MyVec2 operator + (const _MyVec2& other) const {
			return _MyVec2(
				this->x + other.x,
				this->y + other.y
			);
		}

		_MyVec2 operator - (const _MyVec2& other) const {
			return _MyVec2(
				this->x - other.x,
				this->y - other.y
			);
		}

		/*
		* @brief Sablon két sfml Vector2 skaláris szorzatának kiszámítására
		* @return vec1 * vec2 (skaláris szorzat)
		*/
		_MyVec2 operator * (Base value) const {
			return _MyVec2(
				this->x * value,
				this->y * value
			);
		}

		_MyVec2 operator / (Base value) const {
			return _MyVec2(
				this->x / value,
				this->y / value
			);
		}

		_MyVec2& operator += (const _MyVec2& other) {
			this->x += other.x;
			this->y += other.y;
			return *this;
		}

		_MyVec2& operator -= (const _MyVec2& other) {
			this->x -= other.x;
			this->y -= other.y;
			return *this;
		}

		_MyVec2& operator *= (Base value) const {
			this->x *= value;
			this->y *= value;
			return *this;
		}

		_MyVec2& operator /= (Base value) const {
			this->x /= value;
			this->y /= value;
			return *this;
		}

		Base operator * (const _MyVec2& other) {
			return this->x * other.x + this->y * other.y;
		}

		/** @return A vektor hossza */
		Base length() {
			return sqrt(this->x * this->x + this->y * this->y);
		}

		/*
		* @brief A vektor hosszát egység hosszúra állítja.
		* @returns referencia saját magára
		*/
		_MyVec2& normalize() {
			Base l = this->length();
			*this /= l;
			return *this;
		}

		/*
		* @brief Elforgatja a vektort a kívánt szöggel.
		* @param degrees - A forgatás szöge fokban
		* @return referencia saját magára
		*/
		_MyVec2& rotate(Base degrees) {
			Base rad = (degrees * (Base)M_PI) / (Base)180.0,
				s = std::sin(rad),
				c = std::cos(rad),
				x_rot = this->x * c - this->y * s,
				y_rot = this->x * s + this->y * c;

			*this = { x_rot, y_rot };
			return *this;
		}
	};

	template <typename Base>
	vec2<Base> operator - (const vec2<Base>& vec) {
		return vec2<Base>(-vec.x, -vec.y);
	}


	template <typename Base>
	vec2<Base> operator * (Base, const vec2<Base>& vec) {
		return vec * Base;
	}

	/*
	* @brief Létrehoz egy egység hosszú vektort, amely egy minta vektorral azonos irányú.
	* @param vec - A minta vektor
	* @returns Az egység hosszú vektor
	*/
	template <typename Base>
	vec2<Base> normalize(const vec2<Base>& vec) {
		vec2<Base> tmp = vec;
		return tmp.normalize();
	}

	/*
	* @brief Létrehoz egy minta vektorral azonos hosszúságú, elforgatott irányú vektort.
	* @param vec - A minta vektor
	* @param degrees - A forgatás szöge fokban
	* @returns Az elforgatott vektor
	*/
	template <typename Base>
	vec2<Base> rotate(const vec2<Base>& vec, Base degrees) {
		vec2<Base> tmp = vec;
		return tmp.rotate(degrees);
	}

	/*
	* Létrehoz egy minta vektorra merőleges vektort úgy, hogy az új vektor a mintához képest jobbra mutat.
	* @param vec - A minta vektor
	* @return A mintára merőleges vektor
	*/
	template <typename Base>
	vec2<Base> getNormal(const vec2<Base>& vec) {
		return vec2<Base>(vec.y, -vec.x);
	}

	/*
	* @brief Létrehoz egy minta vektorra merőleges vektort úgy, hogy a vektor skaláris szorzata nem negatív legyen egy megadott "side" vektorhoz képest.
	* Azaz a keletkező vektor a minta azon oldala felé mutat, amelyik oldalon a "side" végpontja van.
	* @param vec - A minta vektor
	* @param side - A vektor, ami megadja a merőleges irányát
	* @return A kritériumnak megfelelő, a mintára merőleges vektor
	*/
	template <typename Base>
	vec2<Base> getNormal(const vec2<Base>& vec, const vec2<Base>& side) {
		vec2<Base> n = getNormal(vec);
		bool dir = n * dir < static_cast<Base>(0);
		return dir ? -n : n;
	}

	typedef vec2<float>	 vec2f;
	typedef vec2<double> vec2d;

	typedef glib::array<vec2f>  VertexArrayf;
	typedef glib::array<vec2d> VertexArray;

	template<typename Base>
	sf::Vector2f VectorCast(const vec2<Base>& vec) {
		return sf::Vector2f(
			static_cast<float>(vec.x),
			static_cast<float>(vec.y)
		);
	}

	//template <typename T>
	void VertexArrayCast(
		glib::array<vec2<double>>& src,
		sf::VertexArray& dest,
		sf::PrimitiveType type = sf::LineStrip,
		sf::Color color = sf::Color::White
	) {
		dest.clear();
		dest.resize(src.size());
		for (auto vec : src) {
			sf::Vector2f temp = VectorCast(vec);
			dest.append(sf::Vertex(temp, color));
		}
	}
}
