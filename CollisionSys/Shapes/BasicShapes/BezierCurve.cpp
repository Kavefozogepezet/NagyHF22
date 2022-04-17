#include "BezierCurve.h"

#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	BezierCurve::Segment::Segment(glib::vec2d p0, glib::vec2d p1, glib::vec2d p2, glib::vec2d p3) :
		a(p3 + 3.0 * p1 - 3.0 * p2 - p0),
		b(3.0 * p2 - 6.0 * p1 + 3.0 * p0),
		c(3.0 * p1 - 3.0 * p0),
		d(p0)
	{}

	BezierCurve::BezierCurve(const glib::string& type) :
		AbstractShape(type),
		segments(1)
	{
		this->name = "bezier";
		
		glib::vec2d l[4] = {
			{ -1,  0 },
			{  0, -1 },
			{  0,  1 },
			{ -1,  0 }
		};

		for (size_t i = 0; i < this->segments.size(); i++) {
			glib::vec2d* p = l + i * 3;
			this->segments[i] = Segment(p[0], p[1], p[2], p[3]);
		}
		this->build();
	}

	bool BezierCurve::fromConsole(std::stringstream& buf) {
		if (!AbstractShape::fromConsole(buf)) {
			return false;
		}
		//return true;
		glib::VertexList vlist;
		glib::vec2d temp1, temp2, temp3;

		while (buf >> temp1 >> temp2 >> temp3) {
			vlist.push_back(temp1);
			vlist.push_back(temp2);
			vlist.push_back(temp3);
		}
		// TODO convex check

		size_t
			segc = vlist.size() / 3,
			segidx = 0;

		if (segc < 1) {
			cStyle::error() << "Legalabb egy szegmenst adjon meg." << cStyle::endl;
			return false;
		}

		this->segments.resize(segc);
		vlist.push_back(*vlist.begin());

		for (auto it = vlist.begin(); it != vlist.end(); it++) {
			glib::vec2d&
				a = *(it++), b = *(it++),
				c = *(it++), d = *it;
			this->segments[segidx++] = Segment(a, b, c, d); // szegmens egymás utáni 4 vektorból
		}
		this->build();

		return true;
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
			// a görbe egyenletének deriváltjának zérushelyeit számoljuk ki
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
			else if (eq_b) { // elsőfokó
				eq_res = -(seg.c * dir) / (2.0 * seg.b * dir);
			}
			else if (!eq_c) { // nulladfokú, de 0 = 0 => a szegmens összes pontja megfelelő lenne
				eq_res = 0.0; // vegyük a szegmens elejét
			}
			else { // nincs megoldás ezen a szegmensen
				continue;
			}
			eq_res = std::clamp(eq_res, 0.0, 1.0);
			glib::vec2d p = BezierCurve::getPoint(seg, eq_res);

			if (p * dir > furthest * dir) {
				furthest = p;
			}
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
		this->build();
	}

	void BezierCurve::build() {
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
	}
}
