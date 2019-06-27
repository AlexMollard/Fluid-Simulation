#pragma once
#include <Renderer2D.h>
#include "Input.h"
#include "Cell.h"
#include "CellType.h"

class CellManager
{
public:
	CellManager(int cellAmount);
	~CellManager();

	// Update Functions
	void Update(float deltaTime);
	void Draw(aie::Renderer2D* renderer);

	// Cell Functions
	void NextGeneration();

	// Resize Grid Function
	void Resize();

	void CheckInput(float deltaTime);

	Cell* GetUp(int x, int y);
	Cell* GetDown(int x, int y);
	Cell* GetLeft(int x, int y);
	Cell* GetRight(int x, int y);
private:
	// Cell Variables
	int _CellTotal;
	float _CellSizeY;
	float _CellSizeX;
	float _WaterSizeY;
	float _Timer;
	float _WindowSizeX;
	float _WindowSizeY;

	CellType** _CellType;
	float** _WaterTotal;

	// Cells
	Cell*** _Cell;
	Cell* _UpCell, *_DownCell, *_RightCell, *_LeftCell;

	aie::Font*			_font;
};


