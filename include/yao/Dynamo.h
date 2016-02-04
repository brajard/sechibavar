
/*/////////////////////////////////////////////////////////////////////////////
/////////////////////// 2e HEADER de DYNAMO (dynamo.h) //////////////////////*/
/******************************************************************************
                    dynamo.h  :  ici commence l'encapsulage ...
******************************************************************************/

/*///////////////////////////////////////////////////////////////////////////*/
/*		DES ELEMENTS POUR M1QN3 */
/*#define	Y3_M						 8 	ou en variable globale Y3m ? anywhere now this is done by the generator ! */
int 		Y3run(char vers);			/* add first for "chloro" (Frederic) */
void  	Y3valstate_all();			/* add first for "atmos"  (Cedric)   */
int			Y3windice;						/* indice de travail pour l'affectation des tableaux Y3x et Y3g */
int 		Yc3_run(char vers);
long    Y3modo;               /*  mode de sortie de m1qn3 (a toute fin utile) */

/*///////////////////////////////////////////////////////////////////////////*/
/* 		DES PROTOTYPAGES POUR TOUS LE MONDE                                  */
/*Des fonctions de services de Yao ------------------------------------------*/
template<class TypVal>int	Yinv_tri_vect (int mat_dim, TypVal vect[], TypVal prod[], TypVal inf[], TypVal diag[], TypVal sup[]);
template<class TypVal>int Yinv_tri_mat (int mat_dim, TypVal inf[], TypVal diag[], TypVal sup[], TypVal mat_inv[]);
template<class TypReel>void Yvsmatt(int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[]);
//template<class TypReel>void YvsmattLuigi(int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[]);
template<class TypReel>void Yvsmat (int lig, int col, int max_col, TypReel vout[], TypReel Mat[], TypReel vin[]);
template<class TypValA, class TypValB, class TypValX>void Ymxmat (int la, int cl, int cb, TypValA MatA[], TypValB MatB[], TypValX MatX[]);
template<class TypReal> void Ycx_jac(complex<TypReal> derivee, int j, int i);
template<class TypReal> void Ycxr_jac(complex<TypReal> derivee, int l, int c);

/*Des fonctions standards de Yao (a completer au fur et a mesure selon les
  besoins ...?) -------------------------------------------------------------*/
template<class TypReal>void Yiorwf (char *nmmod, int w4, int w1, int w2, int w3, int wt, TypReal *valout, TypReal valin);
int     Yio_savestate (char *nmmod, char *oaxis, int pdt, char *filename);
void  	Yrun(long Yit);
void 		Ybasic_it();
void 		Yforwardrun(int itraj, int nbpas);	//Yforwardrun(long Yit, int Ypas);
int 		Yforward(int itraj, int topstop);   //Yforward(int Ypas);
int 		Yforward_order();	
void 		Ycobs(); //Ycost();
void 		Ycostlms_all ();
void 		Ybackwardrun(int itraj, int nbpas); //Ybackwardrun(long Yit, int Ypas);
void 		Ybackward(int itraj, int topstop);  //Ybackward(int Ypas);
int  		Ybackward_order();
void    Ylinward(int Ypas);
void 		Yadjust();
void 		Yadjust_all ();
int 		Yimod(char *nmmod);
void    Ysetting(char *codop);
void    Ydbg_ting(char *str, int nbi, char *nmmod);
void    Ydbg_beta(char *str, int nbi, char *nmmod);
void    Ydbg_nanf(char *str, int nbi, char *nmmod);
int     Ytesterrad_mod(char *nmmod, double YLTRes, double YAdRes);
int     Yset_modeltime (int time);  //PLM, je ne traite que le repositionnement au debut du modele !!! (ARAR)
int     Yispace(char *spacename);
int     Yitraj(char *trajname);
int     Ytttt_pdt (int wxtraj, int topy, int wytraj);
int     Yactraj(int cdesc, char *cdes[]);
int     Yfarg(int(*fct)(int argc, char *argv[]), char *args);
int 	  Yrecup_cde (int *cdesc, char *cdeline, char *cdes[]);
void    Ylopera();
void	  Ylistobs(int itraj);
int     Yentry0 (int argc, char *argv[]);

/*Des fonctions reseaux de neurones de Yao: celles-ci sont dans Dynnet.h ----*/


/* ///////////////////////////////////////////////////////////////////////// */
/*			DES TYPES ET STRUCTURES DE DONNEES                                   */
enum YCostKind		/* les fonctions de cout : */
{		COST_APPLI,			 /* fonction de cout propre à l'application */
		COST_LMS				 /* fonction de cout lms standard de Yao */
};
enum YCostL2Kind		/* les fonctions de cout : */
{		//COSTL2_GRAD,		 /*  */
		//COSTL2_ONLY			 /*  */
    //,COSTL3_PROJECT	 /*  */
    //,COSTL3_NANACT	 /*  */
    //,
		WISHL2_NANACT		   /*  */
    ,WISHL2_DIFF	     /*  */
    ,WISHL2_QTEA	     /*  */
    ,COSTL2_WISHDIFF   /*  */
    ,COSTL2_WISHQTEA	 /*  */
    ,COSTL2_GRAD  		 /*  */
};
enum YAdjustKind	/* le type d'ajustement : */
{		ADJUST_APPLI,		 /* fonction d'ajustement propre à l'application */
		ADJUST_STD,			 /* fonction d'ajustement standard (et classique) de Yao */
		ADJUST_M1QN3		 /* M1QN3 (PLM: pas vraiment utile puisque c'est d'office forced avec m1qn3, but ...) */
};
enum YRunL1Kind		/* type de run (run algo mode) au 1er niveau :*/
{		RUNL1_STD,			 /* soit methode de base simple, standard */
	  RUNL1_M1QN3		 /* soit 'a la maniere de'  m1qn3 */
};
enum YRunL2Kind		/* le mode de fonctionnement de run (run algo mode), 2ème niveau : */
{		RUNL2_STD,				/* soit algo de base simple, standard */
		RUNL2_INCR			/* soit algo incremental */
};
enum YioKind		  /* le cas d'io concerne : */
{		YIO_LOADSTATE,	 /* chargement des etats (not yet) */
		YIO_SAVESTATE,	 /* sauvegarde des etats */
		YIO_LOADOBS,		 /* chargement des observations */
		YIO_SAVEOBS,		 /* sauvegarde des observations (not yet & why?) */
		YIO_OUTOOBS,		 /* copie state->obs */
		YIO_OUTOEBX,		 /* copie state->ebauche (in the same arbobs) */
		YIO_OBSTOUT,		 /* copie obs->state (not yet & why?) */
		YIO_LISTOBS			 /* liste des observations */
};
/* type du run courant de trajectoire (pour hidjack)*/
enum YWardKind
{		FORWARD,
		BACKWARD,
		LINWARD
};
/* structure d'un noeud de l'arborescence des observations */
struct   Yst_nodo  {
	int		 iind; /* indice pour l'indicateur t, m, s, i, j */
	struct Yst_nodo *frere;
	struct Yst_nodo *fils;
};

/* structure d'une trajectoire */
struct Yst_traj {
	char		name[STRSIZE80+1];
	char    type;
	int		  nbuptime;
	float   offtime;
	float   dtime;			   // delta t
	float		curtime;
	int			toptime;       // temps unitaire
	int		  nbsteptime;
	float		stoptime;      //=Ybegintime + offtime + (dt*nbsteptime)   (nb: initialement, Ybegintime=0)
	int     (*fward)(int nbt);
	int     (*bward)(int nbt);
	int     (*lward)(int nbt);
	int     (*dfward)(int modop, char *nmmod, int All, int KeKo, int koleft, float pdx, float ptol, int yi, int yj, int yk);
	short   isactiv;
	struct  Yst_nodo *YRobs;      //=NULL; // Racine de l'Arborescence des observations
  struct  Yst_nodo *TRobs;      // un pointeur sur le Time   s/Root courant trouve
};
/* structure d'un espace */
struct Yst_space {
	char		name[STRSIZE80+1];
	char    type;
	int			axi;
	int			axj;
	int			axk;
	char		traj_name[STRSIZE80+1];
};
/* structure d'un operateur */
struct Yst_opera {
	char		name[STRSIZE80+1];
	char    type;
	int			axi;
	int			axj;
	int			axk;
	char		traj_name[STRSIZE80+1];
	short   isactiv;
};
/* structure d'un module */
struct Yst_modul {
	char		Name[STRSIZE80+1];
	void		*Tadr;
	int			dim;
	int			axi;
	int			axj;
	int			axk;
	int			nb_input;
	int			nb_stout;
	int			nb_time;
	int			is_cout;
	int			is_target;
	int			deb_target;
	int			end_target;
	double	scoef;
	double	bcoef;
	double	pcoef;
	int     ctrord;
	char		space_name[STRSIZE80+1];
	//short   sys_flagobs;
};
/* structure d'un reseau de neurone */
typedef enum {SigLin, SigSig} Activation;
struct Yst_netward {
	char		Name[STRSIZE80+1];
	int 		nbweight;
	int			nbinput;
	int			nboutput;
	int 		maxcell;
	double  *tweight;
	Activation activ;
};
/* structure des definitions de valeurs */
struct Yst_defval {
	char		Name[STRSIZE80+1];
	char		macro[STRSIZE80+1];
};

short		 Ysysdbg;	/* reserved for system supervisor */

/* ///////////////////////////////////////////////////////////////////////// */
/* 			 LES GLOBALES DE YAO ET POUR TOUT LE MONDE                           */
long		 Ytop0;									/* un top global pour decompter le temps */
int			 YTemps;								/* horloge des pas de temps a patir d'NBUPTIME */
//float  YTotalCost;						/* cout total */
double   YTotalCost;						/* cout total */
double   YCoefGrad = 1.0;				/* coeff de calcul du gradient (defaut=1) */
enum 		 YCostKind	 YTypeCost=COST_LMS;	/* le type de la fonction de cout (default=lms) */
enum 		 YCostL2Kind YCaseCost=COSTL2_GRAD; //COSTL2_GRAD;	/*  */
enum 		 YAdjustKind YTypeAdjust=ADJUST_STD;	/* le type d'ajustement (default=standard) */
enum		 YRunL1Kind	YAL1Run=RUNL1_STD; /* mode (algo) du run au 1er niveau (default=standard : definie par la commande run utilisee) */
enum		 YRunL2Kind	YAL2Run=RUNL2_STD; /* mode (algo) du run au 2em niveau (default=standard : definie par la commande run utilisee) */
long		 YNbItRun;							/* nombre d'itérations demande ((IO)niter p/m1qn3) */
long		 YItRun;								/* l'iteration courante du run */
long		 YNbExtL;							  /* nombre de boucles externes pour les algo incrementaux */
int			 YDispTime;							/* boolean pour l'affichage du temps */
int			 YDispCost;							/* boolean pour l'affichage du cout */
char		 YPrompt[STRSIZE20+1];	/* le prompt pour la saisie des commandes */
int			 YEcho = ON;						/* echo de la commande */

int                      Yi, Yj, Yk;                                               /* indice de parcourt de l'espace gabarit */
int YY;						/* The variable YY is used only as artifact (in the parallelization) for the bug of openMP 
						 * that does not allow to global variables passed to a function to conserve the value 
						 * of the calling function.
						 * The value passed to the function is uncorrect (probably is the initialization value) */

int			 YY_RUNBRK=ON;					/* gestion signal d'interruption pour breaker le run */
short		 YM_EXE;  							/* Mode d'execution: 'B': Batch; 'I': intercatif */
//int		 YM_GRADTEST = OFF; 		/* Mode test du gradient: pour tester les derivees (ne pas affecter */
				 												/*      les inputs des modules) */
int 		 Yszint, Yszreal; 			/* tailles d'un int et d'un reel (initialises au debut de Dynamo.cpp */
FILE		 *Yiofp;								/* file pointeur pour les fonctions d'entree/sortie */
short	   YioWrite, YioRead;			/* flag (booleen) pour le mode de fonctionnement des fct d'entree/sortie */
short	   YioBin, YioAscii;			/* flag pour le type de stockage des donnees: binaire ou ascii */
short		 YioState, YioTime, YioAxes; /* flag pour la convention, le format de stockage des donnees */
int			 YioszReal;             /* taille d'un reel pour des valeurs (binaire?) a loader (en provenance de l'exterieur */
short		 YioModulot;						/* modulo t pour les sauvegardes de donnees */
int		   YioInsertObsCtr=-1;		/* compteur d'obs inserees (-1 pour savoir si y'a au moins 1 obs. Utile pour testad */	
int		   YioOvrObsCtr;					/* compteur d'obs overidees */	
int		   YioSelectedCtr;				/* compteur de selection */	
int			 YioDi, YioDj, YioDk;		/* dimension d'un espace exterieur a charger */
int			 Yiodi, Yiodj, Yiodk;		/* decalage a appliquer sur les coordonnes d'un espace exterieur a charger */		
char     YioSep[STRSIZE20]="\n";/* chaine de caratères pour la separation de token dans les fct d'io (modifiable pas set_sep) */
int			 Ydmod;									/* dimension du module en cours*/
short		 Ytestad_module;				/* flag de test de l'adjoint par module */
double	 YLTRes, YAdRes;				/* resultat des tests de l'adjoint par module */
double	 Yerrelmax;		  				/* une erreur relative max */
int			 Ynbko, Ymaxko;					/* nombre de ko et le max autorise */
double	 Ypzedi;							  /* un parametre de zero informatique */
enum     YWardKind Ycurward;    /* type du run courant de trajectoire (pour hidjack) */
int      YNbBasicIntL = 1;      /* nombre de boucle interne a basic_it, modifiable avec set_nb_basic_intern_loop (default=1) */
int      YItBasicIntL;          /* iteration de la boucle interne a basic_it */

/* multi-spatio-tempo : ---------*/

/* variables globales a gerer par changement de contexte ! */
float    Ybegintime = 0.0;      /* temps initial du modele global */
float    Yendtime   = 0.0;      /* temps final du modele global   */
#define  MYMAXREEL	 3.40282347e+38
int      YidTraj;							  /* indice de la trajectoire courante*/
int      YNBUPTIME, YNBALLTIME;
int      YA1, YA2, YA3;					/* taille des axes de l'espace courant */
#define  Yt     YTemps					/* temps courant de la trajectoire courante (toptime) */
int      Ytps;									/* variable pour la gestion du temps entre trajectoire */

// For the parallelization: all these global variables should be privates.
// Yi, Yj, Yk is for the correct run of the computation loops.
// YY is for the OpenMP bug that doesn't allow to call a function in a parallel region and mantain the thread value
// of a global private variable. The value in the called function is not what should be.
// Ytps is for the special case of multi_t so when two modules connected have different trajectory and 
// the should be translated with Ytttt_pdt() function.
//#ifdef YO_PARALLEL
#ifdef _OPENMP
  #pragma omp threadprivate(Yi, Yj, Yk, YY, Ytps)	
#endif							



/* certaines variables generees */
extern struct Yst_modul YTabMod[];

//=============================================================================
//                             OU DOIS-JE METTRE Ygetval ???
//=============================================================================
double Ygetval(char *codop) //creed pour dx random des fonctions de test
{   double val;             //utilised par ...
		if (codop[0]=='R')  /* rand in [-1, 1]  * coef */
		{  //return(((double)rand()/RAND_MAX)*2-1);
		   val = ((double)rand()/RAND_MAX)*2-1;
			 if (strlen(codop)>1)
			 {  //si R est suivi d'autres caracteres, ce doit etre un coef a appliquer
			    //sur le random (ex: si codop = "R0.15" alors val = val * 0.15
			    val = val * atof(codop+1);
			 }
			 return(val);
		}
		else
			 return (0.0);
}
//=============================================================================
void Ycreate_errnew()
{    printf("error on space allocation with new when crteating module; see your YAO administrator\n");
     exit(-9);
}
//=============================================================================



//=============================================================================
//                             TOOL BOX 1
//=============================================================================

//=============================================================================
//                  deux fonctions inline MIN et MAX
// Ymin : renvoie le MIN de 2 valeurs (dans le type de la 1ère)
template<class TypVala, class TypValb>inline TypVala Ymin (TypVala a, TypValb b)
{	return (a<b)? a:b;
}
//-----------------------------------------------------------------------------
// Ymax : renvoie le MAX de 2 valeurs (dans le type de la 1ère)
template<class TypVala, class TypValb> TypVala Ymax (TypVala a, TypValb b)
{	return (a>b)? a:b;
}

//=============================================================================
/* Ypr_tabr: affiche les elements d'un tableau de reels */
template<class TypReal>void Ypr_tabr (char *str, TypReal TabReal[], int nbelt, char *sep)
{	printf("%s", str);
  for(int i=0; i<nbelt;++i) printf("[%i] = % -23.15e%s", i, TabReal[i], sep);
}
//=============================================================================

//=============================================================================
//                    some singles lineare algebra functions
/* Yprosca: produit scalaire (de 2 vecteurs)                                 */
template<class TypReel>double Yprosca (TypReel a[], TypReel b[], int sz)
{	int i;
	double res=0.0, cal;
  //for (i=0; i<sz; ++i) res += a[i]*b[i];
	for (i=0; i<sz; ++i)
	{		cal = a[i]*b[i]; res += cal;
			//if (Ysysdbg) printf("Yprosca: % -23.15e <+= % -23.15e * % -23.15e \n", res, a[i], b[i]);
	}
	return(res);
}

//=============================================================================
//                    some singles statiticals functions
/* Ystat_moy:   moyenne d'un tableau de valeurs                              */
template<class TypVal, class TypRes>void Ystat_moy (TypVal TabVal[], int nbelt, TypRes *moy)
{	*moy=0.0;
	for(int i=0; i<nbelt;++i) *moy += TabVal[i];
	*moy = *moy/nbelt;
}
/* --------------------------------------------------------------------------*/
/* Ystat_mv:   moyenne et variance d'un tableau de valeurs                   */
template<class TypVal, class TypRes>void Ystat_mv (TypVal TabVal[], int nbelt, TypRes *moy, TypRes *var)
{
	/* moyenne */
	Ystat_moy(TabVal, nbelt, moy);

	/* variance */
	*var=0.0;
	for(int i=0; i<nbelt;++i) *var += pow((TabVal[i]-*moy),2);
	*var = *var/nbelt;
}
/* --------------------------------------------------------------------------*/
/* Ystat_norm1: normalisation d'un tableau de valeurs */
template<class TypVal, class TypRes>void Ystat_norm1 (TypVal TabVal[], int nbelt, TypRes moy, TypRes sig)
{
	for(int i=0; i<nbelt;++i) TabVal[i] = (TabVal[i]-moy)/sig;
}
/* --------------------------------------------------------------------------*/
/* Ystat_denorm1: de-normalisation d'un tableau de valeurs */
template<class TypVal, class TypRes>void Ystat_denorm1 (TypVal TabVal[], int nbelt, TypRes moy, TypRes sig)
{
	for(int i=0; i<nbelt;++i) TabVal[i] = TabVal[i]*sig + moy;
}

/*===========================================================================*/
/*===========================================================================*/

//FOR DEBUG
//void Ytrobs (int indic, char *nmmod, int nout, int iaxe, int jaxe, int kaxe, int pdt, YREAL *vobs, YREAL *qtea)
void Ytrobs (int indic, char *nmmod, int nout, int iaxe, int jaxe, int kaxe, int pdt, double *vobs, double *qtea)
{	 printf("%i %s %i %i %i %i t=%i %e %e \n",indic, nmmod, nout, iaxe, jaxe, kaxe, pdt, *vobs, *qtea);
}
//void Ywlobs(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, YREAL *r1, YREAL *r2), int indic);
void Ywlobs(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, double *r1, double *r2), int indic);


//=============================================================================
//                       GENERIC MODUL OBJECT FUNCTION
//=============================================================================
template <class TypVal, class TypClass> int Ysetstate_generic (char *nmmod, TypVal val, TypClass *pc[])
{  		int imod, nbs, axi, axj, axk;
   		imod = Yimod(nmmod);
	 		nbs = YTabMod[imod].nb_stout;
	 		axi = YTabMod[imod].axi;
	 		axj = YTabMod[imod].axj;
	 		axk = YTabMod[imod].axk;

		 //cas non tempo
	    if (axk>0)
			{  //cas 3D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
			   for (int Yw2=0; Yw2<axj; ++Yw2){
			   for (int Yw3=0; Yw3<axk; ++Yw3){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 pc[Yw1*axj*axk+Yw2*axk+Yw3]->Ystate[Yws] = val;}}}}
			}
			else if (axj>0)
			{  //cas 2D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
			   for (int Yw2=0; Yw2<axj; ++Yw2){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 pc[Yw1*axj+Yw2]->Ystate[Yws] = val;}}}
			}
			else
			{  //cas 1D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 pc[Yw1]->Ystate[Yws] = val;}}
			}
     return(1); //?
}
//=============================================================================
template <class TypVal, class TypClass> int Ysetstatet_generic (char *nmmod, TypVal val, TypClass *pc[])
{  		int imod, nbs, axi, axj, axk, axt;
   		imod = Yimod(nmmod);
	 		nbs = YTabMod[imod].nb_stout;
	 		axi = YTabMod[imod].axi;
	 		axj = YTabMod[imod].axj;
	 		axk = YTabMod[imod].axk;
	 		axt = YTabMod[imod].nb_time;

  		//cas tempo
	    if (axk>0)
			{  //cas 3D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
			   for (int Yw2=0; Yw2<axj; ++Yw2){
			   for (int Yw3=0; Yw3<axk; ++Yw3){
		     for (int Ywt=0; Ywt<axt; ++Ywt){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 //pc[Yw1][Yw2][3]->Ystate[Ywt][Yws] = val;}}}}}
				 pc[Yw1*axj*axk+Yw2*axk+Yw3]->Ystate[Ywt][Yws] = val;}}}}}
			}
			else if (axj>0)
			{  //cas 2D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
			   for (int Yw2=0; Yw2<axj; ++Yw2){
		     for (int Ywt=0; Ywt<axt; ++Ywt){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 //pc[Yw1][Yw2]->Ystate[Ywt][Yws] = val;}}}}
				 pc[Yw1*axj+Yw2]->Ystate[Ywt][Yws] = val;}}}}
			}
			else
			{  //cas 1D
			   for (int Yw1=0; Yw1<axi; ++Yw1){
		     for (int Ywt=0; Ywt<axt; ++Ywt){
			   for (int Yws=0; Yws<nbs; ++Yws){
				 pc[Yw1]->Ystate[Ywt][Yws] = val;}}}
			}
      return(1); //?
}
//=============================================================================
//=============================================================================



/*///////////////////////////////////////////////////////////////////////////*/
/*			L'INCLUDE DE L'APPLI GENERATRICE et de son nom                       */
