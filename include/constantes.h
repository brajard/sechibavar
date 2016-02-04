  
/***************************************************************************
                 module classe constantes.h  -  description
                             -------------------

Description: "" module contains public physical constants of soil



===========================================================================*/


double tp_00= 273.15;					// 0 degre Celsius in degre Kelvin (K)
double undef_sechiba= 1.E+20;				// The undef value used in SECHIBA (-)
double min_sechiba= 0.000000000000001;				// Epsilon to detect a near zero floating point (-)
int un = 1;						//
double nitermax = 100;					//variable used in hydrolc
double one_day = 86400; 				//one day in seconds (s)
//double one_day = 3600*24; 				//one day in seconds (s)
double one_day_pdt = one_day / dT;
double pi = 3.141592653589793238;			//
double year_pdt = 365* one_day_pdt -1;
double val_exp = 99999 ; // archivo de constantes







//===========================================================================

//************************** FIN DU MODULE constantes.h ********************************




