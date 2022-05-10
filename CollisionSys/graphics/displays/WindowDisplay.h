#pragma once

#include "sfmlinclude.h"

#include "display.h"
#include "general/list.h"

namespace CollSys {
	class ConvexShape;

	class WindowDisplay : public display
	{
#ifndef CPORTA
	public:
		static double
			linear_speed,
			angular_speed;
	private:
		using ShapeList = glib::list<ConvexShape*>;
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
#else
	public:
		using display::display;
#endif // !CPORTA
	};
}
