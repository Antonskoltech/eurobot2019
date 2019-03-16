#ifndef _H_MOTOR_KINEMATICS_
#define _H_MOTOR_KINEMATICS_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*
 * Robot's movement time allowed in seconds
 */
#define MOTOR_OPERATING_TIME           99

/*
 * SET_PWM command args structure
 */
typedef struct {
        uint8_t channel;
        float pwm_value;
} __attribute__((packed)) cmd_set_pwm_t;

#define SET_PWM_ARGS_ERR(cmd_args) \
        ((cmd_args)->channel > 3) || ((cmd_args)->channel < 1) || \
        (fabsf((cmd_args)->pwm_value) < 0.1f) || \
        (fabsf((cmd_args)->pwm_value) > 1.0f)

/*
 * SET_SPEED command args structure
 */
typedef struct {
        float vx;
        float vy;
        float wz;
} __attribute__((packed)) cmd_set_speed_t;

/*
 * Motor control structure definition
 */
enum motor_kinem_status_flags {
        MK_PWM_CONTROL_BIT,
        MK_SPEED_CONTROL_BIT,
        MK_STOP_MOTORS_BIT
};

#define ENUM_FLAG(name) name = 1 << name##_BIT
enum mk_flags {
        ENUM_FLAG(MK_PWM_CONTROL),
        ENUM_FLAG(MK_SPEED_CONTROL),
        ENUM_FLAG(MK_STOP_MOTORS)
};

#define ROBOT_SESSION_COMPETITION       0
#define ROBOT_SESSION_DEBUG             1

#define is_manip_flag_set(manip_ctrl, bit) \
        (manip_ctrl->flags & bit)

typedef struct {
        int status;
        uint8_t session;
        uint8_t cord_status;
        float vel_x;
        float vel_y;
        float wz;
        TaskHandle_t mk_notify;
        SemaphoreHandle_t lock;
        float pwm_motors[3];
} motors_ctrl_t;

/*
 * Memory for motor kinematics task
 */
#define MOTOR_KIN_STACK_DEPTH    1024
StackType_t motor_kinematics_ts[MOTOR_KIN_STACK_DEPTH];
StaticTask_t motor_kinematics_tb;

/*
 * Semaphore service structure
 */
StaticSemaphore_t mutex_buffer;

/*
 * Forward kinematics parameters
 */
#define MK_MAX_ROT_SPEED 22.4399f
#define MK_LIN_KIN_MATRIX \
        -32.0750f,    18.5158f,    0.0f, \
        0.0f,         37.0370f,    0.0f, \
        32.0750f,     18.5158f,    0.0f

#define MK_ROT_KIN_MATRIX \
        0.0f,   0.0f,    3.67f,  \
        0.0f,   0.0f,   -3.67f,  \
        0.0f,   0.0f,    3.67f

#define MK_SPEED2PWM_A \
        0.03565f, \
        0.03565f, \
        0.03565f

#define MK_SPEED2PWM_B \
        0.1f, \
        0.1f, \
        0.1f

/*
 * Main freertos task
 */
void motor_kinematics(void *arg);

#endif
