/**
* @file dossiers.c
* @brief Contient toutes les fonction de traitement de dossier
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#include "dossiers.h"
#include "utilitaire.h"


//liste les noms desfichiers d'un dossier
//Note: fonction non-récursive (apparemment pas necessaire pour un spooler)
//TODO: utiliser cet algo pour commencer à analyser les nom des fichier
void afficher_dossier(const char* chemin){

    DIR *dir;
    struct dirent *entry;
    int indent = 1;
    //on vérifie que le dossier peut être ouvert
    if (!(dir = opendir(chemin))){
        gestErr("Ouverture dossier copyDir()");
        return;
    }

    //lecture de chaque élément du répertoire, l'un après l'autre
    while ((entry = readdir(dir)) != NULL) {
        //si l'élément est un répertoire
        if (entry->d_type == DT_DIR) {
            char path[1024];
            //on ignore le dossier courant ou son parent
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", chemin, entry->d_name);
            printf("%*s[%s]\n", indent, "", path/*entry->d_name*/);


        }
        //sinon c'est un fichier
        else {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", chemin, entry->d_name);
            printf("%*s- %s\n", indent, "", path /*entry->d_name*/);
        }
    }
    closedir(dir);
}
