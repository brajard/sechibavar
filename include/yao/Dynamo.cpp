
//=============================================================================
/* ////////////////////////////////////////////////////////////////////////////
//               DES FONCTIONS DE SERVICES (boite a outils)									 //
//////////////////////////////////////////////////////////////////////////// */

/*===========================================================================*/
/* Yvsmat:    multiplication d'une Sous-MATrice par un Vecteur               */
/* --------------------------------------------------------------------------*/
/*template<class TypReel>void oldYvsmat (int lig, int col, int max_col, TypReel vout[], TypReel Mat[], TypReel vin[])
{	int l, c, lmc;
  TypReel cal;
	for (l=0; l<lig; ++l)
	{		vout[l] = 0.0;
			lmc=l*max_col;
			for (c=0; c<col; ++c)
			{		//vout[l] += vin[c]* Mat[lmc+c];
					cal = vin[c] * Mat[lmc+c]; vout[l] += cal;
			}
	}
}*/
template<class TypReel> void Yvsmat (int lig, int col, int max_col, TypReel vout[], TypReel Mat[], TypReel vin[])
{	int l, c;
  TypReel cal, *M;
  for (l=0; l<lig; ++l)
	{   vout[l] = 0.0;
	    M=&Mat[l*max_col];
	    for (c=0; c<col; ++c)
			{		cal = vin[c] * M[c];
					vout[l] += cal;
			}
	}
}
/*===========================================================================*/
/* Yvsmatt:   multiplication d'une Sous-MATrice Transposee par un Vecteur    */
/* --------------------------------------------------------------------------*/
/*template<class TypReel>void oldYvsmatt (int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[])
{	int l, c;
  TypReel cal;
	for (c=0; c<col; ++c)
	{		vout[c] = 0.0;
			for (l=0; l<lig; ++l)
			{	  //vout[c] += vin[l]* Mat[(l*max_col)+c];
				  cal = vin[l]* Mat[(l*max_col)+c]; vout[c] += cal;
//printf("xxxxxxxx>  %e * %e +=> %e  \n", vin[l], Mat[(l*max_col)+c], vout[c]);
			}
	}
  //if (Yi==0 && Yj==0 && Yk==0 && YTemps==2)
	//   printf ("Yvmat: vin[%i]=%f Mat(%i,%i)=%f\n", l, vin[l], l, c, Mat[(l*max_col)+c]);
}*/
template<class TypReel> void Yvsmatt (int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[])
{	int l, c;
  TypReel cal, *M;
	memset(vout,0,col*sizeof(TypReel));
	for (l=0; l<lig; ++l)
	{   M=&Mat[l*max_col];
	    for (c=0; c<col; ++c)
	    {   cal = vin[l] * M[c];
	        vout[c] += cal;
	    }
	}
}
/*
// Cette implementation est equivalente à celle du haut. La première est prbablement un peu plus performant dans le cas
// de grande matrices puisqu'elle exploite le concept de localité spatiale des donnée en accedant la matrice par ligne
// (localité propre au stockage des données dans une matrice C). En tout cas conceptualement à ce stade là on a jamais des
// grandes matrices.
template<class TypReel> void Yvsmatt (int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[]){
//  int l, c;
  memset(vout,0,col*sizeof(TypReel));
//#pragma omp parallel for  num_threads(2)
  for(int c=0; c<col; ++c)
    for(int l=0; l<lig; ++l)
      vout[c] += vin[l] * Mat[l][c];
}
*/



// Juste pour le parallelisme: on est en train ici d'essayer de paralleliser la moltiplication Yvsmatt.
// Cette solution n'est pas optimelle mais ça nous permet déjà de voir que le nested parallelism ne marche pas dans ce cas.
// On fait pas assez de calcule dans cette fonction pour justifier l'overhead du au parallelisme.
// On laisse cette fonction pour futures études.
// Les amolioration à faire c'est des gérer mieu toutes les variables et trouver un moyen pour ne pas faire le dernier for: lastprivate ou du genre.
/*template<class TypReel> void YvsmattLuigi (int lig, int col, int max_col, TypReel vin[], TypReel Mat[], TypReel vout[])
{
  TypReel v[col];
  memset(vout,0,col*sizeof(TypReel));
  memset(v,0,col*sizeof(TypReel));
#pragma omp parallel for 
  //shared(v,col,lig,max_col, Mat, vin) private(vout) default(none) num_threads(1)
  for(int c=0; c<col; ++c){   
    TypReel *M;
    TypReel cal=0;
    M=&Mat[c];
    for(int l=0; l<lig; ++l)
      cal += vin[l] * M[l*max_col];
      //vout[c] += vin[l] * M[l*max_col];
    v[c] = cal;
  }
  for(int c=0; c<col; ++c)
    vout[c] = v[c];
}
*/

/*===========================================================================*/
/* Ymxmat:   multiplication de Matrices : X = A * B                          */
/* --------------------------------------------------------------------------*/
template<class TypValA, class TypValB, class TypValX>void Ymxmat (int la, int cl, int cb, TypValA MatA[], TypValB MatB[], TypValX MatX[])
{ //la = nbre de lignes de A
  //cl = nombre de colonnes de A = nombre de lignes de B 
  //cb = nombre de colonnes de B
  int l, n, c;
  int lxcl;
  TypValX cal;
	for (c=0; c<cb; ++c)
	{		for (l=0; l<la; ++l)
			{   cal = (TypValX)0.0;
			    lxcl = l*cl;
					for (n=0; n<cl; ++n)
					{ 	cal += MatA[lxcl+n] * MatB[c+n*cb];					
					}
					MatX[l*cb+c] = cal; 
			}
	}
}
/*===========================================================================*/
/*                      Calcul de derivees complexes                         */
/*---------------------------------------------------------------------------*/
template<class TypReal> void Ycx_jac(complex<TypReal> derivee, int j, int i)
{ /* Peut etre utilised si le module n'a que des complexes
     en entrees-sorties. Les indices j, i a passer doivent
	   correspondre a un indicage sur les complexes          */
	int l, c;
	l=j*2-2; c=i*2-2;
	Yjac[l  ][c  ]   = real(derivee);
	Yjac[l+1][c  ]   = imag(derivee);
	Yjac[l  ][c+1]   = -Yjac[l+1][c  ];
	Yjac[l+1][c+1]   = Yjac[l  ][c  ];
}
//-----------------------------------------------------------------------------
template<class TypReal> void Ycxr_jac(complex<TypReal> derivee, int l, int c)
{	/* Peut etre utilised lorsque le module melange reels et
	   complexes en entrees-sorties. Les indices l, c a passer doivent
	   correspondre a un indicage sur les reels                         */
	Yjac[l-1][c-1]   = real(derivee);
	Yjac[l  ][c-1]   = imag(derivee);
	Yjac[l-1][c  ]   = -Yjac[l  ][c-1];
	Yjac[l  ][c  ]   = Yjac[l-1][c-1];
}

/*===========================================================================*/
/*                Inversion d'une matrice (carree) tri-diagonale             
  input: 	mat_dim : dimension des vecteur et de la matrice
  				inf			:	vecteur inferieur
  				diag		:	vecteur diagonal
  				sup			:	vecteur superieur
  output:	mat_inv	:	la matrice inverse
  code retour			: 0=Ok, 1=Ko 																							 */
/* ------------------------------------------------------------------------- */
template<class TypVal>int Yinv_tri_mat (int mat_dim, TypVal inf[], TypVal diag[], TypVal sup[], TypVal mat_inv[])
{		/* Construction de la matrice inverse */
    int				i, j;  
    TypVal   	*vect, *prod; //TypVal   vect[mat_dim], prod[mat_dim];
    vect   = 	(TypVal*) calloc(mat_dim, sizeof(TypVal));
    prod   = 	(TypVal*) calloc(mat_dim, sizeof(TypVal));

    for (i = 0; i < mat_dim; i++)
    {
    		for (j = 0; j < mat_dim; j++)
        		vect[j] = (TypVal)0;
      	vect[i] = (TypVal)1;

      	if (!Yinv_tri_vect (mat_dim, vect, prod, inf, diag, sup))
      	{		cout << "~~~~~>Yinv_tri_vect:  erreur 1:  ????????????????????????\n";
        		return 0;	/* csr: avant y'avait que if DEBUG_... */
		  	}
	
      	for (j = 0; j < mat_dim; j++)
        		mat_inv[(j*mat_dim)+i] = prod[j];
    }
    free(vect); free(prod);
    return 1;
}
/* -------------------- Inversion matrice (suite) !? ----------------------- */
template<class TypVal>int	Yinv_tri_vect (int mat_dim, TypVal vect[], TypVal prod[], TypVal inf[], TypVal diag[], TypVal sup[])
{		int       i;
    TypVal    bet;
    TypVal    *R; //(TypVal)   R[mat_dim];
    R		   = 	(TypVal*) calloc(mat_dim, sizeof(TypVal));

    if (diag[0] == (TypVal)0)
    {		cout << "~~~~~>Yinv_tri_vect:  erreur 1:  ???????????????????????????\n";
      	return 0;
   	}
    prod[0] = vect[0] / (bet = diag[0]);

    for (i = 1; i < mat_dim; i++)
    {
      	R[i] = sup[i - 1] / bet;
      	bet  = diag[i] - inf[i] * R[i];

      	if (bet == (TypVal)0.0)
      	{		cout << "~~~~~>Yinv_tri_vect:  erreur 2:  ???????????????????????????\n";
        		return 0;
      	}
      	prod[i] = (vect[i] - inf[i] * prod[i - 1]) / bet;
    }

    for (i = (mat_dim - 2); i >= 0; i--)
    {		prod[i] -= R[i + 1] * prod[i + 1];
    }
    free(R);
    return 1;
}


/* ////////////////////////////////////////////////////////////////////////////
//                      LES FONCTIONS STANDARS DE YAO												 //
//////////////////////////////////////////////////////////////////////////// */
/* ----------------------------------------------------------------------------
//================== Help me if you can I'm feeling bad ==================== */
void Yhelp(char lang)
{ printf("\n Not include: user_fct,  Not yet include : Yauto_fct\n");

	if (lang=='f') /* ..................... FRANCAIS ..........................*/
	{	printf (" Abreviations: file: fichier;  str: chaine de caractère;  modname: nom de module\n");
	  printf ("   nbpdt: nombre de pas de temps  val: valeur entiere   real: un reel\n");
	  printf ("   nout: n° de sortie\n");
		printf ("   (I) (resp (B)): valable seulement en mode Interactif (resp Batch)\n");
		printf ("   [0 ou 1 au choix] {1 seul choix obligatoire}\n");
		printf ("   la saisie soit en minuscule soit en majuscule exclusivement est acceptee\n");
		printf ("-----------------------------------------------------------------------------\n");
		//divers
		printf (" help | aide|? : [e | f]: l'aide que vous venez d'afficher\n");
		printf (" exit |quit|bye: pour en finir avec Yao \n");
		printf (" !             : str: permet de passer la commande systeme contenue dans str\n");
		printf (" echo          : {on | off}: active ou pas l'affichage de la commande (B)\n");
		printf (" set_prompt    : str:  pour avoir un prompt à son goût\n");
		printf (" pause         : marque une pause: se met en attente d'un RC pour continuer\n");
		printf (" ltraj         : liste des trajectoires avec certaines informations\n");
		printf (" straj         : schema des trajectoires (valide si offtime et dtime sont des entiers)\n");
		printf (" pdt2pdt       : fromtrajname steptime totrajname : (sous reserve) donne l'equivalence du\n");
		printf ("               : steptime de fromtrajname pour la trajectoire totrajname\n");
		printf (" activ|unactiv : {trajname ,typetraj} [{trajname ,typetraj}[...]] [{only, less}]: activation\n");
		printf ("               : ou neutralisation de trajectoire (cf doc)\n");
		printf (" lspace        : liste des espaces avec certaines informations\n");
		printf (" nmod          : modname: donne le numero du module modname\n");
		printf (" lmod          : liste des modules avec certaines informations\n");
		printf (" lval          : liste des valeurs prédéfinies \n");
 		printf (" load_inst     : file: execute les instructions contenues dans file\n");
 		printf (" goto          : label: pour sauter les instructions suivantes jusqu'a la chaine 'label'\n");
		//pour preparer un run
    printf (" set_begintime : val: affecte la valeur val au temps initial du modele global\n");
    printf (" set_modeltime : 0: positionne toutes les trajectoires sur leur temps initial\n");
    printf (" set_toptime   : trajname val: affecte le temps unitaire courant de la trajectoire a val\n");
    printf (" set_dtime     : trajname val: affecte la valeur val au dt de la trajectoire\n");
    printf (" set_offtime   : trajname val: affecte val au decalage temporel de la trajectoire\n");
		printf (" print_cost    : [on | off |]: soit affiche le cout (YTotalCost) soit met a ON, OFF ou LAST\n");
		printf ("                 le flag qui permet de l'afficher au cours (ou a la fin avec LAST) des iterations \n");
		printf (" print_time    : [on | off]: soit affiche le top de toutes les trajectoires, soit switch\n");
		printf ("               : le flag qui permet l'affichage du temps au cours d'itérations\n");
		printf (" cost          : {lms real | appli}: choix de la fonction de cout, et définition d'un\n");
		printf ("                 coef pour la calculer qui s'applique à tous les modules, mais ... \n");
		printf (" set_scoef     : modname coef: positionne un coef pour modname en lieu et place de\n");
		printf ("                 celui défini par cost\n");
		printf (" set_bcoef     : modname coef: positionne un coef pour modname pour le calcul du cout\n");
		printf ("                 sur les termes d'ébauche\n");
		printf (" set_pcoef     : modname coef: positionne un coef ou %cage pour modname: utilisation :\n",'%');
		printf ("                 1: delta pour l'incremental; 2: ...\n");
		printf (" adjust        : {std | appli}: choix de la formule d'ajustement de ou des variables\n");
		printf ("                 à minimiser\n");
		printf (" testad        : pdx [{-1, tol}] maxcase [zinfo]]: test de l'adjoint global [ou local]:\n");
		printf ("                               < M'(Xo).dx, dy > = < dx, M*(Xo).dy >\n");
		printf (" testdf        : i j k codop [%c]pdx ptol [modname-ko]: fonction de vérification de la\n", '%');
		printf ("                 programmation des dérivées au point ijk, au temps t=1,\n");
		printf ("                 codop (reel, 0, N, R, r : cf doc) determine les valeurs des entrées des modules,\n");
		printf ("                 pdx=une perturbation (en %ctage si %c) de x, et ptol une tolérance en %c.\n", '%', '%', '%');
		printf (" testlt        : pdx alpha fdec nbloop [modop]: test du lineaire tangent, et \n");
		printf (" testof          test de la fonction objective (cf doc...) avec dx = Xo*pdx, alpha : \n");
		printf ("                 parametre de tendance,  fdec : facteur de decroissance de alpha, nbloop :\n");
		printf ("                 nombre de boucles de decroissance, et modop : un mode operatoire 0 ou 1\n");
		printf (" set_iosep     : chaine de caracteres: a utiliser comme separateur de token dans les fonction d'i/o\n");
		printf (" outoobs       : modname nout liste_pdt: Utilisation des états commes observations\n");
		printf (" loadobs       : cf paramètres dans la doc: Chargement d'observations à partir d'un fichier\n");
		printf (" loadstate     : cf paramètres dans la doc: Chargement d'états à partir d'un fichier\n");
		printf (" savestate     : objname nout oaxis t[%c] {A,B} {0,1,2,3} [filename]: sauvegarde de modules selon\n",'%');
		printf ("                 objname (cf doc) pour la (ou toutes les (si 0)) sortie selon l'ordre oaxis des\n");
		printf ("                 axes au (ou pour tous les si 0) pas de temps t (ou modulo(t)), en Ascii\n");
		printf ("                 ou Binaire. puis vient:  0=>que la valeur, 1=>+les axes, 2=>+le temps, 3=1+2.\n");
		printf ("                 les sorties se font dans filename ou à défaut, sur la sortie standard (écran)\n");
		printf (" outoebx       : modname nout liste_pdtI: Utilisation des états initiaux comme d'ebauche\n");
		printf (" lobs          : [trajname]: liste des observations limitee a trajname si precise\n");
		printf (" sampleof/i    : modulx infx supx dx moduly infy supy dy outfile: echantillonage de la fonction\n");
		printf ("                 de cout pour une ou deux variables\n");
		printf (" nnet          : netname: donne le numero du réseau de neurones netname\n");
		printf (" lnet          : liste des réseaux de neurones avec certaines informations\n");
		printf (" netload       : netname filename : chargement du réseau netname avec le fichier filename\n");
		printf (" setn_activ    : netname activation : choix de la fonction d'activation pour le réseau\n");
		printf ("                 netname. activaltion=SIGSIG => que sigmoide; avec SIGLIN les sorties\n");
		printf ("                 sont linéaires\n");
		printf (" setn_plin     : dmin dmax th: positionne les parametres de la fonction\n");
		printf ("                 lineair(O_NETWARD)\n");
		printf (" setn_psig     : mx dmin scale offset: positionne les paramètres de la\n");
		printf ("                 fonction sigmoïde(O_NETWARD)\n");
		printf (" forward       : nbpdt: effectue une passe avant sur nbpdt pas de temps\n");
		printf (" backward      : nbpdt: effectue une passe arrière sur nbpdt pas de temps\n");
		printf (" set_nbiter    : val: positionne le nombre d'itérations (YNbIter) à effectuer\n");
		printf ("                 par les fonctions run\n");
		printf (" set_nbextl    : val: positionne le nombre de boucles externes (YNbExtL) à effectuer\n");
		printf ("                 par les algorithmes incrementaux\n");
		printf (" run           : Lance YNbIter fois l'itération 'de base': passe avant, calcul\n");
		printf ("                 du cout, passe arrière, ajustement des variables\n");
		printf (" runi          : version incremental de run m avec YNbExtL boucles externes\n");
		#ifdef YO_M1QN3
		//pour m1qn3 (a aussi besoin de set_nbiter; le 1er mot se termine par un m)
		printf ("\n setm_io       : val:                   Ces instructions \n");
		printf (" setm_impres   : val:                   concernent M1QN3.\n");
		printf (" setm_mode     : val:                   Il est préférable\n");
		printf (" setm_nsim     : val:                    de se repporter\n");
		printf (" setm_dxmin    : [real]:              directement au chapitre\n");
		printf (" setm_epsg     : real:                  qui le concerne\n");
		printf (" setm_ddf1     : real:                      dans la\n");
		printf (" runm et runim :                         documen-\n\n");
		printf (" runm2 & runim2:                                tation\n\n");
		#endif
		//instructions des Modules
		printf (" setstate      : modname real: affecte la valeur real à tous les états de toutes les\n");
		printf ("                 occurences (ijkt) du modul modname\n");
		printf (" setstate_all  : real: affecte la valeur real à tous les états de tous les modules\n");
		printf (" setepsi       : modname real: affecte la valeur real à tous les epsilons de toutes les\n");
		printf ("                 occurences (ijk) du modul modname\n");
		printf (" setepsi_all   : real: affecte la valeur real à tous les epsilons de tous les modules target\n");
		printf ("-----------------------------------------------------------------------------\n");
	}
	else if (lang=='e') /* ................... ANGLAIS ........................*/
	{ printf ("Abreviations:  str: string;  modname: modul name  nstept: number of time step\n");
	  printf ("  (I) (resp (B)): only for Interactive (resp Batch) mode\n");
	  printf ("  nout: n° de out\n");
	  printf ("  val: integer   real: real\n");
	  printf ("  [0 or 1 to choose] {must only one choice}\n");
		printf ("  both upper or lower case input is availuable but not mixed\n");
		printf ("---------------------------------------------------------------------------\n");
		//divers
		printf ("help | aide|? : [e | f]: ###\n");
		printf ("exit |quit|bye: to get aways from Yao\n");
		printf ("!             : str: ### \n");
		printf ("echo          : {on | off}: ### n");
		printf ("set_prompt    : str: ### \n");		
		printf ("pause         : ###\n");
		printf ("ltraj         : ###\n");
		printf ("straj         : ###\n");
		printf ("pdt2pdt       : fromtrajname steptime totrajname: ###\n");
		printf ("activ|unactiv : {trajname ,typetraj} [{trajname ,typetraj}[...]] [{only, less}]: ###\n");
		printf ("lspace        : ###\n");
		printf ("nmod          : modname: ###\n");
		printf ("lmod          : ###\n");
		printf ("lval          : ###\n");
		printf ("load_inst     : file: ###\n");
 		printf ("goto          : label: ###\n");		
		//pour preparer un run
    printf ("set_begintime : val: ###\n");
    printf ("set_modeltime : 0: ###\n");
    printf ("set_toptime   : trajname val: ###\n");
    printf ("set_dtime     : trajname val: ###\n");
    printf ("set_offtime   : trajname val: ###\n");
		printf ("print_cost    : [on | off | last]: ###\n");
		printf ("print_time    : [on | off]: ###\n");
		printf ("cost          : {lms val | appli}: ###\n");
		printf ("set_scoef     : modname coef: ###\n");
		printf ("set_bcoef     : modname coef: ###\n");
		printf ("set_pcoef     : modname coef: ###\n");
		printf ("adjust        : {std | appli}: ###\n");
		printf ("testad        : pdx [{-1, tol}] maxcase [zinfo]]: ###\n");
		printf ("testdf        : i j k codop [%c]pdx ptol: ###\n", '%');
		printf ("testlt        : pdx alpha fdec nbloop [modop]: ###\n");
		printf ("testof        : pdx alpha fdec nbloop [modop]: ###\n");          				
		printf ("set_iosep       : chaine de caracteres: ###\n");
		printf ("outoobs       : modname nout liste_pdt: ###\n");
		printf ("loadobs       : cf paramètres dans la doc: ###\n");
		printf ("loadstate     : cf paramètres dans la doc: ###\n");		
		printf ("savestate     : objname nout oaxis t[%c] {A,B} {0,1,2,3} [filename]: ###\n",'%');
		printf ("outoebx       : modname nout liste_pdtI: ###\n");		
		printf ("lobs          : [trajname]:###\n");
		printf ("sampleof/i    : modulx infx supx dx moduly infy supy dy outfile: ###\n");
		printf ("nnet          : netname: ###\n");		
		printf ("lnet          : ###\n");		
		printf ("netload       : netname filename : ###\n");
		printf ("setn_activ    : netname activation : ###\n");
		printf ("setn_plin     : dmin dmax th: ###\n");
		printf ("setn_psig     : mx dmin scale offset: ###\n");		
		printf ("forward       : nstept: ###\n");
		printf ("backward      : nstept: ###\n");
		printf ("set_nbiter    : val: ###\n");
		printf ("set_nbextl    : val: ###\n");
		printf ("run           : ###\n");
		printf ("runi          : ###\n");
		#ifdef YO_M1QN3		
		//pour m1qn3 (also need set_nbiter; first word end letter is m)
		printf ("setm_io       : val: ###\n");
		printf ("setm_impres   : val: ###\n");
		printf ("setm_mode     : val: ###\n");
		printf ("setm_nsim     : val: ###\n");
		printf ("setm_dxmin    : [real]: ###\n");
		printf ("setm_epsg     : real: ###\n");
		printf ("setm_ddf1     : real: ###\n");
		printf ("runm et runim : ###\n");
		printf ("runm2 & runim2: ###\n");
		#endif							
		//modul instruction
		printf ("setstate      : modname real: ###\n");
		printf ("setstate_all  : real: ###\n");
		printf ("setepsi       : modname real: ###\n");
		printf ("setepsi_all   : real: ###\n");
		printf ("-----------------------------------------------------------------------------\n");
	}
	printf("\n");
}

/* ----------------------------------------------------------------------------
//=========== appelle d'une fonction sous la forme argc argv =============== */
//                   avec constitution des arguments
int Yfarg(int(*fct)(int argc, char *argv[]), char *args)
{  int  cdesc;
   char *cdes[NB_MAX_ARG_CDE];
   char cdeline[BUFSIZE+1];

	 strcpy(cdeline, args);
	 if (!Yrecup_cde (&cdesc, cdeline, cdes))
	 			return(0);
	 return(fct(cdesc, cdes));
}
/* ----------------------------------------------------------------------------
//================ Indice d'une definition dans YTabDefval ================= */
int Yisdefval(char *name)
{	int	wi;
	for (wi=0; wi<YNBDEFVAL; ++wi)
	{	if (strcmp(name, YTabDefval[wi].Name) == 0)
			 return (wi);
	}
	return(-1);
}
/*----------------------- liste des valeurs définies ------------------------*/
void Ylval()
{
	printf (" |  N° |     nom      |  valeur  | \n");	
	for (int w1=0; w1<YNBDEFVAL; ++w1)
		  printf (" | %3i | %-12s | %8s |\n", w1+1,
						  YTabDefval[w1].Name, YTabDefval[w1].macro);
  printf("\n");
}
/* ----------------------------------------------------------------------------
//=============== Valorisation du tableau Yting pour testdf ================ */
void Ysetting(char *codop)
{
	double val;
	if      (codop[0]=='N')	/* input value = input indice */
	{	 for(int wi=0; wi<YMAX_NBI; ++wi) Yting[wi]=wi+1;
	}	
	else if (codop[0]=='r') /* rand in [0, 1] */
	{	 for(int wi=0; wi<YMAX_NBI; ++wi) Yting[wi]=(double)rand()/RAND_MAX;
	}	
	else if (codop[0]=='R') /* rand in [-1, 1] */
	{	 for(int wi=0; wi<YMAX_NBI; ++wi) Yting[wi]=((double)rand()/RAND_MAX)*2 -1;
	}
	else if ( codop[0]=='F' || codop[0]=='f')
	{/*nb codop=='F' used for dfward: see testdf in Yao8.c (so don't use it here)*/
	}
	else if (codop[0]=='U')
	{/* the User must it-self feel Yting before calling testdf */
	}	
	else /* otherwise we considere it is a real */
	{   val = atof(codop);
			for(int wi=0; wi<YMAX_NBI; ++wi) Yting[wi]=val;
	}
}

/* ----------------------------------------------------------------------------
//=============== test de l'erreur sur l'adjoint par module ================ */
int Ytesterrad_mod(char *nmmod, double YLTRes, double YAdRes)
{	double errel;

	/* max case demanded deja atteind */
	if (Ynbko>=Ymaxko) return(0);
	
	/* cas de valeurs proches d'un zero informatique ... ->ok !? */
	if (Yerrelmax!=-1)
	 if (fabs(YLTRes)<=Ypzedi  && fabs(YAdRes)<=Ypzedi)
	    return(1);

	/* autres cas, on calcul et on test l'erreur relative */
	errel = (YLTRes-YAdRes)/YLTRes;
	if (fabs(errel)>=Yerrelmax || Yerrelmax<0.0)	
	{  printf (" %8s(%i %i %i; %i)  % 23.15e - % 23.15e = % -23.15e  relerr=% 21.15e\n",
		     	    nmmod, Yi+1, Yj+1, Yk+1, Yt+1, YLTRes, YAdRes, YLTRes-YAdRes, errel);
	   ++Ynbko;
	   //if (Ynbko>=Ymaxko) return(0);
	}
	return(1);
}

/* ----------------------------------------------------------------------------
//======================== Fonction pour le deboggage ====================== */
void Ydbg_nanf(char *str, int nbi, char *nmmod)
{
	for (int wi=0; wi<nbi; ++wi)
	{
		if (finite((double)Yting[wi]) == 0)
		{ printf("dbg: %s %s[%i]: (%i %i %i : %i) : NaN or Inf found for Yting !!!=> exit process\n",
		               str, nmmod, wi+1,  Yi+1, Yj+1, Yk+1, Yt+1);
          exit(0);
		}
		if (finite((double)Ytbeta[wi]) == 0)
		{ printf("dbg: %s %s[%i]: (%i %i %i : %i) : NaN or Inf found for Ytbeta !!!=> exit process\n",
							     str, nmmod, wi+1,  Yi+1, Yj+1, Yk+1, Yt+1);
          exit(0);
		}
	}
}
/* ------------------------------------------------------------------------- */
void Ydbg_ting(char *str, int nbi, char *nmmod)
{
   printf ("dbg: %s module %s : (%i %i %i ; %i) \n", str, nmmod, Yi+1, Yj+1, Yk+1, Yt+1);
	 for (int wi=0; wi<nbi; ++wi)
	 {   printf("dbg:  input [%i]: => %e \n", wi+1,  Yting[wi]);
	 }
}
void Ydbg_ting_wrk(char *str, int nbi, char *nmmod)
{	if(!strcmp(str, "F: ")) return;
	if (Yt!=1) return;
	     printf ("dbg: %s module %s : (%i %i %i ; %i) \n", str, nmmod, Yi+1, Yj+1, Yk+1, Yt+1);
	 for (int wi=0; wi<nbi; ++wi)
	 {   printf("dbg:  input [%i]: => %22.16e \n", wi+1,  Yting[wi]);
	 }
}
/* --------------------------------------------------------------------------*/
void Ydbg_beta(char *str, int nbi, char *nmmod)
{			 printf ("dbg: %s module %s : (%i %i %i ; %i) \n", str, nmmod, Yi+1, Yj+1, Yk+1, Yt+1);
	 for (int wi=0; wi<nbi; ++wi)
	 {   printf("dbg:   grad [%i]: => %e \n", wi+1, Ytbeta[wi]);
	 }
}

//============================= trajectoire : ...  ==========================*/
void Ydispcurstep(int itraj)  /* not realy used because not practical */
{ /* pour voir la progression des trajectoires.
     echelle de tps non respectee si dt n'est pas entier ! */
  int   i, j;
  float wtime;
	float startime;
	printf ("   |");
  wtime = Ybegintime;
	startime = Ybegintime + YTabTraj[itraj].offtime;
	while (wtime < startime)
	{ printf(" "); ++wtime;
	}
	while (wtime < YTabTraj[itraj].curtime)
	{	for (i=0; i<YTabTraj[itraj].dtime; ++i) printf(" ");
    wtime += YTabTraj[itraj].dtime;
	}
	if (wtime < YTabTraj[itraj].stoptime)
	{	for (j=1; j<YTabTraj[itraj].dtime; ++j) printf("_");
	  printf(".");
	}
	printf("\n");
}
/*-------------------------*/
void Ystraj()
{ int   i, j;
  float wtime;
  float startime;
	printf("\n\n            representation graphique des trajectoires\n");
	printf("attention, echelle de temps non respectée si dt et offt ne sont pas entiers !\n\n");

	for (i=0; i<YNBTRAJ;++i)
	{   printf(" %8s:  |", YTabTraj[i].name);
	    wtime = Ybegintime;
			startime = Ybegintime + YTabTraj[i].offtime;
	    while (wtime < startime)
	    { printf(" "); ++wtime;
			}
	    while (wtime < YTabTraj[i].stoptime)
			{ for (j=1; j<YTabTraj[i].dtime; ++j) printf("_");
			  printf(".");
				wtime += YTabTraj[i].dtime;
			}
			printf("|\n\n");
	}
}

//============== trajectoire : repositionnement sur le modele ===============*/
int Yset_modeltime (int time)  //PLM, je ne traite que le repositionnement au debut du modele !!! (ARAR)
{  int wi;
	 for (wi=0; wi<YNBTRAJ; ++wi)
	 {	 YTabTraj[wi].toptime = YTabTraj[wi].nbuptime;
			 YTabTraj[wi].curtime = Ybegintime + YTabTraj[wi].offtime;
   }
   return(0);
}
//=================== trajectoire : recalcule de curtime ====================*/
int Yupdcurtime(int itraj)
{
	if (itraj<0 || itraj >= YNBTRAJ) return(-1);
	YTabTraj[itraj].curtime = Ybegintime + YTabTraj[itraj].offtime
	                        + (YTabTraj[itraj].toptime - YTabTraj[itraj].nbuptime)
	                          * YTabTraj[itraj].dtime;
	return(0);
}
/*=================== trajectoire : recalcule de stoptime ===================*/
int Yupdstoptime(int itraj)
{
	if (itraj<0 || itraj >= YNBTRAJ) return(-1);
	YTabTraj[itraj].stoptime = Ybegintime + YTabTraj[itraj].offtime
	                        + YTabTraj[itraj].dtime * YTabTraj[itraj].nbsteptime;
	return(0);
}
/*============ Indice Trajectoire dans tableau des trajectoires =============*/
int Yitraj(char *nmtraj)
{
  for (int w1=0; w1<YNBTRAJ; ++w1)
		if (strcmp(YTabTraj[w1].name, nmtraj) == 0)
				return (w1);
	return(-1);	
}
/*---------------------------------------------
int Yitrajimod(int imod)
{return(Yitraj(YTabSpace[Yispace(YTabMod[imod].space_name)].traj_name));
} cf v */
/*---------------------------------------------*/
int Ypdt2pdt (int wxtraj, int topy, int wytraj)
{   /* ATTENTION: cette fonction doit etre en phase 
       avec celle, de meme nom, qui est dans Yao8.c */
    float offy, offx, dty, dtx;
    int   topx, upty, uptx;

    offy = YTabTraj[wytraj].offtime;
    offx = YTabTraj[wxtraj].offtime;
    dty  = YTabTraj[wytraj].dtime;
    dtx  = YTabTraj[wxtraj].dtime;
    upty = YTabTraj[wytraj].nbuptime;
    uptx = YTabTraj[wxtraj].nbuptime;

    topx = (int)( (offy + (topy-upty)*dty - dty - offx) / dtx ) + uptx + 1;
    return(topx);
}
/*---------------------------------------------*/
int Ytttt_pdt (int wxtraj, int topy, int wytraj)
{ /* traduction du temps d'une trajectoire a l'autre mais en considerant
     cette fois qu'on travaille avec des indices C et qu'on borne
     le resultat au temps de la trajectoire */
   int  topx;
   topx = Ypdt2pdt(wxtraj, topy+1, wytraj) - 1;
   if (topx < 0) topx = 0;
   else if (topx >= YTabTraj[wxtraj].nbuptime + YTabTraj[wxtraj].nbsteptime)
      topx = YTabTraj[wxtraj].nbuptime + YTabTraj[wxtraj].nbsteptime - 1;
      
   return(topx);
}
/*==================== liste du tableau des trajectoires ====================*/
void Yltraj()
{
	printf (" |  id | traj name type| upt.  offt.    dt    curt.  topt. nbstept.  stopt. act| \n");	
	for (int w1=0; w1<YNBTRAJ; ++w1)
		  printf (" | %3i | %-10s  %c | %2i   %6.3f %6.3f  %6.3f  %2i      %2i     %6.3f   %1i |\n",
		          w1+1,
						  YTabTraj[w1].name,
						  YTabTraj[w1].type,
						  YTabTraj[w1].nbuptime,
						  YTabTraj[w1].offtime,
						  YTabTraj[w1].dtime,
						  YTabTraj[w1].curtime,
						  YTabTraj[w1].toptime,
						  YTabTraj[w1].nbsteptime,
						  YTabTraj[w1].stoptime,
						  YTabTraj[w1].isactiv
						 );
  printf("\n");
}

/*================ activation ou desactivation de trajectoire ===============*/
//syntax : {activ, unactiv} {trajname ,typetraj} [{trajname ,typetraj}[...]] [only]
//exemple: active Traj1 M Q Traj7 R only
int Yisa_typetraj(char typetraj)
{   int w1;
    for (w1=0; w1<YNBTRAJ; ++w1)
		  if (YTabTraj[w1].type == typetraj)
				 return (1);
	  return(0);
}
/*---------------------------------------------*/
int Yactraj(int cdesc, char *cdes[])
{
    int w1, w2; int only=0; int less=0; int activ=0;
    if (strcmp(cdes[0], "activ")==0 || strcmp(cdes[0], "ACTIV")== 0)
       activ=1;

	  /* verif trajname et/ou typetraj existe */
	  for (w1=1; w1<cdesc; ++w1)
	  { if (strlen(cdes[w1])==1) //un caractere => doit etre un type de trajectoire
	    {  if (!Yisa_typetraj(cdes[w1][0]))
	       {  printf("(un)activ: trajectory type (%c) does not exist\n", cdes[w1][0]);
			      return(0);
	       }
	    }
	    else if (strcmp(cdes[w1], "ONLY") == 0 || strcmp(cdes[w1], "only") == 0)
	    {  //only ou less doit etre le dernier token
	       if (w1 != cdesc-1)
	       {  printf("(un)activ: \"only\" must be the last token\n");
			      return(0);
	       }
	       only=1;
	    }
	    else if (strcmp(cdes[w1], "LESS") == 0 || strcmp(cdes[w1], "less") == 0)
	    {  //only ou less doit etre le dernier token
	       if (w1 != cdesc-1)
	       {  printf("(un)activ: \"less\" must be the last token\n");
			      return(0);
	       }
	       less=1;
	    }
	    
	    else
	    {  //ce doit etre le nom d'une trajectoire
				 if (Yitraj(cdes[w1])<0)
		     {	 printf("(un)activ: unknwon trajectory name (%s) \n", cdes[w1]);
			       return(0);
		     }
	    }
	  }

	  //----------------------------------
	  if (only) /* si only est utilised */
	  {  for (w1=0; w1<YNBTRAJ; ++w1)
	     {   if (activ) YTabTraj[w1].isactiv = 0; //pour activ on commence par tout desactiver
	         else if (YTabTraj[w1].fward != NULL) //pour unactiv, on commence par tout activer
	                 YTabTraj[w1].isactiv = 1;    //sauf les traj non operationnelle
	     }
	  }
	  else if (less) /* si less est utilised */
	  {  for (w1=0; w1<YNBTRAJ; ++w1)
	     {   if (!activ) YTabTraj[w1].isactiv = 0; //pour unactiv on commence par tout desactive
					 else if (YTabTraj[w1].fward != NULL)  //pour active on commence par tout activer sauf les traj non operationnelle
	                 YTabTraj[w1].isactiv = 1;
	     }
	  }

	  //----------------------------------
	  /* puis on va activer ou desactiver selon chaque token */
	  for (w1=1; w1<cdesc; ++w1)
	  {   if (strcmp(cdes[w1], "ONLY") == 0 || strcmp(cdes[w1], "only") == 0
	      ||  strcmp(cdes[w1], "LESS") == 0 || strcmp(cdes[w1], "less") == 0)
	         break;
	      for (w2=0; w2<YNBTRAJ; ++w2)
	      {   if (  strcmp(cdes[w1], YTabTraj[w2].name)==0
	          ||   (strlen(cdes[w1])==1 && cdes[w1][0]==YTabTraj[w2].type)
	             )
	          {  /* on ne touche pas aux trajectoires non oprationnelles */
	             if (YTabTraj[w2].fward == NULL)
	                continue;
	             if (activ) //pour activ on active sauf si less
	             {  if (less) YTabTraj[w2].isactiv = 0;
	             		else YTabTraj[w2].isactiv = 1;
	             }
	             else //pour unactiv, on desactive sauf si less
							 {  if (less) YTabTraj[w2].isactiv = 1;
	             		else YTabTraj[w2].isactiv = 0;
	             }	             
						}	      
	      }
	  }

	  return(1);
}

/*======================= liste du tableau des espaces ======================*/
void Ylspace()
{
	printf (" |  id | space name type | axi axj axk | traj name | \n");	
	for (int w1=0; w1<YNBSPACE; ++w1)
		  printf (" | %3i |  %-10s  %c  | %3i %3i %3i | %-10s|\n", w1+1,
						  YTabSpace[w1].name, YTabSpace[w1].type,
						  YTabSpace[w1].axi, YTabSpace[w1].axj, YTabSpace[w1].axk,
						  YTabSpace[w1].traj_name);
  printf("\n");
}
/*============== Indice espace dans tableau des espaces =====================*/
int Yispace(char *nmspace)
{
  for (int w1=0; w1<YNBSPACE; ++w1)
		if (strcmp(YTabSpace[w1].name, nmspace) == 0)
				return (w1);
	return(-1);
}
/*---------------------------------------------*/
int Yispaceimod(int imod) //!?
{return(Yispace(YTabMod[imod].space_name));
} //!?


/*======================= liste du tableau des operateurs ===================*/
void Ylopera()
{ printf (" |  id | opera name type | axi axj axk | traj name | act | \n");
	for (int w1=0; w1<YNBOPERA; ++w1)
		  printf (" | %3i |  %-10s  %c  | %3i %3i %3i | %-10s|  %1i  |\n", w1+1,
						  YTabOpera[w1].name, YTabOpera[w1].type,
						  YTabOpera[w1].axi, YTabOpera[w1].axj, YTabOpera[w1].axk,
						  YTabOpera[w1].traj_name, YTabOpera[w1].isactiv);
  printf("\n");
}
/* ---------------- indice operateur dans tableau des operateurs ------------*/
int Yiopera(char *nmopera)
{ for (int w1=0; w1<YNBOPERA; ++w1)
		if (strcmp(YTabOpera[w1].name, nmopera) == 0)
				return (w1);
	return(-1);
}
/*----------------- activation/desactivation d'operateurs -------------------*/
void Yact_operator(char type, short etat)
{ int w1;
  for (w1=0; w1<YNBOPERA; ++w1)
    if (YTabOpera[w1].type==type || type=='*') YTabOpera[w1].isactiv=etat;
}
/*---------------------------------------------*/
int Yioperaimod(int imod) //!?
{return(Yiopera(YTabMod[imod].space_name));
} //!?

/*======================= Indice du tableau des operateurs ===================*/
int Yitrajimod(int imod)
{   //renvoie l'indice de la trajectoire a laquelle le
    //module est rattached via un espace ou un operateur
    int w1;
    if      ( (w1=Yioperaimod(imod)) >=0)
       return(Yitraj(YTabOpera[w1].traj_name));
    else if ( (w1=Yispaceimod(imod)) >=0)
       return(Yitraj(YTabSpace[w1].traj_name));   
    else
    {  //unpossible case
       printf ("Ysys error #1\n"); return(-9);
    }
}
/* ----------------------------------------------------------------------------
//================= Indice Module dans tableau des modules ================= */
int Yimod(char *nmmod)
{ for (int w1=0; w1<YNBMODUL; ++w1)
		if (strcmp(YTabMod[w1].Name, nmmod) == 0)
				return (w1);
	return(-1);	
}
/*================ print la liste du tableau des modules =================== */
void Ylmod(int argc, char *argv[])
{  int all = 1; int target = 0; int cout = 0;
   /* entete */
   //printf (" | id | nbi nbs | dim | axi axj axk | nbt | C T O |    scoef      bcoef      pcoef   | name / space\n");
	 printf (" | id | nbi nbs | dim | axi axj axk | nbt | C  T deb end |    scoef      bcoef      pcoef   | name / space\n");

	 /* positionnement de flag pour savoir ce qu'on doit printer */
	 for(int wi=1; wi<argc; ++wi)
	 {  if (!strcmp(argv[wi], "target") || !strcmp(argv[wi], "TARGET") ) {all=0; target=1;}
	    if (!strcmp(argv[wi], "cout")   || !strcmp(argv[wi], "COUT") )   {all=0; cout=1;}
	 }

	 /* boucle sur les modules pour l'affichage */
	 for (int w1=0; w1<YNBMODUL; ++w1)
	 {
       if( all || (target==1 && YTabMod[w1].is_target) || (cout==1 && YTabMod[w1].is_cout) )
			 {  //printf (" |%3i | %3i %3i | %3i | %3i %3i %3i |%4i | %1i %1i %1i | %5.4e %5.4e %5.4e | %s / %s\n", w1+1,
	        printf (" |%3i | %3i %3i | %3i | %3i %3i %3i |%4i | %1i  %1i%3i %3i  | %5.4e %5.4e %5.4e | %s / %s\n", w1+1,
						   YTabMod[w1].nb_input, YTabMod[w1].nb_stout, YTabMod[w1].dim,
						   YTabMod[w1].axi, YTabMod[w1].axj, YTabMod[w1].axk, YTabMod[w1].nb_time,
						   YTabMod[w1].is_cout, YTabMod[w1].is_target, YTabMod[w1].deb_target, YTabMod[w1].end_target,
						   YTabMod[w1].scoef, YTabMod[w1].bcoef, YTabMod[w1].pcoef, YTabMod[w1].Name, YTabMod[w1].space_name);
							 //YTabMod[w1].ctrord,
			 }
	 }
   printf("\n");
}
/*=================== liste association module/operateur =================== */
void Ylmocop()
{  int w1, w2;
   printf (" ----- association list moduls/operators -----\n");
	 for (w1=0; w1<YNBMODUL; ++w1)
	 {   if (YTabMocop[w1][0] != -1) //y'a au moins un operateur associed
		   {  printf ("%s: ", YTabMod[w1].Name);
		      w2=0;
		      while (w2<YNBOPERA && YTabMocop[w1][w2]!=-1)
		      {   printf("%8s ", YTabOpera[YTabMocop[w1][w2]].name);
		          ++w2;
		      }
		      printf("\n");
		   }
	 }
	 printf("\n");
}
/* ----------------------------------------------------------------------------
//================ Indice Reseaux dans tableau des Resaux ================== */
#ifdef YO_NETWARD
int Yinet(char *nmnet)
{
	for (int w1=0; w1<YNBNET; ++w1)
		if (strcmp(YTabNet[w1].Name, nmnet) == 0)
				return (w1);
	return(-1);
}
//================ print la liste du tableau des reseaux =================== */
void Ylnet ()
{
	char activ[10];
	printf (" | ind |    nom   | nbw  | nbin | nbout | maxcell |  tweight  | factiv |\n");	
	for (int w1=0; w1<YNBNET; ++w1)
	{      if (YTabNet[w1].activ==SigLin) sprintf(activ,"siglin");
		else if (YTabNet[w1].activ==SigSig) sprintf(activ,"sigsig");
		else sprintf(activ,"unknown!?");
		printf (" | %3i | %8s | %3i  | %3i  |  %3i  |   %3i   | %9p | %s |\n", w1+1,
						YTabNet[w1].Name, YTabNet[w1].nbweight,
						YTabNet[w1].nbinput, YTabNet[w1].nboutput, YTabNet[w1].maxcell,
						YTabNet[w1].tweight, activ);
	}
}
#endif
/* ----------------------------------------------------------------------------
//======== positionnement des indices de parcourt des axes de l'espace ===== */
int YsetPaxe(int dim, char *oaxis, int  axe1, int  axe2, int axe3,
																	 int *axo1, int *axo2, int *axo3)
{
	if (dim==3)
	{
			if      (strcmp(oaxis, "ijk") == 0 || strcmp(oaxis, "IJK") == 0)
			{	 *axo1=axe1; *axo2=axe2; *axo3=axe3;}
			else if (strcmp(oaxis, "ikj") == 0 || strcmp(oaxis, "IKJ") == 0)
			{	 *axo1=axe1; *axo2=axe3; *axo3=axe2;}			
			else if (strcmp(oaxis, "jik") == 0 || strcmp(oaxis, "JIK") == 0)
			{	 *axo1=axe2; *axo2=axe1; *axo3=axe3;}			
			else if (strcmp(oaxis, "jki") == 0 || strcmp(oaxis, "JKI") == 0)
			{	 *axo1=axe2; *axo2=axe3; *axo3=axe1;}			
			else if (strcmp(oaxis, "kij") == 0 || strcmp(oaxis, "KIJ") == 0)
			{	 *axo1=axe3; *axo2=axe1; *axo3=axe2;}			
			else if (strcmp(oaxis, "kji") == 0 || strcmp(oaxis, "KJI") == 0)
			{	 *axo1=axe3; *axo2=axe2; *axo3=axe1;}
			else
				 return(0);
			/* default when error :	{*axo1=axe1; *axo2=axe2; *axo3=axe3;}	*/
	}
	else if (dim==2)
	{
			if      (strcmp(oaxis, "ij") == 0 || strcmp(oaxis, "IJ") == 0)
			{	 *axo1=axe1; *axo2=axe2;}
			else if (strcmp(oaxis, "ji") == 0 || strcmp(oaxis, "JI") == 0)
			{	 *axo1=axe2; *axo2=axe1;}
			else if (strcmp(oaxis, "ik") == 0 || strcmp(oaxis, "IK") == 0)
			{	 *axo1=axe1; *axo2=axe3;}
			else if (strcmp(oaxis, "ki") == 0 || strcmp(oaxis, "KI") == 0)
			{	 *axo1=axe3; *axo2=axe1;}
			else if (strcmp(oaxis, "jk") == 0 || strcmp(oaxis, "JK") == 0)
			{	 *axo1=axe2; *axo2=axe3;}
			else if (strcmp(oaxis, "kj") == 0 || strcmp(oaxis, "KJ") == 0)
			{	 *axo1=axe3; *axo2=axe2;}			
			else
				 return(0);
	}
	else	/* ...(dim==1) */
	{
				if    (strcmp(oaxis, "i") == 0 || strcmp(oaxis, "I") == 0)
			{	 *axo1=axe1;}
			else if (strcmp(oaxis, "j") == 0 || strcmp(oaxis, "J") == 0)
			{	 *axo1=axe2;}
			else if (strcmp(oaxis, "k") == 0 || strcmp(oaxis, "K") == 0)
			{	 *axo1=axe3;}
			else
				 return(0);
	}
	return(1);
}


/*---------------------------------------------------------------------------*/
/* gestion des entrees - sorties et des observations pour Yao  :						 */
#include "Dyniob.h"
/*---------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------
//============================ FONCTIONS DE COUT =========================== */
void Ycost()  //TRobs, YNBUPTIME, YidTraj, YTemps doivent etre posisionned
{   long	curtop;

		if (TRobs==NULL) return;

		Ytrupdt = TRobs->iind;											/* true pdt : temps initiaux et trajectoire*/
		if (Ytrupdt<0) Ytrupdt = -TRobs->iind-1;    /* true pdt : cas ebauches  */

		Yact_operator('*', OFF);      /* on desactive tous les operateurs */

    /* traitement OPERATEURS HHHHH */
		YCaseCost = WISHL2_NANACT;    /* pour WISH: ca mettra NAN (niveau module) */
		Ycobs_TR(TRobs->fils);			  /* puis ca active les operateurs associed au module (H, R, ...) */

    Yforward_operator('H'); 		  /* forward de tous les operateurs 'H' qui auront ete actived :M(x)->YS */

    if (YAL2Run==RUNL2_STD)       /* cas d'un run standard */
    {  /* DIFFERENCE [H(M(x)) - Yo] : Ywish=Ystate-Obs */
		   YCaseCost = WISHL2_DIFF;      /* puis, avec ce flag, on va faire la difference sur la        */
		   Ycobs_TR(TRobs->fils);			   /* zone WISH (Ywish=Ystate-Obs) (uniquement la ou y'a des obs) */
    }
#ifdef YO_VARINCR
		else if(YAL2Run==RUNL2_INCR)  /* cas d'un run incremental */
    {  //Ylinward_operator('H');       /* pour pousser le calculer du lineaire tangent sur H: H'M'(x)dx ->YG  */
		   //YCaseCost = WISHL2_QTEA;      /* puis, avec ce flag, on va calculer l'ex qtea directement dans Ywish */
		   //Ycobs_TR(TRobs->fils);			   /* (qtea)=Ywish  =  YG - (Y° - YS)  =  H'M'(Xk).dx - (y° - H(M(Xk))     */
       //qtea a deja ete valorised par Ylinward_flopobs
       //on projete simplement YS->YW
		   YCaseCost = WISHL2_QTEA;     /* puis, avec ce flag, on va  projeter la  zone qtea dans la     */
		   Ycobs_TR(TRobs->fils);			   /* zone WISH (Ywish=qtea = YG - (Y° -YS)  =  H'M'(Xk).dx - (y° - H(M(Xk)) (uniquement la ou y'a des obs) */
    }
#endif
       
    /* traitement OPERATEURS de covariance ('B' ou/et 'R', ou/et 'K') */
   if (TRobs->iind < 0) /* sur la partie ebauche */
		  Yforward_operator('B');  /* lance tous les operateurs 'B' qui auront ete actived */
	 else									/* sur la trajectoire */
			Yforward_operator('R');  /* lance tous les operateurs 'R' qui auront ete actived */
	 Yforward_operator('K');     /* ebauche et traj : lance tous les operateurs 'K' qui auront ete actived */

	  /* Calcul cout et gradient avec les zones Ywish en se rappelant que maintenant Ywish=Ystate-Obs
	     (ou dans le cas de l'incremental Ywish=Ystate+Ygrad-Obs):
	     mais eventuellement aussi avec les zones Ystate lorsqu'il y a un operateur 'R' */
    if (YAL2Run==RUNL2_STD)       /* cas d'un run standard */
    {  YCaseCost = COSTL2_WISHDIFF;  /* puis avec ce flag */
		   Ycobs_TR(TRobs->fils);		     /* on calcule : grad = costdiff(YW, YW||YS=COV(YW))   */
    }                                /* grad = coef*Ywish, et cout = coef*Ywish*COV(Ywish) */
#ifdef YO_VARINCR
		else if(YAL2Run==RUNL2_INCR)  /* cas d'un run incremental */
    {  YCaseCost = COSTL2_WISHQTEA;  /* puis avec ce flag */
		   Ycobs_TR(TRobs->fils);		     /* on calcule : grad = costdiff(qtea, YW||YS=COV(YW)  */
    }																 /* grad = coef*qtea, et cout = coef*Yqtea*COV(Ywish) */
#endif

		/* Retropropagation operateurs H actived */
		Ybackward_operator('H');
		
    /* affichage du cout if required */
	  if (YDispCost==ON)
		{	 curtop = time((time_t *)NULL);
			 if (TRobs->iind < 0)  /* partie ebauche*/
			 {  printf(" >>> Yiter %-3ld traj %-8s pdE=%i after %lds: Total cost = % -23.15e\n",
						     YItRun, YTabTraj[YidTraj].name, -TRobs->iind, curtop-Ytop0, YTotalCost);
			 }
			 else if(TRobs->iind < YNBUPTIME)	/* partie temps initiaux */
			 {  printf(" >>> Yiter %-3ld traj %-8s pdI=%i after %lds: Total cost = % -23.15e\n",
						     YItRun, YTabTraj[YidTraj].name, TRobs->iind+1, curtop-Ytop0, YTotalCost);
			 }
			 else
		   {  printf(" >>> Yiter %-3ld traj %-8s pdt=%i after %lds: Total cost = % -23.15e\n",
							   YItRun, YTabTraj[YidTraj].name, YTemps+1, curtop-Ytop0, YTotalCost);
		   }
		}
		
		/* pour avancer sur cette liste des temps */
		TRobs = TRobs->frere;
}
/*---------------------------------------------------------------------------*/
/* ---------- cout sur terme d'ebauche et observations initiales  -----------*/
void Ycostebx()
{
	for (YidTraj=0; YidTraj<YNBTRAJ; ++YidTraj)
	{   TRobs = YTabTraj[YidTraj].TRobs;
	    YNBUPTIME = YTabTraj[YidTraj].nbuptime;
      //while (TRobs!=NULL)
      while (TRobs != NULL  && TRobs->iind < YNBUPTIME)
	    {   //printf("costebx: traj=%i Yt=%i \n", YidTraj, YTemps);
	        if (TRobs->iind >= 0)	      // partie temps initiaux
						 YTemps = TRobs->iind;
					else												// partie ebauche
						 YTemps = -TRobs->iind-1;

	        Ycost();
	    }
			YTabTraj[YidTraj].TRobs = TRobs; //est-ce bien utile ?
	}
}
/* ----------------------------------------------------------------------------
//======================== FONCTIONS D'AJUSTEMENT ========================== */
void Yadjust()
{
	  if (YTypeAdjust == ADJUST_APPLI)
			 adjust_target ();
	  else if (YTypeAdjust == ADJUST_STD) /* default */
	     Yadjust_all ();
}
/*memo: for a kind of temperature
float			epsi_fact;
epsi_fact = 0.0; //1.0; //0.02;
YS1_Ke(0)		-= ( Epsi_Ke    * pow(1-epsi_fact, YItRun) ) * YG1_Ke(0);
*/
/* ----------------------------------------------------------------------------*/
#ifdef YO_VARINCR
void Yc_adjustd()		/* cas de l'incremental: ajustement dx dans la boucle interne */
{	   if (YTypeAdjust == ADJUST_APPLI)
			  adjust_target ();	/* nb: le user peut tester les flags pour savoir dans quel cas il est */
	   else if (YTypeAdjust == ADJUST_STD) /* default */
	      Yc_adjustd_all ();
}
#endif


/* ----------------------------------------------------------------------------
//============================= FONCTION FORWARD =========================== */
int Yforward_elect()
{   int wi;
    int candidat = -1;
    float wtime = MYMAXREEL; //DBL_MAX; //999999;

    for (wi=0; wi<YNBTRAJ; ++wi)								//dans l'ordre des declaration
    {   if (YTabTraj[wi].curtime < wtime				//qui est la + en retard
        &&  YTabTraj[wi].toptime < YTabTraj[wi].nbuptime + YTabTraj[wi].nbsteptime	//qui n'est pas en bout de course
        &&  YTabTraj[wi].isactiv								// qui est active
      //&&  YTabTraj[wi].curtime < Ystoptime  ...!?   ARAR
      //&&  YTabTraj[wi].curtime < Yendtime   ...!?   ARAR
           )
        {  candidat = wi;
           wtime = YTabTraj[wi].curtime;
        }
    }
    return(candidat);
}
/*---------------------------------------------------------------------------*/
int Yforward(int itraj, int topstop)
{  int itrajelue; int nbcall=0;
	 /* passe avant sur l'ensemble du modele global (i.e. sur ttes les trajectoires )
	    de la ou on est positionned ! jusqu'a : 2 cas :
	      si itraj = -1: on ira jusqu'a la fin du modele (et topstop ne sert pas)
	      sinon, on s'arrete des que le toptime de la trajectoire passed en parametre
	             (qui sert donc de reference) a atteint le topstop (non inclus: i.e. on s'arrete avant) */

		while ((itrajelue=Yforward_elect()) >= 0)  // le while controle la progression
		{   //ici, on n'avance QUE d'UN PDT sur la trajectoire selectionnee !
				YTemps = YTabTraj[itrajelue].toptime;  //(ctx pour les modules par exemple)
				if (itrajelue==itraj && YTemps >= topstop) return(nbcall); //cf^ (eventuel arret sur une traj de ref)
				YidTraj = itrajelue;                   //(ctx pour les modules par exemple)

        if (YDispTime)
        {   printf (" >>> Traj %s, Current forward time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
						//Ydispcurstep(itrajelue);
				}
        forward_before(0); //arar
				YTabTraj[itrajelue].fward(0); //<-: forward_order (pour LA trajectoire selectionnee)
				++nbcall;
				forward_after(0); //arar
        {++YTabTraj[itrajelue].toptime; //<-: ++YTemps
         YTabTraj[itrajelue].curtime += YTabTraj[itrajelue].dtime; //<-: multi
        }
    }
    return(nbcall);
}

/*...........................................................................*/
void Yforwardrun(int itraj, int nbpdt)
{   long topend;
		Ytop0 = time((time_t *)NULL);

		if (YTabTraj[itraj].fward==NULL)
		{  printf("no forward function defined for that trajectory\n");
		   return;
		}		
		if (YTabTraj[itraj].toptime + nbpdt > YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime )
		{	 printf ("required forward step out of upper limit: \n");
		   printf ("         current top (%i) + required step (%i) > top max (%i) \n",
				        YTabTraj[itraj].toptime, nbpdt, YTabTraj[itraj].nbuptime + YTabTraj[itraj].nbsteptime);
			 return;
		}

    YidTraj  = itraj; //(ctx pour les modules par exemple)
		//YTemps = YTabTraj[itraj].toptime; //(ctx pour les modules par exemple)		  //-

    before_it(1);
		for (int Ywt=1; Ywt<= nbpdt; ++Ywt)
		{		YTemps = YTabTraj[itraj].toptime; //(ctx pour les modules par exemple)		//+		
        if (YDispTime) printf (" >>> Traj %s, Current forward time = %i \n", YTabTraj[itraj].name, YTemps+1);

        forward_before(Ywt);
				YTabTraj[itraj].fward(0); //<-: forward_order
				forward_after(Ywt);
				//++YTemps;                //ctx																					//-
        {++YTabTraj[itraj].toptime; //<-: ++YTemps
         YTabTraj[itraj].curtime += YTabTraj[itraj].dtime; //<-: multi
        }
		}
		topend = time((time_t *)NULL); /* time(&topend); topend = time((long *)NULL);*/
		printf("forward duration ....: %lds\n", topend-Ytop0);
		return;
}

/* ----------------------------------------------------------------------------
//============== fonction de calcul du cout en passe avant ...  ============= */
void Yforwardcost()
{
		long	curtop;
		int   witraj;
		int   itrajelue;
		
		/* init ... */
//	YCaseCost = COSTL2_ONLY;  ne devrait plus etre utile ici puisque reposisionned apres par Ycost !!!
		YTotalCost = 0.0;				//cout a 0
				
	  //TRobs = YRobs->fils;
	  for (witraj=0; witraj<YNBTRAJ; ++witraj)    /* init des pointeurs des arbo d'obs supposed */
	  {   YTabTraj[witraj].TRobs = YTabTraj[witraj].YRobs->fils; /* inversed (i.e: ascending)   */
	  }

		/* Calcul du cout sur les termes d'ebauche et aux temps initiaux*/
		if (YTypeCost==COST_APPLI)			/* l'utilisateur devra prendre totalement */
		{	 cost_function(-1); 					//(YUPTIME-1)	/* en charge le calcul du cout ! */
		}
		else
		{  /* a faire trajectoire par trajectoire (si obs il y a) */
		   Ycostebx();
		}

		/*Puis on va calculer le cout en passe avant */
		Yset_modeltime(0);
		before_it(1);      //(YItRun);

    while ((itrajelue=Yforward_elect()) >= 0)  // le while controle la progression
		{     //ici, on n'avance QUE d'UN PDT sur la trajectoire selectionnee !

				YTemps = YTabTraj[itrajelue].toptime;  //(ctx pour les modules par exemple)
				YidTraj = itrajelue;                   //(ctx pour les modules par exemple)
				YNBUPTIME = YTabTraj[itrajelue].nbuptime;

        if (YDispTime)
        {  printf (" >>> Traj %s, Current forwardcost time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
					 //Ydispcurstep(itrajelue);
        }
        forward_before(0); //arar
				if (YAL2Run==RUNL2_STD)
					 YTabTraj[itrajelue].fward(0); //<-: Yforward_order ();
#ifdef YO_VARINCR
				else if (YAL2Run==RUNL2_INCR)
					 YTabTraj[itrajelue].lward(0); //<-: Ylinward_order ();
#endif
				forward_after  (0); //arar
				
				if (YTypeCost == COST_APPLI)		/* l'utilisateur devra prendre totalement */
					 cost_function (YTemps);	 		/* en charge le calcul du cout !          */
				else
				{  TRobs = YTabTraj[itrajelue].TRobs;  /* on se positionne sur le top d'obs courant de la trajectoire elue*/
				   //Ycobs();			/* calcul du cout au pas de temps (YTemps) courant (s'il y a lieu) */
				   //YTabTraj[itrajelue].TRobs = TRobs;  /* pour avancer sur cette liste des temps */
					 if (TRobs!=NULL && TRobs->iind==YTemps) //si cette traj a des obs au temps courant
					 {  Ycost(); //calcul du cout
				      YTabTraj[itrajelue].TRobs = TRobs;  /* pour avancer sur cette liste des temps */
				   }
				}
												
        {++YTabTraj[itrajelue].toptime; //<-: ++YTemps
         YTabTraj[itrajelue].curtime += YTabTraj[itrajelue].dtime; //<-: multi
        }
		}

		if (YDispCost==2)	//LAST
		{	 curtop = time((time_t *)NULL);
		 	 printf(" >>> at iter %3ld after %lds: Total cost = % -23.15e\n", YItRun, curtop-Ytop0, YTotalCost);
		}
}

/* ----------------------------------------------------------------------------
//============================ FONCTIONS BACKWARD ========================== */
int Ybackward_elect()
{   int wi;
    int candidat = -1;
    float wtime = -MYMAXREEL; //DBL_MAX; //999999;
		float wprectime;

    for (wi=YNBTRAJ-1; wi>=0; --wi)
    {   wprectime = YTabTraj[wi].curtime - YTabTraj[wi].dtime;
        if (wprectime > wtime
        &&  YTabTraj[wi].toptime > YTabTraj[wi].nbuptime
        &&  YTabTraj[wi].isactiv
      //&&  YTabTraj[wi].curtime > Ybegintime   ...!?   ARAR
           )
        {  candidat = wi;
           wtime = wprectime;
        }
    }
    return(candidat);
}
/*---------------------------------------------------------------------------*/
void Ybackward(int itraj, int topstop) //meme principe que forward
{  int itrajelue, wi;
	
	 /* passe arriere sur l'ensemble du modele global (i.e. sur ttes les trajectoires )
	    de la ou on est positionned ! */
		
	 //YCaseCost = COSTL2_GRAD;	//opera
	 				  		
	 for (wi=0; wi<YNBTRAJ; ++wi) 		/* init des pointeurs des arbo d'observations  */
	 {   YTabTraj[wi].TRobs = YTabTraj[wi].YRobs->fils;    //TRobs = YRobs->fils;
	 }

   /* passe arriere sur tout le modele global (i.e. sur ttes les trajectoires ) */
    while ((itrajelue=Ybackward_elect()) >= 0)  // le while controle la progression
		{   //ici, on ne recule QUE d'UN PDT sur la trajectoire selectionnee !
				YidTraj = itrajelue;                    //(ctx pour les modules par exemple)
	      YTemps = YTabTraj[itrajelue].toptime;   //(ctx pour les modules par exemple)
				if (itrajelue==itraj && YTemps <= topstop) return; //eventuel arret sur une traj de ref

        {--YTemps; --YTabTraj[itrajelue].toptime; 			//<-: --YTemps
         YTabTraj[itrajelue].curtime -= YTabTraj[itrajelue].dtime; //<-: multi
         YNBUPTIME = YTabTraj[itrajelue].nbuptime;
        }
        if (YDispTime)
        {  printf (" >>> Traj %s, Current backward time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
					 //Ydispcurstep(itrajelue);
        }

				backward_before (0); /* arar *//* doit obligatoirement exister ds le user's global ? */
				/*---------------------*/
				if (YTypeCost == COST_APPLI)		/* l'utilisateur devra prendre totalement */
					 cost_function (YTemps);	 		/* en charge le calcul du cout !          */
				else
				{  TRobs = YTabTraj[itrajelue].TRobs;  /* on se positionne sur le top d'obs courant de la trajectoire elue */
					 //YCaseCost = COSTL2_GRAD;
				   //Ycobs();			/* calcul du cout au pas de temps (YTemps) courant (s'il y a lieu) */
				   //YTabTraj[itrajelue].TRobs = TRobs;    /* pour avancer sur cette liste des temps */
					 if (TRobs!=NULL && TRobs->iind==YTemps) /* si cette traj a des obs au temps courant */
					 {  Ycost();                             /* calcul du cout */
				      YTabTraj[itrajelue].TRobs = TRobs;   /* pour avancer sur cette liste des temps */
				   }
				}
				/*---------------------*/
				YTabTraj[itrajelue].bward(0); //<-: backward_order du modele sur la trajectoire
				Yrazgrad_only(itrajelue);   		 /* apres backprop, mise a 0 du gradient pour tous les modules qui n'ont pas de temps de la trajectoire ! */
				backward_after (0);   /* arar */ /* doit obligatoirement exister ds le user's global ? */
    }
}
/*................................................................................*/
void Ybackwardrun(int itraj, int nbpdt)
{   long topend;
		Ytop0 = time((time_t *)NULL);
		
		if (YTabTraj[itraj].bward==NULL)
		{  printf("no backward function defined for that trajectory\n");
		   return;
		}		
		if (YTabTraj[itraj].toptime - nbpdt < YTabTraj[itraj].nbuptime)
		{	 printf ("required backward step out of lower limit: \n");
		   printf ("         current top (%i) - required step (%i) < nbuptime (%i) \n",
				        YTabTraj[itraj].toptime, nbpdt, YTabTraj[itraj].nbuptime);
			 return;
		}
    //YCaseCost = COSTL2_GRAD; ne devrait plus etre utile ici puisque reposisionned apres par Ycost !!!
		TRobs = YTabTraj[itraj].YRobs->fils; //TRobs = YRobs->fils; init du pointeur sur l'arbo des observations

    YidTraj = itraj; //(ctx pour les modules par exemple)
		YTemps = YTabTraj[itraj].toptime; //(ctx pour les modules par exemple)
		YNBUPTIME = YTabTraj[itraj].nbuptime;

		for (int Ywt=1; Ywt<= nbpdt; ++Ywt)
		{   {--YTemps; --YTabTraj[itraj].toptime; //ctx //<-: --YTemps
         YTabTraj[itraj].curtime -= YTabTraj[itraj].dtime; //<-: multi
        }
        if (YDispTime) printf (" >>> Traj %s, Current backward time = %i \n", YTabTraj[itraj].name, YTemps+1);
        backward_before(Ywt);
				if (YTypeCost == COST_APPLI)
				   cost_function (YTemps);
				//else Ycobs();
				else //si cette traj a des obs au temps courant alors calcul du cout (?)
				{  if (TRobs!=NULL && TRobs->iind==YTemps)
				      Ycost(); //nb c'est Ycost qui avance TRobs (TRobs=TRobs->frere)
							YTabTraj[itraj].TRobs = TRobs; //pour avancer sur cette liste des temps: est-ce bien utile
        }                                    //puisque, ici, il n'y a qu'une trajectoire.
				YTabTraj[itraj].bward(0); //<-: backward_order
        Yrazgrad_only(itraj); //razgrad_only que sur la trajectoire concernee !
        backward_after(Ywt);
		}
		after_it(1);

		topend = time((time_t *)NULL);
		printf("backward duration ....: %lds\n", topend-Ytop0);
		return;
}


/* ----------------------------------------------------------------------------
//============================= FONCTIONS LINWARD ========================== */
#ifdef YO_GRADTEST
#define YO_LINWARD
#endif
#ifdef YO_VARINCR
#define YO_LINWARD
#endif
#ifdef YO_LINWARD
void Ylinward(int bidon) //int bidon : pour rester compatible en attendant que tout soit termine !!!
{   int itrajelue;	
	  // passe avant lineaire sur l'ensemble du modele global (i.e. sur ttes les trajectoires )
	  // de la ou on est positionned !
    while ((itrajelue=Yforward_elect()) >= 0)   // le while controle la progression
		{   //ici, on n'avance QUE d'UN PDT sur la trajectoire selectionnee !
				
				YTemps = YTabTraj[itrajelue].toptime; //(ctx pour les modules par exemple)
				YidTraj = itrajelue;                  //(ctx pour les modules par exemple)

        if (YDispTime) printf (" >>> Traj %s, Current linward time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
        forward_before(0); //arar
				YTabTraj[itrajelue].lward(0); //<-: linward_order
				forward_after(0); //arar
        {++YTabTraj[itrajelue].toptime; //<-: ++YTemps
         YTabTraj[itrajelue].curtime += YTabTraj[itrajelue].dtime; //<-: multi
        }
    }
}
		/* faut-il ou pas mettre les forward_before et forward_after ? */
		/*? plutot NON a priori car les etats sont deja calcules !  ...*/
		/* et finalement plutot OUI car qui sait si l'on aurait pas,   */
		/* par exemple, besoin de forcage dans le calcul des derivees !*/
		/* et puis qui peut le + peut le - */
#endif
/*---------------------------------------------------------------------------*/
#ifdef YO_VARINCR
void Yset_qteaebx() //calcul de qtea sur la partie ebauche seulement
{   int wi;
    for (wi=0; wi<YNBTRAJ; ++wi)
    {   TRobs = YTabTraj[wi].YRobs->fils;
        while (TRobs != NULL)
        {   if (TRobs->iind < 0)
            {  Yrovobs_TR(TRobs, Yc_dstar_all, 9);
            }
            TRobs = TRobs->frere;
        }         
	  }
}
/*---------------------------------------------------------------------------*/
void Ylinward_flopobs(int bidon) //int bidon : pour rester compatible en attendant que tout soit termine !!!
{   //YLINWARD sur le modele +, et en passant, si y'a des obs pour un modules :
    //                          Forward+Linward des OPerateurs associed aux modules qui ont des OBS
    int itrajelue, wi;

	  for (wi=0; wi<YNBTRAJ; ++wi) 		/* init des pointeurs des arbo d'observations  */
    {   YTabTraj[wi].TRobs = YTabTraj[wi].YRobs->fils;    //TRobs = YRobs->fils;
	  }

	  /* passe avant lineaire sur l'ensemble du modele global (i.e. sur ttes les trajectoires )
	    de la ou on est positionned ! */
    while ((itrajelue=Yforward_elect()) >= 0)   // le while controle la progression
		{   //ici, on n'avance QUE d'UN PDT sur la trajectoire selectionnee !

				YTemps = YTabTraj[itrajelue].toptime; //(ctx pour les modules par exemple)
				YidTraj = itrajelue;                  //(ctx pour les modules par exemple)

        if (YDispTime) printf (" >>> Traj %s, Current linward time = %i \n", YTabTraj[itrajelue].name, YTemps+1);
        forward_before(0); //arar
				YTabTraj[itrajelue].lward(0); //<-: linward_order

        /* because opera ... */
			  TRobs = YTabTraj[itrajelue].TRobs;      /* on se positionne sur le top d'obs courant de la trajectoire elue */
				if (TRobs!=NULL && TRobs->iind==YTemps) /* si cette traj a des obs au temps courant */
				{
				   Yact_operator('*', OFF);
		       YCaseCost = WISHL2_NANACT; /* pour WISH: ca mettra NAN (niveau module) */
		       Ycobs_TR(TRobs->fils);			/* puis ca active les operateurs associed au module (H, R, ...) */
		       		       
           Yforward_operator('H'); 	  /* il faut faire forward avant pour avoir les etats H(M(x)) ensuite on peut faire linward */
           Ylinward_operator('H'); 	  /* je ne lance pas les autres operateurs (B,R,K) car ce sont des operateurs de covariance
                                         dont les modules ne devraient pas etre cout (bien que plm, ce n'est pas verified) */				
				   Yrovobs_TR(TRobs, Yc_dstar_all, 0);	/* pour calculer qtea=H'M'(Xk).dx - (y° - H(M(Xk)) */				
				   YTabTraj[itrajelue].TRobs = TRobs->frere;  /* pour avancer sur cette liste des temps */
				}
								
				forward_after(0); //arar
        {++YTabTraj[itrajelue].toptime; //<-: ++YTemps
         YTabTraj[itrajelue].curtime += YTabTraj[itrajelue].dtime; //<-: multi
        }
    }
}
		/* faut-il ou pas mettre les forward_before et forward_after ? */
		/*? plutot NON a priori car les etats sont deja calcules !  ...*/
		/* et finalement plutot OUI car qui sait si l'on aurait pas,   */
		/* par exemple, besoin de forcage dans le calcul des derivees !*/
		/* et puis qui peut le + peut le - */
#endif
/* ----------------------------------------------------------------------------
//================================= Iteration de base ====================== */
/*
void old_Ybasic_it()
{
		long	curtop;
					
		Yset_modeltime(0);
		before_it(YItRun); 	// doit obligatoirement exister ds le user's global ?

		if(YAL2Run==RUNL2_STD)
			Yforward (-1, 0); //Yforward (YNBSTEPTIME);
#ifdef YO_VARINCR
		else if(YAL2Run==RUNL2_INCR)
		{
      Yset_qteaebx();      //pour calculer qtea sur la partie ebauche
      Ylinward_flopobs(0); //Lineaire tangent pour calculer qtea=H'M'(Xk).dx - (y° - H(M(Xk)) sur la trajectoire
		}
#endif

		Yrazgrad_all();  // avant fct de cout et backprop : sur tous les pas de temps, raz de tous les gradients de tous les modules
		//YTotalCost = YTotGrCost = 0.0;	// Raz aussi du Cout et de son Gradient total avant les calculs de cout
		YTotalCost = 0.0;	// Raz aussi du Cout avant les calculs de cout
		Ybackward(-1, 0); // Ybackward (YNBSTEPTIME);// AD (adjoint):-> d*x =M*(X).dX : Yjac * YG -> Ytbeta

		// calcul du cout et gradient sur les termes d'ebauches et obs initiales
		if (YTypeCost==COST_APPLI)		 // l'utilisateur devra prendre totalement
					cost_function (-1); //(YUPTIME-1); // en charge le calcul du cout !  (partie<uptime)
		else
		{			//Ycobx();	calcul du cout et gradient sur les termes d'ebauches et obs initiales
					Ycostebx();  // calcul du cout et gradient sur les termes d'ebauches et obs initiales
		}
		
		if (YDispCost==2)	//LAST
		{	 curtop = time((time_t *)NULL);
		 	 printf(" >>> at iter %3ld after %lds: Total cost = % -23.15e\n", YItRun, curtop-Ytop0, YTotalCost);
		}			

		if(YAL1Run==RUNL1_STD)
		{ if(YAL2Run==RUNL2_STD)
				Yadjust();
#ifdef YO_VARINCR
			else if(YAL2Run==RUNL2_INCR)
				Yc_adjustd();
#endif
			after_it(YItRun);				// doit obligatoirement exister ds le user's global ?
		}	
} ----------------------------------------------------------------------------*/
void Ybasic_it()
{
		long	curtop;

		double sumJ = 0.0;				/* cumul (summation) de la fonction de cout J (i.e. YTotalCost) */
    YREAL *TGSumTarget=NULL;	/* Tableau de cumul (SUMmation) des Gradients des Target */

		before_it(YItRun); 	/* doit obligatoirement exister ds le user's global ? */

    if (YNbBasicIntL>1)
	  {		//YREAL *TGSumTarget=new YREAL[YSIZEPB];	// nb: ne pas declarer TGSumTarget ici, sinon ce reste local au if {!!!}
				TGSumTarget = new (std::nothrow) YREAL[YSIZEPB];		// tableau de cumul des gradient des variables a controler
				if (TGSumTarget==NULL){printf("Yao=>ABORT: pb on new TabYG in Ybasic_it (see Yao system administrator)\n"); exit(-9);}
				memset(TGSumTarget, 0, YSIZEPB*sizeof(YREAL));
    }

		//for (int YItBasicIntL=1; YItBasicIntL<=YNbBasicIntL; ++YItBasicIntL) //BASIC INTERN LOOP ......................
		//      |_> was local !!!
		for (YItBasicIntL=1; YItBasicIntL<=YNbBasicIntL; ++YItBasicIntL) //BASIC INTERN LOOP ......................
    {
				Yset_modeltime(0);
				//before_it(YItRun); 	/* doit obligatoirement exister ds le user's global ? */

				if(YAL2Run==RUNL2_STD)
					Yforward (-1, 0); //Yforward (YNBSTEPTIME);
		#ifdef YO_VARINCR
				else if(YAL2Run==RUNL2_INCR)
				{  //nb:dans le cas incremental, le forward est supposed avoir deja ete fait par Yforwardcost dans la boucle externe
					 Yset_qteaebx();      //pour calculer qtea sur la partie ebauche (qtea = dx-(Xb-Xk) = YGo-(Xb-YSo))
					 Ylinward_flopobs(0); //Lineaire tangent pour calculer qtea=H'M'(Xk).dx - (y° - H(M(Xk)) sur la trajectoire
				}
		#endif

				Yrazgrad_all();  /* avant fct de cout et backprop : sur tous les pas de temps, raz de tous les gradients de tous les modules */
				YTotalCost = 0.0;	/* Raz aussi du Cout avant les calculs de cout */
				Ybackward(-1, 0); // Ybackward (YNBSTEPTIME);/* AD (adjoint):-> d*x =M*(X).dX : Yjac * YG -> Ytbeta */

				/* calcul du cout et gradient sur les termes d'ebauches et obs initiales */
				if (YTypeCost==COST_APPLI)		 /* l'utilisateur devra prendre totalement */
							cost_function (-1); //(YUPTIME-1); /* en charge le calcul du cout !  (partie<uptime)   */
				else
				{			//Ycobx();	calcul du cout et gradient sur les termes d'ebauches et obs initiales
							Ycostebx();  /* calcul du cout et gradient sur les termes d'ebauches et obs initiales */
				}

				if (YNbBasicIntL>1)
				{  sumJ += YTotalCost; 			//cumul de la fonction de cout
					 //cumul des gradients
					 Y3windice=0;
           YgradCTOtab_target(TGSumTarget);			/* Cumul de YGo dans [TGSumTarget]  */
				}
		} //FIN INTERN LOOP ..................................................................

		if (YNbBasicIntL>1)
	  {  //on transfert les cumuls de la fonction de cout dans la fonction de cout
		   YTotalCost = sumJ;
       //on transfert les cumuls des gradients dans les zones YG
			 Y3windice=0;
       YtabTOgrad_target(TGSumTarget);
		   if (YDispCost==ON)	//=1
		   {	//curtop = time((time_t *)NULL); on affiche le cout cumule
		 	    printf(" >>> Cumul Total cost after basic internal loop = % -23.15e\n", YTotalCost);
		   }
		}

		if (YDispCost==2)	//LAST
		{	 curtop = time((time_t *)NULL);
		 	 printf(" >>> at iter %3ld after %lds: Total cost = % -23.15e\n", YItRun, curtop-Ytop0, YTotalCost);
		}

		if(YAL1Run==RUNL1_STD)
		{ if(YAL2Run==RUNL2_STD)
				Yadjust();
#ifdef YO_VARINCR
			else if(YAL2Run==RUNL2_INCR)
				Yc_adjustd();
#endif
			after_it(YItRun);				/* doit obligatoirement exister ds le user's global ? */
		}

		if (YNbBasicIntL>1) delete[] TGSumTarget;  /* on libere TGSumTarget */
}

/* ----------------------------------------------------------------------------
//================================= FONCTION RUN =========================== */
void Yrun(long Yit)
{
		long topend;
		Ytop0 = time((time_t *)NULL);
  	printf("\nstart run time: %s\n", ctime(&Ytop0));

		YY_RUNBRK=OFF;	/* pour gere le break d'un run en interactif */

		/* YTypeMode=MODE_STD; gestion standard de l'adjust et de l'after_it */
		YAL1Run=RUNL1_STD;	// Run niveau 1 standard
		YAL2Run=RUNL2_STD;	// Run niveau 2 standard

		for (YItRun=1; YItRun<=Yit; ++YItRun)
		{		Ybasic_it();
				if (YY_RUNBRK) break;
		}
  	printf("\nstart run time ....: %s", ctime(&Ytop0));
  	topend = time((time_t *)NULL);
	  printf("  end run time ....: %s", ctime(&topend));
		printf("  run duration ....: %lds\n\n", topend-Ytop0);
		YY_RUNBRK=ON;
}

/* ============================================================================
// POUR RUNNER AVEC M1QN3 ... a voir a faire (commentaires: ../omean/)			 */
#ifdef YO_M1QN3
#include "Dynmqn.h"
#endif

/* ============================================================================
// FONCTIONS DE TEST POUR VALIDATION                                         */
#ifdef YO_GRADTEST
#include "Dyntst.h"
#endif

/* ============================================================================
//                      IMPLEMENTATION POUR L'ALGO L'INCREMENTAL             */
#ifdef YO_VARINCR
/* --------------------------------------------------------------------------*/
void Yc_run(long Yit)					//runi
{
		//int  witraj;
		int  extl, intl;
		long topend;
		Ytop0 = time((time_t *)NULL);
  	printf("\nstart run time: %s\n", ctime(&Ytop0));

		YY_RUNBRK=OFF;      /* pour gere le break d'un run en interactif */
		YAL1Run=RUNL1_STD;  /* run de niveau 1 standard */
		YItRun = 1;         /* init increment des iteration */

		//INITIALISATION:--------------------------------------
		//1) On suppose X(k=0) initialised par l'utilisateur (setstate ou xuserfct)
    //2) On suppose que les obs (y°) et les ebauches ebx (Xb) ont ete charges (comme Nal et d'hab)
    //   avec loadobs, outoobs, (loadstate) outoebx
    //3) On suppose que l'utilisateur a positionned avec set_pcoef et pour chaque target module,
    //   les %ages qui serviront a initialiser les dx (avec la fct Ysetpdelta) dans la boucle externe

    //BOUCLE EXTERNE ET INTERNE:---------------------------
		//boucle EXTERNE: .....................................
		for (extl=1; extl<=YNbExtL; ++extl)
		{ 	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ extern loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf(" BE:%3i", extl);

				/* A) MD (Modele Direct):-> M(Xk) : states : [X] */
				YAL2Run=RUNL2_STD; /* run de niveau 2 standard pour forwardcost*/
				Yobs_reverse_time_lists();	// inversion de l'ordre du temps (descending->ascending)
				Yforwardcost();						  // passe avant (=>les YS sont calculed) avec calcul du cout sur la vraie fonction
				Yobs_reverse_time_lists();	// re-inversion de l'ordre du temps (ascending->descending)
				YAL2Run=RUNL2_INCR; /* mais run de niveau 2 incremental pour l'incremental */

				//B) SET de dx : ca depend de X et pcoef                                 
        //for (witraj=0; witraj<YNBTRAJ; ++witraj)
        //  YdeltaEQPCstate_traj(witraj, "Y#T", 0, YTabTraj[witraj].nbuptime);
        YdeltaEQPCstate_target(); //V7: YdeltaEQPCstate_all ("Y#T", 0, YUPTIME);	//%1
				
				//C) boucle INTERNE: .................................
				for (intl=1; intl<=Yit; ++intl)
				{
						printf(" BI:%3i", intl);
						++YItRun;

						//0) RAZ des zones Ygrad car ils vont etre propaged par le LT. en particulier
						//   et initialement les modules non target doivent avoir un dx=0 (isn-it).
						Yrazgrad_all();

						//1) Transfert dans la zone Ygrad des Ydelta initialement affectes par YdeltaEQPCstate, puis ...?
						//   car c'est cette zone la (Ygrad) qui est propagee par le LT.
            YgradEQPdelta_target(1.0);

//----------------------
						Ybasic_it();
//----------------------
						if (YY_RUNBRK) break;

				}//fin boucle INTERNE ..............................

				//suite boucle EXTERNE .............................
				//D) Xk+1 = Xk + dx
				Yc_adjustk_all ();

				if (YY_RUNBRK) break;

		}//fin boucle EXTERNE .................................
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ end extern loop ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

		/* cout sur la vraie fonction */
		printf(" BE:%3i", extl-1);
		YAL2Run=RUNL2_STD; /* run de niveau 2 standard pour forwardcost*/
		Yobs_reverse_time_lists();	// inversion de l'ordre du temps (descending->ascending)
		Yforwardcost();						// passe avant avec calcul du cout
		Yobs_reverse_time_lists();	// re-inversion de l'ordre du temps (ascending->descending)	

  	printf("\nstart run time ....: %s", ctime(&Ytop0));
  	topend = time((time_t *)NULL);
	  printf("  end run time ....: %s", ctime(&topend));
		printf("  run duration ....: %lds\n\n", topend-Ytop0);
		YY_RUNBRK=ON;
}

// fin du code specifique pour l'INCREMENTAL (si YO_VARINCR)
#endif
/*-------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
//============== echantillonnage fonction de cout pour plotting ============= */
int Ysampleof(char *modP, double Pinf, double Psup, double dP,
              char *modQ, double Qinf, double Qsup, double dQ, char *nmfile)
{
    FILE  *fpout;
    int   iPmod, iQmod;
    YREAL Pval, Qval, SUPP, SUPQ;
    long  topend;
    int		iP=0, iQ=0;
    int   iPtraj, iQtraj; /* indices des trajectoires des modules */

    /* INIT et VERIF */
    iPmod = Yimod(modP);
    iQmod = Yimod(modQ);

    if (iPmod<0 || iQmod<0)
    {  printf("sampleof(i): unknown module (%s or %s)\n", modP, modQ);
       return(0);
    }

	  if (iPmod==iQmod)
    {  printf("sampleof(i): Oh Oh! a modul with it-self !!! \n");
    }

	  /*if (YioInsertObsCtr == -1)	// il faut des obs pour pouvoir calculer un cout !
	  {  printf("sampleof(i): you may had load obs\n");
	     return(0);
	  } cf ci-dessous */

    if (Pinf>Psup || Qinf>Qsup)
    {  printf("sampleof(i): sup values must be greater (or equal) than inf values\n");
       return(0);
    }

    if (dP<=0. || dQ<=0.) //pour pas boucler
    {  printf("sampleof(i): step values must be greater than 0\n");
       return(0);
    }

    if ((fpout = fopen(nmfile, "w")) <= 0)
    {  printf ("sampleof(i): open file %s failed \n", nmfile);
       return (0);
    }

		Ytop0 = time((time_t *)NULL);
  	printf("\nstart sampleof(i) time: %s\n", ctime(&Ytop0));

  	iPtraj = Yitrajimod(iPmod);  /* indice des traj */
  	iQtraj = Yitrajimod(iQmod);  /*   des modules   */

		//Yobs_reverse_time_lists();		  //inversion de l'ordre du temps -> ascending
		if (Yobs_reverse_time_lists()==0) //inversion de l'ordre du temps -> ascending
    {  // il faut des obs pour pouvoir calculer un cout !
       printf("sampleof(i): you may had load obs\n");
	     return(0);
    }
		YItRun =1;

		if (YAL2Run==RUNL2_INCR)
		{	 Yset_modeltime(0);
			 before_it(YItRun);
			 Yforward(-1,0);    //hyp:Xo initialised et on calcule YS pour avoir YSn = M(Xo)
		}

	  /* boucle de calcul du cout pour chaque couple de valeur sur la plage donnee */
    Qval = Qinf;
    iQ = 0;
		SUPQ = Qsup + (Qsup * 1.0e-16); //pb of machine precision !!??
		while (Qval <= SUPQ)
    {		if (YAL2Run==RUNL2_STD)
    			 Ysetstate_mod (modQ, Qval);
#ifdef YO_VARINCR
    		else if (YAL2Run==RUNL2_INCR)
					 YgradEQval_traj(iQtraj, modQ, YTabMod[iQmod].deb_target, YTabMod[iQmod].end_target, Qval);
#endif
    		Pval = Pinf;
    		iP = 0;
				SUPP = Psup + (Psup * 1.0e-16); //pb of machine precision !!??
				while (Pval <= SUPP)
        { 		
						if (YAL2Run==RUNL2_STD)
    			     Ysetstate_mod (modP, Pval);
#ifdef YO_VARINCR
    		    else if (YAL2Run==RUNL2_INCR)        			
					     YgradEQval_traj(iPtraj, modP, YTabMod[iPmod].deb_target, YTabMod[iPmod].end_target, Pval);
#endif
            Yforwardcost();	//calcul du cout en passe avant
						++YItRun;
            //fprintf(fpout, "%e \t", YTotalCost);fflush(fpout);
            fprintf(fpout, "% 23.15e \t", YTotalCost);fflush(fpout);
            printf("o"); fflush(stdout);
            ++iP;
						Pval = Pinf + (dP * iP);
        }
        fprintf(fpout, "\n"); printf("\n");
        ++iQ;
				Qval = Qinf + (dQ * iQ);
    }
		printf("sampleof(i): dim deduced: %i lines, %i columns\n", iQ, iP);

		/* On laisse les choses comme on les a trouved */
		Yobs_reverse_time_lists();	// re-inversion de l'ordre du temps -> descending

    fclose (fpout);

		//printf("\nsampleof(i): completed,\n");
    printf("sampleof(i) start time ....: %s", ctime(&Ytop0));
  	topend = time((time_t *)NULL);
	  printf("  sampleof(i) end time ....: %s", ctime(&topend));
		printf("  sampleof(i) duration ....: %lds\n\n", topend-Ytop0);

    return (1);
}
/*-------------------------------------------------------------------------- */
/* ///////////////////////////////////////////////////////////////////////// */




/* ////////////////////////////////////////////////////////////////////////////
//============================== INTERPRETEUR =================================
// PROCEDURE RUN : c'est un interpréteur de fichier d'instructions !!!
//                 ou de commandes intercatives !!!
//=============================================================================
//////////////////////////////////////////////////////////////////////////// */
/* SOME PROTOTYPAGE */
//int 	Yrecup_cde (int *cdesc, char *cdeline, char *cdes[]); -> Dynamo.h
int 	Yinterprete_cde(int cdesc, char *cdes[]);
/* SOME OTHER GLOABLE */
int   Yrecurlev=0; 					  /* niveau d'appel recursif de load_inst */
int   Yerr1tpret=0; 					/* flag d'erreur */
char	cdeline[BUFSIZE + 1];		/* ligne de commande lue */
char  *pcdeline;							/* pointeur sur la ligne de commande lue */
char  *cdes[NB_MAX_ARG_CDE];	/* tableau des arguments de la commande */
int   cdesc;									/* nombre d'argument de la commande */

/* __________________________________________________________________________*/
void Ysignal(int isig)
{	/* interception de ctrl C pour breaker un run en cours */
	if (YY_RUNBRK) exit(0);
	printf("Break required; will act at next iteration\n");
	YY_RUNBRK=ON;
}

/* __________________________________________________________________________*/
int Yload_inst (FILE *fpinst, char *nmfile, int *nbenrlus)
{	/* lecture et execution des instructions qui sont dans le fichier nmfile */
	int			goinglabel=0;
	char    label[LG_MAX_NAME+1];
	int			wi;		

	while (fgets (cdeline, BUFSIZE, fpinst) != NULL)
	{
			++*nbenrlus;
			
			if (cdeline[strlen(cdeline)-1] == '\n')			
				 cdeline[strlen(cdeline)-1] = '\0';
			/* ?strupr(cdeline); */
			pcdeline = cdeline;
			
			/* positionnement en debut de ligne. i.e.: 1er caractere significatif */
			while (pcdeline[0] == ' ' || pcdeline[0] == '\t') ++pcdeline;
			
			if (pcdeline[0] == '#') /* commentaire en debut de ligne (#) */
					continue;
					
			wi=0;													/* commentaire en fin de ligne (|)*/
			while (pcdeline[wi]!='\0')
			{ if (pcdeline[wi]=='|') {pcdeline[wi]='\0'; break;}
  			++wi;
			}

			if (pcdeline[0] == '!' && !goinglabel) /* commande systeme */
			{		system (&pcdeline[1]);
					continue;
			}
			if (YEcho && !goinglabel) printf ("Yao#>%s\n", cdeline);
			if (!Yrecup_cde (&cdesc, pcdeline, cdes))
			{	 printf ("inst: file %s, commande %s, ligne %i : error found=>stop \n", nmfile, cdes[0], *nbenrlus);
				 return(0);
			}
			if (cdesc == 0)
				continue;
				
			/* traitement d'un goto label */
			if (goinglabel)
			{  if (!strcmp(cdes[0], label)) goinglabel=0;
			   continue;
			}
			if (!strcmp(cdes[0], "GOTO") || !strcmp(cdes[0], "goto"))
			{ if (cdesc != 2)
			  { printf("inst: file %s, line %i, goto must be folling by one label \n", nmfile, *nbenrlus);
			    return (0);
			  }
			  strcpy(label, cdes[1]);
			  goinglabel = 1;
			  continue;
			}
		
			/* inserer ici (si besoin) un upercase de l'instruction (cdes[0]) */
			if (!Yinterprete_cde(cdesc, cdes))
			{	 if(!Yerr1tpret)
			     printf ("inst: file %s, commande %s, ligne %i : error found=>stop \n", nmfile, cdes[0], *nbenrlus);
				 Yerr1tpret=1;
				 return(0);
			}
	}

  fclose(fpinst);
	return(1);
}

/* ============================================================================
// LE PROGRAMME MAIN IT-SELF """"""""""""""""""""""""""""""""""""""""""""""" */
int Yboot_appli (int argc, char *argv[])
{
  int   wi;                    /* variable de travail local (indice de boucle, ...) */

	/* SOME INIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	Yszint  = sizeof(int);			/* taille d'un int */
	Yszreal = sizeof(YREAL);		/* taille d'un reel pour les states (...) {float ou double} */
	srand( (unsigned)time( NULL ) );
	YDispTime = OFF;
	YDispCost = OFF;
	sprintf(YPrompt, "Yao:%s> ", PRJNAME);

	for (wi=0; wi<YNBTRAJ; ++wi)
	{ YTabTraj[wi].YRobs = (struct Yst_nodo*) malloc(sizeof(struct Yst_nodo)); /* init du Root des arborescences */
	  YTabTraj[wi].YRobs->frere = YTabTraj[wi].YRobs->fils = NULL;	/* d'observations pour chaque trajectoire */
	}

	memset(Yting, 0, YMAX_NBI*sizeof(YREAL));							    // y'avait double au lieu
	//memset(Yjac, 0, YMAX_NBS*YMAX_NBI*sizeof(YREAL));			  // de YREAL et ca provoquait
	memset(Yjac, 0, YMAX_JAC_NBS*YMAX_JAC_NBI*sizeof(YREAL));	// de YREAL et ca provoquait
	memset(Ytbeta, 0, YMAX_NBI*sizeof(YREAL));						    // un plantage quand on etait en float !!!

	/* pour intercepter ctrl C lors d'un run et l'arreter */
	signal(SIGINT, Ysignal);

	/* message de demarrage */
	printf ("\nStarting Yao : %s ", VERREF);
	printf ("\nProject %s. \n(You are in ", PRJNAME);
	if (sizeof(YREAL)==sizeof(double)) printf("double mode)\n"); else printf("float mode)\n");

#ifdef PARALLEL
	cout << "This is a parallel version of the application, the max number of threads that the system supports are: " 
	  << omp_get_max_threads() << "." << endl;
#endif

	/* CREATION DU MODELE: (le fait on systématiquement ou est-ce au user de   */
	Ycreate_all ();			 /* le mettre dans les instructions? Quid Initialiser? */

	/* Une fonction de demarrage pour l'appli specifique du user; */
	appli_start	(argc, argv);

  return (0);
}
/*----------------------------------*/
int Yentry1 (int argc, char *argv[]); //prototypage de Yentry1 qui prolonge Yentry0 !
int Yentry0 (int argc, char *argv[])
{
	/* l'ancienne syntaxe etait 	:
	//				prjname					:=> execution en batch avec le fichier prjname.i
	//   ou   prjname nmfile	:=> execution en batch avec le fichier nmfile
	//   ou   prjname [] [-i]	:=> execution en mode interactif 								 */
	/* NOUVELLE SYNTAXE :
	          prjname [...] [-b -e -i [fichier_d'instruction]]
		 OPTION :
		 	-b 		: boot de l'appli ((some init, Creat, appli_start)
			        sans execution d'instruction aucune.
			-e    : execution d'instructions (cf ci-apres) ce qui suppose que
			        le boot de l'appli (option -b) est deja fait.
			Par defaut :  -b  et  -e

			-i [nmfile]
					si nmfile n'est pas fourni
					   alors : on execute en mode Interactif
					si nmfile est fourni et qu'il ne correspond pas a un fichier existant
					   alors : on execute en mode Interactif
					si nmfile est fourni et qu'il correspond a un fichier qui existe
					   alors : l'appli est lancee en Batch avec nmfile pour fichier d'instructions
			Par defaut : on essaie de lancer en Batch avec prjname.i si il existe, sinon -> erreur
	*/
	/* gestion des options */
	int optb=0, opte=0, opti=0;
	int wi=0;
	while (wi<argc)		//boucle de positionnement des options
	{		if (!strcmp(argv[wi], "-b"))
			{	 optb=1;
			}
	    else if (!strcmp(argv[wi], "-e"))
			{	 opte=1;
			}
	    else if (!strcmp(argv[wi], "-i"))
			{	 opti=wi; //ici, on memorise l'indice de l'argument
			}
			++wi;
	}
	/* gestion des options */
	if (optb==0 && opte==0)
	{  //par defaut :-> b et e
	   optb=opte=1;
	}
	if (opti>0 && !opte)
	{  //l'option -i n'a de sens que pour une execution
	   printf(" -i option has no sens without execution\n");
		 return(0);
	}
  if (optb)
	{  Yboot_appli(argc, argv);	//boot, ammorcage, init, creat de l'appli
	}
	//if (opti>0) //fichier d'instruction ...{} : c'est dans Yentry1 que ca se passe
  if (opte)
	{  Yentry1(argc, argv);			//execution de l'appli
	}

	/* decoupage supplementaire pour pouvoir eviter d'allouer a chaque fois
	   un nouvel espace par Ycreate_all lorsque une appli est appeled plusieurs
		 fois par une autre, ... de plus, ainsi on evite que les modules soit remis
		 a 0 entre 2+ appels ...
	   Tout cela amene a definir un nouveau mode de lancement qu'on pourrait par
	   exemple appeler un mode de boot uniquement de l'appli : option "-b"
		 Donc l'option -b ne doit etre utilise qu'une et une seul fois pour toutes
		 puis ensuite, on devrait pouvoir utilise l'option -e autant de fois que necesaire !? */
	return(0);
}
/*----------------------------------*/
int Yentry1 (int argc, char *argv[])
{
  char	fileinst[STRSIZE80+1]; /* fichier d'instructions initial */
  FILE  *fpinst=NULL; 				 /* file pointeur du fichier d'instruction */
  int   nbenrlus;							 /* nbr d'enr du fichier d'instruction lus */
	int   wi=0;

	/* determination du mode d'execution (Interactif ou batch) et fichier d'instructions */
	YM_EXE=0;
	while (wi<argc)
	{  //on recherche l'indication du fichier d'instruction qui desormais peut se trouver n'importe ou parmi les options
	   if (!strcmp(argv[wi], "-i"))
		 {  ++wi;
		    if (wi<argc)
				{		strcpy (fileinst, argv[wi]); //le parametre qui suit -i pourrait etre un fichier d'instruction
						if ((fpinst = fopen(fileinst, "r")) <= 0) 	//ce qui suit n'est pas un fichier qui
	  				{	 YM_EXE = 'I';														//existe => mode d'execution Interactif !
	  				}
						else //mode d'execution Batch
						{  YM_EXE = 'B';
						}
				}
				else //par d'argument apres -i => mode d'execution interactif !
				{	 YM_EXE = 'I';
				}
				break;
		 }
		 ++wi;
	}
	if (YM_EXE==0) //=> -i n'a pas ete rencontre; si un fichier prjname.i existe,
	{							 //   on l'executera en mode Batch sinon erreur
			sprintf (fileinst, "%s.i", PRJNAME);
			if ((fpinst = fopen(fileinst, "r")) <= 0)
	  	{	 printf ("instruction file %s not found \n", fileinst);
		   	 return(0);
	  	}
			YM_EXE = 'B';
	}

	/* Execution des instructions :~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (YM_EXE=='B')
	{	nbenrlus=0;
	  ++Yrecurlev;
		if (!Yload_inst(fpinst, fileinst, &nbenrlus)) // on s'arrete a la 1ere erreur,
		{	 printf("-error found while file %s, line %i. sequence aborted \n", fileinst, nbenrlus);
			 exit(9);														
		}  /* de toute facon, on s'arrete a la fin */			
	}
	else if (YM_EXE=='I')
	{	/* En mode INTERACTIF, on entre dans une boucle "sans fin..." */
		printf("\n\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("         Yao welcome you friendly      \n");
		printf("       and wish you a happy new year    \n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
		while (1)
		{	printf("%s", YPrompt);	/*le prompt */
			/* printf("yao> "); //le prompt */

			/* récupération de la commande gets (cdeline); */
			fgets(cdeline, BUFSIZE, stdin);

			/* strupr (cdeline); */
			pcdeline = cdeline;
			while (pcdeline[0] == ' ') ++pcdeline;
		
			/* commande systeme */		
			if (pcdeline[0] == '!')
			{		system (&pcdeline[1]);
					continue;
			}
			
			/* commande yao */
			if (!Yrecup_cde(&cdesc, pcdeline, cdes))			
				 continue;

			if (cdesc == 0)
				 continue;
			if (strcmp(cdes[0], "EXIT") == 0 || strcmp(cdes[0], "exit") == 0 ||
			    strcmp(cdes[0], "QUIT") == 0 || strcmp(cdes[0], "quit") == 0 ||
			    strcmp(cdes[0], "BYE") == 0  || strcmp(cdes[0], "bye")  == 0  )
			{	  if (rand()%2==0) printf("\n             I'LL BE  BACK !\n\n");
					else  					 printf("\n        Yao c'est un truc de OUF !\n\n");
					return(0); //exit(0);
			}
			else
			{	Yerr1tpret=0;
			  if (!Yinterprete_cde(cdesc, cdes))
			  {	 if (rand()%2==0) printf ("        Yao, c'est plus fort que toi. \n");
					 else  					  printf ("          Are you talking to me !? \n");
			  }
			}
		}
	}
	else
		printf ("unpossible case #@§?§???§§§\n");

	return(0);
}
/*
int main(int argc, char *argv[])
{   PRJMAIN (argc, argv);
    return(0);
}
*/
/* ========================================================================= */
/* fonction de découpage de la commande par chaine de caractère dans un tableau */
int Yrecup_cde (int *cdesc, char *cdeline, char *cdes[])
{
	int     i;
  i=strlen(cdeline) - 1; //printf("\n %i line=|%s|", i+1, cdeline);

	/* while (cdeline[i] == ' ' || cdeline[i] == '\t') --i; //je me positionne à la fin de la chaine pour y mettre '\0' */
	/* while (cdeline[i] == ' ' || cdeline[i] == '\t' || cdeline[i] == '\n') --i; //je me positionne à la fin de la chaine pour y mettre '\0' */
  /*while (cdeline[i] == '\n') --i; // je me positionne à la fin de la chaine pour y mettre '\0' */
  while (cdeline[i] == '\n' || cdeline[i] == ' ' || cdeline[i] == '\t') --i; /* je me positionne à la fin de la chaine pour y mettre '\0' */
    cdeline[i+1] = '\0';

	i=0;																		/* je me repositionne au début de la chaine */
	*cdesc = 0;	
	while (cdeline[i] != '\0')							/* tant qu'on a pas fini de balayer toute la chaine */
	{
		if (*cdesc>=NB_MAX_ARG_CDE)									/* verif qu'on ne depasse pas le nombre */
		{  printf("too much arg in that order\n");	/* d'arguments defini                   */
			 return(0);
		}
	
		while (cdeline[i] == ' ' || cdeline[i] == '\t') ++i;	/* je suis avant le prochain mot et je vais à sa rencontre */
		cdes[*cdesc]=&cdeline[i];															/* je suis dessus, je stock son adresse */
		++*cdesc;																							/* j'incremente le nombre de mot trouves */

		/* cas particulier pour le changement du prompt: on s'interrompt avant */
		if ((strcmp(cdes[0], "set_prompt")==0	|| strcmp(cdes[0], "SET_PROMPT")==0) && (*cdesc==2)) return(1);			
		/* de meme, cas particulier pour le changement du separateur d'io: on s'interrompt avant */
		if ((strcmp(cdes[0], "set_iosep")==0	|| strcmp(cdes[0], "SET_IOSEP")==0) && (*cdesc==2)) return(1);			
		
		while (cdeline[i] != ' ' && cdeline[i] != '\0')
		{	++i;																/* j'avance sur le mot jusqu'à sa fin */
			if (cdeline[i] == '\t') break;			/* (pour pouvoir passer ensuite au mot suivant) */
		}
		if (cdeline[i] != '\0')
		{	cdeline[i] = '\0';
			++i;
		}
			
	}
	return (1);
}	

/* ========================================================================= */
/* fonction d'interpretation et de traduction de la commande */
int Yinterprete_cde(int cdesc, char *cdes[])
{
	int 	 w1, w2;
	double ww1;
	int		 codret = 1;						/* init a pas d'erreur */

  char	fileinst[STRSIZE80+1];	/* un fichier d'instructions */
  FILE  *fpinst; 							  /* file pointeur du fichier d'instruction */
  int   nbenrlus;								/* nbr d'enr du fichier d'instruction lus */

  int   witraj; 								/* multi: indice de trajectoire */
	
	//printf (">>>%s\n", cdes[0]);
	
	/* pre-traitement pour la substitution des mots definis */
	for (w1=0; w1<cdesc; ++w1)
	{		w2=Yisdefval(cdes[w1]);
			if (w2 >=0 )
			{	 strcpy(cdes[w1], YTabDefval[w2].macro);
			}
	}
	
	/*---------------------------------------------------------*/		
	if (strcmp(cdes[0], "LVAL") == 0	|| strcmp(cdes[0], "lval") == 0)
	{  Ylval();
	}	
	else if (strcmp(cdes[0], "STRAJ") == 0 || strcmp(cdes[0], "straj") == 0)
	{	 Ystraj();
	}
	else if (strcmp(cdes[0], "PDT2PDT") == 0 || strcmp(cdes[0], "pdt2pdt") == 0)
	{ //syntax: pdt2pdt fromtraj steptime  totraj
		if (cdesc != 4)
		{  printf("pdt2pdt: syntax is: pdt2pdt fromtraj steptime  totraj\n");
		   return(1);
		}
		w1 = Yitraj(cdes[1]);
		w2 = Yitraj(cdes[3]);
		if (w1<0 || w2<0)
		{	 printf("pdt2pdt: unknwon trajectory name (%s) or (%s) \n", cdes[1], cdes[3]);
			 return(0);
		}
		/*if (atoi(cdes[2])<=YTabTraj[w1].nbuptime)
		{	 printf("pdt2pdt: steptime must be > nbuptime of trajectory %s\n", cdes[1]);
			 return(0);
		} il ne s'agit ici que de verifier le comportement de la fonction de traduction Ypdt2pdt */
		printf("pdt %i [%i] of %s  --is--> pdt %i [%i] for %s \n", atoi(cdes[2]), atoi(cdes[2])-1,	cdes[1],
	          Ypdt2pdt(w2, atoi(cdes[2]), w1), Ypdt2pdt(w2, atoi(cdes[2]), w1)-1,
	          cdes[3]);
	}
	else if (strcmp(cdes[0], "SET_TOPTIME") == 0 || strcmp(cdes[0], "set_toptime") == 0)
	{ //syntax: set_toptime trajname integer
		if (cdesc != 3)	return(0);

		witraj = Yitraj(cdes[1]);
		if (witraj<0)
		{	 printf("set_toptime: unknwon trajectory name (%s) \n", cdes[1]);
			 return(0);
		}
		w1 = atoi(cdes[2]);
		if (w1<YTabTraj[witraj].nbuptime || w1 >= YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime)
		{	 printf("set_toptime: value must be >=%d and <=%d\n", YTabTraj[witraj].nbuptime,
               YTabTraj[witraj].nbuptime + YTabTraj[witraj].nbsteptime -1);
			 return(0);
		}
		YTabTraj[witraj].toptime = w1;
		Yupdcurtime(witraj);
	}
	else if (strcmp(cdes[0], "SET_MODELTIME") == 0 || strcmp(cdes[0], "set_modeltime") == 0)
	{
		if (cdesc != 2)	return(0);
		w1 = atoi(cdes[1]);
		if (w1!=0)
		{	 printf("set_modeltime: work only with 0, found %s \n", cdes[1]);
			 return(0);
		}
		Yset_modeltime(w1);
	}
	else if (strcmp(cdes[0], "SET_NBITER") == 0	|| strcmp(cdes[0], "set_nbiter") == 0)
	{
		if (cdesc != 2)	return(0);
		YNbItRun = atol(cdes[1]);
	}
	else if (strcmp(cdes[0], "SET_NBEXTL") == 0	|| strcmp(cdes[0], "set_nbextl") == 0)
	{
		if (cdesc != 2)	return(0);		/* pas de test sur YO_VARINCR :    */
		YNbExtL = atoi(cdes[1]);			/* cela ne porte pa a concequence */
	}
	else if (strcmp(cdes[0], "SET_NB_BASIC_INTERN_LOOP") == 0	|| strcmp(cdes[0], "set_nb_basic_intern_loop") == 0)
	{
		if (cdesc != 2)	return(0);
		w1 = atoi(cdes[1]);
		if (w1<=0)
		{	 printf("the basic intern loop number must be positive \n");
			 return(0);
		}
		YNbBasicIntL = w1;
	}
	else if (strcmp(cdes[0], "RUN") == 0	|| strcmp(cdes[0], "run") == 0)
	{
		/* if (cdesc != 2)	return(0); */
		if (YNbItRun<=0)
		{	 printf("run: number of run iteration not seted; use set_nbiter please\n");
			 return(0);
		}
		if (YioInsertObsCtr<0)
		   printf("run: warning : oh oh, run with no obs !!! \n");
		Yrun (YNbItRun);
	}
	else if (strcmp(cdes[0], "RUNI") == 0	|| strcmp(cdes[0], "runi") == 0)
	{
		#ifdef YO_VARINCR
		if (YNbItRun<=0)
		{	 printf("runi: number of run iteration seted; use set_nbiter please\n");
			 return(0);
		}
		if (YNbExtL<=0)
		{	 printf("runi: number of extern loop not seted; use set_nbextl please\n");
			 return(0);
		}
		if (YioInsertObsCtr<0)
		   printf("runi: warning : oh oh, run with no obs !!! \n");
		Yc_run(YNbItRun);
		#else
		printf("runi: incremental option (O_VARINCR) is not active \n");
		return(0);
		#endif
	}
	else if (strcmp(cdes[0], "SET_DXALEA") == 0	|| strcmp(cdes[0], "set_dxalea") == 0)
	{ //valorisation d'une grandeur aleatoire pour la determination de dx dans certaines fonctions de test
		#ifdef YO_GRADTEST
		if (cdesc == 1)        //sans passage de parametre :
		{	 Ydxalea[0] = '\0';  //-> neutralise l'alea
		}
		else //on prend tel quel le parametre passed ; pour bien faire, ce ne doit pas etre
		{    // n'importe quoi mais plm je ne controle pas !
		   //je verifie seulement la longueur
			 if (strlen(cdes[1]) > STRSIZE20)
		   {  printf("set_dxalea: too long parameter, max is %i \n", STRSIZE20);
		      return(0);
			 }
		   strcpy(Ydxalea, cdes[1]);
		}
		#else
		printf("set_dxalea: GRADTEST option is not active \n");
		return(0);
		#endif
	}
	else if (strcmp(cdes[0], "TESTDF") == 0	|| strcmp(cdes[0], "testdf") == 0)
	{ //test des derivees des modules,
		#ifdef YO_GRADTEST
		if (sizeof(YREAL)!=sizeof(double))
		{		printf("testdf: real format must be double (use option O_REAL double in .d)\n");
			 return(0);
		}
		if (cdesc <8)
		{	 printf("testdf error: syntaxe is : testdf i j k t codop [%c]pdx ptol [nmmod-ko-max][max]\n", '%');
			 return(0);
		}
		Ytestdf(cdesc, cdes);
		#else
		printf("testdf: GRADTEST option is not active \n");
		return(0);
		#endif
	}
	else if (strcmp(cdes[0], "TESTAD") == 0	|| strcmp(cdes[0], "testad") == 0)
	{ //test de l'ADjoint
		#ifdef YO_GRADTEST
		if (sizeof(YREAL)!=sizeof(double))
		{		printf("testad: real format must be double (use option O_REAL double in .d)\n");
			 return(0);
		}
		if (cdesc == 2)
		{  if (YioInsertObsCtr != -1)
		   {	 printf("testad error: obs must not be used before global Adjoint test\n");
		   	   //because: ca modifie le dy au cours de la redescente qui ne sera plus le meme
		   	   //         que celui utilised lors du calcul de LTRes.
		   	   //         Cette contrainte n'est pas utile pour testad_mod car le test est local.
			     return(0);
		   }							
			 Ytestad     (atof(cdes[1])); //Ytestad(atof(cdes[1]), 0.0, 0);
		}
		else if (cdesc == 4)
			 Ytestad_mod (atof(cdes[1]), atof(cdes[2]), atoi(cdes[3]), 0.0);
		else if (cdesc == 5)
			 Ytestad_mod (atof(cdes[1]), atof(cdes[2]), atoi(cdes[3]), atof(cdes[4]));
		else
		{	 printf("testad error: syntaxe is : testad pdx [errelmax maxko][pzedi]\n");
			 return(0);
		}					
		#else
		printf("testad: GRADTEST option is not active \n");
		return(0);
		#endif 				
	}		
	else if (strcmp(cdes[0], "TESTLT") == 0	|| strcmp(cdes[0], "testlt") == 0)
	{ //test du Lineaire Tangent
		#ifdef YO_GRADTEST
		if (sizeof(YREAL)!=sizeof(double))
		{		printf("testlt: real format must be double (use option O_REAL double in .d)\n");
			 return(0);
		}
		if      (cdesc==5)		
			 Ytestlt(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), 1, 0); //0, 0); //default
		else if (cdesc==6)			
			 Ytestlt(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), atoi(cdes[5]), 0);
		else if (cdesc==7)			
			 Ytestlt(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), atoi(cdes[5]), atoi(cdes[6]));
		else
		{	 printf("testlt error: syntaxe is : testlt pdx alpha fdec nbloop [modop]\n");			
			 return(0);
		}			
		#else
		printf("testlt: GRADTEST option is not active \n");
		return(0);
		#endif 				
	}		
	else if (strcmp(cdes[0], "TESTOF") == 0	|| strcmp(cdes[0], "testof") == 0)
	{ //test de l'Objective Fonction
		#ifdef YO_GRADTEST
		if (sizeof(YREAL)!=sizeof(double))
		{		printf("testof: real format must be double (use option O_REAL double in .d)\n");
			 return(0);
		}
		if      (cdesc==5)
			 Ytestof(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), 1, 0);  //0, 0);
		else if (cdesc==6)
			 Ytestof(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), atoi(cdes[5]), 0);
		else if (cdesc==7)
			 Ytestof(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atoi(cdes[4]), atoi(cdes[5]), atoi(cdes[6]));
		else
		{	 printf("testof error: syntaxe is : testof pdx alpha fdec nbloop [modop] \n");			
			 return(0);
		}
		#else
		printf("testof: GRADTEST option is not active \n");
		return(0);
		#endif 				
	}			
	else if (strcmp(cdes[0], "FORWARD") == 0 || strcmp(cdes[0], "forward") == 0)
	{  //syntax: forward [trajname nbpdt]
	  if (cdesc!=1 && cdesc!=3)	return(0);	
	  if (cdesc==1) //forward sur le modele global: i.e du debut jusqu'a la 'fin' de
	  {             //toutes les trajectoires sur tous les modeles (pour creer des obs par exemple)
	     //Yset_modeltime(0); en attendant de mettre cette instruction a disposition du user (fait pour 0)
       before_it(1);
	     Yforward(-1, 0);
	  }
	  else //(cdesc==3) //forward trajname nbpdt: forward nbpdt sur une trajectoire particuliere
	  {  witraj = Yitraj(cdes[1]);
		   if (witraj>=0) //cas de la syntaxe: forward trajname nbp :=> forward sur une traj particuliere
		   {	w1 = atoi(cdes[2]);
		      if (w1<=0)
		      {	 printf("forward: bad step_time given, must be >=1\n");
			       return(0);
		      }
		      Yforwardrun(witraj, w1);
		   }
		   else //cas de syntaxe: forward nbp trajname :=> trajname est la trajectoire de reference
		   {    //pour avancer tout le modele de nbp pas de temps par rapport a cette trajectoire.
					witraj = Yitraj(cdes[2]);
		      if (witraj<0)
		      {	 printf("forward: unknwon trajectory name (%s) \n", cdes[2]);
			       return(0);
		      }
	        w1 = atoi(cdes[1]);
		      if (w1<=0)
		      {	 printf("forward: bad step_time given, must be >=1\n");
			       return(0);
		      }
		      before_it(1);
		      Yforward(witraj, YTabTraj[witraj].toptime + w1);
		   }
		}
	}			
	else if (strcmp(cdes[0], "BACKWARD") == 0 || strcmp(cdes[0], "backward") == 0)
	{  //syntax: backward [trajname nbpdt]
	  if (cdesc!=1 && cdesc!=3)	return(0);	
	  if (cdesc==1) //backward sur le modele global: i.e d'ou il est jusqu'au debut de
	  {             //toutes les trajectoires sur tous les modeles
	     Ybackward(-1, 0);
	     after_it(1);
	  }
	  else //(cdesc==3) //2 cas possibles :
	  {  witraj = Yitraj(cdes[1]);
		   if (witraj>=0) //cas de la syntaxe: backward trajname nbp :=> backward sur une traj particuliere
		   {	w1 = atoi(cdes[2]);
		      if (w1<=0)
		      {	 printf("backward: bad step_time given, must be >=1\n");
			       return(0);
		      }
		      Ybackwardrun(witraj, w1);
		   }
		   else //cas de syntaxe: backward nbp trajname :=> trajname est la trajectoire de reference
		   {    //pour reculer tout le modele de nbp pas de temps par rapport a cette trajectoire.
					witraj = Yitraj(cdes[2]);
		      if (witraj<0)
		      {	 printf("backward: unknwon trajectory name (%s) \n", cdes[2]);
			       return(0);
		      }
	        w1 = atoi(cdes[1]);
		      if (w1<=0)
		      {	 printf("backward: bad step_time given, must be >=1\n");
			       return(0);
		      }
		      Ybackward(witraj, YTabTraj[witraj].toptime - w1);
		      after_it(1);
		   }
		}
	}
	else if ( strcmp(cdes[0], "ACTIV") == 0 || strcmp(cdes[0], "activ") == 0
	     ||   strcmp(cdes[0], "UNACTIV") == 0 || strcmp(cdes[0], "unactiv") == 0 )
	{	//syntax: {activ, unactiv} {trajname ,typetraj} [{trajname ,typetraj}[...]] [only]
	  //exemple: active Traj1 M Q Traj7 R only
	  if (cdesc <2)	return(0);	  
		codret = Yactraj(cdesc, cdes);
		return(codret);
	}
	else if (strcmp(cdes[0], "SET_BEGINTIME") == 0 || strcmp(cdes[0], "set_begintime") == 0)
	{	//syntax: set_begintime valeur
	  if (cdesc != 2)	return(0);
		ww1 = atof(cdes[1]);
		if (ww1<0.0)
		{	 printf("set_begintime: bad value given for set_begintime, must not be negativ \n");
			 return(0);
		}
		Ybegintime = ww1;
    for (w1=0; w1<YNBTRAJ; ++w1) {Yupdcurtime(w1);Yupdstoptime(w1);}
	}
	else if (strcmp(cdes[0], "SET_OFFTIME") == 0 || strcmp(cdes[0], "set_offtime") == 0)
	{	//syntax: set_offtime trajname valeur
	  if (cdesc != 3)	return(0);
		ww1 = atof(cdes[2]);
		if (ww1<0.0)
		{	 printf("set_offtime: bad value given for set_offtime, must not be negativ \n");
			 return(0);
		}
		witraj = Yitraj(cdes[1]);
		if (witraj<0)
		{	 printf("set_offtime: unknwon trajectory name (%s) \n", cdes[1]);
			 return(0);
		}
		YTabTraj[witraj].offtime = ww1;
		Yupdcurtime(witraj); Yupdstoptime(witraj);
	}
	else if (strcmp(cdes[0], "SET_DTIME") == 0 || strcmp(cdes[0], "set_dtime") == 0)
	{	//syntax: set_dtime trajname valeur
	  if (cdesc != 3)	return(0);
		ww1 = atof(cdes[2]);
		if (ww1<=0.0)
		{	 printf("set_dtime: bad value given for set_dtime, must be greater 0 \n");
			 return(0);
		}
		witraj = Yitraj(cdes[1]);
		if (witraj<0)
		{	 printf("set_dtime: unknwon trajectory name (%s) \n", cdes[1]);
			 return(0);
		}
		YTabTraj[witraj].dtime = ww1;
		Yupdcurtime(witraj);Yupdstoptime(witraj);
	}
	else if (strcmp(cdes[0], "PRINT_TIME") == 0 || strcmp(cdes[0], "print_time") == 0)
	{
			if (cdesc==1)
			{  printf ("^^^^^^^ CURRENT TRAJ TIME: ");
			   for (w1=0; w1<YNBTRAJ; ++w1) printf ("%s:->%d, ", YTabTraj[w1].name, YTabTraj[w1].toptime);
			   printf("\n");
			}
			else if (strcmp(cdes[1], "ON") == 0 || strcmp(cdes[1], "on") == 0)
							YDispTime = ON;
					 else
							YDispTime = OFF;
	}
	else if (strcmp(cdes[0], "PRINT_COST") == 0 || strcmp(cdes[0], "print_cost") == 0)
	{
			if (cdesc==1)
					 printf ("^^^^^^^ TOTAL COST = % -23.15e \n", YTotalCost);
			else if (strcmp(cdes[1], "ON") == 0 || strcmp(cdes[1], "on") == 0)
						  YDispCost = ON;		//=1
			else if (strcmp(cdes[1], "LAST") == 0 || strcmp(cdes[1], "last") == 0)
						  YDispCost = 2;		//=2
					 else
							YDispCost = OFF;	//=0
	}
	else if (strcmp(cdes[0], "COST") == 0 || strcmp(cdes[0], "cost") == 0)
	{
			if (cdesc<=1) return(0);
			if (strcmp(cdes[1], "LMS") == 0 || strcmp(cdes[1], "lms") == 0)
			{ /*syntaxe: cost lms [val] */
					if (cdesc<2 || cdesc>3) return(0);					
					if (cdesc==3)
					{  if (!strtod(cdes[2], NULL)) return(0);
						 for(w1=0; w1<YNBMODUL;++w1)
								 YTabMod[w1].scoef = strtod(cdes[2], NULL);
					}														
					YTypeCost = COST_LMS;
					return(1);
			}
			else if (strcmp(cdes[1], "APPLI") == 0 || strcmp(cdes[1], "appli") == 0)
			{
					if (cdesc != 2) return(0);
							YTypeCost = COST_APPLI;
					return(1);
			}			
			return(0);
	}
	else if (strcmp(cdes[0], "set_scoef") == 0	|| strcmp(cdes[0], "SET_SCOEF") == 0)
	{	/* syntaxe: set_scoef nmmod scoef */
		if (cdesc != 3)	return(0);
		YTabMod[Yimod(cdes[1])].scoef = atof(cdes[2]);
	}
	else if (strcmp(cdes[0], "set_bcoef") == 0	|| strcmp(cdes[0], "SET_BCOEF") == 0)
	{	/* syntaxe: set_bcoef nmmod bcoef */
		if (cdesc != 3)	return(0);
		YTabMod[Yimod(cdes[1])].bcoef = atof(cdes[2]);
	}
	else if (strcmp(cdes[0], "set_pcoef") == 0	|| strcmp(cdes[0], "SET_PCOEF") == 0)
	{	/* syntaxe: set_pcoef nmmod pcoef */
		if (cdesc != 3)	return(0);
	  YTabMod[Yimod(cdes[1])].pcoef = atof(cdes[2]);
	}	
	else if (strcmp(cdes[0], "ADJUST") == 0 || strcmp(cdes[0], "adjust") == 0)
	{
			if (cdesc!=2) return(0);
			if (strcmp(cdes[1], "STD") == 0 || strcmp(cdes[1], "std") == 0)
						YTypeAdjust = ADJUST_STD;
			else if (strcmp(cdes[1], "APPLI") == 0 || strcmp(cdes[1], "appli") == 0)
						YTypeAdjust = ADJUST_APPLI;
			else
					return(0);
			return(1);
	}
	else if (strcmp(cdes[0], "ECHO") == 0 || strcmp(cdes[0], "echo") == 0)
	{
			if (cdesc != 2) return(0);
			if (strcmp(cdes[1], "ON") == 0 || strcmp(cdes[1], "on") == 0)
					YEcho = ON;
			if (strcmp(cdes[1], "OFF") == 0 || strcmp(cdes[1], "off") == 0)
					YEcho = OFF;	
	}
	else if (strcmp(cdes[0], "SETM_DXMIN") == 0	|| strcmp(cdes[0], "setm_dxmin") == 0)
	{
		#ifdef YO_M1QN3		
		if (cdesc==2)										/* si 1 parametre est passed on considere qu'il */
		   Y3dxmin[0] = atof(cdes[1]);	/* s'agit du scalaire dxmin pour m1qn3          */
		else if (cdesc == 1)
		{
		   #ifdef YO_M2QN1							/* si aucun parametre, on considere, a condition*/
		   Y3valdxmin_all();						/* d'avoir M1QN2, que c'est un vecteur qu'il faut utiliser */
			 #else
			 printf("setm_dxmin: m2qn1 (via o_m1qn3) option is not active \n");
		   return(0);
		   #endif
		}
		#else
		{	printf("setm_dxmin: m1qn3 option is not active \n");
			return(0);
		}
		#endif			
	}
	else if (strcmp(cdes[0], "SETM_DDF1") == 0	|| strcmp(cdes[0], "setm_ddf1") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3ddf1 = atof(cdes[1]);
		#else
		printf("setm_ddf1: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}					
	else if (strcmp(cdes[0], "SETM_EPSG") == 0	|| strcmp(cdes[0], "setm_epsg") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3epsg = atof(cdes[1]);
		#else
		printf("setm_epsg: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SETM_IMPRES") == 0	|| strcmp(cdes[0], "setm_impres") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3impres = atol(cdes[1]);
		#else
		printf("setm_impres: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SETM_IO") == 0	|| strcmp(cdes[0], "setm_io") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3io = atol(cdes[1]);
		#else
		printf("setm_io: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SETM_MODE") == 0	|| strcmp(cdes[0], "setm_mode") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3mode = atol(cdes[1]);
		#else
		printf("setm_mode: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SETM_NSIM") == 0	|| strcmp(cdes[0], "setm_nsim") == 0)
	{
		#ifdef YO_M1QN3
		if (cdesc != 2)	return(0);
		Y3nsim = atol(cdes[1]);
		#else
		printf("setm_nsim: m1qn3 option is not active \n");
		return(0);
		#endif 		
	}	
	else if (strcmp(cdes[0], "SETM_XINF") == 0	|| strcmp(cdes[0], "setm_xinf") == 0)
	{
		#ifdef YO_M2QN1
		Y3valxinf_all();
		#else
		printf("setm_xinf: m2qn1 (via o_m1qn3) option is not active  \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SETM_XSUP") == 0	|| strcmp(cdes[0], "setm_xsup") == 0)
	{
		#ifdef YO_M2QN1
		Y3valxsup_all();
		#else
		printf("setm_xsup: m2qn1 (via o_m1qn3) option is not active  \n");
		return(0);
		#endif 		
	}	
	else if (strcmp(cdes[0], "RUNM") == 0	|| strcmp(cdes[0], "runm") == 0
	     ||  strcmp(cdes[0], "RUNM2")== 0	|| strcmp(cdes[0], "runm2")== 0)
	{	
		#ifndef YO_M1QN3
		printf("runm(2): m1qn3 option is not active \n");
		return(0);
		#endif
		#ifndef YO_M2QN1
		if (strcmp(cdes[0], "RUNM2") == 0	|| strcmp(cdes[0], "runm2") == 0)
		{  printf("runm2: m2qn1 (via o_m1qn3) option is not active \n");
		   return(0);
		}
		#endif					
		
		#ifdef YO_M1QN3
		if (YNbItRun<=0)
		{	 printf("runm(2): number of run iteration not seted; use set_nbiter please\n");
			 return(0);
		}
		if (Y3ddf1<=0)
		{	printf("runm(2): expected positive fcost decrease missed; use setm_ddf1\n");
			 return(0);
		}
		if (YioInsertObsCtr<0)
		   printf("runm(2): warning : oh oh, run with no obs !!! \n");
		
		YTypeAdjust = ADJUST_M1QN3; //d'office avec M1QN3
		if (strcmp(cdes[0], "RUNM") == 0	|| strcmp(cdes[0], "runm") == 0)
		   return(Y3run ('0'));
		else
		   return(Y3run ('2'));
		#endif
	}	
	else if (strcmp(cdes[0], "RUNIM") == 0	|| strcmp(cdes[0], "runim") == 0
	     ||  strcmp(cdes[0], "RUNIM2")== 0	|| strcmp(cdes[0], "runim2")== 0)
	{
		#ifndef YO_M1QN3
		printf("runim(2): m1qn3 option is not active \n");
		return(0);
		#endif		
		#ifndef YO_VARINCR
		printf("runim(2): incremental option (O_VARINCR) is not active \n");
		return(0);
		#endif
		#ifndef YO_M2QN1
		if (strcmp(cdes[0], "RUNIM2") == 0	|| strcmp(cdes[0], "runim2") == 0)
		{  printf("runim2: m2qn1 (via o_m1qn3) option is not active \n");
		   return(0);
		}
		#endif				
												       /* - - - - - - - - - - - - - - */			
		#ifdef YO_M1QN3
		#ifdef YO_VARINCR
		if (YNbItRun<=0)
		{	 printf("runim(2): number of run iteration not seted; use set_nbiter please\n");
			 return(0);
		}
		if (Y3ddf1<=0)
		{	printf("runim(2): expected positive fcost decrease missed; use setm_ddf1\n");
			 return(0);
		}
		if (YNbExtL<=0)
		{	 printf("runim(2): number of extern loop not seted; use set_nbextl please\n");
			 return(0);
		}
		if (YioInsertObsCtr<0)
		   printf("runim(2): warning : oh oh, run with no obs !!! \n");

		YTypeAdjust = ADJUST_M1QN3; //d'office avec M1QN3
		if (strcmp(cdes[0], "RUNIM") == 0	|| strcmp(cdes[0], "runim") == 0)
		   return(Yc3_run ('0'));
		else
		   return(Yc3_run ('2'));
		#endif
		#endif
	}
	else if (strcmp(cdes[0], "LTRAJ") == 0	|| strcmp(cdes[0], "ltraj") == 0)
	{  Yltraj();
	}
	else if (strcmp(cdes[0], "LSPACE") == 0	|| strcmp(cdes[0], "lspace") == 0)
	{  Ylspace();
	}
	else if (strcmp(cdes[0], "LOPERA") == 0	|| strcmp(cdes[0], "lopera") == 0)
	{  Ylopera();
	}
	else if (strcmp(cdes[0], "NMOD") == 0	|| strcmp(cdes[0], "nmod") == 0)
	{  if (cdesc != 2)	return(0);
		 printf("^^^^^^^ %i est le numero du module %s \n", Yimod(cdes[1])+1, cdes[1]);
	}			
	else if (strcmp(cdes[0], "LMOD") == 0	|| strcmp(cdes[0], "lmod") == 0)
	{  Ylmod(cdesc, cdes);
	}
	else if (strcmp(cdes[0], "LMOCOP") == 0	|| strcmp(cdes[0], "lmocop") == 0)
	{  Ylmocop();
	}
	else if (strcmp(cdes[0], "NNET") == 0	|| strcmp(cdes[0], "nnet") == 0)
	{
		#ifdef YO_NETWARD
		if (cdesc != 2)	{printf("nnet: syntax is nnet name\n"); return(0);}
		printf("^^^^^^^ %i est le numero du reseau %s \n", Yinet(cdes[1])+1, cdes[1]);
		#else
		printf("nnet: netward option is not active \n");
		return(0);
		#endif 		
	}			
	else if (strcmp(cdes[0], "LNET") == 0	|| strcmp(cdes[0], "lnet") == 0)
	{
		#ifdef YO_NETWARD
		Ylnet();
		#else
		printf("nnet: netward option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "SAVESTATE") == 0	|| strcmp(cdes[0], "savestate") == 0)
	{	/* syntaxe:		[0]			   [1]    [2]	   [3]	 [4]	      [5]	             [6]	      [7]
								savestate objname s(ortie) oaxis pdt {a(scii), b(inaire)} {0, 1, 2, 3} [filename] */
		if (cdesc < 7)
		{	 printf("savestate: syntax error: savestate objname numout oaxis pdt {A, B} {0, 1, 2, 3} [filename]\n");
			 return(0);
		}
					
		YioModulot = OFF;		
		if (cdes[4][strlen(cdes[4])-1] == '%')
		{	 YioModulot = ON;
		}

		//if (atoi(cdes[4])<0 || atoi(cdes[4])>YNBALLTIME)
		if (atoi(cdes[4])<0) //pdt
		{	 // les pas de temps vont de 1 a YNBALLTIME,
		   //   et 0:=> tous les pas de temps (pour les modules temporise)
			 //printf("savestate: out of time, must be 0 (for all) or in [1, %i]\n", YNBALLTIME);
			 printf("savestate: out of time, must be 0 (for all) or greater\n");
			 return(0);
		} //test refait dans Yio_savestate, car il faut apprecier nballtime selon la trajectoire

		YioWrite = ON; /*=>*/ YioRead = OFF;		/* on s'apprete a ecrire */
		
		YioState = atoi(cdes[2]); /* si 0 alors tous les states sinon, que celle demandee */
		
		YioAscii=YioBin=OFF;
		if      (cdes[5][0]=='a' || cdes[5][0]=='A') YioAscii=ON;
		else if (cdes[5][0]=='b' || cdes[5][0]=='B') YioBin  =ON;
				
		YioAxes=YioTime=OFF;
		if      (cdes[6][0]=='1')  YioAxes=ON;
		else if (cdes[6][0]=='2')  YioTime=ON;
		else if (cdes[6][0]=='3') {YioAxes=ON; YioTime=ON;}

		if (cdesc==7)
		{	if (YioBin)
			{	 printf("savestate: only ascii coding allowed if no file provided\n");
			   return(0);
			}
			codret = Yio_savestate(cdes[1], cdes[3], atoi(cdes[4]), "stdout");
		}
		else if (cdesc==8)
			codret = Yio_savestate(cdes[1], cdes[3], atoi(cdes[4]), cdes[7]);
		else
		{	printf("savestate: syntax error\n");
			return(0);
		}
		return(codret);
	}
	else if (  strcmp(cdes[0], "LOADSTATE") == 0	|| strcmp(cdes[0], "loadstate") == 0
					|| strcmp(cdes[0], "LOADOBS")   == 0	|| strcmp(cdes[0], "loadobs")   == 0)
	{	/* syntaxe:		[0]			 [1]    [2]	   [3]	 [4]	      [5]	            [6]	  [7]           [8]
								load...  nmmod s(ortie) oaxis  pdt {a(scii), b(inaire)} {0, 1} nmfile {f(loat), d(ouble)
								 cas YioAxes					 ----- cas non YioAxes -----
								 di  dj   dk 						Di  di   Dj   dj   Dk   dk
								[9] [10] [11]					 [9] [10] [11] [12] [13] [14]
		*/
		if (cdesc < 9)
		{	 printf("%s: syntax error #1\n", cdes[0]);
			 return(0);
		}		
		//if (atoi(cdes[4])<0 || atoi(cdes[4])>YNBALLTIME)
		if (atoi(cdes[4])<0)
		{	 /* les pas de temps vont de 1 a YNBALLTIME,
		      et 0:=> tous les pas de temps (pour les modules temporise) */
			 //printf("%s: out of time, must be 0 (for all) or in [1, %i]\n", cdes[0], YNBALLTIME);
			 printf("%s: out of time, must be 0 (for all) or greater\n", cdes[0]);
			 return(0);
		}

		YioWrite = OFF; /*=>*/ YioRead = ON;		/* on s'apprete a lire */
		
		YioState = atoi(cdes[2]); /* si 0 alors tous les states sinon, que celle demandee */
		
		YioAscii=YioBin=OFF;
		if      (cdes[5][0]=='a' || cdes[5][0]=='A') YioAscii=ON;
		else if (cdes[5][0]=='b' || cdes[5][0]=='B') YioBin  =ON;
		else
		{	 printf("%s: error on coding type, must be A for ascii or B fo Binary\n", cdes[0]);
			 return(0);
		}		
				
		YioAxes=OFF;		
		if 			(cdes[6][0]=='1')  YioAxes=ON;

		YioTime=OFF;
		if      (cdes[4][0]=='0')  YioTime=ON;
		
		if      (cdes[8][0]=='f' || cdes[8][0]=='F') YioszReal=sizeof(float);
		else if (cdes[8][0]=='d' || cdes[8][0]=='D') YioszReal=sizeof(double);
		else
		{	 printf("%s: error on real type, must be F for float or D for double\n", cdes[0]);
			 return(0);
		}	
		
		/* prise en compte eventuelle d'un décalage */
		//was:     YioDi=YA1;    #ifdef YA2 YioDj=YA2; #endif     #ifdef YA3 YioDk=YA3; #endif
		/* multi :
		if (cdes[1][0] != '*')
		{  wimod = Yimod(cdes[1]);
		   if (wimod<0)
		   {	printf("%s: unknwon modul name (%s) \n", cdes[0], cdes[1]);
			    return(0);
		   }
		   YioDi=YTabMod[wimod].axi; YioDj=YTabMod[wimod].axj;YioDk=YTabMod[wimod].axk;
		}*/
		YioDi=YioDj=YioDk=0; 	/* multi: lors du load on positonnera ces valeurs selon le module
		                                dans la mesure ou le user ne les aura pas valoriseed    */
		Yiodi=Yiodj=Yiodk=0;
		if (!YioAxes)		/*  Di  di   Dj   dj   Dk   dk  */
		{								/* [9] [10] [11] [12] [13] [14] */
		  if (cdesc==10 || cdesc==12 || cdesc==14 || cdesc>15)
			{	 printf("%s: syntax error #2\n", cdes[0]);
			 	 return(0);
			}		
			if (cdesc>10)
			{  YioDi=atoi(cdes[9]); Yiodi=atoi(cdes[10]);}	
			if (cdesc>12)
			{  YioDj=atoi(cdes[11]); Yiodj=atoi(cdes[12]);}	
			if (cdesc>14)
			{  YioDk=atoi(cdes[13]); Yiodk=atoi(cdes[14]);}
		}
		else /*(YioAxes	:   di  dj   dk				*/
		{								/* [9] [10] [11]      */
			if (cdesc>12)
			{	 printf("%s: syntax error #3\n", cdes[0]);
			 	 return(0);
			}			
			if (cdesc>9)	
			{	 Yiodi=atoi(cdes[9]);}
			if (cdesc>10)	
			{	 Yiodj=atoi(cdes[10]);}
			if (cdesc>11)	
			{	 Yiodk=atoi(cdes[11]);}
		}
												
		if (strcmp(cdes[0], "LOADSTATE") == 0	|| strcmp(cdes[0], "loadstate") == 0)
		{	 codret = Yio_load(YIO_LOADSTATE, cdes[1], cdes[3], atoi(cdes[4]), cdes[7]);
		}
		else if (strcmp(cdes[0], "LOADOBS")   == 0	|| strcmp(cdes[0], "loadobs")   == 0)
		{	 codret = Yio_load(YIO_LOADOBS, cdes[1], cdes[3], atoi(cdes[4]), cdes[7]);
		}
		return(codret);
	}
	else if ( !strcmp(cdes[0], "OUTOOBS") || !strcmp(cdes[0], "outoobs")
	       || !strcmp(cdes[0], "OUTOEBX") || !strcmp(cdes[0], "outoebx") )
	{	/* syntaxe:		[0]			[1]    [2]	  [3]	  [4]  ...
								outoobs  nmmod s(ortie) pdt1  pdt2 ...
								outoebx  nmmod s(ortie) pdt1  pdt2 ... */
		if (cdesc < 4)
		{	 printf("%s: syntax error, argument missing\n", cdes[0]);
			 return(0);
		}
		
		if (strcmp(cdes[0], "OUTOOBS") == 0	|| strcmp(cdes[0], "outoobs") == 0)
		{	codret = Youtoobs(YIO_OUTOOBS, cdesc, cdes);
		}
		else if (strcmp(cdes[0], "OUTOEBX")   == 0	|| strcmp(cdes[0], "outoebx")   == 0)
		{	codret = Youtoobs(YIO_OUTOEBX, cdesc, cdes);
		}
		return(codret);		
	}
	else if (strcmp(cdes[0], "lobs") == 0	|| strcmp(cdes[0], "LOBS") == 0)
	{ /* syntaxe:  lobs [trajname] */
    if (cdesc >2)	return(0);
	  if (cdesc==1) Ylistobs(YNBTRAJ);
	  else
	  { w1 = Yitraj(cdes[1]);
	    if (w1<0)
	    {  printf ("lobs %s: unknwon trajectorie name \n", cdes[1]);
	       return(0);
	    }
	    Ylistobs(w1);
	  }	
	}
	else if (strcmp(cdes[0], "setn_psig") == 0	|| strcmp(cdes[0], "SETN_PSIG") == 0)
	{
		#ifdef YO_NETWARD		
		/* syntaxe: setn_psig mx dmin scale offset */
		if (cdesc != 5)	return(0);
		Ynet_setpsig(atof(cdes[1]), atof(cdes[2]), atof(cdes[3]), atof(cdes[4]));
		#else
		printf("netward option is not active \n");
		return(0);
		#endif 				 		
	}
	else if (strcmp(cdes[0], "setn_plin") == 0	|| strcmp(cdes[0], "SETN_PLIN") == 0)
	{	
		#ifdef YO_NETWARD		
		/* syntaxe: setn_plin dmin dmax th */
		if (cdesc != 4)	return(0);
		Ynet_setplin(atof(cdes[1]), atof(cdes[2]), atof(cdes[3])); 		
		#else
		printf("netward option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "setn_activ") == 0	|| strcmp(cdes[0], "SETN_ACTIV") == 0)
	{	
		#ifdef YO_NETWARD		
		/* syntaxe: setn_activ nmnet {siglin sigsig} */
		if (cdesc != 3)	return(0);
		if (Yinet(cdes[1])<0)
		{  printf("setn_activ: bad netward name\n"); return(0);
		}
		if       (strcmp(cdes[2],"siglin")==0 || strcmp(cdes[2],"SIGLIN")==0)
		   				YTabNet[Yinet(cdes[1])].activ = SigLin;		
		else if (strcmp(cdes[2],"sigsig")==0 || strcmp(cdes[2],"SIGSIG")==0)
		   				YTabNet[Yinet(cdes[1])].activ = SigSig;
		else
		{  printf("setn_activ: bad function name\n"); return(0);
		}
		#else
		printf("netward option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "dispn_plin") == 0	|| strcmp(cdes[0], "DISPN_PLIN") == 0)
	{	
		#ifdef YO_NETWARD		
		Ynet_displin(); 		
		#else
		printf("netward option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "dispn_psig") == 0	|| strcmp(cdes[0], "DISPN_PSIG") == 0)
	{	
		#ifdef YO_NETWARD		
		Ynet_dispsig(); 		
		#else
		printf("netward option is not active \n");
		return(0);
		#endif 		
	}
	else if (strcmp(cdes[0], "set_prompt") == 0	|| strcmp(cdes[0], "SET_PROMPT") == 0)
	{	
		if (cdesc==2)
				strncpy(YPrompt, cdes[1], STRSIZE20);
		else
				YPrompt[0]='\0';
	}
	else if (strcmp(cdes[0], "SET_IOSEP") == 0	|| strcmp(cdes[0], "set_iosep") == 0)
	{
    /*if (cdesc==1)
    {  YioSep[0]='\0';
       return(1);
    }
    if (cdesc!=2)
		{	 printf("set_iosep: need no more than a single string parameter\n"); icicicici
			 return(0);
		}
    if (cdes[1][0]!='"' || cdes[1][strlen(cdes[1])-1]!='"')
		{	 printf("set_iosep: string parameter must be delimited by \"\n");
			 return(0);
		}
    strcpy(YioSep, cdes[1]);*/
		if (cdesc==2)
		{	 //strncpy(YioSep, cdes[1], STRSIZE20);
       //on va copier caractere par caractere jusqu'à STRSIZE20 maximum
       //en substituant certains caracteres speciaux (\n, \t, ..?) par leur equivalent
       w1=w2=0;
       while (cdes[1][w1]!='\0' && w1<STRSIZE20)
       {	if (cdes[1][w1]=='\\')
	        {  if      (cdes[1][w1+1]=='n') {YioSep[w2]='\n'; w1+=2; ++w2; continue;} //new line
	           else if (cdes[1][w1+1]=='t') {YioSep[w2]='\t'; w1+=2; ++w2; continue;} //horizontal tab
             //...??? cf .../UDEV/specar.c
          }
          YioSep[w2]=cdes[1][w1]; ++w1; ++w2;
       }
    }
		else
				YioSep[0]='\0';
	}
	else if (strcmp(cdes[0], "LOAD_INST") == 0 || strcmp(cdes[0], "load_inst") == 0)
	{	
	  strcpy(fileinst, cdes[1]);			
	  if ((fpinst = fopen(fileinst, "r")) <= 0)
	  {	 printf ("file %s not found \n", fileinst);
		   return(0);
	  }
	  nbenrlus = 0;
	  ++Yrecurlev;
		if (!Yload_inst(fpinst, fileinst, &nbenrlus)) // on s'arrete a la 1ere erreur,
		{	 for (w1=0; w1<Yrecurlev; ++w1) printf("-");
		   printf("error found while file %s, line %i. sequence aborted \n", fileinst, nbenrlus);
		   --Yrecurlev;		  	
		   return(0);														
		}
    --Yrecurlev;
  	return(1);			
	}
	else if (strcmp(cdes[0], "SAMPLEOF") == 0 || strcmp(cdes[0], "sampleof") == 0)
	{	//syntaxe: sampleof modP Pinf Psup dP modQ Qinf Qsup dQ nmfile
		if (cdesc != 10)
		{	 printf("sampleof: syntax error: sampleof modP Pinf Psup dP modQ Qinf Qsup dQ nmfile\n");
			 return(0);
		}
		YAL2Run=RUNL2_STD;
		return(Ysampleof(cdes[1], atof(cdes[2]), atof(cdes[3]), atof(cdes[4]),
		                 cdes[5], atof(cdes[6]), atof(cdes[7]), atof(cdes[8]), cdes[9]));	
	}	
	else if (strcmp(cdes[0], "SAMPLEOFI") == 0 || strcmp(cdes[0], "sampleofi") == 0)
	{	//syntaxe: sampleofi modP Pinf Psup dP modQ Qinf Qsup dQ nmfile
		#ifdef YO_VARINCR		
		if (cdesc != 10)
		{	 printf("sampleofi: syntax error: sampleofi modP Pinf Psup dP modQ Qinf Qsup dQ nmfile\n");
			 return(0);
		}
		YAL2Run=RUNL2_INCR;
		return(Ysampleof(cdes[1], atof(cdes[2]), atof(cdes[3]), atof(cdes[4]),
		                 cdes[5], atof(cdes[6]), atof(cdes[7]), atof(cdes[8]), cdes[9]));	
		#else
		printf("incremental option (VARINCR) is not active \n");
		return(0);
		#endif 		
	}	
	else if (  strcmp(cdes[0], "HELP") == 0 || strcmp(cdes[0], "help") == 0
	        || strcmp(cdes[0], "AIDE") == 0 || strcmp(cdes[0], "aide") == 0 || cdes[0][0]=='?'
	        )
	{	
		 if (cdesc>1)
		 {	if (cdes[1][0]=='e' || cdes[1][0]=='E' )
			 		 Yhelp('e');
			  else
			 		 Yhelp('f');
		 }
		 else
		    Yhelp('f');
	}
	else if (!strcmp(cdes[0], "setstate")    || !strcmp(cdes[0], "SETSTATE"))
	{	 if (cdesc!=3) return(0);
		 if (Yimod(cdes[1])<0)
		 {	printf("setstate: unknown modul %s \n", cdes[1]);
				return (0);
		 }						
		 return(Ysetstate_mod(cdes[1], atof(cdes[2])));
	}
	else if (!strcmp(cdes[0], "setstate_all") || !strcmp(cdes[0], "SETSTATE_ALL"))
	{	 if (cdesc!=3) return(0);
		 return(Ysetstate_mod("Y#A", atof(cdes[1])));
	}
	else if (strcmp(cdes[0], "PAUSE") == 0 || strcmp(cdes[0], "pause") == 0)
	{	 getchar();
	}
	else if (strcmp(cdes[0], "YBID") == 0 || strcmp(cdes[0], "ybid") == 0)
	{
			//	Ytryrov();
	}
	else /* sinon il peut s'agit d'une auto ou d'une user fonction */
		 if (!Yauto_call(cdesc, cdes))
				if (!Yuser_call(cdesc, cdes))
					 codret = 0;

	return(codret);
}

/* ////////////////////////////////////////////////////////////////////////////////
///////////////////////////// fin fichier Yao.cpp /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////// */

