  
/***************************************************************************
                 module  H_canop_interp.h  -  description

Limits the effect of interception and sum what receives soil (precisol)

outputs: 
	YS1 = precisol--->precitipation water on the ground
	YS2 = qsintveg--->Water on vegetation due to interception (modified);

zqsintvegnew --> Temporary variable for intercepted water amount  (kg m^{-2}) 
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward

  forward (YREAL qsintveg, YREAL qsintmax, YREAL veget, YREAL precip_rain, YREAL qsintveg_init)
  {
	double zqsintvegnew = 0;

	if(Yt == 1)
	{
		YS1 = 0; 
		YS2 = qsintveg_init;
	}
	else
	{
		// Limits the effect and sum what receives soil, zqsintvegnew = MIN (qsintveg,qsintmax);
		if(qsintveg > qsintmax)
			zqsintvegnew = qsintmax ;
		else if(qsintveg < qsintmax)
			zqsintvegnew = qsintveg;
		else
			zqsintvegnew = 0;
			
		

		YS1 = (veget*throughfall_by_pft[Yj] * precip_rain) + qsintveg - zqsintvegnew;

		// swap qsintveg to the new value
		YS2 = zqsintvegnew;
	}
	
  }	

  //===========================================================================
  //                           backward
  backward (YREAL qsintveg, YREAL qsintmax, YREAL veget, YREAL precip_rain, YREAL qsintveg_init)
  {
	//derivadas
	double dzqsintvegnew_qsintveg=0;
	double dzqsintvegnew_qsintmax=0;

	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = 0;	 
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = 0;
		YJ2I5 = 1;	
	}
	else
	{
		// Limits the effect and sum what receives soil, zqsintvegnew = MIN (qsintveg,qsintmax);
		if(qsintveg > qsintmax)
		{
			dzqsintvegnew_qsintmax = 1;
		}
		else if(qsintveg < qsintmax)
		{
			dzqsintvegnew_qsintveg = 1;
		}
	
		YJ1I1 = 1- dzqsintvegnew_qsintveg;
		YJ1I2 = dzqsintvegnew_qsintmax;
		YJ1I3 = throughfall_by_pft[Yj] * precip_rain;
		YJ1I4 = veget*throughfall_by_pft[Yj];	 
		
		YJ2I1 = dzqsintvegnew_qsintveg;
		YJ2I2 = dzqsintvegnew_qsintmax;
		YJ2I3 = YJ2I4 = YJ2I5 = YJ1I5 = 0;
	
	}	
  }

  //===========================================================================

//************************** End of MODULE H_canop_interp.h ********************

