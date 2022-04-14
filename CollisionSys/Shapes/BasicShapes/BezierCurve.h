#pragma once

#include "Shapes/Bases/AbstractShape.h"
#include "general/vec2.h"
#include "general/array.h"

namespace CollSys {
	class BezierCurve : public AbstractShape
	{
	public:
		struct Segment {
			glib::vec2d a, b, c, d;
			Segment(glib::vec2d p0, glib::vec2d p1, glib::vec2d p2, glib::vec2d p3);
		};
		using SegmentArray = glib::array<Segment>;
	public:
		BezierCurve(const glib::string& type);

		bool fromConsole(std::stringstream& buf);
	private:
		static glib::vec2d getPoint(const Segment& segment, double t);
	private:
		SegmentArray segments;

		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		void build();
	};
}
