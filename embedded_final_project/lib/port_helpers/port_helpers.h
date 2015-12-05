/*
 * port_helpers.h
 *
 * Created: 9/29/2015 10:36:31 PM
 *  Author: Harrison Lambeth
 */ 


#ifndef PORT_HELPERS_H_
#define PORT_HELPERS_H_

// To use these headers, you should define each pin with a _PORT and _PIN suffix
// to specify where the pin is located. For example, to specify an LED pin on
// port B, 3rd pin, you would do the following:
// #define LED_PORT B
// #define LED_PIN 2 // Using pin 2 because 0 is the first pin, 1 is second, 2 is third, etc.
//
// Now, all you have to do is set the port to output mode
// PORT_OUTPUT(LED);
// and then do whatever you want to it
// PORT_SET(LED);
// PORT_UNSET(LED);
// PORT_TOGGLE(LED);
//
// See the comments below for usage on some of the other functions.

// These concat the appropriate prefix to the port letter passed into this macro
// You should generally not call these directly in code.
#define DDR_INNER(P) (DDR ## P)
#define PORT_INNER(P) (PORT ## P)
#define PIN_INNER(P) (PIN ## P)

// These get the address of the specified port. (e.g. DDR(A) becomes &DDRA)
// You can use these functions to pass the desired port as a parameter into an unsigned char*, as an example
#define DDR(P) ((unsigned char*)&DDR_INNER(P))
#define PORT(P) ((unsigned char*)&PORT_INNER(P))
#define PIN(P) ((unsigned char*)&PIN_INNER(P))

// These are useful for quickly generating parameters in a function call.
// For example, if you have a function is_pressed(unsigned char* port, unsigned char pin), you can use
// is_pressed(PIN_PARAM(LED)) to pass the required parameters
#define DDR_PARAM(PRT) DDR(PRT ## _PORT), PRT ## _PIN
#define PORT_PARAM(PRT) PORT(PRT ## _PORT), PRT ## _PIN
#define PIN_PARAM(PRT) PIN(PRT ## _PORT), PRT ## _PIN

// These functions will take a port address and set (to 1), unset (to 0), toggle, or read the value at that
// port and pin number.
#define SET(PORT,PIN) (*PORT |= (1 << PIN))
#define UNSET(PORT,PIN) (*PORT &= ~(1 << PIN))
#define TOGGLE(PORT,PIN) (*PORT ^= (1 << PIN))
#define READ(PORT,PIN) ( (*PORT & (1 << PIN)) >> PIN)

// These functions make it really easy to do port operations. Simply pass the pin's prefix to the desired function to
// do the operation.
// ex: PORT_OUTPUT(LED); // sets to output mode
//     PORT_SET(LED);    // turns on led
// The INPUT and OUTPUT functions set the direction (DDRX)
// The SET, UNSET, and TOGGLE functions set, unset, or toggle the port (PORTX)
// The READ function reads the value on a port (PINX)
#define PORT_OUTPUT(PRT) (SET(DDR(PRT ## _PORT),PRT ## _PIN))
#define PORT_INPUT(PRT) (UNSET(DDR(PRT ## _PORT),PRT ## _PIN))
#define PORT_SET(PRT) (SET(PORT(PRT ## _PORT), PRT ## _PIN))
#define PORT_UNSET(PRT) (UNSET(PORT(PRT ## _PORT), PRT ## _PIN))
#define PORT_TOGGLE(PRT) (TOGGLE(PORT(PRT ## _PORT), PRT ## _PIN))
#define PORT_READ(PRT) (READ(PIN(PRT ## _PORT), PRT ## _PIN))

#endif /* PORT_HELPERS_H_ */