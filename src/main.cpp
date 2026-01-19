#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

enum ShaderType {
    VERTEX,
    FRAGMENT
};

void getShaderCompilationError(unsigned int shaderID, GLenum ERROR_TYPE, ShaderType type) {
    int success;
    char log[512];
    std::string stype;
    if (type == VERTEX) {
        stype = "VERTEX";
    }
    else {
        stype = "FRAGMENT";
    }
    glGetShaderiv(shaderID, ERROR_TYPE, &success);

    if (!success) {
        glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", stype.c_str(), log);
    }
}

void getShaderLinkError(unsigned int program, GLenum ERROR_TYPE) {
    int success;
    char log[512];
    glGetProgramiv(program, ERROR_TYPE, &success);

    if (!success) {
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", log);
    }
}

int main(void)
{
    GLFWwindow* window;
    int windowWidth = 800;
    int windowHeight = 600;
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    float rectangle[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int indicies[] = {
        0, 1, 3,
        1, 2, 3
    };

    /* Initialize the library */
    if (!glfwInit())
    {
        printf("Crash at glfw init\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "if you read this, you are gae", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Crash at window\n");
        return -1;
    }
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Calculate centered position
    
	printf("Screen Width: %d, Height: %d\n", mode->width, mode->height);

    int xpos = (mode->width - windowWidth) / 2;
    int ypos = (mode->height - windowHeight) / 2;

    // Set position
    glfwSetWindowPos(window, xpos, ypos);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    getShaderCompilationError(vertexShader, GL_COMPILE_STATUS, VERTEX);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    getShaderCompilationError(fragmentShader, GL_COMPILE_STATUS, FRAGMENT);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    getShaderLinkError(shaderProgram, GL_LINK_STATUS);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        /* Render here */
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO2);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}