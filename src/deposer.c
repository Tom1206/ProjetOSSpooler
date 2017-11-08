/**
* @file deposer.c
* @brief Contient toutes les fonction nécessaires à la création de la commande deposer
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 06/11/2017
*
*/

#include "deposer.h"
#include "utilitaires.h"

int main(int argc, char const *argv[]) {
    // Pour chacuns des fichiers en paramètres...
    for (int i = 1; i < argc; i++) {

        // On teste si le fichier existe
        if (access(argv[i], F_OK) != 0) {
            printf("Le fichier \"%s\" n'existe pas\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        // On teste si le spool existe
        if (access(getRepSpool(), F_OK) != 0) {
            printf("Le spool \"%s\" n'existe pas\n", getRepSpool());
            exit(EXIT_FAILURE);
        }

        // On génère l'adresse du verrou et on teste son existence
        

        // On crée le fichier job, de la forme j_nomDuFichierOriginal_XXXXXX
        char tmpName[512];
        strncpy(tmpName, getRepSpool(), 512);

        strncat(tmpName, "/j_", 512);
        char * totalName = malloc(sizeof(char));
        strcpy(totalName, argv[i]);
        strncat(tmpName, basename(totalName), 512);
        free(totalName);
        strncat(tmpName, "_XXXXXX", 512);
        //printf("%s\n", tmpName);
        int fd = mkstemp(tmpName);

        //TODO : vérifier que le dossier data/spool existe, sinon le programme crash

        if(fd == -1){
            char msg[1024];
            snprintf(msg,sizeof(msg),"deposer -> mkstemp \n tmpName = %s",tmpName);
            gestErr(msg);
        }

        // On copie le fichier à ajouter dans le nouveau fichier crée
        copyFile(argv[i], fd);

    }
    return 0;
}
