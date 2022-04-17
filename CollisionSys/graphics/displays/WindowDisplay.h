#pragma once

#include <SFML/Graphics.hpp>

#include "display.h"
#include "general/list.h"

namespace CollSys {
	class AbstractShape;

	class WindowDisplay : public display
	{
	public:
		static double
			linear_speed,
			angular_speed;
	private:
		using ShapeList = glib::list<AbstractShape*>;
	public:
		WindowDisplay(Sandbox& parent, bool active = false);
	protected:
		void onActivation() override;
		void onDeactivation() override;
		void onUpdate() override;
	private:
		ShapeList::forwardIt selected;
		sf::RenderWindow window;

		void setView();

		void colorShapes();

		void handleEvents();
		void render();
	};
}
