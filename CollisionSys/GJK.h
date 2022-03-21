#pragma once

#include <SFML/Graphics.hpp>

#include "Shapes/Shapes.h"

namespace CollSys {
	/*
	* @brief Két síkidom érintkezését leíró osztály.
	*/
	class Contact {
		friend class GJKSolver;
	public:
		Contact(
			AbstractShape& shape1,
			AbstractShape& shape2,
			const glib::vec2d& contactPoint,
			const glib::vec2d& contactnormal
		);

		Contact(
			AbstractShape& shape1,
			AbstractShape& shape2
		);

		Contact(const Contact& other);

		/*
		* @brief Egy Collision típusú objektumot 0-1-el indexelve visszatér az egyik résztvevő síkidommal.
		* @return [1]: actor1; [2]: actor2
		*/
		const AbstractShape& operator [] (size_t idx);
	private:
		AbstractShape& actor1, &actor2;
		glib::vec2d point, normal;
	};

	class GJKSolver {
	public:
		GJKSolver(AbstractShape& shape1, AbstractShape& shape2);

		bool isContact();
		const Contact& getContact();

	private:
		AbstractShape& s1, & s2;

		bool is_contact;
		Contact contact;
	private:
		glib::VertexArray simplex;
		glib::VertexList polytope;

		bool getSupportPoint(const glib::vec2d& direction, glib::vec2d& point);

		bool findFirstSimplex();
		bool findNextPoint(glib::vec2d& point);

		void checkOverlap();
		void calcContact();
	};

	/*
	* @brief Az összes paraméterként kapott síkidomra teszteli, hogy azok érintkeznek e.
	* @params shapes - A síkidomok listája
	* @param n - A síkidomok száma
	* @todo - Rendes listát írni, azt használni paraméterként
	*/
	void checkCollisions(AbstractShape* shapes, size_t n);

	/*
	* @brief Megvizsgálja hogy két síkidom átfedi e egymást
	* @param shape1 - Az egyik síkidom
	* @param shape2 - A másik síkidom
	* @return true: átfedik egymást; false:nem fedik át egymást
	*/
	bool checkOverlapping(const AbstractShape& shape1, const AbstractShape& shape2);

	/*
	* @brief Megvizsgálja hogy két síkidom átfedi e egymást, ha igen, kiszámolja az érintkezés pontját
	* @param shape1 - Az egyik síkidom
	* @param shape2 - A másik síkidom
	* @param collData - Ha 
	* @return true: true: a síkidomok érintkeznek; false:nem érintkeznek
	*/
	bool checkCollision(const AbstractShape& shape1, const AbstractShape& shape2, Contact& collData);
}

