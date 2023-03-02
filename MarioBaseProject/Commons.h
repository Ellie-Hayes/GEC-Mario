#pragma once

struct Vector2D
{
	float x; 
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float intialX, float initalY)
	{
		x = intialX;
		y = initalY;

	}
};

enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_MENU, 
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER, 
	SCREEN_HIGHSCORES
};