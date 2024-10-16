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

    float compute(float temperature);
    void setKp(float kp);
    void setKi(float ki);
    void setKd(float kd);
};
#endif //GLA_PID