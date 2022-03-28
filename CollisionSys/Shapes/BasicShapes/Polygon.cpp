#include "Polygon.h"

#include "../../debug/memtrace.h"

namespace CollSys {
	Polygon::Polygon() {
		this->name = "Polygon";
		this->build({
			{ -1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.53f, -0.4f },
			{ 0.8f, -1.74f },
			{ -1.0f, -1.0f }
		} );
	}

	Polygon::Polygon(std::initializer_list<glib::vec2d> points) {
		this->name = "Polygon";
		this->build(points);
	}

	glib::vec2d Polygon::objSpaceSupport(const glib::vec2d& dir) const {
		double max_distance = this->shape[0] * dir;
		glib::vec2d point = this->shape[0];

		for (size_t i = 1; i < this->shape.size(); i++) {
			const glib::vec2d& p = this->shape[i];
			double dot_product = p * dir;

			if (dot_product > max_distance) {
				max_distance = dot_product;
				point = p;
			}
		}
		return point;
	}

	void Polygon::build(const std::initializer_list<glib::vec2d>& points) {
		this->shape.resize(points.size());

		size_t i = 0;
		for (auto & point : points) {
			this->shape[i++] = point;
		}

		/*
		this->shape.resize(6);

		this->shape[0] = glib::vec2d(-1.0f, 1.0f);
		this->shape[1] = glib::vec2d(1.0f, 1.0f);
		this->shape[2] = glib::vec2d(1.53f, -0.4f);
		this->shape[3] = glib::vec2d(0.8f, -1.74f);
		this->shape[4] = glib::vec2d(-1.0f, -1.0f);
		this->shape[5] = glib::vec2d(-1.0f, 1.0f);
		*/
	}
}