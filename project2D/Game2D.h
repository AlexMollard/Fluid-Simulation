#pragma once
#include "Game.h"
#include "Renderer2D.h"
#include "CellManager.h"

class Game2D : public aie::Game
{
public:
	Game2D(const char* title, int width, int height, bool fullscreen);
	virtual ~Game2D();

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	aie::Renderer2D*	_2dRenderer;
	aie::Font*			_font;
	CellManager*		_CellManager;
};