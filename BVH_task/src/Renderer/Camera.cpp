/* Camera.cpp
 * Author : Kozhukharov Nikita
 */

#include <Renderer/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>


void Camera::updateViewMatrix() {
	viewMatrix = std::move(glm::lookAt(position, position + direction, up));
}

Camera::Camera(glm::vec3 pos) 
	: position(std::move(pos)) {}

glm::mat4 Camera::getViewMatrix() {
	updateViewMatrix();
	return viewMatrix;
}

glm::mat4 Camera::getProjMatrix(float projAngle, float width_height, float near, float far) {
	return glm::perspective(glm::radians(projAngle), width_height, near, far);
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
}

void Camera::moveByMouse(float xoffset, float yoffset, bool constrainPitch) {
	const float maxPitchAbs = 89.0;
	/// todo: process pitch to move around target point
	yaw += xoffset * mouseSensitivity;
	pitch += -yoffset * mouseSensitivity;
	pitch = std::clamp(pitch, -maxPitchAbs, maxPitchAbs);
	updateDirVector();
}

void Camera::updateDirVector() {
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = std::move(glm::normalize(direction));
	right = std::move(glm::normalize(glm::cross(direction, up)));
}

