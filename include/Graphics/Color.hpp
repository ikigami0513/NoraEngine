#ifndef COLOR_HPP
#define COLOR_HPP

class Color {
    public:
        Color(float red = 0, float green = 0, float blue = 0, float alpha_ = 1) {
            r = red;
            g = green;
            b = blue;
            alpha = alpha_;
        }

        float r;
        float g;
        float b;
        float alpha;
};

#endif