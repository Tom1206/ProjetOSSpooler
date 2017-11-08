/**
* @file retirer.c
* @brief Contient toutes les fonction nécessaires à la création de la commande retirer
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 07/11/2017
*
*/

#include "retirer.h"

int main(int argc, char const *argv[]) {
    
    // Pour chacuns des id en paramètres...
    for (int i = 1; i < argc; i++) {

        // On crée un test pour savoir si on trouve l'id
        int test = 0;

        DIR *dp ;
        struct dirent *d ;
        dp = opendir(getRepSpool());

        while ((d = readdir (dp)) != NULL) {

            if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {

                // On vérifie avec un REGEX que l'id demandé existe
                regex_t regex;
                regcomp(&regex, argv[i], 0);

                // Si on le trouve, on supprime le fichier de la file
                if (! regexec(&regex, d->d_name, 0, NULL, 0)) {
                    // On doit formater le nom du fichier à supprimer
                    char * target = malloc(sizeof(char));
                    strcpy(target, getRepSpool());
                    strcat(target, "/");
                    strcat(target, d->d_name);
                    remove(target);
                    free(target);

                    test = 1;

                }

                regfree(&regex);
            }
        }

        // Si test = 0 c'est que l'on a pas trouvé l'id recherché
        // On ferme le fichier et on affiche si l'id n'existe pas

        if (closedir(dp) == -1) {
            debugInfo("Erreur à la fermeture du fichier");
        }

        if (test == 0) {
            //printf("Cannot find id %s\n", argv[i]);
            return 1;
        }

    }
    return 0;
}
