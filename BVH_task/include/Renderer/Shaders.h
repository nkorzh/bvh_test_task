/* Shaders.h
 * Author : Kozhukharov Nikita
 */

#pragma once

#include <string>


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
	bool valid;
	unsigned int programId;
public:
	ShaderProgram(const char*, const char*);
	bool isValid() const;
	void use() const;
	// reload if necessary
};
