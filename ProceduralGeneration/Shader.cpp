#include "Shader.h"


ShaderSource Shader::ReadShader(const std::string & filePath)
{
	std::ifstream shaderFile(filePath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream strStream[2];
	ShaderType type = ShaderType::NONE;

	while (getline(shaderFile, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				//set to vertex shader
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				//set to fragment shader
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			strStream[(int)type] << line << '\n';
		}
	}

	return { strStream[0].str(), strStream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string source)
{
	unsigned int shaderID = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);
	//Error Handling
	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::string shaderTypeText;
		if (type == GL_VERTEX_SHADER)
			shaderTypeText = "VERTEX";
		else if (type == GL_FRAGMENT_SHADER)
			shaderTypeText = "FRAGMENT";
		else if (type == GL_TESS_EVALUATION_SHADER)
			shaderTypeText = "TESS EVALUATION";
		else if (type == GL_GEOMETRY_SHADER)
			shaderTypeText = "GEOMETRY";
		else if (type == GL_TESS_CONTROL_SHADER)
			shaderTypeText = "TESS CONTROL";

		std::cout << "Error compiling " << shaderTypeText << " shader: " << std::endl;
		std::cout << message << std::endl;

		return 0;
	}

	return shaderID;
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string fragmentShader)
{
	vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


unsigned int Shader::GetProgramID()
{
	return id;
}

Shader::Shader()
{
	fileName = "";
}

void Shader::Bind() const
{
	glUseProgram(id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
#pragma region UNIFORMS

int Shader::GetUniformLocation(const std::string & name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}
	int location = glGetUniformLocation(id, name.c_str());

	if (location == -1) {
		std::cout << "Uniform " << name << " doesn't exist" << std::endl;
	}
	
	uniformLocationCache[name] = location;
	return location;
}



void Shader::SetUniform1i(const std::string & name, int var)
{
	glUniform1i(GetUniformLocation(name), var);
}

void Shader::SetUniform1f(const std::string & name, float f)
{
	glUniform1f(GetUniformLocation(name), f);
}

void Shader::SetUniformVec2(const std::string & name, glm::vec2 InputVec)
{
	glUniform2f(GetUniformLocation(name), InputVec.x, InputVec.y);
}

void Shader::SetUniformVec3(const std::string & name, glm::vec3 InputVec)
{
	glUniform3f(GetUniformLocation(name), InputVec.x, InputVec.y, InputVec.z);
}

void Shader::SetUniformVec4(const std::string & name, glm::vec4 InputVec)
{
	glUniform4f(GetUniformLocation(name), InputVec.x, InputVec.y, InputVec.z, InputVec.w);
}

void Shader::SetUniformMat4(const std::string & name, glm::mat4 InputMat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(InputMat));
}

#pragma endregion setting the uniforms


Shader::Shader(const std::string& file)
{
	source = ReadShader(file);
	fileName = file;
	id = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const char * vPath, const char * fPath)
{
	vertexPath = vPath;
	fragmentPath = fPath;
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vPath);
		fShaderFile.open(fPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}


Shader::~Shader()
{
	glDeleteProgram(id);
}
