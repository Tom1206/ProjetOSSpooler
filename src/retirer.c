/**
* @file retirer.c
* @brief Short description
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 07/11/2017
*
*/

#include "retirer.h"

int main(int argc, char const *argv[]) {
    // Pour chacuns des id en paramètres...
    for (int i = 1; i < argc; i++) {

        DIR *dp ;
        struct dirent *d ;
        dp = opendir(getRepSpool());

        while ((d = readdir (dp)) != NULL) {

            if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {

                regex_t regex;
                regcomp(&regex, argv[i], 0);

                if (! regexec(&regex, d->d_name, 0, NULL, 0)) {
                    char * target = malloc(sizeof(char));
                    strcpy(target, getRepSpool());
                    strcat(target, "/");
                    strcat(target, d->d_name);
                    remove(target);
                    free(target);
                }

                regfree(&regex);
            }
        }

        if (closedir (dp) == -1) {
            printf("Erreur à la fermeture du fichier\n");
        }

    }
    return 0;
}
