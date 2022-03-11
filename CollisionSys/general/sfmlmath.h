#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>

#include <iostream>

namespace sfmath {
	/*
	* @brief Sablon két sfml Vector2 skaláris szorzatának kiszámítására
	* @param vec1 - Egyik vektor
	* @param vec2 - Másik vektor
	* @return vec1 * vec2 (skaláris szorzat)
	*/
	template <typename T>
	T dot(const sf::Vector2<T>& vec1, const sf::Vector2<T>& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	/*
	* @brief Sablon egy Vector2 hosszának meghatározására.
	* @param vec1 - A vektor, aminek a hosszát meg szeretnénk állapítani
	* @return A paraméterként kapott vektor hossza
	*/
	template <typename T>
	T length(const sf::Vector2<T>& vec) {
		float vec_x = static_cast<float>(vec.x), 
			   vec_y = static_cast<float>(vec.y);
		float length = std::sqrtf(vec_x * vec_x + vec_y * vec_y);
		return static_cast<T>(length);
	}
	 /*
	 * @brief Egy Vector2 méretét egység hosszúra állítja.
	 * @param vec - A vektor
	 * @return Egység hosszú, vec-el azonos irányú vektor
	 */
	template <typename T>
	sf::Vector2<T> normalize(const sf::Vector2<T>& vec) {
		T l = length(vec);
		return sf::Vector2f(vec.x / l, vec.y / l);
	}

	/*
	* @brief Egy Vektor2 irányát elfordítja a kívánt szöggel.
	* @param vec - A forgatni kívánt vektor
	* @param degrees - A forgatás szöge fokban
	* @return Megadott szöggel elforgatott vektor
	*/
	template <typename T>
	sf::Vector2<T> rotate(const sf::Vector2<T>& vec, float degrees) {
		float rad = (degrees * (float)M_PI) / 180.0f,
			s = std::sinf(rad),
			c = std::cosf(rad),
			x = static_cast<float>(vec.x),
			y = static_cast<float>(vec.y);

		float x_rot = x * c - y * s;
		float y_rot = x * s + y * c;

		return sf::Vector2<T>(
			static_cast<T>(x_rot),
			static_cast<T>(y_rot)
		);
	}

	/*
	* @brief Egy Vektor2 objektumot Vektor3 objektummá egészít ki
	* @param vec - A kiegészítendő vektor
	* @param z - A keletkező vektor 3. koordinátája (Opcionális, alapérték 0.0)
	* @return - Három dimenziós vektor
	*/
	template <typename T>
	sf::Vector3<T> cast(const sf::Vector2<T>& vec, T z = static_cast<T>(0.0)) {
		return sf::Vector3<T>(vec.x, vec.y, z);
	}

	/*
	* @brief Egy Vektor3 objektumot Vektor2 objektummá alakít az utolsó koordináta elhagyásával
	*/
	template <typename T>
	sf::Vector2<T> cast(const sf::Vector3<T>& vec) {
		return sf::Vector2<T>(vec.x, vec.y);
	}
}

