module CameraController;


import Math;

import Engine.Application.Input;
import Engine.GameObject;

import <iostream>;

void
CameraController::onConstruct()
{
	mPreviousTimePoint = std::chrono::system_clock::now();

	auto pixelPos = Engine::Input::getMousePosition();
	mPreviousMousePos = { pixelPos.x, pixelPos.y };
}


void
CameraController::onUpdate(float deltaT)
{
	auto now = std::chrono::system_clock::now();
	
	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - mPreviousTimePoint).count();
	deltaT = delta * 1e-6;
	mPreviousTimePoint = now;

	Math::Vec3 velocity{ 0.f, 0.f, 0.f };
	velocity.z() += Engine::KeyState::PRESSED == Engine::Input::getKeyState(Engine::Key::W);
	velocity.z() -= Engine::KeyState::PRESSED == Engine::Input::getKeyState(Engine::Key::S);
	velocity.x() += Engine::KeyState::PRESSED == Engine::Input::getKeyState(Engine::Key::A);
	velocity.x() -= Engine::KeyState::PRESSED == Engine::Input::getKeyState(Engine::Key::D);

	auto length = Math::length(velocity);

	if (length > 0 )
	{
		velocity = velocity / length;

		auto currentSpeed = Math::length(mVelocity) + mAcceleration * deltaT;
		currentSpeed = std::min(currentSpeed, mMaxSpeed);

		mVelocity = velocity * currentSpeed;
	}
	else
	{
		mVelocity = mVelocity * mSlowDownFactor;
	}

	auto distanceTravelled = mVelocity * deltaT;

	auto distanceTravelledWS = transform().forward() * distanceTravelled.z() + transform().right() * distanceTravelled.x();
	transform().setWorldPosition(transform().worldPosition() + distanceTravelledWS);

	auto pixelPos = Engine::Input::getMousePosition();
	Math::Vec2 currentPixelPos = { pixelPos.x, pixelPos.y };

	auto deltaMousePos = currentPixelPos - mPreviousMousePos;

	mPreviousMousePos = currentPixelPos;

	if (Engine::Input::getMouseButtonState(Engine::MouseButton::BUTTON_RIGHT) == Engine::KeyState::PRESSED)
	{
		Math::Quat deltaX = Math::Quat::fromEulerAngles(Math::Vec3{ -deltaMousePos.y() * mRotationSpeed, 0.f, 0.f });
		Math::Quat deltaY = Math::Quat::fromEulerAngles(Math::Vec3{ 0.f, -deltaMousePos.x() * mRotationSpeed, 0.f });

		transform().setLocalRotation(deltaY * transform().localRotation() * deltaX);
	}
}