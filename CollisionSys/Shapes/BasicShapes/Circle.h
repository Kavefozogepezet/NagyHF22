#pragma once

#include "Shapes/Bases/ConvexShape.h"

namespace CollSys {
	class Circle : public ConvexShape
	{
	public:
		Circle(const glib::string& type, double radius = 0.5f);
	
		void fromConsole(std::stringstream& buf) override;
	private:
		double r;

		/** @brief Feltölti a síkidom körvonalának listáját a kört közelítő pontokkal. */
		void build();
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;
	};
}
