#include "BezierCurve.h"

//#define SUP_TEST

namespace CollSys {
	BezierCurve::BezierCurve(const glib::string& type) :
		AbstractShape(type),
		segments()
	{
		this->name = "Bezier";
		
		glib::vec2d l[10] = {
		{-1, 1},
		{-0.5, 1},

		{0, 0.9},
		{0, 0},
		{0, -1.3},

		{-0.9, -1.6},
		{-1, -1},
		{-1.1, -0.4},

		{-1.18, 0.88},
		{-1, 1}
		};
		
		this->segments.resize(3);

		for (size_t i = 0; i < this->segments.size(); i++) {
			glib::vec2d* p = l + i * 3;

			glib::vec2d
				a = p[3] + 3.0 * p[1] - 3.0 * p[2] - p[0],
				b = 3.0 * p[2] - 6.0 * p[1] + 3.0 * p[0],
				c = 3.0 * p[1] - 3.0 * p[0];
			this->segments[i] = { a, b, c, p[0] };
		}
		this->build();
	}

	glib::vec2d BezierCurve::getPoint(const Segment& segment, double t) {
		return
			t * t * t * segment.a +
			t * t * segment.b +
			t * segment.c + segment.d;
	}

	glib::vec2d BezierCurve::objSpaceSupport(const glib::vec2d& dir) const {
		glib::vec2d furthest = BezierCurve::getPoint(this->segments[0], 0.0);

		for (auto& seg : this->segments) {
			// a görbe egyenletének deriválthának zérushelyeit számoljuk ki
			// ezek az egyenlet együtthatói
			double
				eq_b = 2.0 * seg.b * dir,
				eq_a = 3.0 * seg.a * dir,
				eq_c = seg.c * dir,

				eq_sqrt = sqrt(eq_b * eq_b - 4 * eq_a * eq_c),
				eq_res;

			if (eq_a) { // az egyenlet másodfokú
				eq_res = (-eq_b - eq_sqrt) / (2 * eq_a);
			}
			else if(eq_b) { // elsőfokó
				eq_res = (seg.c * dir) / (2.0 * seg.b * dir);
			}
			else if(!eq_c) { // nulladfokú, de 0 = 0 => a szegmens összes pontja megfelelő lenne
				eq_res = 0.0; // vegyük a szegmens elejét
			}
			else { // nincs megoldás
				continue;
			}
			/*
			glib::vec2d p = BezierCurve::getPoint(seg, eq_res);
			if (eq_res >= 0.0 && eq_res <= 1.0) {
				furthest = p;
				break;
			}
			*/
			
			eq_res = std::clamp(eq_res, 0.0, 1.0);
			//std::cout << "t: " << eq_res;
			glib::vec2d p = BezierCurve::getPoint(seg, eq_res);

			if(p * dir > furthest * dir) {
				furthest = p;
				//std::cout << " leghosszabb " << p << " " << p * dir;
			}
			//std::cout << std::endl;
			
		}
		return furthest;
	}

	void BezierCurve::write(std::ostream& stream) const {
		AbstractShape::write(stream);
		stream << ' ' << this->segments.size();
		for (auto& seg : segments) {
			stream << ' ' << seg.a << ' ' << seg.b << ' ' << seg.c << ' ' << seg.d;
		}
	}

	void BezierCurve::read(std::istream& stream) {
		AbstractShape::read(stream);
		size_t segc;
		stream >> segc;
		this->segments.resize(segc);
		for (auto& seg : this->segments) {
			stream >> seg.a >> seg.b >> seg.c >> seg.d;
		}
	}

	void BezierCurve::build() {
#ifndef SUP_TEST 
		const size_t res = 16;
		this->shape.resize(this->segments.size() * res);

		size_t segc = 0;
		for (auto& seg : this->segments) {
			for (size_t i = 0; i < res; i++) {
				double t = static_cast<double>(i) / static_cast<double>(res);
				this->shape[segc * res + i] = BezierCurve::getPoint(seg, t);
			}
			segc++;
		}
#else
		const size_t res = 64;
		this->shape.resize(res);
		double angle = 360.0 / static_cast<double>(res);
		glib::vec2d dir(1.0, 0.0);

		for (size_t i = 0; i < res; i++) {
			this->shape[i] = this->objSpaceSupport(dir);
			//std::cout << this->shape[i];
			dir.rotate(angle);
		}
		std::cout << std::endl;
#endif
	}
}

#undef SUP_TEST
