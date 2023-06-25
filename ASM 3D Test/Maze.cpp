#include "pch.h"
#include "Maze.h"

void Maze::DrawWall( Int32 x, Uint32 height, Float brightness )
{
	for ( Int32 h = 0u; h < static_cast< Int32 >( height ); ++h )
	{
		Int32 offset = static_cast< Int32 >( Math::Sin( x * 0.05f + forge::Time::GetRealTime() * 10.0f ) * 15.0f );
		offset = 0;
		m_renderer.SetPixel( x, h + offset, brightness );
		m_renderer.SetPixel( x, -h + offset, brightness );
	}
}

void Maze::Update( Float dt )
{
	for ( Int32 y = -m_renderer.GetMaxY(); y < 0; ++y )
	{
		m_renderer.FillLine( y, '/' );
	}

	m_renderer.FillLine( 0, '-' );

	for ( Int32 y = 1; y <= m_renderer.GetMaxY(); ++y )
	{
		m_renderer.FillLine( y, '\\' );
	}

	Map map;

	m_player.Update( forge::Time::GetDeltaTime() );

	const Float fov = FORGE_PI_HALF * 0.5f;
	const Float halfFov = fov * 0.5f;

	for ( Int32 x = -m_renderer.GetMaxX(); x < m_renderer.GetMaxX(); ++x )
	{
		Float wallHeight = map.GetWallHeight( m_player.GetPosition(), m_player.GetAngle(), -static_cast< Float >( x ) / m_renderer.GetMaxX() * halfFov );
		DrawWall( x, static_cast< Uint32 >( wallHeight ), 1.0f );
	}

	m_renderer.Draw();
}
