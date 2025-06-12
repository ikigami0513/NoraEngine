#ifndef MOUSE_HPP
#define MOUSE_HPP

enum class MouseMode {
    NORMAL,
    HIDDEN,
    DISABLED,
};

class Mouse {
    public:
        static void SetMode(MouseMode);
        static MouseMode GetMode();

    private:
        static MouseMode m_currentMode;
        static int GLFWMode(MouseMode mode);
};

#endif