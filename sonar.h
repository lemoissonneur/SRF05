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

#ifndef SONAR_H_INCLUDED
#define SONAR_H_INCLUDED


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
	typedef struct Sonar {
		// registres du port
		volatile uint8_t *PORT_SONAR;
		volatile uint8_t *W_SONAR;
		volatile uint8_t *R_SONAR;
		// numero des E/S
		uint8_t TRIGGER_SONAR;
		uint8_t ECHO_SONAR;
		// distance mesuré par le sonar
		float distance;
	} Sonar;

/*Prototypes*/
	//Renvoi la distance en cm entre le Sonar et l'objet le plus proche
	//Ne necessite pas d'initialisation, fonctionne avec SRF04 et SRF05, quelquesoit le mode du SRF05
	float Distance_Sonar(struct Sonar *sonar);




	
#endif // SONAR_H_INCLUDED
