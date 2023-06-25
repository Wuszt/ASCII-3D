#include "pch.h"
#include <windows.h>
#include "Player.h"

void Player::Update( Float dt )
{
	if ( GetKeyState( 'W' ) & 0x8000 )
	{
		m_position += Vector2( cos( m_angle ), sin( m_angle ) ) * forge::Time::GetDeltaTime();
	}
	else if ( GetKeyState( 'S' ) & 0x8000 )
	{
		m_position -= Vector2( cos( m_angle ), sin( m_angle ) ) * forge::Time::GetDeltaTime();
	}

	if ( GetKeyState( 'A' ) & 0x8000 )
	{
		m_position += Vector2( sin( m_angle ), cos( m_angle ) ) * forge::Time::GetDeltaTime();
	}
	else if ( GetKeyState( 'D' ) & 0x8000 )
	{
		m_position -= Vector2( sin( m_angle ), cos( m_angle ) ) * forge::Time::GetDeltaTime();
	}

	if ( GetKeyState( VK_LEFT ) & 0x8000 )
	{
		m_angle += forge::Time::GetDeltaTime() * 0.5f;
	}
	else if ( GetKeyState( VK_RIGHT ) & 0x8000 )
	{
		m_angle -= forge::Time::GetDeltaTime() * 0.5f;
	}
}
