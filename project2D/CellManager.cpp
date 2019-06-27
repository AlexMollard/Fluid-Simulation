#include "CellManager.h"
#include "Application.h"
#include "Font.h"
#include <iostream>
using namespace std;

#define CellSize 10.0f

CellManager::CellManager(int cellAmount)
{
	// Get amount of cells squared
	_CellTotal = cellAmount;

	_Timer = 0;

	_font = new aie::Font("./font/consolas.ttf", 14);

	// Set cell size
	_CellSizeX = CellSize;
	_CellSizeY = CellSize;

	// Get Window Size
	aie::Application* application = aie::Application::GetInstance();
	_WindowSizeX = application->GetWindowWidth();
	_WindowSizeY = application->GetWindowHeight();

	// Create Cells
	_Cell = new Cell**[_CellTotal];
	_CellType = new CellType*[_CellTotal];
	_WaterTotal = new float*[_CellTotal];

	for (int i = 0; i < cellAmount; i++)
	{
		_Cell[i] = new Cell*[_CellTotal];
		_CellType[i] = new CellType[_CellTotal];
		_WaterTotal[i] = new float[_CellTotal];

		for (int j = 0; j < _CellTotal; j++)
			_Cell[i][j] = new Cell();
	}

	//Set cells index
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			if (x == 0 || x == _CellTotal - 1 || y == 0 || y == _CellTotal - 1)
				_CellType[x][y] = CellType_Solid;
			else 
				_CellType[x][y] = CellType_Empty;

			_WaterTotal[x][y] = 0.0f;
			_Cell[x][y]->SetIndex(x, y);
			_Cell[x][y]->SetWaterTotal(_WaterTotal[x][y]);
			_Cell[x][y]->SetType(_CellType[x][y]);
		}
	}

	// Resize grid
	Resize();
}

CellManager::~CellManager()
{
	// Delete Cells
	for (int x = 0; x < _CellTotal; x++)
	{
		delete[] _Cell[x];
		delete[] _Cell;
		
		delete[] _CellType[x];
		delete[] _CellType;
		
		delete[] _WaterTotal[x];
		delete[] _WaterTotal;
	}
}

void CellManager::Update(float deltaTime)
{
	aie::Application* application = aie::Application::GetInstance();
	_Timer += 10 * deltaTime;
	if (_WindowSizeX != application->GetWindowWidth() || _WindowSizeY != application->GetWindowHeight())
	{
		// Resize grid
		Resize();
	}

	// Get user input
	CheckInput(deltaTime);

	if (_Timer > 0.5)
	{
		NextGeneration();

		for (int x = 0; x < _CellTotal; x++)
		{
			for (int y = 0; y < _CellTotal; y++)
			{
				_Cell[x][y]->SetWaterTotal(_WaterTotal[x][y]);
				_Cell[x][y]->SetType(_CellType[x][y]);
			}
		}

		_Timer = 0;
	}
}

Cell* CellManager::GetUp(int x, int y)
{
	_UpCell = nullptr;

	if (y < _CellTotal - 1)
		_UpCell = _Cell[x][y + 1];

	return _UpCell;
}

Cell* CellManager::GetDown(int x, int y)
{
	_DownCell = nullptr;

	if (y > 0)
		_DownCell = _Cell[x][y - 1];	

	return _DownCell;
}

Cell* CellManager::GetRight(int x, int y)
{
	_RightCell = nullptr;

	if (x < _CellTotal - 1)
		_RightCell = _Cell[x + 1][y];

	return _RightCell;
}

Cell* CellManager::GetLeft(int x, int y)
{
	_LeftCell = nullptr;

	if (x > 0)
		_LeftCell = _Cell[x - 1][y];

	return _LeftCell;
}

void CellManager::NextGeneration()
{
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{

			if (_Cell[x][y]->GetType() == CellType_Water && GetDown(x, y)->GetType() == CellType_Empty)
			{
				_CellType[x][y] = CellType_Empty;
				_WaterTotal[x][y] = 0;
			}
			else if (_Cell[x][y]->GetType() == CellType_Water && GetDown(x, y)->GetType() == CellType_Water && GetDown(x, y)->GetWaterTotal() < 1)
			{
				float waterNeeded = 1 - GetDown(x, y)->GetWaterTotal();

				_WaterTotal[x][y] -= waterNeeded;

				if (_WaterTotal[x][y] < 0)
					_WaterTotal[x][y] = 0;
			}
			else if (_Cell[x][y]->GetType() == CellType_Water && GetDown(x, y)->GetType() == CellType_Water && GetDown(x, y)->GetWaterTotal() < _Cell[x][y]->GetWaterTotal())
			{
				_WaterTotal[x][y] -= _Cell[x][y]->GetWaterTotal() / 8;
			}

			else if (_Cell[x][y]->GetType() == CellType_Empty && GetUp(x, y)->GetType() == CellType_Water)
			{
				_CellType[x][y] = CellType_Water;
				_WaterTotal[x][y] = _WaterTotal[x][y] + GetUp(x, y)->GetWaterTotal();

			}
			else if (_Cell[x][y]->GetType() == CellType_Water && GetUp(x, y)->GetType() == CellType_Water && _Cell[x][y]->GetWaterTotal() < 1)
			{
				float waterNeeded = 1 - _WaterTotal[x][y];
				if (waterNeeded > GetUp(x, y)->GetWaterTotal())
					_WaterTotal[x][y] += waterNeeded;
				else
					_WaterTotal[x][y] += GetUp(x, y)->GetWaterTotal();

				_WaterTotal[x][y] += waterNeeded;
			}
			else if (_Cell[x][y]->GetType() == CellType_Water && GetUp(x, y)->GetType() == CellType_Water && _Cell[x][y]->GetWaterTotal() < GetUp(x, y)->GetWaterTotal())
			{
				_WaterTotal[x][y] += GetUp(x, y)->GetWaterTotal() / 8;
			}

			else if (_Cell[x][y]->GetType() == CellType_Water && GetRight(x, y)->GetType() != CellType_Solid && GetDown(x, y)->GetType() != CellType_Empty)
			{
				_WaterTotal[x][y] -= _Cell[x][y]->GetWaterTotal() / 4;
				if (_Cell[x][y]->GetWaterTotal() < 0.005f)
				{
					_WaterTotal[x][y] = 0;
					_CellType[x][y] = CellType_Empty;
				}
			}
			else if (_Cell[x][y]->GetType() != CellType_Solid && GetRight(x, y)->GetType() == CellType_Water && GetDown(x, y)->GetType() != CellType_Empty)
			{
				_CellType[x][y] = CellType_Water;
				_WaterTotal[x][y] += GetRight(x, y)->GetWaterTotal() / 4;
			}

			else if (_Cell[x][y]->GetType() == CellType_Water && GetLeft(x, y)->GetType() == CellType_Empty && GetDown(x, y)->GetType() != CellType_Empty)
			{
				_WaterTotal[x][y] -= _Cell[x][y]->GetWaterTotal() / 4;
				if (_Cell[x][y]->GetWaterTotal() < 0.005f)
				{
					_WaterTotal[x][y] = 0;
					_CellType[x][y] = CellType_Empty;
				}
			}
			else if (_Cell[x][y]->GetType() != CellType_Solid && GetLeft(x, y)->GetType() == CellType_Water && GetDown(x, y)->GetType() != CellType_Empty)
			{
				_CellType[x][y] = CellType_Water;
				_WaterTotal[x][y] += GetLeft(x, y)->GetWaterTotal() / 4;
			}

			else if (_Cell[x][y]->GetWaterTotal() >= 50.0f && _CellType[x][y] == CellType_Water)
			{
				_WaterTotal[x][y] = 50.0f;
			}

			//if (_Cell[x][y]->GetWaterTotal() <= 0.000005f && _CellType[x][y] == CellType_Water)
			//{
			//	_WaterTotal[x][y] = 0.0f;
			//	_CellType[x][y] = CellType_Empty;
			//}
		}
	}
}

void CellManager::Resize()
{
	aie::Application* application = aie::Application::GetInstance();
	_WindowSizeX = application->GetWindowWidth();
	_WindowSizeY = application->GetWindowHeight();

	// Cell size
	_CellSizeX = (_WindowSizeX * 0.9f) / _CellTotal;
	_CellSizeY = (_WindowSizeY * 0.9f) / _CellTotal;
}

void CellManager::CheckInput(float deltaTime)
{
	aie::Input* input = aie::Input::GetInstance();

	if (input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		for (int x = 0; x < _CellTotal; x++)
		{
			for (int y = 0; y < _CellTotal; y++)
			{
				if (x == 0 || x == _CellTotal - 1 || y == 0 || y == _CellTotal - 1)
				{

				}
				else if (input->GetMouseX() >= (x * _CellSizeX) + (_WindowSizeX * 0.05f) && input->GetMouseX() <= (x * _CellSizeX) + (_WindowSizeX * 0.05f) + (_CellSizeX) &&
					input->GetMouseY() >= (y * _CellSizeY) + (_WindowSizeY * 0.05f) && input->GetMouseY() <= (y * _CellSizeY) + (_WindowSizeY * 0.05f) + (_CellSizeY))
				{
					if (input->IsKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
						_CellType[x][y] = CellType_Empty;
					else
						_CellType[x][y] = CellType_Solid;

					_WaterTotal[x][y] = 0.0f;

				}
			}
		}
	}
	else if (input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		for (int x = 0; x < _CellTotal; x++)
		{
			for (int y = 0; y < _CellTotal; y++)
			{
				if (x == 0 || x == _CellTotal - 1 || y == 0 || y == _CellTotal - 1)
				{

				}
				else if (input->GetMouseX() >= (x * _CellSizeX) + (_WindowSizeX * 0.05f) && input->GetMouseX() <= (x * _CellSizeX) + (_WindowSizeX * 0.05f) + (_CellSizeX) &&
					input->GetMouseY() >= (y * _CellSizeY) + (_WindowSizeY * 0.05f) && input->GetMouseY() <= (y * _CellSizeY) + (_WindowSizeY * 0.05f) + (_CellSizeY))
				{
					if (input->IsKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
					{
						_WaterTotal[x][y] = 0.0f;
						_CellType[x][y] = CellType_Empty;
					}
					else
					{
						_CellType[x][y] = CellType_Water;
						_WaterTotal[x][y] += 2.0f * deltaTime;
					}
				}
			}
		}
	}

}

void CellManager::Draw(aie::Renderer2D* renderer)
{
	// Draw cells
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			if (_Cell[x][y]->GetType() == CellType_Water)
			{

				_WaterSizeY = _CellSizeY * _Cell[x][y]->GetWaterTotal();

				if (_WaterSizeY > _CellSizeY)
					_WaterSizeY = _CellSizeY;


				renderer->SetRenderColour(-_Cell[x][y]->GetWaterTotal() / 10 + 0.4f, -_Cell[x][y]->GetWaterTotal() / 10 + 0.4f, -_Cell[x][y]->GetWaterTotal() / 10 + 0.9f);
				renderer->DrawSprite(nullptr, (x * _CellSizeX) + (_WindowSizeX * 0.05f), (y * _CellSizeY) + (_WindowSizeY * 0.05f), _CellSizeX, _WaterSizeY, 0.0f, 0.0f, 0.0f, 0.0f);
				
				renderer->SetRenderColour(1.0f, 0.0f, 0.0f);
				char str[10];
				sprintf(str, "%.4f", _Cell[x][y]->GetWaterTotal());
				renderer->DrawText2D(_font, str, (x * _CellSizeX) + (_WindowSizeX * 0.05f), (y * _CellSizeY) + (_WindowSizeY * 0.05f));
			}
			else if (_CellType[x][y] == CellType_Solid)
			{
				renderer->SetRenderColour(0.4f, 0.4f, 0.4f);
				renderer->DrawSprite(nullptr, (x * _CellSizeX) + (_WindowSizeX * 0.05f), (y * _CellSizeY) + (_WindowSizeY * 0.05f), _CellSizeX, _CellSizeY, 0.0f, 0.0f, 0.0f, 0.0f);
			}
		}
	}
}