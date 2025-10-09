#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

void button_reading(void);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);
unsigned char is_button_pressed_30ms(unsigned char index);

extern uint8_t segments[10];
extern uint16_t SEG_Pins[7];
extern volatile uint8_t mode;

extern uint8_t isButton0Pressed;
extern uint8_t isButton1Pressed;
extern uint8_t isButton2Pressed;

extern uint8_t flagForIncreasingCounter;

#endif /* INC_INPUT_READING_H_ */
