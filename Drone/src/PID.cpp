#include "PID.h"

PID::PID(float p, float i, float d){
    this->p = p;
    this->i = i;
    this->d = d;
}

float PID::update(float pos, float speed, float dt){
    acc += dt*pos*i;

    float output = acc + (speed*d) + (pos*p);

    return output;
}