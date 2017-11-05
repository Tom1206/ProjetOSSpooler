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

#define REPDEMON "../data"


void gestErr(char * msg);
char * getRepDemon();

#endif // UTILITAIRES_H
