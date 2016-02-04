  
/***************************************************************************
                 module classe E_evap_trans.h  -  description
                             -------------------
outputs:

	YS1 = vevapwet ---> wet soil evaporation
	YS2 = transpir ---> Transpiration;
	YS3 = gpp  ---> Assimilation, gC/m**2 total area. Only if stomate is activated
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vbeta2, YREAL vbeta3, YREAL xx)
  {
	if(Yt > 1)
	{
		//Interception loss        
		YS1 = xx * vbeta2 ;

		//Transpiration
		YS2 = xx * vbeta3 ;// * one_day_pdt;

		//gpp
		YS3 = 0;
	}	
	else
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
	}


  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL vbeta2, YREAL vbeta3, YREAL xx)
  {

	if(Yt > 1)
	{
		//Interception loss  
		YJ1I1 = xx;
		YJ1I2 = 0;
		YJ1I3 = vbeta2;      

		//Transpiration
		YJ2I1 = 0;
		YJ2I2 = xx;
		YJ2I3 = vbeta3;  

		//gpp
		YJ3I1 = 0;
		YJ3I2 = 0;
		YJ3I3 = 0;  
	}	
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;      

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;  

		YJ3I1 = 0;
		YJ3I2 = 0;
		YJ3I3 = 0;  
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_evap_trans.h ********************************














