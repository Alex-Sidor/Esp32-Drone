#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <math.h>

Adafruit_MPU6050 mpu;

struct vec3
{
    float x;
    float y;
    float z;
    
    vec3() : x(0), y(0), z(0){}
    
    vec3(float X, float Y, float Z){
        x = X;
        y = Y;
        z = Z;
    }

    vec3 operator+(const vec3& other) const{
        return vec3(x + other.x,y + other.y,z + other.z);
    }

    vec3 operator-(const vec3& other) const{
        return vec3(x - other.x,y - other.y, z - other.z);
    }

    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator/(float scalar) const {
        return vec3(x / scalar, y / scalar, z / scalar);
    }

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }
};

struct vec2
{
    float x;
    float y;
    
    vec2() : x(0), y(0){}
    
    vec2(float X, float Y){
        x = X;
        y = Y;
    }

    vec2 operator+(const vec2& other) const{
        return vec2(x + other.x,y + other.y);
    }

    vec2 operator-(const vec2& other) const{
        return vec2(x - other.x,y - other.y);
    }

    vec2& operator+=(const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    vec2 operator*(float scalar) const {
        return vec2(x * scalar, y * scalar);
    }
};

class pid{
public:
    pid(float p, float i, float d){
        this->p = p;
        this->i = i;
        this->d = d;
    }

    float update(float pos, float speed, float dt){
        acc += -dt*pos*i;

        float output = acc - (speed*d) - (pos*p);

        return output;
    }
private:

    float acc;

    float p, i, d;

};

const int freq = 20000;
const int resolution = 10;

float filterGyroBias = 0.99f;

sensors_event_t a, g, temp;

unsigned long prevTime = 0;
float deltaT = 0.0;

vec2 globalRotation;

pid xrot(1.0f,1.0f,1.0f);
pid yrot(1.0f,1.0f,1.0f);



void createInitRotation(sensors_event_t a){
  globalRotation = gravityToPitchRoll(vec3{a.acceleration.x,a.acceleration.y,a.acceleration.z});
}

void complamentryFilter(sensors_event_t a,sensors_event_t g, float dt){
  vec2 accelerometer = gravityToPitchRoll(vec3{a.acceleration.x,a.acceleration.y,a.acceleration.z});
  vec2 gyro = vec2{(g.gyro.y*dt*57.2957795f) + globalRotation.x,(g.gyro.x*dt*57.2957795f) + globalRotation.y};
  globalRotation = (gyro * filterGyroBias) + (accelerometer*(1.0f-filterGyroBias));
}

vec2 gravityToPitchRoll(vec3 v) {
  vec2 output; 
  output.x = atan2(-v.x, sqrt(v.y * v.y + v.z * v.z)) * 180.0 / PI;
  output.y  = atan2(v.y, sqrt(v.x*v.x + v.z*v.z)) * 180.0 / PI;
  return output;
}

float minmax(float input, float bottom, float top){
    return(max(min(input,top),bottom));
}

void outputToMotors(vec2 outputVector, float thrust, float outputScale){
    
    outputVector.x = minmax(outputVector.x/outputScale,-1.0f,1.0f);
    outputVector.y = minmax(outputVector.y/outputScale,-1.0f,1.0f);

    thrust = minmax(thrust,0.0f,1.0f);

    outputVector = outputVector * 1023;
    
    ledcWrite(4,static_cast<int>(thrust - outputVector.y + outputVector.x));
    ledcWrite(2,static_cast<int>(thrust + outputVector.y + outputVector.x));
    ledcWrite(25,static_cast<int>(thrust - outputVector.y - outputVector.x));
    ledcWrite(32,static_cast<int>(thrust + outputVector.y - outputVector.x));
}

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) { delay(10); }
    }
  
    Serial.println("MPU6050 Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


    ledcAttach(4, freq, resolution);//1
    ledcAttach(2, freq, resolution);//2
    ledcAttach(25, freq, resolution);//3
    ledcAttach(32, freq, resolution);//4

    mpu.getEvent(&a, &g, &temp);
    createInitRotation(a);

    //turn on drivers
    pinMode(14, OUTPUT);
    digitalWrite(14, LOW); 

    pinMode(17, OUTPUT);
    digitalWrite(17, LOW); 

    ledcWrite(0, 200);
    ledcWrite(1, 20);
    ledcWrite(2, 200);
    ledcWrite(3, 200);

    delay(500);

    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
}

void loop() {   
    /*mpu.getEvent(&a, &g, &temp);

    unsigned long currentTime = micros();
    deltaT = (currentTime - prevTime) / 1e6;
    prevTime = currentTime;
    
    complamentryFilter(a,g,deltaT);

    vec2 pidOutput;

    pidOutput.x = xrot.update(globalRotation.x,g.gyro.x, deltaT);
    pidOutput.y = yrot.update(globalRotation.y,g.gyro.y, deltaT);

    outputToMotors(vec2(0.0f,0.0f)-pidOutput,0.0f,45.0f);

    Serial.print("t:");
    Serial.print(-90);
    Serial.print(",");
    Serial.print("b:");
    Serial.print(90);
    Serial.print(",");

    Serial.print("x:");
    Serial.print(pidOutput.x);
    Serial.print(",");
    Serial.print("y:");
    Serial.println(pidOutput.y);

    //Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" C");
    */

    if (Serial.available()) {
        String msg = Serial.readStringUntil('\n'); // read one line
        msg.trim(); // remove whitespace/newline

        int motor = msg.toInt(); // convert to number

        int pwmValue = 1023; // adjust throttle 0-1023

        switch(motor) {
            case 1: ledcWrite(4, pwmValue); break;
            case 2: ledcWrite(2, pwmValue); break;
            case 3: ledcWrite(25, pwmValue); break;
            case 4: ledcWrite(32, pwmValue); break;
            default: break;
        }

        delay(1000); // run for 1 second

        // turn motor off
        switch(motor) {
            case 1: ledcWrite(4, 0); break;
            case 2: ledcWrite(2, 0); break;
            case 3: ledcWrite(25, 0); break;
            case 4: ledcWrite(32, 0); break;
            default: break;
        }
    }
}
