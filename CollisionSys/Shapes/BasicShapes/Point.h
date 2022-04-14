#pragma once

#include "Shapes/Bases/AbstractShape.h"

namespace CollSys {
	class Point : public AbstractShape
	{
	public:
		Point(const glib::string& type, double X = 0.0, double Y = 0.0);

		Point(const glib::string& type, const glib::vec2d& point);
	
		bool fromConsole(std::stringstream& buf) override;
	protected:
		glib::vec2d objSpaceSupport(const glib::vec2d& direction) const override;

		void write(std::ostream& stream) const override;

		void read(std::istream& stream) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
