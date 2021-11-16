#pragma once

class Vector2D {
	public: 
		float x, y;

		Vector2D()
			:x(0.0f), y(0.0f)
		{
		}

		Vector2D(float xParam, float yParam)
			:x(xParam), y(yParam)
		{
		}

		// Multiply vector by scalar
		inline Vector2D operator * (const float scalar) const {
			return Vector2D(x * scalar, y * scalar);
		}
};