#pragma once
#include <Renderer2D.h>
#include "Cell.h"
#include "Input.h"


class GridManager
{
public:
	// Overloaded Constructor
	GridManager(int cellAmount, float windowSizeX, float windowSizeY);

	// Update Functions
	void Update(aie::Input* input, float deltaTime , float windowWidth, float windowHeight);
	void Draw(aie::Renderer2D* renderer);

	// Cell Functions
	void CheckNeighbours(aie::Input* input, float deltaTime);
	void NextGeneration(int x, int y);
	int GetCellType(Cell cell);

	// Other Functions
	void Resize(float windowWidth, float windowHeight);

private:
	// Window Variables
	float _WindowSizeX;
	float _WindowSizeY;

	// Cell Variables
	int _CellTotal;
	int _CellSizeX;
	int _CellSizeY;

	// Neighbour Variables
	int _AliveNeighbours;
	int _NeighbourType;
	int _Type1Neighbours;
	int _Type2Neighbours;
	int _Type3Neighbours;

	// Neighbour Cells
	Cell *TR, *TM, *TL;
	Cell *MR, *ML;
	Cell *BR, *BM, *BL;

	int _TRType, _TMType, _TLType;
	int _MRType, _MLType;
	int _BRType, _BMType, _BLType;

	// Other Variables
	float _Timer;
	int _Move;

	// Cells
	Cell** _Cells;
	Cell _Wall;

};

