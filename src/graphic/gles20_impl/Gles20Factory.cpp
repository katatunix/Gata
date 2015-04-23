#include "Gles20Factory.h"

#include "Gles20Image.h"
#include "Gles20Graphic.h"

namespace gata {
	namespace graphic {
//============================================================================
Image* Gles20Factory::createImage()
{
	return new Gles20Image();
}

Graphic* Gles20Factory::createGraphic()
{
	return new Gles20Graphic();
}
//============================================================================
	}
}
