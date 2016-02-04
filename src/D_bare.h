  
/***************************************************************************
                 module  D_bare.h  -  description
                             -------------------

artificial beta coefficient : bare soil
vbeta4: Beta for bare soil evaporation

Soil resistance and beta for bare soil, veget (  ,1) contains the fraction of bare soil

veget: only first dimension

 ***************************************************************************/

  //===========================================================================
  //                          forward
  forward (YREAL q_cdrag, YREAL wind, YREAL rsol, YREAL veget)
  {
	double temp1;
	
	
	   
	if((Yt > 1) && (veget > min_sechiba))

	{

			temp1 = 1 + wind * q_cdrag * rsol;
			YS1 = veget / temp1;
		

 	}
	else
		YS1 = 0;

	
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL q_cdrag, YREAL wind, YREAL rsol, YREAL veget)
  {
	double temp1 = 0;
	double d_temp_wind = 0;
	double d_temp_qc = 0;
	double d_temp_rsol = 0;
	double d_YS1_temp = 0;
	double d_YS1_veget = 0;

	if((Yt > 1) && (veget > min_sechiba))	
	{
			temp1 = 1 + wind * q_cdrag * rsol;

			d_temp_wind = q_cdrag * rsol;
			d_temp_qc = wind * rsol;
			d_temp_rsol = wind * q_cdrag;
			
			d_YS1_temp = -veget / (temp1*temp1);
			d_YS1_veget = 1 / temp1;

			
			YJ1I1 = d_YS1_temp * d_temp_qc;
			YJ1I2 = d_YS1_temp * d_temp_wind;
			YJ1I3 = d_YS1_temp * d_temp_rsol;
			YJ1I4 = d_YS1_veget;	 
		

 	}
	else
	{
		YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =0;
	}

  }

  //===========================================================================

//************************** End of MODULE D_bare.h ********************************














