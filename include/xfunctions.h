
/***************************************************************************
                 module classe xfunctions.h  -  description
                             -------------------

Description: contains personal functions


===========================================================================*/


const int year = 17520;		//Forçage size

const int talla = STm;		//Forçage size
double tair[talla];		//forçage
double qair[talla];		//forçage
double u[talla];		//forçage
double v[talla];		//forçage
double sw[talla];		//forçage
double lw[talla];		//forçage
double pb[talla];		//forçage
double nieve[talla];		//forçage
double precip[talla];

double tair_c[year];		//forçage
double qair_c[year];		//forçage
double wind_c[year];		//forçage
double v_c[year];		//forçage
double sw_c[year];		//forçage
double lw_c[year];		//forçage
double psurf_c[year];		//forçage
double nieve_c[year];		//forçage
double precip_c[year];

string c;
double lai_c[17520];
ifstream numberfile; 
int PFT_c;
int lai_imp = 0;



int modo = 0;		//variable para las saisons
double iteraciones = 1;
double sensibilite;



//parametros del modelo
double rsol_cste1;
double so_capa_wet1;
double so_capa_dry1;
double min_drain1;
double max_drain1;
double exp_drain1;
double so_capa1;
double so_cond1;
double dpu_cste11;
double mx_eau_eau1;
double rveg_par1;
double z0_par1;
double z0_over1;
double albedovis1;
double albedonir1;
double emis_par11;
//double bq_cste;
//double gq_cste;
double hum_c1, hum_c2, hum_c3, hum_c4, hum_c5, hum_c6, hum_c7, hum_c8, hum_c9, hum_c10, hum_c11, hum_c12, hum_c13 ;

//Parametros varios
int bypass_jour = 0;		//imposer jour de l'annee pour comencer la simulation
double bypass_lai;		//imposer lai
int bypass_lai_active = 0;	//lai imposé active ou pas active
string path_f; 		//path des fichiers gradients
string sufix; 		//sufix des fichers gradients
char *path;
double obs_pdt; 	//indica el PDT de l'obs disponible
//para el albedo
int ib = 1;		//Number of points for which the data needs to be interpolated (dimensionless)
double soilalb_dry[1][2];
double soilalb_wet[1][2];
double soilalb_moy[1][2];


//prototipos
void xsoil_albedo();


//________________________________________________________________________________________________________________
double epsilon(double i)
{
	return DBL_EPSILON;
}



//________________________________________________________________________________________________________________
//fonction d'inicialisation de forcing modules
void xinit_forcing(int argc, char *argv[])
{
	//------------------------------------------------------------------------------------------------------------------------
	int n_arch = argc -2;		//numero de archivos	
	int annees = 1;			//annees de lecture: corresponds soit au nombre des fichers
	int PDT;			//Pas De Temps a faire	
	int PDT_init = 0;		// traitement saisoniere
	int index  = 1;			//indice de referencia para el valor de los modulos F_
	//int anio = 2005;



	path = argv[1];		//path des fichiers
	char *file;			//nom du ficher
	string rest_file;		//nom du fichier en string
	int te ;				//longitude de la chaine ficher+path
	char result[500] ;		//nombre du ficher en char*

	
	//------------------------------------------------------------------------------------------------------------------------	
	int rest_file_id;
	int temp_id1,temp_id2,temp_id3,temp_id4,temp_id6,temp_id7,temp_id8,temp_id9 ;
	//int temp_id5;
	printf("start from rest_file\n");	
	//------------------------------------------------------------------------------------------------------------------------
	//validations

	//PDT depends de la quantite de ficher a lire
	if(n_arch == 1) 
		PDT = STm;
	else
	{
		PDT = year_pdt;
		annees = n_arch;
	}	


	//traitement de assimilation saisoniere--> mode = 0-hiver, 1-printemps, 2-ete, 3-automne
	/*if((modo !=0)&&(bypass_jour == 0))	
	{
		PDT_init = (double)modo * saison + 1;
		PDT = STm + PDT_init;

		printf("\n*****PDT_init = %d    PDT %d ******\n",PDT_init,PDT);
	}*/

	if(bypass_jour != 0)	
	{
		PDT_init = bypass_jour;
		PDT = STm + PDT_init;
		
		printf("\n*****JOUR By_passed    PDT_init = %d    PDT %d ******\n",PDT_init,PDT);

	}
	
	
	//printf("\n\n bypass_jour %d       PDT_init %d    PDT %d    \n\n ", bypass_jour,PDT_init , PDT  );


	printf("\n------------\nnro fichers: %d\nPas De Temps:%d\n------------\n",annees,PDT);

	YS_F_lwdown(0,0,0)=0;
	YS_F_swdown(0,0,0)=0;
	YS_F_precip(0,0,0)=0;
	YS_F_u(0,0,0)=0;
	YS_F_qair(0,0,0)=0;
	YS_F_pb(0,0,0)=0;
	YS_F_temp_air(0,0,0)=0;
	YS_F_precip_snow(0,0,0)=0;


	for(int i = 0 ; i < annees ; i++)
	{		
		//construction de la chaine qui represente le nom du ficher avec son path (defini dans le .i)
		file = argv[i+2];

		
		strcpy(result,path);
		strcat(result,file);
		te = strlen(path)+strlen(file);
		rest_file.assign(result,te) ;

		printf("\n------------\nFichier %d: %s\n------------\n",i+1,rest_file.c_str());

		//parametres du fichier .nc (FLUXNET)
		rest_file_id=Ouvre_nc(rest_file.c_str());
		temp_id3=Var_id(rest_file_id,"Qair");
		temp_id2=Var_id(rest_file_id,"PSurf");
		temp_id1=Var_id(rest_file_id,"Tair");
		temp_id4=Var_id(rest_file_id,"Wind");
		//temp_id4=Var_id(rest_file_id,"Wind_N");
		//temp_id5=Var_id(rest_file_id,"Wind_E");
		temp_id6=Var_id(rest_file_id,"Rainf");
		temp_id7=Var_id(rest_file_id,"SWdown");
		temp_id8=Var_id(rest_file_id,"LWdown");	
		temp_id9=Var_id(rest_file_id,"Snowf");	
	
		for(int t = PDT_init+1 ; t <= PDT ; t++)
		{
			for( int i = 0; i < kjindex ; i++)
			{
				//if(t < year_pdt)
				//{
					//tair
					zonclire(t, kjindex, rest_file_id, temp_id1,tair);			
					YS_F_temp_air(0,i,index)=tair[0];					
					//Psurf
					zonclire(t, kjindex, rest_file_id, temp_id2,pb);
					YS_F_pb(0,i,index)=pb[0]/100; // en pascal, no se si debo convertirla a hPa--->/100;
					//qair
					zonclire(t, kjindex, rest_file_id, temp_id3,qair);
					YS_F_qair(0,i,index)=qair[0];
					//Wind_N
					zonclire(t, kjindex, rest_file_id, temp_id4,u);
					YS_F_u(0,i,index)=u[0];
					//Wind_E
					//zonclire(t, kjindex, rest_file_id, temp_id5,v);
					//YS_F_v(0,i,index)=v[0];
					//Rainf
					zonclire(t, kjindex, rest_file_id, temp_id6,precip);
					YS_F_precip(0,i,index)= precip[0] *dT ; 
					//SWdown
					zonclire(t, kjindex, rest_file_id, temp_id7,sw);
					YS_F_swdown(0,i,index)=sw[0];
					//LWdown
					zonclire(t, kjindex, rest_file_id, temp_id8,lw);
					YS_F_lwdown(0,i,index)=lw[0];
					//snowf
					zonclire(t, kjindex, rest_file_id, temp_id9,nieve);
					YS_F_precip_snow(0,i,index)=nieve[0];
					//incremento indice de los modulos F_
					index++;

			}
		}
	}
	nc_close(rest_file_id);

}

//________________________________________________________________________________________________________________
//fonction d'inicialisation de forcing du LAI
void xforcing_lai(int argc, char *argv[])
{
	//------------------------------------------------------------------------------------------------------------------------
	int n_arch = argc -2;		//numero de archivos	
	int annees = 1;			//annees de lecture: corresponds soit au nombre des fichers
	int PDT;			//Pas De Temps a faire	
	int PDT_init = 0;		// traitement saisoniere
	int index  = 1;			//indice de referencia para el valor de los modulos F_
	int numm = 0;
	string line;

	path = argv[1];		//path des fichiers
	char *file;			//nom du ficher
	string rest_file;		//nom du fichier en string
	int te ;				//longitude de la chaine ficher+path
	char result[500] ;		//nombre du ficher en char*

	
	printf("start from rest_file\n");	
	//------------------------------------------------------------------------------------------------------------------------
	//validations

	lai_imp = 1;

	//PDT depends de la quantite de ficher a lire
	if(n_arch == 1) 
		PDT = STm;
	else
	{
		PDT = year_pdt;
		annees = n_arch;
	}	


	if(bypass_jour != 0)	
	{
		PDT_init = bypass_jour;
		PDT = STm + PDT_init;
		
		printf("\n*****JOUR By_passed    PDT_init = %d    PDT %d ******\n",PDT_init,PDT);

	}
	

	printf("\n------------\nnro fichers: %d\nPas De Temps:%d\n------------\n",annees,PDT);

	for(int i = 0 ; i < annees ; i++)
	{		
		//construction de la chaine qui represente le nom du ficher avec son path (defini dans le .i)
		file = argv[i+2];

		
		strcpy(result,path);
		strcat(result,file);
		te = strlen(path)+strlen(file);
		rest_file.assign(result,te) ;

		printf("\n------------\nFichier %d: %s\n------------\n",i+1,rest_file.c_str());



		numberfile.open(rest_file.c_str(), std::ifstream::in);//opens file		
		getline(numberfile,c);		
		lai_c[0] = atof(c.c_str());
		
		while (!numberfile.eof()) 
		{
			numm++;				
			getline(numberfile,c);
			lai_c[numm] = atof(c.c_str());			
		}
		numberfile.close();

			
		for(int t = PDT_init-1 ; t <= PDT ; t++)
		{
			for( int i = 0; i < kjindex ; i++)
			{
				YS_S_lai(0,i,PFT_c,index) = lai_c[t];
				index++;
			}
		}    
	
	}
}


//________________________________________________________________________________________________________________
//fonction pour chosir la saison
/* Pour choisir la saison, il faut:
	1) appel a fonction xsaison depuis le .i, en passant la saison desiree (0-hiver, 1-printemps, 2-ete, 3-automne)
	2) On choisi le PDT qu'on veux, la sim comence le 1er jour de la saison
	3) seulemenet pour un seul fichier, sur un an (a ver)
	4) */
void xsaison(int argc, char *argv[])
{
	
	modo = atoi(argv[1]);

	if(modo == 0)
		printf("\nSaison : Hiver \n \n");
	if(modo == 1)
		printf("\nSaison : Printemps \n \n");
	if(modo == 2)
		printf("\nSaison : Ete \n \n");
	if(modo == 3)
		printf("\nSaison : Autonme \n \n");
}




//________________________________________________________________________________________________________________
//fonction pour choisir a volonte le jour de l'annee pour commencer la simulation du modele (en Pas De Temps)
void xbypass_jour(int argc, char *argv[])
{	
	bypass_jour = atoi(argv[1]);
	xsoil_albedo();
}


//________________________________________________________________________________________________________________
//fonction pour imposer la valeur du LAI
void xbypass_lai(int argc, char *argv[])
{	
	bypass_lai = atof(argv[1]);

	bypass_lai_active = 1;

	printf("bypass_lai %en",bypass_lai);

}


//________________________________________________________________________________________________________________
//fonction pour indiquer le PDT de l'obs utilisé (utilisé pour faire le test du gradient)
void xobs_pdt(int argc, char *argv[])
{	
	obs_pdt = atoi(argv[1]);
	
}

//________________________________________________________________________________________________________________
//fonction pour initialiser les valeurs des parametres de la  humidite du sol
void xparam_humrel(int argc, char *argv[])
{
	 hum_c1= atof(argv[1]);
	hum_c2= atof(argv[2]);
	hum_c3= atof(argv[3]); 
	hum_c4= atof(argv[4]); 
	hum_c5= atof(argv[5]); 
	hum_c6= atof(argv[6]); 
	hum_c7= atof(argv[7]); 
	hum_c8= atof(argv[8]); 
	hum_c9= atof(argv[9]); 
	hum_c10= atof(argv[10]); 
	hum_c11= atof(argv[11]); 
	hum_c12= atof(argv[12]); 
	hum_c13= atof(argv[13]); 

	printf(" \n\n\n**********************************************************\n");
	printf("********INITALISATION DES PARAMETRES HUMIDITE DU SOL************\n");
	printf("**********************************************************\n\n");
	printf("    hum_c_1   %e         hum_c_2   %e        hum_c_3   %e       \n \n ",hum_c1, hum_c2, hum_c3);
	printf("    hum_c_4   %e         hum_c_5   %e        hum_c_6   %e       \n \n ",hum_c4, hum_c5, hum_c6);
	printf("    hum_c_7   %e         hum_c_8   %e        hum_c_9   %e       \n \n ",hum_c7, hum_c8, hum_c9);
	printf("    hum_c_10   %e        hum_c_11   %e       hum_c_12   %e       \n \n ",hum_c10, hum_c11, hum_c12);
	printf("    hum_c_13   %e          \n \n ",hum_c13);
	printf("*********************************************************\n\n\n");

}



//________________________________________________________________________________________________________________
//fonction pour initialiser les valeurs des parametres du modele
void xparam_init(int argc, char *argv[])
{
	rsol_cste1 = atof(argv[1]) ;
	so_capa_wet1 = atof(argv[2]);
	so_capa_dry1 = atof(argv[3]);
	min_drain1 = atof(argv[4]);
	max_drain1 = atof(argv[5]);
	exp_drain1 = atof(argv[6]);
	so_capa1 = atof(argv[7]);
	so_cond1 = atof(argv[8]);
	dpu_cste11 = atof(argv[9]);
	mx_eau_eau1 = atof(argv[10]);
	rveg_par1 = atof(argv[11]);
	z0_par1 = atof(argv[12]);
	z0_over1 = atof(argv[13]);
	albedovis1 = atof(argv[14]);
	albedonir1 = atof(argv[15]);
	emis_par11 = atof(argv[16]);
	//bq_cste = atof(argv[17]);
	//gq_cste = atof(argv[18]);


       

	printf(" \n\n\n**********************************************************\n");
	printf("********INITALISATION DES PARAMETRES DU MODELE************\n");
	printf("**********************************************************\n\n");
	printf("    rsol_cste1 %e        so_capa_wet1 %e     so_capa_dry1 %e       \n \n ",rsol_cste1, so_capa_wet1, so_capa_dry1);
	printf("    exp_drain1 %e        so_capa1     %e     so_cond1 %e          \n \n ",exp_drain1, so_capa1, so_cond1 );
	printf("    dpu_cste11 %e        mx_eau_eau1 %e      rveg_par %e      \n \n ",dpu_cste11, mx_eau_eau1, rveg_par1 );
	printf("    z0_par     %e         min_drain1 %e      max_drain1 %e      \n \n",z0_par1, min_drain1, max_drain1 );
	printf("    z0_over    %e         albvis     %e      albnir   %e        \n \n",z0_over1,albedovis1,albedonir1 );
	printf("    emis       %e	\n \n",emis_par11);
	printf("*********************************************************\n\n\n");

	
	printf("PFT activos: ");
	for(int i = 0 ; i < nvm ; i++)
	{
		if(sechiba_vegmax[0][i] != 0)
			printf("PFT %d = %e , ",i+1,sechiba_vegmax[0][i]);
	}
	printf("\n\n*********************************************************\n\n\n\n");


}

//________________________________________________________________________________________________________________
// initialisation du ficher des gradients
void xsensib(int argc, char *argv[])
{
	sensibilite = atof(argv[1]) ;
	
	printf("sensibilidad %e\n\n",sensibilite);
}



//________________________________________________________________________________________________________________
// definition du max_veget, maximum de vegtation pour chaque type de vegetation
void xPFT(int argc, char *argv[])
{
	for(int i =0 ; i < nvm ; i++)
	{
		sechiba_vegmax[0][i] = atof(argv[i+1]);
		if(sechiba_vegmax[0][i]!= 0)
			PFT_c = i;
	}

}




//________________________________________________________________________________________________________________
//nom du fichier des gradients
void xgrad_fichier(int argc, char *argv[])
{	
	path_f = argv[1];		//path des fichiers
	sufix = argv[2];		//sufix des fichers

	printf("\n %s    %s\n------------\n",path_f.c_str(),sufix.c_str());

}





//________________________________________________________________________________________________________________
//nom du fichier des gradients
void xsoil_albedo()
{


	//Levels of soil colour classification
	int classnb = 9;

	//Corresponding soil albedo values to soil colour classification:
	//wet and dry soil albedo values in visible and near-infrared range \n
	double vis_dry[] = {0.24, 0.22, 0.20, 0.18, 0.16, 0.14, 0.12, 0.10, 0.27};
	double nir_dry[] = {0.48, 0.44, 0.40, 0.36, 0.32, 0.28, 0.24, 0.20, 0.55};
	double vis_wet[] = {0.12, 0.11, 0.10, 0.09, 0.08, 0.07, 0.06, 0.05, 0.15};
	double nir_wet[] = {0.24, 0.22, 0.20, 0.18, 0.16, 0.14, 0.12, 0.10, 0.31};

	//Corresponding soil albedo values to soil colour classification:
	//Averaged soil albedo values (mean of wet and soil albedo values) 
	//in visible and near-infrared range
	double albsoil_vis[] = {0.18, 0.16, 0.16, 0.15, 0.12, 0.105, 0.09, 0.075, 0.255};
	double albsoil_nir[] = {0.36, 0.34, 0.34, 0.33, 0.30, 0.25, 0.20, 0.15, 0.45};

	double sum_vis_dry = 0 , sum_nir_dry = 0, sum_vis_wet = 0 , sum_nir_wet = 0, sum_albvis = 0, sum_albnir = 0;
	//double soilalb_dry[ib][2], soilalb_wet[ib][2],soilalb_moy[ib][2];
 



	for(int t = 0 ; t < classnb; t++)
	{
		sum_vis_dry += vis_dry[t];
		sum_nir_dry += nir_dry[t];
		sum_vis_wet += vis_wet[t];
		sum_nir_wet += nir_wet[t];
		sum_albvis += albsoil_vis[t];
		sum_albnir += albsoil_nir[t];


	}


	for(int i = 0 ; i < ib ; i++)
	{
		soilalb_dry[ib][0] = (sum_vis_dry/classnb + sum_vis_wet/classnb)/2;
		soilalb_dry[ib][1] = (sum_nir_dry/classnb + sum_nir_wet/classnb)/2;
		soilalb_wet[ib][0] = (sum_vis_dry/classnb + sum_vis_wet/classnb)/2;
		soilalb_wet[ib][1] = (sum_nir_dry/classnb + sum_nir_wet/classnb)/2;
		soilalb_moy[ib][0] = sum_albvis/classnb;
		soilalb_moy[ib][1] = sum_albnir/classnb;
		/*soilalb_dry(ib,0) = (sum(vis_dry)/classnb + SUM(vis_wet)/classnb)/deux
		soilalb_dry(ib,1) = (SUM(nir_dry)/classnb + SUM(nir_wet)/classnb)/deux
		soilalb_wet(ib,0) = (SUM(vis_dry)/classnb + SUM(vis_wet)/classnb)/deux
		soilalb_wet(ib,1) = (SUM(nir_dry)/classnb + SUM(nir_wet)/classnb)/deux
		soilalb_moy(ib,0) = SUM(albsoil_vis)/classnb
		soilalb_moy(ib,1) = SUM(albsoil_nir)/classnb	*/

		//printf("\n\n\nsoildry[0] %e  soildry[1] %e     soilwet [0] %e   soilwet[1] %e   soilmoy[0] %e  soilmoy[1] %e\n\n",soilalb_dry[ib][0],soilalb_dry[ib][1], soilalb_wet[ib][0], soilalb_wet[ib][1],soilalb_moy[ib][0], soilalb_moy[ib][1]);
	}




}
























//________________________________________________________________________________________________________________
//fonction d'nicialisation de forcing modules (OLD)
void init_forcing_OLD()
{
	//cases
	/*
		1) un solo archivo forcing, si se hace para mas de un anio, se retoman los valores desde la 1era medida
		2) mas de un archivo: la cantidad de archivos determina la talla de la ventana de asimilacion
	*/	
	//char const    *rest_file    = "/usr/home/hbplod/data/DOCTORADO/Yao stuff/SECHIBA/sechiba2/input/dat/Qle.dat";

	char const    *rest_file    = "/usr/home/hbplod/data/DOCTORADO/Yao stuff/SECHIBA/sechiba3/input/HV_1992.nc";
	int rest_file_id;
	rest_file_id=Ouvre_nc(rest_file);
	//double temp;
	int temp_id1,temp_id2,temp_id3,temp_id4,temp_id5,temp_id6,temp_id7,temp_id8,temp_id9 ;

	
	printf("start from rest_file\n");

	temp_id3=Var_id(rest_file_id,"Qair");
	temp_id2=Var_id(rest_file_id,"PSurf");
	temp_id1=Var_id(rest_file_id,"Tair");
	temp_id4=Var_id(rest_file_id,"Wind_N");
	temp_id5=Var_id(rest_file_id,"Wind_E");
	temp_id6=Var_id(rest_file_id,"Rainf");
	temp_id7=Var_id(rest_file_id,"SWdown");
	temp_id8=Var_id(rest_file_id,"LWdown");	
	temp_id9=Var_id(rest_file_id,"Snowf");	

	YS_F_lwdown(0,0,0)=0;
	YS_F_swdown(0,0,0)=0;
	YS_F_precip(0,0,0)=0;
	YS_F_u(0,0,0)=0;
	YS_F_qair(0,0,0)=0;
	YS_F_pb(0,0,0)=0;
	YS_F_temp_air(0,0,0)=0;
	YS_F_precip_snow(0,0,0)=0;

	int index  = 1;

	for(int t = 0 ; t < STm ; t++)
	{
		for( int i = 0; i < kjindex ; i++)
		{
			if(t < year_pdt)
			{
				//tair
				zonclire(t, kjindex, rest_file_id, temp_id1,tair);			
				YS_F_temp_air(0,i,t+1)=tair[0];				
				//Psurf
				zonclire(t, kjindex, rest_file_id, temp_id2,pb);
				YS_F_pb(0,i,t+1)=pb[0]/100; // en pascal, no se si debo convertirla a hPa--->/100;
				//qair
				zonclire(t, kjindex, rest_file_id, temp_id3,qair);
				YS_F_qair(0,i,t+1)=qair[0];
				//Wind_N
				zonclire(t, kjindex, rest_file_id, temp_id4,u);
				YS_F_u(0,i,t+1)=u[0];
				//Wind_E
				zonclire(t, kjindex, rest_file_id, temp_id5,v);
				YS_F_v(0,i,t+1)=v[0];
				//Rainf
				zonclire(t, kjindex, rest_file_id, temp_id6,precip);
				YS_F_precip(0,i,t+1)= precip[0] *1800 ; //* 3600 * 24; //  input:kg/m2/s--> conversion a  mm/d
				//printf("rain: %e \n",YS_F_precip(0,i,t+1));
				//SWdown
				zonclire(t, kjindex, rest_file_id, temp_id7,sw);
				YS_F_swdown(0,i,t+1)=sw[0];
				//LWdown
				zonclire(t, kjindex, rest_file_id, temp_id8,lw);
				YS_F_lwdown(0,i,t+1)=lw[0];
				//snowf
				zonclire(t, kjindex, rest_file_id, temp_id9,nieve);
				YS_F_precip_snow(0,i,t+1)=nieve[0];// * 3600 * 24; //  input:kg/m2/s--> conversion a  mm/d
			}
			else
			{
				//tair		
				YS_F_temp_air(0,i,t+1)= YS_F_temp_air(0,i,index);		
				//Psurf
				YS_F_pb(0,i,t+1)= YS_F_pb(0,i,index); // en pascal, no se si debo convertirla a hPa--->/100;
				//qair				
				YS_F_qair(0,i,t+1)= YS_F_qair(0,i,index);
				//Wind_N
				YS_F_u(0,i,t+1)=YS_F_u(0,i,index);
				//Wind_E
				YS_F_v(0,i,t+1)=YS_F_v(0,i,index);
				//Rainf
				YS_F_precip(0,i,t+1)= YS_F_precip(0,i,index) ; //* 3600 * 24; //  input:kg/m2/s--> conversion a  mm/d
				//printf("rain: %e \n",YS_F_precip(0,i,t+1));
				//SWdown
				YS_F_swdown(0,i,t+1)=YS_F_swdown(0,i,index);
				//LWdown
				YS_F_lwdown(0,i,t+1)=YS_F_lwdown(0,i,index);
				//snowf
				YS_F_precip_snow(0,i,t+1)=YS_F_precip_snow(0,i,index);// * 3600 * 24; //  input:kg/m2/s--> conversion a  mm/d

				index++;
			}

		}
	}
}

//________________________________________________________________________________________________________________
//fonction d'inicialisation de forcing modules --- SMOSREX data
void xinit_forcing_SMOSREX(int argc, char *argv[])
{
	int n_arch = argc -2;		//numero de anios	
	int PDT;			//Pas De Temps a faire	
	int PDT_init = 0;		// traitement saisoniere
	int annees = 1;			//annees de lecture: corresponds soit au nombre des fichers 
	//ostringstream anio = 2005;

	path = argv[1];		//path des fichiers
	char *file;			//nom du ficher
	string rest_file_sw;		//nom du fichier en string
	string rest_file_lw;		//nom du fichier en string
	string rest_file_precip;	//nom du fichier en string
	string rest_file_wind;		//nom du fichier en string
	string rest_file_tair;		//nom du fichier en string
	string rest_file_qair;		//nom du fichier en string
	string rest_file_psurf;		//nom du fichier en string

	string line_sw, line_lw, line_precip, line_wind, line_tair, line_qair, line_psurf;
	
	int ii = 0;
	int index = 0;

	int te ;				//longitude de la chaine ficher+path
	char result[500] ;		//nombre du ficher en char*
	

	//PDT depends de la quantite de ficher a lire
	if(n_arch == 1) 
		PDT = STm;
	else
	{
		PDT = year_pdt;
		annees = n_arch;
	}

	if(bypass_jour != 0)	
	{
		PDT_init = bypass_jour;
		PDT = STm + PDT_init;		
		printf("\n*****JOUR By_passed    PDT_init = %d    PDT %d ******\n",PDT_init,PDT);

	}


	printf("\n------------\nnro fichers: %d\nPas De Temps:%d\n------------\n",annees,PDT);

	YS_F_lwdown(0,0,0)=0;
	YS_F_swdown(0,0,0)=0;
	YS_F_precip(0,0,0)=0;
	YS_F_u(0,0,0)=0;
	YS_F_qair(0,0,0)=0;
	YS_F_pb(0,0,0)=0;
	YS_F_temp_air(0,0,0)=0;
	YS_F_precip_snow(0,0,0)=0;
	
	

	for(int i = 0 ; i < annees ; i++)
	{		
		//construction de la chaine qui represente le nom du ficher avec son path (defini dans le .i)
		//strcpy(file,argv[i+2]);
		//strcat(file,"_swdown.asc");
		file = argv[i+2];
/*
		//-----------------------------------------------SWDOWN---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_swdown.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_sw.assign(result,te) ;
		//printf("\n------------\nFichier %d: \n------------         %s\n",i+1,rest_file_sw.c_str());		
		ifstream file_sw(rest_file_sw.c_str());
		if (file_sw.is_open())
		{
			while ( file_sw.good() )
			{
				getline(file_sw,line_sw);
				sw_c[ii++] = atof(line_sw.c_str());		
			}
			file_sw.close();
		}	



		//-----------------------------------------------LWDOWN---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_lwdown.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_lw.assign(result,te) ;
		
		ifstream file_lw(rest_file_lw.c_str());
		if (file_lw.is_open())
		{
			while ( file_lw.good() )
			{
				getline(file_lw,line_lw);
				lw_c[ii++] = atof(line_lw.c_str());		
			}
			file_lw.close();
		}	



		//-----------------------------------------------PRECIP---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_precip.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_precip.assign(result,te) ;
		
		ifstream file_precip(rest_file_precip.c_str());
		if (file_precip.is_open())
		{
			while ( file_precip.good() )
			{
				getline(file_precip,line_precip);
				precip_c[ii++] = atof(line_precip.c_str());		
			}
			file_precip.close();
		}	




		//-----------------------------------------------WIND---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_wind.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_wind.assign(result,te) ;
		
		ifstream file_wind(rest_file_wind.c_str());
		if (file_wind.is_open())
		{
			while ( file_wind.good() )
			{
				getline(file_wind,line_wind);
				wind_c[ii++] = atof(line_wind.c_str());		
			}
			file_wind.close();
		}	


		//-----------------------------------------------TAIR---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_tair.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_tair.assign(result,te) ;
		
		ifstream file_tair(rest_file_tair.c_str());
		if (file_tair.is_open())
		{
			while ( file_tair.good() )
			{
				getline(file_tair,line_tair);
				tair_c[ii++] = atof(line_tair.c_str());		
			}
			file_tair.close();
		}	



		//-----------------------------------------------QAIR---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_qair.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_qair.assign(result,te) ;
		
		ifstream file_qair(rest_file_qair.c_str());
		if (file_qair.is_open())
		{
			while ( file_qair.good() )
			{
				getline(file_qair,line_qair);
				qair_c[ii++] = atof(line_qair.c_str());		
			}
			file_qair.close();
		}	



		//-----------------------------------------------PSURF---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_psurf.asc");
		te = strlen(path)+strlen(file)+10;
		rest_file_psurf.assign(result,te) ;
		ifstream file_psurf(rest_file_psurf.c_str());
		if (file_psurf.is_open())
		{
			while ( file_psurf.good() )
			{
				getline(file_psurf,line_psurf);
				psurf_c[ii++] = atof(line_psurf.c_str());		
			}
			file_psurf.close();
		}	



*/
		//-----------------------------------------------SWDOWN---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_swdown.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_sw.assign(result,te) ;		


		//-----------------------------------------------LWDOWN---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_lwdown.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_lw.assign(result,te) ;	


		//-----------------------------------------------PRECIP---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_precip.asc");
		te = strlen(path)+strlen(file)+11;
		rest_file_precip.assign(result,te) ;
			

		//-----------------------------------------------WIND---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_wind.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_wind.assign(result,te) ;
		

		//-----------------------------------------------TAIR---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_tair.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_tair.assign(result,te) ;
			



		//-----------------------------------------------QAIR---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_qair.asc");
		te = strlen(path)+strlen(file)+9;
		rest_file_qair.assign(result,te) ;
			



		//-----------------------------------------------PSURF---------------------------------------------------////
		strcpy(result,path);
		strcat(result,file);
		strcat(result,"_psurf.asc");
		te = strlen(path)+strlen(file)+10;
		rest_file_psurf.assign(result,te) ;



		ifstream file_psurf, file_precip,file_qair,file_tair,file_wind,file_sw,file_lw ;


		file_psurf.open(rest_file_psurf.c_str(), std::ifstream::in);
		file_precip.open(rest_file_precip.c_str(), std::ifstream::in);
		file_qair.open(rest_file_qair.c_str(), std::ifstream::in);
		file_tair.open(rest_file_tair.c_str(), std::ifstream::in);
		file_wind.open(rest_file_wind.c_str(), std::ifstream::in);
		file_sw.open(rest_file_sw.c_str(), std::ifstream::in);
		file_lw.open(rest_file_lw.c_str(), std::ifstream::in);

	

		//for(int st = 0 ; st < year -1 ; st++)
		while ( file_psurf.good() )
		{
			getline(file_psurf,line_psurf);
			psurf_c[ii++] = atof(line_psurf.c_str());

			getline(file_sw,line_sw);
			sw_c[ii++] = atof(line_sw.c_str());	

			getline(file_lw,line_lw);
			lw_c[ii++] = atof(line_lw.c_str());
						
			getline(file_precip,line_precip);
			precip_c[ii++] = atof(line_precip.c_str());

			getline(file_wind,line_wind);
			wind_c[ii++] = atof(line_wind.c_str());	

			getline(file_tair,line_tair);
			tair_c[ii++] = atof(line_tair.c_str());						

			getline(file_qair,line_qair);
			qair_c[ii++] = atof(line_qair.c_str());	



		}


		file_precip.close();
		file_qair.close();
		file_tair.close();
		file_wind.close();
		file_sw.close();
		file_lw.close();
		file_psurf.close();



	


		for(int t = PDT_init+1 ; t <= PDT ; t++)
		{
			for( int i = 0; i < kjindex ; i++)
			{
				//tair						
				YS_F_temp_air(0,0,index)=tair_c[t-1];					
				//Psurf				
				YS_F_pb(0,i,index)=psurf_c[t-1]/100; 
				//qair				
				YS_F_qair(0,i,index)=qair_c[t-1];
				//Wind_N				
				YS_F_u(0,i,index)=wind_c[t-1];
				//precip
				YS_F_precip(0,i,index)= precip_c[t-1] *dT ; 
				//SWdown				
				YS_F_swdown(0,i,index)=sw_c[t-1];
				//LWdown				
				YS_F_lwdown(0,i,index)=lw_c[t-1];
				//snowf				
				YS_F_precip_snow(0,i,index)=0;
				
				//incremento indice de los modulos F_
				index++;

			}
		}		
	}
}


