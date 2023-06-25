#pragma once
class ShaderToy
{
public:
	void Update( Float dt );
	void DrawPixel(const Vector2& uv, const Vector3& eyePos, const Matrix& viewMatrix );

private:
	Renderer m_renderer = Renderer( 71, 1.0f, 2u );
};

