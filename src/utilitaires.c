/**
* @file utilitaire.c
* @brief Toutes les fonctions diverses utiles au projet
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#include "utilitaires.h"

/**
* @brief Permet d'obtenir le répertoire utilisé pour la fille d'attente
* @return Le répertoire
*
* Si la variable d'environnement PROJETSE n'est pas définie, renvoie REPDEMON défini
* Sinon renvoie la valeur de PROJETSE
*
*/
char * getRepDemon() {
    if (getenv("PROJETSE") == NULL) {
        return REPDEMON;
    } else {
        return getenv("PROJETSE");
    }
}

/**
* @brief Affiche proprement un message d'erreur
* @param msg Permet de rajouter une info supplémentaire 
* (ex: la fonction d'où provient l'erreur)
* @return void
*/
void gestErr(char * msg){
    printf("ERREUR: %s : Errno %d : %s\n",msg, errno, strerror(errno));
}

/**
*@brief Affiche les messages de debug (seulement si l'option "-d" a été donnée)
* @param msg Le message à afficher
* @return void
*/
void debugInfo(char * msg){
    printf("DEBUG: %s\n", msg);
}