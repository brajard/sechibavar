  
/***************************************************************************
                 module classe D_snow_corr.h  -  description
                             -------------------

Description: Add the beta coefficients calculated from other surfaces types (snow on ice,lakes, cities...)

output: vbeta1_corr

 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL frac_nobio,YREAL snow_nobio,YREAL speed,YREAL q_cdrag,YREAL rau,YREAL qsatt,YREAL qair)
  {
	double vbeta1_add, subtest;

	vbeta1_add = frac_nobio * MAX(MIN(snow_nobio/snowcri,1), 0);

	//Limitation of sublimation in case of snow amounts smaller than the atmospheric demand. 

	subtest = (Yt - 1) * dT * vbeta1_add * speed * q_cdrag * rau * ( qsatt - qair );

	if ( subtest > 0 ) 
	{
              zrapp = snow_nobio / subtest;
		if ( zrapp < 1 ) 
			vbeta1_add = vbeta1_add * zrapp;
              
	}

           YS1  = vbeta1 + vbeta1_add;

		

	
	
  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL frac_nobio,YREAL snow_nobio,YREAL speed,YREAL q_cdrag,YREAL rau,YREAL qsatt,YREAL qair)
  {
	
	
  }

  //===========================================================================

//************************** End of MODULE D_snow_corr.h ********************************














