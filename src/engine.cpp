#include "engine.h"
#include <fstream>
using namespace std;


enum state {start, play, over};
state screen = start;

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

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    //bat shader
    batShader = shaderManager->loadShader("../res/shaders/sprite.vert", "../res/shaders/sprite.frag",  nullptr, "sprite");
    spriteRenderer = make_unique<SpriteRenderer>(shaderManager->getShader("sprite"));
    batTexture = shaderManager->loadTexture("../art/bat-down.png", true, "bat");

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
    batShader.use();
    batShader.setInteger("image", 0);
    batShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // Relative file path from inside cmake-build-debug folder to the txt file
    readFromFile("../res/art/scene.txt");

    //Loading Sprite
    bat = make_unique<Bat>(batShader, vec2(400, 400), vec2(78, 52), color(1, 1, 1));
    // Init Cloud
    clouds.push_back(Cloud(shapeShader, vec2(rand() % 200 + 100, rand() % 500 + 100)));
    clouds.push_back(Cloud(shapeShader, vec2(400, rand () % 520 + 50)));
    clouds.push_back(Cloud(shapeShader, vec2(325, rand () % 480 + 75)));
    if (clouds.size() > 4) {
        clouds.push_back(Cloud(shapeShader, vec2(rand () % 200 + 125, rand () % 480 + 75)));
    }
    line = make_unique<Rect>(shapeShader, vec2(0,0), vec2 (1,800), color(0, 0, 0));
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

    if (screen == over) {
        if (keys[GLFW_KEY_R]) {
            screen = play;
        }

    }
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
    for ( Cloud& r : clouds) {
        if (r.isOverlapping(*line)) {
            clouds.pop_back();
        }
        // game over if bat hits ground
        if (bat->getBottom() <= 0) screen = over;
        //makes bat fall
        if (!keys[GLFW_KEY_SPACE]) {
            bat->fall();
        }
    }
    if (keys[GLFW_KEY_SPACE]) {
        bat->fly();
    }
}
void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float score = glfwGetTime();
    if (screen == play){
        for (Cloud& c : clouds) {
            c.moveXWithinBounds(-1, width);

        }
        for ( Cloud& r : clouds) {
            if (r.isOverlapping(*bat)) {
                screen = over;
            }
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
        spriteRenderer->DrawSprite(shaderManager->getTexture("bat"),
        glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }



    switch (screen) {
        case start: {
            string message = "Press left click to start";
            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            // NOTE: This line changes the shader being used to the font shader.
            //  If you want to draw shapes again after drawing text,
            //  you'll need to call shapeShader.use() again first.
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, PROJECTION, 1, vec3{1, 1, 1});
            break;
        }

        case play: {
            //TODO: Figure out what we exactly need here.


            break;
        }
        case over: {
            string message = "You lose! Press R to restart, or escape to exit!";
            float score = glfwGetTime();
            string scoreString = to_string(score);
            //TODO: This section should display time/score with an "GAME OVER" type message
            this ->fontRenderer->renderText(message,width/2 - (12 * message.length()), height/2, PROJECTION, 1, vec3{1, 1, 1} );
            this ->fontRenderer->renderText(scoreString,width/2 - (12 * message.length()), height/3, PROJECTION, 1, vec3{1, 1, 1} );
            break;
        }
    }
    glfwSwapBuffers(window);
}

void Engine::readFromFile(std::string filepath) {
    ifstream ins(filepath);
    if (!ins) {
        cout << "Error opening file" << endl;
    }
    ins >> noskipws;
    int xCoord = 0, yCoord = height-static_cast<float>(BACKGROUND);
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
                yCoord -= static_cast<float>(BACKGROUND);
        }
        if (draw) {
            squares.push_back(make_unique<Rect>(shapeShader, vec2(xCoord + static_cast<float>(BACKGROUND)/2, yCoord + static_cast<float>(BACKGROUND)/2), vec2(static_cast<float>(BACKGROUND), static_cast<float>(BACKGROUND)), c));
            xCoord += static_cast<float>(BACKGROUND);
        }
    }
    ins.close();
}

/*
void Engine::batUp() {
    ifstream ins("../res/art/bat-up.txt");
    bat->up();
    if (!ins) {
        cout << "Error opening file" << endl;
    }
    ins >> noskipws;
    int xCoord = bat->getLeft(), yCoord = bat->getBottom()-static_cast<float>(BAT);
    char letter;
    bool draw;
    color c;
    while (ins >> letter) {
        draw = true;
        switch(letter) {
            case 'w': c = color(0, 0, 0, 0); break;
            case 'c': c = color(32.0/255, 5.0/255, 33.0/255); break;
            case 'd': c = color(134.0/255, 73.0/255, 135.0/255); break;
            case 'e': c = color(237.0/255, 155.0/255, 186.0/255); break;
            case 'f': c = color(195.0/255, 83.0/255, 129.0/255); break;
            case 'a': c = color(1, 1, 1); break;

            default:
                draw = false;
                xCoord = bat->getLeft();
                yCoord -= static_cast<int>(BAT);
        }
        if (draw) {
            batSquares.push_back(make_unique<Rect>(shapeShader, vec2(xCoord + static_cast<float>(BAT)/2, yCoord + static_cast<float>(BAT)/2), vec2(static_cast<float>(BAT), static_cast<float>(BAT)), c));
        }
    }
    ins.close();
}

void Engine::batDown() {
    ifstream ins("../res/art/bat-down.txt");
    bat->down();
    if (!ins) {
        cout << "Error opening file" << endl;
    }
    ins >> noskipws;
    int xCoord = bat->getLeft(), yCoord = bat->getBottom()-static_cast<float>(BAT);
    char letter;
    bool draw;
    color c;
    while (ins >> letter) {
        draw = true;
        switch(letter) {
        case 'w': c = color(0, 0, 0, 0); break;
        case 'c': c = color(32.0/255, 5.0/255, 33.0/255); break;
        case 'd': c = color(134.0/255, 73.0/255, 135.0/255); break;
        case 'e': c = color(237.0/255, 155.0/255, 186.0/255); break;
        case 'f': c = color(195.0/255, 83.0/255, 129.0/255); break;
        case 'a': c = color(1, 1, 1); break;

        default:
            draw = false;
            xCoord = bat->getLeft();
            yCoord -= static_cast<float>(BAT);
        }
        if (draw) {
            batSquares.push_back(make_unique<Rect>(shapeShader, vec2(xCoord + static_cast<float>(BAT)/2, yCoord + static_cast<float>(BAT)/2), vec2(static_cast<float>(BAT), static_cast<float>(BAT)), c));
        }
    }
    ins.close();
}
*/


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