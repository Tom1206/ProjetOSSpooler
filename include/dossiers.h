/**
* @file dossiers.h
* @brief Header for dossiers.c
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#ifndef DOSSIERS_H
#define DOSSIERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //write
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h> //open()
#include <sys/stat.h> // O_RDONLY ...
#include <string.h> //strerror

void afficher_dossier(const char* chemin);

#endif // DOSSIERS_H
