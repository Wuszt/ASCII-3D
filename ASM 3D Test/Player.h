#pragma once
class Player
{
public:
	const Vector2& GetPosition() const
	{
		return m_position;
	}

	Float GetAngle() const
	{
		return m_angle;
	}

	void Update( Float dt );

private:
	Vector2 m_position = {5.5f,5.5f};
	Float m_angle = 0.0f;
};

