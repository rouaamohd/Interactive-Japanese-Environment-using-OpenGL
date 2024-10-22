#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

const float FOV = 45.0f;

Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(0.0f, 1.0f, 0.0f),
	mRight(0.0f, 0.0f, 0.0f),
	World_Up(0.0f, 1.0f, 0.0f),
	mYaw(glm::pi<float>()),
	mPitch(0.0f),
	mFOV(FOV)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}


const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}
const glm::vec3& Camera::getLook() const
{
	return mLook;
}
const glm::vec3& Camera::getRight() const
{
	return mRight;
}

const glm::vec3& Camera::getUp() const
{
	return mUp;
}



// Implement FPS Camera
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

// start camera looking at a point and target to look at
FPSCamera::FPSCamera(glm::vec3 postion, glm::vec3 target)
{
	mPosition = postion;
	mTargetPos = target;

	glm::vec3 lookDir = postion - target;

	mPitch = -atan2(lookDir.y, sqrt(lookDir.x * lookDir.x + lookDir.z * lookDir.z));
	mYaw = atan2(lookDir.x, lookDir.z) + glm::pi<float>();
}

void FPSCamera::setPosition(const glm::vec3& position)
{
	mPosition = position;
}

void FPSCamera::rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	//contrain the angles
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	if (mYaw > glm::two_pi<float>())
		mYaw -= glm::two_pi<float>();
	else if (mYaw < 0.0f)
		mYaw += glm::two_pi<float>();

	updateCameraVector();
}

void FPSCamera::move(const glm::vec3& offestpos)
{
		mPosition.x += offestpos.x;
		mPosition.z += offestpos.z;
}


void FPSCamera::updateCameraVector()
{
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	mRight = glm::normalize(glm::cross(mLook, World_Up));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}