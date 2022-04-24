#include "RegularPolygon.h"

#include "graphics/consoleStyle.h"
#include "Commands/Base/Command.h"

namespace CollSys {
	using cStyle = consoleStyle;

	CollSys::RegularPolygon::RegularPolygon(const glib::string& type) :
		Polygon(type)
	{
		this->name = "regular_polygon";
		this->build(3, 0.5);
	}

	void RegularPolygon::fromConsole(std::stringstream& buff) {
		ConvexShape::fromConsole(buff);

		size_t pc;
		double el;
		if (!(buff >> pc >> el)) {
			throw Commands::Error("Nem adott meg csucsszamot es elhosszat.");
		}
		else if (pc < 3) {
			throw Commands::Error("Minimum haromszoget adjon meg.");
		}
		else if (el <= 0.0) {
			throw Commands::Error("Az oldal hossza nem pozitiv szam volt.");
		}
		this->build(pc, el);
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
