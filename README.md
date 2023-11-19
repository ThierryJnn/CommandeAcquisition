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

Voici le code présent dans la boucle infinie qui permet de 

![image](https://github.com/ThierryJnn/CommandeAcquisition/assets/144686292/194977eb-22d1-4895-9551-93957defd865)
