#include "Ellipse.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"
#include "Commands/Base/Command.h"

namespace CollSys {
	using cStyle = consoleStyle;

	Ellipse::Ellipse(const glib::string& type, double a, double b) :
		ConvexShape(type),
		a(a),
		b(b)
	{
		this->name = "ellipse";
		this->build();
	}

	void Ellipse::fromConsole(std::stringstream& buf) {
		ConvexShape::fromConsole(buf);

		if (!(buf >> this->a >> this->b)) {
			throw Commands::Error("Nem adott meg kis- és nagytengelyt");
		}
		this->build();
	}

	glib::vec2d Ellipse::objSpaceSupport(const glib::vec2d& dir) const {
		double a2 = this->a * this->a,
			sqr = (dir.y / dir.x) * (this->b / a2),
			x = 1.0f / std::sqrt(sqr * sqr + 1 / a2),
			y = this->b * std::sqrt(1 - (x * x) / a2);

		if (dir.x < 0) { x = -x; }
		if (dir.y < 0) { y = -y; }

		return glib::vec2d(x, y);
	}

	void Ellipse::write(std::ostream& stream) const {
		ConvexShape::write(stream);
		stream << ' ' << this->a << ' ' << this->b;
	}

	void Ellipse::read(std::istream& stream) {
		ConvexShape::read(stream);
		stream >> this->a >> this->b;
		this->build();
	}

	void Ellipse::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			double angle = 2.0f * M_PI * static_cast<double>(i) / 32.0f;
			glib::vec2d d(std::cos(angle), std::sin(angle));
			glib::vec2d p = this->objSpaceSupport(d);
			this->shape[i] = p;
		}
	}
}
