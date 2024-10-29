#ifndef GLA_PID
#define GLA_PID
#include <stdint.h>


class PID
{
private:
    
    float kp, ki, kd;

    float previous_error;
    float integral;
    uint32_t previous_time;
    float setpoint;

    float max_integral_contribution = 0.75;//this is the most the integral term can contribute
    float max_integral_value = 0.0; //This is how large the integral term can become

public:
    PID::PID();
    PID::PID(float kp, float ki, float kd);
    float compute(float temperature);
    void setKp(float kp);
    void setKi(float ki);
    void setKd(float kd);
    float PID::getKp(void);
    float PID::getKi(void);
    float PID::getKd(void);
};
#endif //GLA_PID