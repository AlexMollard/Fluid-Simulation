#pragma once
#include "Renderer2D.h"
#include "Font.h"
#include <iostream>
enum Cell
{
	_Air,
	_Ground,
	_Water
};
class WaterSimulation
{
public:
	WaterSimulation(int _Size);
	~WaterSimulation();

	void Update(float deltaTime);
	void Draw(aie::Renderer2D* _Renderer);

	void Resize();
	void CheckNeighbours(float deltaTime);
	void CheckInput(float deltaTime);

	float constrain(float x, float a, float b); // Keeps given value within to other varibles
	float GetStableWater(float _TotalMass);		// Get amount of water needed to stabilize water
private:
	// Grid sizes
	int _GridSize;					   // The size of the grid squared.
	int _CellSizeX;
	int _CellSizeY;
	int _WindowSizeX, _WindowSizeY;

	// Data structures
	int** _Cell;
	float** _Mass;
	float** _NewMass;
	bool** _Falling;

	// Water Properties
	float _MaxMass = 1.0f;			   // The amount of water a cell can hold without being pressurized.
	float _MaxCompression = 0.1f;	   // How much extra water a cell can hold, Compared to the cell above it.
	float _MinMass = 0.0001f;		   // Amount of water that is classified as being empty.
	float _MinFlow = 0.01;
	float _MaxSpeed = 1;
	float _Timer;
	aie::Font* _Font;
};

