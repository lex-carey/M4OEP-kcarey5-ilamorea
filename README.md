# M4OEP-kcarey5-ilamorea
Base logic for shape, rect, engine, and cloud borrowed from in-class activity and runner GP (as well as shaders). All other code for this project was authored by Alexis Carey and Issac Lamorea for the M4OEP. 

## Project Summary
This program is based off the game flappy bird, except you play as a bat who has to weave between clouds instead of pipes.

We used the following concepts from the Module 4 concepts menu:
* OpenGL, GLFW, or any other graphics framework for C++
  * We used the graphics frameworks taught to us in-class to create this program
* Drawing
  * All elements seen on-screen were rendered via drawing
* Keyboard input(s)
  * If escape key is pressed, window will close. R is pressed to start the game. Space is used to make the bat flap.
* Non-input based event(s)
  * The cloud obstacles move across the screen towards the bat continuously through a non-input based event.
* Multiple screens in the same graphics window
* Graphics with C++ classes
  * Cloud and bat are separated into their own C++ classes with header files.

With more time it would have been fun to figure out how to add sound effects to the game.

## Bugs
We currently know of no unresolved bugs

## Grading
As of right now, the bat is nonexistent. I (Alexis) didn't have enough time to figure out how to render it the way that I wanted to so it currently does not show on-screen. The bat class itself is not very fleshed out as my first order of business was attempting to get the bat to show on screen. That being said, the program still has completed features which meet the guidelines of the project. We believe we have done enough at this point to earn 60 points. 