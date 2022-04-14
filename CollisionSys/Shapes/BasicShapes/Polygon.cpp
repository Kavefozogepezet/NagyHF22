#include "Polygon.h"

#include "debug/memtrace.h"
#include "graphics/consoleStyle.h"

namespace CollSys {
	using cStyle = consoleStyle;

	Polygon::Polygon(const glib::string& type) :
		AbstractShape(type)
	{
		this->name = "Polygon";
		this->build({
			{ -1.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.53f, -0.4f },
			{ 0.8f, -1.74f },
			{ -1.0f, -1.0f }
		} );
	}

	Polygon::Polygon(const glib::string& type, std::initializer_list<glib::vec2d> points) :
		AbstractShape(type)
	{
		this->name = "Polygon";
		this->build(points);
	}

	bool Polygon::fromConsole(std::stringstream& buf) {
		if (!AbstractShape::fromConsole(buf)) {
			return false;
		}
		glib::VertexList vlist;
		glib::vec2d temp;

		while (buf >> temp) {
			vlist.push_back(temp);
		}
		// TODO convex check

		if (vlist.size() < 3) {
			cStyle::error() << "Minimum haromszoget adjon meg." << cStyle::endl;
			return false;
		}

		this->shape.resize(vlist.size());
		auto itl = vlist.begin();
		for (
			auto its = this->shape.begin();
			its != this->shape.end();
			its++, itl++) {
			(*its) = (*itl);
		}
		return true;
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

	void Polygon::write(std::ostream& stream) const {
		AbstractShape::write(stream);
		stream << ' ' << this->shape.size();
		for (auto& p : this->shape) {
			stream << ' ' << p;
		}
	}

	void Polygon::read(std::istream& stream) {
		AbstractShape::read(stream);
		size_t size;
		stream >> size;
		this->shape.resize(size);
		for (auto& p : this->shape) {
			stream >> p;
		}
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
