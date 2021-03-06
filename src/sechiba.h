///////////////////////////////////////////////////////////////////////////////
// define, globales, fonctions perso (obligatoire et autres) ...
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <float.h>
#include <fstream>
#include <string.h>
#include <limits.h>


#include"./include/ncutil.h"//netcdf		//Read nc files
#include"./include/constantes_veg.h"		//Vegetal constants
#include"./include/constantes_soil.h"		//soil constants
#include"./include/run_sechiba.h"		//Initial conditions of the experience
#include"./include/constantes_norm.h"		//Normalization constants
#include"./include/derivfunc.h"			//derivative of sechiba.h functions
#include"./include/xfunctions.h"		//personal functions

//___________________________________DEFINE_____________________________________
#define MAX(a,b) (a>=b?a:b)
#define MIN(a,b) (a<=b?a:b)

// JB
//__________________________________MACRO MODIFYNG GRADIENT_____________________

//List of control parameters (to change if changing .d)
#define CTL_PARAMETERS {"emis_c","rveg_c","z0_c","so_cond_c","so_capa_c","albvis_c","rsol_c","dpu_c","mx_eau_c","hum_c_12"}

#define RESET_GRAD(MOD) do				\
	{							\
      		if (!strcmp(#MOD, modname))			\
								\
           		Yrazgrad_##MOD();				\
        							\
	} while(0) //To oblige writing a ';' after the macro

// \JB


//__________________________________VARIABLES___________________________________
//contantes
// globales
double dtradia; 		//variable de discretizacion temporal (PDT en seconds)





//CONDITIONES INITIALES
double netr = 0;
double fsens = 0;
double flat = 0;
double evap = 0;
double transp = 0;

//condveg cuando no lo calculo
double zlev = 2;
double rough = roughheight_scal;
double albedo_old = albedo_scal[0];
double albedo1_old = albedo_scal[1];



//////////////////// variables inicializadas en el var_init de los modulos
//1)hydro
double temp_mean_bqsb[kjindex];
double temp_mean_gqsb[kjindex];
double temp_mean_dsg[kjindex];

double zsoil[nbdl];


//Variables para los archivos de grandiente
//gradient
ofstream temp_file;
ofstream rsol_file;
ofstream min_drain_file;
ofstream max_drain_file;
ofstream exp_drain_file;
ofstream dpu_file;
ofstream so_capa_file;
ofstream so_cond_file;
ofstream mx_eau_file;
ofstream rveg_file;
ofstream z0_file;
//ofstream z0_over_file;
ofstream humc_file;
ofstream albvis_file;
ofstream albnir_file;
ofstream emis_file;
ofstream tempsol_yw;


//______________________________________________________________________________

// JB
//Variable to set the control parameters
int mask_control[YSIZEPB] ; //works if control parameters are scalars.
//char ctl_parameters[YSIZEPB]= CTL_PARAMETERS; //array containing control parameters
string ctl_parameters[]= {"emis_c","rveg_c","z0_c","so_cond_c","so_capa_c","albvis_c","rsol_c","dpu_c","mx_eau_c","hum_c_12"};
// \JB


//_____________________________PROTOTYPAGES_____________________________________

void xdisplay();
float qsatcalc(double,double);
float dev_qsatcalc(double,double);
double fz(double);
double epsilon(double);
double tempfunc(double);
void init_hydrol();
void init_thermosoil();double intfactw_w[ngrnd][nbdl];		 //thermosoil_humlev
double intfactw_s[nbdl][ngrnd];		 //thermo_soil_diaglev
double zz_coef[ngrnd]; 			 // init_thermosoil
void init_diffuco();
void init_condveg();
void init_slowproc();
//void init_qsfrict();
void xsaison(int argc, char *argv[]);
void fichier_gradients(int t);

//_______________________________________________________________________________

// JB
//Init mask for control parameters
void init_mask_control();

//Set the module modname as control parameter
void xset_control(int argc, char *argv[]); 

//set to zero gradient of non-control variables
void reset_grad_noncontrol();
// \JB

//Fortran headers


/*extern "C"
{
	void tridia1_(double *,double *,double *,double *,int *);
}*/

// Les fonctions propres à l'appli :===========================================
//   les OBLIGATOIREST_coef_soil.h: In member function ‘void T_coef_soil::forward(double, double, double, double)’:


//_____________________________________________________________________________
void appli_start (int argc, char *argv[])
{			// permet si besoin de prendre la main des le debut de l'appli


  printf("////////////////////////////////////////////////////////////////////////\n");
  printf("//                          SECHIBA-YAO                               //\n");
  printf("//                    H. Benavides 30-07-2011                         //\n");
  printf("//                      CLIMMOD-LOCEAN-LSCE                           //\n");
  printf("//====================================================================//\n");


	//printf("simple epsilon %e    epsilon %e   log2(epsilon_sim) %e  log2(epsilon) %e \n\n ",FLT_EPSILON, DBL_EPSILON,log2(FLT_EPSILON),log2(DBL_EPSILON));
  
	//call to inizialize some variables from each routine
	
	//init_slowproc();
	init_hydrol();
	init_thermosoil();
	init_diffuco();
	init_condveg();	
	init_qsfrict();

	// JB
	init_mask_control();
	// \JB
}

//_____________________________________________________________________________
void before_it (int Yit)
{	//printf("-----------------pase 4\n");
	//printf("Yt %d\n",Yt);
	//printf("fluxlat : %e \n",YS_E_flux_vefl(0,Yi,Yt));// permet d'intervenir si besoin avant une iteration
}
//_____________________________________________________________________________
void cost_function (int pdt)
{			// fonction de cout (si les standards ne conviennent pas)

	
}
//_____________________________________________________________________________
void adjust_target ()
{			// fonction d'ajustement (si la standard ne convient pas)
}
//_____________________________________________________________________________
void after_it (int nit)
{	// permet d'intervenir si besoin apres une iteration
		
	xdisplay();	

	// JB
	//set to zero gradient of non-control variables
	reset_grad_noncontrol();
	// \JB
}
//_____________________________________________________________________________
void forward_before (int ctrp)
{			// permet d'intervenir si besoin avant le forward
	dtradia = dT;




	//printf("Yt %d   mult %e  one_day_pdt %e \n",Yt,mult,one_day_pdt);
}
//_____________________________________________________________________________
void forward_after (int ctrp)
{			// permet d'intervenir si besoin apres le forward
//pb: affichage des valeurs: ca affiche toujours les memes valeurs (initiales)
		/*if(Yt >= 1) printf("%d %e \n",Yt,YS_E_tempsol(0,Yi,Yt-1));

		if(Yt == STm)
			printf("%e \n",YS_temp_sol(0,0));*/

		//printf("fluxsens: %e      fluxlat: %e   \n",YS_fluxsens(0,0),YS_fluxsens(0,0));

}
//_____________________________________________________________________________
void backward_before (int ctrp)
{			// permet d'intervenir si besoin avant le backward
	//printf("pase1 \n");

}
//_____________________________________________________________________________
void backward_after (int ctrp)
{

	//if(Yt > obs_pdt-4)
	//printf("%d YG_dpu_cste(0,0) %e    YG_dpu_c(0,0) %e   \n ",Yt,YG_dpu_cste(0,0), YG_dpu_c(0,0));
	//printf("%d dpu_cste1(0,0) %e      dpu_c(0,0) %e         mx_eau_eau_c(0,0) %e      mx_eau_c(0,0) %e   \n  ",Yt,YG_dpu_cste1(0,0), YG_dpu_c(0,0) ,YG_mx_eau_eau(0,0), YG_mx_eau_c(0,0)   );
	//printf("   YG_rveg(0,0) %e    YG_rveg_c(0,0) %e    \n",YG_rveg_par(0,0), YG_rveg_c(0,0));
	//}

	

	
	
	if(Yt==0)
	{
		if(sensibilite == 0)
		{
			fichier_gradients(0);
			sensibilite =1;
		}
		else
		{
			fichier_gradients(1);
		}

		//gradient
		/*rsol_file << YG_rsol_cste(0,0) << "\n";
		min_drain_file << YG_min_drain(0,0) << "\n";
		max_drain_file << YG_max_drain(0,0) << "\n";
		exp_drain_file << YG_exp_drain(0,0) << "\n";
		dpu_file << YG_dpu_cste1(0,0) << "\n";
		so_capa_file << YG_so_capa(0,0) << "\n";
		so_cond_file << YG_so_cond(0,0) << "\n";
		mx_eau_file  << YG_mx_eau_eau(0,0) << "\n";
		rveg_file << YG_rveg_par(0,0) << "\n";
		z0_file << YG_z0_par(0,0) << "\n";
		humc_file << YG_humr_c_12(0,0) << "\n";
		//z0_over_file << YG_z0_over(0,0) << "\n";
		albvis_file << YG_albedovis(0,0) << "\n";
		albnir_file << YG_albedonir(0,0) << "\n";*/

		//printf("%d YG_rsol_cste(0,0) %e    YG_rsol_c(0,0) %5.15e    ",Yt,YG_rsol_cste(0,0), YG_rsol_c(0,0));

		rsol_file << YG_rsol_c(0,0) << "\n";
		min_drain_file << YG_min_drain_c(0,0) << "\n";
		max_drain_file << YG_mx_drain_c(0,0) << "\n";
		exp_drain_file << YG_e_drain_c(0,0) << "\n";
		dpu_file << YG_dpu_c(0,0) << "\n";
		so_capa_file << YG_so_capa_c(0,0) << "\n";
		so_cond_file << YG_so_cond_c(0,0) << "\n";
		mx_eau_file  << YG_mx_eau_c(0,0) << "\n";
		rveg_file << YG_rveg_c(0,0) << "\n";
		z0_file << YG_z0_c(0,0) << "\n";
		humc_file << YG_hum_c_12(0,0) << "\n";
		//z0_over_file << YG_z0_over_c(0,0) << "\n";
		albvis_file << YG_albvis_c(0,0) << "\n";
		albnir_file << YG_albnir_c(0,0) << "\n";
		emis_file << YG_emis_c(0,0) << "\n";


		//tempsol_yw<< YW_E_tempsol(0,0) << "\n";


		//closing files
		rsol_file.close();
		min_drain_file.close();
		max_drain_file.close();
		exp_drain_file.close();
		dpu_file.close();
		so_capa_file.close();
		so_cond_file.close();
		mx_eau_file.close();
		rveg_file.close();
		z0_file.close();
		humc_file.close();
		tempsol_yw.close();
		//z0_over_file.close();
		albvis_file.close();
		albnir_file.close();
		//printf("Yt %d   hgfhgfhgfhgfh %e \n",Yt, YG_rsol_c(0,0));

		
	}

	//printf("Yt %d    YG_mx_eau %e \n",Yt, YG_rsol_c(0,0));

}
//_____________________________________________________________________________
short select_io(int indic, char *nmmod, int sortie, int iaxe, int jaxe, int kaxe, int pdt, YREAL *val)
{	
	return(1);

}


//-----------------
void xdisplay ()
{

}

//________________________________________________________________________________________________________________

//------------------------ controle des selections pour select_io -------double cstgrnd;

int xsetsel (int argc, char *argv[])
{
  return(1);
}
//________________________________________________________________________________________________________________

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////FUNCIONES varias////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


//________________________________________________________________________________________________________________
//de thermosoil
// fz(rk), the function's result, is the "rk"th element of a geometric series with first element fz1 and ration zalpha.
//         this function is used to calculate the thermal layer's boundaries and center depths.
double fz(double rk)
{
	return fz1 * (pow(zalph,rk) - 1) / (zalph - 1);
}


//________________________________________________________________________________________________________________
//this function interpolates value between ztempmin and ztempmax used for lai detection.
double tempfunc(double temp_in)
{
	/* --------------------------------------------------------------------
	 this function interxpolates value between ztempmin and ztempmax
	 used for lai detection
	--------------------------------------------------------------------*/

	double tempfunc_result; // Temperature in degre Kelvin
	double ztempmin=273;    // Temperature for laimin
	double ztempmax=293;    // Temperature for laimax
	double zfacteur;        // Interpolation factor

	zfacteur = 1/pow(ztempmax-ztempmin,2); //0.0025
	if (temp_in > ztempmax)
		tempfunc_result = 1;
	else if(temp_in < ztempmin) 
		tempfunc_result = 0;
	else 
		tempfunc_result = 1-zfacteur*pow(ztempmax-temp_in,2);

	//printf("%d,%d  tempmax %e     temp_in %5.15e    tempfunc_result %e",Yt,Yj, ztempmax, temp_in,  tempfunc_result );

	return tempfunc_result;

}


//________________________________________________________________________________________________________________
//------------------computes qsat interpolation into two successive temperature ------------
float dev_qsatcalc(double temp_in, double pres_in)
{
	float dev_qsat_out; //output

	float jt, zz_a, zz_b, zz_c, zz_f;

	
	//1. computes qsat interpolation into two successive temperature
	jt = int(float(temp_in)+0.5);

	//Pixels where the temperature is too high 
	if(jt >= max_temp-1)   
		jt = max_temp-1;

	//Pixels where the temperature is too low
	if(jt <= min_temp)   
		jt = min_temp;

	//Temporary variables needed for interpolation
	zz_f = float(temp_in) +0.5 - float(jt);
	zz_a = qsfrict[int(jt-1)];
	zz_b = qsfrict[int(jt)];
	zz_c = qsfrict[int(jt+1)];

	//2. interpolates between this two values
	dev_qsat_out = ( (zz_c-2*zz_b+zz_a) * (zz_f-1) + zz_c-zz_b)/pres_in;

	//printf("Yt %d     tempin %e    jt %e    jt %d     dev %e   zzf %e    zza %e   zzb %e   zzc %e  \n",Yt,temp_in,jt,int(jt),dev_qsat_out,zz_f,zz_a,zz_b,zz_c );

	return dev_qsat_out;	
}



//________________________________________________________________________________________________________________
float qsatcalc(double temp_in,double pres_in)
{
	float qsat_out; //sortie

	float zz_a, zz_b, zz_f, jt;

	//1. computes qsat interpolation into two successive temperature

	//Diagnostic pixels where the temperature is too high 
	jt = int(temp_in);
	if(jt >= max_temp-1)   
		jt = max_temp-1;
	//Diagnostic pixels where the temperature is too low
	if (jt <= min_temp) 
		jt = min_temp;


	zz_f = float(temp_in) -float(jt);
	zz_a = qsfrict[int(jt)];
	zz_b = qsfrict[int(jt+1)];

	//2. interpolates between this two values
    	qsat_out = ((zz_b-zz_a)*zz_f+zz_a)/pres_in;
	
	return qsat_out;	
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////FUNCIONES DE INICIALIZACION DE MODULOS//////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//________________________________________________________________________________________________________________
//fonction d'nicialisation de diffuco
void init_diffuco()
{
}

//________________________________________________________________________________________________________________
//fonction d'nicialisation de condveg
void init_condveg()
{
	//calcul de albedo dry and wet
	xsoil_albedo();

}

//________________________________________________________________________________________________________________
//fonction d'nicialisation de thermosoil
void init_thermosoil()
{
	cstgrnd=sqrt(one_day / pi);
 	fz1 = 0.3 * cstgrnd;
	zalph = 2;

		

}

//________________________________________________________________________________________________________________
//fonction d'nicialisationde hydrolc
void init_hydrol()
{
	for(int i = 0 ; i < nvm ; i++)
	{
		throughfall_by_pft[i] = 0.3; 
		//wmax_veg[i] = 150; 
		//printf("th %e \n",throughfall_by_pft[i]);
	
	}
}


//________________________________________________________________________________________________________________
//fonction d'nicialisation de slowproc
void init_slowproc()
{
}



// JB

//Init mask for control parameters
void init_mask_control() 
{
	int j;
  	for (j=0;j<YSIZEPB;j++) 
	{
    		mask_control[j]=0;
  	}
}

//Set the module modname as control parameter
void xset_control(int argc, char *argv[])
{ 
  	int j;
  	for (j=0;j<YSIZEPB;j++) 
	{
    		if (!strcmp(ctl_parameters[j].c_str(),argv[1]))
		{
			mask_control[j]=1;
			break;
     		 }
  	}
}

//set to zero gradient of non-control variables
void reset_grad_noncontrol() 
{
	int j;
	char modname[15];
	//string modname;
	for (j=0;j<YSIZEPB;j++) 
	{
		if (mask_control[j]==0) 
		{ //grad has to be reset
			strcpy(modname,ctl_parameters[j].c_str());
			RESET_GRAD(emis_c);
			RESET_GRAD(rveg_c);
			RESET_GRAD(z0_c);
			RESET_GRAD(so_cond_c);
			RESET_GRAD(so_capa_c);
			RESET_GRAD(albvis_c);
			RESET_GRAD(rsol_c);
			RESET_GRAD(dpu_c);
			RESET_GRAD(mx_eau_c);
			RESET_GRAD(hum_c_12);
     		 }
    	}
 }
// \JB



//________________________________________________________________________________________________________________
//
void fichier_gradients(int t)
{


	string file;			//nom du ficher
	string chemin = "/usr/home/hbplod/data/DOCTORADO/yao_projects/SECHIBA/sechiba4/output/gradients";
	//string chemin = "/home/hbplod/Documents/sechiba22/output/gradients";
	string extension = ".dat";	//Extension du fichier
	string rest_file;		//nom du fichier en string
	int te ;			//longitude de la chaine ficher+path
	char result[1000] ;		//nombre du ficher en char

	if(t == 0)
	{
		//1) rsol
		file = "YG_rsol_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		rsol_file.open (rest_file.c_str(),fstream::trunc);

		//2) mindrain
		file = "YG_mindrain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		min_drain_file.open (rest_file.c_str(),fstream::trunc);

		//3) max_drain
		file = "YG_max_drain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		max_drain_file.open (rest_file.c_str(),fstream::trunc);

		//4) exp_drain
		file = "YG_exp_drain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		exp_drain_file.open (rest_file.c_str(),fstream::trunc);

		//5) dpu
		file = "YG_dpu_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		dpu_file.open (rest_file.c_str(),fstream::trunc);

		//6) so_capa
		file = "YG_so_capa_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		so_capa_file.open (rest_file.c_str(),fstream::trunc);
		
		//7) so_cond
		file = "YG_so_cond_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		so_cond_file.open (rest_file.c_str(),fstream::trunc);

		//8) mx_eau
		file = "YG_mx_eau_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		mx_eau_file.open (rest_file.c_str(),fstream::trunc);

		//9) rveg
		file = "YG_rveget_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		rveg_file.open (rest_file.c_str(),fstream::trunc);

		//10) z0
		file = "YG_z0_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		z0_file.open (rest_file.c_str(),fstream::trunc);
	
		//11) humc
		file = "YG_humc_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		humc_file.open (rest_file.c_str(),fstream::trunc);

		/*//12) z0_over_c
		file = "YG_z0_over_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		z0_over_file.open (rest_file.c_str(),fstream::trunc);*/

		//13) albvis_c
		file = "YG_albvis_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		albvis_file.open (rest_file.c_str(),fstream::trunc);

		//14) albnir_c
		file = "YG_albnir_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		albnir_file.open (rest_file.c_str(),fstream::trunc);
		
		//15) emis_c
		file = "YG_emis_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		emis_file.open (rest_file.c_str(),fstream::trunc);


		/*rsol_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_rsol_1dia.dat",fstream::trunc);
		min_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_mindrain_1dia.dat",fstream::trunc);
		max_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_maxdrain_1dia.dat",fstream::trunc);
		exp_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_expdrain_1dia.dat",fstream::trunc);
		dpu_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_dpu_1dia.dat",fstream::trunc);
		so_capa_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_so_capa_1dia.dat",fstream::trunc);
		so_cond_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_so_cond_1dia.dat",fstream::trunc);
		mx_eau_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_mx_eau_1dia.dat",fstream::trunc);
		rveg_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_rveget_1dia.dat",fstream::trunc);
		z0_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_z0_1dia.dat",fstream::trunc);
		humc_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_humc_1dia.dat",fstream::trunc);			
		tempsol_yw.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YW_tempsol_1dia.dat",fstream::trunc);*/
	}
	else
	{
		//1) rsol
		file = "YG_rsol_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		rsol_file.open (rest_file.c_str(),fstream::app);

		//2) mindrain
		file = "YG_mindrain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		min_drain_file.open (rest_file.c_str(),fstream::app);

		//3) max_drain
		file = "YG_max_drain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		max_drain_file.open (rest_file.c_str(),fstream::app);

		//4) exp_drain
		file = "YG_exp_drain_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		exp_drain_file.open (rest_file.c_str(),fstream::app);

		//5) dpu
		file = "YG_dpu_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		dpu_file.open (rest_file.c_str(),fstream::app);

		//6) so_capa
		file = "YG_so_capa_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		so_capa_file.open (rest_file.c_str(),fstream::app);
		
		//7) so_cond
		file = "YG_so_cond_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		so_cond_file.open (rest_file.c_str(),fstream::app);

		//8) mx_eau
		file = "YG_mx_eau_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		mx_eau_file.open (rest_file.c_str(),fstream::app);

		//9) rveg
		file = "YG_rveget_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		rveg_file.open (rest_file.c_str(),fstream::app);

		//10) z0
		file = "YG_z0_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		z0_file.open (rest_file.c_str(),fstream::app);
	
		//11) humc
		file = "YG_humc_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		humc_file.open (rest_file.c_str(),fstream::app);

		/*//12) z0_over_c
		file = "YG_z0_over_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		z0_over_file.open (rest_file.c_str(),fstream::app);*/

		//13) albvis_c
		file = "YG_albvis_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		albvis_file.open (rest_file.c_str(),fstream::app);

		//14) albnir_c
		file = "YG_albnir_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		albnir_file.open (rest_file.c_str(),fstream::app);

		//15) emis_c
		file = "YG_emis_";
		strcpy(result,chemin.c_str());
		strcat(result,path_f.c_str());
		strcat(result,file.c_str());
		strcat(result,sufix.c_str());
		strcat(result,extension.c_str());
		te = strlen(chemin.c_str())+ strlen(path_f.c_str())+strlen(file.c_str())+strlen(sufix.c_str())+strlen(extension.c_str());
		rest_file.assign(result,te) ;
		//printf("---%s---\n",rest_file.c_str());
		emis_file.open (rest_file.c_str(),fstream::app);



			
		/*rsol_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_rsol_1dia.dat",fstream::app);
		min_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_mindrain_1dia.dat",fstream::app);
		max_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_maxdrain_1dia.dat",fstream::app);
		exp_drain_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_expdrain_1dia.dat",fstream::app);
		dpu_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_dpu_1dia.dat",fstream::app);
		so_capa_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_so_capa_1dia.dat",fstream::app);
		so_cond_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_so_cond_1dia.dat",fstream::app);
		mx_eau_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_mx_eau_1dia.dat",fstream::app);
		rveg_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_rveget_1dia.dat",fstream::app);
		z0_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_z0_1dia.dat",fstream::app);
		humc_file.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YG_humc_1dia.dat",fstream::app);
		tempsol_yw.open ("./output/gradients/analyse/jour/PFT12/ete/resultats/YW_tempsol_1dia.dat",fstream::app);*/

		}
}

