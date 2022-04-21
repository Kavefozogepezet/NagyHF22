#include "Circle.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"
#include "Commands/Base/Command.h"

namespace CollSys {
	using cStyle = consoleStyle;

	Circle::Circle(const glib::string& type, double radius) :
		AbstractShape(type),
		r(radius)
	{
		this->name = "circle";
		this->build();
	}

	void Circle::fromConsole(std::stringstream& buf) {
		AbstractShape::fromConsole(buf);

		if (!(buf >> this->r)) {
			throw Commands::Error("Nem adott meg sugarat");
		}
		this->build();
	}

	glib::vec2d Circle::objSpaceSupport(const glib::vec2d& dir) const {
		glib::vec2d normalized = glib::normalize(dir);
		return this->r * normalized;
	}

	void Circle::build() {
		this->shape.resize(33);
		for (size_t i = 0; i < 33; i++) {
			double angle = 2.0f * M_PI * static_cast<double>(i) / 32.0;
			glib::vec2d d(std::cos(angle), std::sin(angle));
			this->shape[i] = this->r * d;
		}
	}

	void Circle::write(std::ostream& stream) const {
		AbstractShape::write(stream);
		stream << ' ' << this->r;
	}

	void Circle::read(std::istream& stream) {
		AbstractShape::read(stream);
		stream >> this->r;
		this->build();
	}
}
