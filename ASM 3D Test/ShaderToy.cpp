#include "pch.h"
#include "ShaderToy.h"
#include <thread>

const int STEPS_AMOUNT = 750;
const float STEP_VALUE = 0.02f;
const float AMBIENT_LIGHT = 0.1f;

float SphereSDF( const Vector3& p )
{
	return p.Mag() - 1.0f;
}

float CubeSDF( const Vector3& p )
{
	Vector3 d = Vector3{ Math::Abs( p.X ), Math::Abs( p.Y ), Math::Abs( p.Z ) } - Vector3( 1.0f, 1.0f, 1.0f );

	float inside = Math::Min( 0.0f, Math::Max( d.X, Math::Max( d.Y, d.Z ) ) );

	float outside = Math::Max( Math::Max(d.X, Math::Max(d.Y, d.Z ) ), 0.0f );

	return inside + outside;
}

float Intersect( float a, float b )
{
	return Math::Max( a, b );
}

float Difference( float a, float b )
{
	return Intersect( a, -b );
}

float Union( float a, float b )
{
	return Math::Min( a, b );
}

float SceneSDF( const Vector3& p )
{
	Vector3 a = p * ( 1.0f / 1.2f );
	return Union( SphereSDF( Vector3( sin( forge::Time::GetTime() ) * 5.0f, 0.0f, 0.0f ) + p * 2.0f ) * 0.5f, Difference( CubeSDF( p ) * 1.2f, SphereSDF( a ) * 1.2f ) );
}

Vector3 GetNormal( const Vector3& p )
{
	float e = 0.00001f;

	Vector3 normal;
	normal.X = SceneSDF( Vector3( p.X + e, p.Y, p.Z ) ) - SceneSDF( Vector3( p.X - e, p.Y, p.Z ) );
	normal.Y = SceneSDF( Vector3( p.X, p.Y + e, p.Z ) ) - SceneSDF( Vector3( p.X, p.Y - e, p.Z ) );
	normal.Z = SceneSDF( Vector3( p.X, p.Y, p.Z + e ) ) - SceneSDF( Vector3( p.X, p.Y, p.Z - e ) );

	return normal.Normalized();
}

Float ProcessLighting( const Vector3& eyePos, const Vector3& surfacePoint )
{
	Vector3 diffuseDir = Vector3( sin( forge::Time::GetTime() ), cos( forge::Time::GetTime() ), 1.0f ).Normalized();
	float diffuseIntensity = 0.7f;

	float color = AMBIENT_LIGHT;

	Vector3 normal = GetNormal( surfacePoint );

	float d = ( -diffuseDir ).Dot( normal );

	d = abs( d );

	color += d * diffuseIntensity;

	return color;
}

Float GetColor( Vector3 eyePos, Vector3 dir )
{
	for ( int i = 0; i < STEPS_AMOUNT; ++i )
	{
		Vector3 currentPoint = eyePos + dir * float( i ) * STEP_VALUE;
		if ( SceneSDF( currentPoint ) < 0.0f )
		{
			return ProcessLighting( eyePos, currentPoint );
		}
	}

	return 0.0f;
}

Vector3 rayDirection( float fieldOfView, Vector2 size, Vector2 fragCoord ) {
	Vector2 xy = fragCoord - Vector2{size.X / 2.0f, size.Y / 2.0f};
	float z =  static_cast< Float >( size.Y / tan( DEG2RAD * ( fieldOfView ) / 2.0 ) );
	return Vector3( xy, z ).Normalized();
}

Matrix GetViewMatrix( Vector3 eyePos, Vector3 lookAt, Vector3 worldUp )
{
	Vector3 forward = lookAt - eyePos;
	Vector3 side = forward.Cross( worldUp );
	Vector3 up = side.Cross( forward );

	forward.Normalize();
	side.Normalize();
	up.Normalize();

	return Matrix(
		Vector4( side, 0.0f ),
		Vector4( up, 0.0f ),
		Vector4( forward, 0.0f ),
		Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) );
}

void ShaderToy::Update( Float dt )
{
	const Vector3 eyePos = Vector3( 8.0f * sin( forge::Time::GetTime() ), sin( forge::Time::GetTime() ) * 5.0f, 8.0f * cos( forge::Time::GetTime() ) );
	const Matrix viewMatrix = GetViewMatrix( eyePos, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );

	auto drawFunc = [ this, &viewMatrix, &eyePos ]( Int32 fromY, Int32 toY )
	{
		for ( Int32 y = fromY; y < toY; ++y )
		{
			for ( Int32 x = 0; x < m_renderer.GetResolution().GetIntX(); ++x )
			{
				DrawPixel( { static_cast< Float >( x ),static_cast< Float >( y ) }, eyePos, viewMatrix );
			}
		}
	};

	const Int32 c_threadsAmount = 3; 
	const Int32 linesAmount = m_renderer.GetResolution().GetIntY();
	const Int32 linesPerThread = linesAmount / (c_threadsAmount + 1);
	std::vector< std::thread > threads;

	for ( Int32 i = 0; i < c_threadsAmount; ++i )
	{
		threads.emplace_back( drawFunc, linesPerThread * i, linesPerThread * ( i + 1 ) );
	}

	drawFunc( c_threadsAmount * linesPerThread, m_renderer.GetResolution().GetIntY() );

	for ( auto& thread : threads )
	{
		thread.join();
	}

	m_renderer.Draw();
}

void ShaderToy::DrawPixel( const Vector2& uv, const Vector3& eyePos, const Matrix& viewMatrix )
{
	Vector3 dir = rayDirection( 45.0f, m_renderer.GetResolution(), uv );

	dir = viewMatrix.TransformVector( Vector4( dir, 0.0f ) ).AsVector3();

	m_renderer.SetPixel( static_cast< Int32 >( uv.X ) - m_renderer.GetResolution().GetIntX() / 2, static_cast< Int32 >( uv.Y ) - m_renderer.GetResolution().GetIntY() / 2, 1.5f * GetColor( eyePos, dir ) );
}
