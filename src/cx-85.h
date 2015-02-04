#define LAST(k,n) ((k) & ((1<<(n))-1))
#define MID(k,m,n) LAST((k)>>(m),((n)-(m)))

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

// This keypad supports 1 key rollover unfortunately.  Only one key can be pressed at a time.
// Since the cx85 uses the same plug and port as an Atari joystick, it actually works like one.
// It sends a trigger signal to imply a button is down.
// The trigger is on port B0
// Bit set to 1 on the trigger means no buttons are pressed.  
// Bit set to 0 on the trigger means a button is pressed.
// Along with it it sends a binary signal to indicate which button is down.
// The binary signal is on ports B1-B5

//				Teensy B Port	54321
//--------------------------------------
#define BUTTONF1 	0x18	//	11000
#define BUTTONF2 	0x09	//	01001
#define BUTTONF3 	0x01	//	00001
#define BUTTONF4 	0x11	//	10001
#define BUTTON0 	0x19	//	11001
#define BUTTON1 	0x13	//	10011
#define BUTTON2 	0x15	//	10101
#define BUTTON3 	0x17	//	10111
#define BUTTON4 	0x03	//	00011
#define BUTTON5 	0x05	//	00101
#define BUTTON6 	0x07	//	00111
#define BUTTON7 	0x0B	//	01011
#define BUTTON8 	0x0D	//	01101
#define BUTTON9 	0x0F	//	01111
#define BUTTONPLUS 	0x1D	//	11101
#define BUTTONMINUS	0x1F	//	11111
#define BUTTONDOT 	0x1B	//	11011


// Actions assigned to each button.
// Use consumer_key = XXXX when you are using media keys
// Use keyboard_keys[0] = XXXX when you want to use a regular keyboard key
#define ACTIONF1 consumer_key = TRANSPORT_PLAY_PAUSE
#define ACTIONF2 consumer_key = AUDIO_VOL_UP
#define ACTIONF3 consumer_key = AUDIO_VOL_DOWN 
#define ACTIONF4 consumer_key = AUDIO_MUTE
#define ACTION0 keyboard_keys[0] = KEYPAD_0
#define ACTION1 keyboard_keys[0] = KEYPAD_1
#define ACTION2 keyboard_keys[0] = KEYPAD_2
#define ACTION3 keyboard_keys[0] = KEYPAD_3
#define ACTION4 keyboard_keys[0] = KEYPAD_4
#define ACTION5 keyboard_keys[0] = KEYPAD_5
#define ACTION6 keyboard_keys[0] = KEYPAD_6
#define ACTION7 keyboard_keys[0] = KEYPAD_7
#define ACTION8 keyboard_keys[0] = KEYPAD_8
#define ACTION9 keyboard_keys[0] = KEYPAD_9
#define ACTIONPLUS consumer_key = TRANSPORT_NEXT_TRACK
#define ACTIONMINUS consumer_key = TRANSPORT_PREV_TRACK
#define ACTIONDOT keyboard_keys[0] = KEYPAD_PERIOD

// Teensy 	Atari Interface Atari Plug		CX85 Board
// ---------------------------------------------------
// B0 		Atari Trigger	Pin 6			Green
// B1 		Atari BPot		Pin 5			Blue
// B2 		Atari Up		Pin 1			Brown
// B3 		Atari Down		Pin 2			Red
// B4 		Atari Left		Pin 3			Orange
// B5 		Atari Right		Pin 4			Yellow
// +5 		+5				Pin 7			Violet
// Gnd 		Gnd				Pin 8			Gray
// Unused   Atari APot		Pin 9 			White

// 	Atari Plug 
//	_____________
//	\ 5 4 3 2 1 /
//	 \ 9 8 7 6 /
//    ---------	

// Atari Board from top
//------------------------------------
// B | R | O | Y | G | B | V | G | W
// r | e | r | e | r | l | i | r | h
// o | d | a | l | e | u | o | a | i
// w |   | n | l | e | e | l | y | t
// n |   | g | o | n |   | e |   | e
//   |   | e | w |   |   | t |   | 