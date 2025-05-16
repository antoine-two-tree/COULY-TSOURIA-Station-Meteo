# COULY-TSOURIA-Station-Meteo
Projet microcontroleur STM32

Ballon Météo Autonome
Description du projet
Le projet "Ballon Météo Autonome" est un système de collecte de données météorologiques capable de s'élever à une hauteur définie, de recueillir des données (température, humidité, accélération), et de revenir automatiquement au sol en cas de conditions défavorables. Ce système est conçu pour être robuste, fiable et facile à utiliser.
Architecture du système
Le système est composé de deux unités principales :
1. Station Sol

Microcontrôleur STM32 pour le traitement principal
Treuil motorisé utilisant un moteur pas à pas 28BYJ-48 piloté par un contrôleur N2003
Anémomètre pour mesurer la vitesse du vent
Interface utilisateur avec bouton poussoir et potentiomètre
Écran LCD pour l'affichage des données et de l'état du système
Module ESP32 pour la communication sans fil
Connexion UART vers le microcontrôleur

2. Ballon Porteur

Microcontrôleur STM32
Capteur de température
Capteur d'humidité
Accéléromètre trois axes
Module ESP32 pour la communication sans fil avec la station sol
Alimentation par batterie Li-Po

Fonctionnement

Initialisation : La station sol vérifie tous les périphériques et configure les seuils de sécurité.
Déploiement : Le treuil déroule progressivement le câble, permettant au ballon de s'élever.
Collecte des données : Le ballon mesure et transmet périodiquement les données météorologiques.
Surveillance de sécurité : Le système surveille constamment les conditions suivantes :

Vitesse du vent (via l'anémomètre au sol)
Accélération du ballon (détection de turbulences)
Commandes manuelles (bouton de rappel)


Retour automatique : Le ballon est ramené au sol en cas de :

Vitesse du vent excessive
Turbulences ou chocs importants
Activation manuelle du bouton de rappel



Matériel requis
Microcontrôleurs

2x STM32 (série F4 ou L4 recommandée)
2x ESP32 pour la communication sans fil

Station Sol

Moteur pas à pas 28BYJ-48
Contrôleur de moteur N2003
Anémomètre (sortie à impulsions)
Écran LCD (interface I2C recommandée)
Bouton poussoir
Potentiomètre 10kΩ
Câble pour le treuil (longueur adaptée à l'altitude souhaitée)
Mécanisme de treuil (bobine, guide-fil)

Ballon

Ballon gonflable résistant aux UV
Capteur de température (ex: DS18B20 ou intégré dans SHT30)
Capteur d'humidité (ex: SHT30)
Accéléromètre MPU-6050
Batterie Li-Po (3.7V, capacité adaptée à la durée de vol souhaitée)
Nacelle légère pour contenir l'électronique

Configuration des périphériques STM32
GPIO

LED indicateur d'état (sortie)
Bouton poussoir (entrée avec interruption)
Connexions pour le contrôleur N2003 du moteur pas à pas

TIMER

Timer pour le pilotage du moteur pas à pas
Timer pour la lecture de l'anémomètre (comptage d'impulsions)

ADC

Lecture du potentiomètre pour les réglages de seuils

UART

Communication avec l'ESP32 pour la transmission des données
Sortie de débogage (printf)

I2C/SPI

Interface avec l'écran LCD (I2C)
Communication avec les capteurs du ballon (I2C pour MPU-6050 et SHT30)

Pilotage du moteur pas à pas 28BYJ-48
Le moteur 28BYJ-48 est un moteur pas à pas unipolaire à 5 fils, piloté par le contrôleur N2003 (basé sur le circuit ULN2003). Voici les caractéristiques principales :

Tension : 5V DC
4 phases
Angle de pas : 5.625° (64 pas par révolution)
Avec réducteur interne 1:64 (4096 pas pour un tour complet)
Couple : environ 34 mN.m

