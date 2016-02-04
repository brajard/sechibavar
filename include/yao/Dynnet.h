
/* ////////////////////////////////////////////////////////////////////////////
//
//		Ynet functions : allowed Yao to deal more or less with neural network
//
//////////////////////////////////////////////////////////////////////////// */


/* ********************************* MACROS ***********************************
//-------------------------------------------------------------------------- */
#define poids(i,j) poids[3*i+j]

double etats[YNMAXCELL];

double gradients[YNMAXCELL];

/* en raison de pbs avec le tableau dynamique input (dans Ynet_backward) */
double input[YNMAXCELL] ;

#ifdef _OPENMP
  #pragma omp threadprivate(etats, input, gradients)
#endif

/* **************************** TYPES PRESONNALISES ***************************
//-------------------------------------------------------------------------- */
//typedef enum {SigLin, SigSig} Activation; /* -> Dynamo.h */

/* ************************* PROTOTYPAGES DES FONCTIONS ***********************
//-------------------------------------------------------------------------- */
/* création d'une matrice des poids a partir d'un fichier SN */
int Ynet_load (const int inet, char *fichier);

/* fonctions utilisateur de propagation */
void Ynet_forward (const int inet, const Activation activ, const double poids[],
		 const int nbconnex, const YREAL etat_entree[], const int nbentree,
		 YREAL etat_sortie[], const int nbsortie);
		
/* fonctions jamais utilisée (donc jamais appellée par le code généré ave le Translator) */
void Ynet_fward1(const int inet, const YREAL etat_entree[],YREAL etat_sortie[]);

void Ynet_fward2(const int inet, YREAL etat_sortie[]);


/* fonction utilisateur de retropropagation */
void Ynet_backward (const int inet, const Activation activ, const double poids[],
		 const int nbconnex,const YREAL grad_sortie[], const int nbsortie,
		 const YREAL etat_entree[],YREAL grad_entree[], const int nbentree);
		
void Ynet_bward1(const int inet, YREAL grad_sortie[],
							   const double etat_entree[],double grad_entree[]);
		
void Ynet_bward2(const int inet, YREAL grad_sortie[]);


/* fonction pour le Lineaire Tangent (avec passe avant) */
void Ynet_flinward (const int inet, Activation activ,
		const double poids[], const int nbconnex,
		const YREAL etat_entree[], const YREAL grad_entree[], const int nbentree,
		YREAL etat_sortie[], YREAL grad_sortie[], const int nbsortie);
		
void Ynet_flward1(const int inet, YREAL etat_sortie[], YREAL grad_sortie[],
                  const YREAL etat_entree[], YREAL grad_entree[]);

void Ynet_flward2(const int inet, YREAL etat_sortie[], YREAL grad_sortie[]);


/* fonctions d'activation employees et leurs derivees */
double Ynet_sig  (const double x); 	/* fonction sigmoide */
double Ynet_dsig (const double x);	/* dérivée d'une fonction sigmoide */
double Ynet_lin  (const double x);	/* fonction lineaire */
double Ynet_dlin (const double x);  /* derivee d'une fonction lineaire */

/* fonction utile: */
/* indice maximum des neurones d'un tableau de poids */
int Ynet_max_tab (const double poids[], const int nbconnex);


/* ************************** VARIABLES GLOBALES ******************************
//-------------------------------------------------------------------------- */
// parametres d'une sigmoide (avec valeurs par defaut)
double Yn_sig_mx 		 = ((double)2)/((double)3); //mx1 ;
double Yn_sig_dmin   = 0 ;
double Yn_sig_scale  = 1/ tanh (((double) 2) / ((double) 3)); /* scale1 ; */
double Yn_sig_offset = 0 ;
/* parametres d'une fonction lineaire (avec valeurs par defaut) */
double Yn_lin_dmin = 1 ;
double Yn_lin_dmax = 1 ;
double Yn_lin_th   = 1 ;


/* ****************************** FONCTIONS ***********************************
//-------------------------------------------------------------------------- */
void Ynet_dispsig ()
{			/* affichage des parametres de la fonction sigmoide */
		printf("sigmoid parameters : \n\tmx    =% -23.15e \n\tdmin  =% -23.15e \n\tscale =% -23.15e \n\toffset=% -23.15e\n",
						Yn_sig_mx, Yn_sig_dmin, Yn_sig_scale, Yn_sig_offset);
}
//-------------------------------------------------------------------------- */
void Ynet_displin ()
{			/* affichage des parametres de la fonction linear */
		printf("linear parameters : \n\tdmin=% -23.15e \n\tdmax=% -23.15e \n\tth  =% -23.15e\n",
						Yn_lin_dmin, Yn_lin_dmax,	Yn_lin_th);
}
//-------------------------------------------------------------------------- */
void Ynet_setpsig (double mx, double dmin, double scale, double offset)
{			/* affectation des parametres de la fonction sigmoide */
		Yn_sig_mx  = mx;
		Yn_sig_dmin = dmin;
		Yn_sig_scale = scale;
		Yn_sig_offset = offset;
}
//-------------------------------------------------------------------------- */
void Ynet_setplin (double dmin, double dmax, double th)
{			/* affectation des parametres de la fonction lineaire */
		Yn_lin_dmin = dmin;
		Yn_lin_dmax = dmax;
		Yn_lin_th   = th;
}

/* ------------------------------------------------------------------------- */
//int Ynet_load (int inet, char *fichier, double *&poids)
int Ynet_load (int inet, char *fichier)
{			/* creation de la matrice des poids
			// in : . indice du reseau
			//		  . nom du fichier contenant les poids (genere par SN)
			// out: . matrice des poids
			// return : nombre de connexions */

	double *&poids = YTabNet[inet].tweight;	/* pour faciliter la programmation */
					
	/* recherche du nombre de connexions */
	int nbconnex;		/* nombre de connexions */
	char cmd[BUFSIZE] = "wc -l ";		/* commande = wc -l fichier.wei */
	strcat(cmd, fichier) ;
	/* appel systeme et recuperation du resultat */
	char buf[BUFSIZE];
	FILE *ptr;
	if ((ptr = popen(cmd, "r")) != NULL)
		fgets(buf, BUFSIZE, ptr);
	/* récuperation du nombre de lignes du fichier */
	sscanf(buf,"%d",&nbconnex) ;	
	nbconnex-- ;		/* nombre de connexions = nombre de lignes - 1 */
		
	poids = new double[nbconnex*3] ;	/* allocation du tableau des poids */
	/*const int long_ligne = 30;	       nb d'elts max par ligne */

	ifstream f;			/* fichie */
	char *ligne ;		/* ligne parsee */
	double val = 0;	/* elt parse */	
	int ind = 0;		/* indice pour parcourir la matrice */
	
	/* allocation de l'espace pour une ligne parsee */
	/*ligne = new char [long_ligne+1];   +1 pour le 0 terminal */
	ligne = new char [BUFSIZE+1] ; /* +1 pour le 0 terminal */

	f.open(fichier,ios::in); /* ouverture du fichier en lecture */
	/* si echec a l'ouverture : message d'erreur */
	if (f.fail()) cout << "erreur à l'ouverture" ;
	
	/*f.getline(ligne,long_ligne,'\n');    parcours de la ligne d'en-tete */
	f.getline(ligne,BUFSIZE,'\n'); /* parcours de la ligne d'en-tete */
	/* on repete */
	/*do {	f.getline(ligne,long_ligne,' ');	  parsage jusqu'a un espace */		
	do {	f.getline(ligne,BUFSIZE,' ');	/* parsage jusqu'a un espace */		
				if (strcmp(ligne,"\0"))	/* si la ligne lue n'est pas un espace */
				{		sscanf(ligne,"%le",&val); /* conversion en double */					
					*(poids + ind++) = val ; /* mise dans la matrice et incrementation de l'indice */
				}
		 }	
	while (!f.eof()); /* jusqu'a la fin du fichier */
	
	delete ligne; 		/* on libère la mémoire */
	
	/* On va chercher le numcell max lors du chargement des poids
  plutot qu'a chaque appel des fct *ward !                */
  int ind_max = Ynet_max_tab(poids, nbconnex); /* indice du dernier neurone de sortie */	
  if (ind_max >= YNMAXCELL)
  {	printf("Probem of size : required %d ; maximum %d\n",ind_max,YNMAXCELL) ;
	  exit(0) ;
  }
  /* et on le stock ainsi que le nombre de connexionx dans le tableau des reseaux */
  YTabNet[inet].maxcell  = ind_max;
  YTabNet[inet].nbweight = nbconnex;
	
	return nbconnex;	/* on retourne le nombre de connexions du réseau */
}

/* ------------------------------------------------------------------------- */
int Ynet_max_tab (const double poids[], const int nbconnex)
{			/* indice maximum des neurones d'un tableau de poids	
			// in : matrice des poids et le nombre de poids
			// return : indice maximal des neurones references dans la matrice des poids */
				
	double ind = poids[1];	/* variable tempo stockant l'indice maxi,
													// initialise au premier indice rencontre */	
	int x = 0; 	/* variables de boucle */
	
	/* parcours de la matrice des poids */
	for (x = 1; x < nbconnex ; x++)		
		if (poids(x,1) > ind)/* si l'indice rencontre est superieur a la variable temporaire */			
			ind = poids(x,1);  /* on met a jour la variable temporaire */
	
	return (int) ind; /* on retourne la valeur */
}

/* ------------------------------------------------------------------------- */
double Ynet_sig (const double x)
{		/* fonction sigmoide
		// in : parametres (en global) de la sigmoide et le point x
		// return : sig(x) = scale * tanh (mx * x)+ offset + dmin * x 					 */

	  return (Yn_sig_scale * tanh (Yn_sig_mx * x) + Yn_sig_offset + Yn_sig_dmin * x);
}

/* ------------------------------------------------------------------------- */
double Ynet_dsig (const double x)
{		/* derivee d'une fonction sigmoide
		// in : parametres (en global) de la sigmoide et le point x
		// return : derivee de sig en x 																				 */

	  return (Yn_sig_scale * Yn_sig_mx * (1 - tanh (Yn_sig_mx * x) *
			      tanh (Yn_sig_mx * x)) + Yn_sig_dmin);
}

/* ------------------------------------------------------------------------- */
double Ynet_lin (const double x)
{		/* fonction lineaire (generalisee)
		// in : parametres de la fonction lineaire generalisee et le point
		// PRE : th >= 0
		// return : lin(x) =
		// 		dmin * x - (dmax - dmin) * th 	si x <= - th
		//		dmax * x			si -th <= x <= th
		//		dmin * x + (dmax - dmin) * th	si x >= th 													 */
			
	double fx; /* variable locale stockant f(x) */
	
	/* calcul de fx en fonction de la valeur de x */
	if (x < -Yn_lin_th)
		fx = Yn_lin_dmin * x - (Yn_lin_dmax - Yn_lin_dmin) * Yn_lin_th ;
	else
		if (x > Yn_lin_th)
			fx = Yn_lin_dmin * x + (Yn_lin_dmax - Yn_lin_dmin) * Yn_lin_th ;
		else
			fx = Yn_lin_dmax * x ;
			
	/* on retourne le resultat */
	return fx ;
}

/* ------------------------------------------------------------------------- */
double Ynet_dlin (const double x)
{		/* derivee d'une fonction linéaire (generalisee)
		// in : parametres (en global) de la fonction lineaire generalisee et le point x
		// return : dérivée de lin en x
		// CONVENTION : pour les points de discontinuité,
		// 		on prend comme derivee la moyenne
		//		des derivees a gauche et a droite 																 */
			
	double dfx = 0;	/* variable locale stockant df(x) */
	
	/* calcul de dfx en fonction de la valeur de x */
	if (x < -Yn_lin_th || x > Yn_lin_th)
		dfx = Yn_lin_dmin ;
	else
		if (x > -Yn_lin_th && x < Yn_lin_th)
			dfx = Yn_lin_dmax ;
		else
			dfx = (Yn_lin_dmax + Yn_lin_dmin) / 2 ;
			
	/* on retourne le resultat */
	return dfx ;
}


/* ------------------------------------------------------------------------- */
void Ynet_fward1(const int inet, const YREAL etat_entree[],YREAL etat_sortie[])
{	Ynet_forward(inet, YTabNet[inet].activ,YTabNet[inet].tweight,YTabNet[inet].nbweight,etat_entree,
							 YTabNet[inet].nbinput,etat_sortie,YTabNet[inet].nboutput);
}
void Ynet_fward2(const int inet, YREAL etat_sortie[])
{	Ynet_forward(inet, YTabNet[inet].activ,YTabNet[inet].tweight,YTabNet[inet].nbweight,Yting,
							 YTabNet[inet].nbinput,etat_sortie,YTabNet[inet].nboutput);
}
/* ------------------------------------------------------------------------- */
void Ynet_forward (const int inet, Activation activ,
		const double poids[], const int nbconnex,
		const YREAL etat_entree[], const int nbentree,
		YREAL etat_sortie[], const int nbsortie)
{		/* fonction de propagation pour l'utilisateur
		// in : indice du reseau, activation (type defini), matrice des poids,
		//	    nb de poids, tableau des etats des entrees, nombre d'entrees,
		//	    nombre de sorties
		// out: tableau des etats des sorties 																	 */

	int ind_max = YTabNet[inet].maxcell; /* indice du dernier neurone de sortie */

	int sortie1 = ind_max - nbsortie + 1;	/* indice du premier neurone de sortie */
	int x = 0 ; /* variable de boucles */
	int pre_crt = -1 ; /* neurone présynaptique courant */
	int pre_new ; /* neurone présynaptique nouveau */

	etats[0] = 1; /* biais = 1 */

	/* insertion des entrees voulues */
	for (x=0 ; x<nbentree ; x++)
		etats[x+1] = etat_entree[x];

	/* initialisation : */
	for (x=nbentree+1 ; x <= ind_max ; x++)
		etats[x] = 0 ;

	/* propagation */
	for (x=0 ; x<nbconnex ; x++)
	{	pre_new = (int)poids(x,0) ;
		if (pre_new != pre_crt)
		{	pre_crt = pre_new ;
			if (pre_crt > nbentree && pre_crt < sortie1)
				etats[pre_crt] = Ynet_sig(etats[pre_crt]) ;
		}
		etats[(int)poids(x,1)] += etats[pre_new] * poids(x,2);
	}

	/* pour chaque neurone de la couche de sortie */
	switch (activ)
	{	case SigSig :
			for (x=sortie1 ; x<=ind_max ; x++)
				etats[x] = Ynet_sig(etats[x]) ;
			break ;
		case SigLin :
			for (x=sortie1 ; x<=ind_max ; x++)
				etats[x] = Ynet_lin(etats[x]) ;
			break ;
	}

	/* for (x=0 ; x <= ind_max ; x++) printf("neurone %d : état %e\n",x,etats[x]) ; */

	/* valeurs de retour */
	for (x = 0 ; x < nbsortie ; x++)
	{	etat_sortie[x] = etats[ind_max-nbsortie+1+x];
		/* printf("sortie %d : ->%e<-\n",x,etat_sortie[x]); */
	}
}

/* ------------------------------------------------------------------------- */
void Ynet_bward1(const int inet, YREAL grad_sortie[], const YREAL etat_entree[], YREAL grad_entree[])
{		 Ynet_backward(inet, YTabNet[inet].activ,YTabNet[inet].tweight,YTabNet[inet].nbweight,
                       grad_sortie, YTabNet[inet].nboutput,etat_entree,grad_entree,YTabNet[inet].nbinput);
}
void Ynet_bward2(const int inet, YREAL grad_sortie[])
{		 Ynet_backward(inet, YTabNet[inet].activ,YTabNet[inet].tweight,YTabNet[inet].nbweight,
                       grad_sortie, YTabNet[inet].nboutput,Yting,Ytbeta,YTabNet[inet].nbinput);
}
/* ------------------------------------------------------------------------- */
void Ynet_backward (const int inet, Activation activ,
		const double poids[], const int nbconnex,
		const YREAL grad_sortie[], const int nbsortie,
		const YREAL etat_entree[],
		YREAL grad_entree[], const int nbentree)
{		/* fonction de retropropagation pour l'utilisateur
		// in : indice du reseau, activation (type défini), matrice des poids,
		//	    nb de poids,tableau des gradients de sortie, nb de sorties,
		//	    tableau des états d'entree, nb d'entrees
		// out: tableau des gradients d'entree 																	 */

	int ind_max = YTabNet[inet].maxcell;  /* indice du dernier neurone de sortie */

	int sortie1 = ind_max - nbsortie + 1;	/* indice du premier neurone de sortie */
	int x = 0 ; /* variable de boucles */
	int pre_crt = -1 ; /* neurone présynaptique courant */
	int pre_new ; /* neurone présynaptique nouveau */

	etats[0] = 1; // biais = 1 */

	/* insertion des entrees voulues */
	for (x=0 ; x<nbentree ; x++)
		  etats[x+1] = etat_entree[x];

	/* initialisation : */
	for (x=nbentree+1 ; x <= ind_max ; x++)
		  etats[x] = 0 ;

	/* propagation */
	for (x=0 ; x<nbconnex ; x++)
	{	  pre_new = (int)poids(x,0) ;
		  if (pre_new != pre_crt)
		  {	 pre_crt = pre_new ;
			   if (pre_crt > nbentree && pre_crt < sortie1)
			   {  input[pre_crt] = etats[pre_crt] ;
			   	  etats[pre_crt] = Ynet_sig(etats[pre_crt]) ;
			   }
		  }
		  etats[(int)poids(x,1)] += etats[pre_new] * poids(x,2) ;
	}

	/* pour chaque neurone de la couche de sortie */
	switch (activ)
	{	case SigSig :
			for (x=sortie1 ; x<=ind_max ; x++)
			{	  input[x] = etats[x] ;
				  etats[x] = Ynet_sig(etats[x]) ;
			}
			break ;
		case SigLin : 
			for (x=sortie1 ; x<=ind_max ; x++)
			{	  input[x] = etats[x] ;
				  etats[x] = Ynet_lin(etats[x]) ;
			}
			break ;
	}
	
	pre_crt = sortie1 - 1 ;

	/* initialisation : */
	for (x=0 ; x < sortie1 ; x++)
		gradients[x] = 0 ;

	/* insertion des gradients de sorties voulues */
	switch (activ)
	{	case SigSig :
			for (x = ind_max ; x >= sortie1 ;  x--)
				  gradients[x] = Ynet_dsig(input[x]) * grad_sortie[x-sortie1];
			break ;
		case SigLin :
			for (x = ind_max ; x >= sortie1 ;  x--)
				  gradients[x] = Ynet_dlin(input[x]) * grad_sortie[x-sortie1] ;
			break ;
	}

	/* répropagation */
	for (x=nbconnex-1 ; x>=0 ; x--)
	{	pre_new = (int)poids(x,0) ;

		if (pre_new != pre_crt)
		{	if (pre_crt > nbentree && pre_crt < sortie1)
				gradients[pre_crt] *= Ynet_dsig(input[pre_crt]) ;
			pre_crt = pre_new ;
		}
		gradients[pre_new] += gradients[(int)poids(x,1)] * poids(x,2) ;
	}

	/* remplissage du poids des gradients d'entree */
	for (x = 0 ; x < nbentree ; x++)
			grad_entree[x] = gradients[x+1];							    //bg+							
			//grad_entree[x] = Ynet_dsig(0)*gradients[x+1];		//bg-

	/* delete[] input;   free(input); */
}

/* ------------------------------------------------------------------------- */
void Ynet_flward1(const int inet, YREAL etat_sortie[], YREAL grad_sortie[],
                 const YREAL etat_entree[], YREAL grad_entree[])
{    Ynet_flinward(inet, YTabNet[inet].activ,
     YTabNet[inet].tweight,YTabNet[inet].nbweight,
	   etat_entree, grad_entree, YTabNet[inet].nbinput,
	   etat_sortie, grad_sortie,  YTabNet[inet].nboutput);

}
void Ynet_flward2(const int inet, YREAL etat_sortie[], YREAL grad_sortie[])
{	   Ynet_flinward(inet, YTabNet[inet].activ,
     YTabNet[inet].tweight,YTabNet[inet].nbweight,
	   Yting, Ytbeta, YTabNet[inet].nbinput,
	   etat_sortie, grad_sortie,  YTabNet[inet].nboutput);
}
/* ------------------------------------------------------------------------- */
void Ynet_flinward (const int inet, Activation activ,
		const double poids[], const int nbconnex,
		const YREAL etat_entree[], const YREAL grad_entree[], const int nbentree,
		YREAL etat_sortie[], YREAL grad_sortie[], const int nbsortie)
{
	int ind_max = YTabNet[inet].maxcell;
	int sortie1 = ind_max - nbsortie + 1;	/* indice du premier neurone de sortie */
	int x = 0 ; /* variable de boucles */
	int pre_crt = -1 ; /* neurone présynaptique courant */
	int pre_new ; /* neurone présynaptique nouveau */

	etats[0]     = 1;  /* biais = 1 */
	gradients[0] = 0;  /* en passe avant, le gradient du biais = 0 !*/
	
	/* insertion des entrees (etats et gradients) */
	for (x=0 ; x<nbentree ; x++)
	{	  etats[x+1] = etat_entree[x];
		  gradients[x+1] = grad_entree[x];							  //bg+
		  //gradients[x+1] = Ynet_dsig(0)*grad_entree[x];	//bg-
	}

	/* initialisation */
	for (x=nbentree+1 ; x <= ind_max ; x++)
	{	  etats[x] = 0; gradients[x] = 0;}

	/* propagation des etats et de leurs derivees sur les couches cachees */
	for (x=0 ; x<nbconnex ; x++)
	{	  pre_new = (int)poids(x,0);
		  if (pre_new != pre_crt)
		  {	 pre_crt = pre_new;
			   if (pre_crt > nbentree && pre_crt < sortie1)
			   {	gradients[pre_crt] *= Ynet_dsig(etats[pre_crt]);
			      etats[pre_crt] = Ynet_sig(etats[pre_crt]);		
			   }
		  }
		  etats[(int)poids(x,1)] += etats[pre_new] * poids(x,2) ;
		  gradients[(int)poids(x,1)] += gradients[pre_new] * poids(x,2);
	}

	/* puis pour chaque neurone de la couche de sortie */
	switch (activ)
	{	case SigSig :
			for (x=sortie1 ; x<=ind_max ; x++)
			{	  gradients[x] *= Ynet_dsig(etats[x]);
			    etats[x] = Ynet_sig(etats[x]);
			}
			break ;
		case SigLin :
			for (x=sortie1 ; x<=ind_max ; x++)
			{	  gradients[x] *= Ynet_dlin(etats[x]);
			    etats[x] = Ynet_lin(etats[x]);
			}
			break ;
	}

	/* remplissage des etats et gradients en sortie */
	for (x=ind_max; x>=sortie1; --x)
	{	  grad_sortie[x-sortie1] = gradients[x];
			etat_sortie[x-sortie1] = etats[x];
  }		
}
/* ========================================================================= */

