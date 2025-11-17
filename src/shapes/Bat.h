#ifndef M4OEP_BAT_H
#define M4OEP_BAT_H
#include "rect.h"
#include "shape.h"
#include "../shader/shader.h"

class Bat : public Rect {

    public:
        Bat(Shader &shader, vec2 pos, vec2 shape);
        void drawUp();
        void drawDown();
};


#endif //M4OEP_BAT_H