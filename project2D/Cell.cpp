#include "Cell.h"
#include <iostream>
#include "Input.h"
using namespace std;

Cell::Cell()
{
	// Initializing variables
	_Alive = true;
	_DeathRow = false;
	_R = 0.0f;
	_G = 0.0f;
	_B = 0.0f;
	_CellX = 0.0f;
	_CellY = 0.0f;
	_Survive = true;
	_Type = 1;
	_Timer = 0;
	_Wall = false;
	_ChangeType = 0;
}

void Cell::Draw(aie::Renderer2D* renderer)
{
}

void Cell::Update(float DeltaTime, aie::Input* input)
{
	_Timer += DeltaTime * 10;

	_Type = _ChangeType;

	MouseOver(input);

	// Set colour for each type of cell
	if (_Wall)
	{
		_R = 1.0f;
		_G = 1.0f;
		_B = 1.0f;
	}
	else if (_Alive && _Type == 1)	// Solid
	{
		_R = 0.4f;
		_G = 0.4f;
		_B = 0.4f;
	}
	else if (_Alive && _Type == 2)	// Liquid 1
	{
		_R = 0.2f;
		_G = 0.2f;
		_B = 0.85f;
	}
	else if (_Alive && _Type == 3)	// Liquid 2
	{
		_R = 0.0f;
		_G = 0.0f;
		_B = 0.0f;
	}
	
	// If the cell is dead make it black
	if (!_Alive && !_Wall)
	{
		_R = 0.0f;
		_G = 0.0f;
		_B = 0.0f;
	}
}

void Cell::MouseOver(aie::Input* input)
{
	// Save Cell
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (_Timer > 0.5)
		{
			if (input->getMouseX() >= _CellX - (_SizeX) && input->getMouseX() <= _CellX + (_SizeX) &&
				input->getMouseY() >= _CellY - (_SizeY) && input->getMouseY() <= _CellY + (_SizeY))
			{
				_ChangeType += 1;
				if (_ChangeType == 4)
					_ChangeType = 1;
			}
			_Timer = 0;
		}
	}

	// Kill Cell
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		if (_Timer > 0.5)
		{
			if (input->getMouseX() >= _CellX - (_SizeX / 2) && input->getMouseX() <= _CellX + (_SizeX / 2) &&
				input->getMouseY() >= _CellY - (_SizeY / 2) && input->getMouseY() <= _CellY + (_SizeY / 2))
			{
				_ChangeType = 3;
			}
			_Timer = 0;
		}
	}
}