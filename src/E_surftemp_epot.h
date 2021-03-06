  
/***************************************************************************
                    module  E_surftemp_epot.h  -  description
                             -------------------

outpût: YS1 = epot_air_new ----> new potential air energy

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL zikt,YREAL sensfl_old,YREAL sensfl_sns,YREAL dtheta,YREAL psnew)
  {
	if (Yt > 1)
		YS1 = zikt * (sensfl_old - sensfl_sns * dtheta) + psnew;
	else
		YS1 = 0;

	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL zikt,YREAL sensfl_old,YREAL sensfl_sns,YREAL dtheta,YREAL psnew)  
  {
	if (Yt > 1)
	{
		YJ1I1 = (sensfl_old - sensfl_sns * dtheta);
		YJ1I2 = zikt;
		YJ1I3 = -zikt * dtheta;
		YJ1I4 = -zikt * sensfl_sns;
		YJ1I5 = 1;
	}
	else	
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;
		YJ1I5 = 0;
	}

		
  }

  //===========================================================================

//************************** End of MODULE E_surftemp_epot.h ********************************














