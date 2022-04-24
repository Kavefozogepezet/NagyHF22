#pragma once

#include <SFML/Graphics.hpp>

#include "Shapes/Shapes.h"

namespace CollSys {
	class GJKSolver {
	public:
		GJKSolver(ConvexShape& shape1, ConvexShape& shape2);

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

