# M4OEP-kcarey5-ilamorea
Base logic for shape, rect, engine, and cloud borrowed from in-class activity and runner GP (as well as shaders). All other code for this project was authored by Alexis Carey and Issac Lamorea for the M4OEP. 

## Project Summary
This program is based off the game flappy bird, except you play as a bat who has to weave between clouds instead of pipes. The initial idea for this was to make a custom sprite for the bat by rendering a png as a custom shader, but we were not able to figure it out in time. As of now, our sprite is a rectangle.

We used the following concepts from the Module 4 concepts menu:
* OpenGL, GLFW, or any other graphics framework for C++
  * We used the graphics frameworks taught to us in-class to create this program
* Drawing
  * All elements seen on-screen were rendered via drawing
* Mouse input(s)
  * The start screen prompts the user to left click to start the game. Once the user does this, they are taken to the game screen.
* Keyboard input(s)
  * If escape key is pressed, window will close. R is pressed to restart the game once the player has lost. The space bar is used to move the sprite up (make the sprite "flap")
* Non-input based event(s)
  * The cloud obstacles move across the screen towards the bat continuously through a non-input based event.
* Multiple screens in the same graphics window
  * Start screen, game screen, game over screen
* Graphics with C++ classes
  * Cloud and bat are separated into their own C++ classes with header files.

With more time it would have been fun to figure out how to add sound effects to the game. The obvious thing we'd like to do with more time is figure out how to render the custom sprite as we think it would add a lot to the overall vibe of the game. 

## Bugs
We currently know of no unresolved bugs

## Grading
We believe we have met the expectations defined for this project in the rubric. Despite not being able to achieve everything we wanted to for this project, this is a fully functional game (though not so pretty) and the features we have included are implemented well in our minds.