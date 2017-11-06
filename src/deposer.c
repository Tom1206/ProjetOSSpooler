/**
* @file deposer.c
* @brief Contient toutes les fonction nécessaires à la création de la commande deposer
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 06/11/2017
*
*/

#include "deposer.h"

int main(int argc, char const *argv[]) {
    // Pour chacuns des fichiers en paramètres...
    for (int i = 1; i < argc; i++) {
        // On teste si le fichier existe
        if (access(argv[i], F_OK) != 0) {
            printf("Le fichier \"%s\" n'existe pas\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        // On crée le fichier job dans
        char tmpName[512];
        strncpy(tmpName, getRepSpool(), 512);
        strncat(tmpName, "/j_", 512);
        //strncat(tmpName, argv[i], 512);
        strncat(tmpName, "_XXXXXX", 512);
        printf("%s\n", tmpName);
        int fd = mkstemp(tmpName);

        copyFile(argv[i], fd);

    }
    return 0;
}
