#include "RegularPolygon.h"
#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	CollSys::RegularPolygon::RegularPolygon(const glib::string& type) :
		Polygon(type)
	{
		this->name = "regular_polygon";
		this->build(3, 0.5);
	}

	bool RegularPolygon::fromConsole(std::stringstream& buff) {
		if (AbstractShape::fromConsole(buff)) {
			size_t pc;
			double el;
			if (!(buff >> pc >> el)) {
				cStyle::error() << "Rossz parameterezes" << cStyle::endl;
				return false;
			}

			if (pc < 3) {
				cStyle::error() << "Minimum haromszoget adjon meg." << cStyle::endl;
				return false;
			}

			if (el <= 0.0) {
				cStyle::error() << "Az oldal hossza nem pozitiv szam volt." << cStyle::endl;
				return false;
			}

			this->build(pc, el);
			return true;
		}
		else {
			return false;
		}
	}

	void CollSys::RegularPolygon::build(size_t pointc, double edgelen) {
		double
			deg = 360.0 / static_cast<double>(pointc),
			rad = 2 * M_PI / static_cast<double>(pointc),
			radius = edgelen / std::sin(rad / 2.0) * 2.0;
		glib::vec2d r_vec(0.0, -radius);

		this->shape.resize(pointc);

		for (auto& p : this->shape) {
			p = r_vec;
			r_vec.rotate(deg);
		}
	}
}
