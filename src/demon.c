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
int DELAY;
int CONTINUE;
int main(int argc, char const *argv[]) {

    debugInfo("Demon\n");

    DELAY = 2;
    int flag_foreground = 0; 

    //analyse des options donnés en arguments
    char c;
    while ((c = getopt(argc, (char * const*)argv, "dfi:")) != -1){

        switch (c){
            case 'd':
                _DEBUG_FLAG = 1;
                debugInfo("param d");
                break;
            case 'f':
                flag_foreground = 1;
                debugInfo("param f");
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
    char* date_courante = getCurrentDate();
    snprintf(msg_log, sizeof(msg_log), "Starting at %s\n",date_courante);
    log_demon(msg_log); //on enregistre la date de lancement du démon

    debugInfo("debug flag activated"); //s'affiche seulement si -d a été donné

    snprintf(msg,sizeof(msg),"délai d'analyse : %d secondes\n",DELAY);
    debugInfo(msg);

    CONTINUE = 1;
    if(flag_foreground == 0){ //on lance le prog en arrière-plan
        pid_t child_pid = fork();
        if(child_pid >= 0){ //fork réussi
            if(child_pid ==0){ //on est dans le fils
                start();
            }
            else{
                exit(0); //on tue le père
            }
        }else{
            gestErr("demon.c -> main() -> fork()");
            exit(1);
        }
    }
    else{
        start();
    }
    return 0;
}

void start(){
    while(CONTINUE){
        snprintf(msg,100,"analyse du dossier %s",getRepSpool());
        debugInfo(msg);

        // On vérifie le verrou, et on bloque tant que verrouillé
        while (verifVerrou() != 0);

        analyser_dossier(getRepSpool());
        debugInfo("\n\n");
        sleep(DELAY);
    }
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
            if(errno == 2){
                mkdir(chemin, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);            
            }else{
                gestErr("demon.c -> analyser_dossier() : ouverture dossier");
                return;
            }
            
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
                snprintf(msg,sizeof(msg),"%*s[%s]\n", indent, "", path/*entry->d_name*/);
                debugInfo(msg);
            }
            //sinon c'est un fichier
            else {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", chemin, entry->d_name);
                snprintf(msg,sizeof(msg),"%*s- %s\n", indent, "", path /*entry->d_name*/);
                debugInfo(msg);

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