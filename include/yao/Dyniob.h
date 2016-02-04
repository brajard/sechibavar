
/*=============================================================================
	 Fonctions de Gestion des entrees - sorties et des observations pour Yao
=============================================================================*/

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/* structure d'une observation -> genere dans Y1 a cause de ... */
		
/* structure d'un noeud de l'arborescence des observations
struct   Yst_nodo  {
	int		 iind; // indice pour l'indicateur t, m, s, i, j
	struct Yst_nodo *frere;
	struct Yst_nodo *fils;
}; //mis dans Dynamo.h may be because la structure Yst_traj l'utilise
*/

/* structure d'une feuille de l'arborescence des observations */
struct   Yst_fobs  {
	int		 iind; /* indice pour l'indicateur i, j ou k selon la dim ! */
	YREAL  vobs; /* valeur de l'observation */
  #ifdef YO_VARINCR
	YREAL  qtea; /* quantite adjointe (dx*) */
	#endif
	
	struct Yst_fobs *frere;
};

struct Yst_nodo *YRobs=NULL; /* Racine de l'Arborescence des observations */
struct Yst_nodo *TRobs; /* un pointeur sur le Time   s/Root courant trouve */
struct Yst_nodo *MRobs; /* un pointeur sur le Modul  s/Root courant trouve */
struct Yst_nodo *SRobs; /* un pointeur sur la Sortie s/Root courant trouve (dim>0 forcement)*/
struct Yst_nodo *IRobs; /* un pointeur sur la Sortie s/Root courant trouve (si dim>1)*/
struct Yst_nodo *JRobs; /* un pointeur sur la Sortie s/Root courant trouve (si dim>2 =>3) */
struct Yst_fobs *YLobs; /* un pointeur sur l'element courant de la liste des obs */
int    Ytrupdt;         /* a cause des ebauches qui sont stocke dans l'arbo des obs
                           avec un pdt<0 on a besoin de le distinguer avec le vrai pdt !*/

/* Tree Obsevations globales variables */
/*int YTOLevel;	niveau dans l'arbre des observations (not used, but ...) */
int		YTODimMod; /* Dim d'un modul (en cours) dans l'arbre des obs*/
char	YTONamMod[STRSIZE80+1];	/* le nom du module en cours ... */

/*===========================================================================*/
void Yobs_disp(struct Yst_obs *aobs)
{
		 printf("(d=%i)", YTabMod[aobs->imod].dim);
		 printf("(t m s i j k v) :::> (%i %i %i %i %i %i %f) \n",
						  aobs->time+1, aobs->imod+1, aobs->smod+1,
						  aobs->iaxe+1, aobs->jaxe+1, aobs->kaxe+1, aobs->vobs);
}	

/*===========================================================================*/
int Yobs_insert(struct Yst_obs *aobs)
{		/* insertion d'une observation dans l'arborescence des observations      */
    /* WARNING: PLM, on considere que l'observation match sur les ordres
    						d'axes i ou ij, ou ijk : les autres combinaisons ne sont pas
    						traites !!!                                                  */		

		struct Yst_nodo *p1, *p2; /* des pointeurs pour parcourir les noeuds.. */	
		struct Yst_nodo *newn;    /* un pointeur pour creer les noeuds.. */
#ifndef YAP_IOBNOCHK
		struct Yst_fobs *p3, *p4; /* des pointeurs pour parcourir les feuilles.. */
#endif
		struct Yst_fobs *newl;    /* un pointeur pour creer les feuilles.. */
		int		 dimod; 						/* la dimension du module */
	
		dimod = YTabMod[aobs->imod].dim;	/* positionne la dimension du module   */
		
		/* Root de l'arbo d'obs pour la trajectoire du module ! (c'est de la que tout part) */
		//YRobs = YTabTraj[Yitraj(YTabSpace[Yispace(YTabMod[aobs->imod].space_name)].traj_name)].YRobs; //faut suivre
		YRobs = YTabTraj[Yitrajimod(aobs->imod)].YRobs;
		
		/*Yobs_disp(aobs); 				pour verif et mise au point
		   printf("       Yobs_insert><");Yobs_disp(aobs);			
		*/
		/*-----------------------------------------------------------------------*/
		/* Il semble plus prudent pour aller au devant de deconvenue de verifier
		   du bon usage de Yobs_insert qui ne devrait etre utilisee que pour des
			 modules target ou cout  */
    if (!YTabMod[aobs->imod].is_target && !YTabMod[aobs->imod].is_cout)
		{  printf ("attempting to insert obs/ebx for a not target or cout module (%s)\n",
		            YTabMod[aobs->imod].Name);
			 exit(-7); //vaut mieux tout stopped ...!?
		}

		/*-----------------------------------------------------------------------*/
		/* 1) positionnement sur le temps =======================================*/
		p2 = p1 = YRobs->fils;	/* a priori on part de la racine */
		while(p1 != NULL && p1->iind > aobs->time)
		{		p2 = p1;	
				p1 = p1->frere;
		}
		if (p1 == NULL || p1->iind < aobs->time) /* pour inserer dans l'ordre du temps */
		{ /* temps nouveau a inserer */
			newn = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo));
			newn->iind = aobs->time;
			newn->fils = NULL;
			if (p1 != NULL)
			{	 newn->frere = p1;
				 if(p2!=p1) p2->frere = newn;			
			}
			else
			{	 newn->frere = NULL;
				 if (p2!=NULL) p2->frere = newn;
			}
			TRobs = newn;
				
			/* la racine doit pointer sur le temps le plus grand de l'arborescence */	
			if (YRobs->fils == NULL || YRobs->fils==p1)
					YRobs->fils = newn;
		}
		else
			TRobs = p1;
		
		/*-----------------------------------------------------------------------*/										
		/* 2) positionnement sur le module ======================================*/
		p2 = p1 = TRobs->fils;	/* a priori on part de la position Time Root trouve */		
		while(p1 != NULL && p1->iind > aobs->imod)	/* ordre descending */
		//while(p1 != NULL && p1->iind < aobs->imod)		/* ordre ascending  */
		{		p2 = p1;
				p1 = p1->frere;
		}
		if (p1 == NULL || p1->iind < aobs->imod)	/* ici aussi, on se moque de l'ordre ... */ /*descending*/
		//if (p1 == NULL || p1->iind > aobs->imod)		/*	... pas si sur			    						 */ /* ascending*/
		{ /* nouveau module a inserer */
			newn = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo));
			newn->iind = aobs->imod;
			newn->fils = NULL;
			if (p1 != NULL)
			{	 newn->frere = p1;
				 if(p2!=p1) p2->frere = newn;			
			}
			else
			{	 newn->frere = NULL;
				 if (p2!=NULL) p2->frere = newn;
			}
			MRobs = newn;	
			
			/* on fait pointer la racine sur le module le plus grand (ce n'est pas une oblig; pouvu qu'il pointe */	
			if (TRobs->fils == NULL || TRobs->fils==p1)
					TRobs->fils = newn;
		}
		else
			MRobs = p1;

		/*-----------------------------------------------------------------------*/										
		/* 3) positionnement sur la sortie du module ============================*/
		p2 = p1 = MRobs->fils;	/* a priori on part de la position Modul Root trouve */
		while(p1 != NULL && p1->iind > aobs->smod)	/* descending */
		//while(p1 != NULL && p1->iind < aobs->smod)	/* ascending  */
		{		p2 = p1;
				p1 = p1->frere;
		}
		if (p1 == NULL || p1->iind < aobs->smod)	/* descending */
		//if (p1 == NULL || p1->iind > aobs->smod)	/* ascending  */
		{ /* nouvelle sortie a inserer */
			newn = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo));
			newn->iind = aobs->smod;
			newn->fils = NULL;
			if (p1 != NULL)
			{	 newn->frere = p1;
				 if(p2!=p1) p2->frere = newn;
			}
			else
			{	 newn->frere = NULL;
				 if (p2!=NULL) p2->frere = newn;
			}			
			SRobs = newn;	
			
			/* on fait pointer la racine sur le module le plus grand (ce n'est pas une obli; pouvu qu'il pointe */	
			if (MRobs->fils == NULL || MRobs->fils==p1)
					MRobs->fils = newn;
		}
		else
			SRobs = p1;

		/*-----------------------------------------------------------------------*/												
		/* 4) positionnement sur l'axe I du module ==============================*/
		if (dimod > 1)
		{/* on traite un poind d'axe I comme une racine °°°°°°°°°°°°°°°°°°°°°°°°°*/
			p2 = p1 = SRobs->fils;	/* a priori on part de la position Sortie Root trouve */		
			while(p1 != NULL && p1->iind > aobs->iaxe)	/* ordre descending */
			//while  (p1 != NULL && p1->iind < aobs->iaxe)	/* ordre  ascending */
			{		p2 = p1;	
					p1 = p1->frere;
			}
			if (p1 == NULL || p1->iind < aobs->iaxe) /* ici aussi, on se moque de l'ordre ... */ /*descending*/
			//if   (p1 == NULL || p1->iind > aobs->iaxe) /*     ... pas si sur !?                 */ /* ascending*/
			{ /* nouvel axe i a inserer */
				newn = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo));
				newn->iind = aobs->iaxe;
				newn->fils = NULL;
				if (p1 != NULL)
				{	 newn->frere = p1;
					 if(p2!=p1) p2->frere = newn;			
				}
				else
				{	 newn->frere = NULL;
					 if (p2!=NULL) p2->frere = newn;
				}			
				IRobs = newn;	
				
				/* on fait pointer la racine sur le noeud le plus grand (ce n'est pas une oblig; pouvu qu'il pointe */
				if (SRobs->fils == NULL || SRobs->fils==p1)
						SRobs->fils = newn;
			}
			else
				IRobs = p1;		
		}
		else
		{ /*sinon le point d'axe I doit etre traite comme une liste de feuilles°°*/
#ifdef YAP_IOBNOCHK
		  /* on insere en debut de liste sans aller jusqu'a la fin (=> pas de verif de doublons */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind = aobs->iaxe;
			newl->vobs = aobs->vobs;
			newl->frere = (struct Yst_fobs *) SRobs->fils;
			SRobs->fils = (struct Yst_nodo *) newl;
			++YioInsertObsCtr;
			return(1); /*!!!!!!!!*/
#else
			p4 = p3 = (struct Yst_fobs *) SRobs->fils;	/* On part de la position de Sortie Root trouve */
			while(p3 != NULL)	/* aller jusqu'a la fin de la liste permet de verifier */
			{		if (p3->iind == aobs->iaxe)	 /* que l'obs n'a pa deja ete rencontree */
			    {	/*Yobs_disp(aobs);
			    	//printf("Yobs_insert; Ilist; bad action : observable already inserted \n");
			    	//return(0); */
			    	p3->vobs = aobs->vobs; 	/* on accepterait d'overrider une obs */
						++YioOvrObsCtr;
			    	return(1);
			    }
					p4 = p3;
					p3 = p3->frere;
			}			
			/* creation et insertion de la leave */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind = aobs->iaxe;
			newl->vobs = aobs->vobs;			
			newl->frere = NULL;	

			/* si c'est le 1er elt, il doit etre ajouter a la racine de la liste,
			   sinon on le rajoute en fin de liste */
			if (SRobs->fils == NULL)
					SRobs->fils = (struct Yst_nodo *) newl;
			else
				 p4->frere = newl;
				
			++YioInsertObsCtr;
			return(1); /*!!!!!!!!*/				
#endif
		}
		
		/*-----------------------------------------------------------------------*/										
		/* 5) positionnement sur l'axe J du module ==============================*/
		if (dimod > 2)
		{/* on traite un poind d'axe J comme une racine °°°°°°°°°°°°°°°°°°°°°°°°°*/
			p2 = p1 = IRobs->fils;	/* a priori on part de la position d'axe I s/Root trouve */	
			while(p1 != NULL && p1->iind > aobs->jaxe)		/* ordre descending */
			//while  (p1 != NULL && p1->iind < aobs->jaxe)			/* ordre  ascending */
			{		p2 = p1;	
					p1 = p1->frere;
			}
			if (p1 == NULL || p1->iind < aobs->jaxe) /* ici aussi, on se moque de l'ordre ... */ /*descending*/
			//if   (p1 == NULL || p1->iind > aobs->jaxe) /*     ... pas si sur !?                 */ /* ascending*/
			{ /* nouvel axe j a inserer */
				newn = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo));
				newn->iind = aobs->jaxe;
				newn->fils = NULL;
				if (p1 != NULL)
				{	 newn->frere = p1;
					 if(p2!=p1) p2->frere = newn;			
				}
				else
				{	 newn->frere = NULL;
					 if (p2!=NULL) p2->frere = newn;
				}			
				JRobs = newn;	
				
				/* on fait pointer la racine sur le noeud le plus grand (ce n'est pas une oblig; pouvu qu'il pointe */	
				if (IRobs->fils == NULL || IRobs->fils==p1)
						IRobs->fils = newn;
			}
			else
				JRobs = p1;		
		}
		else
		{ /*sinon le point d'axe J doit etre traité comme une liste de feuilles°°*/
#ifdef YAP_IOBNOCHK
		  /* on insere en debut de liste sans aller jusqu'a la fin (=> pas de verif de doublons */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind  = aobs->jaxe;
			newl->vobs  = aobs->vobs;
			newl->frere = (struct Yst_fobs *) IRobs->fils;
			IRobs->fils = (struct Yst_nodo *) newl;
			++YioInsertObsCtr;
			return (1); /*!!!!!!!!*/
#else
			p4 = p3 = (struct Yst_fobs *) IRobs->fils;	/* On part de la position l'axe I s/Root trouve */
			while(p3 != NULL)	/* aller jusqu'a la fin de la liste permet de verifier */
			{		if (p3->iind == aobs->jaxe)	 /* que l'obs n'a pa deja ete rencontree */
			    {	/*Yobs_disp(aobs);
			    	//printf("Yobs_insert; Jlist; bad action : observable already inserted \n");			    	
			    	//return(0); */			    	
			    	p3->vobs = aobs->vobs; 	/* on accepterait d'overrider une obs */
						++YioOvrObsCtr;
			    	return(1);			    	
			    }
					p4 = p3;
					p3 = p3->frere;
			}			
			/* creation et insertion de la leave */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind = aobs->jaxe;
			newl->vobs = aobs->vobs;			
			newl->frere = NULL;	

			/* si c'est le 1er elt, il doit etre ajouter a la racine de la liste,
			   sinon on le rajoute en fin de liste */
			if (IRobs->fils == NULL)
			   IRobs->fils = (struct Yst_nodo *) newl;
			else
				 p4->frere = newl;

			++YioInsertObsCtr;
			return (1); /*!!!!!!!!*/
#endif
		}

		/*-----------------------------------------------------------------------*/										
		/* 6) si on est rendu ici, c'est qu'il s'agit du 3eme axe  ==============*/
		{ /*il s'agit forcement d'une liste de feuilles °°°°°°°°°°°°°°°°°°°°°°°°°*/
#ifdef YAP_IOBNOCHK
		  /* on insere en debut de liste sans aller jusqu'a la fin (=> pas de verif de doublons */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind = aobs->kaxe;
			newl->vobs = aobs->vobs;
			newl->frere = (struct Yst_fobs *) JRobs->fils;
			JRobs->fils = (struct Yst_nodo *) newl;
			++YioInsertObsCtr;
			return(1);
#else
			p4 = p3 = (struct Yst_fobs *) JRobs->fils;	/* On part de la position l'axe J s/Root trouve */
			while(p3 != NULL)	/* aller jusqu'a la fin de la liste permet de verifier */
			{		if (p3->iind == aobs->kaxe)	 /* que l'obs n'a pa deja ete rencontree */
			    {	/*Yobs_disp(aobs);
			    	//printf("Yobs_insert; Klist; bad action : observable already inserted \n");			    	
			    	//return(0); */
			    	p3->vobs = aobs->vobs; 	/*on accepterait d'overrider une obs */
						++YioOvrObsCtr;
			    	return(1);			    	
			    }
					p4 = p3;
					p3 = p3->frere;
			}			
			/* creation et insertion de la leave */
			newl = (struct Yst_fobs*) malloc(sizeof(struct Yst_fobs));
			newl->iind = aobs->kaxe;
			newl->vobs = aobs->vobs;			
			newl->frere = NULL;	

			/* si c'est le 1er elt, il doit etre ajouter a la racine de la liste,
			   sinon on le rajoute en fin de liste */
			if (JRobs->fils == NULL)
					JRobs->fils = (struct Yst_nodo *) newl;
			else
				 p4->frere = newl;

			++YioInsertObsCtr;
			return(1);
#endif
		}
									
} /*fin de Yobs_insert */

/*===========================================================================*/
int Yobs_insert_data(char *nmmod, int sortie, int iaxe, int jaxe, int kaxe,
                     int pdt, YREAL val)
{ /* insertion d'une obs dans l'arbo avec les donnees passees en parametre    */
	Yaobs.time = pdt;
 	Yaobs.imod = Yimod(nmmod);
 	Yaobs.smod = sortie;
 	Yaobs.iaxe = iaxe;
 	Yaobs.jaxe = jaxe;
 	Yaobs.kaxe = kaxe;
 	Yaobs.vobs = val;
 	return(Yobs_insert(&Yaobs));
}

/*===========================================================================*/
void Yobs_unload(char *nmmod, char *nsmod)
{	/*syntaxe : load_obs   nom_du_module   numero_de_sortie */
}

/*===========================================================================*/
//void Yobs_reverse_time_lists()
void Yobs_reverse_time_list_traj(struct Yst_nodo *Rlist)
{ /* inversion de la liste du temps dans l'arborescence des observations d'une
     trajectoire (utilised pour la fonction Yforwardcost)                    */
	struct Yst_nodo *p1, *p2, *p3;

  if (Rlist->fils==NULL) return;	//la liste est vide, y'a donc rien a inverser

  p1 = Rlist->fils;               //p1 = YRobs->fils;
	p2 = p1->frere;                 //p2 = YRobs->fils->frere;
	p1->frere = NULL;               //YRobs->fils->frere = NULL;

	while (p2 != NULL)
	{   //printf("%p:%i\n", p1, p1->iind);
			p3 = p2->frere;
			p2->frere = p1;
			p1 = p2;
			p2 = p3;
	}
  Rlist->fils = p1;								//YRobs->fils = p1;
}
/*---------------------------------------------------------------------------*/
int Yobs_reverse_time_lists()
{ int witraj;
  int nblistnotnul=0;
  for (witraj=0; witraj<YNBTRAJ; ++witraj)
  {   if (YTabTraj[witraj].YRobs != NULL) //forcement vrai en principe
      {
			   /*
				 if (YTabTraj[witraj].YRobs->fils != NULL)
            Yobs_reverse_time_list_traj(YTabTraj[witraj].YRobs);
            ++nblistnotnul;
         il semble qu'il manquait les parentheses */
			   if (YTabTraj[witraj].YRobs->fils != NULL)
         {  Yobs_reverse_time_list_traj(YTabTraj[witraj].YRobs);
            ++nblistnotnul;
				 }
      }
   }
   return (nblistnotnul);
}


/*===========================================================================*/
template<class TypReal>void Yiorwf (char *nmmod, int w4, int w1, int w2, int w3, int wt, TypReal *valout, TypReal valin)
{								/* I/O read write fonction*/
	int	ww4, ww1, ww2, ww3, wwt;													/* pour l'extérieur, */
	ww4=w4+1; ww1=w1+1; ww2=w2+1; ww3=w3+1; wwt=wt+1;			/* on incremente pour l'exterieur */
  //printf("Yiorw: %i  %i %i %i  %i   %e %e \n", w4, w1, w2, w3, wt, *valout, valin);

	if (YioWrite) /* ECRITURE (SORTIE) SAVE */
	{
		if (!select_io(YIO_SAVESTATE, nmmod, w4, w1, w2, w3, wt, valout))
		{		return;
		}
		++YioSelectedCtr;
											/* s t i [j [k]] v, wb: ecriture binaire */
		if 			(YioBin)
		{			  if (!YioState) fwrite(&ww4, Yszint, 1, Yiofp);		  /* s */
						if (YioTime)   fwrite(&wwt, Yszint, 1, Yiofp);		  /* t */
						if (YioAxes)  {fwrite(&ww1, Yszint, 1, Yiofp);		  /* i */
						if (Ydmod>1)   fwrite(&ww2, Yszint, 1, Yiofp);		  /* j */
						if (Ydmod>2)   fwrite(&ww3, Yszint, 1, Yiofp);}		  /* k */
												   //fwrite(&valout, Yszreal,1, Yiofp); /* v */
												   fwrite(valout, Yszreal,1, Yiofp); 	  /* v */
		}
											 /* s t i [j [k]] v, w : ecriture ascii */
		else if (YioAscii)
		{				if (!YioState) fprintf(Yiofp, "%i " ,  ww4);			  /* s */
						if (YioTime)   fprintf(Yiofp, "%i " ,  wwt);			  /* t */
						if (YioAxes)  {fprintf(Yiofp, "%i " ,  ww1);			  /* i */
						if (Ydmod>1)   fprintf(Yiofp, "%i " ,  ww2);			  /* j */
						if (Ydmod>2)   fprintf(Yiofp, "%i " ,  ww3);}			  /* k */
						if (Yszreal==sizeof(float))								  			  /* v */
						   fprintf(Yiofp, "%e", *valout);   							  /* v */
						else /* double */								  			  					/* v */
						   fprintf(Yiofp, "% 23.15e", *valout);   				  /* v */

            /* choix de la chaine de separation d'io (modifiable par set_iosep; default is '\n'*/
            fprintf(Yiofp, "%s", YioSep);
		}				
	}
	
	if (YioRead) /* LECTURE (ENTREE) LAOD */
	{
			*valout = valin;  //*valout = (YREAL) valin;
	} 		
}

/*===========================================================================*/
//int Yio_lspace (char *nmmod, char *oaxis, int pdt, int state)		
int Yio_lspace (char *nmmod, char *odaxe, int pdt, int state)		
{								/* Boucle sur tout l'espace */
	  char oaxis[3+1];

		int   yp1, yp2, yp3; 				/* pour le parcours de l'espace selon oaxis */			
		int		imod;		
		int 	WA1, WA2, WA3;/*Pour eviter le warning "division by zero" a la compil !? */
		imod  = Yimod(nmmod); /* imod = indice du module nmmod dans le tableau YTabMod */			
		WA1 = YTabMod[imod].axi; //was YA1
		WA2 = YTabMod[imod].axj; //was YA2
		WA3 = YTabMod[imod].axk; //was YA3	

		/*--- because multi ----*/
	  //on aligne d'office oaxis sur le dim du module ...
	  strcpy(oaxis, odaxe);
	  if (Ydmod<3) oaxis[2]='\0';
	  if (Ydmod<2) oaxis[1]='\0';
	  /*----- fin multi ------*/
			
		if (!YsetPaxe (Ydmod, oaxis, WA1, WA2, WA3, &yp1, &yp2, &yp3))
		{	 printf("load: Error on oaxis parameter #1\n");
			 return(0);
		}
	
		if 		  (Ydmod==3)
		{		for (int wp1=0; wp1<yp1; ++wp1)
					for (int wp2=0; wp2<yp2; ++wp2)
						for (int wp3=0; wp3<yp3; ++wp3)
						{
								if  		(strcmp(oaxis, "ijk") == 0 || strcmp(oaxis, "IJK") == 0)
										Yio_mod (nmmod, state, wp1, wp2, wp3, pdt, 0.0);																				
								else if (strcmp(oaxis, "ikj") == 0 || strcmp(oaxis, "IKJ") == 0)
										Yio_mod (nmmod, state, wp1, wp3, wp2, pdt, 0.0);									
								else if (strcmp(oaxis, "jik") == 0 || strcmp(oaxis, "JIK") == 0)
										Yio_mod (nmmod, state, wp2, wp1, wp3, pdt, 0.0);																		
								else if (strcmp(oaxis, "jki") == 0 || strcmp(oaxis, "JKI") == 0)
										Yio_mod (nmmod, state, wp3, wp1, wp2, pdt, 0.0);									
								else if (strcmp(oaxis, "kij") == 0 || strcmp(oaxis, "KIJ") == 0)
										Yio_mod (nmmod, state, wp2, wp3, wp1, pdt, 0.0);								
								else if (strcmp(oaxis, "kji") == 0 || strcmp(oaxis, "KJI") == 0)
										Yio_mod (nmmod, state, wp3, wp2, wp1, pdt, 0.0);
								else
								{  	printf ("fio: bad oaxis value (dimod=%i oaxis=%s)\n", Ydmod, oaxis);
										return (0);
								}																
						}			
		}
		else if (Ydmod==2)
		{		for (int wp1=0; wp1<yp1; ++wp1)
					for (int wp2=0; wp2<yp2; ++wp2)
					{		
							if  		(strcmp(oaxis, "ij") == 0 || strcmp(oaxis, "IJ") == 0)
									Yio_mod (nmmod, state, wp1, wp2, 0, pdt, 0.0);														
							else if (strcmp(oaxis, "ik") == 0 || strcmp(oaxis, "IK") == 0)
									Yio_mod (nmmod, state, wp1, 0, wp2, pdt, 0.0);																	
							else if (strcmp(oaxis, "ji") == 0 || strcmp(oaxis, "JI") == 0)
									Yio_mod (nmmod, state, wp2, wp1, 0, pdt, 0.0);															
							else if (strcmp(oaxis, "jk") == 0 || strcmp(oaxis, "JK") == 0)
									Yio_mod (nmmod, state, 0, wp1, wp2, pdt, 0.0);
							else if (strcmp(oaxis, "ki") == 0 || strcmp(oaxis, "KI") == 0)
									Yio_mod (nmmod, state, wp2, 0, wp1, pdt, 0.0);								
							else if (strcmp(oaxis, "kj") == 0 || strcmp(oaxis, "KJ") == 0)
									Yio_mod (nmmod, state, 0, wp2, wp1, pdt, 0.0);
							else
							{ 	printf ("fio: bad oaxis value (dimod=%i oaxis=%s)\n", Ydmod, oaxis);
									return (0);
							}																																									
					}
		}
		else  /* Ydmod=1 */
		{		for (int wp1=0; wp1<yp1; ++wp1)
				{
							if  		(strcmp(oaxis, "i") == 0 || strcmp(oaxis, "I") == 0)
									Yio_mod (nmmod, state, wp1, 0, 0, pdt, 0.0);					
							else if (strcmp(oaxis, "j") == 0 || strcmp(oaxis, "J") == 0)
									Yio_mod (nmmod, state, 0, wp1, 0, pdt, 0.0);					
							else if (strcmp(oaxis, "k") == 0 || strcmp(oaxis, "K") == 0)
									Yio_mod (nmmod, state, 0, 0, wp1, pdt, 0.0);					
							else
							{ 	printf ("fio: bad oaxis value (dimod=%i oaxis=%s)\n", Ydmod, oaxis);
									return (0);
							}																
				}				
		}

		return (1);	/*ok*/
}

/*===========================================================================*/
int Yio_lstate (char *nmmod, char *oaxis, int pdt)
{ 								/* Boucle ou pas sur toutes les sorties */
		int	imod, nbs, codret;

		imod  = Yimod(nmmod);					
		nbs   = YTabMod[imod].nb_stout;		/* on recupere le nombre de sorties du dit module */
		if (YioState>nbs)
		{	 printf("fio_lstate: required state %i, the module have only %i\n", YioState, nbs);
			 return(0);
		}	

		if (YioState == 0) /* on veut toutes les sorties (state) */
		{	 for (int Yw4=0; Yw4<nbs; ++Yw4)
			 {	 codret = Yio_lspace (nmmod, oaxis, pdt, Yw4);
			 		 if (codret==0) return(0);
			 }
		}
		else /* on ne veut que la sortie demandee */
		{		codret = Yio_lspace (nmmod, oaxis, pdt, (YioState-1));
				if (codret==0) return(0);
		}

		return(1);
}

/*===========================================================================*/
int Yio_ltime (char *nmmod, char *oaxis, int pdt)
{								/* Boucle ou pas sur les pas de temps */
    int		imod, nbt, codret;
    int		reste;
    int   itraj, nballtime;

		imod  = Yimod(nmmod);					/* imod = indice du module nmmod dans le tableau YTabMod */
		nbt	= YTabMod[imod].nb_time;	/* on recupere le nombre de pas de temps du dit module */
		if (pdt>nbt)
		{	 printf("fio: step time required (%i) greater the module one (%i)\n", pdt, nbt);
			 return(0);
		}
		
		/*multi : ALLTIME par trajectoire*/
		itraj = Yitrajimod(imod);
		nballtime = YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime;

		if      (nbt>0 && pdt==0)	/* module temporise dont on veut tous les pas de temps */
		{	 for (int wt=0; wt<nballtime; ++wt)			//for (int wt=0; wt<YNBALLTIME; ++wt)
			 {	 codret = Yio_lstate (nmmod, oaxis, wt);
			  	 if (codret==0) return(0);
			 }
		}
		else if (nbt>0 && YioModulot)	/* module temporise dont on veut le 1er pdt, les pdt       */				
		{															/* modulo pdt et le dernier pdt (en memoire de xfoutstate) */
			for (int wt=0; wt<nballtime; ++wt)  //for (int wt=0; wt<YNBALLTIME; ++wt)
			 {	reste=(wt+1)%pdt;
					if (wt==0 || reste == 0 || wt==nballtime-1)  //if (wt==0 || reste == 0 || wt==YNBALLTIME-1)
					{	  //if (YEcho) printf("[%i]\n", wt+1);
			 		 		codret = Yio_lstate (nmmod, oaxis, wt);
			  	 		if (codret==0) return(0);
			  	}
			 }
		}
		else	/* => il s'agit soit d'un module non temporise ou d'un module temporise dont
		            on veut 1 seul pas de temps */
		{	 if  (nbt==0) codret = Yio_lstate (nmmod, oaxis, 0);
			 else      	  codret = Yio_lstate (nmmod, oaxis, (pdt-1));
			 if (codret==0) return(0);
		}

		return (1);
}

/*===========================================================================*/
int Yio_start_file (char *filename)
{
		/* verif nom du fichier a ecrire ou lire */		
		if (strlen(filename)<=0 || filename==NULL)	
		{	 printf("fio: file name missing \n");
			 return(0);
		}		
		/* ouverture du fichier */  		
		if      (YioWrite)
		{	 if			 (YioAscii)
			 {	if (strcmp(filename, "stdout")==0)
			 				Yiofp=stdout;
			 		else
			 				Yiofp = fopen(filename, "w");	
			 }
			 else if (YioBin)   Yiofp = fopen(filename, "wb");
		}
		else if (YioRead)
		{ if      (YioAscii) Yiofp = fopen(filename, "r");
			else if (YioBin)   Yiofp = fopen(filename, "rb");
		}
		if (Yiofp <= 0)
		{ printf ("fio: file %s : open failed \n", filename);
			return (0);
		}
		return (1);
}
/*-------------------------*/
int Yio_start (char *nmmod, char *oaxis, int pdt, char *filename)				
{								/* IO start : Verif, Init, Open pour les i/o ... */
		int		imod;

    /* printf("nmmod=%s oaxis=%s t=%i filename=%s \n", nmmod, oaxis, t, filename); */
    /* verif si module particulier */
		if (strcmp(nmmod, "*"))
		{  imod  = Yimod(nmmod);						/* imod = indice du module nmmod dans le tableau YTabMod */	
		   if (imod<0 || imod>=YNBMODUL)		/* verif existance du module */
		   {	printf("fio: module %s not found \n", nmmod);
			    return(0);
		   }
		   Ydmod = YTabMod[imod].dim;				/* on positionne en variable globale la dim du module */
		}
		
		return(Yio_start_file(filename));
}

/*===========================================================================*/
int Yio_end()
{		if (Yiofp!=stdout) fclose (Yiofp);
		return(1);	
}

/*===========================================================================*/
int Yio_savestate (char *objname, char *oaxis, int pdt, char *filename)
{   int witraj, wjtraj, wispace, wimod, wpdt, nballtime;
    int ctrselall, ctrseltraj, ctrselspace;
    int wipdtraj;

		if (Yio_start_file(filename)==0)
		{  printf("savestate: io failed on file %s\n", filename);
			 return(0);
		}

		if (strcmp(objname, "*")==0) //syntax: '*' ...
		{  //boucles sur tous les modules en acceptant que ceux qui sont compatibles
		   //avec le pdt passed en parametre
		   ctrselall = 0;
		   for (wimod=0; wimod<YNBMODUL; ++wimod)	 // QUE dans l'ordre de la table des modules !?
		   {   witraj = Yitrajimod(wimod);
		     nballtime = YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime;
		     if (pdt > nballtime)
		     {  printf("savestate: modul %s lower of step required: not treated\n", YTabMod[wimod].Name);
		       continue;
		     }
		     if (YioState!=0) //si on a demandee un n° de sortie particulier, on ne sauvegarde que
		     {  if (YioState>YTabMod[wimod].nb_stout) //si le module a une sortie qui correspond
		       {  printf("savestate: modul %s out number too big: not treated\n", YTabMod[wimod].Name);
			 continue;
		       }
		     }
		     YioSelectedCtr=0;
		     Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */
		     wpdt = pdt;												     /* pour les modules non tempo,  */
		     if (YTabMod[wimod].nb_time==0) wpdt=0;  /* on force un pas de temps nul */
		     if (Yio_ltime (YTabMod[wimod].Name, oaxis, wpdt)) /* boucle sur les pas de temps pour le module */
		     {	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, YTabMod[wimod].Name);
		     }
		     ctrselall += YioSelectedCtr;
		   }
		   printf(" %i state(s) selected saving '*'\n", ctrselall);
		}
		else if (objname[0]=='*') //syntaxe '*trajname' ...
		{  witraj = Yitraj(objname+1);
		   if (witraj<0)
			 {  printf("savestate: unknown trajectory %s\n", objname+1);
					return(0);
			 }
			 nballtime = YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime;
			 if (pdt < 0 || pdt > nballtime)
			 {  printf("savestate: out of time, must be 0 (for all) or in [1, %i]\n", nballtime);
			    return(0);
			 }
		   //modules de toutes les trajectoires en alignant le pdt sur celui de trajname
		   //(sauf si pdt==0 car alors, on veut tous les pdt)
		   //(nb: les outers ne seront pas pris car il ne sont pas sur une trajectoire -> plus vrai: y'a plus d'outer)
			 //on fait ca dans l'ordre des trajectoires puis des modules (mais pas des espaces !?)
			 ctrselall = 0;
			 for (wjtraj=0; wjtraj<YNBTRAJ; ++wjtraj)
			 {
			     if (pdt==0) wipdtraj = 0;
				   else //traduire le pas de temps d'une trajectoire a l'autre (...)
				   {  wipdtraj = Ypdt2pdt (wjtraj, pdt, witraj); //je ne suis pas encore bien sur de cette fonction ...
							//printf("%i %i %i -> %i \n", wjtraj, pdt, witraj, wipdtraj);
				   }

				   /* quid si lower ...!? */
			     nballtime = YTabTraj[wjtraj].nbuptime + YTabTraj[wjtraj].nbsteptime;
				   if (wipdtraj > nballtime)
				   {  printf("savestate: trajectory %s lower of step required: not treated\n", YTabTraj[wjtraj].name);
				      continue;  // on ne prend pas !
				      //wpdt = nballtime; si lower, on pourrait choisir de se limite au max !?
				   }

			     ctrseltraj = 0;
					 for (wimod=0; wimod<YNBMODUL; ++wimod)	 //ordre de la table des modules !
			 		 {  if (Yitrajimod(wimod)==wjtraj)
			 		    {
           			 if (YioState!=0) //si on a demandee un n° de sortie particulier, on ne sauvegarde que
           			 {  if (YioState>YTabMod[wimod].nb_stout) //si le module a une sortie qui correspond
						  			{  printf("savestate: modul %s out number too big: not treated\n", YTabMod[wimod].Name);
			           			 continue;
										 }
           			 }

			 		       wpdt = wipdtraj;
                 if (YTabMod[wimod].nb_time==0) wpdt=0;  /* pour les modules non tempo on force un pas de temps nul */

			 		       YioSelectedCtr=0;
			 		    	 Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */
					       if (Yio_ltime (YTabMod[wimod].Name, oaxis, wpdt))	/* boucle sur les pas de temps pour le module */
			 	         {	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, YTabMod[wimod].Name);
			 	         }
			 	         ctrseltraj += YioSelectedCtr;
			 	      }
			 	   }
					 printf(" %i state(s) selected saving trajectory %s\n", ctrseltraj, YTabTraj[wjtraj].name);
			 	   ctrselall += ctrseltraj;
			 }
			 printf(" %i state(s) selected saving all trajectory\n", ctrselall);
		}
		else
		{		if      ((witraj=Yitraj(objname)) >= 0)
		    {  //il s'agit d'une TRAJECTOIRE dont on veut tous les modules.
				   nballtime = YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime;
				   if (pdt < 0 || pdt > nballtime)
				   {  printf("savestate: out of time, must be 0 (for all) or in [1, %i]\n", nballtime);
			        return(0);
				   }
				   ctrseltraj = 0;
				   for (wispace=0; wispace<YNBSPACE; ++wispace)	 // dans l'ordre des espaces, puis des modules !
			 		 {  if (strcmp(objname, YTabSpace[wispace].traj_name) == 0)
			 		    {  //l'espace appartient a la trajectoire, donc on le traite.
					 			 ctrselspace = 0;
					 			 for (wimod=0; wimod<YNBMODUL; ++wimod)	 // PUIS dans l'ordre de la table des modules !
			 			 		 {   if (strcmp(YTabSpace[wispace].name, YTabMod[wimod].space_name) == 0)
			 		    			 {  //le module appartient a l'espace, donc on le traite.
           			 			  if (YioState!=0) //si on a demandee un n° de sortie particulier, on ne sauvegarde que
           			 				{  if (YioState>YTabMod[wimod].nb_stout) //si le module a une sortie qui correspond
						  						 {  printf("savestate: modul %s out number too big: not treated\n", YTabMod[wimod].Name);
			           			 				continue;
										 			 }
           			 				}
			 		   				    YioSelectedCtr=0;
			 		   				    Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */
			 		   				    wpdt = pdt;												      /* pour les modules non tempo,  */
						 				    if (YTabMod[wimod].nb_time==0) wpdt=0;  /* on force un pas de temps nul */
												if (Yio_ltime (YTabMod[wimod].Name, oaxis, wpdt))	/* boucle sur les pas de temps pour le module */
			 	   							{	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, YTabMod[wimod].Name);
			 	     				    }
			 	     				    ctrselspace += YioSelectedCtr;
			 								}
			 					 }
			 					 printf(" %i state(s) selected saving space %s\n", ctrselspace, YTabSpace[wispace].name);
			 					 ctrseltraj += ctrselspace;
			 		    }
			 		 }
			 		 printf(" %i state(s) selected saving trajectoty %s\n", ctrseltraj, objname);
		    }
		    else if ((wispace=Yispace(objname)) >= 0)
		    {  //il s'agit d'un ESPACE dont on veut tous les modules.
		       witraj = Yitraj(YTabSpace[wispace].traj_name);
				   nballtime = YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime;
				   if (pdt < 0 || pdt > nballtime)
				   {  printf("savestate: out of time, must be 0 (for all) or in [1, %i]\n", nballtime);
			        return(0);
				   }
				   ctrselspace = 0;
					 for (wimod=0; wimod<YNBMODUL; ++wimod)	 // dans l'ordre de la table des modules !
			 		 {  if (strcmp(objname, YTabMod[wimod].space_name) == 0)
			 		    {  //le module appartient a l'espace, donc on le traite.
           			 if (YioState!=0) //si on a demandee un n° de sortie particulier, on ne sauvegarde que
           			 {  if (YioState>YTabMod[wimod].nb_stout) //si le module a une sortie qui correspond
						  		  {  printf("savestate: modul %s out number too big: not treated\n", YTabMod[wimod].Name);
			           		   continue;
										}
           			 }
			 		       YioSelectedCtr=0;
			 		       Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */
			 		       wpdt = pdt;												  /* pour les modules non tempo,  */
						     if (YTabMod[wimod].nb_time==0) wpdt=0;  /* on force un pas de temps nul */
								 if (Yio_ltime (YTabMod[wimod].Name, oaxis, wpdt))	/* boucle sur les pas de temps pour le module */
			 	   			 {	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, YTabMod[wimod].Name);
			 	         }
			 	         ctrselspace += YioSelectedCtr;
			 				}
			 		 }
			 		 printf(" %i state(s) selected saving space %s\n", ctrselspace, objname);
		    }
		    else if ((wispace=Yiopera(objname)) >= 0)
		    {  //il s'agit d'un OPERATEUR dont on veut tous les modules.
				   if (pdt != 0)
				   {  printf("savestate: for an operator, steptime (pdt) must be 0 \n");
			        return(0);
				   }
				   ctrselspace = 0;
					 for (wimod=0; wimod<YNBMODUL; ++wimod)	 // dans l'ordre de la table des modules !
			 		 {  if (strcmp(objname, YTabMod[wimod].space_name) == 0)
			 		    {  //le module appartient a l'operateur, donc on le traite.
           			 if (YioState!=0) //si on a demandee un n° de sortie particulier, on ne sauvegarde que
           			 {  if (YioState>YTabMod[wimod].nb_stout) //si le module a une sortie qui correspond
						  		  {  printf("savestate: modul %s out number too big: not treated\n", YTabMod[wimod].Name);
			           		   continue;
										}
           			 }
			 		       YioSelectedCtr=0;
			 		       Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */
			 		       wpdt = pdt;												  /* pour les modules non tempo,  */
						     if (YTabMod[wimod].nb_time==0) wpdt=0;  /* on force un pas de temps nul */
								 if (Yio_ltime (YTabMod[wimod].Name, oaxis, wpdt))	/* boucle sur les pas de temps pour le module */
			 	   			 {	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, YTabMod[wimod].Name);
			 	         }
			 	         ctrselspace += YioSelectedCtr;
			 				}
			 		 }
			 		 printf(" %i state(s) selected saving operator %s\n", ctrselspace, objname);
		    }
		    else if ((wimod=Yimod(objname)) >= 0)
		    {  //il s'agit d'un seul MODULE.
				   witraj = Yitrajimod(wimod);
				   nballtime = YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime;
				   if (pdt < 0 || pdt > nballtime)
				   {  printf("savestate: out of time, must be 0 (for all) or in [1, %i]\n", nballtime);
			        return(0);				
				   }
				   YioSelectedCtr=0;
		       Ydmod = YTabMod[wimod].dim; /* dim du module en variable globale */							 						
			 	   if (Yio_ltime (objname, oaxis, pdt))		/* boucle sur les pas de temps pour le module */
			 	   {	 printf(" %i state(s) selected saving module %s\n", YioSelectedCtr, objname);
			 	   }
		    }
		    else		
			  {  printf("savestate: unknown object name %s\n", objname);
					 return(0);
			  }		
		}
		fflush(Yiofp);
		return(Yio_end());
}

/*===========================================================================*/
//int Yio_load (YioKind yiokind, char *modname, char *oaxis, int pdt, char *filename)
int Yio_load (YioKind yiokind, char *modname, char *odaxe, int pdt, char *filename)
{
		int    imod;
		char   nmmod[LG_MAX_NAME+1];
		double wdouble;
		int    nbt;

		/*struct Yst_obs *aobs;	un pointeur pour une observation */	/* maintenant: la globale Yaobs*/
		int	   wws, wp1, wp2, wp3, wwt;											/* de l'exterieur, */
		int	        ww1, ww2, ww3;													/* avec decalage   */
		int					wwi, wwj, wwk;							   /* a l'interieur (re-cale)  */
		int    previous_wws=-1;		    /* pour detecter un changement de sortie pour la re-initialisation
		                                 des axes lorsque ceux-ci doivent etre déduit (!YioAxe) */
		double vald; /*was: YREAL  val; but ... */
		float  valf;
		YREAL  val;
		int		 nbitem, nb_lus, nbelt;
		int 	 WA1, WA2, WA3;	/*Pour eviter le warning "division by zero" a la compil !*/
		short  selected;
		int		 nb_enr_mod=0;  /* nb enr a lire du module */
		int    nb_lus_mod=0;  /* nb enr lu     du module */
		int		 nb_enr_smod=0; /* nb enr a lire pour une sortie du module */
		int    nb_lus_smod=0; /* nb enr lu     pour la  sortie courante du module */
		int    nb_sel_mod=0;
		int    all=0;

		int    itraj, nballtime;		/* multi */
		char   oaxis[3+1];					/* multi */
	  strcpy(oaxis, odaxe); 			/* multi */

    /* printf("nmmod=%s oaxis=%s t=%i filename=%s \n", nmmod, oaxis, t, filename); */

		/* positionnement pour le decalage
		WA1 = YioDi;   because
		WA2 = YioDj;    multi
		WA3 = YioDk;  cf ci-apres */

		if (!strcmp(modname, "*")) // cas: TOUS les modules
		{ /* dans ce cas, le fichier en entree est supposed contenir les donnees (toutes les sorties)
		     de tous les modules sur tout l'espace au temps pdt ou tous les pdt (et forcement 0 pour les non tempo)
		     dans l'ordre de leur apparition dans la table des modules
		     On ne traite que du cas de LOADSTATE et pas de LOADOBS */
				 if (yiokind==YIO_LOADOBS)
			   {	printf("star(*) usage is not availuable for loadobs\n");
				    return(0);
			   }

			   /* init cas all */
		     all=1;
		     imod = -1;
		     nb_enr_mod  = nb_lus_mod  - 1;
		     nb_enr_smod = nb_lus_smod - 1;
		}
		else										// cas : UN SEUL module
		{		 strcpy(nmmod, modname);
				 imod  = Yimod(nmmod); /* imod = indice du module nmmod dans le tableau YTabMod */

 				 /* multi */
 				 itraj = Yitrajimod(imod);
 				 nballtime = YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime;
 				 if (pdt < 0 || pdt > nballtime)
			   {  printf("load: out of time, must be 0 (for all) or in [1, %i]\n", nballtime);
			      return(0);
			   }

			   	nbt	= YTabMod[imod].nb_time;	/* on recupere le nombre de pas de temps du dit module */
		     //mais, pour un modul associed a un operateur, c'est le temps de la trajectoire de laquelle
		     //depend l'operateur qu'il faut utiliser !?
		     if (Yioperaimod(imod)>=0) 								 										 
		        nbt = YTabTraj[itraj].nbuptime+YTabTraj[itraj].nbsteptime;
		        		   
				 /* pour un module non tempo, on verifie que pdt==0 et on positione d'office
				    YioTime a 0 car on ne doit pas avoir de pdt dans le fichier */
				 //if (YTabMod[imod].nb_time==0)
				 if (nbt==0)
				 {  if (pdt != 0)
				 	  {	printf("module %s is not tempo, step time must be 0\n", nmmod);
		  		    return(0);
		  		  }
				    YioTime=OFF;
				 }
				 

				 /* pour LOADOBS, on verifie que le module est de type cout */
				 if (yiokind==YIO_LOADOBS)
				 {	if (!YTabMod[imod].is_cout)
					  {	 printf("%s modul is not a 'cout' one\n", nmmod);
				 		 	 return(0);
					  }
		  			YioInsertObsCtr=YioOvrObsCtr=0;
				 }
 				 //ww1=ww2=ww3=-1; wp1=wp2=wp3=-1; no more convenient here; see further previous_wws				
		}
		
		/* autres verif (nb: here, modname may still be '*' ) */
		if (!Yio_start(modname, oaxis, pdt, filename))	/* Verif, Init, Open, ... */
				return(0);

		/*...............................................................*/
		
		YioSelectedCtr=0;
		nbitem=99; nb_lus=0;	
		while (nbitem>0) /* tq pas fin de fichier ou probleme */
		{		
			if (all) //cas de all ('*') (possible que pour loadstate, pas pour loadobs cf^)
			{ 				
				 if (nb_lus_smod >= nb_enr_smod) //A faire si changement de sortie du module
				 {  nb_lus_smod= 0;							 //(ce n'est plus tres utile, ... remplaced par
				 }															 // l'utilisation de previous_wws)
 								
				 if(nb_lus_mod >= nb_enr_mod) //A faire si changement de module
				 {
					 if (imod>=0)
					 		printf("A:modul %s: %i data read ; %i selected \n", nmmod, nb_lus_mod, nb_sel_mod);
									
					 //++imod; /* increment de l'indice du module dans le tableau YTabMod pour passer au suivant */
					 /* multi : il faut incrementer jusqu'a tomber sur un module ok par rapport au pdt
					            ou qu'on soit en fin du tableau des modules */
					 while (1)
 					 {  ++imod;
 					    if (imod >= YNBMODUL) break; 					
 					    itraj = Yitrajimod(imod);
 					    nballtime = YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime;
 					    if (pdt < 0 || pdt > nballtime)
			           printf("load: modul %s lower of step required: not treated\n", YTabMod[imod].Name);					
			        else
			           break;
			     }
			     if (imod >= YNBMODUL) break;		 					

					 Ydmod = YTabMod[imod].dim; /* dim du module en variable globale */
					 strcpy(nmmod, YTabMod[imod].Name);
										
	         //because multi: on aligne d'office oaxis sur le dim du module.
	         //strcpy(oaxis, odaxe);
	         if (Ydmod<3) oaxis[2]='\0';
	         if (Ydmod<2) oaxis[1]='\0';
										
					 nb_lus_mod = 0;	/* re-init du nombre d'enregistrement lus du module */
					 nb_sel_mod = 0;	/* re-init du nombre de selectionned pour le module */
 					 previous_wws=-1; /* re-init detection d'un changement de sortie */
					
					 /* determination du nombre (supposed) d'enregistrements du module */
					 nb_enr_smod = YTabMod[imod].axi;
					 if (YTabMod[imod].dim > 1) nb_enr_smod = nb_enr_smod * YTabMod[imod].axj;
					 if (YTabMod[imod].dim > 2) nb_enr_smod = nb_enr_smod * YTabMod[imod].axk;
					
					 if (pdt == 0 && YTabMod[imod].nb_time > 0) //=>tous les pdt
					 { 	nb_enr_smod = nb_enr_smod * YTabMod[imod].nb_time;
					 }
					
					 if (YioState==0) /* si 0 alors tous les states sinon, que celle demandee */
					 {  nb_enr_mod = nb_enr_smod * YTabMod[imod].nb_stout;
					 }
					 else
					 		nb_enr_mod = nb_enr_smod;					
				 } //fin changement (rupture) de module
			} //fin if (all)
			
		  /* multi : positionnement pour le decalage si pas indiqued par le user et selon la dim du module */
		  WA1=WA2=WA3=0; //ARAR
		  if (YioDi==0) WA1 = YTabMod[imod].axi;
		  if (YioDj==0) WA2 = YTabMod[imod].axj;
		  if (YioDk==0) WA3 = YTabMod[imod].axk;		
		
			/*---------------*/	
			nbelt = 0;
											 /* s t i [j [k]] v, rb: lecture binaire */
			if 		(YioBin)
			{			if (!YioState)																			/* s */
						{								nbitem=fread(&wws, Yszint, 1, Yiofp);
														if (nbitem==1) ++nbelt;
						}
						else {wws = YioState; ++nbelt;}
						if (YioTime)																				/* t */
						{								nbitem=fread(&wwt, Yszint, 1, Yiofp);
														if (nbitem==1) ++nbelt;
						}
						else {wwt = pdt; ++nbelt;}
						if (YioAxes)																									/* else: cf !v */
						{								nbitem=fread(&wp1, Yszint, 1, Yiofp);	/* i */
														if (nbitem==1) ++nbelt;
							if (Ydmod>1) {nbitem=fread(&wp2, Yszint, 1, Yiofp);	/* j */
													  if (nbitem==1) ++nbelt;}
							if (Ydmod>2) {nbitem=fread(&wp3, Yszint, 1, Yiofp);	/* k */
														if (nbitem==1) ++nbelt;}
						}
						if (YioszReal==sizeof(float))
						{	 nbitem=fread(&valf, YioszReal,1, Yiofp);
							 val  = (YREAL) valf;
						}
						else
						{	 nbitem=fread(&vald, YioszReal,1, Yiofp);
							 val  = (YREAL) vald;
						}
						if (nbitem==1) ++nbelt;
						else break;
			}
			else 	if (YioAscii)
			{			if (!YioState)																			/* s */
						{								//nbitem=fscanf(Yiofp, "%i", &wws);
														nbitem=fscanf(Yiofp, "%lf", &wdouble);
														wws = int(wdouble);									
														if (nbitem==1) ++nbelt;
						}
						else {wws = YioState; ++nbelt;}

						if (YioTime)																					/* t */
						{								//nbitem=fscanf(Yiofp, "%i", &wwt);
														nbitem=fscanf(Yiofp, "%lf", &wdouble);
														wwt = int(wdouble);									
														if (nbitem==1) ++nbelt;
						}
						else {wwt = pdt; ++nbelt;}

						if (YioAxes)																									/* else: cf !v */
						{								//nbitem=fscanf(Yiofp, "%i", &wp1);			/* i */
														nbitem=fscanf(Yiofp, "%lf", &wdouble);
														wp1 = int(wdouble);									
														if (nbitem==1) ++nbelt;
							if (Ydmod>1) {//nbitem=fscanf(Yiofp, "%i", &wp2);			/* j */
														nbitem=fscanf(Yiofp, "%lf", &wdouble);
														wp2 = int(wdouble);									
													  if (nbitem==1) ++nbelt;
													 }
							if (Ydmod>2) {//nbitem=fscanf(Yiofp, "%i", &wp3);			/* k */
														nbitem=fscanf(Yiofp, "%lf", &wdouble);
														wp3 = int(wdouble);									
														if (nbitem==1) ++nbelt;
													 }
						}

						if (YioszReal==sizeof(float))
						{	 nbitem=fscanf(Yiofp, "%f", &valf);
							 val  = (YREAL) valf;
						}
						else
						{	 nbitem=fscanf(Yiofp, "%lf", &vald);
							 //nbitem=fscanf(Yiofp, "%le", &vald); ?
							 val  = (YREAL) vald;
						}

						if (nbitem==1) ++nbelt;
            else break;
			}
		
			if (nbelt>0)
			{
				 --wws; --wwt;			   /* decrement pour passer aux indices internes */
				 /*------------------------------------------------------------------*/
				 /* Positionnement des indices des axes de l'ordre externe vers
				    l'ordre interne                                                  */
				 if (YioAxes)
				 {	/* cas ou les indices des axes ont ete recuperes dans le fichier */
				 		if (!YsetPaxe (Ydmod, oaxis, wp1, wp2, wp3, &ww1, &ww2, &ww3))
				 		{	 printf("load : error on oaxis parameter #2\n");
							 return(0);
						}

						/* decrement pour passer aux l'indices internes */
					  --ww1;
					  if (Ydmod>1) 	--ww2;
					  if (Ydmod>2) 	--ww3;

					 	//printf("%i %i %i %i %i %f\n", wws, wwt, wp1, wp2, wp3, val);

				 }
				 else	/*(!YioAxes):	Les indices des axes ne sont pas dans le fichier */
				 {		/* 	=> il faut les deduire en interne.(YsetPaxe ne suffit pas) */
				
				   // detection changement de sortie, ... si oui, il faut (re)initier
				   // les variables d'axe (... on suppose que pour une sortie donnee,
				   // on aura consécutivement toutes ses valeurs sur l'espace)
				   if (wws!=previous_wws)
				   {  ww1=ww2=ww3=-1; 	// (re)init des
					    wp1=wp2=wp3=-1;   // coordonnees spatiales				
				 		  previous_wws = wws;
				   }
				   			 				
					 /* puis ... */
					 if      (Ydmod==1)
					 {	if      (strcmp(oaxis, "i") == 0 || strcmp(oaxis, "I") == 0)
					 			 {ww1=(ww1+1)%WA1;} //{++ww1;}
					 		else if (strcmp(oaxis, "j") == 0 || strcmp(oaxis, "J") == 0)
					 			 {ww2=(ww2+1)%WA2;} //{++ww2;}
					 		else if (strcmp(oaxis, "k") == 0 || strcmp(oaxis, "K") == 0)
					 			 {ww3=(ww3+1)%WA3;} //{++ww3;}
							else
							{	 printf("load : error on oaxis parameter #3 (dim=%i, oaxis=%s)\n", Ydmod, oaxis);
								 return(0);
							}
							nbelt +=1;
					 }
					 else if (Ydmod==2)
					 {	if 			(strcmp(oaxis, "ij") == 0 || strcmp(oaxis, "IJ") == 0)
								 			{ww2=(ww2+1)%WA2;   ww1=(ww1+(ww2==0))%WA1;}
							else if (strcmp(oaxis, "ik") == 0 || strcmp(oaxis, "IK") == 0)
											{ww3=(ww3+1)%WA3; 	ww1=(ww1+(ww3==0))%WA1;}
							else if (strcmp(oaxis, "ji") == 0 || strcmp(oaxis, "JI") == 0)
											{ww1=(ww1+1)%WA1; 	ww2=(ww2+(ww1==0))%WA2;}
							else if (strcmp(oaxis, "jk") == 0 || strcmp(oaxis, "JM") == 0)
											{ww3=(ww3+1)%WA3; 	ww2=(ww2+(ww3==0))%WA2;}
							else if (strcmp(oaxis, "ki") == 0 || strcmp(oaxis, "KI") == 0)
											{ww1=(ww1+1)%WA1; 	ww3=(ww3+(ww1==0))%WA3;}
							else if (strcmp(oaxis, "kj") == 0 || strcmp(oaxis, "KJ") == 0)
											{ww2=(ww2+1)%WA2; 	ww3=(ww3+(ww2==0))%WA3;}
							else
							{	 printf("load : error on oaxis parameter #4 (dim=%i, oaxis=%s)\n", Ydmod, oaxis);
								 return(0);
							}
							nbelt += 2;
					 }
					 else if (Ydmod==3) /* forcement toutes les combi de i, j, k */
					 {	if      (strcmp(oaxis, "ijk") == 0 || strcmp(oaxis, "IJK") == 0)
											{ww3=(ww3+1)%WA3; 	ww2=(ww2+(ww3==0))%WA2; 	ww1=(ww1+((ww2+ww3)==0))%WA1;}
							else if (strcmp(oaxis, "ikj") == 0 || strcmp(oaxis, "IKJ") == 0)
											{ww2=(ww2+1)%WA2; 	ww3=(ww3+(ww2==0))%WA3; 	ww1=(ww1+((ww2+ww3)==0))%WA1;}
							else if (strcmp(oaxis, "jik") == 0 || strcmp(oaxis, "JIK") == 0)
											{ww3=(ww3+1)%WA3; 	ww1=(ww1+(ww3==0))%WA1; 	ww2=(ww2+((ww1+ww3)==0))%WA2;}
							else if (strcmp(oaxis, "jki") == 0 || strcmp(oaxis, "JKI") == 0)
											{ww1=(ww1+1)%WA1; 	ww3=(ww3+(ww1==0))%WA3; 	ww2=(ww2+((ww1+ww3)==0))%WA2;}
							else if (strcmp(oaxis, "kij") == 0 || strcmp(oaxis, "KIJ") == 0)
											{ww2=(ww2+1)%WA2; 	ww1=(ww1+(ww2==0))%WA1; 	ww3=(ww3+((ww1+ww2)==0))%WA3;}
							else if (strcmp(oaxis, "kji") == 0 || strcmp(oaxis, "KJI") == 0)
											{ww1=(ww1+1)%WA1; 	ww2=(ww2+(ww1==0))%WA2; 	ww3=(ww3+((ww1+ww2)==0))%WA3;}
							else
							{	 printf("load : error on oaxis parameter #5 (dim=%i, oaxis=%s)\n", Ydmod, oaxis);
								 return(0);
							}
							nbelt += 3;
							//printf("inY: %i %i %i %i %i %f\n", wws, wwt, ww1, ww2, ww3, val);
					 }
				 }

				 /*------------------------------------------------------------------*/
				 /* verif qu'on a le bon compte d'elements */
				 //printf("dimod=%i nbelt=%i\n", Ydmod, nbelt);
				 if ((Ydmod==1 && nbelt!=4) || (Ydmod==2 && nbelt!=5) || (Ydmod==3 && nbelt!=6))
				 {	printf("load : bad format on data %i (dim=%i, nbelt=%i)\n", nb_lus+1, Ydmod, nbelt);
				 		return(0);
				 }

				 /*------------------------------------------------------------------*/
				 /* prise en compte du decalage, verif fitting the space et selection*/
				 wwi=ww1+Yiodi; wwj=ww2+Yiodj; wwk=ww3+Yiodk;

				 //printf("A) (%i  %i  %i) (%i  %i  %i) (%i  %i  %i) \n", ww1, ww2, ww3,
				 //wwi, wwj, wwk, YTabMod[imod].axi, YTabMod[imod].axj, YTabMod[imod].axk);
				 if ( (YTabMod[imod].nb_time>0 && (wwt<0 || wwt>=YTabMod[imod].nb_time))
				 ||   (                            wwi<0 || wwi>=YTabMod[imod].axi)
				 ||   (Ydmod>1                 && (wwj<0 || wwj>=YTabMod[imod].axj))
			   ||   (Ydmod>2                 && (wwk<0 || wwk>=YTabMod[imod].axk))
			      ) // non selection d'office car l'item n'est pas dans l'espace-temps du module
				 		selected = NON;										
				 else /* sinon: selection ...: le user decide */
				 {	selected = select_io(yiokind, nmmod, wws, wwi, wwj, wwk, wwt, &val);
				 }
				
				 if (selected!=0 && selected!=1)
				 {	printf("load: bad return on select_io (%i), must be 0 or 1\n", selected);
				 		return(0);
				 }
				
				 YioSelectedCtr += selected;
				 nb_sel_mod     += selected;

				 /*------------------------------------------------------------------*/
				 /* cas de loadobs */
				 if (selected && yiokind==YIO_LOADOBS)
				 {	Yaobs.time = wwt;
				 		Yaobs.imod = Yimod(nmmod);
				 		Yaobs.smod = wws;
				 		Yaobs.iaxe = wwi;
				 		Yaobs.jaxe = wwj;
				 		Yaobs.kaxe = wwk;
				 		Yaobs.vobs = val;
						//Yobs_disp(&Yaobs);

						/*WARNING: PLM, Yobs_insert ne gere pas toutes les combinaisons d'axes */
			 		if (!Yobs_insert(&Yaobs)) return(0); /* mais que i ou ij ou ijk ...!    */
				 }

 				 /*------------------------------------------------------------------*/
 				 /* cas de loadstate */
				 if (selected && yiokind==YIO_LOADSTATE)
 				 {  Yio_mod(nmmod, wws, wwi, wwj, wwk, wwt, val);
						//printf("%s:[%i](%i %i %i; %i)=%e \n", nmmod, wws, wwi, wwj, wwk, wwt, val);
 				 }

				 /*------------------------------------------------------------------*/

				 ++nb_lus;
				 ++nb_lus_mod;
				 ++nb_lus_smod;
				
			} //fin du if nbelt>0

		}/*fin du while*/
		if (yiokind==YIO_LOADOBS)
		{		printf("modul %s: %i data read ; %i state(s) selected as obs (%i inserted + %i overrided)\n",
					   nmmod, nb_lus, YioSelectedCtr, YioInsertObsCtr, YioOvrObsCtr);
		}
		else
		{		//printf("B:modul %s: %i data read ; %i selected \n", nmmod, nb_lus_mod, nb_sel_mod);
		    if(all)
		    {  printf("all: %i data read ; %i selected \n", nb_lus, YioSelectedCtr);
		       //printf("last: imod=%i, nb_lus_smod=%i \n", imod, nb_lus_smod);
		       if (imod!=YNBMODUL || nb_lus_smod!=0)
		       {  printf("warning: file may not exactly match ... \n");
		       }
		       else
		       {  printf("load completed\n");
		       }
		    }
		    else
		    {  printf("B:modul %s: %i data read ; %i selected \n", nmmod, nb_lus_mod, nb_sel_mod);
		    }
    }

		/*...............................................................*/
		return(Yio_end ());
}

/*===========================================================================*/
/*YREAL Ygradcost(YREAL vcal, YREAL vobs)
{	 double gradient;
	 // if (lms) //...return(Ygradcost_lms(vcal, vobs));
	 {  gradient = 2 * YCoefGrad * (vcal - vobs);
	    YTotalCost += YCoefGrad * pow(vcal - vobs , 2);
	 }
   //if (mytrc) printf(" %i %i %e  %e  %e  \n", IRobs->iind, YLobs->iind, vcal, vobs, YTotalCost);
	 return(gradient);
} NO MORE */
/*---------------------------------------------------------------------------*/
/* YREAL Ycostonly(YREAL vcal, YREAL vobs)
{	 double quotacost;
	 // if (lms)	//...return(Ygradcost_lms(vcal, vobs));
	 {quotacost = YCoefGrad * pow(vcal - vobs , 2);
	 }
	 YTotalCost += quotacost;
	 return(quotacost);
} NO MORE */
/*---------------------------------------------------------------------------*/
YREAL Ycostdiff(YREAL Dinov, YREAL Dcov)
{	 double gradient;
   //printf("Ycostdiff:(inov, cov)=(%e, %e)\n", Dinov, Dcov);
	 {  gradient = 2 * YCoefGrad * Dcov;
	    YTotalCost += YCoefGrad * Dinov * Dcov;
	 }
	 return(gradient);
}

/*===========================================================================*/
/*
#ifdef YO_VARINCR
YREAL Yc_gradcost(YREAL qtea)
{	 double gradient;
	 // if (lms)  PLM, on ne considere QUE le cas lms
	 {  gradient = 2 * YCoefGrad * qtea;
	    YTotalCost += YCoefGrad * pow(qtea , 2);
	 }
	 return(gradient);
} NO MORE
									//-------------------------------------------//
YREAL Yc_costonly(YREAL grad, YREAL vcal, YREAL vobs)
{	 double qtea, quotacost;
   //if (lms)  PLM, on ne considere QUE le cas lms
	 {  qtea = grad - (vobs - vcal);
	    quotacost =  YCoefGrad * pow(qtea , 2);
	 }
	 YTotalCost += quotacost;
	 return(quotacost);
} NO MORE
#endif
*/
/*===========================================================================*/
void Ycobs1_list(struct Yst_fobs *lobs)
{	/* calcul du cout sur la liste des observations ; cas de la dim 1 */
	if (lobs == NULL) return;
	YLobs = lobs;

	if (YAL2Run==RUNL2_STD)
	{	/*if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Ycobsgrad_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Ycobsonly_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_DIFF)
		{  while (YLobs != NULL)
			 {	 Ywishdiff_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_WISHDIFF)
		{  while (YLobs != NULL)
			 {	 Ycostwishdiff_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt);
					 YLobs = YLobs->frere;
			 }
		}
	}
#ifdef YO_VARINCR
	if (YAL2Run==RUNL2_INCR)
	{	/*if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Yc_cobsgrad_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Yc_cobsonly_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_QTEA)
		{  while (YLobs != NULL)
			 {	 Ywishqtea_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
    else if (YCaseCost==COSTL2_WISHQTEA)
		{  while (YLobs != NULL)
			 {	 Ycostwishqtea_all(YTONamMod, SRobs->iind, YLobs->iind, 0, 0, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
	}
#endif
}
/*---------------------------------------------------------------------------*/
void Ycobs2_list(struct Yst_fobs *lobs)
{	/* calcul du cout sur la liste des observations ; cas de la dim 1 */
	if (lobs == NULL) return;
	YLobs = lobs;
	
	if (YAL2Run==RUNL2_STD)
	{	/*if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Ycobsgrad_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Ycobsonly_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_DIFF)
		{  while (YLobs != NULL)
			 {	 Ywishdiff_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_WISHDIFF)
		{  while (YLobs != NULL)
			 {	 Ycostwishdiff_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt);
					 YLobs = YLobs->frere;
			 }
		}
	}
#ifdef YO_VARINCR
	if (YAL2Run==RUNL2_INCR)
	{	/*if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Yc_cobsgrad_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Yc_cobsonly_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_QTEA)
		{  while (YLobs != NULL)
			 {	 Ywishqtea_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
    else if (YCaseCost==COSTL2_WISHQTEA)
		{  while (YLobs != NULL)
			 {	 Ycostwishqtea_all(YTONamMod, SRobs->iind, IRobs->iind, YLobs->iind, 0, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
	}
#endif
}
/*---------------------------------------------------------------------------*/
void Ycobs3_list(struct Yst_fobs *lobs)
{	/* calcul du cout sur la liste des observations ; cas de la dim 1 */
	if (lobs == NULL) return;
	YLobs = lobs;
	
	if (YAL2Run==RUNL2_STD)
	{	/* if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Ycobsgrad_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Ycobsonly_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_DIFF)
		{  while (YLobs != NULL)
			 {	 Ywishdiff_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_WISHDIFF)
		{  while (YLobs != NULL)
			 {	 Ycostwishdiff_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt);
					 YLobs = YLobs->frere;
			 }
		}
	}
#ifdef YO_VARINCR
	if (YAL2Run==RUNL2_INCR)
	{	/* if (YCaseCost==COSTL2_GRAD)
		{  while (YLobs != NULL)
			 {	 Yc_cobsgrad_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
		else if (YCaseCost==COSTL2_ONLY)
		{  while (YLobs != NULL)
			 {	 Yc_cobsonly_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->vobs);
					 YLobs = YLobs->frere;
			 }
		}
		else */
    if (YCaseCost==WISHL2_QTEA)
		{  while (YLobs != NULL)
			 {	 Ywishqtea_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
    else if (YCaseCost==COSTL2_WISHQTEA)
		{  while (YLobs != NULL)
			 {	 Ycostwishqtea_all(YTONamMod, SRobs->iind, IRobs->iind, JRobs->iind, YLobs->iind, Ytrupdt, YLobs->qtea);
					 YLobs = YLobs->frere;
			 }
		}
	}
#endif
}

/*===========================================================================*/
//void Ycobs_TR_rec(struct Yst_nodo *arbobs){} --> voir V7 si necessaire

//------------------------------------------------------------------------
void Ycobs_TR(struct Yst_nodo *arbobs)
{
    int w1, iopera;
    
		//printf("lev=%i\n", YTOLevel);
		if (arbobs == NULL) return;
		
		/* en entree : *arbobs : pointeur sur le premier module d'un pas de temps */
		/* Au cours de parcourt de l'arborescence, il faut garder le pointeur
		   sur les s/Root courants: TRobs, MRobs, SRobs [, IRobs [, JRobs]]*/
	
		MRobs = arbobs;										/* MRobs */
		while (MRobs!=NULL)		  //liste des modules -------------
		{ YTODimMod = YTabMod[MRobs->iind].dim;					/* la dim du module */		
			strcpy(YTONamMod, YTabMod[MRobs->iind].Name);	/* le nom du module */
	    if(TRobs->iind>=0)													  /* le coef de la fct cout du module : */
      {  YCoefGrad = YTabMod[MRobs->iind].scoef;	  /*    .scoef pour les observations */
			}
	    else
	  	{  YCoefGrad = YTabMod[MRobs->iind].bcoef;    /*    .bcoef pour les ebauches */
			}

	    /* because opera ----- */
      if (YCaseCost==WISHL2_NANACT) //met NAN dans Ywish et active les operateurs du module
      {		Ysetwish_mod(MRobs->iind, log(-1));
          w1=0;
          while (w1<YNBOPERA)
          {   iopera = YTabMocop[MRobs->iind][w1];
              if (iopera<0) break;
              YTabOpera[iopera].isactiv = 1;
              ++w1;
          }         
					MRobs = MRobs->frere;
          continue;								//!!! <- avec NANACT ca s'arrete ici
      } /*-------------------*/
		
		  SRobs = MRobs->fils;						 /* SRobs */
		  while (SRobs!=NULL)		//liste des sorties d'un module -------------
		  {		if     (YTODimMod==1)													               // si 1D:
		         Ycobs1_list( (struct Yst_fobs *) SRobs->fils);            // liste des I
		      else																								         // puis
		      {  IRobs = SRobs->fils;															         // sinon
		      	 while (IRobs!=NULL)															         // si
		      	 {   if(YTODimMod==2)															         // 2D
		      	 		   Ycobs2_list( (struct Yst_fobs *) IRobs->fils);      // liste des J
		      	 		 else //=>YTODimMod==3																 // sinon
		      	 		 {  JRobs = IRobs->fils;															 // c'est
										while (JRobs!=NULL)							                   // 3D
		      	 				{   Ycobs3_list( (struct Yst_fobs *) JRobs->fils); // liste des K	      	 		
		                    JRobs = JRobs->frere;
		      	        }		
								 }
		             IRobs = IRobs->frere;
		      	 }
		      }		
		      SRobs = SRobs->frere;
		  } 		
		  MRobs = MRobs->frere;
		}
}
/*===========================================================================*/


/*=============================================================================
...............................................................................
void Yrovobs(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, YREAL *r1, YREAL *r2), int indic)
void Yrovobs(struct Yst_nodo *traj_robs, void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, YREAL *r1, YREAL *r2),int indic)
	Cette fonction permet de travailler avec l'arborescence des observations d'une trajectoire en la parcourant.
	Elle utilise la fonction Yrovobs1 ou Yrovobs2 ou Yrovobs3 selon la dim du modul.
	Elle recoit en parametre :
		- un pointeur de fonction. la fonction pointed doit avoir en parametre :
				int i1 		 		 : indicateur a toute fin utile
				char * str     : nom du module
				int i2				 : n° de sortie du module
				int i3, i4, i5 : les coordonnees d'axe i, j, k du point d'obs courant. si un axe
				                 est hors des dimension du module, la valeur -1 est passee.
				int i6         : le pas de temps du point d'obs courant
				YREAL *r1      : adresse de la valeur d'observation (vobs) du point d'obs courant
				YREAL *r2      : adresse la valeur de la quantite adjointe (qtea) associee. Cette donnee
												 n'existe qu'avec l'option O_VARINCR. Si cette option n'est pas
												 prise, la valeur vobs est redondamment passee.
				rem : pour *r1 et *r2, s'agissant d'adresse, les valeurs correspondantes peuvent etre modifiees.

		- in int : indicateur a toute fin utile (repassed a la fonction pointed)

	Nb: Une utilisation recursive de cette fonction est a proscrire ...

  Exemple d'utilisation :
								declaration (et programmation) de la fonction a appeler en chaque point d'obs :
		void Yc_dstar_all (int indic, char *nmmod, int nout, int iaxe, int jaxe, int kaxe, int pdt, YREAL *vobs, YREAL *qtea)
		{...}
								appel a Yrovobs pour parcourir l'arbo des obs et recuperer la main dans la fonction passee a chaque obs :
		Yrovobs(Yc_dstar_all, 0);
*/
//-----------------------------------------
void Yrovobs1(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, int i7, YREAL *r1, YREAL *r2),
												int indic, char *nmmod, int warbpdt, int wtrupdt, struct Yst_nodo *MwRobs)
{
  struct Yst_nodo *SwRobs; /* un pointeur sur la Sortie s/Root */
  struct Yst_fobs *YwLobs; /* un pointeur sur l'element courant de la liste des obs */

	SwRobs = MwRobs->fils;
	while(SwRobs!=NULL)
	{ YwLobs = (struct Yst_fobs *) SwRobs->fils;
	  while(YwLobs!=NULL)
	  {
        #ifdef YO_VARINCR
	  		fct(indic, nmmod, SwRobs->iind, YwLobs->iind, -1, -1, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->qtea);
        #else
	  		fct(indic, nmmod, SwRobs->iind, YwLobs->iind, -1, -1, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->vobs);
				#endif
		  	YwLobs=YwLobs->frere;
	  }
	  SwRobs = SwRobs->frere;
	}
}
//-----------------------------------------
void Yrovobs2(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, int i7, YREAL *r1, YREAL *r2),
												int indic, char *nmmod, int warbpdt, int wtrupdt, struct Yst_nodo *MwRobs)
{
  struct Yst_nodo *SwRobs; /* un pointeur sur la Sortie s/Root */
  struct Yst_nodo *IwRobs; /* un pointeur sur la Sortie s/Root (si dim>1)*/
  struct Yst_fobs *YwLobs; /* un pointeur sur l'element courant de la liste des obs */

	SwRobs = MwRobs->fils;
	while(SwRobs!=NULL)
	{ IwRobs = SwRobs->fils;
	  while (IwRobs!=NULL)
	  {	YwLobs = (struct Yst_fobs *) IwRobs->fils;
	  	while(YwLobs!=NULL)
	  	{
        #ifdef YO_VARINCR
	  		fct(indic, nmmod, SwRobs->iind, IwRobs->iind, YwLobs->iind, -1, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->qtea);
        #else
	  		fct(indic, nmmod, SwRobs->iind, IwRobs->iind, YwLobs->iind, -1, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->vobs);
				#endif
		  	YwLobs=YwLobs->frere;
	  	}
	  	IwRobs = IwRobs->frere;
	  }
	  SwRobs = SwRobs->frere;
	}
}
//-----------------------------------------
void Yrovobs3(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, int i7, YREAL *r1, YREAL *r2),
												int indic, char *nmmod, int warbpdt, int wtrupdt, struct Yst_nodo *MwRobs)
{
  struct Yst_nodo *SwRobs; /* un pointeur sur la Sortie s/Root */
  struct Yst_nodo *IwRobs; /* un pointeur sur la Sortie s/Root (si dim>1)*/
  struct Yst_nodo *JwRobs; /* un pointeur sur la Sortie s/Root (si dim>2)*/
  struct Yst_fobs *YwLobs; /* un pointeur sur l'element courant de la liste des obs */

  SwRobs = MwRobs->fils;
	while(SwRobs!=NULL)
	{ IwRobs = SwRobs->fils;
	  while (IwRobs!=NULL)
	  { JwRobs = IwRobs->fils;
	    while(JwRobs!=NULL)
	  	{ YwLobs = (struct Yst_fobs *) JwRobs->fils;
	  		while(YwLobs!=NULL)
	  		{
          #ifdef YO_VARINCR
	  		  fct(indic, nmmod, SwRobs->iind, IwRobs->iind, JwRobs->iind, YwLobs->iind, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->qtea);
          #else
	  		  fct(indic, nmmod, SwRobs->iind, IwRobs->iind, JwRobs->iind, YwLobs->iind, warbpdt, wtrupdt, &YwLobs->vobs, &YwLobs->vobs);
				  #endif
		  	  YwLobs=YwLobs->frere;
	  		}
	  		JwRobs = JwRobs->frere;
	  	}
	  	IwRobs = IwRobs->frere;
	  }
	  SwRobs = SwRobs->frere;
	}
}
/*---------------------------------------------------------------------------*/
void Yrovobs_TR(struct Yst_nodo *traj_TRobs,
                void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, int i7, YREAL *r1, YREAL *r2),
                int indic)
{
	  struct Yst_nodo *TwRobs; /* un pointeur sur le Time s/Root */
	  struct Yst_nodo *MwRobs; /* un pointeur sur le Moduls/Root */

	  char	ModName[STRSIZE80+1];
	  int 	ModDim;
	  int   wtrupdt;

    TwRobs = traj_TRobs;

    if (TwRobs->iind<0) wtrupdt = -TwRobs->iind-1; /* cas des termes d'ebauche avec temps fictivement */
		else wtrupdt = TwRobs->iind;									 /* negatif: on retablit le vrai pdt                */

		MwRobs = TwRobs->fils;
		while (MwRobs!=NULL) //liste des modules-------------
		{   strcpy(ModName, YTabMod[MwRobs->iind].Name);	/* le nom du module */
	  	  /* YCoefGrad = YTabMod[MwRobs->iind].scoef;	: le coef de la fct cout: ce n'est pas le propos ici ... */
		    ModDim = YTabMod[MwRobs->iind].dim;
		    if     (ModDim==1)
		  	   Yrovobs1(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    else if(ModDim==2)
		  	   Yrovobs2(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    else //=>ModDim==3
		  	   Yrovobs3(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    MwRobs = MwRobs->frere;
		}
}

//------------------------------------------------------------
//void Yrovobs(void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, YREAL *r1, YREAL *r2), int indic)
void Yrovobs(struct Yst_nodo *traj_Robs,
             void(*fct)(int i1, char *str, int i2, int i3, int i4, int i5, int i6, int i7, YREAL *r1, YREAL *r2),
             int indic)
{
	  struct Yst_nodo *TwRobs; /* un pointeur sur le Time s/Root */
/*  struct Yst_nodo *MwRobs; // un pointeur sur le Moduls/Root
	  char	ModName[STRSIZE80+1];
	  int 	ModDim;
	  int   wtrupdt;
*/
		//TwRobs = YTabTraj[witraj].YRobs->fils; //Robs = traj_robs->fils; //YRobs->fils;
    TwRobs = traj_Robs->fils;
		while(TwRobs!=NULL)	//liste des temps --------------------
		{
/*    if (TwRobs->iind<0) wtrupdt = -TwRobs->iind-1; // cas des termes d'ebauche avec temps fictivement
			else wtrupdt = TwRobs->iind;									 // negatif: on retablit le vrai pdt
			MwRobs = TwRobs->fils;
		  while (MwRobs!=NULL) //liste des modules-------------
		  { strcpy(ModName, YTabMod[MwRobs->iind].Name);	// le nom du module
	  		// YCoefGrad = YTabMod[MwRobs->iind].scoef;	: le coef de la fct cout: ce n'est pas le propos ici ...
		    ModDim = YTabMod[MwRobs->iind].dim;
		    if     (ModDim==1)
		    	Yrovobs1(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    else if(ModDim==2)
		    	Yrovobs2(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    else //=>ModDim==3
		    	Yrovobs3(fct, indic, ModName, TwRobs->iind, wtrupdt, MwRobs);
		    MwRobs = MwRobs->frere;
		  }
*/    Yrovobs_TR(TwRobs, fct, indic);

			TwRobs = TwRobs->frere;
		}
}

/*===========================================================================*/
void	Yprintobs(int indic, char *nmmod, int nout, int iaxe, int jaxe, int kaxe,
                int arbpdt, int pdt, YREAL *vobs, YREAL *qtea)
{
   if (!select_io(YIO_LISTOBS, nmmod, nout, iaxe, jaxe, kaxe, pdt, vobs))
			return;
	 if (arbpdt<0)
	    printf("ebx %s:%i(%i %i %i; %i) = (%e %e)\n", nmmod, nout+1, iaxe+1, jaxe+1, kaxe+1, pdt+1, *vobs, *qtea);
	 else
	    printf("obs %s:%i(%i %i %i; %i) = (%e %e)\n", nmmod, nout+1, iaxe+1, jaxe+1, kaxe+1, pdt+1, *vobs, *qtea);
}
void	Ylistobs(int itraj) //si itraj vaut YNBTRAJ alors toutes les trajectoires
{  int witraj;
   if (itraj==YNBTRAJ)
	    for (witraj=0; witraj<YNBTRAJ; ++witraj)
	        Yrovobs(YTabTraj[witraj].YRobs, Yprintobs, 0);
	 else
	    Yrovobs(YTabTraj[itraj].YRobs, Yprintobs, 0);
}
/*...........................................................................*/
/*===========================================================================*/


/*===========================================================================*/
int Youtoobs(YioKind yiokind, int cdesc, char *cdes[]) /* state to obs or ebx */
{
		//pour cette fct, j'utilise (et j'accede) directement les parametres de la commande lues
		/* syntaxe:		[0]			[1]    [2]	  [3]	  [4]  ...
								outoobs  nmmod s(ortie) pdt1  pdt2 ...
								outoebx  nmmod s(ortie) pdt1  pdt2 ...  */
		int imod, nbs, nbt;	// indice du module et son nombre de sortie et de pas de temps
		int zenbs; 			    // la sortie demandees
		int zepdt, wpdt;    // pour le parcourt des pas d'observations ...
		int arbpdt;         // ... le pdt dans l'arbo des obs (distinction necessaire pour les
		                    //     ebauches pour lesquelles on utilise un pdt fictivement negatif
		int itraj;          // indice de la trajectoire du module

		imod  = Yimod(cdes[1]);
		if (imod<0 || imod>=YNBMODUL)			/* verif existance du module */
		{	 printf("%s: module %s not found \n", cdes[0], cdes[1]);
			 return(0);
		}

		/* cas d'obs: verif que le module est de type cout */
		if (yiokind==YIO_OUTOOBS && !YTabMod[imod].is_cout)
		{	  printf("%s: modul %s is not a 'cout' one\n", cdes[0], cdes[1]);
			  return(0);
		}

		/* cas d'ebx: verif que le module est de type target */
		if (yiokind==YIO_OUTOEBX && !YTabMod[imod].is_target)
		{	  printf("%s: modul %s is not a 'target' one\n", cdes[0], cdes[1]);
			  return(0);
		}

		nbs  = YTabMod[imod].nb_stout;		/* on recupere et on verifie le nombre */
		zenbs = atoi(cdes[2]);						/* de sorties du dit module */
		if (zenbs>nbs)
		{	 printf("%s: required state %i, the module have only %i\n", cdes[0], zenbs, nbs);
			 return(0);
		}

    itraj = Yitrajimod(imod); 		/* positionne l'indice de trajectoire */

		nbt	= YTabMod[imod].nb_time;	/* on recupere le nombre de pas de temps du dit module */
		//mais, pour un modul associed a un operateur, c'est le temps de la trajectoire de laquelle
		//depend l'operateur qu'il faut utiliser !?
		if (Yioperaimod(imod)>=0)
		   nbt = YTabTraj[itraj].nbuptime+YTabTraj[itraj].nbsteptime;
		   		
		if (nbt==0 && cdesc !=4)
		{ printf("%s: module %s is not tempo, it required only a 0 step time\n", cdes[0], cdes[1]);
		  return(0);
		}

		Ydmod = YTabMod[imod].dim;		/* on positionne en variable globale la dim du module */
		YioInsertObsCtr = 0;					/* re-init du compteur d'obs inserees */
		YioOvrObsCtr = 0;					  	/* re-init du compteur d'obs overridees */
		
    /*multi: l'uptime est par trajectoire
		itraj = Yitrajimod(imod); (opera: deplaced ^) */
		YNBUPTIME = YTabTraj[itraj].nbuptime; 

		//Pour chaque pdti
		for (wpdt=3;wpdt<cdesc;++wpdt)
		{		//printf("nbt=%i wpdt=%i : %s\n", nbt, wpdt, cdes[wpdt]);
				zepdt = atoi(cdes[wpdt]);
									
				if (yiokind==YIO_OUTOOBS && zepdt>nbt)
				{	 printf("outoobs: step time required (%i) greater the module one (%i)\n", zepdt, nbt);
			 		 return(0);
				}
				if (yiokind==YIO_OUTOEBX && zepdt>YNBUPTIME)
				{	 printf("outoebx: step time required (%i) greater the up_time one (%i)\n", zepdt, YNBUPTIME);
//quid target sur la trajectoire !!!???
			 		 return(0);
				}

				if (nbt==0 && zepdt!=0)
				{	 printf("%s: module %s is not tempo, step time must be 0\n", cdes[0], cdes[1]);
		  		 return(0);
				}

				if(nbt>0)
				{	if (zepdt<=0)
					{	 printf("%s: module %s is tempo, step time must be greater than 0\n", cdes[0], cdes[1]);
		  		 	return(0);
					}
					--zepdt; /* pdt ext --> pdt interne */
				}
													
				if (yiokind==YIO_OUTOOBS)
				{	 if (nbt>0) arbpdt=zepdt; else arbpdt=0;
				}				
				else /*=>YIO_OUTOEBX: dans le cas des ebauches, on va se servir d'indice */
				{		 /* de pas de temps négatif fictif */
					 if (nbt>0) arbpdt=-zepdt-1; else arbpdt=-1;
				}
								
				if (zenbs!=0) //=> 1 seule sortie
				{  Youtoobs_mod(yiokind, cdes[1], zenbs-1, zepdt, arbpdt);
				}
				else //donc ca implique qu'ici, zenbs vaut 0 ...
				{  //=> on doit boucler sur toutes les sorties
           for (zenbs=0; zenbs<nbs; ++zenbs)
							 Youtoobs_mod(yiokind, cdes[1], zenbs, zepdt, arbpdt);
					 //>>remettre zenbs à 0 apres la boucle pour le pas d'iteration suivant !!!!
					 zenbs = 0;
				}
		}

		printf(" %i state(s) selected as obs/ebx (%i inserted + %i overrided)\n",
					   YioInsertObsCtr+YioOvrObsCtr, YioInsertObsCtr, YioOvrObsCtr);
		return(1);
}

/*===========================================================================*/
