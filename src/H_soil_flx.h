  
/*****************************************************************************
                 module  H_soil_flux.h -  description

we have only one flux field corresponding to water evaporated from the surface

outputs:
	YS1 = zeflux--->Soil evaporation;
	YS2 = zpreci--->Soil precipitation;
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL transpir, YREAL precisol)
  {
	if(Yt > 1)
	{
		if( veget > 0 ) 
		{
			YS1 = transpir/veget;
			YS2 = precisol/veget;
		}
		else
		{
			YS1 = 0;
			YS2 = 0;
		}
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget, YREAL transpir, YREAL precisol)
  {
	if(Yt > 1)
	{
		if( veget > 0 ) 
		{
			YJ1I1 = -transpir/pow(veget,2);
			YJ1I2 = 1/veget;
			YJ1I3 = 0;

			YJ2I1 = -precisol/pow(veget,2);
			YJ2I2 = 0;			
			YJ2I3 = 1/veget;
		}
		else
		{
			YJ1I1 =YJ1I2 =YJ1I3 = 0;
			YJ2I1 =YJ2I2 =YJ2I3 = 0;
		}
	}
	else
	{
		YJ1I1 =YJ1I2 =YJ1I3 = 0;
		YJ2I1 =YJ2I2 =YJ2I3 = 0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_flx.h ***********************

