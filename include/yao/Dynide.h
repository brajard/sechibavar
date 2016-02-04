
/*/////////////////////////////////////////////////////////////////////////////
/////////////////////// 1e HEADER de DYNAMO (dynide.h) //////////////////////*/
/******************************************************************************
                        dynide.h  :  Include, DEfine, (Etc...)
Ysys error #next=2
******************************************************************************/

/*///////////////////////////////////////////////////////////////////////////*/
/*              LES INCLUDES DE DYNAMO ET POUR TOUT LE MONDE                 */
//#include <iomanip.h>
#include <iostream>

#include <iomanip>
#include <stdio.h>	
#include <string.h>
//#include <fstream.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>	
#include <math.h>
#include <signal.h>
#include <stdarg.h>
//#include <ccomplex>
#include <complex>
//#include <complex.h>  //pour octopus (Mandrake et ces pb de deprecated ...!?)

/*///////////////////////////////////////////////////////////////////////////*/
/*                  DES DEFINES PROPRES A YAO                                */
#define BUFSIZE						1024
#define STRSIZE20					20
#define STRSIZE80					80
#define NB_MAX_ARG_CDE		30		/* nombre maxi d'arguments pour une commande */
#define ON								 1
#define OFF								 0
#define OK								 1
#define KO								 0
#define OUI								 1
#define NON								 0
#define PI                 3.14159265358979323846
#define	LG_MAX_NAME				24				/* longueur max d'un nom ou mot a usage divers */

using namespace std;
/*///////////////////////////////////////////////////////////////////////////*/

#ifdef LINUXPC
						// ici les fonctions sont underscorees
extern void *scpctl_;
extern void *ctlcan_;
extern void *canctl_;
//extern "C" void m1qn3_ (void f(long *, long *, float [], float *, float [], long [], float [], double []),
extern "C" void m1qn3_ (void f(int *, long *, float [], float *, float [], long [], float [], double []),
                        void **, void **, void **,
                        long *, float [], float *, float [], float *, float *,
                        float *, long *, long *, long *, long *, long *, long [], float [], long *,
                        long [], float [], double []
                        );
//#ifdef YO_M2QN1 ici, on ne sait pas encore si YO_M2QN1 va etre defini ...
//extern "C" void m2qn1_ (void f(long *, long *, float [], float *, float [], long [], float [], double []),
extern "C" void m2qn1_ (void f(int *, long *, float [], float *, float [], long [], float [], double []),
                        long *, float [], float *, float [], float [], float *, float *,
                        long *, long *, long *, long *, long *, float [], float [],
                        long [], float [], long [], float [], double []
                        );
//#endif
#else
						// la elles ne le sont pas
extern void *scpctl;
extern void *ctlcan;
extern void *canctl;
//extern "C" void m1qn3 (void f(long *, long *, float [], float *, float [], long [], float [], double []),
extern "C" void m1qn3 (void f(int *, long *, float [], float *, float [], long [], float [], double []),
                        void **, void **, void **,
                        long *, float [], float *, float [], float *, float *,
                        float *, long *, long *, long *, long *, long *, long [], float [], long *,
                        long [], float [], double []
                        );
//#ifdef YO_M2QN1 ... idem ici, (ce ne devrait pas etre genant (?)  ;
//extern "C" void m2qn1 (void f(long *, long *, float [], float *, float [], long [], float [], double []),
extern "C" void m2qn1 (void f(int *, long *, float [], float *, float [], long [], float [], double []),
                        long *, float [], float *, float [], float [], float *, float *,
                        long *, long *, long *, long *, long *, float [], float [],
                        long [], float [], long [], float [], double []
                        );
//#endif
#endif

/*///////////////////////////////////////////////////////////////////////////*/
