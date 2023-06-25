#pragma once
#include "Helpers.h"

class Renderer
{
	typedef void* HANDLE;
public:
	Renderer( Uint32 height, Float aspectRatio, Uint32 pixelWidth);

	void Draw();
	void SetPixel( Int32 x, Int32 y, char value );
	void SetPixel( Int32 x, Int32 y, float brightness );
	char GetPixelChar( Float brightness );
	void InitializeBuffer();
	Int32 GetMaxX();
	Int32 GetMaxY();

private:
	VectorInt2 m_resolution;
	VectorInt2 m_bufferResolution;

	HANDLE m_backBuffer;
	HANDLE m_frontBuffer;
	HANDLE m_secondBuffer;

	Int32 m_pixelWidth = 1u;

	std::vector< Char > m_frameBuffer;
};

