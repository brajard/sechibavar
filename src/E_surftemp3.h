  
/***************************************************************************
                    module classe E_surftemp3.h  -  description

the sensitivity terms are computed, We here calculate the sensitivity for the different fluxes to changes in Yt, which is the
change in the surface static energy over the model time step (Yt).

outputs:
	YS1= netrad_sns -- > Net radiation sensitivity
	YS2= sensfl_sns --> Sensible heat flux sensitivity
	YS3= larsub_sns --> Latent heat flux sensitivity (sublimation)
  	YS4= lareva_sns --> Latent heat flux sensitivity (evaporation)                       


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL emis,YREAL psold,YREAL zikt,YREAL petAcoef,YREAL vbeta1,YREAL pdqsold,YREAL valpha, YREAL vbeta, YREAL peqAcoef)
  {
	if(Yt > 1)
	{

		YS1 = zicp * 4 * emis * c_stefan * pow( zicp * psold ,3);
		YS2 = 1 / (zikt -  petAcoef);
		YS3 = chalsu0 * vbeta1 * zicp * pdqsold / (zikt - peqAcoef);
		YS4 = chalev0 * (1 - vbeta1) * vbeta * valpha * zicp * pdqsold / (zikt - peqAcoef);
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
  backward (YREAL emis,YREAL psold,YREAL zikt,YREAL petAcoef,YREAL vbeta1,YREAL pdqsold,YREAL valpha, YREAL vbeta, YREAL peqAcoef)
  {   

	if(Yt > 1)
	{

		YJ1I1 =  zicp * 4 * c_stefan * pow( zicp * psold ,3);
		YJ1I2 = 3 * zicp * 4 * emis * c_stefan * pow( psold ,2) * pow( zicp ,3);
		YJ1I3 = YJ1I4 =YJ1I5 =YJ1I6 = YJ1I7 =YJ1I8 = YJ1I9 = 0;

		YJ2I1 = YJ2I2 =YJ2I5 = YJ2I6 = YJ2I7 = YJ2I8 = YJ2I9 = 0;
		YJ2I3 = -1 / pow(zikt -  petAcoef , 2);
		YJ2I4 = 1/ pow(zikt -  petAcoef , 2);

		YJ3I1 = YJ3I2 = YJ3I4 = YJ3I7 = YJ3I8 = 0;
		YJ3I3 = -chalsu0 * vbeta1 * zicp * pdqsold / pow(zikt - peqAcoef , 2);
		YJ3I5 = chalsu0 * zicp * pdqsold / (zikt - peqAcoef);
		YJ3I6 = chalsu0 * vbeta1 * zicp / (zikt - peqAcoef);
		YJ3I9 = chalsu0 * vbeta1 * zicp * pdqsold / pow(zikt - peqAcoef , 2);



		YJ4I1 = YJ4I2 = YJ4I4 = 0;
		YJ4I3 = -chalev0 * (1 - vbeta1) * vbeta * valpha * zicp * pdqsold / pow(zikt - peqAcoef , 2);
		YJ4I5 = -chalev0 * vbeta * valpha * zicp * pdqsold / (zikt - peqAcoef);
		YJ4I6 = chalev0 * (1 - vbeta1) * vbeta * valpha * zicp / (zikt - peqAcoef);
		YJ4I7 = chalev0 * (1 - vbeta1) * vbeta * zicp * pdqsold / (zikt - peqAcoef);
		YJ4I8 = chalev0 * (1 - vbeta1) * valpha * zicp * pdqsold / (zikt - peqAcoef);
		YJ4I9 = chalev0 * (1 - vbeta1) * vbeta * valpha * zicp * pdqsold / pow(zikt - peqAcoef , 2);
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 = YJ1I8 = YJ1I9 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = YJ2I7 = YJ2I8 = YJ2I9 = 0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = YJ3I7 = YJ3I8 = YJ3I9 = 0;
		YJ4I1 = YJ4I2 = YJ4I3 = YJ4I4 = YJ4I5 = YJ4I6 = YJ4I7 = YJ4I8 = YJ4I9 = 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp3.h ********************************














