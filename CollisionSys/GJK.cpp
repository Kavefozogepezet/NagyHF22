#include "GJK.h"

#include "debug/memtrace.h"

namespace CollSys {
	GJKSolver::GJKSolver(AbstractShape& shape1, AbstractShape& shape2) :
		s1(shape1),
		s2(shape2),
		is_contact(false),
		simplex(2)
	{
		this->checkOverlap();
	}

	bool GJKSolver::isContact() { return this->is_contact; }

	bool GJKSolver::getSupportPoint(const glib::vec2d& direction, glib::vec2d& point) {
		point = s1.support(direction) - s2.support(-direction);
		return point * direction > 0.0;
	}

	bool GJKSolver::findFirstSimplex() {
		glib::vec2d direction = this->s2.getPosition() - this->s1.getPosition();
		if (direction.length() != 0.0) { direction.normalize(); }
		else { direction = { 1.0, 0.0 }; }

		if (!this->getSupportPoint(direction, simplex[0])) {
			return false;
		}
		direction = -this->simplex[0];
		if (!this->getSupportPoint(direction, simplex[1])) {
			return false;
		}
		return true;
	}

	bool GJKSolver::findNextPoint(glib::vec2d& point) {
		glib::vec2d
			edge = this->simplex[0] - this->simplex[1],
			to_origo = -this->simplex[1],
			normal = glib::getNormal(edge, to_origo);
		return this->getSupportPoint(normal, point);
	}

	void GJKSolver::checkOverlap() {
		if (!this->findFirstSimplex()) { return; }

		glib::vec2d next_point;
		bool foundTriangle = false, foundValidPoint = true;

		do {
			foundValidPoint = this->findNextPoint(next_point);

			if (foundValidPoint) {
				glib::vec2d
					normal1 = glib::getNormal(
						this->simplex[0] - next_point,
						next_point - this->simplex[1]
					),
					normal2 = glib::getNormal(
						this->simplex[1] - next_point,
						next_point - this->simplex[0]
					),
					np_to_origin = -next_point;

				if (normal1 * np_to_origin > 0.0) {
					this->simplex[1] = next_point;
				}
				else if (normal2 * np_to_origin > 0.0) {
					this->simplex[0] = next_point;
				}
				else {
					foundTriangle = true;
				}
			}
		} while (foundValidPoint && !foundTriangle);

		this->is_contact = foundTriangle;
	}
}

