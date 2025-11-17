//
// Created by Alexis Carey on 11/13/25.
//

#include "Bat.h"

Bat::Bat(Shader &shader, vec2 pos, vec2 size, struct color color)
    : Rect(shader, pos, size, color) {}

void Bat::drawUp() {
    setSizeX(50);
    setSizeY(74);
}

void Bat::drawDown() {
    setSizeX(78);
    setSizeY(52);

}

