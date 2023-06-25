#pragma once
#include "Helpers.h"

class Renderer
{
	typedef void* HANDLE;
public:
	Renderer( Uint32 height, Float aspectRatio, Uint32 pixelWidth);

	void Draw();
	void SetPixel( Int32 x, Int32 y, Char value );
	void SetPixel( Int32 x, Int32 y, Float brightness );
	void FillLine( Int32 line, Char pixel );
	char GetPixelChar( Float brightness );
	Int32 GetMaxX();
	Int32 GetMaxY();

private:
	void InitializeBuffer();

	VectorInt2 m_resolution;
	VectorInt2 m_bufferResolution;

	HANDLE m_backBuffer;
	HANDLE m_frontBuffer;

	Int32 m_pixelWidth = 1u;

	std::vector< Char > m_frameBuffer;
};

