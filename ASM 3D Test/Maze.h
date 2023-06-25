#pragma once
#include "Renderer.h"
#include "Player.h"
class Maze
{
public:
	void Update( Float dt );

private:
	void DrawWall( Int32 x, Uint32 height, Float brightness );

	Player m_player;
	Renderer m_renderer = Renderer( 201, 16.0f / 9.0f, 2u );
};

