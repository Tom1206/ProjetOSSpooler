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

    // On vérifie si on est le même utilisateur que celui qui a lancé le spool
    // Pour ce faire on vérifie qui a crée le verrou

    // On génère le chemin du verrou
    char cheminVerrou[255];
    snprintf(cheminVerrou, sizeof(cheminVerrou), "%s/verrou", getRepSpool());

    // bonUID vaut 0 si même utilisateur que spool, et autre chose sinon
    int bonUID = strcmp(getUserID(cheminVerrou), getlogin());

    // afficheNom vaut 0 si on doit afficher les noms de fichier, et autre chose sinon
    int afficheNom = 1;

    // testUIDaAfficher vaut 0 si on doit afficher un utilisateur précis, et autre chose sinon
    int testUIDaAfficher = 1;
    // Dans le cas 0, l'utilisateur à afficher
    char * UIDaAfficher;

    // On teste les options
    int c;
    while ((c = getopt(argc, (char * const*)argv, "lu:")) != -1){

        switch (c) {
            case 'l':
                if (bonUID == 0) {
                    afficheNom = 0;
                } else {
                    printf("Options -l ou -u sont limitées au propriétaire du spool\n");
                    return 1;
                }
                break;
            case 'u':
                if (bonUID == 0) {
                    testUIDaAfficher = 0;
                    UIDaAfficher = malloc(strlen(optarg) * sizeof(char));
                    UIDaAfficher = optarg;
                } else {
                    printf("Options -l ou -u sont limitées au propriétaire du spool\n");
                    return 1;
                }
                break;
            default:
                return 1;
        }
    }


    DIR *dp ;
    struct dirent *d ;
    dp = opendir(getRepSpool());

    while ((d = readdir (dp)) != NULL) {

        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {

            // On vérifie que c'est bien un job

            char substr_[3];
            //on récupère les 2 premiers carac. du nom du fichier
            strncpy(substr_,d->d_name,2);
            substr_[2] = '\0';

            if(strcmp(substr_,"j_")==0) {

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


                // On affiche le tout en fonction des options

                if (testUIDaAfficher == 0) {
                    if (strcmp(UIDaAfficher, user) == 0) {
                        printf("%s %s ", id, user);
                        if (afficheNom == 0) {
                            printf("%s", name);
                        }
                        printf("     %s", date);
                    }
                } else {
                    printf("%s %s ", id, user);
                    if (afficheNom == 0) {
                        printf("%s", name);
                    }
                    printf("     %s", date);
                }

                // On libère la mémoire
                free(id);
                free(name);
                free(user);
                free(date);

            }
        }
    }


    // On ferme le répertoire
    if (closedir(dp) == -1) {
        debugInfo("Erreur à la fermeture du fichier");
    }

    return 0;
}
