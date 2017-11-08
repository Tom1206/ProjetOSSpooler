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
    
    char c;
    while ((c = getopt(argc, (char * const*)argv, "di:")) != -1){

        switch (c){
            case 'd':
                _DEBUG_FLAG = 1;    
                debugInfo("param d");
                break;
            case 'i':
                DELAY = atoi(optarg);
                debugInfo("param i");
                break;
        }
    }

    // On crée un fichier verrou
    char verrou[50];
    char cheminVerrou[255];
    snprintf(verrou, sizeof(verrou),"J'suis le verrou !");
    snprintf(cheminVerrou, sizeof(cheminVerrou), "%s/verrou", getRepSpool());
    int fd = open(cheminVerrou, O_CREAT | O_APPEND | O_WRONLY, 0777);
    write(fd,verrou,strlen(verrou));
    close(fd);
    

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

                    //on fait un fork
                    pid_t child_pid;
                    int status;
            
                    /* get and print my pid and my parent's pid. */
            
                    debugInfo("!!! FORK");
                    /* print error message if fork() fails */
                    if ((child_pid = fork()) < 0){
                        gestErr("demon.c -> gzip() -> fork()");
                    }
                    
                    if (child_pid == 0){       
                        debugInfo("FILS");  
                        gzip(c_path, entry->d_name); 
                        exit(1);
                        //gzip() utilise execl() donc si on arrive ici 
                        //c'est que execl() ne s'est pas correctement déroulé
                    }
                    else{                  
                        //on est dans le proc. père
                        debugInfo("PERE"); //on attend que le processus fils ait fini
                        wait(&status); 
                        //on vérifie que le proc. fils s'est correctement déroulé
                        if(WIFEXITED(status)){ //bon déroulement
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
                        }
                        else{ //erreur
                            gestErr("analyser_dossier() -> gzip()");
                            log_demon("exit=1\n");
                        }
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
void gzip(const char * chemin, const char* nom_fichier){
    snprintf(msg, sizeof(msg),"-> gzip : fichier à compresser : %s", chemin);
    debugInfo(msg);
        //return system(cmd); //TODO : compresser le fichier dans un fichier temproraire
        //https://www.unix.com/programming/53220-execl-redirecting-output-text-files.html
        
        int fd;
        char chemin_sortie_zip[1024];
        strcpy(chemin_sortie_zip, chemin);
        chemin_sortie_zip[strlen(chemin)-strlen(nom_fichier)] = '\0';
        char chemin_final_zip[1024];
        snprintf(chemin_final_zip, sizeof(chemin_final_zip), "%s%s.gz",
            chemin_sortie_zip, nom_fichier+2
        );
        debugInfo("$$$$ chemin_final_zip = ");
        debugInfo(chemin_final_zip);
        if ((fd = open(chemin_final_zip, O_RDWR | O_CREAT)) == -1){ 
            gestErr("demon.c -> gzip() -> open()");
        }
        
        dup2(fd, STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
        close(fd);               /* close the file descriptor as we don't need it more  */
                                                   //si on est dans le proc. fils
        execl("/bin/gzip", "gzip", "-n", "-f", chemin, (char *)0); //TODO : utiliser un fork et exec* au lieu de la fonction system()
        //gestErr("demon.c -> gzip() -> fork() -> execl()");
}
