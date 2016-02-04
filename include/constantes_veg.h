  
/***************************************************************************
                 module classe constantes_veg.h  -  description
                             -------------------

Description: "" module contains public physical constants



===========================================================================*/


//___________________________________________________________________________________________________________________________________________
//SECHIBA
double pa_par_hpa = 100; 				// Transform pascal into hectopascal
double cte_molr = 287.05;				//  Specific constant of dry air (J/kg/K) 
double max_temp=370; 					//  Size of local array to keep saturated humidity at each temperature level
double min_temp=100; 					// Minimum temperature for saturated humidity



//diffuco
double cte_grav = 9.80665; 				//
double cp_air = 1004.675;				//  Specific heat of air (J/kg/K)
//double zicp = 1 / cp_air; 				//  usado en todos los submodulos de surftemp 
double zicp = 0.00099535; 				//  usado en todos los submodulos de surftemp 
double msmlr_air = 28.964e-03;				//  Molecular weight of dry air (kg/mole)
double msmlr_h2o = 18.02e-03;				//  Molecular weight of water vapor (kg/mole)
/*double cp_h2o =    cp_air*(4*msmlr_air)/( 3.5*msmlr_h2o);	//  Specific heat of water vapor (J/kg/K)
double retv = (msmlr_air/msmlr_h2o) -1;			//
double rvtmp2 = (cp_h2o/cp_air) -1;			//  Ratio between molecular weight of dry air and water vapor minus 1(-)*/
double cp_h2o = 1.845530788013318e+03;	//  Specific heat of water vapor (J/kg/K)
double retv = 0.607325194228635;		//
double rvtmp2 = 0.836943079118440;			//  Ratio between molecular weight of dry air and water vapor minus 1(-)

double cepdu2 = 0.01; 					//  (m^2/s^2)
//double ct_karman = 0.35;				//  Van Karmann constant (-)
double ct_karman = 0.40;				//  Van Karmann constant (-)
double cb = 5;						//  Constantes of the Louis scheme (-)
double cc = 5;						//  Constantes of the Louis scheme (-)
double cd = 5;						//  Constantes of the Louis scheme (-)
double min_wind = 0.1;					//  The minimum wind (m/s)
double rayt_cste = 125;					//  Constant in the computation of surface resistance (W/m^2)
double defc_plus= 23e-3;				//  Constant in the computation of surface resistance (K/W)
double defc_mult= 1.5;					//  Constant in the computation of surface resistance (K/W) 
double rstruct[kjindex][nvm] = {{0.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 25.0, 2.5, 2.0, 2.0, 2.0}}; // Structural resistance(s/m). One for each vegetation type
double kzero[nvm]= {0.0, 12.e-5, 12.e-5, 12.e-5, 12.e-5, 25.e-5, 12.e-5, 25.e-5, 25.e-5, 30.e-5, 30.e-5, 30.e-5, 30.e-5}; // A vegetation dependent constant used in the calculation of the surface resistance.  (kg.m/s) (??units??)Value for kzero one for each vegetation type   
double maxmass_glacier=3000;				// kg/m2 (~ 10m snow) : critical snowmass for glaciers\n
double snow_trans=0.3; 					// (kg/m2) : critical constant for snow ageing\n
double max_snow_age=50; 				// (d) : maximum snow age\n
double dew_veg_coeff[6] = {0.887773, 0.205673, 0.110112, 0.014843,  0.000824,  0.000017}; //coefficients of the 5 degree polynomomial used in the equation of coeff_dew_veg


//ENERBIL
double kappa = 0.285714285714286; 			//  Kappa : ratio between specific constant and specific heat of dry air (-)
//double kappa = cte_molr/cp_air; 			//  Kappa : ratio between specific constant and specific heat of dry air (-)
double c_stefan= 5.6697e-8;				//  Stefan-Boltzman constant (W/m^2/K^4)
double chalsu0= 2.8345e+06;				//  Latent heat of sublimation (J/kg)
double chalev0= 2.5008e+06;				//  Latent heat of evaporation (J/kg)
double chalfu0 = 333700;	 			//  Latent heat of fusion (J/kg)
//double chalfu0 = chalsu0-chalev0; 			//  Latent heat of fusion (J/kg)
double snowcri = 1.5;  					//Sets the amount above which only sublimation occures [Kg/m^2]
double sneige=0.0015; 				// critical snowmass for snow melt
//double sneige=snowcri/1000; 				// critical snowmass for snow melt

//CONDVEG
double z0_bare = 0.15;					//  bare soil roughness length (m)
double z0_ice = 0.001;					//  ice roughness length (m)
double is_tree[nvm] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 }; //Is the vegetation type a tree ? (true 1/false 0)
//double height_displacement = 0.75;
double height_displacement = 0.67;

//leaf albedo of vegetation type, VIS+NIR (-)
double alb_leaf[2][nvm] = {{0.00, 0.04, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.10,0.10, 0.10, 0.10}, {0.00, 0.20, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.22, 0.30,0.30, 0.30, 0.30}};


//HYDROLC
double throughfall_by_pft[nvm]; //init_hydrolc  Percent by PFT of precip that is not intercepted by the canopy
//double wmax_veg[nvm];  		//init_hydrolc  Maximum field capacity for each of the vegetations (Temporary). Value of wmax_veg : max quantity of water, one for each vegetation type en Kg/M3
//double humcste[nvm] = {5, 0.8, 0.8, 1, 0.8, 0.8, 1, 1, 0.8, 4, 4, 4, 4}; //constantes_optim



//Slowproc
double llaimin[nvm] = {0, 8, 0, 4, 4.5, 0, 4, 0, 0, 0, 0, 0, 0};     //laimin for maximum lai
double llaimax[nvm] = { 0, 8, 8, 4, 4.5, 4.5, 4, 4.5, 4, 2, 2, 2, 2};//laimax for minimum lai
double ext_coef[nvm] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};  //extinction coefficient of the Monsi&Seaki relationship (1953)




/////////////////////////////////////////////////
//parametres controlees, donc dans une module
//condveg
double z0_over_height = 0.046; 			//constantes_optim





  //===========================================================================

//************************** FIN DU MODULE constantes_veg.h ********************************




