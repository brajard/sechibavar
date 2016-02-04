  
/***************************************************************************
                 module classe constantes_soil.h  -  description
                             -------------------

Description: "" module contains public physical constants of soil



===========================================================================*/

#include "./constantes.h"		//General constants file


//___________________________________________________________________________________________________________________________________________
//Hydrolc
double min_resdis= 2e-5; 	// The minimal size we allow for the upper reservoir (m) --> constantes_soil
double qwilt= 5; 		//  Wilting point (Has a numerical role for the moment) 
double hcrit_litter= 0.08; 	//constantes_optim
double dsg_min = 0.001;



//Thermosoil
double sn_dens = 330;			// Snow density for the soil thermodynamics (unitless)
double sn_capa = 693000;	// Heat capacity for snow (J/m^3/K)
//double sn_capa = 2100.0*sn_dens;	// Heat capacity for snow (J/m^3/K)
double sn_cond = 0.3; 			// Thermal Conductivity of snow  (W/m^2/K)
double so_cond_dry= 0.40; 		// Dry soil Thermal Conductivity of soils (W/m^2/K)
double so_cond_wet= 1.89; 		// Wet soil Thermal Conductivity of soils (W/m^2/K)
double so_cond_m = 1.5396 ; 		// Average Thermal Conductivity of soils (W/m^2/K)
double so_capa_m = 2.0514e+6 ; 		// Average Heat capacity of soils( W/m^2/K)


/*
double cstgrnd= 1.658371917462411e+02;
//double cstgrnd=sqrt(one_day / pi);
double fz1 = 0.3 * cstgrnd;
double zalph = 2;
*/
double cstgrnd;
double fz1;
double zalph;




double lambda1 = 8e-6;
double lambda2 = 14e-6;
double dlambda = 0.5e-6;
double EPSILO = 1e-10;
double hc = 6.62618e-34; 
double ch = 2.997925e+8;
double kb = 1.38066e-23;
double nlamb = 12 ; //NINT((lambda2-lambda1)/dlambda)





//1) diffuco
double iice = 1; 					//Index for land ice (see nnobio)

//parametres controlees ou pas (mais elles sont definise dans des modules)
//thermo
//double so_capa_dry= 1.80e+6; 		// Dry soil Heat capacity of soils (J/m^3/K)
//double so_capa_wet= 3.03e+6; 		// Wet soil Heat capacity of soils (J/m^3/K)


//hydrolc
//double rsol_cste= 33e3; 	//Constant in the computation of resistance for bare  soil evaporation (s/m^2)
//double min_drain= 0.001; 	//Diffusion constant for the slow regime (This is for the diffusion between reservoirs -->constantes_soil
//double max_drain= 0.1;	// Diffusion constant for the fast regime --> constantes_soil2
//double exp_drain= 1.5;	//The exponential in the diffusion law --> constantes_soil
//double dpu_cste = 2;		//Total depth of soil reservoir --> constantes_optim
//double mx_eau_eau = 150; 	//Maximum quantity of water (Kg/M3)
//double so_cond = 1.5396 ; 		// Average Thermal Conductivity of soils (W/m^2/K)
//double so_capa = 2.0514e+6 ; 		// Average Heat capacity of soils( W/m^2/K)



//===========================================================================

//************************** FIN DU MODULE constantes_soil.h ********************************




