//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Factory.h
//  @ Date : 6/15/2012
//  @ Author : 
//
//

#if !defined(_FACTORY_H)
#define _FACTORY_H

namespace gata {
	namespace graphic {
//========================================================================
class Image;
class Graphic;

class Factory
{
public:
	virtual Image* createImage() = 0;
	virtual Graphic* createGraphic() = 0;
};
//========================================================================
	}
}

#endif  //_FACTORY_H
