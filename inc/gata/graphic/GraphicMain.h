//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : GraphicMain.h
//  @ Date : 6/15/2012
//  @ Author : 
//
//


#if !defined(_GRAPHICMAIN_H)
#define _GRAPHICMAIN_H

namespace gata {
	namespace graphic {
//========================================================================
class Factory;

class GraphicMain
{
public:
	static void init();
	static void uninit();

	static Factory* getFactory();

private:
	static Factory* m_spFactory;
	static bool m_sIsInitOk;
};
//========================================================================
	}
}

#endif  //_GRAPHICMAIN_H
