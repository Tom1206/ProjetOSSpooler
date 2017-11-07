/**
* @file demon.c
* @brief Contient toutes les fonction nécessaires à la création de la commande demon
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 25/10/2017
*
*/

#include "demon.h"

char msg[512];
int main(int argc, char const *argv[]) {
    //(void)argc;
    //(void)argv;
    printf("Demon\n");

    int DELAY = 2;
    //analyse des options donnés en arguments
    //note: le dernier arg est l'emplacement du fichier du démon
    int i;
    for(i = 1; i<argc - 1; i++){ 
        //arg debug
        if(strcmp(argv[i],"-d")==0){
            _DEBUG_FLAG = 1;
        }
        if(strcmp(argv[i],"-i")==0){
            DELAY = atoi(argv[i+1]);
        }
    }

    debugInfo("debug flag activated");

    
    snprintf(msg,sizeof(msg),"délai d'analyse : %d secondes",DELAY);
    debugInfo(msg);
    //note: le dossier du daemon n'a pas à être spécifié en paramètre
    //const char* chemin = "/home"; 

    //const char* log_demon = argv[argc-1]; 
    
    
    //appel d'une fonction toutes les DELAY secondes
    int CONTINUE = 1;
    while(CONTINUE){
        snprintf(msg,100,"analyse du dossier %s",getRepSpool());
        debugInfo(msg);
        //afficher_dossier(getRepSpool());
        analyser_dossier(getRepSpool());
        printf("\n");
        sleep(DELAY);
    }

    return 0;
}

/**
* @brief Analyse le contenu d'un dossier
* @param chemin Répertoire à analyser 
* @return void
*/
void analyser_dossier(const char* chemin){
    
        DIR *dir;
        struct dirent *entry;
        int indent = 1;
        //on vérifie que le dossier peut être ouvert
        if (!(dir = opendir(chemin))){
            gestErr("demon.c -> analyser_dossier() : ouverture dossier");
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
                
                //on vérifie que le fichier est un fichier à analyser par le spool
                
                char substr[3];
                //on récupère les 2 premiers carac. du nom du fichier
                strncpy(substr,entry->d_name,2);
                substr[2] = '\0';
                snprintf(msg,sizeof(msg),"2 premiers carac. du nom du fichier = %s", substr);                
                debugInfo(msg);
                if(strcmp(substr,"j_")==0){
                    debugInfo("    -> fichier à analyser");
                }
                else{
                    debugInfo("    -> fichier à ne PAS analyser");
                }
            }
        }
        closedir(dir);
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
    