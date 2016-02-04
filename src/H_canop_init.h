  
/***************************************************************************
                 module  H_canop_init.h  -  description

CANOPY PROCESSES

output
	YS1 = qsintveg--->  Water on vegetation due to interception (modified)
			     -The interception loss is take off the canopy with (qsintveg - vevapwet)
			     -precip_rain is shared for each vegetation type


throughfall_by_pft = Fraction of rainfall that cannot be intercepted and falls to the ground, defined by PFT (1; 1)
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL qsintveg_o, YREAL vevapwet, YREAL veget, YREAL precip_rain)
  {
	
	if(Yt > 1)
		YS1 = qsintveg_o - vevapwet + (veget*(1-throughfall_by_pft[Yj])*precip_rain) ;
	else
		YS1 = 0;

	

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL qsintveg_o, YREAL vevapwet, YREAL veget, YREAL precip_rain)
  {
	if(Yt > 1)
	{
		YJ1I1 = 1;
		YJ1I2 = -1;
		YJ1I3 = (1-throughfall_by_pft[Yj])*precip_rain;
		YJ1I4 = veget*(1-throughfall_by_pft[Yj]);
	}
	else
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = 0;	
  }

  //===========================================================================

//************************** End of MODULE H_canop_init.h ********************************














