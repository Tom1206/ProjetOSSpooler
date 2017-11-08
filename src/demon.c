/**
* @file demon.c
* @brief Contient toutes les fonction nécessaires à la création de la commande demon
* @author Thomas Millot, Wael Hammed
* @version 1.0
* @date 25/10/2017
*
*/

#include "demon.h"

char msg[512]; //pour les messages de debug
char msg_log[512]; //pour les messages dans le log
char* chemin_log_demon;
int main(int argc, char const *argv[]) {
    //(void)argc;
    //(void)argv;
    printf("Demon\n");

    int DELAY = 2;
    //analyse des options donnés en arguments
    //note: le dernier arg est l'emplacement du fichier du démon
    int i;

    int shift_last_param = 0; 
    //permet de savoir quel est le rang du dernier paramètre (chemin log démon)
    
    for(i = 1; i<argc - 1; i++){ 
        //arg debug
        if(strcmp(argv[i],"-d")==0){
            _DEBUG_FLAG = 1;
        }
        if(strcmp(argv[i],"-i")==0){
            DELAY = atoi(argv[i+1]);
            shift_last_param++;
        }
    }

    chemin_log_demon = (char*)argv[argc-1];
    snprintf(msg,sizeof(msg),"Chemin du log du démon : %s", chemin_log_demon);
    debugInfo(msg);
    deleteFile(chemin_log_demon);
    //on récupère la date courante et on enregistre la date de démarrage dans le log
    /*time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    */
    char* date_courante = getCurrentDate();
    //char log_demarrage[150];
    snprintf(msg_log, sizeof(msg_log), "Starting at %s\n",date_courante);
    log_demon(msg_log); //on enregistre la date de lancement du démon

    debugInfo("debug flag activated"); //s'affiche seulement si -d a été donné

    
    snprintf(msg,sizeof(msg),"délai d'analyse : %d secondes\n",DELAY);
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
        printf("\n\n");
        sleep(DELAY);
    }

    return 0;
}

int log_demon(char* infos){
    snprintf(msg, sizeof(msg),"enregistrement log dans %s", chemin_log_demon);
    debugInfo(msg);
    int fd = open(chemin_log_demon, O_CREAT | O_APPEND | O_WRONLY, 0777);
    return write(fd,infos,strlen(infos));
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
                
                char substr_[3];
                //on récupère les 2 premiers carac. du nom du fichier
                strncpy(substr_,entry->d_name,2);
                substr_[2] = '\0';
                snprintf(msg,sizeof(msg),"2 premiers carac. du nom du fichier = %s", substr_);                
                debugInfo(msg);
                if(strcmp(substr_,"j_")==0){
                    //dans le cas où c'est un fichier à traiter
                    debugInfo("    -> fichier à analyser");
                    //on log les premières étapes du spool
                    snprintf(msg_log,sizeof(msg_log),"id=%s orgdate=%s user=%s file=%s orgsize=%d",
                    getIdFromFileName(entry->d_name),
                    getCurrentDate(),
                    getUserID(path),
                    getRealFileName(entry->d_name),
                    getFileSize(path)
                    );
                    log_demon(msg_log);

                    char c_path[strlen(path)];
                    strcpy(c_path, path);

                    if(gzip(c_path, entry->d_name)==-1){ //si erreur
                        gestErr("analyser_dossier() -> gzip()");
                        log_demon("exit=1\n");
                    }else{
                        //TODO : tâche terminée, on enregistre le reste des infos dans le log
                        char chemin_fichier_zip[512];
                        snprintf(chemin_fichier_zip, sizeof(chemin_fichier_zip),"%s%s.gz",
                            substr(path,0,strlen(path)-strlen(entry->d_name)),
                            getRealFileName(entry->d_name)
                        );
                        //debugInfo("!!! chemin_fichier_zip = ");
                        //debugInfo(chemin_fichier_zip);
                        snprintf(msg_log, sizeof(msg_log), " curdate=%s gzipsize=%d exit=0\n",
                        getCurrentDate(),
                        getFileSize(chemin_fichier_zip)
                        );
                        log_demon(msg_log);
                        
                        //on supprime la tâche
                        snprintf(msg,sizeof(msg), "suppression de %s", path);
                        debugInfo(msg);
                        deleteFile(path);

                        
                        /*
                        if(log_demon(msg_log)==-1){
                            gestErr("analyser_dossier() -> log_demon()");
                        }
                        */
                    }

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
    

/**
* @brief Utilise le programme gzip afin de compresser un fichier
* @param chemin L'emplacement du fichier à compresser
* @param nom_fichier Nom du fichier source
* @return void
*/
int gzip(const char * chemin, const char* nom_fichier){
    snprintf(msg, sizeof(msg),"-> gzip : fichier à compresser : %s", chemin);
    debugInfo(msg);
    //char cmd[2*strlen(chemin) + 14 -2];
    char cmd[1024];
    //char * destination = (char *)chemin; //dossier où se trouvera le fichier compressé
    //destination[strlen(chemin)-strlen(nom_fichier)] = '\0';
    /*
    snprintf(cmd, sizeof(cmd),"gzip --quiet < %s > %s/%s.gz", 
        chemin, 
        destination,
        nom_fichier+2);
        */
        //chemin complet du fichier de sortie;
        char* sortie;
        sortie = (char*)chemin; //chemin complet du fichier à compresser
        sortie[strlen(chemin)-strlen(nom_fichier)] = '\0'; //on ne garde que le chemin du repertoire
        snprintf(cmd, sizeof(cmd),"gzip -n < %s%s > %s%s.gz", 
        chemin,nom_fichier,
        sortie,getRealFileName((char*)nom_fichier));// nom_fichier+2);
        debugInfo(cmd);
    return system(cmd); //TODO : compresser le fichier dans un fichier temproraire
    //TODO : utiliser un fork et exec* au lieu de la fonction system()
}
