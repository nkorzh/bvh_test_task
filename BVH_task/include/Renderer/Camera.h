/* Camera.h
 * Author : Kozhukharov Nikita
 */

#pragma once

#include <glm/glm.hpp>

class Camera {
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::mat4 viewMatrix;
	
	float yaw = -63.6f;
	float pitch = -23.1f;

	float movementSpeed = 1.8f;
	float mouseSensitivity = 0.1f;
	float zoom = 45.0f;

public:
	 enum class CameraDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	Camera(glm::vec3 pos = glm::vec3(-2.0f, 2.7f, 4.0f));
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix(float projAngle, float width_height, float near, float far);
	void moveByKeys(CameraDirection dir, float deltaTime);
	void moveByMouse(float xoffset, float yoffset, bool constrainPitch = true);
	void printCameraSettings();
private:
	void updateViewMatrix();
	void updateDirVector();
};

