#ifndef SRF05_H_INCLUDED
#define SRF05_H_INCLUDED

/** @struct SRF05
 *  @brief This structure contains adresses to register needed to communicate with an SRF05, trigger and echo pin number of the specified port, and last distance measured
 *  @var SRF05::DdrRegister 
 *    Member 'DdrRegister' contains the adress of the data direction register of the port connected to the sonar
 *  @var SRF05::PortRegister 
 *    Member 'PortRegister' contains the adress of the write register of the port connected to the sonar
 *  @var SRF05::PinRegister 
 *    Member 'PinRegister' contains the adress of the read register of the port connected to the sonar
 *  @var SRF05::TriggerPinNumber 
 *    Pin number from the port connected to the Trigger of the sonar
 *  @var SRF05::EchoPinNumber 
 *    Pin number from the port connected to the Echo of the sonar
 *  @var SRF05::Distance 
 *    Last measured distance (cm)
 */
typedef struct {
	volatile uint8_t *DdrRegister;	/**< data direction register */
	volatile uint8_t *PortRegister; /**< output write register */
	volatile uint8_t *PinRegister; /**< input read register */
	uint8_t TriggerPinNumber;	/**< pin number for the trigger wire */
	uint8_t EchoPinNumber; /**< pin number for echo wire */
	float Distance; /**< last measured distance (cm) */
}SRF05;

/**
 * @brief make a single distance measure from a sonar
 * @param sonar sonar from which you want to measure
 * @return the distance measured
 */
float SRFMeasureDistance(SRF05 *sonar);
	
#endif // SRF05_H_INCLUDED
