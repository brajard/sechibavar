/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
//char Ydxalea[STRSIZE20+1] = "R0.0000001"; pour un dx (perturbation de Xo) aleatoire ...
char Ydxalea[STRSIZE20+1] = "\0"; // mais initialement (par defaut) , cet alea est neutralised
/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
void Ytestad_mod(double pdx, double errelmax, int maxko, double pzedi)
{		 /* PURPOSE : test de l'ADjoint par module : <m'(Xo)x,y> = <x, m*(Xo).y>
		    pdx :	Pourcentage De perturbation de Xo pour obtenir dx
		    errelmax : Erreur relative max : on test tous les modules en chaque point
		    et maxko : de coordonnee, et on considere que le test est ko si l'erreur
                   relative est > a errelmax. Si on trouve plus de maxko tests ko,
                   le process est interrompu (exit).
			  pzedi    : Parametres pour se donner un niveau de zero informatique
 				           lorsque les 2 |termes| sont < pzedi alors -> pas de test => OK !?

        nb: maintenir en meme temps la fonction testad (s'il y a lieu)

        called fct : YgradEQPstate_all, YdeltaEQPstate_all,YgradTOtab_all, YtabTOgrad_all
                     before_it, Yforward, Ylinward, Yrazgrad_all, Ybackward
		 */

    int   witraj;
    int   NBALLTIME;
		YREAL *MXdx = new YREAL[YSIZECO];		/* tableau de sauvegarde */
		if (MXdx==NULL){printf("Yao=>ABORT: pb on new MXdx for testad (see Yao system administrator)\n"); exit(-9);}
	  memset(MXdx, 0, YSIZECO*sizeof(YREAL));
		Ytop0 = time((time_t *)NULL);

		/* 0) Presentation */
printf("\ntestad : NEW : use of new function set_dxalea (%s)\n", Ydxalea);
		printf ("\n module Adjoint TEST :        dx = Xo * pdx  (with pdx = %e)", pdx);
		printf ("\n --------------------    and  dy = m'(Xo).dx\n\n");

		/* nb: On suppose Xo initialised par l'utilisateur (setstate ou xuserfct -> YSo) */

		/* 1) some init */
		Ytestad_module=1;
		Yerrelmax=errelmax;
		Ynbko=0; Ymaxko=maxko;
		Ypzedi = pzedi;

    YgradEQPOstate_target(pdx, Ydxalea); //Y'avait tout un baratin ici,
    YdeltaEQPgrad_target(1.0);           //il faut le refaire ... ?

		/* 2) MD (Modele Direct): Calculer M(Xo) */
		Yset_modeltime(0);
		before_it(1);				     //(YItRun);
		Yforward (-1, 0);		     //MD : M(Xo) -> YS
    Yact_operator('*', ON);
    Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
    for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		    YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
    }
    Yforward_operator('R');		//et on termine par les
    Yforward_operator('K'); 	//    operateurs R et K

		/* 3) LT (Lineaire Tangent): m'(Xo)dXo */
		Yset_modeltime(0);
		before_it(1);				     //(YItRun);
		Ylinward(0);				     // TL : M'(Xo)dXo -> YG
    Ylinward_operator('*');  //+linward des operateurs a la fin

		/* 4) YGn-> tableau Mxdx: avant RAZ, sauvegarde des dx obtenus sur les modules cout en fin de traj */
    Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);			/* YGn -> [MXdx] */
    }

		/* 5) RAZ tous les gradients pour l'Adjoint */
		Yrazgrad_all();		/* YG = 0 */

		/* 6) et Reprise des dx de fin de trajectoire des modules cout (qui vont etre retro-propagees) */
    Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YtabTOgrad_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);				/* [MXdx] -> YGn */
    }

		/* 7) on se lance dans l'Adjoint */
    Ybackward_operator('*'); //d'abord backward des operateurs en fin de trajectoire
		Ybackward(-1, 0);        //au cours duquel le test sur chaque module se fait (YNBSTEPTIME);
		//quid after_it ?

		printf(" -->-->--> %i case(s) found \n\n", Ynbko);
		/* 8) FIN */
		/*    on libere la memoire		 */
		delete[] MXdx;
		/*    et on laise le temps propre en ordre */
		Yset_modeltime(0);

		/* Restaurer (~) l'etat init ? En principe il n'est pas modified !?
		for (witraj=0; witraj<YNBTRAJ; ++witraj)
        YstateEQPdelta_traj(witraj, "Y#A", 0, YTabTraj[witraj].nbuptime, 1/pdx);
		*/
		Ytestad_module=0;
}

/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
void Ytestad(double pdx)
{		 /* PURPOSE : test de l'ADjoint : <M'(Xo)x,y> = <x, M*(Xo).y>
					ici, dx correspond a x, et dx* a y
					dans Yao la zone utilised pour Xo est YS, et YG est utilised pour dx lors
					du TL (passe avant) puis pour dx* lors de l'Adjoint (passe arriere) d'ou
					la necessite de passer par des tableaux pour sauvegarder et manipuler les
					resultats des calculs.
		    pdx :	Pourcentage De perturbation de Xo pour obtenir dx
		    errelmax : Erreur relative max : si cette valeur est differente de 0 alors
		    et maxko : on test tous les modules en chaque point de coordonnee, et on
		               considere que le test est ko si l'erreur relative est > a errelmax.
		               Si on trouve plus de maxko tests ko, le process est interrompu (exit).

				nb: maintenir en meme temps la fonction testad_mod (s'il y a lieu)

				called fct : YgradEQPstate_all, YgradTOtab_all, YtabTOgrad_all
									   Yrazgrad_all, before_it, Yforward, Ylinward, Ybackward
		 */

		int   witraj;
		int   NBALLTIME;
		double LTRes=0.0, AdRes=0.0;
		YREAL *DX   = new YREAL[YSIZEPB];		/* tableau de sauvegarde de dx */
		if (DX==NULL)  {printf("Yao=>ABORT: pb on new DX for testad (see Yao system administrator)\n"); exit(-9);}
		YREAL *MXdx = new YREAL[YSIZECO];		/* tableau resultat du Lineaire Tangent M'(Xo)dX */
		if (MXdx==NULL){printf("Yao=>ABORT: pb on new MXdx for testad (see Yao system administrator)\n"); exit(-9);}
		YREAL *MAdx = new YREAL[YSIZEPB];		/* tableau resultat de l'Adjoint M*(Xo)d*X */
		if (MAdx==NULL){printf("Yao=>ABORT: pb on new MAdx for testad (see Yao system administrator)\n"); exit(-9);}

	  memset(DX,   0, YSIZEPB*sizeof(YREAL));
	  memset(MXdx, 0, YSIZECO*sizeof(YREAL));
	  memset(MAdx, 0, YSIZEPB*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		/* 0) Presentation */
printf("\ntestad : NEW : use of new function set_dxalea (%s)\n", Ydxalea);
		printf ("\n Adjoint TEST :         dx = Xo * pdx  (with pdx = %e)", pdx);
		printf ("\n --------------    and  dy = M'(Xo).dx\n\n");

		Ytestad_module=0;		/* because c'est la meme instruction qui sert dans les 2 cas, il faut pouvoir distinguer */

		/* nb: On suppose Xo initialised par l'utilisateur (setstate ou xuserfct -> YSo) */

		/* 1) Init de dXo -> YGo */
		Yrazgrad_all();

    /* dXo = Xo * ~pdx (([1->UPTIME]:)YGo_mod=YSo_mod*pdx) */
    YgradEQPOstate_target(pdx, Ydxalea); //calcul de dx avec un alea ...

		/* 2) MD (Modele Direct): Calculer M(Xo) */
		Yset_modeltime(0);
		before_it(1);				//(YitRun);
		Yforward(-1, 0);		//MD : M(Xo) -> YS
    Yact_operator('*', ON);
    Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
    for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		    YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
    }
    Yforward_operator('R');  //et on termine par les
    Yforward_operator('K');  //    operateurs R et K

		/* 3) LT (Lineaire Tangent): M'(Xo)dXo */
		Yset_modeltime(0);
		before_it(1);				//(YitRun);
		Ylinward(0);				//TL : M'(Xo)dXo -> YG
    Ylinward_operator('*'); //linward opera la fin !!?? (car on ne s'interesse qu'aux modules cout en fin de traj)

		/* 4) YGn-> tableau Mxdx et calcul de < M'(Xo)dX, dy > (sur module cout en fin de traj) */
		Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);					/* YGn -> [MXdx] */
    }

		for(int i=0; i<YSIZECO; ++i)
		{LTRes += MXdx[i]*MXdx[i];	/* < M'(Xo)dX, dy > */
		}

		/* 6) au passage, on a peut etre aussi besoin de conserver donc de sauvegarder YGo (isnt-it?) */
		Y3windice=0;
    YgradTOtab_target(DX);			/* YGo -> [DX]  */

		/* 7) RAZ tous les gradients pour l'Adjoint */
		Yrazgrad_all();		/* YG = 0 */

		/* 8) et Reprise des gradients de fin de trajectoire des modules cout */
		Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YtabTOgrad_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);				/* [MXdx] -> YGn */
    }
//
		/* 9) on devrait pouvoir se lancer dans l'adjoint */
    Ybackward_operator('*'); //d'abord backward des operateurs en fin de trajectoire
		Ybackward(-1, 0);  /* AD (adjoint):-> dx =M*(Xo)d*X : Yjac * YG -> Ytbeta */
		//quid after_it ?

		/* 10) YGo-> tableau MAdx et calcul de < dx, M*(Xo).dy > */
		Y3windice=0;
    YgradTOtab_target(MAdx);		 /* YGo -> [MAdx] */

		for(int i=0; i<YSIZEPB; ++i)
		{	AdRes += DX[i] * MAdx[i];									/* < dx, M*(Xo).dy > */
		}

		/* 11) Afficher le resultat */
		printf (" < M'(Xo).dx, dy >  = %- 16.15e \n", LTRes);
		printf (" < dx, M*(Xo).dy >  = %- 16.15e \n", AdRes);
		printf ("                      ----------------------\n");
		printf ("   ecart :            %- 16.15e \t", LTRes-AdRes);
		printf (" relative error = %e \n\n", (LTRes-AdRes)/LTRes);


		/* 12) on informe d'une possible cause d'echec par exemple si des obs sont */
		if (YioInsertObsCtr != -1) /* charger dans une des fonction d'intervention */
		{	 printf("testad warning: obs must not be used for global Adjoint test\n");
		   	   //because: ca modifie le dy au cours de la redescente qui ne sera plus le meme
		   	   //         que celui utilised lors du calcul de LTRes.
		   	   //         Cette contrainte n'est pas utile pour testad_mod car le test est local.
		}

		/* 13) FIN */
		/*    on libere la memoire		 */
		delete[] DX;  delete[] MXdx;  delete[] MAdx;
		/*    et on laise le temps propre en ordre */
		Yset_modeltime(0);
}

/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
void Ytestlt(double pdx, double alpha, double fdec, int nbloop, int modop, int syclop)
{						//PURPOSE : test du Lineaire Tangent :
						// lorsque dx~>0:  M(Xo+dx) - M(Xo) ~> M'(Xo)dx		    (Taylor)
						//								 M(Xo+dx) - M(Xo) -  M'(Xo)dx ~> 0
						// 2eme maniere equivalente, mais (+)difficile a implementer a l'ordre 2 (!) :
						//				   [ (M(Xo+dXo) - M(Xo)) / M'(Xo)dx ] ~> 1
						//
		//pdx   : Pourcentage De perturbation de Xo pour obtenir dx
		//alpha : parametre de tendance
		//fdec  : facteur de decroissance de alpha (fdec=#10)
		//nbloop: nombre de boucles de decroissance
		//modop : mode operatoire: si 0: ->0, sinon ->1
		//syclop: Secret Yao Systeme Code Operation (0: algo officiel
		//        1: algo (initial) theoriquement equivalent a l'algo officiel mais plus lent
		//			  2: algo (initial) bugged (i.e sans re-init de Xo ...) mais qui parfois "marche mieux" !!??
		
		int     witraj;
		int     NBALLTIME;
		double	apdx, svalpha;			//...
		double 	*TrendO1, *TrendO2;	//Tableau des rms-tendances par module
		int			indT;								//indice des TrendO tableaux
		int 		n		   = 0; 				//indice (supposed) correspondant dans les tableaux de sauvegarde
		int 		noco   = 0;					//numero d'ordre des cout modul (parmi les modules cout)
		int 		sizeco; 						// taille atempo d'un module cout
		double	galphaO1=0.0;				//...
		double	galphaO2=0.0;				//...	
		double	sigO1, sigO2; 			//...
		int			nbnanf;						  //nombre de NaN ou Inf found pour le LT si = 0 (pour la 2eme maniere) ..
		int     *Tnbnanf;           //Tableau des nombres de nanf par module
		int			isnanf;							//booleen pour tester que les resultats de calcul sont finite (ie: !Nan && !Inf)
		
		// Tableaux de staokage des valeurs et resultats de calcul ...
		YREAL *MX = new YREAL[YSIZECO];
		if (MX==NULL){printf("Yao=>ABORT: pb on new MX for testlt (see Yao system administrator)\n"); exit(-9);}
		YREAL *TLX = new YREAL[YSIZECO];
		if (TLX==NULL){printf("Yao=>ABORT: pb on new TLX for testlt (see Yao system administrator)\n"); exit(-9);}
		YREAL *MXdx = new YREAL[YSIZECO];
		if (MXdx==NULL){printf("Yao=>ABORT: pb on new MXdx for testlt (see Yao system administrator)\n"); exit(-9);}
		YREAL *MpXdx = new YREAL[YSIZECO];
		if (MpXdx==NULL){printf("Yao=>ABORT: pb on new MpXdx for testlt (see Yao system administrator)\n"); exit(-9);}

	  memset(MX,   0, YSIZECO*sizeof(YREAL));
	  memset(TLX,  0, YSIZECO*sizeof(YREAL));
	  memset(MXdx, 0, YSIZECO*sizeof(YREAL));
	  memset(MpXdx,  0, YSIZECO*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		/* A) INITIALISATION */
		/*   0) presentation : */
		printf ("\n Lineaire Tangent TEST : with: pdx = %e, (a)lpha = %e", pdx, alpha);
		printf ("\n -----------------------       fdec= %e, (n)bloop= %i\n", fdec, nbloop);
		printf (" nb 1) dx = Xo * pdx \n");
		printf ("    2) a(n) = a(n-1) / fdec \n");
		
		/*   1) determination du nombre de module cout : */
		int	nbcout=0;
		for (int w1=0; w1<YNBMODUL; ++w1) nbcout += YTabMod[w1].is_cout;
		
		/* allocation des tableaux de tendance pour chaque module */
		TrendO1 = new double[nbcout*nbloop];
		TrendO2 = new double[nbcout*nbloop];		
		Tnbnanf = new int[nbcout*nbloop];		
				
		/*   2) On suppose Xo initialised par l'utilisateur (setstate ou xuserfct -> YS) */
 		//for (witraj=0; witraj<YNBTRAJ; ++witraj)	/* sauvegarde Xo (i.e: YSo) dans YDo: (([1->UPTIME]:)YDo_mod=YSo_mod) */	
    //    YdeltaEQPstate_traj(witraj, "Y#T", 0,  YTabTraj[witraj].nbuptime, 1);
    YdeltaEQPstate_target(1);
		
		/*   3) sauvegarde du parametre de tendance */
		svalpha = alpha;  				
		
		/*   4) Calcul de M(Xo) et sauvegarde */
		Yset_modeltime(0);
		before_it(1);      //... (YItRun);
		Yforward (-1, 0);	 //MD : M(Xo) -> YS
    Yact_operator('*', ON);
    Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
    for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		    YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
    }
    Yforward_operator('R');  //et on termine par les
    Yforward_operator('K');  //    operateurs R et K

		Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)																	//YSn -> MX	(nb: que module cout
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;	// en fin de trajectoire, qu'il y
        YstateTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MX); 				// ait ou pas d'observations)			
    }

		/*   5) Calcul de M'(Xo)dx une seule fois et sauvegarde : (algo officiel) */
		if (syclop==0)
		{  Yrazgrad_all();//RAZ de YG : YG = 0
		   // on va mettre dx comme petite perturbation a propager 1 fois par le LT
       YgradEQPstate_target(pdx);
		
		   // propagation par le LT
		   Yset_modeltime(0);
		   before_it(1);            //... (YItRun);
		   Ylinward(0);		          //M'(Xo)dX -> YG
       Ylinward_operator('*');  //linward des operateurs en fin de trajectoire

			 Y3windice=0;
       for (witraj=0; witraj<YNBTRAJ; ++witraj)
       {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
           YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MpXdx);		//sauvegarde: YGn -> [MpXdx]			
       }
		}

		/* B) BOUCLE DE CALCUL DES TENDANCES PAR MODULE */
		for (int iloop=0; iloop<nbloop; ++iloop)		//^: nbloop : nombre de boucle
		{		printf("o");fflush(stdout); //evolution loop
				
				/* 1) evolution des p* */
				alpha = alpha / fdec;				//decroissance de alpha
				apdx  = pdx * alpha;

				/* 2) LT (Lineaire Tangent): M'(Xo)a.dXo */
				if (syclop==0) //algo officiel : cette boucle suffit pour avoir M'(Xo)a.dXo -> [TLX]
				{  for(int wi=0; wi<YSIZECO; ++wi) TLX[wi] = MpXdx[wi]*alpha;
				}								
				if (syclop>0)  //algo non officiel (plus lent, eventuellement bugged)
				{	 if (syclop==1) // avec cet algo,  il faut retablir YS (qui est cassed en fin de boucle)
				   {	  YstateEQPdelta_target(1); //on restaure Xo (i.e: YSo) (([1->UPTIME]:)YSo_mod=YDo_mod)
								Yset_modeltime(0);
								before_it(iloop+1);	     //...(YItRun);
								Yforward (-1, 0);		     //MD : M(Xo) -> YS
							  Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
								for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
								{   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
										YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
								}
								Yforward_operator('R'); Yforward_operator('K'); 	//et on termine par les operateurs R et K
					 } // sinon (syclop=2) on est dans la version bugged qui parfois marche mieux

					 // calcul de a.dx et propagation par le LT
					 Yset_modeltime(0); 
					 Yrazgrad_all();		  //RAZ de YG : YG = 0
					
           //for (witraj=0; witraj<YNBTRAJ; ++witraj) //a.dXo = Xo * alpha.pdx (([1->UPTIME]:)YGo_mod=YSo_mod*apdx)
           //     YgradEQPstate_traj(witraj, "Y#T", 0, YTabTraj[witraj].nbuptime, pdx);
           YgradEQPstate_target(pdx);
										
					 before_it(iloop+1); 	    //... (YItRun);
					 Ylinward(0);					    //TL : M'(Xo)a.dXo -> YG
           Ylinward_operator('*');  //linward operateurs la fin

					 Y3windice=0;
           for (witraj=0; witraj<YNBTRAJ; ++witraj)
           {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
               YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , TLX);	    //YGn -> [TLX]			
           }
				}

				/* 3) calcul de M(Xo+alpha.dXo) */
				//   3.1) calcule de Xo+alpha.dXo
        YstateEQPdelta_target(1+apdx); //Xo=Xo+alpha.dXo : YDo * (1+alpha.pdx) -> YSo

		    //   3.2) Modele Direct: M(Xo+dXo) et stockage
				Yset_modeltime(0);
				before_it(iloop+1);	 //...(YItRun);
				Yforward (-1, 0);    //MD : M(Xo+alpha.dXo) -> YS
        Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
        for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
        {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		        YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
        }
        Yforward_operator('R');  //et on termine par les
			  Yforward_operator('K');  //    operateurs R et K

				Y3windice=0;
        for (witraj=0; witraj<YNBTRAJ; ++witraj)
        {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
            YstateTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx); 		//YSn -> MXdx
        }

				/* 4) on calcule les rms-tendances par module, et on les stocke */
				//   (nb: pour un meme module, toutes les sorties sont supposed homogenes)				
				n		 = 0; //indice (supposed) correspondant dans les tableaux de sauvegarde
				noco = 0;	//numero d'ordre des cout modul (parmi les modules cout)			
				for (int w1=0; w1<YNBMODUL; ++w1)
				{		if (YTabMod[w1].is_cout)
				  	{	
				  			sizeco = YTabMod[w1].nb_stout * YTabMod[w1].axi;						//   taille
				    		if (YTabMod[w1].axj>0) sizeco = sizeco * YTabMod[w1].axj;		//   atempo
				    		if (YTabMod[w1].axk>0) sizeco = sizeco * YTabMod[w1].axk;		//  du module
				    		nbnanf = 0; //pour gerer NANF (NaN, Inf /0)
				    		sigO1=sigO2=0.0;
				    		for (int sz=0; sz<sizeco; ++sz)
				    		{
				    				isnanf = (!finite(MXdx[n]) || !finite(MX[n]) || !finite(TLX[n])
// || fabs(MXdx[n])<1.0e-15 || fabs(MX[n])<1.0e-15 || fabs(TLX[n])<1.0e-15									//ARAR
										         );
				    			  if (!isnanf)
				    			  {
											if (modop==0)
											{	galphaO1 = MXdx[n] - MX[n] - TLX[n];				//(1)ok
										  	sigO1 += (galphaO1*galphaO1);		
											}
											else																					//(2) not as easy
											{	 if (TLX[n] != 0.0)

											 	 {   galphaO1 = (MXdx[n] - MX[n]) / TLX[n];									
										   			 galphaO2 = galphaO1 - 1;	
										   			 sigO1 += (galphaO1*galphaO1);				
										   			 sigO2 += (galphaO2*galphaO2);
										   	 }
										   	 else
										   			 ++nbnanf;														
										  }
										}
										else
											++nbnanf;										
										
										++n;				    		
								}

								indT = (nbloop * noco) + iloop;
								Tnbnanf[indT]   = nbnanf;								

								if (nbnanf < sizeco)
								{				    		
									if (modop==0)
									{	TrendO1[indT]   = sqrt(sigO1/sizeco-nbnanf);
										//TrendO1[indT] = galphaO1; //courcircuitage RMS ok if only one component (pour mise au point)
							  		TrendO2[indT]   = TrendO1[indT] / (alpha*alpha);
									}
									else
									{	TrendO1[indT]   = sqrt(sigO1/(sizeco-nbnanf));
								  	TrendO2[indT]   = sqrt(sigO2/(sizeco-nbnanf)) / alpha;
									}
								}
								else
								{	Tnbnanf[indT]   = -nbnanf; /* dans ce cas la, toutes les mailles du module sont
									nanf; sizeco-nbnanf est sensed etre = 0 ce qui est interdit pour la division;
									pour le signaler, on affichera ci-apres un nombre de nanf negatif ! d'ou l'inversion
									de signe; et pour faire bonne mesure, on met les Trend a 0. */
									TrendO1[indT]=TrendO2[indT]=0.0;
								}								
												    		
				    		++noco;	 //numero d'ordre (parmi les cout) des modules cout				
				  	}//fin if module is_cout
				}//fin boucle sur les modules															
		}//fin boucle des calculs avec decroissance des tendances
				
		/* C) AFFICHAGE DES RESULTATS PAR MODULE */
		if (modop==0)
		{  printf("\n                             g = M(Xo+a.dx) - M(Xo) - M'(Xo)a.dx\n");
		   printf("         a decrease     ordre 1: rms(g)-->0    ordre 2: [rms(g)/(a.a)]-->K \n");
		}
		else
		{  printf("\n                             g = (M(Xo+a.dx) - M(Xo)) / M'(Xo)a.dx\n");
		   printf("         a decrease     ordre 1: rms(g)-->1    ordre 2: [rms(g-1)/a]-->K \n");
		}
		noco = 0;	//numero d'ordre des cout modul (parmi les modules cout)			
		for (int w1=0; w1<YNBMODUL; ++w1)
		{		if (YTabMod[w1].is_cout)
				{		if (YTabMod[w1].ctrord > 0)
						{		//si le module est dans l'order (ce qui devrait etre le cas pour un module cout !?)
								printf("\n module cout ->       %s : \n",YTabMod[w1].Name);
								alpha = svalpha;
								for (int iloop=0; iloop<nbloop; ++iloop)
								{		indT = (nbloop * noco) + iloop;
										alpha = alpha / fdec;
										//printf("   %3i :  %13.6e           %13.6e	\n", iloop+1, TrendO1[indT], TrendO2[indT]);
										if (modop==0)
										    printf("   :%3i: %- 15.6e : -0-> %- 15.6e    -K-> %- 15.6e",
										            iloop+1, alpha, TrendO1[indT], TrendO2[indT]);
										else
										    printf("   :%3i: %- 15.6e : -1-> %- 15.6e    -K-> %- 15.6e",
										            iloop+1, alpha, TrendO1[indT], TrendO2[indT]);
										if (Tnbnanf[indT]!=0) printf("  %i NANF", Tnbnanf[indT]);
										printf("\n");										
								}
						}
						else
						{		printf("\n module ->               %s : is not in the ORDER run over !?\n\n",YTabMod[w1].Name);
						}
						++noco;
				}
		}
				
		/* D) FIN */
   	//    on restaure, autant que faire ce peut, l'etat initial
    YstateEQPdelta_target(1);
		//    on libere la memoire				
		delete[]MX;  delete[]TLX;  delete[]MXdx;	delete[]MpXdx;	delete[]TrendO1; delete[]TrendO2;
		//    et on laise le temps propre en ordre
		Yset_modeltime(0);
}

/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
void Ytestof(double pdx, double alpha, double fdec, int nbloop, int modop, int syclop)
{		//PURPOSE : test de l'Objective Fonction :
		//pdx   : pourcentage De perturbation de Xo pour obtenir dx
		//alpha : parametre de tendance
		//fdec  : facteur de decroissance de pdec alias alpha
		//nbloop: nombre de boucles de decroissance
		//modop : mode operatoire: si 0: ->0, sinon ->1
		//syclop: Secret Yao Systeme Code Operation (0: algo officiel par defaut)
		//        1: algo (initial) theoriquement equivalent a l'algo officiel mais plus lent

		double	apdx;
		double	JXdx;
		double	trendO1, trendO2;
		double	DJdx=0.0;
		double  JX=0.0;
		YREAL		*Jpx = new YREAL[YSIZEPB];		// tableau pour J'(Xo)dx (avant c'etait pour les a.dx)
		if (Jpx==NULL){printf("Yao=>ABORT: pb on new Jpx for testof (see Yao system administrator)\n"); exit(-9);}
		YREAL		*DJ  = new YREAL[YSIZEPB];		// tableau de gradinat YG
		if (DJ==NULL) {printf("Yao=>ABORT: pb on new DJ for testof (see Yao system administrator)\n"); exit(-9);}
	
	  memset(Jpx, 0, YSIZEPB*sizeof(YREAL));
	  memset(DJ,  0, YSIZEPB*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		//   0) presentation :
printf("\ntestof : NEW : use of new function set_dxalea (%s)\n", Ydxalea);
		printf ("\n Objective Function TEST : with: pdx = %e, (a)lpha = %e", pdx, alpha);
		printf ("\n -------------------------       fdec= %e, (n)bloop= %i\n", fdec, nbloop);
		printf (" nb 1) dx = Xo * pdx \n");
		printf ("    2) a(n) = a(n-1) / fdec \n");
		if (modop==0)
		{ printf ("                              g = (J(Xo+a.dx) - J(Xo) - (@J/@Xo).a.dx\n");
		  printf ("         a decrease        ordre 1: (g)-->0     ordre 2: [(g)/(a.a)]-->K \n");
		}
		else
		{ printf ("                              g = (J(Xo+a.dx) - J(Xo)) / (@J/@Xo).a.dx\n");
		  printf ("         a decrease        ordre 1: (g)-->1     ordre 2: [(g-1)/a]-->K \n");
		}
		
		YAL1Run=RUNL1_M1QN3; //ruse pour ne pas faire l'Yadjust inutilement dans basic_it
												 //de+, rappel: basic_it prevoit un razgrad_all apres la passe avant

    //A) On suppose que les obs (y°) et les ebauches ebx (Xb) ont ete charges (comme Nal et d'hab)
    //   avec loadobs, outoobs, outoebx ou une xuserfct.
		//B) On suppose Xo initialised par l'utilisateur (setstate, loadstate ou xuserfct -> YS);
		//   on sauvegarde Xo (i.e: YSo) dans YDo:
    YdeltaEQPstate_target(1); //(([1->UPTIME]:)YDo_mod=YSo_mod) -> YDo

		//C) algo officiel : calcul de J(Xo) et @J/@Xo.dx (une seule fois)
		if (syclop==0)
		{	// init de J(Xo) et (@J/@Xo).dX
 			Ybasic_it();	//(MD+AD): J(Xo)->YTotalCost, et @J/@Xo->YGo
			JX = YTotalCost;
      printf("                                 Cost function: J(Xo) = %e   J(Xo+a.dx)  (@J/@Xo).a.dx\n",JX);
      YstateEQPOdelta_target(pdx, Ydxalea);

		  Y3windice=0;
      YstateTOtab_target(Jpx); //dX -> Jpx

		  Y3windice=0;
      YgradTOtab_target(DJ);	 //@J/@Xo -> DJ

      //for (int wi = 0; wi<YSIZEPB; ++wi) Jpx[wi] = Jpx[wi] * DJ[wi];	//@J/@Xo.dX
      //attention, pour garder le meme random de dx par la suite, on intervertis Jpx et DJ
      for (int wi = 0; wi<YSIZEPB; ++wi) DJ[wi] = Jpx[wi] * DJ[wi];	//@J/@Xo.dX
		}

		//D) boucle de test
		for (int iloop=1; iloop<=nbloop; ++iloop)
		{
				//1) evolution des p*
				alpha = alpha / fdec;			//decroissance de alpha
				apdx  = pdx * alpha;

				//2 Xo + a.dX
        Y3windice=0;
        YstateEQAPTdelta_target(alpha,Jpx); //: YDo + alpha*Jpx -> YSo

				//3) iteration de base (MD+AD) -> J(Xo+a.dX)  (et @J/@(Xo+dX) inutile)
				Ybasic_it();	//(MD+AD): J(Xo+a.dX)->YTotalCost, et @J/@(Xo+.dX)->YGo (inutile)
				JXdx = YTotalCost;

				if (syclop==0) /*(nouvel algo officiel (default one))*/
				{	 DJdx = 0.0;
           //for (int wi = 0; wi<YSIZEPB; ++wi) DJdx += Jpx[wi] * alpha;
           for (int wi = 0; wi<YSIZEPB; ++wi) DJdx += DJ[wi] * alpha; //nb: on a intervertis les roles de Jpx & DJ
				}
				if (syclop==1)	/*(ancien algo)*/
				{  printf ("testof: this old mode is no more maintened ; forget it\n"); break;
				   /*
				   //4)retauration de Xo :
           YstateEQPdelta_target(1); //(([1->UPTIME]:)YSo_mod=YDo_mod)	->YSo
				   //5) iteration de base (MD+AD) -> J(Xo)  et  @J/@Xo
				   Ybasic_it();	//(MD+AD): J(Xo)->YTotalCost, et @J/@Xo->YGo
				   JX = YTotalCost; //...
				   //6) calculer le denominateur (Djdx)
				   //   retauration de a.dx dans [Jpx]
            YstateEQPdelta_target(apdx);  //(([1->UPTIME]:)YSo_mod=YDo_mod*ppd)->YSo	: a.dx -> YSo
		       Y3windice=0;
           YstateTOtab_target(Jpx); //YSo -> [Jpx]	:	a.dX -> [Jpx]
				   //	 Mettre YGo ie @J/@Xo dans [DJ]
		       Y3windice=0;
           YgradTOtab_target(DJ);		//YGo -> [DJ]
				   //   Calcul de Djdx = @J/@Xo . a.dX
				   DJdx = 0.0; //@J/@Xo . p????
				   for (int wi = 0; wi<YSIZEPB; ++wi) DJdx += DJ[wi] * Jpx[wi];
					 */
				}

				//7) Resultat des tendences aux Ordre 1 et 2 et affichage du resultat
				//   1ere maniere
				if (modop==0)
				{  trendO1 = JXdx - JX - DJdx;
				   trendO2 = trendO1 / (alpha*alpha);
				   printf("   :%3i: %- 15.6e : -0-> %- 15.6e -K-> %- 15.6e %- e %- e\n", iloop, alpha, trendO1, trendO2, JXdx, DJdx);
				}
				else //   2eme maniere
				{  trendO1 = (JXdx-JX)/DJdx;
				   trendO2 = (trendO1 - 1)/alpha;
				   printf("   :%3i: %- 15.6e : -1-> %- 15.6e -K-> %- 15.6e %- e %- e\n", iloop, alpha, trendO1, trendO2, JXdx, DJdx);
				}
		}

		//E) FIN
   	//   on restaure, autant que faire ce peut, l'etat initial
    YstateEQPdelta_target(1);
		//   on libere la memoire
		delete[] Jpx; delete[] DJ;
		//   et on laise le temps propre en ordre
		Yset_modeltime(0);
}

/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/
int Ydftestijkt(int imod)
{   // verif que les cordonnees courantes (Yi, Yj, Yk, Yt) sont compatibles
    // pour le module passed en parametre en se limitant aux coordonnées
    // definies pour le module (fait pour testdf). Envoie un message d'erreur
    // et un mauvais cr si ce n'est pas le cas
    int codret=1;
    if (Yi>=YTabMod[imod].axi) codret=0;
    if (YTabMod[imod].dim>=2)
		{	 if (Yj>=YTabMod[imod].axj) codret=0;}
    if (YTabMod[imod].dim>=3)
		{	 if (Yk>=YTabMod[imod].axk) codret=0;}
		if (YTabMod[imod].nb_time>0)
		{	 if (YTemps>=YTabMod[imod].nb_time) codret=0;}  //ARAR
		if (codret==0)
			 printf("testdf: coordinates out for module %s; not proceeded\n", YTabMod[imod].Name);
		return(codret);
}
/* --------------------------------------------------------------------------*/
int Ydftesttt (int itraj)
{   // test que le Temps courant est compatible avec une Trajectoire
		//printf("itraj %i: %i %i %i \n", itraj, YTemps, YTabTraj[itraj].nbuptime,YTabTraj[itraj].nbsteptime);
	  if (YTemps >= YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime
	  ||  YTemps < YTabTraj[itraj].nbuptime)
		{  printf("testdf: time out for trajecyory %s; not proceeded\n", YTabTraj[itraj].name);
		   return(0);
		}
		return(1);
}
/* --------------------------------------------------------------------------*/
int Ytestdf(int argc, char *argv[])
{		/*syntaxe: testdf i   j   k   t  codop [%]pdx ptol [nmmod-ko-max]
	              [0]  [1] [2] [3] [4]  [5]    [6]   [7]      [8]            */

		int   witraj, itrajelue;
		int   modop=0;
		int   yi, yj, yk, yt, wsyi, wsyj, wsyk, wsyt;
		float pdx, ptol;
		int 	All, KeKo;
		char  nmmod[LG_MAX_NAME+1];
		int   nbko=0;
		int	  NbMaxKo=0;

		//int		wt;

		Ytop0 = time((time_t *)NULL);

		yi=atoi(argv[1]);yj=atoi(argv[2]);yk=atoi(argv[3]);yt=atoi(argv[4]);

    //if (yi<0 || yj<0 || yk<0 || yt<=0)
    if (yi<=0 || yj<=0 || yk<=0 || yt<=0)
    {  printf("testdf: coordinates (ijkt) must be positive \n");
       return(0);
    }

 		for (witraj=0; witraj<YNBTRAJ; ++witraj)    //sauvegarde dans YD de l'etat init
        YdeltaEQPstate_traj(witraj, "Y#A", 0,  YTabTraj[witraj].nbuptime, 1);
				
		--yi; --yj; --yk, --yt;
		printf("\ntestdf: derivative calculus check starting : \n");

		if (argv[6][0]=='%')
		{	 pdx=atof(&argv[6][1]);
			 printf("perturbation percent  : pdx=%e \n", pdx);
		}
		else
		{	 modop=1; pdx=atof(argv[6]);
		   printf("fixed perturbation    : pdx=%e \n", pdx);
		}

		ptol=atof(argv[7]);
		printf("tolerence percent     : ptol=%e\n", ptol);

		wsyi=Yi; wsyj=Yj; wsyk=Yk; wsyt=Yt; /* sauvegarde des coordonnees globales */
		Yi=yi; Yj=yj; Yk=yk; YTemps=yt;		  /* nb: dans le cas du codop = 'F' ca va etre ecrased */
			
		All=1; KeKo=0; nmmod[0]='\0';
		witraj = -1;
		if (argc>=9)
		{	if(Yimod(argv[8])>=0)
		  {  All=0;
		     strcpy(nmmod, argv[8]);
		     witraj = Yitrajimod(Yimod(argv[8])); //trajectoire de ref = celle du module ou ...
		  }		
			else
			{  KeKo=1;
			   if (Yitraj(argv[8])>=0) witraj=Yitraj(argv[8]); //une trajectoire de ref it-self
			   else //ce doit etre un maxFko > 0
			   {  if (atoi(argv[8])<=0)
			      {  printf("unknwon object (modul or trajectory) or bad value for MaxFko\n");
			         return(0);
			      }
			   }
			}
		}

		printf("\n Modulus |   grad   |  calculated  | approximeted |   |Ecart|    : test|     input    | (i j k t)\n");

		if     (argv[5][0]=='f') /* test a la mode forward_order que sur une maille dont les coordonnees */
		{	nbko = Ydfward_order_maille(modop, nmmod, All, KeKo, pdx, ptol); /* globales Yi, Yj, Yk, Yt sont deja positionnees */
		  printf ("--> %i KO found at coordinate (%i %i %i %i)  \n", nbko, Yi+1, Yj+1, Yk+1, Yt+1);
		}
		else if(argv[5][0]=='F') /* test sur l'espace temps a partir des coordonnees passe en parametre...*/
		{	/* Si le 9ième/10ième argument ne sont pas ou mal renseigned, on prend 100 ko par defaut avant de stoper */
			KeKo=1;
			if (argc==9)  NbMaxKo=atoi(argv[8]);
			if (argc==10) NbMaxKo=atoi(argv[9]);
			if (NbMaxKo==0) NbMaxKo=YNbItRun; 	/* first default nbmaxko value  */
			if (NbMaxKo==0) NbMaxKo=100;				/* second default nbmaxko value */

			/* une passe avant a la mode dfward */
			Yset_modeltime(0);
			before_it(1);
			if (witraj==-1) //trajectoire de ref par defaut
		  {  witraj=0;
		     printf("default trajectorie is %s \n", YTabTraj[witraj].name);
		  }
		  //il faut avancer jusqu'au temps voulu sans faire de test (avec trajectoire de ref)
			Yforward(witraj, yt);
			//puis continuer en faisant les tests
		  while ((itrajelue=Yforward_elect()) >= 0)   // le while controle la progression
		  {   YTemps  = YTabTraj[itrajelue].toptime;
				  YidTraj = itrajelue;
          if (YDispTime)
          {   printf (" >>> Traj %s, Current dfward time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
						 //Ydispcurstep(itrajelue);
				  }
				  forward_before(0); //arar
				  nbko += YTabTraj[itrajelue].dfward(modop, nmmod, All, KeKo, NbMaxKo-nbko, pdx, ptol, yi, yj, yk); //<-: dfward_order (pour LA trajectoire selectionnee)
				  forward_after(0); //arar
          {++YTabTraj[itrajelue].toptime; //<-: ++YTemps
           YTabTraj[itrajelue].curtime += YTabTraj[itrajelue].dtime; //<-: multi
          }
					if (nbko >= NbMaxKo) break;
		  }
		  /* retablissement coordonnees au bord si besoin pour affichage, ci-apres, en mode externe*/
			/*
			if (Yi>=YA1) Yi=YA1-1; if (Yi<0) Yi=0;
			#ifdef YA2
			if (Yj>=YA2) Yj=YA2-1; if (Yj<0) Yj=0;
			#endif
			#ifdef YA3
			if (Yk>=YA3) Yk=YA3-1; if (Yk<0) Yk=0;
			#endif
			if (Yt>=YNBALLTIME) Yt=YNBALLTIME-1;
		  printf ("--> %i KO found [from] to reached coordinate (%i %i %i %i)  \n", nbko, Yi+1, Yj+1, Yk+1, Yt+1);
		  */
      /*ARAR*///printf ("--> %i KO found [from] to reached coordinate (%i %i %i %i)  \n", nbko, Yi+1, Yj+1, Yk+1, YTemps+1);
      printf ("--> %i KO found to reached coordinate\n", nbko); /*ARAR*/
		}
		else
		{	Ysetting(argv[5]);
		  nbko = Ydfward_all(modop, nmmod, All, KeKo, pdx, ptol);
		  printf ("--> %i KO found at coordinate (%i %i %i %i)  \n", nbko, Yi+1, Yj+1, Yk+1, Yt+1);
		}
		
		Yi=wsyi; Yj=wsyj; Yk=wsyk; YTemps=wsyt; /* on laisse les globales comme on les a trouved */
		printf("\n");

		for (witraj=0; witraj<YNBTRAJ; ++witraj)  //restaur l'etat init autant que possible
        YstateEQPdelta_traj(witraj, "Y#A", 0, YTabTraj[witraj].nbuptime, 1);				

		return(0);
}
/* ----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------*/

/**OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO**/
/**    OLD  VERSION      OLD  VERSION      OLD  VERSION      OLD  VERSION      OLD  VERSION      OLD  VERSION      OLD  VERSION **/
/**OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO**/
/*
void Ytestad_mod(double pdx, double errelmax, int maxko, double pzedi)
{		 // PURPOSE : test de l'ADjoint par module : <m'(Xo)x,y> = <x, m*(Xo).y>
		 // pdx :	Pourcentage De perturbation de Xo pour obtenir dx
		 // errelmax : Erreur relative max : on test tous les modules en chaque point
		 // et maxko : de coordonnee, et on considere que le test est ko si l'erreur
     //            relative est > a errelmax. Si on trouve plus de maxko tests ko,
     //            le process est interrompu (exit).
		 // pzedi    : Parametres pour se donner un niveau de zero informatique
 		 //		         lorsque les 2 |termes| sont < pzedi alors -> pas de test => OK !?
     //
     // nb: maintenir en meme temps la fonction testad (s'il y a lieu)
     //
     // called fct : YgradEQPstate_all, YdeltaEQPstate_all,YgradTOtab_all, YtabTOgrad_all
     //              before_it, Yforward, Ylinward, Yrazgrad_all, Ybackward
		 //

    int   witraj;
    int   NBALLTIME;
		YREAL *MXdx = new YREAL[YSIZECO];		// tableau de sauvegarde
		if (MXdx==NULL){printf("Yao=>ABORT: pb on new MXdx for testad (see Yao system administrator)\n"); exit(-9);}
	  memset(MXdx, 0, YSIZECO*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		// 0) Presentation
		printf ("\n module Adjoint TEST :        dx = Xo * pdx  (with pdx = %e)", pdx);
		printf ("\n --------------------    and  dy = m'(Xo).dx\n\n");

		// nb: On suppose Xo initialised par l'utilisateur (setstate ou xuserfct -> YSo)

		// 1) some init
		Ytestad_module=1;
		Yerrelmax=errelmax;
		Ynbko=0; Ymaxko=maxko;
		Ypzedi = pzedi;

		for (witraj=0; witraj<YNBTRAJ; ++witraj)
		{		NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YgradEQPstate_traj (witraj, "Y#A", 0, NBALLTIME, pdx); 	//il faut initialiser tous les dx et en
        YdeltaEQPstate_traj(witraj, "Y#A", 0, NBALLTIME, pdx);	//avoir l'equivalent dans les delta
    } //Avant (paradigme V7) on devait pouvoir se suffir d'initialiser jusqu'a uptime.
      //Maintenant (paradigme V8) version multi-spatio-tempo + target sur trajectoire,
      //il vaut mieux tout initialiser (i.e. toute la trajectoire) sachant que le LT
      //ecrasera par les nelles valeurs qu'il aura calculees
      //(c'est d'ailleurs aussi la raison pour laquelle un razgrad_all ne devrait pas etre utile !)
			   			 		
		// 2) MD (Modele Direct): Calculer M(Xo)
		Yset_modeltime(0);
		before_it(1);				     //(YItRun);
		Yforward (-1, 0);		     //MD : M(Xo) -> YS
    Yact_operator('*', ON);
    Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
    for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		    YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
    }
    Yforward_operator('R');		//et on termine par les
    Yforward_operator('K'); 	//    operateurs R et K

		// 3) LT (Lineaire Tangent): m'(Xo)dXo
		Yset_modeltime(0);
		before_it(1);				     //(YItRun);
		Ylinward(0);				     // TL : M'(Xo)dXo -> YG
    Ylinward_operator('*');  //+linward des operateurs a la fin

		// 4) YGn-> tableau Mxdx: avant RAZ, sauvegarde des dx obtenus sur les modules cout en fin de traj
    Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);			// YGn -> [MXdx]
    }

		// 5) RAZ tous les gradients pour l'Adjoint
		Yrazgrad_all();		// YG = 0

		// 6) et Reprise des dx de fin de trajectoire des modules cout (qui vont etre retro-propagees)
    Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YtabTOgrad_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);				// [MXdx] -> YGn
    }

		// 7) on se lance dans l'Adjoint
    Ybackward_operator('*'); //d'abord backward des operateurs en fin de trajectoire
		Ybackward(-1, 0);        //au cours duquel le test sur chaque module se fait (YNBSTEPTIME);
		//quid after_it ?

		printf(" -->-->--> %i case(s) found \n\n", Ynbko);
		// 8) FIN
		//    on libere la memoire
		delete[] MXdx;
		//    et on laise le temps propre en ordre
		Yset_modeltime(0);

		// Restaurer (~) l'etat init ? En principe il n'est pas modified !?
		//for (witraj=0; witraj<YNBTRAJ; ++witraj)
    //    YstateEQPdelta_traj(witraj, "Y#A", 0, YTabTraj[witraj].nbuptime, 1/pdx);
		Ytestad_module=0;
}
*/
/**OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO**/
/*
void Ytestad(double pdx)
{		 // PURPOSE : test de l'ADjoint : <M'(Xo)x,y> = <x, M*(Xo).y>
		 //		ici, dx correspond a x, et dx* a y
		 //		dans Yao la zone utilised pour Xo est YS, et YG est utilised pour dx lors
		 //		du TL (passe avant) puis pour dx* lors de l'Adjoint (passe arriere) d'ou
		 //		la necessite de passer par des tableaux pour sauvegarder et manipuler les
		 //		resultats des calculs.
		 //  pdx :	Pourcentage De perturbation de Xo pour obtenir dx
		 //  errelmax : Erreur relative max : si cette valeur est differente de 0 alors
		 //  et maxko : on test tous les modules en chaque point de coordonnee, et on
		 //             considere que le test est ko si l'erreur relative est > a errelmax.
		 //             Si on trouve plus de maxko tests ko, le process est interrompu (exit).
     //
		 //	nb: maintenir en meme temps la fonction testad_mod (s'il y a lieu)
     //
		 //	called fct : YgradEQPstate_all, YgradTOtab_all, YtabTOgrad_all
		 //						   Yrazgrad_all, before_it, Yforward, Ylinward, Ybackward
		 //

		int   witraj;
		int   NBALLTIME;
		double LTRes=0.0, AdRes=0.0;
		YREAL *DX   = new YREAL[YSIZEPB];		// tableau de sauvegarde de dx
		if (DX==NULL)  {printf("Yao=>ABORT: pb on new DX for testad (see Yao system administrator)\n"); exit(-9);}
		YREAL *MXdx = new YREAL[YSIZECO];		// tableau resultat du Lineaire Tangent M'(Xo)dX
		if (MXdx==NULL){printf("Yao=>ABORT: pb on new MXdx for testad (see Yao system administrator)\n"); exit(-9);}
		YREAL *MAdx = new YREAL[YSIZEPB];		// tableau resultat de l'Adjoint M*(Xo)d*X
		if (MAdx==NULL){printf("Yao=>ABORT: pb on new MAdx for testad (see Yao system administrator)\n"); exit(-9);}

	  memset(DX,   0, YSIZEPB*sizeof(YREAL));
	  memset(MXdx, 0, YSIZECO*sizeof(YREAL));
	  memset(MAdx, 0, YSIZEPB*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		// 0) Presentation
		printf ("\n Adjoint TEST :         dx = Xo * pdx  (with pdx = %e)", pdx);
		printf ("\n --------------    and  dy = M'(Xo).dx\n\n");

		Ytestad_module=0;		// because c'est la meme instruction qui sert dans les 2 cas, il faut pouvoir distinguer

		// nb: On suppose Xo initialised par l'utilisateur (setstate ou xuserfct -> YSo)

		// 1) Init de dXo -> YGo
		Yrazgrad_all();

    // dXo = Xo * pdx (([1->UPTIME]:)YGo_mod=YSo_mod*pdx)
    YgradEQPstate_target(pdx);

		// 2) MD (Modele Direct): Calculer M(Xo)
		Yset_modeltime(0);
		before_it(1);				//(YitRun);
		Yforward(-1, 0);		//MD : M(Xo) -> YS
    Yact_operator('*', ON);
    Yforward_operator('H');                                                  //forward operateurs a la fin; d'abord H seulement,
    for (witraj=0; witraj<YNBTRAJ; ++witraj)															   //puis il faut mettre YW = YS pour les modules de
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime; //(M ou H) connectes vers des modules operateurs
		    YwishEQPstate_traj_tocov(witraj, NBALLTIME-1, 1.0);									 //de covariance (R ou K) ... puisque ... sinon ...
    }
    Yforward_operator('R');  //et on termine par les
    Yforward_operator('K');  //    operateurs R et K

		// 3) LT (Lineaire Tangent): M'(Xo)dXo
		Yset_modeltime(0);
		before_it(1);				//(YitRun);
		Ylinward(0);				//TL : M'(Xo)dXo -> YG
    Ylinward_operator('*'); //linward opera la fin !!?? (car on ne s'interesse qu'aux modules cout en fin de traj)

		// 4) YGn-> tableau Mxdx et calcul de < M'(Xo)dX, dy > (sur module cout en fin de traj)
		Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YgradTOtab_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);					// YGn -> [MXdx]
    }

		for(int i=0; i<YSIZECO; ++i)
		{LTRes += MXdx[i]*MXdx[i];	// < M'(Xo)dX, dy >
		}

		// 6) au passage, on a peut etre aussi besoin de conserver donc de sauvegarder YGo (isnt-it?)
		Y3windice=0;
    YgradTOtab_target(DX);			// YGo -> [DX]

		// 7) RAZ tous les gradients pour l'Adjoint
		Yrazgrad_all();		// YG = 0

		// 8) et Reprise des gradients de fin de trajectoire des modules cout
		Y3windice=0;
    for (witraj=0; witraj<YNBTRAJ; ++witraj)
    {   NBALLTIME = YTabTraj[witraj].nbsteptime + YTabTraj[witraj].nbuptime;
        YtabTOgrad_traj(witraj, "Y#C", NBALLTIME-1, NBALLTIME , MXdx);				// [MXdx] -> YGn
    }

		// 9) on devrait pouvoir se lancer dans l'adjoint
    Ybackward_operator('*'); //d'abord backward des operateurs en fin de trajectoire
		Ybackward(-1, 0);  // AD (adjoint):-> dx =M*(Xo)d*X : Yjac * YG -> Ytbeta
		//quid after_it ?

		// 10) YGo-> tableau MAdx et calcul de < dx, M*(Xo).dy >
		Y3windice=0;
    YgradTOtab_target(MAdx);		 // YGo -> [MAdx]

		for(int i=0; i<YSIZEPB; ++i)
		{	AdRes += DX[i] * MAdx[i];									// < dx, M*(Xo).dy >
		}

		// 11) Afficher le resultat
		printf (" < M'(Xo).dx, dy >  = %e \n", LTRes);
		printf (" < dx, M*(Xo).dy >  = %e \n", AdRes);
		printf ("                     --------------\n");
		printf ("   ecart :            %e \t", LTRes-AdRes);
		printf (" relative error = %e \n\n", (LTRes-AdRes)/LTRes);

		// 12) on informe d'une possible cause d'echec par exemple si des obs sont
		if (YioInsertObsCtr != -1) // charger dans une des fonction d'intervention
		{	 printf("testad warning: obs must not be used for global Adjoint test\n");
		   	   //because: ca modifie le dy au cours de la redescente qui ne sera plus le meme
		   	   //         que celui utilised lors du calcul de LTRes.
		   	   //         Cette contrainte n'est pas utile pour testad_mod car le test est local.
		}

		// 13) FIN
		//    on libere la memoire
		delete[] DX;  delete[] MXdx;  delete[] MAdx;
		//    et on laise le temps propre en ordre
		Yset_modeltime(0);
}
*/
/**OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO**/
/*
void Ytestof(double pdx, double alpha, double fdec, int nbloop, int modop, int syclop)
{		//PURPOSE : test de l'Objective Fonction :
		//pdx   : pourcentage De perturbation de Xo pour obtenir dx
		//alpha : parametre de tendance
		//fdec  : facteur de decroissance de pdec alias alpha
		//nbloop: nombre de boucles de decroissance
		//modop : mode operatoire: si 0: ->0, sinon ->1
		//syclop: Secret Yao Systeme Code Operation (0: algo officiel par defaut)
		//        1: algo (initial) theoriquement equivalent a l'algo officiel mais plus lent
				
		double	apdx;
		double	JXdx;
		double	trendO1, trendO2;
		double	DJdx=0.0;
		double  JX=0.0;
		YREAL		*Jpx = new YREAL[YSIZEPB];		// tableau pour J'(Xo)dx (avant c'etait pour les a.dx)
		if (Jpx==NULL){printf("Yao=>ABORT: pb on new Jpx for testof (see Yao system administrator)\n"); exit(-9);}
		YREAL		*DJ  = new YREAL[YSIZEPB];		// tableau de gradinat YG
		if (DJ==NULL) {printf("Yao=>ABORT: pb on new DJ for testof (see Yao system administrator)\n"); exit(-9);}

	  memset(Jpx, 0, YSIZEPB*sizeof(YREAL));
	  memset(DJ,  0, YSIZEPB*sizeof(YREAL));

		Ytop0 = time((time_t *)NULL);

		//   0) presentation :
		printf ("\n Objective Function TEST : with: pdx = %e, (a)lpha = %e", pdx, alpha);
		printf ("\n -------------------------       fdec= %e, (n)bloop= %i\n", fdec, nbloop);
		printf (" nb 1) dx = Xo * pdx \n");
		printf ("    2) a(n) = a(n-1) / fdec \n");
		if (modop==0)
		{ printf ("                             g = J(Xo+a.dx) - J(Xo) - (@J/@Xo).a.dx\n");
		  printf ("         a decrease        ordre 1: (g)-->0    ordre 2: [(g)/(a.a)]-->K \n\n");
		}
		else
		{ printf ("                             g = (J(Xo+a.dx) - J(Xo)) / (@J/@Xo).a.dx\n");
		  printf ("         a decrease        ordre 1: (g)-->1    ordre 2: [(g-1)/a]-->K \n\n");
		}

		YAL1Run=RUNL1_M1QN3; //ruse pour ne pas faire l'Yadjust inutilement dans basic_it
												 //de+, rappel: basic_it prevoit un razgrad_all apres la passe avant

    //A) On suppose que les obs (y°) et les ebauches ebx (Xb) ont ete charges (comme Nal et d'hab)
    //   avec loadobs, outoobs, outoebx ou une xuserfct.
		//B) On suppose Xo initialised par l'utilisateur (setstate, loadstate ou xuserfct -> YS);
		//   on sauvegarde Xo (i.e: YSo) dans YDo:
    YdeltaEQPstate_target(1); //(([1->UPTIME]:)YDo_mod=YSo_mod) -> YDo

		//C) algo officiel : calcul de J(Xo) et @J/@Xo.dx (une seule fois)
		if (syclop==0)
		{	// init de J(Xo) et (@J/@Xo).dX
 			Ybasic_it();	//(MD+AD): J(Xo)->YTotalCost, et @J/@Xo->YGo
			JX = YTotalCost;
      printf("                                 Cost Function: J(Xo)=%e     J(Xo+a.dx)  (@J/@Xo).a.dx\n",JX);
      YstateEQPdelta_target(pdx); //dX

		  Y3windice=0;
      YstateTOtab_target(Jpx); //dX -> Jpx

		  Y3windice=0;
      YgradTOtab_target(DJ);			//@J/@Xo -> DJ

 			for (int wi = 0; wi<YSIZEPB; ++wi) Jpx[wi] = Jpx[wi] * DJ[wi];	//@J/@Xo.dX
		}

		//D) boucle de test
		for (int iloop=1; iloop<=nbloop; ++iloop)
		{
				//1) evolution des p*
				alpha = alpha / fdec;			//decroissance de alpha
				apdx  = pdx * alpha;

				//2 Xo + a.dX
        YstateEQPdelta_target(1+apdx); //: YDo * (1+apdx) -> YSo

				//3) iteration de base (MD+AD) -> J(Xo+a.dX)  (et @J/@(Xo+dX) inutile)
				Ybasic_it();	//(MD+AD): J(Xo+a.dX)->YTotalCost, et @J/@(Xo+.dX)->YGo (inutile)
				JXdx = YTotalCost;

				if (syclop==0) //(nouvel algo officiel (default one))
				{	 DJdx = 0.0;
 					 for (int wi = 0; wi<YSIZEPB; ++wi) DJdx += Jpx[wi] * alpha;
				}
				if (syclop==1)	//(ancien algo)
				{	 //4)retauration de Xo :
           YstateEQPdelta_target(1); //(([1->UPTIME]:)YSo_mod=YDo_mod)	->YSo

				   //5) iteration de base (MD+AD) -> J(Xo)  et  @J/@Xo
				   Ybasic_it();	//(MD+AD): J(Xo)->YTotalCost, et @J/@Xo->YGo
				   JX = YTotalCost; //...
				   //6) calculer le denominateur (Djdx)
				   //   retauration de a.dx dans [Jpx]
            YstateEQPdelta_target(apdx);  //(([1->UPTIME]:)YSo_mod=YDo_mod*ppd)->YSo	: a.dx -> YSo

		       Y3windice=0;
           YstateTOtab_target(Jpx); //YSo -> [Jpx]	:	a.dX -> [Jpx]

				   //	 Mettre YGo ie @J/@Xo dans [DJ]
		       Y3windice=0;
           YgradTOtab_target(DJ);		//YGo -> [DJ]

				   //   Calcul de Djdx = @J/@Xo . a.dX
				   DJdx = 0.0; //@J/@Xo . p????
				   for (int wi = 0; wi<YSIZEPB; ++wi) DJdx += DJ[wi] * Jpx[wi];
				}

				//7) Resultat des tendences aux Ordre 1 et 2 et affichage du resultat
				//   1ere maniere
				if (modop==0)
				{  trendO1 = JXdx - JX - DJdx;
				   trendO2 = trendO1 / (alpha*alpha);
				   //printf("   :%3i: %- 15.6e : -0-> %- 15.6e    -K-> %- 15.6e\n", iloop, alpha, trendO1, trendO2);
				   printf("   :%3i: %- 15.6e : -0-> %- 15.6e -K-> %- 15.6e %- e %- e\n", iloop, alpha, trendO1, trendO2, JXdx, DJdx);
				}
				else //   2eme maniere
				{  trendO1 = (JXdx-JX)/DJdx;
				   trendO2 = (trendO1 - 1)/alpha;
				   //printf("   :%3i: %- 15.6e : -1-> %- 15.6e    -K-> %- 15.6e\n", iloop, alpha, trendO1, trendO2);
				   printf("   :%3i: %- 15.6e : -1-> %- 15.6e -K-> %- 15.6e %- e %- e\n", iloop, alpha, trendO1, trendO2, JXdx, DJdx);
				}
		}

		//E) FIN
   	//   on restaure, autant que faire ce peut, l'etat initial
    YstateEQPdelta_target(1);
		//   on libere la memoire
		delete[] Jpx; delete[] DJ;
		//   et on laise le temps propre en ordre
		Yset_modeltime(0);
}
*/
/**OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO**/










