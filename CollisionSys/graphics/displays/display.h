#pragma once

namespace CollSys {
	class Sandbox;

	class display
	{
	public:
		display(Sandbox& parent, bool active = false) :
			m_parent(parent),
			m_active(false)
		{
			this->makeActive(active);
		}

		void update() {
			if (this->m_active) {
				this->onUpdate();
			}
		}

		void makeActive(bool active) {
			if (this->m_active && !active) {
				this->onDeactivation();
			}
			else if (!this->m_active && active) {
				this->onActivation();
			}
			this->m_active = active;
		}

		bool isActive() {
			return this->m_active;
		}
	protected:
		Sandbox& m_parent;

		virtual void onActivation() {};
		virtual void onDeactivation() {};
		virtual void onUpdate() {};
	private:
		bool m_active;
	};
}