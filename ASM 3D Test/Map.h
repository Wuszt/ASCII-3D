#pragma once

struct Map
{
	static constexpr Uint32 c_dimension = 22;
	//https://www.youtube.com/watch?v=eOCQfxRQ2pY

	Bool Cells[ c_dimension ][ c_dimension ] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	Bool IsYEdgeOccupied( Int32 x, Int32 y )
	{
		return y < 0 || y >= c_dimension || x < 0 || x >= c_dimension || Cells[ c_dimension - 1 - y ][ x ] || Cells[ c_dimension - y ][ x ];
	}

	Bool IsXEdgeOccupied( Int32 x, Int32 y )
	{
		return y < 0 || y >= c_dimension || x < 0 || x >= c_dimension || Cells[ c_dimension - y - 1 ][ x ] || Cells[ c_dimension - y - 1 ][ x - 1 ];
	}

	Float GetWallHeight( const Vector2& pos, Float directionAngle, Float offsetAngle )
	{
		Float finalAngle = directionAngle + offsetAngle;

		if( finalAngle > FORGE_PI_TWO )
		{
			finalAngle -= FORGE_PI_TWO;
		}
		else if( finalAngle < 0.0f )
		{
			finalAngle += FORGE_PI_TWO;
		}

		Float tileStepX = ( ( FORGE_PI_HALF > finalAngle ) || ( finalAngle > FORGE_PI + FORGE_PI_HALF ) ) ? 1.0f : -1.0f;
		Float tileStepY = ( finalAngle > 0.0f && FORGE_PI > finalAngle ) ? 1.0f : -1.0f;
		Int32 currentXTile = static_cast< Int32 >( floorf( pos.X ) );
		Int32 currentYTile = static_cast< Int32 >( floorf( pos.Y ) );

		Float dx = pos.X - currentXTile;
		Float dy = pos.Y - currentYTile;

		currentXTile += 1;
		currentYTile += 1;

		Float stepX = Math::Tg( finalAngle );
		Float stepY = Math::Ctg( finalAngle );

		Float intersectionWithX = pos.Y + ( 1.0f - dx ) * stepX;
		Float intersectionWithY = pos.X + ( 1.0f - dy ) * stepY;

		while( true )
		{
			while( intersectionWithX < currentYTile )
			{
				if( IsXEdgeOccupied( currentXTile, static_cast< Int32 >( intersectionWithX ) ) )
				{
					Vector2 diff = Vector2{ static_cast<Float>( currentXTile ),  intersectionWithX } -pos;
					Float d = diff.Mag();
					return 100.0f / d;
				}
				currentXTile += static_cast< Int32 >( tileStepX );
				intersectionWithX += stepX;
			}

			while( intersectionWithY < currentXTile )
			{
				if( IsYEdgeOccupied( static_cast< Int32 >( intersectionWithY ), currentYTile ) )
				{
					Vector2 diff = Vector2{ intersectionWithY, static_cast<Float>( currentYTile ) } -pos;
					Float d = diff.Mag();
					return 100.0f / d;
				}
				currentYTile += static_cast< Int32 >( tileStepY );
				intersectionWithY += stepY;
			}
		}



		return 0.0f;
	}

};

