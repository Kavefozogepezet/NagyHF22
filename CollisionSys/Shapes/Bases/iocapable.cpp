#include "iocapable.h"

namespace CollSys {
	std::ostream& operator<<(std::ostream& stream, const IOCapable& obj) {
		obj.write(stream);
		return stream;
	}

	std::istream& operator>>(std::istream& stream, IOCapable& obj) {
		obj.read(stream);
		return stream;
	}
}