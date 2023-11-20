# CommandeAcquisition
## Environnement 

Le but de ce projet est de gérer la commande d'une MCC grâce à 4 transistors du hacheur en commande complémentaire décalée. Pour cela on génèrera des PWMs sur une carte Nucleo-STM32G474RE. Cette carte est intégrée à un PCB qui fait pont entre la commande et l'alimentation (la puissance) distribuée à la MCC.

## Console UART

Dans un premier temps nous allons paramétrer la liaison UART de la carte STM32-G474RE.
La liaison UART devra avoir comme fonctionnalité :
-   La présence d'un écho pour tous les caractères pour voir ce que l'on envoie.
-   Lorsque le caractère "ENTER" est détecté (voir sa valeur dans la table ASCII), traiter la chaine de caractères en la comparant aux commandes connues, pour le moment nous resterons à un nombre limitée de commandes :
-   help : qui affiche toutes les commandes disponibles,
-   pinout : qui affiche toutes les broches connectées et leur fonction  
-   start : qui allume l'étage de puissance du moteur (pour l'instant nous ne ferons qu'afficher un message de "Power ON" dans la console)  
-   stop : qui éteind l'étage de puissance du moteur (pour l'instant nous ne ferons qu'afficher un message de "Power OFF" dans la console)
-   toute autre commande renverra un message dans la console "Command not found".
-   Vider la chaine de caractère et mettez l'index pointant vers le prochain caractère à remplir à 0.

Le projet stm32cubeIDE sur lequel on se base configure déjà les connexions, notament celles de l'UART pour pouvoir communiquer avec un terminal type Shell sur le PC.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/d1854832-a024-478c-bdf7-e82cbf9315c3)

On utilisera donc l'USART2.

Il est relativement simple d'afficher un message sur un terminal shell en connexion série (avec puTTY ou Tera Term) depuis la carte nucleo or nous souhaitons
également intéragire avec depuis le pc, c'est le principe d'une console après tout.
Or dans l'état actuel, un fois le programme executé il est impossible d'écrire dans la console et donc de soumettre des commandes.
Pour y parvenir nous devons gérer les interruptions sur les ports UART2 depuis le panneau de contrôle NVIC dans le fichier .ioc

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/73b41efb-5f0f-4394-8650-58b16d39e2df)

Maintenant lorsque l'on ecrira dans la console shell cela génèrera une interruption dans le programme qui poura lui permettre de prendre en compte les caractères envoyés.

Dans main.c on commance par définir les variables qui nous servirons à executer des commandes et on en profite pour ajouter un message d'accueil au Shell qui nous sera utile notamment pour savoir si le programme à bien été réimplémenté par exemple.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/aaa5f0aa-3cb8-450b-b145-d89ee8ab19a2)

On affiche le message d'accueil avec la fonction HAL_UART_Transmit(), la fonction HAL_UART_Receive_IT() nous permet de stocker tous les caractères reçus lors d'interruption sur l'UART2 dans le buffer uartRxBuffer.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/217868b4-267f-4b30-8010-d5a12f4d1ab3)

Voici le code présent dans la boucle infinie:

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/194977eb-22d1-4895-9551-93957defd865)

Pour résumer, si une interruption est générée (it_uart2==1) et qu'elle n'est pas le caractère ENTER (uartRxBuffer[0]!=ENTER) on ajoute le caractère dans la chaîne de caractère command (command[idx++]=uartRxBuffer[0]). Si le caractère est ENTER on fait un retour à la ligne en envoyant "\r\n", on envoie la commande dans la fonction processCommand() qui pourra la traiter, on remet idx à 0 et on efface le contenue de la chaîne command en mémoire.
Après réception de n'importe quel caractère on remet la variable d'interruption à 0 (it_uart2=0).

La fonction processCommand dont le code est le suivant, est relativement simple puisqu'il s'agit de simples comparaisons.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/b08320f0-924c-403f-9d38-adc33f6ea744)

On compare donc la chaîne de caractère command avec les mots help, start, stop et pinout grâce a strcmp() pour les fonctions ne prenant pas d'arguments.
Pour les fonctions prenant des arguments comme speed (dont on parlera après) on utilise strncmp(). Si la chaîne de caractère ne correspond à aucune commande alors on affiche un message d'erreur "Command not found" 

Ce n'est pas le cas sur la capture mais dans la partie dédiée à la fonction help on peut afficher un descriptif de chaque fonction simplement avec la fonction HAL_UART_Transmit(), tel est le principe de cette fonction.
Les fonctions pinout, start et stop sont définie dans leur propre fichier pour rendre le code plus lisible et ordonné.
La variable externe Power initialisée à 0 puisque le moteur commence "éteint" lorsque le programme est téléversé.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/93e42f79-8d59-436e-99eb-98d15e0ed222)

La fonction pinout() est relativement similaire en terme de fonctionnement à help car il s'agit simplement d'afficher un message sur la console or ici on utilise une structure pour simplifier l'écriture du code.
Là aussi la fonction n'est pas complète sur la capture puisqu'il manque l'information de certaines broches mais le principe est le même et la commande fonctionne.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/82b136c2-766e-4465-a7af-b604677bf442)

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/250fecdb-2e7e-4ada-9503-aa6a9f2bcd6c)

Les commandes start et stop redirigent directement vers les fonctions startMotor et stopMotor dans lequelles ont définit une variable externe Power agissant comme un booléen valant 1 si le moteur est allimenté et 0 si il ne l'est pas. Dans ces fonctions, on regarde si le moteur est allumé ou éteint et on change son état.

Pour le démarrer on utilise les fonction HAL_TIM_PWM_Start() pour les PWMs des channels 1 et 2, HAL_TIMEx_PWM_Start() pour les PWMs complémentaires des channels 1 et 2.
A l'inverse, pour l'éteindre on utilise les fonction HAL_TIM_PWM_Stop() pour les PWMs des channels 1 et 2, HAL_TIMEx_PWM_Stop() pour les PWMs complémentaires des channels 1 et 2.
On est donc bien en commande complémentaire décalée. 

Par mesure de précaution après un arret ou un démarrage, on met le moteur à 50% de rapport cyclique (donc à vitesse 0) avec les commandes __HAL_TIM_SET_COMPARE() sur les channels 1 et 2.
Les valeurs dans ces fonctions représente le rapport cyclique et doivent être complémentaires pour atteindre 1024. Par exemple pour un rapport cylique de 50% comme ici on met les deux channels à 512 (512+512=1024).

Si le moteur est déjà dans l'état dans lequel on veut le mettre on le signale simplement sur le shell.
La variable Power étant externe, elle est commune aux deux fonctions ce qui permet donc cette vérification.

## Commande MCC basique

Cahier des charges :  
-   Fréquence de la PWM : 20kHz.
-   Temps mort minimum : à voir selon la datasheet des transistors (faire valider la valeur). 
-   Résolution minimum : 10bits.

Voici comment configurer les PWMs associées au TIM1 dans la section Timers du fichier .ioc .

Il est important de régler le dead time (ou temps mort) à 44 ms, une valeur que l'on a récupéré sur la documentation des transistors utilisés sur le hacheur du PCB.
Le temps mort est la période durant laquelle le transistor passe d'un état à l'autre, elle doit être prise en compte pour éviter une désynchronisation entre le programme et l'electronique du PCB ce qui pourrait l'endommager et empécher son bon fonctionnement.

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/8f852da5-4d97-48bf-bb10-13151deeb2ad)
![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/d34216b6-6df2-481c-adcf-ff8c81f0069f)
![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/180b66fd-006a-4201-98b1-af7a2892cf1e)
![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/66345e67-5ab2-47e9-801b-03aafa22f024)

La clock de la carte est de fréquence 170MHz or on souhaite des PWMs de fréquence 20kHz, on règle donc le PSC à 7 et le ARR à 1023 puisque $\frac{freq clock}{(PSC+1)(ARR+1)}=\frac{170MHz}{8*1024}=20751kHz\simeq 20kHz$

Le pulse des channels doit être complémentaire à 1024 comme expliqué précédement, ici on le règle à 512 pour les deux soit un rapport cyclique de 50% donc un moteur à l'arret mais cela n'a pas beaucoup d'importance puisque ces valeurs sont modifiées par la suite dans le programme.

On peut observer ces 4 PWMs sur un oscilloscope ci-dessous, 1 et 2 sont complémentaire tout comme 3 et 4. 

![tek00000](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/b7be2008-98be-4b9c-9a7e-343f58ef9222)

On peut également visualiser le temps mort ou dead time de 44ms.

![tek00001](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/4c87789f-19b9-4b1e-959f-2dfe1d0c3ae1)

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/681c06f3-9c57-4535-8975-aba01dcd2fd0)

