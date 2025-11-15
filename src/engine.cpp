#include "engine.h"
#include <fstream>
using namespace std;


enum state {start, play, over};
state screen;

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "flappy bat", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // Relative file path from inside cmake-build-debug folder to the txt file
    readFromFile("../res/art/scene.txt");
}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // If mouse overlaps with spawn button, change color
    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (mousePressed == true) {
        screen = play;
    }
    // Save mousePressed for next frame
    mousePressedLastFrame = mousePressed;
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (screen == play){
        for (Cloud& c : clouds) {
            c.moveXWithinBounds(-1, width);
        }

    }
}
void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    if (screen == play) {
        for (unique_ptr<Shape> &square : squares) {
            square->setUniforms();
            square->draw();
        }

        for (Cloud& c : clouds) {
            c.setUniformsAndDraw();
        }
    }

    switch (screen) {
        case start: {
            string message = "Press m1 to start";
            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            // NOTE: This line changes the shader being used to the font shader.
            //  If you want to draw shapes again after drawing text,
            //  you'll need to call shapeShader.use() again first.
         //   this->fontRender->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            break;
        }
        /*
        case play: {
            //TODO: Figure out what we exactly need here.
            // Render font on top of spawn button
            for (unique_ptr<Shape> &c : confetti) {
                c->setUniforms();
                c->draw();
            }
            spawnButton->setUniforms();
            this->spawnButton->draw();
            fontRenderer->renderText("Spawn", spawnButton->getPos().x - 30, spawnButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            break;
        }
        case over: {
            string message = "You win!";
            //TODO: This section should display time/score with an "GAME OVER" type message
            this ->fontRenderer->renderText(message,width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1} );
            break;
        }
    } */
    glfwSwapBuffers(window);
}

void Engine::readFromFile(std::string filepath) {
    ifstream ins(filepath);
    if (!ins) {
        cout << "Error opening file" << endl;
    }
    ins >> noskipws;
    int xCoord = 0, yCoord = height-SIDE_LENGTH;
    char letter;
    bool draw;
    color c;
    while (ins >> letter) {
        draw = true;
        switch(letter) {
            case '1': c = color(138.0/255, 131.0/255, 185.0/255); break;
            case '2': c = color(154.0/255, 147.0/255, 203.0/255); break;
            case '3': c = color(167.0/255, 160.0/255, 217.0/255); break;
            case '4': c = color(185.0/255, 179.0/255, 228.0/255); break;
            case '5': c = color(32.0/255, 21.0/255, 57.0/255); break;
            case '6': c = color(51.0/255, 35.0/255, 89.0/255); break;
            case '7': c = color(79.0/255, 60.0/255, 122.0/255); break;
            case '8': c = color(100.0/255, 81.0/255, 141.0/255); break;
            case '9': c = color( 157.0/255, 172.0/255, 222.0/255); break;
            case 'b': c = color(0, 0, 0); break;

            default: // newline
                draw = false;
                xCoord = 0;
                yCoord -= SIDE_LENGTH;
        }
        if (draw) {
            squares.push_back(make_unique<Rect>(shapeShader, vec2(xCoord + SIDE_LENGTH/2, yCoord + SIDE_LENGTH/2), vec2(SIDE_LENGTH, SIDE_LENGTH), c));
            xCoord += SIDE_LENGTH;
        }
    }
    ins.close();
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}