/**
* @file utilitaire.c
* @brief Toutes les fonctions diverses utiles au projet
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#include "utilitaires.h"

/**
* @brief Permet d'obtenir le répertoire utilisé pour la fille d'attente
* @return Le répertoire
*
* Si la variable d'environnement PROJETSE n'est pas définie, renvoie REPDEMON défini
* Sinon renvoie la valeur de PROJETSE
*
*/
char * getRepSpool() {
    if (getenv("PROJETSE") == NULL) {
        return REPSPOOL;
    } else {
        return getenv("PROJETSE");
    }
}

/**
* @brief Affiche proprement un message d'erreur
* @param msg Permet de rajouter une info supplémentaire
* (ex: la fonction d'où provient l'erreur)
* @return void
*/
void gestErr(char * msg){
    printf("ERREUR: %s : Errno %d : %s\n",msg, errno, strerror(errno));
}

/**
*@brief Affiche les messages de debug (seulement si l'option "-d" a été donnée)
* @param msg Le message à afficher
* @return void
*/
void debugInfo(char * msg){
    if(_DEBUG_FLAG){
        printf("DEBUG: %s\n", msg);
    }
}

/**
* @brief recopie un fichier dans un autre
* @param sourceFile le fichier source
* @param targetFile le fichier cible
* @return void
*/
void copyFile(const char * sourceFile, int fd) {

    FILE *source, *target;
    int ch;

    source = fopen(sourceFile, "r");

    if (source == NULL) {
        exit(EXIT_FAILURE);
    }

    target = fdopen(fd, "w");

    if (target == NULL) {
        fclose(source);
        exit(EXIT_FAILURE);
    }

    // On copie tous les caractères
    while ((ch = getc(source)) != EOF) {
        putc(ch, target);
    }

    fclose(source);
    fclose(target);

}

/**
* @brief Supprime un fichier, s'il existe
* @param path Chemin du fichier à supprimer
* @return void
*/
void deleteFile(const char* path){
    remove(path);
}

char* getIdFromFileName(char* fileName){
    // On récupere l'id
    char * id = malloc(7 * sizeof(char));
    id[6] = '\0';
    int j = 5;
    for (int i = strlen(fileName) - 1; i > (int)strlen(fileName) - 7; i--) {
      id[j] = fileName[i];
      j--;
    }
    return id;
}
char* getCurrentDate(){
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char* date_courante = malloc(sizeof(char)*100);
    strftime(date_courante, 100, "%c", timeinfo);
    return date_courante;
}

char* getUserID(char* chemin){
    struct stat attr;
    stat(chemin, &attr);
    struct passwd *pwd;
    pwd = getpwuid(attr.st_uid);
    char * user = malloc(strlen(pwd->pw_name) * sizeof(char));
    strcpy(user, pwd->pw_name);
    return user;
}

char* substr(char *src,int pos,int len) {
    char *dest=NULL;
    if (len>0) {
      /* allocation et mise à zéro */
      dest = calloc(len+1, 1);
      /* vérification de la réussite de l'allocation*/
      if(NULL != dest) {
          strncat(dest,src+pos,len);
      }
    }
    return dest;
}

char* getRealFileName(char* rawFileName){
    //(void*)rawFileName;
    return substr(rawFileName,2,strlen(rawFileName)-7-2);
}

int getFileSize(char* chemin){
    int fd = open(chemin, O_RDONLY);
    struct stat buf;
    fstat(fd, &buf);
    return buf.st_size;
}
