#pragma once
#include "../Math/Vector2.h"
#include "../Core/Types.h"

struct VectorInt2 : public Vector2
{
	VectorInt2() = default;

	VectorInt2( Int32 x, Int32 y )
		: Vector2( static_cast< Float >( x ), static_cast< Float >( y ) )
	{}

	Int32 GetIntX() const { return static_cast< Int32 >( X ); }
	Int32 GetIntY() const { return static_cast< Int32 >( Y ); }
};