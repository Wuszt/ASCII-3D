#include "pch.h"
#include <windows.h>
#include "Renderer.h"

Player m_player;
Renderer m_renderer( 201, 16.0f / 9.0f, 2u );

void DrawWall( Int32 x, Uint32 height, Float brightness )
{
	for( Int32 h = 0u; h < static_cast< Int32 >( height ); ++h )
	{
		Int32 offset = static_cast< Int32 >( Math::Sin( x * 0.05f + forge::Time::GetRealTime() * 10.0f ) * 15.0f );
		offset = 0;
		m_renderer.SetPixel( x, h + offset, brightness );
		m_renderer.SetPixel( x, -h + offset, brightness );
	}
}

int main()
{
	forge::Time::Initialize();

	while( true )
	{
		forge::Time::Update();
		forge::StopWatch sw;
		m_renderer.InitializeBuffer();
		
		Map map;

		m_player.Update( forge::Time::GetDeltaTime() );

		const Float fov = FORGE_PI_HALF * 0.5f;
		const Float halfFov = fov * 0.5f;

		for( Int32 x = -m_renderer.GetMaxX(); x < m_renderer.GetMaxX(); ++x )
		{
			Float wallHeight = map.GetWallHeight( m_player.GetPosition(), m_player.GetAngle(), -static_cast< Float >( x ) / m_renderer.GetMaxX() * halfFov );
			DrawWall( x, static_cast< Uint32 >( wallHeight ), 1.0f );
		}

		m_renderer.Draw();
		SetConsoleTitle( std::to_wstring( 1.0f / sw.GetDuration() ).c_str() );
	}
}