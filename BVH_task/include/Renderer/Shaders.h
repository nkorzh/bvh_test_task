/* Shaders.h
 * Author : Kozhukharov Nikita
 */

#pragma once

#include <string>
#include <glm/glm.hpp>

class ShaderProgram {
	class Shader {
		unsigned int id;
		unsigned int type;
		bool valid;
	public:
		Shader(int, const char*);
		void reload(const char*);
		bool isValid();
		unsigned int getId();
		~Shader();
	};
	int getUniformLocation(const std::string& varName) const;
	bool valid;
	unsigned int programId;
public:
	ShaderProgram(const char*, const char*);
	bool isValid() const;
	void use() const;
	unsigned int getProgramId();
	// uniform setters
	void setVec3(const glm::vec3&, const std::string&) const;
	void setVec4(const glm::vec4&, const std::string&) const;
	void setMat4(const glm::mat4&, const std::string&) const;
	void setBool(const bool&, const std::string&) const;
	// reload if necessary
};

