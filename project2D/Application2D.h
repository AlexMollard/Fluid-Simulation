#pragma once
#include "GridManager.h"
#include "Application.h"
#include "Renderer2D.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Font*			_Font;
	aie::Renderer2D*	_2dRenderer;
	GridManager*		_GridManager;
};