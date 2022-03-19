#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <SFML/Graphics/VertexArray.hpp>

#include "../debug/gtest_lite.h"
#include "array.h"
#include "list.h"

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

		bool operator == (const _MyVec2& other) const {
			return
				gtest_lite::almostEQ(this->x, other.x) &&
				gtest_lite::almostEQ(this->y, other.y);
		}

		bool operator != (const _MyVec2& other) const {
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

		_MyVec2& operator *= (Base value) {
			this->x *= value;
			this->y *= value;
			return *this;
		}

		_MyVec2& operator /= (Base value) {
			this->x /= value;
			this->y /= value;
			return *this;
		}

		Base operator * (const _MyVec2& other) const {
			return this->x * other.x + this->y * other.y;
		}

		/** @return A vektor hossza */
		Base length() const {
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

		/*
		* @brief A vektor x és y komponensét szorozza a megadott értékekkel.
		* @param scale_x - Az érték amivel az x komponens szorzódik
		* @param scale_y - Az érték amivel az y komponens szorzódik
		* @return referencia saját magára
		*/
		_MyVec2& scale(Base scale_x, Base scale_y) {
			this->x *= scale_x;
			this->y *= scale_y;
			return *this;
		}
	};

	/** @return Negált vektor */
	template <typename Base>
	vec2<Base> operator - (const vec2<Base>& vec) {
		return vec2<Base>(-vec.x, -vec.y);
	}


	template <typename Base>
	vec2<Base> operator * (Base value, const vec2<Base>& vec) {
		return vec * value;
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
	* @brief Létrehoz egy vektort amely x és y komponense egy minta vektor x és y komponensének szorzata a megadott értékekkel.
	* @param scale_x - Az érték amivel az x komponens szorzódik
	* @param scale_y - Az érték amivel az y komponens szorzódik
	* @return Az átméretezett vektor
	*/
	template <typename Base>
	vec2<Base> scale(const vec2<Base>& vec, Base scale_x, Base scale_y) {
		vec2<Base> tmp = vec;
		return tmp.scale(scale_x, scale_y);
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
		bool dir = (n * side) < static_cast<Base>(0);
		return dir ? -n : n;
	}

	typedef vec2<float>	 vec2f;
	typedef vec2<double> vec2d;

	typedef glib::array<vec2f>  VertexArrayf;
	typedef glib::array<vec2d> VertexArray;

	typedef glib::list<vec2f> VertexListf;
	typedef glib::list<vec2d> VertexList;

	/*
	* @brief Egy vec2<Base> típusú vektort átalakít sf:Vector2f típusúvá
	* @param vec - Az átalakítandó vektor
	* @returns Átalakított vektor
	*/
	template<typename Base>
	sf::Vector2f VectorCast(const vec2<Base>& vec) {
		return sf::Vector2f(
			static_cast<float>(vec.x),
			static_cast<float>(vec.y)
		);
	}

	/*
	* @brief Egy vec2<Base> típusú vektort átalakít sf:Vector2f típusúvá
	* @param vec - Az átalakítandó vektor
	* @returns Átalakított vektor
	*/
	template<typename Base>
	vec2<Base> VectorCast(const sf::Vector2f& vec) {
		return vec2<Base>(
			static_cast<Base>(vec.x),
			static_cast<Base>(vec.y)
		);
	}

	/*
	* @brief Feltölt egy sfml formátumú vertex array-t egy glib formátumú tartalmával.
	* @param dest - Ebbe másol
	* @param src - Innen másol
	* @param color - A pontok színe
	* @param type - A primitív alakzat típusa amit az array tárol
	*/
	template <typename Base>
	void VertexArrayCast(
		sf::VertexArray& dest,
		const glib::array<vec2<Base>>& src,
		sf::Color color = sf::Color::White,
		sf::PrimitiveType type = sf::LineStrip
	) {
		dest.clear();
		dest.setPrimitiveType(type);
		dest.resize(src.size());
		for (size_t i = 0; i < src.size(); i++) {
			dest[i].position = VectorCast(src[i]);
		}
	}

	template <typename Base>
	void OutlineCast(
		sf::VertexArray& dest,
		const glib::array<vec2<Base>>& src,
		sf::Color color = sf::Color::White
	) {
		dest.clear();
		dest.setPrimitiveType(sf::LineStrip);
		dest.resize(src.size() + 1);
		for (size_t i = 0; i < src.size(); i++) {
			dest[i].position = VectorCast(src[i]);
		}
		dest[dest.getVertexCount() - 1].position = src[0];
	}

	template <typename Base>
	vec2<Base> operator * (const sf::Transform& transform, vec2<Base> vec) {
		sf::Vector2f temp = transform * VectorCast(vec);
		return VectorCast<Base>(temp);
	}
}
