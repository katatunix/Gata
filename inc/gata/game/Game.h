#ifndef _GAME_H_
#define _GAME_H_

#include "GameState.h"
#include "../graphic/Graphic.h"

#include "../core/CircularQueue.h"

namespace gata {
	namespace game {
//====================================================================================================
#define MAX_STATE_STACK_LENGTH	32
#define MAX_DATA_PATH_LENGTH	128
#define MAX_KEY_CODE_LENGTH		256
#define MAX_FPS_CACHES_LENGTH	24

class Game
{
public:
	typedef struct _MouseEventInfo
	{
		_MouseEventInfo() { }
		_MouseEventInfo(int _action, int _x, int _y, int _id) : action(_action), x(_x), y(_y), id(_id) { }
		int action;
		int x, y;
		int id;
	} MouseEventInfo;

	Game();
	virtual ~Game();

	virtual void init() = 0;

	//
	bool loop();

	void pause();
	void resume();

	//
	void pushState(GameState* pState);
	void popState();
	void clearState();
	GameState* currentState();

	//
	gata::graphic::Graphic* getGraphic() { return m_pGraphic; }

	//
	void resize(int width, int height);
	int width() { return m_width; }
	int height() { return m_height; }

	// Events
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode);

	void onMouseDown(int x, int y, int id);
	void onMouseHover(int x, int y, int id);
	void onMouseUp(int x, int y, int id);

	//
	bool isKeyDown(int keyCode);
	
	bool hasMouse();
	void getMouseInfo(int& action, int& x, int& y, int& id);

	//
	float fps();

	bool isInitAlready() { return m_isInitAlready; }

protected:
	void updateFps();
	void resetFps();

	//
	bool m_isPaused;
	bool m_isInitAlready;

	GameState* m_ppStateStack[MAX_STATE_STACK_LENGTH];
	int m_topStateStack;

	bool m_isMouseDowning;

	int m_width, m_height;

	gata::graphic::Graphic* m_pGraphic;

	bool m_pIsKeyDown[MAX_KEY_CODE_LENGTH];

	// todo
	//bool m_hasMouse;
	//int m_xMouse, m_yMouse, m_typeMouse;
	//
	gata::core::CircularQueue<MouseEventInfo> m_mefQueue;
	//--------------------------------------------

	unsigned long long		m_lastUpdate;
	int		m_fpsUpdateInterval;
	int		m_numFrames;
	float	m_fps;
};

//====================================================================================================
	}
}

#endif
