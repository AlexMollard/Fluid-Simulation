#include "GridManager.h"
#include "Font.h"
#include <iostream>
using namespace std;

GridManager::GridManager(int cellAmount, float windowSizeX, float windowSizeY)
{
	_Font = new aie::Font("./font/consolas.ttf", 0);

	// Set how many cells there are
	_CellTotal = cellAmount;

	// Set window size varibles
	_WindowSizeX = windowSizeX;
	_WindowSizeY = windowSizeY;

	// Set Cell Size
	_CellSizeX = 10;
	_CellSizeY = 10;

	// Set Cell Neighbour Variables
	_AliveNeighbours = 0;

	// Set Timer for each generation
	_Timer = 0;

	// Create Cells
	_Cells = new Cell**[_CellTotal];
	for (int i = 0; i < cellAmount; i++)
	{
		_Cells[i] = new Cell*[_CellTotal];

		for (int j = 0; j < _CellTotal; j++)
			_Cells[i][j] = new Cell();
	}

	// Resize Cells for window size
	Resize(windowSizeX, windowSizeY);

	// Set cells
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			// Set the walls
			if (x == 0 || x == _CellTotal - 1 || y == 0 || y == _CellTotal - 1)
			{
				_Cells[x][y]->SetWall(true);
				_Cells[x][y]->SetAlive(false);
				_Cells[x][y]->SetSurvive(false);
			}
			else
			{
				_Cells[x][y]->SetSurvive(false);
				_Cells[x][y]->SetType(CellType_Empty);
			}
		}
	}

	// Create wall cell
	_Wall = _Cells[0][0];
}

void GridManager::Update(aie::Input* input, float deltaTime, float windowWidth, float windowHeight)
{
	// Timer tracker
	_Timer += deltaTime * 10;

	// Resize Cells if window size has changed
	if (_WindowSizeX != windowWidth || _WindowSizeY != windowHeight)
		Resize(windowWidth, windowHeight);

	// input->isKeyDown(aie::INPUT_KEY_SPACE) &&
	// Check all cells neighbours

		CheckNeighbours(input, deltaTime);


	// Update all the cells
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			_Cells[x][y]->Update(deltaTime, input);
		}
	}

}

void GridManager::Resize(float windowWidth, float windowHeight)
{
		// Window Size
		_WindowSizeX = windowWidth;
		_WindowSizeY = windowHeight;

		// Cell size
		_CellSizeX = (_WindowSizeX * 0.90) / _CellTotal;
		_CellSizeY = (_WindowSizeY * 0.90) / _CellTotal;

		// Set all cells positions
		for (int x = 0; x < _CellTotal; x++)
		{
			for (int y = 0; y < _CellTotal; y++)
			{
				// Set Cell Position
				_Cells[x][y]->SetPos(x * _CellSizeX + (_WindowSizeX * 0.05), y *_CellSizeY  + (_WindowSizeY * 0.05));
				_Cells[x][y]->SetSize(_CellSizeX, _CellSizeY);
			}
		}
}

void GridManager::CheckNeighbours(aie::Input* input, float deltaTime)
{
	_Move = 0;
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			if (!_Cells[x][y]->GetWall())
			{

				// How many neighbours does the cell have
				_AliveNeighbours = 0;
				_Type1Neighbours = 0;
				_Type2Neighbours = 0;
				_Type3Neighbours = 0;

				// Reset neighbours
				TR	= _Wall;
				TM	= _Wall;
				TL	= _Wall;
				MR	= _Wall;
				ML	= _Wall;
				BR	= _Wall;
				BM	= _Wall;
				BL	= _Wall;

				// Reset neighbours types
				_TRType = 0;
				_TMType = 0;
				_TLType = 0;
				_MRType = 0;
				_MLType = 0;
				_BRType = 0;
				_BMType = 0;
				_BLType = 0;

				// Neighbour Cells
				if (!_Cells[x + 1][y + 1]->GetWall())
					TR = _Cells[x + 1][y + 1];

				if (!_Cells[x][y + 1]->GetWall())
					TM = _Cells[x][y + 1];

				if (!_Cells[x - 1][y + 1]->GetWall())
					TL = _Cells[x - 1][y + 1];

				if (!_Cells[x + 1][y]->GetWall())
					MR = _Cells[x + 1][y];

				if (!_Cells[x - 1][y]->GetWall())
					ML = _Cells[x - 1][y];

				if (!_Cells[x + 1][y - 1]->GetWall())
					BR = _Cells[x + 1][y - 1];

				if (!_Cells[x][y - 1]->GetWall())
					BM = _Cells[x][y - 1];

				if (!_Cells[x - 1][y - 1]->GetWall())
					BL = _Cells[x - 1][y - 1];

				// Get how many neighbours alive and type
				if (TR->GetAlive())
					_TRType = GetCellType(*TR);
										
				if (TM->GetAlive())						
					_TMType = GetCellType(*TM);
										
				if (TL->GetAlive())						
					_TLType = GetCellType(*TL);
										
				if (MR->GetAlive())						
					_MRType = GetCellType(*MR);
										
				if (ML->GetAlive())						
					_MLType = GetCellType(*ML);
										
				if (BL->GetAlive())						
					_BLType = GetCellType(*BL);
										
				if (BM->GetAlive())						
					_BMType = GetCellType(*BM);
									
				if (BR->GetAlive())						
					_BRType = GetCellType(*BR);

				// Set Cells new status
				NextGeneration(x,y);

			}
		}
	}
}

void GridManager::NextGeneration(int x, int y)	// Rules Would be put in here
{
	// Syntax: Name     | Color   | Get Type					  | Get neighbour types
	// Type 1: Solid    | (Grey)  | { _Cells[x][y]->GetType() == 1 | _Type1Neighbours == int}
	// Type 2: Liquid 1 | (Blue)  | { _Cells[x][y]->GetType() == 2 | _Type2Neighbours == int}
	// Type 3: Empty	| (Black) | { _Cells[x][y]->GetType() == 3 | _Type3Neighbours == int}


	// Gravity
	if (BM->GetType() == CellType_Empty && _Cells[x][y]->GetType() == CellType_Water)
	{
		BM->SetType(CellType_Water);
		BM->SetWaterTotal(_Cells[x][y]->GetWaterTotal());
		_Cells[x][y]->SetWaterTotal(0);
		_Cells[x][y]->SetType(CellType_Empty);

	}

	// Gravity on water
	if (_Cells[x][y]->GetType() == CellType_Water && BM->GetType() == CellType_Water)
	{
		if (BM->GetWaterTotal() < 1)
		{
			BM->SetWaterTotal(BM->GetWaterTotal() + _Cells[x][y]->GetWaterTotal() / 2);
			_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() / 2);

			if (_Cells[x][y]->GetWaterTotal() < 0.005f)
			{
				_Cells[x][y]->setSizeBeforeY(0);
				_Cells[x][y]->SetFall(false);
				_Cells[x][y]->SetWaterTotal(0);
				_Cells[x][y]->SetType(CellType_Empty);
			}
		}
	}
	
	
	// ---------------
	if (BM->GetType() != CellType_Empty && MR->GetType() == CellType_Empty && ML->GetType() == CellType_Empty && MR->GetWaterTotal() < _Cells[x][y]->GetWaterTotal() && ML->GetWaterTotal() < _Cells[x][y]->GetWaterTotal())
	{
		MR->SetType(CellType_Water);
		MR->SetType(CellType_Water);
		MR->SetWaterTotal(_Cells[x][y]->GetWaterTotal() / CellType_Water);
	
		ML->SetType(CellType_Water);
		ML->SetType(CellType_Water);
		ML->SetWaterTotal(_Cells[x][y]->GetWaterTotal() / CellType_Water);
	
		_Cells[x][y]->SetWaterTotal(0);
	}
	else if (BM->GetType() != CellType_Empty && MR->GetType() == CellType_Empty && MR->GetWaterTotal() < _Cells[x][y]->GetWaterTotal())
	{
		MR->SetType(CellType_Water);
		MR->SetType(CellType_Water);
		MR->SetWaterTotal(_Cells[x][y]->GetWaterTotal() / CellType_Water);
		_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() - (_Cells[x][y]->GetWaterTotal() / CellType_Water));
	}
	else if (BM->GetType() != CellType_Empty && ML->GetType() == CellType_Empty && ML->GetWaterTotal() < _Cells[x][y]->GetWaterTotal())
	{
		ML->SetType(CellType_Water);
		ML->SetType(CellType_Water);
		ML->SetWaterTotal(_Cells[x][y]->GetWaterTotal() / CellType_Water);
		_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() - (_Cells[x][y]->GetWaterTotal() / CellType_Water));
	}
	
	if (_Cells[x][y]->GetType() == CellType_Water)
	{
		if (MR->GetType() == CellType_Water && ML->GetType() == CellType_Water)									// Work on this
		{
				MR->SetWaterTotal(MR->GetWaterTotal() + _Cells[x][y]->GetWaterTotal() / 4);
				ML->SetWaterTotal(ML->GetWaterTotal() + _Cells[x][y]->GetWaterTotal() / 4);

				_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() - _Cells[x][y]->GetWaterTotal() / 2);

		}
		if (MR->GetType() == CellType_Water && _Cells[x][y]->GetWaterTotal() > MR->GetWaterTotal())
		{
			MR->SetWaterTotal(MR->GetWaterTotal() + _Cells[x][y]->GetWaterTotal() / 4);
			_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() - _Cells[x][y]->GetWaterTotal() / 4);
		}
		if (ML->GetType() == CellType_Water && _Cells[x][y]->GetWaterTotal() > ML->GetWaterTotal())
		{
			ML->SetWaterTotal(ML->GetWaterTotal() + _Cells[x][y]->GetWaterTotal() / 4);
			_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() - _Cells[x][y]->GetWaterTotal() / 4);
		}

	}

	//Pressure
	if (_Cells[x][y]->GetType() == CellType_Water && _Cells[x][y]->GetWaterTotal() > TM->GetWaterTotal() && (TM->GetType() == CellType_Water || TM->GetType() == CellType_Empty))
	{
		TM->SetType(CellType_Water);
		TM->SetWaterTotal((_Cells[x][y]->GetWaterTotal() * 0.5f) + TM->GetWaterTotal());
		_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() * 0.5f);
	}
	//else if (_Cells[x][y]->GetType() == CellType_Water && BM->GetWaterTotal() > 1 && _Cells[x][y]->GetWaterTotal() > BM->GetWaterTotal() && BM->GetType() == CellType_Water)
	//{
	//	_Cells[x][y]->SetWaterTotal(_Cells[x][y]->GetWaterTotal() * 0.5);
	//	BM->SetWaterTotal((_Cells[x][y]->GetWaterTotal() * 0.5f) + BM->GetWaterTotal());
	//}

	// Check the sizes
	if ((_Cells[x][y]->GetType() != CellType_Solid && _Cells[x][y]->GetType() != CellType_Wall) && _Cells[x][y]->GetWaterTotal() < 0.01f)
	{
		_Cells[x][y]->SetWaterTotal(0.0f);
		_Cells[x][y]->SetType(CellType_Empty);
	}

	if (TM->GetType() == CellType_Water)
		_Cells[x][y]->SetFall(true);
	
	if (_Cells[x][y]->GetFall() == true)
	{
		_Cells[x][y]->setSizeBeforeY(_CellSizeY);
		_Cells[x][y]->SetFall(false);
	}
	else if (_Cells[x][y]->GetType() == CellType_Water)
	{
		_Cells[x][y]->setSizeBeforeY(_CellSizeY * _Cells[x][y]->GetWaterTotal());
		if (_Cells[x][y]->GetSizeBeforeY() < _CellSizeY)
			_Cells[x][y]->setSizeBeforeY(_CellSizeY * _Cells[x][y]->GetWaterTotal());
	
		if (_Cells[x][y]->GetSizeBeforeY() > _CellSizeY)
			_Cells[x][y]->setSizeBeforeY(_CellSizeY);
	}
}

int GridManager::GetCellType(Cell cell)
{
	_AliveNeighbours += 1;

	// Get Neighbour types
	_NeighbourType = cell.GetType();
	if (_NeighbourType == 1)
	{
		_Type1Neighbours += 1;
		return 1;
	}
	else if (_NeighbourType == 2)
	{
		_Type2Neighbours += 1;
		return 2;
	}
	else if (_NeighbourType == 3)
	{
		_Type3Neighbours += 1;
		return 3;
	}
}

void GridManager::Draw(aie::Renderer2D* renderer)
{
	// Draw cells
	for (int x = 0; x < _CellTotal; x++)
	{
		for (int y = 0; y < _CellTotal; y++)
		{
			renderer->setRenderColour(_Cells[x][y]->GetR(), _Cells[x][y]->GetG(), _Cells[x][y]->GetB());

			if (_Cells[x][y]->GetType() == 2 && !_Cells[x][y]->GetWall())
			{
				renderer->drawSprite(nullptr, _Cells[x][y]->GetX(), _Cells[x][y]->GetY(), _Cells[x][y]->GetSizeX(), _Cells[x][y]->GetSizeBeforeY(), 0.0f, 0.0f, 0.0f, 0.0f);  // Needs to be fixed
				//char str[10];
				//sprintf(str, "%.4f", _Cells[x][y]->GetWaterTotal());
				renderer->setRenderColour(8, 8, 8);
				//renderer->drawText(_Font, str, _Cells[x][y]->GetX() + (_Cells[x][y]->GetSizeX()/20), _Cells[x][y]->GetY());
			}
			//else
				//renderer->drawSprite(nullptr, _Cells[x][y]->GetX(), _Cells[x][y]->GetY(), _Cells[x][y]->GetSizeX(), _Cells[x][y]->GetSizeY(), 0.0f, 0.0f, 0.0f, 0.0f);  // Needs to be fixed
		}
	}
}