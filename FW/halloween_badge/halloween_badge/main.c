#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/sleep.h>

#define PWM_TIMER_MAX 0x400

static uint8_t counter = 0;

int main(void)
{
	TCCR1A = (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = (rand() % (PWM_TIMER_MAX/2)) + (PWM_TIMER_MAX/2);
	
	/* Enable Compare Match A and Overflow interrupts */
	TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);

	DDRC = 0x3E; /* Set as output */
	PORTC = 0x3E;

	sei();

	sleep_enable();
	
	while (1)
	{
		sleep_cpu();
	}
}

ISR(TIMER1_COMPA_vect)
{
	/* Turn OFF LEDs */
	PORTC = 0x00;
}

ISR(TIMER1_OVF_vect)
{
	/* Turn ON LEDs */
	PORTC = 0x3E;
	
	if (counter >= 60)
	{
		OCR1A = (rand() % (PWM_TIMER_MAX/2)) + (PWM_TIMER_MAX/2);
		counter = 0;
	}
	else
	{
		counter++;
	}
}
