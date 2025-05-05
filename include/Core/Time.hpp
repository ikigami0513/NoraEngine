#ifndef TIME_HPP
#define TIME_HPP

class Time {
    public:
        static void UpdateDeltaTime(double currentTime);
        static void CalculateFPS(double currentTime);
        static float DeltaTime();
        static float FPS();

    private:
        static double lastTime;
        static double fpsLastTime;
        static float fps;
        static float deltaTime;
        static int frameCount;
};

#endif