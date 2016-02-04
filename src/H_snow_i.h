  
/***************************************************************************
                 module  H_snow_i.h  -  description
                             -------------------

Description: initial value of snow  

output: YS1 = snow

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL snow1, YREAL totfrac_nobio, YREAL precip_snow)
  {

	YS1 = snow1 + (1 - totfrac_nobio)*precip_snow;
		
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL snow1, YREAL totfrac_nobio, YREAL precip_snow)
  {
	YJ1I1 = 1;
	YJ1I2 = -precip_snow;
	YJ1I3 = (1 - totfrac_nobio);	
	
  }

  //===========================================================================

//************************** End of MODULE H_snow_i.h ********************************














