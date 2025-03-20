#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float x_mod = 0;
float y_mod = -5.0f;
float z_mod = -50.0f; 
float x_rot = 0;
float y_rot = 0;
float z_rot = 0;
float scale = 1.5f;
float camera_z = 10.0f;


void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) ||
        (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)))
    {
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            z_mod -= 1.f;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            z_mod += 1.f;
        }
    }
    if ((key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) ||
        (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) ||
        (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) ||
        (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)))
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            y_rot -= 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            y_rot += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            x_rot += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            x_rot -= 0.1f;
        }
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			z_rot += 0.1f;
		}
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            z_rot -= 0.1f;
        }
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        camera_z -= 1.0f; // Move the camera forward
    }
}

void checkShaderCompilation(GLuint shader) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << shader << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

void checkProgramLinking(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << program << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float height = 1080.0f;
    float width = 1920.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    int img_width,
        img_height,
        colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes = stbi_load("3D/rua com faixada.jpg", &img_width, &img_height, &colorChannels, 0);

    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* vert = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* frag = fragS.c_str();
    
    std::fstream skyboxVertSrc("Shaders/skyboxVertShader.vert");
    std::stringstream skyboxVertBuff;
    skyboxVertBuff << skyboxVertSrc.rdbuf();
    std::string skyboxVertS = skyboxVertBuff.str();
    const char* skyboxVert = skyboxVertS.c_str();

    std::fstream skyboxFragSrc("Shaders/skyboxFragShader.frag");
    std::stringstream skyboxFragBuff;
    skyboxFragBuff << skyboxFragSrc.rdbuf();
    std::string skyboxFragS = skyboxFragBuff.str();
    const char* skyboxFrag = skyboxFragS.c_str();
    

    // Compile vertex shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vert, NULL);
    glCompileShader(vertShader);
    checkShaderCompilation(vertShader);

    // Compile fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &frag, NULL);
    glCompileShader(fragShader);
    checkShaderCompilation(fragShader);

    // Link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    
    // Compile skybox vertex shader
    GLuint skyboxVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVertShader, 1, &skyboxVert, NULL);
    glCompileShader(skyboxVertShader);
    checkShaderCompilation(skyboxVertShader);

    // Compile skybox fragment shader
    GLuint skyboxFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFragShader, 1, &skyboxFrag, NULL);
    glCompileShader(skyboxFragShader);
    checkShaderCompilation(skyboxFragShader);

    // Link skybox shader program
    GLuint skyboxShaderProgram = glCreateProgram();
    glAttachShader(skyboxShaderProgram, skyboxVertShader);
    glAttachShader(skyboxShaderProgram, skyboxFragShader);
    glLinkProgram(skyboxShaderProgram);
    checkProgramLinking(skyboxShaderProgram);

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };



    unsigned int skyboxVao, skyboxVbo, skyboxEbo;
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);
    glGenBuffers(1, &skyboxEbo);

    glBindVertexArray(skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"
    };

    unsigned int skyboxTexture;
    glGenTextures(1, &skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, skyCChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &width, &height, &skyCChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            std::cout << "Loaded texture: " << facesSkybox[i] << " (" << width << "x" << height << ")" << std::endl;
        }
        else {
            std::cerr << "Failed to load texture: " << facesSkybox[i] << std::endl;
        }
        stbi_set_flip_vertically_on_load(true);
    }


    std::string path = "3D/untitled.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]); 
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]); 
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);        
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    GLuint vbo, vao, ebo, vbo_uv;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)normalPtr);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 


    std::string goKartPath = "uploads_files_4679076_GOkart.obj";
    std::vector<tinyobj::shape_t> goKartShapes;
    std::vector<tinyobj::material_t> goKartMaterials;
    std::string goKartWarning, goKartError;
    tinyobj::attrib_t goKartAttributes;

    bool goKartSuccess = tinyobj::LoadObj(&goKartAttributes, &goKartShapes, &goKartMaterials, &goKartWarning, &goKartError, goKartPath.c_str());

    if (!goKartSuccess) {
        std::cerr << "Failed to load object: " << goKartError << std::endl;
        if (!goKartWarning.empty()) {
            std::cerr << "Warning: " << goKartWarning << std::endl;
        }
        return -1;
    }
    else if (goKartShapes.empty()) {
        std::cerr << "Object has no shapes." << std::endl;
        return -1;
    }

    std::vector<GLuint> goKartMeshIndices;

    for (int i = 0; i < goKartShapes[0].mesh.indices.size(); i++)
    {
        goKartMeshIndices.push_back(goKartShapes[0].mesh.indices[i].vertex_index);
    }

    std::vector<GLfloat> goKartFullVertexData;

    for (int i = 0; i < goKartShapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = goKartShapes[0].mesh.indices[i];

        if (vData.vertex_index * 3 + 2 >= goKartAttributes.vertices.size() ||
            vData.normal_index * 3 + 2 >= goKartAttributes.normals.size() ||
            vData.texcoord_index * 2 + 1 >= goKartAttributes.texcoords.size()) {
            std::cerr << "Index out of bounds while accessing vertex data." << std::endl;
            return -1;
        }

        goKartFullVertexData.push_back(goKartAttributes.vertices[vData.vertex_index * 3]);
        goKartFullVertexData.push_back(goKartAttributes.vertices[vData.vertex_index * 3 + 1]);
        goKartFullVertexData.push_back(goKartAttributes.vertices[vData.vertex_index * 3 + 2]);
        goKartFullVertexData.push_back(goKartAttributes.normals[vData.normal_index * 3]);
        goKartFullVertexData.push_back(goKartAttributes.normals[vData.normal_index * 3 + 1]);
        goKartFullVertexData.push_back(goKartAttributes.normals[vData.normal_index * 3 + 2]);
        goKartFullVertexData.push_back(goKartAttributes.texcoords[vData.texcoord_index * 2]);
        goKartFullVertexData.push_back(goKartAttributes.texcoords[vData.texcoord_index * 2 + 1]);
    }

    // Load the texture for the new object (LP_Material.001_BaseColor.jpg)
    unsigned char* goKartTexBytes = stbi_load("LP_Material.001_BaseColor.png", &img_width, &img_height, &colorChannels, 0);

    GLuint goKartTexture;
    glGenTextures(1, &goKartTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, goKartTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, goKartTexBytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(goKartTexBytes);

    GLuint goKartVbo, goKartVao, goKartEbo;
    glGenVertexArrays(1, &goKartVao);
    glGenBuffers(1, &goKartVbo);
    glBindVertexArray(goKartVao);
    glBindBuffer(GL_ARRAY_BUFFER, goKartVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* goKartFullVertexData.size(), goKartFullVertexData.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)normalPtr);

    uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::string goKartPath2 = "uploads_files_4679076_GOkart.obj";
    std::vector<tinyobj::shape_t> goKartShapes2;
    std::vector<tinyobj::material_t> goKartMaterials2;
    std::string goKartWarning2, goKartError2;
    tinyobj::attrib_t goKartAttributes2;

    bool goKartSuccess2 = tinyobj::LoadObj(&goKartAttributes2, &goKartShapes2, &goKartMaterials2, &goKartWarning2, &goKartError2, goKartPath2.c_str());

    if (!goKartSuccess2) {
        std::cerr << "Failed to load object: " << goKartError2 << std::endl;
        if (!goKartWarning2.empty()) {
            std::cerr << "Warning: " << goKartWarning2 << std::endl;
        }
        return -1;
    }
    else if (goKartShapes2.empty()) {
        std::cerr << "Object has no shapes." << std::endl;
        return -1;
    }

    std::vector<GLuint> goKartMeshIndices2;

    for (int i = 0; i < goKartShapes2[0].mesh.indices.size(); i++) {
        goKartMeshIndices2.push_back(goKartShapes2[0].mesh.indices[i].vertex_index);
    }

    std::vector<GLfloat> goKartFullVertexData2;

    for (int i = 0; i < goKartShapes2[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = goKartShapes2[0].mesh.indices[i];

        if (vData.vertex_index * 3 + 2 >= goKartAttributes2.vertices.size() ||
            vData.normal_index * 3 + 2 >= goKartAttributes2.normals.size() ||
            vData.texcoord_index * 2 + 1 >= goKartAttributes2.texcoords.size()) {
            std::cerr << "Index out of bounds while accessing vertex data." << std::endl;
            return -1;
        }

        goKartFullVertexData2.push_back(goKartAttributes2.vertices[vData.vertex_index * 3]);
        goKartFullVertexData2.push_back(goKartAttributes2.vertices[vData.vertex_index * 3 + 1]);
        goKartFullVertexData2.push_back(goKartAttributes2.vertices[vData.vertex_index * 3 + 2]);
        goKartFullVertexData2.push_back(goKartAttributes2.normals[vData.normal_index * 3]);
        goKartFullVertexData2.push_back(goKartAttributes2.normals[vData.normal_index * 3 + 1]);
        goKartFullVertexData2.push_back(goKartAttributes2.normals[vData.normal_index * 3 + 2]);
        goKartFullVertexData2.push_back(goKartAttributes2.texcoords[vData.texcoord_index * 2]);
        goKartFullVertexData2.push_back(goKartAttributes2.texcoords[vData.texcoord_index * 2 + 1]);
    }

    // Load the texture for the second GoKart
    unsigned char* goKartTexBytes2 = stbi_load("LP_Material.001_BaseColor.png", &img_width, &img_height, &colorChannels, 0);

    GLuint goKartTexture2;
    glGenTextures(1, &goKartTexture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, goKartTexture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, goKartTexBytes2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(goKartTexBytes2);

    GLuint goKartVbo2, goKartVao2, goKartEbo2;
    glGenVertexArrays(1, &goKartVao2);
    glGenBuffers(1, &goKartVbo2);
    glBindVertexArray(goKartVao2);
    glBindBuffer(GL_ARRAY_BUFFER, goKartVbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * goKartFullVertexData2.size(), goKartFullVertexData2.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)normalPtr);

    uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Repeat the same process for the third GoKart
    std::string goKartPath3 = "uploads_files_4679076_GOkart.obj";
    std::vector<tinyobj::shape_t> goKartShapes3;
    std::vector<tinyobj::material_t> goKartMaterials3;
    std::string goKartWarning3, goKartError3;
    tinyobj::attrib_t goKartAttributes3;

    bool goKartSuccess3 = tinyobj::LoadObj(&goKartAttributes3, &goKartShapes3, &goKartMaterials3, &goKartWarning3, &goKartError3, goKartPath3.c_str());

    if (!goKartSuccess3) {
        std::cerr << "Failed to load object: " << goKartError3 << std::endl;
        if (!goKartWarning3.empty()) {
            std::cerr << "Warning: " << goKartWarning3 << std::endl;
        }
        return -1;
    }
    else if (goKartShapes3.empty()) {
        std::cerr << "Object has no shapes." << std::endl;
        return -1;
    }

    std::vector<GLuint> goKartMeshIndices3;

    for (int i = 0; i < goKartShapes3[0].mesh.indices.size(); i++) {
        goKartMeshIndices3.push_back(goKartShapes3[0].mesh.indices[i].vertex_index);
    }

    std::vector<GLfloat> goKartFullVertexData3;

    for (int i = 0; i < goKartShapes3[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = goKartShapes3[0].mesh.indices[i];

        if (vData.vertex_index * 3 + 2 >= goKartAttributes3.vertices.size() ||
            vData.normal_index * 3 + 2 >= goKartAttributes3.normals.size() ||
            vData.texcoord_index * 2 + 1 >= goKartAttributes3.texcoords.size()) {
            std::cerr << "Index out of bounds while accessing vertex data." << std::endl;
            return -1;
        }

        goKartFullVertexData3.push_back(goKartAttributes3.vertices[vData.vertex_index * 3]);
        goKartFullVertexData3.push_back(goKartAttributes3.vertices[vData.vertex_index * 3 + 1]);
        goKartFullVertexData3.push_back(goKartAttributes3.vertices[vData.vertex_index * 3 + 2]);
        goKartFullVertexData3.push_back(goKartAttributes3.normals[vData.normal_index * 3]);
        goKartFullVertexData3.push_back(goKartAttributes3.normals[vData.normal_index * 3 + 1]);
        goKartFullVertexData3.push_back(goKartAttributes3.normals[vData.normal_index * 3 + 2]);
        goKartFullVertexData3.push_back(goKartAttributes3.texcoords[vData.texcoord_index * 2]);
        goKartFullVertexData3.push_back(goKartAttributes3.texcoords[vData.texcoord_index * 2 + 1]);
    }

    // Load the texture for the third GoKart
    unsigned char* goKartTexBytes3 = stbi_load("LP_Material.001_BaseColor.png", &img_width, &img_height, &colorChannels, 0);

    GLuint goKartTexture3;
    glGenTextures(1, &goKartTexture3);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, goKartTexture3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, goKartTexBytes3);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(goKartTexBytes3);

    GLuint goKartVbo3, goKartVao3, goKartEbo3;
    glGenVertexArrays(1, &goKartVao3);
    glGenBuffers(1, &goKartVbo3);
    glBindVertexArray(goKartVao3);
    glBindBuffer(GL_ARRAY_BUFFER, goKartVbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * goKartFullVertexData3.size(), goKartFullVertexData3.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)normalPtr);

    uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glEnable(GL_DEPTH_TEST);

    //glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f); 
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 100.0f);

    glm::vec3 lightPos = glm::vec3(-10, 3, 0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 1.57f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 1.0f;
    float specPhong = 16;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        // Inside the while loop

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, camera_z);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Camera front direction
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Camera up direction

        // Move the camera forward when the W key is pressed
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraFront * 0.1f; // Adjust the speed as needed
        }

        // Calculate the view matrix
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProgram);
        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        // Set the view and projection matrices for the skybox
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(sky_view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProgram, "projection");
        glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProgram, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        // Bind the skybox VAO and texture, then draw the skybox
        glBindVertexArray(skyboxVao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Re-enable depth writing and set depth function back to less than
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);

        // Create transformation matrix for the road
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x_mod, y_mod, z_mod));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, glm::vec3(scale * 50.0f, scale, scale));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Pass transformation matrix to shader
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindTexture(GL_TEXTURE_2D, texture);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glUniform1i(tex0Address, 0);

        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform3fv(ambientStrAddress, 1, glm::value_ptr(ambientColor));
        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform1f(ambientColorAddress, ambientStr);

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);

        // Draw the GoKart object
        glm::mat4 goKartTransform = glm::mat4(1.0f);
        goKartTransform = glm::translate(goKartTransform, glm::vec3(0.0f, 1.0f, -50.0f)); // Position the GoKart above the road
        goKartTransform = glm::scale(goKartTransform, glm::vec3(1.0f, 1.0f, 1.0f)); // Adjust the scale as needed

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(goKartTransform));

        glBindTexture(GL_TEXTURE_2D, goKartTexture);
        GLuint tex1Address = glGetUniformLocation(shaderProgram, "tex1");
        glUniform1i(tex1Address, 1);

        glBindVertexArray(goKartVao);
        glDrawArrays(GL_TRIANGLES, 0, goKartFullVertexData.size() / 8);

        // Draw the second GoKart object
        glm::mat4 goKartTransform2 = glm::mat4(1.0f);
        goKartTransform2 = glm::translate(goKartTransform2, glm::vec3(5.0f, 1.0f, -50.0f)); // Position the GoKart above the road
        goKartTransform2 = glm::scale(goKartTransform2, glm::vec3(1.0f, 1.0f, 1.0f)); // Adjust the scale as needed

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(goKartTransform2));

        glBindTexture(GL_TEXTURE_2D, goKartTexture2);
        GLuint tex1Address2 = glGetUniformLocation(shaderProgram, "tex1");
        glUniform1i(tex1Address2, 1);

        glBindVertexArray(goKartVao2);
        glDrawArrays(GL_TRIANGLES, 0, goKartFullVertexData2.size() / 8);

        // Draw the third GoKart object
        glm::mat4 goKartTransform3 = glm::mat4(1.0f);
        goKartTransform3 = glm::translate(goKartTransform3, glm::vec3(-5.0f, 1.0f, -50.0f)); // Position the GoKart above the road
        goKartTransform3 = glm::scale(goKartTransform3, glm::vec3(1.0f, 1.0f, 1.0f)); // Adjust the scale as needed

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(goKartTransform3));

        glBindTexture(GL_TEXTURE_2D, goKartTexture3);
        GLuint tex1Address3 = glGetUniformLocation(shaderProgram, "tex1");
        glUniform1i(tex1Address3, 1);

        glBindVertexArray(goKartVao3);
        glDrawArrays(GL_TRIANGLES, 0, goKartFullVertexData3.size() / 8);

        glEnd();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}
