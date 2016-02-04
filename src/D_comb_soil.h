  
/***************************************************************************
                 module  D_comb_soil.h  -  description
                             -------------------

-modify valpha and vbetas where necessary. (when toveg=1), put coefficient into the interception reservoir and on the bare soil.

outputs
	YS1 = vbeta_m;
	YS2 = vbeta1_m;
	YS3 = vbeta4_m;
	YS4 = valpha;

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL toveg, YREAL veget, YREAL vbeta, YREAL vbeta4, YREAL vbeta1, YREAL tosnow)
  {

    	if(Yt > 1)
	{
		YS1 = vbeta;
		YS2 = vbeta1;
		YS3 = vbeta4;

		if(toveg > 0.5)
		{
			YS1 = vbeta4;
			YS2 = 0;
			YS3 = veget;
				
		}
		if(tosnow > 0.5)
		{
			YS1 = 1;
			YS2 = 1;
			YS3 = 0;
						
		}
		YS4 = 1;

		
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
		YS4 = 0;
	}

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL toveg, YREAL veget, YREAL vbeta, YREAL vbeta4, YREAL vbeta1, YREAL tosnow)
  {

	if(Yt > 1)
	{
		YJ1I1 = YJ1I2 =YJ1I4 =YJ1I5 = YJ1I6 = 0;
		YJ1I3 = 1;

		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I6 = 0;
		YJ2I5 = 1;

		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I5 = YJ3I6 = 0;
		YJ3I4 = 1;

		if(toveg > 0.5)
		{
			
			YJ1I1 = YJ1I2 =YJ1I3 = YJ1I5 = YJ1I6 = 0;
			YJ1I4 = 1;

			YJ2I1 =YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 = 0;

			YJ3I1 =  YJ3I3 = YJ3I4 =YJ3I5 = YJ3I6 = 0;
			YJ3I2 = 1;
		}
		if(tosnow > 0.5)
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 =YJ1I5 = YJ1I6 = 0;
			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =YJ2I5 = YJ2I6 = 0;
			YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 =YJ3I5 = YJ3I6 = 0;
						
		}
		YJ4I1 = YJ4I2 =YJ4I3 = YJ4I4 = YJ4I5 = YJ4I6 = 0;

	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 =YJ1I5 = YJ1I6 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =YJ2I5 = YJ2I6 = 0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 =YJ3I5 = YJ3I6 = 0;
		YJ4I1 = YJ4I2 = YJ4I3 = YJ4I4 = YJ4I5 = YJ4I6 = 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE D_comb_soil.h ********************************














