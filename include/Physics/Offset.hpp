#ifndef OFFSET_HPP
#define OFFSET_HPP

class Offset {
    public:
        float up;
        float down;
        float left;
        float right;

        Offset(float p_up = 0.0f, float p_down = 0.0f, float p_left = 0.0f, float p_right = 0.0f) {
            up = p_up;
            down = p_down;
            left = p_left;
            right = p_right;
        }
};

#endif