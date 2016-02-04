  
/***************************************************************************
                 module  H_snow_melt_i.h  -  description
        

Descripcion: snow melt only if temperature positive
                     -------------------
outputs: sn --> snow_nobio, when snowing. Snow mass on nobio areas (kg/m^2)


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL snow_nobio1, YREAL  frac_nobio, YREAL  precip_snow, YREAL  precip_rain, YREAL subsnownobio)
  {
	YS1 = (snow_nobio1 + frac_nobio*precip_snow + frac_nobio*precip_rain) - subsnownobio;	
  }
	
  //===========================================================================
  //                           backward
  backward(YREAL snow_nobio1, YREAL  frac_nobio, YREAL  precip_snow, YREAL  precip_rain, YREAL subsnownobio)
  {
	YJ1I1 = 1;
	YJ1I2 = precip_snow + precip_rain;
	YJ1I3 = frac_nobio;
	YJ1I4 = frac_nobio;
	YJ1I5 = -1;
  }

  //===========================================================================

//************************** End of MODULE H_snow_melt_i.h ********************************














