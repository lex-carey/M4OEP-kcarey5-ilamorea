#include "shaderManager.h"
#include <fstream>
#include <sstream>
#include "stb_image.h"


ShaderManager::~ShaderManager() {
    clear();
}

Shader ShaderManager::loadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name) {
    return shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

Shader &ShaderManager::getShader(std::string name) {
    return shaders[name];
}

Texture2D ShaderManager::loadTexture(const char *file, bool alpha, std::string name) {
    return textures[name] = loadTextureFromFile(file, alpha);
}

Texture2D ShaderManager::getTexture(std::string name) {
    return textures[name];
}

void ShaderManager::clear() {
    // delete all shaders: "iter" here is const std::pair<std::string, Shader>&, so we need to use
    // "iter.second" to get the Shader, and delete the program by ID
    for (const auto &iter: shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ShaderManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception &e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ShaderManager::loadTextureFromFile(const char *file, bool alpha) {
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}