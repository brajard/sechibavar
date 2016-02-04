  
/***************************************************************************
                 module classe run_sechiba.h  -  description
                             -------------------


#************************************************************************************
#          LISTE D'OPTIONS NON ACTIVEES dans cette simulation
#************************************************************************************
# DEBUG_INFO is not set
# STOMATE_WATCHOUT is not set
# AGRICULTURE is not set
# HERBIVORES is not set
# TREAT_EXPANSION is not set
# RELAXATION is not set
# ALLOW_WEATHERGEN is not set
# HYDROL_OK_HDIFF is not set
# FORCESOIL_STEP_PER_YEAR=12



===========================================================================*/



// 4) condveg
double impaze = 1 ;			//
double albedo_scal[2] = {0.25,0.25};	//
//double alb_bare_model = 0.25;		//
double z0_scal = 0.01;			//  
double roughheight_scal = z0_scal;		//
double alb_bare_model = 0.25;
double emis_scal = 1 ;			//



//6) SLOWPROC

double height_presc[nvm] = { 0.,30.,30.,20.,20.,20.,15.,15.,15.,.5,.6,1,1.};		//prescribed height of vegetation. one for each vegetation type							
double qsintcst = 0.1; 									// valor de  si es q esta ahi
//int lcanop; 										//calculado. inicializado pal , pero no ta
double zcanop = 0.5; 									// ---> SECHIBA_ZCANOP, no esta, valor por default
double sechiba_vegmax[kjindex][nvm]; // = { {1, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 0}}; 	//Max fraction of vegetation type 
//double sechiba_vegmax[kjindex][nvm] = { {0.01, 0.01, 0.01, 0.88, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01}}; 	//Max fraction of vegetation type


//diffuco
double rveg_pft[nvm] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 			// trans: Artificial parameter to increase or decrease canopy resistance. This parameter is set by PFT.


/*
string VEGETATION_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/INIT/SRF/carteveg5km.nc"; 
string SOILALB_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/INIT/SRF/soils_param.nc"; 
string SOILTYPE_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/INIT/SRF/soils_param.nc"; 
string ROUTING_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/INIT/SRF/routing.nc"; 
string REFTEMP_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/INIT/SRF/reftemp.nc"; 
string LAI_FILE="/home/orchidee01/mmancip/IGCM/STORAGE/BC/SRF/lai2D.nc"; */
