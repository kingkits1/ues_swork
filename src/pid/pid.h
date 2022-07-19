// File:pid.h
#ifndef __PID_H
#define __PID_H
typedef struct
{

    /* Controller gains */
    float Kp;
    float Ki;
    float Kd;

    /* Derivative low-pass filter time constant */
    float tau; // normal: = T * 2

    /* Output limits */
    float limMin;
    float limMax;

    /* Integrator limits */
    float limMinInt;
    float limMaxInt;

    /* Sample time (in seconds) */
    float T;

    /* Controller "memory" */
    float integrator;
    float prevError;			/* Required for integrator */
    float differentiator;
    float prevMeasurement;		/* Required for differentiator */

    /* Controller output */
    float out;

} PIDController;


void PIDController_prepare_parameters(
    PIDController *pid,
    /* Controller gains */
    float Kp,
    float Ki,
    float Kd,
    /* Derivative low-pass filter time constant */
    float tau,
    /* Output limits */
    float limMin,
    float limMax,
    /* Integrator limits */
    float limMinInt,
    float limMaxInt,
    /* Sample time (in seconds) */
    float T);

void PIDController_set_integrator(PIDController *pid, float new_int_val);

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);
#endif



// End of file pid.h
