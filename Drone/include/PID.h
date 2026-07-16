#pragma once

class PID{
public:
    PID(float p, float i, float d);

    float update(float pos, float speed, float dt);
private:
    float acc;
    float p, i, d;
};