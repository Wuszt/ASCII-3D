#include "pch.h"
#include <windows.h>
#include "Renderer.h"

Renderer::Renderer( Uint32 height, Float aspectRatio, Uint32 pixelWidth )
	: m_pixelWidth( pixelWidth )
{
	Int32 xResolution = static_cast< Int32 >( aspectRatio * height );
	m_resolution = { xResolution + ( xResolution + 1 ) % 2, static_cast< Int32 >( height ) };

	m_frontBuffer = GetStdHandle( STD_OUTPUT_HANDLE );
	m_secondBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		nullptr,
		CONSOLE_TEXTMODE_BUFFER,
		nullptr );

	m_backBuffer = m_secondBuffer;

	m_bufferResolution = { m_resolution.GetIntX() * m_pixelWidth, m_resolution.GetIntY() };
	m_frameBuffer.resize( ( m_bufferResolution.GetIntX() + 3u ) * ( m_bufferResolution.GetIntY() + 2u ) );

	SetConsoleScreenBufferSize( m_frontBuffer, COORD{ static_cast< short >( m_bufferResolution.GetIntX() + 3u ), static_cast< short >( m_bufferResolution.GetIntY() + 2u )});
	SetConsoleScreenBufferSize( m_secondBuffer, COORD{ static_cast< short >( m_bufferResolution.GetIntX() + 3u ), static_cast< short >( m_bufferResolution.GetIntY() + 2u ) } );
}

void Renderer::Draw()
{
	SetConsoleCursorPosition( m_backBuffer, COORD{ short{ 0 }, short{ 0 } } );
	WriteConsoleA( m_backBuffer, &m_frameBuffer[ 0 ], static_cast< DWORD >( m_frameBuffer.size() ), nullptr, nullptr );
	SetConsoleActiveScreenBuffer( m_backBuffer );
	std::swap( m_backBuffer, m_frontBuffer );
}

void Renderer::SetPixel( Int32 x, Int32 y, Float brightness )
{
	SetPixel( x, y, GetPixelChar( brightness ) );
}

void Renderer::SetPixel( Int32 x, Int32 y, char value )
{
	x = x + m_resolution.GetIntX() / 2;
	y = y + m_resolution.GetIntY() / 2;

	if ( x >= 0 && x < m_resolution.GetIntX() && y >= 0 && y < m_resolution.GetIntY() )
	{
		for ( Int32 i = 0u; i < m_pixelWidth; ++i )
		{
			m_frameBuffer[ i + 1 + x * m_pixelWidth + ( m_bufferResolution.GetIntX() + 3u ) * ( y + 1 ) ] = value;
		}
	}
}

char Renderer::GetPixelChar( Float brightness )
{
	const char colors[] = " -.:+#%@";
	const Uint32 colorsAmount = sizeof( colors ) / sizeof( char );

	const Uint32 index = static_cast< Uint32 >( ( brightness ) * ( colorsAmount - 2u ) );
	return colors[ index ];
}

void Renderer::InitializeBuffer()
{
	for ( Int32 x = 0; x <= static_cast< Int32 >( m_bufferResolution.GetIntX() + 2u ); ++x )
	{
		m_frameBuffer[ x ] = '#';
		m_frameBuffer[ x + ( m_bufferResolution.GetIntX() + 3u ) * ( m_bufferResolution.GetIntY() + 1u ) ] = '#';
	}

	for ( Int32 y = 0; y < static_cast< Int32 >( m_bufferResolution.GetIntY() + 1u ); ++y )
	{
		m_frameBuffer[ ( m_bufferResolution.GetIntX() + 3u ) * y ] = '#';
		m_frameBuffer[ m_bufferResolution.GetIntX() + 1 + ( m_bufferResolution.GetIntX() + 3u ) * y ] = '#';
		m_frameBuffer[ m_bufferResolution.GetIntX() + 2u + ( m_bufferResolution.GetIntX() + 3u ) * y ] = '\n';
	}

	m_frameBuffer[ ( m_bufferResolution.GetIntX() + 3u ) * ( m_bufferResolution.GetIntY() + 2u ) - 1u ] = 0;

	for ( Int32 x = -GetMaxX(); x <= GetMaxX(); ++x )
	{
		for ( Int32 y = -GetMaxY(); y < 0; ++y )
		{
			SetPixel( x, y, '/' );
		}

		SetPixel( x, 0, '-' );

		for ( Int32 y = 1; y <= GetMaxY(); ++y )
		{
			SetPixel( x, y, '\\' );
		}
	}
}

Int32 Renderer::GetMaxX()
{
	return m_resolution.GetIntX() / 2;
}

Int32 Renderer::GetMaxY()
{
	return m_resolution.GetIntY() / 2;
}
