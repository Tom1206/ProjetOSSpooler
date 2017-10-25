#include "dossiers.h"
#include "utilitaire.h"

int main(int argc, char const *argv[]) {
    //(void)argc;
    //(void)argv;
    printf("Hello World\n");

    const char* chemin = (int)argc > 1 ? argv[1] : "/home";
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
