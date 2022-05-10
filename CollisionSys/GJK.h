#pragma once

#include "Shapes/Shapes.h"

namespace CollSys {
	/** @brief Az osztály a GJK algoritmust futtatva két megadott síkidomra eldönti hogy a síkidomok érintkeznek e. */
	class GJKSolver {
	public:
		GJKSolver(ConvexShape& shape1, ConvexShape& shape2);

		/*
		* @returns true: A két síkidom érintkezik; false: A két síkidom nem érintkezik
		*/
		bool isContact();
	private:
		ConvexShape& s1, & s2;
		bool is_contact;
	private:
		glib::VertexArray simplex;

		bool getSupportPoint(const glib::vec2d& direction, glib::vec2d& point);

		bool findFirstSimplex();
		bool findNextPoint(glib::vec2d& point);

		void checkOverlap();
	};
}

