/*			DES DEFINES POUR M1QN3 */
/*#define	Y3_SZ_IZ 		5	now this is done by the generator!  =  2n+1 */
#define	Y3_SZ_IZS			3
#define	Y3_SZ_RZS			1
#define	Y3_SZ_DZS			1
//#define	Y3_M			8 /*->Dynamo.h   (ou en variable globale Y3m ?) */

/*Des fonctions pour m1qn3 --------------------------------------------------*/
int 		Y3run(char vers);			/*add first for "chloro" (Frederic) */
void  	Y3valstate_all();			/*add first for "atmos"  (Cedric)   */

/* globales spécif pour m1qn3 */
/*long	Y3indic;							   	 passe a simul ... a revoir peut-etre */
long		Y3n;										/* taille du pb (a alimenter par YSIZEPB a generer par Yao */
float		Y3f;										/* fonction de cout a passer a m1qn3 */
long		Y3nrz;									/* taille de la zone rz */
//float	Y3dxmin;								/* (I)resolution: 2 points distants de moins de dxmin ne sont pas différencies */
float		Y3ddf1;									/* (I)pour calculer df1 */
float		Y3df1;									/* (I)decroissance =  fcost/ddf1 souhaitee a la 1ere iterationxminxminxminxmin */
float		Y3epsg;									/* (IO)critere d'arret: |grad(k)| / |grad1| < epsg */
long		Y3impres;								/* (I)control des impressions (cf art.m1qn3) */
long		Y3io;										/* (I)canal de sortie pour les impressions */
long		Y3mode;									/* (IO)DIS or SIS mode (0,1); cold start or warm restart (0,1) */
long		Y3nsim;									/* (IO)nombre maxi de simulation (cf art.m1qn3) */
long 		Y3iz[Y3_SZ_IZ];					/* (IO)working array */
long		Y3izs[Y3_SZ_IZS];				/* (I)working areas */	
float 	Y3rzs[Y3_SZ_RZS];				/* (I)working areas */
double 	Y3dzs[Y3_SZ_DZS];				/* (I)working areas */
//int		Y3windice;							/*->Dynamo.h: indice de travail pour l'affectation des tableaux Y3x et Y3g */


/*////////////////////////////////////////////////////////////////////////////*/
/* "prototypages" des fonctions pour M1QN3 -----------------------------------*/
/* a laisser dans Dynamo.cpp car les defines utilises ne sont pas connus avant*/


/* les declarations et prototypage sont desormais dans Dynide.h               */
/* euclide/?prosca : 	extern void *scpctl_;*/
/* ctonbe/?ctonb	 : 	extern void *ctlcan_;*/
/* ctcabe/?ctcab	 : 	extern void *canctl_;*/
/*
#ifdef LINUXPC
						// ici les fonctions sont underscorees
extern void *scpctl_;
extern void *ctlcan_;
extern void *canctl_;
extern "C" void m1qn3_ (void f(long *, long *, float [], float *, float [], long [], float [], double []),
                        void **, void **, void **,
                        long *, float [], float *, float [], float *, float *,
                        float *, long *, long *, long *, long *, long *, long [], float [], long *,
                        long [], float [], double []
                        );
#ifdef YO_M2QN1
extern "C" void m2qn1_ (void f(long *, long *, float [], float *, float [], long [], float [], double []),
                        long *, float [], float *, float [], float [], float *, float *,
                        long *, long *, long *, long *, long *, float [], float [],
                        long [], float [], long [], float [], double []
                        );
#endif
#else
						// la elles ne le sont pas
extern void *scpctl;
extern void *ctlcan;
extern void *canctl;
extern "C" void m1qn3 (void f(long *, long *, float [], float *, float [], long [], float [], double []),
                        void **, void **, void **,
                        long *, float [], float *, float [], float *, float *,
                        float *, long *, long *, long *, long *, long *, long [], float [], long *,
                        long [], float [], double []
                        );
#ifdef YO_M2QN1
extern "C" void m2qn1 (void f(long *, long *, float [], float *, float [], long [], float [], double []),
                        long *, float [], float *, float [], float [], float *, float *,
                        long *, long *, long *, long *, long *, float [], float [],
                        long [], float [], long [], float [], double []
                        );
#endif
#endif
*/

/* ============================================================================
// POUR RUNNER AVEC M1QN3 ... a voir a faire (commentaires: ../omean/)
//-------------------------------------------------------------------------- */
//void Y3simul (long *indic, long *n, float x[], float *fcost, float g[],
void Y3simul (int *indic, long *n, float x[], float *fcost, float g[],
				 			long izs[], float rzs[], double dzs[])
{
if      (*indic == 1) printf("simul(indic=%i): DO NOT MODIF F & G ... BUT I DID !?!?!?!\n", *indic);
else if (*indic != 4) printf("simul(indic=%i): WHAT TO DO IN THAT CASE ........ !?!?!?!\n", *indic); 	
		/* etats adjustes par m1qn3 */
		Y3getstate_all(x);			/* affectation pour yao des etats calcules par m1qn3 (x->Yao_state) */
		after_it(YItRun);

		/* iteration suivante */
		++YItRun;
		Ybasic_it();						    /* iteration de base pour avoir f et g */
		*fcost = (float)YTotalCost;	/* affectation pour m1qn3 de la fonction objective calculee par Yao */
		Y3valgrad_all(g);				    /* affectation pour m1qn3 des gradients calcules par Yao */

		if (YY_RUNBRK) *indic=0;
}
/* ------------------------------------------------------------------------- */
int Y3run(char vers)
{
		/* int	 w1; */
		double sv_epsg;											 /* pour sauvegarder et restituer des*/
		long   sv_mode, sv_nbitrun, sv_nsim; /* des parametres modifies par m1qn3*/
		float  dxmin;

		long topend;
		Ytop0 = time((time_t *)NULL);
		printf("\nstart run time: %s\n", ctime(&Ytop0));

		YY_RUNBRK=OFF;	/* pour gere le break d'un run en interactif */

		/* l'adjust et l'after_it sont gered differemment que dans le cas standard */
		
		YAL1Run=RUNL1_M1QN3;	// Run niveau 1 : ' a la maniere d'm1qn3
		YAL2Run=RUNL2_STD;		// Run niveau 2 standard

		/* verif des modes autorised */
		if (vers=='0') // cas m1qn3
		{	if (Y3mode<0 || Y3mode>3) return (0); /* bad mode */			
		}
		if (vers=='2') // cas m2qn1
		{	if (Y3mode<1 || Y3mode>4) return(0); /* bad mode */	
		}
		
		/* ----------------------------------------------------------------- */
		/* sauvegarde de parmametres pour les restituer a la fin */
		sv_epsg=Y3epsg; sv_mode=Y3mode; sv_nbitrun=YNbItRun; sv_nsim=Y3nsim;

		/* ----------------------------------------------------------------- */
		/* des initialisations: */
		/* Y3indic = 1;						?que ce passe t-il si je met aut'chose */
		Y3n 	= YSIZEPB;					/* c'est pour passer a m1qn3 */
		Y3nrz = Y3_SZ_RZ; 				/* cf calcul ^ */

    /* suppression des inits qui semblaient inutiles ... */

		/* affectation de x1 pour m1qn3 (les etats sont supposes avoir prealablement */
		/* ete affectes, avec ysetstate_... par exemple) */
		Y3valstate_all ();		/* Yaostate -> Y3x */

		/* determination de fcost et g[] au point initial x1 (1ere iteration) */
		YItRun=1; //0;
		Ybasic_it();				/* iteration de base pour avoir f et g */
		Y3f = (float) YTotalCost;		/* affectation pour m1qn3 de la fonction objective calculee par Yao */
		Y3valgrad_all(Y3g);	/* affectation pour m1qn3 des gradients calcules par Yao (Yaograd -> Y3g) */
					
		/* ----------------------------------------------------------------- */
		/* reading iz et rz pour un warm restart */
		/* pour le moment, je ne fait pas de warm restart (je me contente du cold) */
		
		/* Y3izs[2]=1; tentative optimal step size for quadratique cost fonction:-> plantage & deboires */

		/* ----------------------------------------------------------------- */
		/* ok pour l'appel de m1qn3 (if YTypeAdjust==ADJUST_M1QN3: plm: ici, y'a que m1qn3 !...*/
		Y3df1 = Y3f/Y3ddf1;
		if (vers=='0')
		{	dxmin = Y3dxmin[0];
			#ifdef LINUXPC
			m1qn3_ (&Y3simul, &scpctl_, &ctlcan_, &canctl_, &Y3n, Y3x, &Y3f, Y3g, &dxmin, &Y3df1,
							&Y3epsg, &Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3iz, Y3rz, &Y3nrz,
							Y3izs, Y3rzs, Y3dzs
					 	 );
			#else
			m1qn3  (&Y3simul, &scpctl, &ctlcan, &canctl, &Y3n, Y3x, &Y3f, Y3g, &dxmin, &Y3df1,
							&Y3epsg, &Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3iz, Y3rz, &Y3nrz,
							Y3izs, Y3rzs, Y3dzs
					 	 );
			#endif
		}
		#ifdef YO_M2QN1
		else if (vers=='2')							/* ... et m2qn1 maintenant if required by O_M1QN3 option */
		{
			#ifdef LINUXPC
			m2qn1_ (&Y3simul, &Y3n, Y3x, &Y3f, Y3g, Y3dxmin, &Y3df1, &Y3epsg,
							&Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3xinf, Y3xsup,
							Y3iz, Y3rz, Y3izs, Y3rzs, Y3dzs
					 	  );
			#else
			m2qn1  (&Y3simul, &Y3n, Y3x, &Y3f, Y3g, Y3dxmin, &Y3df1, &Y3epsg,
							&Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3xinf, Y3xsup,
							Y3iz, Y3rz, Y3izs, Y3rzs, Y3dzs
					 	  );
			#endif
		}
		#endif

		/* ----------------------------------------------------------------- */
		/* printf ("after m1qn3: \n");		so what : nothing ?		
		à la fin, affectation pour yao des etats calcules par m1qn3 (Y3x->Yao_state):
		Y3getstate_all(Y3x);
		Mais en fait, il ne faut pas faire ca, car il semble qu'a la fin, m1qn3 renvoie
		les valeurs initiales (et les gradients finaux) !				
		for (w1=0; w1<YSIZEPB; ++w1)
		{	printf ("state[%i] = %24.12f	\t\t  grad[%i] = %24.12f \n", w1, Y3x[w1], w1, Y3g[w1]);
		}
		*/
		if      (Y3mode==0)
			 		  printf("Yao#>(0) Stop requiered by project %s \n", PRJNAME);	
		else if (Y3mode==1)
			 			printf("Yao#>(1) Stop on epsg (fin de decroissance du gradient) \n");	
		else if (Y3mode==2)
			 			printf("Yao#>(2) Mauvaise initialisation (call inconsistant) \n");	
		else if (Y3mode==3)
		{	 			printf("Yao#>(3) m1qn3:Line-search bloqué sur tmax=10**20:\n");
						printf("Yao#>(3) m2qn2:La matrice quasi-newton devient non definie positive:\n");
		}	
		else if (Y3mode==4)
			 			printf("Yao#>(4) Nombre d'iteration atteint \n");	
		else if (Y3mode==5)
			 			printf("Yao#>(5) Nombre de simulation atteint \n");	
		else if (Y3mode==6)
			 			printf("Yao#>(6) Arret sur dxmin pendant le line-search \n");	
		else if (Y3mode==7)
			 			printf("Yao#>(7) <g,d> > 0  or  <y,s> <0 \n");	
		else
			 			printf("Yao#>Unknown end condition\n");	
    Y3modo = Y3mode; /* mode de sortie de m1qn3 en var glob avant restauration a toute fin utile */
		/* ----------------------------------------------------------------- */
		/* restitution de parmametres modifies par m1qn3 */
		Y3epsg=sv_epsg; Y3mode=sv_mode; YNbItRun=sv_nbitrun; Y3nsim=sv_nsim;		

		/* ------------------------------ FIN ------------------------------ */
		topend = time((time_t *)NULL);
		printf("\nstart run time ....: %s", ctime(&Ytop0));
	  printf("  end run time ....: %s", ctime(&topend));
		printf("  run duration ....: %lds\n\n", topend-Ytop0);

		YY_RUNBRK=ON;			
		return(1);
}

/* ============================================================================
//                      IMPLEMENTATION POUR L'ALGO L'INCREMENTAL             */
#ifdef YO_VARINCR
//-------------------------------------------------------------------------------
//void Yc3_simul (long *indic, long *n, float x[], float *fcost, float g[],
void Yc3_simul (int *indic, long *n, float x[], float *fcost, float g[],
				 			  long izs[], float rzs[], double dzs[])
{    //int witraj;
if      (*indic == 1) printf("simul(indic=%i): DO NOT MODIF F & G ... BUT I DID !?!?!?!\n", *indic);
else if (*indic != 4) printf("simul(indic=%i): WHAT TO DO IN THAT CASE ........ !?!?!?!\n", *indic); 		
		/* variables adjustees par m1qn3 */
		Y3getdelta_all(x);			/* affectation pour yao des valeurs calculees par m1qn3 (x->Yao_delta) */
		after_it(YItRun);

		/* iteration suivante */
		++YItRun;
		Yrazgrad_all();	
		
    //for (witraj=0; witraj<YNBTRAJ; ++witraj)
    //    YgradEQPdelta_traj(witraj, "Y#T", 0, YTabTraj[witraj].nbuptime, 1.0);   //YGo = YDo
    YgradEQPdelta_target(1.0);		//YGo = YDo		//%1

		printf(" BI:(+)");					/* n° boucle interne non gered: meme valeur que la variable simul affiched par m1qn3*/
		Ybasic_it();						    /* iteration de base pour avoir f et g */
		*fcost = (float)YTotalCost;	/* affectation pour m1qn3 de la fonction objective calculee par Yao */
		Y3valgrad_all(g);				    /* affectation pour m1qn3 des gradients calcules par Yao */

		if (YY_RUNBRK) *indic=0;
}
//-------------------------------------------------------------------------------
int Yc3_run(char vers)		//runim void Yc3_run(long Yit) avec m1qn3
{
    //int    witraj; //%1
		double sv_epsg;                      /* pour sauvegarder et restituer des*/
		long   sv_nbitrun, sv_mode, sv_nsim; /* des parametres modifies par m1qn3*/
		long   topend;
    int    extl;
    float  dxmin;

		#ifdef YO_M2QN1
		float dxinf[YSIZEPB];
		float dxsup[YSIZEPB];
		#endif
	
    //A) DEBUT ie INITIALISATIONS GENERALES	
		Ytop0 = time((time_t *)NULL);
		printf("\nstart run time: %s\n", ctime(&Ytop0));

		YY_RUNBRK=OFF;        /* pour gere le break d'un run en interactif */
		/* YTypeMode=MODE_M1QN3;  l'adjust et l'after_it sont gered differemment que dans le cas standard */
		YAL1Run= RUNL1_M1QN3; /* algo de run niveau 2 : M1QN3 */		
		YItRun = 1;			      /* init increment des iteration */

		/* verif des modes autorised */
		if (vers=='0') // cas m1qn3
		{	if (Y3mode!=0 && Y3mode!=1 && Y3mode!=2 && Y3mode!=3 && Y3mode!=20 && Y3mode!=31)
				 return (0); /* bad mode */
		}
		if (vers=='2') // cas m2qn1
		{	if (Y3mode>3 || Y3mode <-43) return(0);  /* bad mode */	
		  if (Y3mode<1 && Y3mode >-41) return (0); /* bad mode */	
				//nb1: les mode -41 a -43 sont des syclop mode
				//nb2: plm, je considere que 4 fait partie des modes interdits car
				//     il semble que m2qn1 peut provoquer un plantage dans ce cas
				//     quand bien meme je l'ai patcher pour l'eviter mais sans
				//     aucune garantie de coherence !
		}
		
		/* ----------------------------------------------------------------- */
		/* sauvegarde de parmametres pour les restituer a la fin */
		sv_epsg=Y3epsg; sv_nbitrun=YNbItRun; sv_nsim=Y3nsim; sv_mode=Y3mode;

    /* est-ce cold au debut !? */
    if (vers=='0')	/* cas m1qn3 */
    {	//if (Y3mode==2) Y3mode=0; else if (Y3mode==3) Y3mode=1;
    	if      (Y3mode==20) Y3mode=0;
    	else if (Y3mode==31) Y3mode=1;
    }
    #ifdef YO_M2QN1
		else if (vers=='2')	/* cas m2qn1 */
    {	//if (Y3mode==-4) Y3mode=1;
    	if      (Y3mode==-41) Y3mode=1;
    	else if (Y3mode==-42) Y3mode=2;
    	else if (Y3mode==-43) Y3mode=3;
    }
    #endif
		
		/* ----------------------------------------------------------------- */
		//1) On suppose X(k=0) initialised par l'utilisateur (setstate ou xuserfct)
    //2) On suppose que les obs (y°) et les ebauches ebx (Xb) ont ete charges (comme Nal et d'hab)
    //   avec loadobs, outoobs, (loadstate) outoebx
    //3) On suppose que l'utilisateur a positionned avec set_pcoef et pour chaque target module,
    //   les %ages qui serviront a initialiser les dx (avec la fct Ysetpdelta) dans la boucle externe
		/* ----------------------------------------------------------------- */
		/* des initialisations: */
		/* Y3indic = 1;				      ?que ce passe t-il si je met aut'chose */
		Y3n 	= YSIZEPB;			   /* c'est pour passer a m1qn3 */
		Y3nrz = Y3_SZ_RZ; 		   /* cf calcul ^ */

    /* suppression des inits qui semblaient inutiles ... */

    //B) BOUCLE EXTERNE =====================
    for (extl=1; extl<=YNbExtL; ++extl)
    {   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ extern loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf(" BE:%3i", extl);		
				
				/* A) MD (Modele Direct):-> M(Xk) : states : [X] */					
				//Yset_modeltime(0); before_it(1); Yforward (-1, 0);
				YAL2Run=RUNL2_STD; /* run de niveau 2 standard pour forwardcost*/
				Yobs_reverse_time_lists();	// inversion de l'ordre du temps (descending->ascending)
				Yforwardcost();						  // passe avant avec calcul du cout sur la vraie fonction	
				Yobs_reverse_time_lists();	// re-inversion de l'ordre du temps (ascending->descending)
				YAL2Run=RUNL2_INCR; /* mais run de niveau 2 incremental pour l'incremental */
				
		    /* apres ces manips, on assure en re-initialisant le temps */
		    Yset_modeltime(0);

				//B) SET de dx : ca depend de X et pcoef
        //for (witraj=0; witraj<YNBTRAJ; ++witraj)
        //    YdeltaEQPCstate_traj(witraj, "Y#T", 0, YTabTraj[witraj].nbuptime);
        YdeltaEQPCstate_target(); //%1

				//B+) set pour dx des bornes inf et sup dans le cas de m2qn1
				#ifdef YO_M2QN1
				if (vers=='2')
				{  Y3valstate_all (); //Yaostate -> Y3x
				   for (long wi=0; wi<YSIZEPB;++wi)
				   {   dxinf[wi] = Y3xinf[wi]-Y3x[wi];
						   dxsup[wi] = Y3xsup[wi]-Y3x[wi];
						   //printf("!!!!!!! %ld : dxinf=%24.12f < dx=000 < dxsup=%24.12f  :: ddxmin=%24.12f \n ",wi,dxinf[wi],dxsup[wi],Y3dxmin[wi]);
					 }
				}
				#endif
						
			  //C) BOUCLE INTERNE ==================== : is done by m1qn3
			  
				/* affectation de x1=dx1 pour m1qn3 (les delta sont supposes avoir prealablement ete calcules */
				Y3valdelta_all (); //Yaodelta -> Y3x

				/* determination de fcost et g[] au point initial dx1 (1ere iteration) */
				Yrazgrad_all();	
        //for (witraj=0; witraj<YNBTRAJ; ++witraj)
        //    YgradEQPdelta_traj(witraj, "Y#T", 0, YTabTraj[witraj].nbuptime, 1.0);
        YgradEQPdelta_target(1.0); //%1           
								
				printf(" BI:(+)");	/* n° boucle interne non gered: meme valeur que la variable simul affiched par m1qn3*/
				Ybasic_it();	/* lineaire iteration de base pour avoir f et g */
		
				Y3f = (float) YTotalCost;		/* affectation pour m1qn3 de la fonction objective calculee par Yao */
				Y3valgrad_all(Y3g);	/* affectation pour m1qn3 des gradients calcules par Yao (Yaograd -> Y3g) */

				/* ----------------------------------------------------------------- */
				/* reading iz et rz pour un warm restart */
				/* pour le moment, je ne fait pas de warm restart (je me contente du cold) */

				/* Y3izs[2]=1; tentative optimal step size for quadratique cost fonction:-> plantage & deboires */
					
				/* ----------------------------------------------------------------- */		
				/* ok pour l'appel de m1qn3 (if YTypeAdjust==ADJUST_M1QN3: plm: ici, y'a que m1qn3 !... */
				Y3df1 = Y3f/Y3ddf1;
				if (vers=='0')
				{	dxmin = Y3dxmin[0];
					#ifdef LINUXPC
					m1qn3_ (&Yc3_simul, &scpctl_, &ctlcan_, &canctl_, &Y3n, Y3x, &Y3f, Y3g, &dxmin, &Y3df1,
									&Y3epsg, &Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3iz, Y3rz, &Y3nrz,
									Y3izs, Y3rzs, Y3dzs
							 	 );
					#else
					m1qn3 (&Yc3_simul, &scpctl, &ctlcan, &canctl, &Y3n, Y3x, &Y3f, Y3g, &dxmin, &Y3df1,
									&Y3epsg, &Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, Y3iz, Y3rz, &Y3nrz,
									Y3izs, Y3rzs, Y3dzs
							 	 );
					#endif
				}
		    #ifdef YO_M2QN1
				else if (vers=='2')								/* ... et m2qn1 maintenant if required by O_M1QN3 option*/
				{
					#ifdef LINUXPC
					m2qn1_ (&Yc3_simul, &Y3n, Y3x, &Y3f, Y3g, Y3dxmin, &Y3df1, &Y3epsg,
									&Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, dxinf, dxsup,
									Y3iz, Y3rz, Y3izs, Y3rzs, Y3dzs
					 	  		);
					#else
					m2qn1  (&Yc3_simul, &Y3n, Y3x, &Y3f, Y3g, Y3dxmin, &Y3df1, &Y3epsg,
									&Y3impres, &Y3io, &Y3mode, &YNbItRun, &Y3nsim, dxinf, dxsup,
									Y3iz, Y3rz, Y3izs, Y3rzs, Y3dzs
					 	  		);
					#endif
				}
				#endif

				/* ----------------------------------------------------------------- */
				/* printf ("after m1qn3: \n");		so what : nothing ?
				a la fin, affectation pour yao des etats calcules par m1qn3 (Y3x->Yao_state):
				Y3getstate_all(Y3x);
				Mais en fait, il ne faut pas faire ca, car il semble qu'a la fin, m1qn3 renvoie
				les valeurs initiales (et les gradients finaux) !
				for (w1=0; w1<YSIZEPB; ++w1)
				{	printf ("state[%i] = %24.12f	\t\t  grad[%i] = %24.12f \n", w1, Y3x[w1], w1, Y3g[w1]);
				}
				*/
				if      (Y3mode==0)
					 		  printf("Yao#>(0) Stop requiered by project %s \n\n", PRJNAME);
				else if (Y3mode==1)
					 			printf("Yao#>(1) Stop on epsg (fin de decroissance du gradient) \n\n");
				else if (Y3mode==2)
					 			printf("Yao#>(2) Mauvaise initialisation (call inconsistant) \n\n");
				else if (Y3mode==3)
				{	 			printf("Yao#>(3) m1qn3:Line-search bloqué sur tmax=10**20:\n");
								printf("Yao#>(3) m2qn2:La matrice quasi-newton devient non definie positive:\n");
				}	
				else if (Y3mode==4)
					 			printf("Yao#>(4) Nombre d'iteration atteint \n\n");
				else if (Y3mode==5)
					 			printf("Yao#>(5) Nombre de simulation atteint \n\n");
				else if (Y3mode==6)
					 			printf("Yao#>(6) Arret sur dxmin pendant le line-search \n\n");
				else if (Y3mode==7)
					 			printf("Yao#>(7)  <g,d> > 0  or  <y,s> <0 \n\n");
				else
					 			printf("Yao#>Unknown end condition\n\n");
        Y3modo = Y3mode; /* mode de sortie de m1qn3 en var glob avant restauration a toute fin utile */
				/* ----------------------------------------------------------------- */
				//fin boucle INTERNE .............................
				
				//suite boucle EXTERNE ...........................
				//D) Xk+1 = Xk + dx
				Yc_adjustk_all ();

				if (YY_RUNBRK) break;
		
				/* restitution de parmametres modifies par m1qn3 y compris forcage warm
		   		 start ou pas, selon la demande de l'utilisateur (Y3mode=sv_mode) */
				Y3epsg=sv_epsg; YNbItRun=sv_nbitrun; Y3nsim=sv_nsim;
				if      (sv_mode==20)  Y3mode=2;	// m1qn3 incremental : Dis/Cold->Dis/Warm				
				else if (sv_mode==31)  Y3mode=3;	// m1qn3 incremental : Sis/Cold->Sis/Warm				
				else if (sv_mode<=-41) Y3mode=4;	// syclop mode pour m2qn1 incremental : 1,2,3 -> Warm
				else     Y3mode = sv_mode;					
				
 		}//fin boucle EXTERNE ...............................
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ end extern loop ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");


		//D) FIN =============================================
 		
		/* cout sur la vraie fonction */
		printf(" BE:%3i", extl-1);
		YAL2Run=RUNL2_STD; /* run de niveau 2 standard pour forwardcost*/
		Yobs_reverse_time_lists();	// inversion de l'ordre du temps (descending->ascending)
		Yforwardcost();						// passe avant avec calcul du cout	
		Yobs_reverse_time_lists();	// re-inversion de l'ordre du temps (ascending->descending)
		
		/* restitution de parmametres modifies par m1qn3...(EXTERNE ou FIN ?) cf^ */

		topend = time((time_t *)NULL);
		printf("\nstart run time ....: %s", ctime(&Ytop0));
	  printf("  end run time ....: %s", ctime(&topend));
		printf("  run duration ....: %lds\n\n", topend-Ytop0);

		YY_RUNBRK=ON;
		return(1);
}

// fin du code specifique pour l'INCREMENTAL (si O_VARINCR)
#endif
/* ==========================================================================*/
