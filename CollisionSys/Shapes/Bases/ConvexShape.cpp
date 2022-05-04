#include "ConvexShape.h"

namespace CollSys {
	ConvexShape::ConvexShape(const glib::string& type) :
		my_type(type)
	{}

	ConvexShape::~ConvexShape() {}

	glib::vec2d ConvexShape::support(const glib::vec2d& direction) const {
		if (this->getScale() == glib::vec2d(0.0, 0.0)) {
			return this->getPosition();
		}
		glib::vec2d objSpaceDir = direction;
		objSpaceDir.rotate(-this->getRotation()).scale(this->getScale());
		glib::vec2d sup = this->objSpaceSupport(objSpaceDir);
		sup.scale(this->getScale()).rotate(this->getRotation());
		sup += this->getPosition();
		return sup;
		//return this->getTransform() * this->objSpaceSupport(objSpaceDir);
	}

	const glib::string& ConvexShape::getType() {
		return this->my_type;
	}

	void ConvexShape::write(std::ostream& stream) const {
		Shape::write(stream);
		stream << ' ' << this->name;
	}

	void ConvexShape::read(std::istream& stream) {
		Shape::read(stream);
		stream >> this->name;
	}
}
