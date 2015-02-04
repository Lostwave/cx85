/* Keyboard example with debug channel, for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"
#ifdef DEBUGMODE
	#include "print.h"
#endif
#include "cx-85.h"

uint16_t idle_count=0;

int main(void)
{
	uint8_t b, button, reset_idle;
	uint8_t b_prev=0xFF;

	// set for 16 MHz clock
	CPU_PRESCALE(0);

	// Configure all port B pins as inputs with pullup resistors.
	// See the "Using I/O Pins" page for details.
	// http://www.pjrc.com/teensy/pins.html
	DDRB = 0x00;
	PORTB = 0xFF;

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	// Configure timer 0 to generate a timer overflow interrupt every
	// 256*1024 clock cycles, or approx 61 Hz when using 16 MHz clock
	// This demonstrates how to use interrupts to implement a simple
	// inactivity timeout.
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TIMSK0 = (1<<TOIE0);

#ifdef DEBUGMODE
	print("Atari CX-85\n");
#endif
	while (1) {
		// read all port B pins
		b = PINB;
		
		// The last two bits will always be 1 because we aren't hooked up to B6 and B7
		// The first bit will always be 0 because that's the trigger, and we ignore everything when the trigger is not pressed
		// so only look at bits 1-6.
		button = MID(b,1,6);

		reset_idle = 0;
		if((b & 1) == 0)
		{
			//if the key pressed is different than the current key pressed.
			if(button != b_prev)
			{
				switch(button){
					case BUTTONF1:
						ACTIONF1;
						break;
					case BUTTONF2:
						ACTIONF2;
						break;
					case BUTTONF3:
						ACTIONF3;
						break;
					case BUTTONF4:
						ACTIONF4;
						break;
					case BUTTON0:
						ACTION0;
						break;
					case BUTTON1:
						ACTION1;
						break;
					case BUTTON2:
						ACTION2;
						break;
					case BUTTON3:
						ACTION3;
						break;
					case BUTTON4:
						ACTION4;
						break;
					case BUTTON5:
						ACTION5;
						break;
					case BUTTON6:
						ACTION6;
						break;
					case BUTTON7:
						ACTION7;
						break;
					case BUTTON8:
						ACTION8;
						break;
					case BUTTON9:
						ACTION9;
						break;
					case BUTTONPLUS:
						ACTIONPLUS;
						break;
					case BUTTONMINUS:
						ACTIONMINUS;
						break;
					case BUTTONDOT:
						ACTIONDOT;
						break;
				}
				usb_extra_consumer_send();
				usb_keyboard_send();
#ifdef DEBUGMODE
				print("Byte Received ");
				print_byte(button);
				print("\n");
				
				if(keyboard_keys[0] != 0){
					print("Key Pressed ");
				}
				if(consumer_key != 0){
					print("Media Pressed ");
				}
				phex(button);
				print(" ");
				phex(b_prev);
				print("\n");
#endif
			}	

			b_prev = button;
		}
		else{
			// flush 
			if(keyboard_keys[0] != 0){
#ifdef DEBUGMODE
				print("Key Released ");
				phex(0);
				print(" ");
				phex(b_prev);
				print("\n");
#endif
				b_prev = 0;
				keyboard_keys[0] = 0;
				usb_keyboard_send();
			}
			if(consumer_key != 0){
#ifdef DEBUGMODE
				print("Media Released ");
				phex(0);
				print(" ");
				phex(b_prev);
				print("\n");
#endif
				b_prev = 0;
				consumer_key = 0;
				usb_extra_consumer_send();
			}
		}
		
		// if any keypresses were detected, reset the idle counter
		if (reset_idle) {
			// variables shared with interrupt routines must be
			// accessed carefully so the interrupt routine doesn't
			// try to use the variable in the middle of our access
			cli();
			idle_count = 0;
			sei();
		}
		// now the current pins will be the previous, and
		// wait a short delay so we're not highly sensitive
		// to mechanical "bounce".
		_delay_ms(2);
	}
}

// This interrupt routine is run approx 61 times per second.
// A very simple inactivity timeout is implemented, where we
// will send a space character and print a message to the
// hid_listen debug message window.
ISR(TIMER0_OVF_vect)
{
	idle_count++;
	if (idle_count > 61 * 8) {
		idle_count = 0;
		//usb_keyboard_press(KEY_SPACE, 0);
	}
}


