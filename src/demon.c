/**
* @file demon.c
* @brief Contient toutes les fonction nécessaires à la création de la commande demon
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 25/10/2017
*
*/

#include "demon.h"


int main(int argc, char const *argv[]) {
    //(void)argc;
    //(void)argv;
    printf("Demon\n");

    //analyse des arguments
    int i;
    for(i = 1; i<argc; i++){

        //arg debug
        if(strcmp(argv[i],"-d")==0){
            _DEBUG_FLAG = 1;
            debugInfo("debug flag activated\n");
        }
    }

    //note: le dossier du daemon n'a pas à être spécifié en paramètre
    const char* chemin = "/home"; 
    
    //appel d'une fonction toutes les n secondes
    int n = 2;
    for(i = 0; i < 100000; i++){
        afficher_dossier(chemin);
        printf("\n");
        sleep(n);
    }

    return 0;
}

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
