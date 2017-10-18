#include <stdio.h> //sleep
#include <unistd.h> //write
#include <errno.h> 
#include <string.h> //strerror

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h> //open()
#include <sys/stat.h> // O_RDONLY ...

//affichage des erreurs
void gestErr(char * msg){
    printf("ERREUR: %s : Errno %d : %s\n",msg, errno, strerror(errno));
}

//liste les noms desfichiers d'un dossier
//Note: fonction non-récursive (apparemment pas necessaire pour un spooler)
//TODO: utiliser cet algo pour commencer à analyser les n
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

int main(int argc, char const *argv[]) {
    //(void)argc;
    //(void)argv;
    printf("Hello World\n");

    const char* chemin = (int)argc > 1 ? argv[1] : "/home/joseffu/Travaux/SE";
    //appel d'une fonction toutes les n secondes
    int i;
    int n = 2;
    for(i = 0; i < 100000; i++){
        afficher_dossier(chemin);
        printf("\n");
        sleep(n);
    }

    return 0;
}
