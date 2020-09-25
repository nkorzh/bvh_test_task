/* Camera.cpp
 * Author : Kozhukharov Nikita
 */

#include <Renderer/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

/// debug
#include <iostream>

void Camera::updateViewMatrix() {
	viewMatrix = std::move(glm::lookAt(position, position + direction, up));
}

Camera::Camera(glm::vec3 pos) : position(std::move(pos)) {
	updateDirVector();
}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() {
	return projMatrix;
}

void Camera::recountProjMatrix(float projAngle, float width_height, float near, float far) {
	projMatrix = glm::perspective(glm::radians(projAngle), width_height, near, far);
}

void Camera::moveByKeys(CameraDirection dir, float deltaTime) {
	float shiftAmount = movementSpeed * deltaTime;
	switch (dir) {
		case CameraDirection::FORWARD:
			position += direction * shiftAmount;
			break;
		case CameraDirection::BACKWARD:
			position -= direction * shiftAmount;
			break;
		case CameraDirection::LEFT:
			position -= right * shiftAmount;
			break;
		case CameraDirection::RIGHT:
			position += right * shiftAmount;
			break;
	}
	updateViewMatrix();
}

void Camera::moveByMouse(float xoffset, float yoffset, bool constrainPitch) {
	const float maxPitchAbs = 89.0;
	if (!constrainPitch)
		return;
	/// todo: process pitch to move around target point
	yaw += xoffset * mouseSensitivity;
	pitch += -yoffset * mouseSensitivity;
	pitch = std::clamp(pitch, -maxPitchAbs, maxPitchAbs);
	updateDirVector();
}

void Camera::changeStreifSpeed(int val, float deltaTime) {
	const float shift = 5.0;
	val = std::clamp(val, -1, 1);
	movementSpeed = std::clamp(movementSpeed + shift * val * deltaTime, 0.5f, 5.0f);
}

void Camera::printCameraSettings() {
	std::cout << "Position: " << position.x << " " << position.y <<
		" " << position.z << "\nYaw: " << yaw << "\nPitch: " << pitch << "\n\n\n";
}

void Camera::updateDirVector() {
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = std::move(glm::normalize(direction));
	right = std::move(glm::normalize(glm::cross(direction, up)));
	updateViewMatrix();
}
