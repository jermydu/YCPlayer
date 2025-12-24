#include "YCAV.h"
#include "YCAVClassPrivate.h"

namespace YCLIB {
	YCAVStream::YCAVStream():imp(new Imp) {
	}

	YCAVStream::~YCAVStream() {
		if (imp) {
			delete imp;
			imp = nullptr;
		}
	}
}
