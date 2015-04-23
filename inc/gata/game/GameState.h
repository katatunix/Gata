#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

namespace gata {
	namespace game {
//====================================================================================================
class GameState
{
public:
	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual bool update() = 0;
	virtual void render() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual bool isKindOf(int kind) = 0;

	virtual ~GameState() { }
};
//====================================================================================================
	}
}
#endif
