#ifndef _GLES20_FACTORY_H_
#define _GLES20_FACTORY_H_

#include <gata/graphic/Factory.h>

namespace gata {
	namespace graphic {
//======================================================================================
class Gles20Factory : public Factory
{
public:
	Image* createImage();
	Graphic* createGraphic();
};
//======================================================================================
	}
}

#endif
