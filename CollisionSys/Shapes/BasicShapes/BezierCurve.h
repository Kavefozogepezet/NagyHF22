#pragma once

#include "Shapes/Bases/ConvexShape.h"
#include "general/vec2.h"
#include "general/array.h"

namespace CollSys {
	class BezierCurve : public ConvexShape
	{
	public:
		/** @brief Egy bezier görbe egy szegmensét leíró osztály. */
		struct Segment {
			glib::vec2d a, b, c, d;
			Segment(glib::vec2d p0, glib::vec2d p1, glib::vec2d p2, glib::vec2d p3);
		};
		using SegmentArray = glib::array<Segment>;
	public:
		BezierCurve(const glib::string& type);

		void fromConsole(std::stringstream& buf);
	private:
		SegmentArray segments;

		/*
		* @brief Megadja egy bezier szegmens pontját
		* @param segment - A szegmens, amelyen a pontot keressük
		* @param t - A pont helye a szegmensen; 0.0 <= t <= 1.0
		* @returns A szegmens pontjának koordinátája
		*/
		glib::vec2d getPoint(const Segment& segment, double t) const;
		
		/** @brief Feltölti a síkidom körvonalának listáját a szegmenseget közelítő pontokkal. */
		void build();
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

	};
}
