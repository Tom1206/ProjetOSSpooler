/**
* @file utilitaires.h
* @brief Header for utilitaires.h
* @author Thomas Millot, Wael Hamed
* @version 1.0
* @date 25/10/2017
*/

#ifndef UTILITAIRES_H
#define UTILITAIRES_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> //strerror

#define REPSPOOL "../data/spool"

//flag indiquant s'il faut afficher les messages d'erreurs ou non
int _DEBUG_FLAG;

void gestErr(char * msg);
void debugInfo(char * msg);
char * getRepSpool();
void copyFile(const char * sourceFile, int fd);

#endif // UTILITAIRES_H
