//netcdf
#ifndef NCUTIL
#define NCUTIL

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <netcdf.h>

#define YYDOUBLE //Ã©quivalent du YDOUBLE/YFLOAT de YAO
//normalement YYDOUBLE et YBOUBLE doivent Ãªtre dÃ©finis en mÃªme temps
//VerifiÃ© dans appli_start


#ifdef YFLOAT
#define NC_REAL NC_FLOAT
#define nc_put_vara_real nc_put_vara_float
#ifdef YO_CADNA
#define YREAL float_st
#else 
#define YREAL float
#endif

#else
#define NC_REAL NC_DOUBLE
#define nc_put_vara_real nc_put_vara_double
#ifdef YO_CADNA
#define YREAL double_st
#else
#define YREAL double
#endif

#endif

void    handle_error (int status, char *txt);///*fonction de message d'erreur
int Ouvre_nc(char const *var_file );///* Ouvre un fichier nc et renvoie son id var_file_id
int Ouvre_nc_write(char const *var_file);   ///* ouvre un fichier nc pour Ã©criture et renvoie son id var_file_id
int Var_id(int  var_file_id,char* varn); ///*initial traceurs actifs


void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, double vect[]);
void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, int vect[]);
// lire volume 3D au temps t
// le parametre t commence de 0 et pas de 1
// (t,znbval,ynbval,xnbval,ncid, varid):
// lit la variable varid aux points (t,1:znbval,1:ynbval,1:xnbval) et
// renvoie un vecteur de dim znbval*ynbval*xnbval

void voncwrite(int t,int znbval,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);


void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, double vect[]);
void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, float vect[]);
void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, int vect[]);
// lire surface 2D au temps t
// le parametre t commence de 0 et pas de 1
// (t,ynbval,xnbval,ncid, varid):
// lit la variable varid aux points (t,1:ynbval,1:xnbval) et
// renvoie un vecteur de dim ynbval*xnbval
void snclire(int ynbval, int xnbval, int ncid, int varid, float vect[]);
void snclire(int ynbval, int xnbval, int ncid, int varid, double vect[]);

void soncwrite(int t,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);

void zonclire(int t,int znbval, int ncid, int varid, double vect[]);
void zonclire(int t,int znbval, int ncid, int varid, int vect[]);
// lire surface 1D au temps t
// le parametre t commence de 0 et pas de 1
// (t,znbval,ncid, varid):
// lit la variable varid aux points (t,1:znbval) et
// renvoie un vecteur de dim znbval

void znclire(int znbval, int ncid, int varid, float vect[]);
void znclire(int znbval, int ncid, int varid, double vect[]);

void scalarlire (int ncid,int varid, int *var);
void scalarlire (int ncid,int varid, float *var);
void scalarlire (int ncid,int varid, double *var);

void scalarwrite (int ncid,char const *var_name,int var);
void scalarwrite (int ncid,char const *var_name,float var);
void scalarwrite (int ncid,char const *var_name,double var);

void zoncwrite(int t,int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[]);

void zncwrite(int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[]);
void sncwrite(int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);

void twrite(int t, char *var_name,int ncid,int tid,int var);
void twrite(int t, char *var_name,int ncid,int tid,float var);
void twrite(int t, char *var_name,int ncid,int tid,double var);

#endif

