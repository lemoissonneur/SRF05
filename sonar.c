/********************************************************************************\
* Nom du fichier: SONAR_SRF05.c													*
* Microcontrolleur: At90Can128                                                  *
* AUTEUR: Arthur LE BARON                                    	                *
* Date Creation: 06 mai 2012													*
* IUT: GEII Nantes-Carquefou                                                    *
* DESCRPT.: permet la configuration et l'utilisation des sonars SRF05 et SRF04	*
* sans distinction et quelque soit le mode du SRF05, le tout sans initialisation*
* Dernière Revision: 07/03/13					                                *
\********************************************************************************/

/* Librairies Externes au projet*/
#include <avr/io.h>
#include <avr/interrupt.h> //sei
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <ctype.h>

#include "sonar.h"

/****************************************************************************
* Nom			: Init_Timer												*
* Returns		: Aucun														*
* Parametres	: Aucun														*
* Description	:															*
****************************************************************************/


void Init_Timer0A(void)
{
	//TCCR0A= 0x04;
	//      100 // prediviseur par 256 de CLKi/o (sinon 000 pour le rendre inactif)
	TCCR0A= 0x02;
	//0000 0    //Pour les PWM (inutiles ici)
	//      101 // prediviseur par 1024 de CLKi/o (sinon 000 pour le rendre inactif)
	//il y a 255 impulsion d'une periode T=64us
	//Interrupt Flag Register TIFR0
	//Le depassement du timer est indique via le bit "OCF0A" qui est mis a "1"
	//Pour pouvoir redetecter un nouveau debordement il faut que le bit OCF0A soit a "0"
	// Pour cela il faut envoyer un logique "1" sur le bit OCF0A (et non "0" cf p111 datasheet du ucontrolleur)
	//TCNT0 permet lire l'etat du compteur du timer (valeur entre 0 et 255 sur 8 bits)
}

/* Lecture de la distance */
float Distance_Sonar(struct Sonar *sonar)
{
	/*Environnement*/
	//Distance en cm entre l'objet
	float Distance=0;
	//Compte le nombre de depassements du Timer
	unsigned int compt=0;
	//Temps dans le Timer
	unsigned char temps_restant;
	//Temps total Timer+Compteur en us
	unsigned int Temps_total=0;
	//Booleen
	char continuer=1;

	/*Algo*/
	//Trigger en sortie
	*sonar->PORT_SONAR |= ( 1 << sonar->TRIGGER_SONAR );

	//On demmare avec un niveau bas sur le Trigger puis une impulsion de minimum 10us
	*sonar->W_SONAR &= ~(1 << sonar->TRIGGER_SONAR);
	*sonar->W_SONAR |=	(1 << sonar->TRIGGER_SONAR);
	_delay_us(11);
	*sonar->W_SONAR &= ~(1 << sonar->TRIGGER_SONAR);

	//Echo en entree (tri stated et non Pull-Up)
	*sonar->PORT_SONAR &= ~(1 << sonar->ECHO_SONAR);
	*sonar->W_SONAR	&= ~(1 << sonar->ECHO_SONAR);
	
	//Attente d'un front montant sur l'echo (NON BLOQUANT)
	while( !(*sonar->R_SONAR & (1 << sonar->ECHO_SONAR)) && compt < 65333)
	compt++;
	
	//RAZ du compteur
	compt = 0;

	//Lancement du Timer
	Init_Timer0A();
	
	//Mesure de l'impulsion sur l'echo
	while(continuer)
	{ 
		//Si l'impulsion est finie
		if( !(*sonar->R_SONAR & (1 << sonar->ECHO_SONAR)) )
		{
			//Recuperation du temps dans le Timer
			temps_restant = TCNT0;
			//Arret du Timer et RAZ
			TCCR0A = 0;
			TCNT0 = 0;
			//Fin de l'attente
			continuer = 0;
		}
		//Sinon, en cas de depassement du Timer (Interrupt Request)
		else if(TIFR0 & 0x01)
		{
			compt ++;
			//Mise a un pour forcer la RAZ de l'Interrupt Request
			TIFR0 = 0x01;
		}
	}
	
	//Calcul du temps totale en us de l'echo, avec un Timer 8bits de T = 0.5us
	Temps_total= (unsigned int)((256*compt +  temps_restant));
	//Conversion en une distance en cm 
	Distance= ((float)Temps_total/116);

	sonar->distance = Distance;
	
	return (Distance);
}



