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
#include <time.h> //récupérer date courante


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h> //getpwuid()
#include <fcntl.h> //open()


#define REPSPOOL "../data/spool" //TODO : ne fonctionne pas selon l'endroit d'où on lance le programme (demon.c -> analyser_dossier() -> opendir())

//flag indiquant s'il faut afficher les messages d'erreurs ou non
int _DEBUG_FLAG;

void gestErr(char * msg);
void debugInfo(char * msg);
char * getRepSpool();
void copyFile(const char * sourceFile, int fd);
void deleteFile(const char* path);
char* getIdFromFileName(char* fileName);
char* getCurrentDate();
char* getUserID(char* chemin);
char* getRealFileName(char* rawFileName);
int getFileSize(char* chemin);
char* substr(char *src,int pos,int len);
int verifVerrou();

#endif // UTILITAIRES_H
