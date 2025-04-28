#include "Time.hpp"

double Time::lastTime = 0.0;
double Time::fpsLastTime = 0.0;
float Time::fps = 0.0f;
float Time::deltaTime = 0.0f;
int Time::frameCount = 0;

void Time::UpdateDeltaTime(double currentTime) {
    deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;
}

void Time::CalculateFPS(double currentTime) {
    frameCount++;
    if (currentTime - fpsLastTime >= 1.0) {
        fps = static_cast<float>(frameCount);
        frameCount = 0;
        fpsLastTime += 1.0;
    }
}

float Time::DeltaTime() {
    return deltaTime;
}

float Time::FPS() {
    return fps;
}
