#pragma once

namespace CollSys {
	class Sandbox;

	class display
	{
	public:
		display(Sandbox& parent, bool active = false) :
			_parent(parent),
			_active(false)
		{
			this->makeActive(active);
		}

		void update() {
			if (this->_active) {
				this->onUpdate();
			}
		}

		void makeActive(bool active) {
			if (this->_active && !active) {
				this->onDeactivation();
			}
			else if (!this->_active && active) {
				this->onActivation();
			}
			this->_active = active;
		}

		bool isActive() {
			return this->_active;
		}
	protected:
		Sandbox& _parent;

		virtual void onActivation() {};
		virtual void onDeactivation() {};
		virtual void onUpdate() {};
	private:
		bool _active;
	};
}