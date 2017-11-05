/**
* @file utilitaire.c
* @brief Toutes les fonctions diverses utiles au projet
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#include "utilitaires.h"

/**
* @brief Affiche proprement un message d'erreur
* @param msg Permet de rajouter une fonction supplémentaire
* @return void
*/
void gestErr(char * msg){
    printf("ERREUR: %s : Errno %d : %s\n",msg, errno, strerror(errno));
}
