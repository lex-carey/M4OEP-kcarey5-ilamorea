//
// Created by Alexis Carey on 11/13/25.
//

#include "Bat.h"

Bat::Bat(Shader &shader, vec2 pos, vec2 size, struct color color)
    : Rect(shader, pos, size, color) {}

void Bat::up() {
    setSizeX(50);
    setSizeY(74);
}

void Bat::down() {
    setSizeX(78);
    setSizeY(52);
}

void Bat::fall() {
    down();
    moveY(-2);
    if (getBottom() < 0) setPosY(0 + getSize().y / 2);
}

void Bat::fly() {
    up();
    moveY(4);
    if (getTop() < 0) setPosY(0);
}

void Bat::plusX() {
    up();
    moveX(4);
    if (getTop() < 0) setPosY(0);
}
void Bat::minusX() {
    up();
    moveX(-4);
    if (getTop() < 0) setPosY(0);
}