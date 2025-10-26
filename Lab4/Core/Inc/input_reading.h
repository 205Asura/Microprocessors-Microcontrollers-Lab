#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_
#define NO_OF_BUTTONS                 1

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);
unsigned char is_button_pressed_30ms(unsigned char index);

extern uint8_t mode;

#define TIMER_PERIOD_MS 1 // tick

extern uint8_t flagForIncreasingCounter[NO_OF_BUTTONS];

#endif /* INC_INPUT_READING_H_ */
