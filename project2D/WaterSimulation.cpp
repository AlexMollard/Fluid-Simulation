#include "WaterSimulation.h"
#include "Application.h"
#include <algorithm>
#include "Input.h"
using namespace std;
WaterSimulation::WaterSimulation(int _Size)
{
	_GridSize = _Size;

	_Font = new aie::Font("./font/consolas.ttf", 24);

	// Create Cells
	_Cell = new int *[_GridSize + 2];
	_Mass = new float* [_GridSize + 2];
	_NewMass = new float* [_GridSize + 2];
	_Falling = new bool* [_GridSize + 2];
	for (int i = 0; i < _GridSize + 2; i++)
	{
		_Cell[i] = new int [_GridSize + 2];
		_Mass[i] =  new float[_GridSize + 2];
		_NewMass[i] = new float [_GridSize + 2];
		_Falling[i] = new bool [_GridSize + 2];
	}

	//Set all cells to empty
	for (int x = 1; x < _GridSize; x++)
	{
		for (int y = 1; y < _GridSize; y++)
		{
			_Cell[x][y] = _Air;
			_Mass[x][y] = 0.0f;
			_NewMass[x][y] = 0.0f;
			_Falling[x][y] = false;
		}
	}
	
	// Set Cell sizes for window size
	Resize();
	_Timer = 0.0f;
}

WaterSimulation::~WaterSimulation()
{
	for (int x = 0; x < _GridSize + 2; x++)
	{
		delete[] _NewMass[x];
		delete[] _Mass[x];
		delete[] _Cell[x];
		delete[] _Falling[x];
	}

	delete[] _NewMass;
	delete[] _Mass;
	delete[] _Cell;
	delete[] _Falling;

	delete _Font;
}

void WaterSimulation::Update(float deltaTime)
{
	_Timer += 5.0f * deltaTime;
	aie::Application* application = aie::Application::GetInstance();

	if (_WindowSizeX != application->GetWindowWidth() || _WindowSizeY != application->GetWindowHeight())
	{
		// Resize grid
		Resize();
	}

	//Check for user input
	CheckInput(deltaTime);
	
	if (_Timer > 0.2f)
	{
		//Check Cells
		CheckNeighbours(deltaTime);
		_Timer = 0;
	}
}

float WaterSimulation::constrain(float x, float a, float b) {
	if (x < a) {
		return a;
	}
	else if (b < x) {
		return b;
	}
	else
		return x;
}

float WaterSimulation::GetStableWater(float _TotalMass)
{
	if (_TotalMass <= 1.0f)
		return 1.0f;
	else if (_TotalMass < 2.0f * _MaxMass + _MaxCompression)
		return (_MaxMass * _MaxMass + _TotalMass * _MaxCompression) / (_MaxMass + _MaxCompression);
	else
		return (_TotalMass + _MaxCompression) / 2.0f;
}

void WaterSimulation::CheckNeighbours(float deltaTime)
{
	float _Flow = 0.0f;
	float _RemainingMass;

	for (int x = 1; x < _GridSize; x++)
	{
		for (int y = 1; y < _GridSize; y++)
		{
			if (_Cell[x][y] == _Ground)
				continue;
			
			//if (_Cell[x][y + 1] != _Ground && _Cell[x][y - 1] == _Air)
			//	_Falling[x][y] = true;
			//else
			//	_Falling[x][y] = false;

			_Flow = 0.0f;
			_RemainingMass = _Mass[x][y];

			if (_RemainingMass <= 0.0f)
				continue;

			if (_Cell[x][y - 1] != _Ground)
			{
				_Flow = GetStableWater(_RemainingMass + _Mass[x][y - 1]) - _Mass[x][y - 1];

				if (_Flow > _MinFlow)
					_Flow *= 0.5f;

				constrain(_Flow, 0, min(_MaxSpeed, _RemainingMass));
				
				_NewMass[x][y] -= _Flow;
				_NewMass[x][y-1] += _Flow;
				_RemainingMass -= _Flow;
			}
			
			if (_RemainingMass <= 0.0f)
				continue;

			//Left
			if (_Cell[x - 1][y] != _Ground) {
				_Flow = (_Mass[x][y] - _Mass[x - 1][y]) / 4;
				if (_Flow > _MinFlow) { _Flow *= 0.5; }
				_Flow = constrain(_Flow, 0, _RemainingMass);

				_NewMass[x][y] -= _Flow;
				_NewMass[x - 1][y] += _Flow;
				_RemainingMass -= _Flow;
			}

			if (_RemainingMass <= 0) continue;

			//Right
			if (_Cell[x + 1][y] != _Ground) {
				_Flow = (_Mass[x][y] - _Mass[x + 1][y]) / 4;
				if (_Flow > _MinFlow) { _Flow *= 0.5; }
				_Flow = constrain(_Flow, 0, _RemainingMass);

				_NewMass[x][y] -= _Flow;
				_NewMass[x + 1][y] += _Flow;
				_RemainingMass -= _Flow;
			}

			if (_RemainingMass <= 0) continue;

			//Up
			if (_Cell[x][y + 1] != _Ground) {
				_Flow = _RemainingMass - GetStableWater(_RemainingMass + _Cell[x][y + 1]);
				if (_Flow > _MinFlow) { _Flow *= 0.5; }
				_Flow = constrain(_Flow, 0, min(_MaxSpeed, _RemainingMass));

				_NewMass[x][y] -= _Flow;
				_NewMass[x][y + 1] += _Flow;
				_RemainingMass -= _Flow;
			}


		}
	}

	//Copy the new mass values to the mass array
	for (int x = 0; x < _GridSize + 2; x++) {
		for (int y = 0; y < _GridSize + 2; y++) {
			_Mass[x][y] = _NewMass[x][y];
		}
	}

	for (int x = 1; x <= _GridSize; x++) {
		for (int y = 1; y <= _GridSize; y++) {
			
			if (_Cell[x][y] == _Ground) 
				continue;

			if (_Mass[x][y] > _MinMass)
				_Cell[x][y] = _Water;
			else
				_Cell[x][y] = _Air;
		}
	}

	//Remove any water that has left the map
	for (int x = 0; x < _GridSize + 2; x++) {
		_Mass[x][0] = 0;
		_Mass[x][_GridSize + 1] = 0;
	}
	for (int y = 1; y < _GridSize + 1; y++) {
		_Mass[0][y] = 0;
		_Mass[_GridSize + 1][y] = 0;
	}

}

void WaterSimulation::Resize()
{
	aie::Application* application = aie::Application::GetInstance();
	_WindowSizeX = application->GetWindowWidth();
	_WindowSizeY = application->GetWindowHeight();

	// Cell size
	_CellSizeX = (_WindowSizeX * 0.9f) / _GridSize;
	_CellSizeY = (_WindowSizeY * 0.9f) / _GridSize;
}

void WaterSimulation::CheckInput(float deltaTime)
{
	aie::Input* input = aie::Input::GetInstance();

	if (input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		for (int x = 1; x < _GridSize; x++)
		{
			for (int y = 1; y < _GridSize; y++)
			{
				if (input->GetMouseX() >= (x * _CellSizeX) + (_WindowSizeX * 0.04f) && input->GetMouseX() <= (x * _CellSizeX) + (_WindowSizeX * 0.04f) + (_CellSizeX) &&
					input->GetMouseY() >= (y * _CellSizeY) + (_WindowSizeY * 0.04f) && input->GetMouseY() <= (y * _CellSizeY) + (_WindowSizeY * 0.04f) + (_CellSizeY))
				{
					if (input->IsKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
						_Cell[x][y] = _Air;
					else
						_Cell[x][y] = _Ground;

					_NewMass[x][y] = 0.0f;
				}
			}
		}
	}
	else if (input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		for (int x = 1; x < _GridSize; x++)
		{
			for (int y = 1; y < _GridSize; y++)
			{
				if (input->GetMouseX() >= (x * _CellSizeX) + (_WindowSizeX * 0.04f) && input->GetMouseX() <= (x * _CellSizeX) + (_WindowSizeX * 0.04f) + (_CellSizeX) &&
					input->GetMouseY() >= (y * _CellSizeY) + (_WindowSizeY * 0.04f) && input->GetMouseY() <= (y * _CellSizeY) + (_WindowSizeY * 0.04f) + (_CellSizeY))
				{
					if (input->IsKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
					{
						_NewMass[x][y] = 0.0f;
						_Cell[x][y] = _Air;
					}
					else
					{
						_Cell[x][y] = _Water;
						_NewMass[x][y] += 50.0f * deltaTime;
					}
				}
			}
		}
	}

}

void WaterSimulation::Draw(aie::Renderer2D* _Renderer)
{
	for (int x = 1; x < _GridSize; x++)
	{
		for (int y = 1; y < _GridSize; y++)
		{
			if (_Cell[x][y] == _Ground)
			{
				_Renderer->SetRenderColour(0.2f, 0.2f, 0.2f);
				_Renderer->DrawSprite(nullptr, (x * _CellSizeX) + (_WindowSizeX * 0.04f), (y * _CellSizeY) + (_WindowSizeY * 0.04f), _CellSizeX, _CellSizeY, 0.0f, 0.0f, 0.0f, 0.0f);
			}
			else if (_Cell[x][y] == _Water)
			{
				_Renderer->SetRenderColour(-_Mass[x][y] / 5 + 0.4f, -_Mass[x][y] / 5 + 0.4f, -_Mass[x][y] / 5 + 0.9f);

				if (_Mass[x][y] < _MaxMass && _Falling[x][y] == false)
					_Renderer->DrawSprite(nullptr, (x * _CellSizeX) + (_WindowSizeX * 0.04f), (y * _CellSizeY) + (_WindowSizeY * 0.04f), _CellSizeX, _Mass[x][y] * _CellSizeY, 0.0f, 0.0f, 0.0f, 0.0f);
				else
				{
					_Renderer->DrawSprite(nullptr, (x * _CellSizeX) + (_WindowSizeX * 0.04f), (y * _CellSizeY) + (_WindowSizeY * 0.04f), _CellSizeX, _CellSizeY, 0.0f, 0.0f, 0.0f, 0.0f);
				}

				_Renderer->SetRenderColour(1.0f, 1.0f, 1.0f);
				char _DisplayMass[20];
				sprintf(_DisplayMass, "%.4f", _Mass[x][y]);
				//_Renderer->DrawText2D(_Font, _DisplayMass, (x * _CellSizeX) + (_WindowSizeX * 0.04f), (y * _CellSizeY) + (_WindowSizeY * 0.04f));
			}
		}
	}
}