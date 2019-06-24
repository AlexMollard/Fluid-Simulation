#pragma once
#include <Renderer2D.h>
#include "Input.h"
class Cell
{
public:
	Cell();

	// Update Functions
	void Update(float DeltaTime, aie::Input* input);
	void Draw(aie::Renderer2D* renderer);
	void MouseOver(aie::Input* input, float  DeltaTime);

	// Postition Functions
	void SetPos(float x, float y)	{ _CellX = x;  _CellY = y; };
	float GetX()					{ return _CellX; };
	float GetY()					{ return _CellY; };
	
	// Fall Functions
	bool GetFall()					{ return _Fall; };
	void SetFall(bool fall)			{ _Fall = fall; };

	// Alive Functions
	bool GetAlive()					{ return _Alive; };
	void SetAlive(bool alive)		{ _Alive = alive; };

	// Survive Functions
	bool GetSurvive()				{ return _Survive; };
	void SetSurvive(bool Survive)	{ _Survive = Survive; };

	// Wall Functions
	bool GetWall()					{ return _Wall; };
	void SetWall(bool Wall)			{ _Wall = Wall; };

	// WaterFunctions
	float GetWaterTotal()			{ return _WaterTotal; };
	void SetWaterTotal(float Water)	{ _WaterTotal = Water; };

	// ChangeWaterFunctions
	float GetChangeWaterTotal()			{ return _ChangeWaterTotal; };
	void SetChangeWaterTotal(float Water) { _ChangeWaterTotal = Water; };

	// Type Functions
	int GetType()					{ return _Type; };
	void SetType(char type)			{ _Type = type; };

	// ChangeType Functions
	int GetChangeType()				{ return _ChangeType; };
	void SetChangeType(char type)	{ _ChangeType = type; };

	// Size Functions
	float GetSizeX()						{ return _SizeX; };
	float GetSizeY()						{ return _SizeY; };
	float GetSizeBeforeY()					{ return _SizeBeforeY; };
	void SetSize(float sizeX, float sizeY)	{ _SizeX = sizeX; _SizeY = sizeY; };
	void setSizeBeforeY(float sizeY)		{ _SizeBeforeY = sizeY; };

	// Colour Functions
	float GetR()					{ return _R; };
	float GetG()					{ return _G; };
	float GetB()					{ return _B; };


private:
	// Next Generation Status
	bool _Survive;
	bool _Alive;

	// Position Variables
	float _CellX;
	float _CellY;
	bool _Fall;

	// Size Variables
	float _SizeX;
	float _SizeY;
	float _SizeBeforeY;

	// Colour Variables 
	float _R;
	float _G;
	float _B;

	// Type Variables
	int _Type;
	int _ChangeType;
								// Directions in int value
	// Water Variables			//	1 2 3 
	float _WaterTotal;			//  4 X 5
	int _Direction;				//  6 7 8 
	float _ChangeWaterTotal;

	// Wall Variable
	bool _Wall;

	// Timer Variable
	float _Timer;
};

