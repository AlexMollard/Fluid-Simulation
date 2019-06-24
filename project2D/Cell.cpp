#include "Cell.h"
#include <iostream>
#include "Input.h"
using namespace std;

Cell::Cell()
{
	// Initializing variables
	_Alive = true;
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
	_WaterTotal = 0;
	_SizeBeforeY = 0;
	_Fall = false;
	_ChangeWaterTotal = 0;
}

void Cell::Draw(aie::Renderer2D* renderer)
{
}

void Cell::Update(float DeltaTime, aie::Input* input)
{
	_Timer += DeltaTime * 10;

	_Type = _ChangeType;
	_WaterTotal = _ChangeWaterTotal;

	MouseOver(input, DeltaTime);


	// Set colour for each type of cell
	if (_Wall)
	{
		_R = 1.0f;
		_G = 1.0f;
		_B = 1.0f;
		_ChangeWaterTotal = 0;
	}
	else if (_Alive && _Type == 1)	// Solid
	{
		_R = 0.4f;
		_G = 0.4f;
		_B = 0.4f;
		_ChangeWaterTotal = 0;
	}
	else if (_Alive && _Type == 2)	// Water
	{
		if (_SizeBeforeY < _SizeY)
			_SizeBeforeY = _SizeY * _WaterTotal;

		if (_SizeBeforeY > _SizeY)
			_SizeBeforeY = _SizeY;

		_R = -_WaterTotal / 100 + 0.2f;
		_G = -_WaterTotal / 100 + 0.2f;
		_B = -_WaterTotal / 100 + 0.85f;

		if (_R < 0.1)
			_R = 0.1;
		if (_G < 0.1)
			_G = 0.1;
		if (_B < 0.2)
			_B = 0.2;

		if (_R > 0.8)
			_R = 0.8;
		if (_G > 0.8)
			_G = 0.8;
	}
	else if (_Alive && _Type == 3)	// Empty
	{
		_R = 0.10f;
		_G = 0.108f;
		_B = 0.178f;
		_ChangeWaterTotal = 0;
	}

}

void Cell::MouseOver(aie::Input* input, float DeltaTime)
{
	// Solid Cell
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (input->getMouseX() >= _CellX && input->getMouseX() <= _CellX + (_SizeX) &&
			input->getMouseY() >= _CellY && input->getMouseY() <= _CellY + (_SizeY))
		{
			if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
				_ChangeType = 3;
			else
				_ChangeType = 1;
		}
	}// Water Cell
	else if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		if (input->getMouseX() >= _CellX && input->getMouseX() <= _CellX + (_SizeX) &&
			input->getMouseY() >= _CellY && input->getMouseY() <= _CellY + (_SizeY))
		{
			if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
				_ChangeType = 3;
			else
			{
				_ChangeType = 2;
				_SizeBeforeY = 0;
				_ChangeWaterTotal += 40 * DeltaTime;
			}
		}
	}
}