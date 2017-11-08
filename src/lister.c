/**
* @file lister.c
* @brief Contient toutes les fonction nécessaires à la création de la commande lister
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 07/11/2017
*
*/

#include "lister.h"


int main(int argc, char const *argv[]) {
    (void) argc;
    (void) argv;

    DIR *dp ;
    struct dirent *d ;
    dp = opendir(getRepSpool());

    while ((d = readdir (dp)) != NULL) {

        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {

            // On récupere l'id (fin du nom de fichier dans le spool)
            char * id = malloc(7 * sizeof(char));
            id[6] = '\0';
            int j = 5;
            for (int i = strlen(d->d_name) - 1; i > (int)strlen(d->d_name) - 7; i--) {
              id[j] = d->d_name[i];
              j--;
            }


            // On récupère le nom de fichier (milieu du nom de fichier dans le spool)
            char * name = malloc((strlen(d->d_name) - 8) * sizeof(char));
            name[(strlen(d->d_name) - 9)] = '\0';
            j = 0;
            for (int i = 2; i < (int)strlen(d->d_name) - 7; i++) {
              name[j] = d->d_name[i];
              j++;
            }


            // On récupère la date (date de dernière modification)
            char path[512];
            strncpy(path, getRepSpool(), 512);
            strncat(path, "/", 512);
            strncat(path, d->d_name, 512);
            struct stat attr;
            stat(path, &attr);
            char * date = malloc(strlen(ctime(&attr.st_mtime)) * sizeof(char));
            strcpy(date, ctime(&attr.st_mtime));


            // On récupère l'utilisateur
            struct passwd *pwd;
            pwd = getpwuid(attr.st_uid);
            char * user = malloc(strlen(pwd->pw_name) * sizeof(char));
            strcpy(user, pwd->pw_name);


            // On affiche le tout
            printf("%s %s %s       %s\n", id, name, user, date);

            // On libère la mémoire
            free(id);
            free(name);
            free(user);
            free(date);

        }
    }

    printf("%s\n", getlogin());

    // On ferme le répertoire
    if (closedir(dp) == -1) {
        debugInfo("Erreur à la fermeture du fichier");
    }

    return 0;
}
