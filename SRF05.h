/********************************************************************************\
* Nom du fichier: SONAR_SRF05.h													*
* Microcontrolleur: At90Can128                                                  *
* AUTEUR: Arthur LE BARON                                    	                *
* Date Creation: 06 mai 2012													*
* IUT: GEII Nantes-Carquefou                                                    *
* DESCRPT.: permet la configuration et l'utilisation des sonars SRF05 et SRF04	*
* sans distinction et quelque soit le mode du SRF05, le tout sans initialisation*
* Dernière Revision: --					                                        *
\********************************************************************************/

#ifndef SRF05_H_INCLUDED
#define SRF05_H_INCLUDED


/* NB : Si le sonar est un SRF05 en mode Echo et Trigger sur la même broche,
	*	indiquez simplement le meme numero de pin
	*	
	*	
	*	/!\ IMPORTANT : A l'usage, laissez 50ms entre deux mesures pour 
	*			eviter les perturbations
	*
	*
	*	/!\	Exemple de déclaration d'un sonar : /!\
	*		sur la broche 0 du port A
	*
	*		struct Sonar foo = { 
	*								&DDRA,	// registre de direction du port
	*								&PORTA,	// registre d'écriture du port
	*								&PINA,	// registre de lecture du port
	*								0,		// broche 'trigger'
	*								0,		// broche 'echo'
	*								0.0		// init de la distance à 0
	*							};
	*	/!\										/!\	
*/


/* Déclaration du format de l'objet Sonar : */
typedef struct {
	// registres du port
	volatile uint8_t *DdrRegister;
	volatile uint8_t *PortRegister;
	volatile uint8_t *PinRegister;
	// numero des E/S
	uint8_t TriggerPinNumber;
	uint8_t EchoPinNumber;
	// distance mesuré par le sonar
	float Distance;
}SRF05;

/*Prototypes*/
	//Renvoi la distance en cm entre le Sonar et l'objet le plus proche
	//Ne necessite pas d'initialisation, fonctionne avec SRF04 et SRF05, quelquesoit le mode du SRF05
float SRFMeasureDistance(SRF05 *sonar);




	
#endif // SRF05_H_INCLUDED
