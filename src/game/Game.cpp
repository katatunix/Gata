#include <string>

#include <gata/game/Game.h>
#include <gata/core/macro.h>
#include <gata/utils/MyUtils.h>
#include <gata/graphic/GraphicMain.h>

namespace gata {
	namespace game {
//=================================================================================================
Game::Game() :
		m_topStateStack(-1),
		m_isMouseDowning(false),
		m_isPaused(false),
		m_isInitAlready(false)
{
	LOGI("**Game::Game()");
	memset(m_pIsKeyDown, 0, sizeof(m_pIsKeyDown));

	gata::graphic::GraphicMain::init();

	m_pGraphic = gata::graphic::Graphic::create();

	resetFps();
}

Game::~Game()
{
	LOGI("**Game::~Game()");
	SAFE_DEL(m_pGraphic);
	gata::graphic::GraphicMain::uninit();
}

void Game::resize(int width, int height)
{
	if (width <= 0 || height <= 0) return;
	m_width = width;
	m_height = height;
	if ( m_pGraphic->isInit() )
	{
		m_pGraphic->resize(width, height);
	}
}

bool Game::loop()
{
	if ( !m_pGraphic->isInit() || m_isPaused )
	{
		return false;
	}

	if (!m_isInitAlready)
	{
		init();
		m_isInitAlready = true;
	}

	GameState* curState = currentState();
	assert(curState);

	updateFps();

	m_pGraphic->clear();
	curState->render();
	m_pGraphic->flush();

	return curState->update();
}

void Game::pause()
{
	GameState* curState = currentState();
	if (curState)
	{
		curState->pause();
	}
	m_isPaused = true;

	resetFps();
}

void Game::resume()
{
	GameState* curState = currentState();
	if (curState)
	{
		curState->resume();
	}
	m_isPaused = false;
}

void Game::pushState(GameState* pState)
{
	assert(m_topStateStack < MAX_STATE_STACK_LENGTH - 1);
	GameState* curState = currentState();
	if (curState)
	{
		curState->pause();
	}
	pState->create();
	m_topStateStack++;
	m_ppStateStack[m_topStateStack] = pState;
}

void Game::popState()
{
	if (m_topStateStack < 0) return;
	m_ppStateStack[m_topStateStack]->destroy();
	SAFE_DEL(m_ppStateStack[m_topStateStack]);
	m_topStateStack--;
	if (m_topStateStack >= 0)
	{
		m_ppStateStack[m_topStateStack]->resume();
	}
}

void Game::clearState()
{
	while (m_topStateStack >= 0)
	{
		popState();
	}
}

GameState* Game::currentState()
{
	if (m_topStateStack < 0) return 0;
	return m_ppStateStack[m_topStateStack];
}

// Key
void Game::onKeyDown(int keyCode)
{
	assert(0 <= keyCode && keyCode < MAX_KEY_CODE_LENGTH);
	m_pIsKeyDown[keyCode] = true;
}

void Game::onKeyUp(int keyCode)
{
	assert(0 <= keyCode && keyCode < MAX_KEY_CODE_LENGTH);
	m_pIsKeyDown[keyCode] = false;
}

bool Game::isKeyDown(int keyCode)
{
	return m_pIsKeyDown[keyCode];
}

// Mouse
void Game::onMouseDown(int x, int y, int id)
{
	m_isMouseDowning = true;
	
	m_mefQueue.put(MouseEventInfo(0, x, y, id));
}

void Game::onMouseHover(int x, int y, int id)
{
	if (m_isMouseDowning)
	{
		m_mefQueue.put(MouseEventInfo(0, x, y, id));
	}
}

void Game::onMouseUp(int x, int y, int id)
{
	m_isMouseDowning = false;
	m_mefQueue.put(MouseEventInfo(1, x, y, id));
}

float Game::fps()
{
	// todo
	return m_fps;
}

bool Game::hasMouse()
{
	return m_mefQueue.len() > 0;
}

void Game::getMouseInfo(int& action, int& x, int& y, int& id)
{
	assert(hasMouse());
	MouseEventInfo mef = m_mefQueue.get();
	action = mef.action;
	x = mef.x;
	y = mef.y;
	id = mef.id;
}

void Game::updateFps()
{
	m_numFrames++;
	unsigned long long currentUpdate = gata::utils::getCurrentMillis();
	//LOGI("currentUpdate=%d",currentUpdate);
	if( currentUpdate - m_lastUpdate > m_fpsUpdateInterval )
	{
		m_fps = (float)m_numFrames / (float)(currentUpdate - m_lastUpdate) * 1000.0f;
		m_lastUpdate = currentUpdate;
		m_numFrames = 0;
	}
}

void Game::resetFps()
{
	m_lastUpdate		= 0;
	m_fpsUpdateInterval	= 1000;
	m_numFrames			= 0;
	m_fps				= 0;
}

//=================================================================================================
	}
}
