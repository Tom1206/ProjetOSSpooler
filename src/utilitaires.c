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

/**
* @brief Permet d'obtenir le répertoire utilisé pour la fille d'attente
* @return Le répertoire
*
* Si la variable d'environnement PROJETSE n'est pas définie, renvoie REPDEMON défini
* Sinon renvoie la valeur de PROJETSE
*
*/
char * getRepSpool() {
    if (getenv("PROJETSE") == NULL) {
        return REPSPOOL;
    } else {
        return getenv("PROJETSE");
    }
}


/**
* @brief recopie un fichier dans un autre
* @param sourceFile le fichier source
* @param targetFile le fichier cible
* @return void
*/
void copyFile(const char * sourceFile, int fd) {

    FILE *source, *target;
    char ch;

    source = fopen(sourceFile, "r");

    if (source == NULL) {
        exit(EXIT_FAILURE);
    }

    target = fdopen(fd, "w");

    if (target == NULL) {
        fclose(source);
        exit(EXIT_FAILURE);
    }

    // On copie tous les caractères
    while ((ch = getc(source)) != EOF) {
        putc(ch, target);
    }

    fclose(source);
    fclose(target);

}
