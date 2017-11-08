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
char * getRepSpool() {
    if (getenv("PROJETSE") == NULL) {
        return REPSPOOL;
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
    if(_DEBUG_FLAG){
        printf("DEBUG: %s\n", msg);
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

/**
* @brief Supprime un fichier, s'il existe
* @param path Chemin du fichier à supprimer
* @return void
*/
void deleteFile(const char* path){
    remove(path);
}

