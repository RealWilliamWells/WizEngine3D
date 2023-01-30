//
// Created by william on 25/01/23.
//

#ifndef WIZENGINE3D_SHADER_H
#define WIZENGINE3D_SHADER_H

#include "GL/glew.h"
#include <string>

namespace wiz {
    class Shader;
}

class wiz::Shader {
public:
	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath);

    void compileAndLinkProgramShaders(const char *vShaderCode, const char *fShaderCode);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif //WIZENGINE3D_SHADER_H
