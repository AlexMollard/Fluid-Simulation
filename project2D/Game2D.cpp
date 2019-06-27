#include "Game2D.h"
#include "Application.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	_2dRenderer = new aie::Renderer2D();

	_font = new aie::Font("./font/consolas.ttf", 24);

	_CellManager = new CellManager(30);

	aie::Application* application = aie::Application::GetInstance();
	application->SetVSync(false);
	application->SetBackgroundColour(0.1f,0.1f,0.1f);

}

Game2D::~Game2D()
{
	// Delete Font
	delete _font;

	// Delete the renderer.
	delete _2dRenderer;

	// Delete CellManager
	delete _CellManager;
	_CellManager = nullptr;
}

void Game2D::Update(float deltaTime)
{
	// Input example: Update the camera position using the arrow keys.
	aie::Input* input = aie::Input::GetInstance();

	_CellManager->Update(deltaTime);

	// Exit the application if escape is pressed.
	if (input->IsKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		aie::Application* application = aie::Application::GetInstance();
		application->Quit();
	}
}

void Game2D::Draw()
{
	aie::Application* application = aie::Application::GetInstance();
	float time = application->GetTime();

	// Wipe the screen to clear away the previous frame.
	application->ClearScreen();

	// Prepare the renderer. This must be called before any sprites are drawn.
	_2dRenderer->Begin();

	_CellManager->Draw(_2dRenderer);

	// Draw some text.
	_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f);
	float windowHeight = (float)application->GetWindowHeight();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	_2dRenderer->DrawText2D(_font, fps, 15.0f, windowHeight - 32.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	_2dRenderer->End();
}