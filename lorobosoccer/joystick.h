#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "queue_custom.h"
#include "crc_hash.h"

#define START_BYTE (0xA5)

// Button's bit position in the byte
#define B_X (7)
#define B_Y (6)
#define B_A (5)
#define B_B (4)
#define B_UP (3)
#define B_DOWN (2)
#define B_LB (1)
#define B_RB (0)

#define B_START (7)
#define B_BACK (6)
#define B_XBOX (5)
#define B_LEFT (4)
#define B_RIGHT (3)
#define B_L3 (2)
#define B_R3 (1)

#define EMERGENCY_BRAKE_KEY (B_A)
#define BASE_BRAKE_KEY (B_B)
#define BELT_MOTOR_START_KEY (B_UP)
#define BELT_MOTOR_STOP_KEY (B_DOWN)
#define ROLLER_MOTOR_START_KEY (B_X)
#define ROLLER_MOTOR_STOP_KEY (B_Y)

#ifndef _BV
#define _BV(x) (1 << x)
#endif


struct JoyStick_Data {
  uint8_t button1;
  uint8_t button2;
  uint8_t lt;
  uint8_t rt;
  int8_t l_hatx;
  int8_t l_haty;
  int8_t r_hatx;
  int8_t r_haty;
};
// struct JoyStick_Command
// {
//    bool grip_arrow;
//    bool actuate_arm;
//    uint8_t rotate_arrow;
//    bool throwMotorActuate;
//    bool throwPneuActuate;
// };
struct JoyStick_Handle {
  // UART_HandleTypeDef *huart;
  Queue<JoyStick_Data, 2> data;
};

class JoyStick {
public:
  JoyStick()
    : hjoy_(0) {}
  JoyStick(JoyStick_Handle *hjoy) {
    hjoy_ = hjoy;
  }
  JoyStick(JoyStick &&) = default;
  JoyStick(const JoyStick &) = default;
  JoyStick &operator=(JoyStick &&) = default;
  JoyStick &operator=(const JoyStick &) = default;
  ~JoyStick() {}
  void set_config(JoyStick_Handle *hjoy) {
    hjoy_ = hjoy;
  }
  // void init(Servo_config *serv);
  void parse_JoyData();
  //bool is_Empty();
  //void parse();

private:
  JoyStick_Handle *hjoy_;
  // struct JoyStick_Command Joy_Command;
  // Servo_config *servo;
  //JoyStick_Data read();
};
#endif  // !_JOYSTICK_H_
