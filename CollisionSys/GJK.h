#pragma once

#include <SFML/Graphics.hpp>

#include "Shapes/AbstractShape.h"

namespace CollSys::GJK {
	/*
	* @brief Két síkidom érintkezését leíró osztály.
	*/
	class Collision {
	public:
		const sf::Vector2f point;
		const sf::Vector2f normal;

		Collision(
			const AbstractShape& shape1,
			const AbstractShape& shape2,
			sf::Vector2f contactPoint = sf::Vector2f(0.0f, 0.0f),
			sf::Vector2f normal = sf::Vector2f(1.0f, 0.0f)
		);

		/*
		* @brief Egy Collision típusú objektumot 0-1-el indexelve visszatér az egyik résztvevő síkidommal.
		* @return [1]: actor1; [2]: actor2
		*/
		const AbstractShape& operator [] (size_t idx);
	private:
		const AbstractShape& actor1, &actor2;
	};

	/*
	* @brief Az összes paraméterként kapott síkidomra teszteli, hogy azok érintkeznek e.
	* @params shapes - A síkidomok listája
	* @param n - A síkidomok száma
	* @todo - Rendes listát írni, azt használni paraméterként
	*/
	void checkCollisions(AbstractShape* shapes, size_t n);

	/*
	* @brief Megvizsgálja hogy két síkidom befolyási köre metszi e egymást.
	* @param shape1 - Az egyik síkidom
	* @param shape2 - A másik síkidom
	* @return true: a körök metszik egymást; false: nem metszik egymást
	*/
	bool checkInfluenceRange(const AbstractShape& shape1, const AbstractShape& shape2);

	/*
	* @brief Megvizsgálja hogy két síkidom átfedi e egymást
	* @param shape1 - Az egyik síkidom
	* @param shape2 - A másik síkidom
	* @return true: átfedik egymást; false:nem fedik át egymást
	*/
	bool checkOverlapping(const AbstractShape& shape1, const AbstractShape& shape2);

	/*
	* @brief Megvizsgálja hogy két síkidom átfedi e egymást
	* @param shape1 - Az egyik síkidom
	* @param shape2 - A másik síkidom
	* @return true: 
	*/
	const Collision checkCollision(const AbstractShape& shape1, const AbstractShape& shape2);
}

