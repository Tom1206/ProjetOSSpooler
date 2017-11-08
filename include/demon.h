/**
* @file demon.h
* @brief Header for demon.c
* @author Thomas Millot
* @version 1.0
* @date 25/10/2017
*/

#ifndef DEMON_H
#define DEMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //write
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h> //open()
#include <sys/stat.h> // O_RDONLY ...
#include <string.h> //strerror
#include <sys/wait.h> //wait(), fork()

#include "utilitaires.h"


void afficher_dossier(const char* chemin); //TODO : penser à supprimer cette fonction (de test) avant le rendu du projet
void analyser_dossier(const char* chemin);
int gzip(const char * chemin, const char* nom_fichier);
int log_demon(char* infos);

#endif // DEMON_H
