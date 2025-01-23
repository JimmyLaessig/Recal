export module CameraController;

import Math;

import Engine.Component;

import <chrono>;

class CameraController : public Engine::InheritsFrom<CameraController, Engine::Component>
{

public:

	void onConstruct() override;

	void onUpdate(float deltaT) override;

private:

	std::chrono::time_point<std::chrono::system_clock> mPreviousTimePoint;

	/// Current velocity vector of the camera
	Math::Vec3 mVelocity{ 0.f, 0.f, 0.f };

	/// Constant camera acceleration in m/s^2
	float mAcceleration{ 1.0f };

	/// Slow-down factor if the camera is not accelerating
	float mSlowDownFactor{ 0.8f };

	/// Maximum speed in m/s
	float mMaxSpeed{ 3.0 };

	/// rotation speed
	float mRotationSpeed{ 3.14159265358979323846264338327950288f };

	/// The position of the mouse in the previous frame
	Math::Vec2 mPreviousMousePos;
};