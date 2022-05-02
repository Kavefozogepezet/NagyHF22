#pragma once

#pragma once

#include "Shapes/Bases/ConvexShape.h"

namespace CollSys {
	class Ellipse : public ConvexShape
	{
	public:
		Ellipse(const glib::string& type, double a = 0.5f, double b = 0.25f);
	
		void fromConsole(std::stringstream& buf) override;
	private:
		double a, b;

		/** @brief Feltölti a síkidom körvonalának listáját az ellipszist közelítő pontokkal. */
		void build();
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;
	};
}
