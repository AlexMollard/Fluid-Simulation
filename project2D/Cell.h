#pragma once
#include "CellType.h"

class Cell
{
public:
	Cell();
	~Cell();

	// Index Functions
	//----------------
	// Get:
	//----------------
	int GetIndexX() { return _IndexX; }
	int GetIndexY() { return _IndexY; }
	//----------------
	// Set:
	//----------------
	void SetIndexX(int _indexX) { _IndexX = _indexX; }
	void SetIndexY(int _indexY) { _IndexY = _indexY; }
	void SetIndex(int _indexX, int _indexY) { _IndexX = _indexX; _IndexY = _indexY; }

	// Type Functions
	int GetType()					{ return _Type; };
	void SetType(CellType type)		{ _Type = type; };

	// WaterFunctions
	float GetWaterTotal()			{ return _WaterTotal; };
	void SetWaterTotal(float Water) { _WaterTotal = Water; };

private:
	int _IndexX;
	int _IndexY;

	float _WaterTotal;

	CellType _Type;
};

