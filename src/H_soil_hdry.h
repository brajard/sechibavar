  
/***************************************************************************
                 module  H_soil_hdry.h  -  description

outputs:
	YS1 = hdry--->Dry soil heigth in meters ;

linear combination between water weight in depp and surface layer; multiply by the relative soil humidity

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL a_subgrd,YREAL dss,YREAL dsp)
  {

	YS1 = a_subgrd*dss + (1-a_subgrd)*dsp;

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL a_subgrd,YREAL dss,YREAL dsp)
  {
	YJ1I1 = dss - dsp;
	YJ1I2 = a_subgrd;
	YJ1I3 = 1-a_subgrd;
	
  }

  //===========================================================================
//************************** End of MODULE H_soil_hdry.h *********************
