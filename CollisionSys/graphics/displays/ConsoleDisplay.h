#pragma once

#include "display.h"

namespace CollSys {
	class ConsoleDisplay : public display
	{
	public:
		using display::display;
	protected:
		void onUpdate() override;
	};
}
