#include <avr/interrupt.h>
#include <util/delay.h>

#include "SRF05.h"

#define SRFTimeUstoDistanceCm(time) ((float)time/58)

float SRFMeasureDistance(SRF05 *sonar)
{
	uint16_t Counter=0;
	uint16_t TotalTime=0;
	uint8_t RemainingTime=0;
	uint8_t Continue=1;

	// set Trigger pin as output
	*sonar->DdrRegister |= ( 1 << sonar->TriggerPinNumber );

	// send a 10+us signal on Trigger pin
	*sonar->PortRegister &= ~(1 << sonar->TriggerPinNumber);
	*sonar->PortRegister |=	(1 << sonar->TriggerPinNumber);
	_delay_us(11);
	*sonar->PortRegister &= ~(1 << sonar->TriggerPinNumber);

	// set Echo pin as input (tri stated, no Pull-Up)
	*sonar->DdrRegister &= ~(1 << sonar->EchoPinNumber);
	*sonar->PortRegister &= ~(1 << sonar->EchoPinNumber);
	
	// wait for rising of Echo pin (NON BLOQUANT)
	while( !(*sonar->PinRegister & (1 << sonar->EchoPinNumber)) && Counter < 65333)
	Counter++;
	
	// clear Counter, we will use it to measure the echo signal
	Counter = 0;

	// start Timer
	#if F_CPU == 1000000
		TCCR0A = 0x01;	// no prescaler -> T = 1us
		#define SRFTimer0toTimeUs(t) t
	#elif F_CPU == 8000000
		TCCR0A = 0x02;	// prescaler F_CPU/8 -> T = 1us
		#define SRFTimer0toTimeUs(t) t
	#elif F_CPU == 16000000
		TCCR0A = 0x02;	// prescaler F_CPU/8 -> T = 0.5us
		#define SRFTimer0toTimeUs(t) t/2
	#else
		#error "SRF05: please add your CPU frequency/timer0 prescaler configuration"
	#endif
	
	// measure the Echo signal time (one timer overflow = 128us)
	while(Continue)
	{ 
		//if echo impulse ended
		if( !(*sonar->PinRegister & (1 << sonar->EchoPinNumber)) )
		{
			// get extra time spend waiting from the time
			RemainingTime = TCNT0;
			// clear and stop timer
			TCCR0A = 0;
			TCNT0 = 0;
			// exit loop
			Continue = 0;
		}
		// if timer overflow
		else if(TIFR0 & 0x01)
		{
			Counter ++;
			// clear Interrupt request
			TIFR0 = 0x01;
		}
	}
	
	// total echo time measured (us)
	TotalTime = SRFTimer0toTimeUs(256*Counter +  RemainingTime);
	
	// convert impulsion time to distance (cm)
	sonar->Distance = SRFTimeUstoDistanceCm(TotalTime);
	
	return (sonar->Distance);
}



