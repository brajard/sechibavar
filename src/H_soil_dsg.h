  
/*****************************************************************************
                 module  H_soil_dsg.h  -  description

                             -------------------


output: YS1 = mean_dsg ---> Compute an average moisture profile
 ***************************************************************************/
  //===========================================================================
  //                           forward
  forward (YREAL mean_gqsb, YREAL ruu_ch, YREAL vegtot, YREAL mult1)
  {

	if(Yt == 1)
	{
		YS1 = mult1;	
		if(vegtot > 0)
			YS1 = mean_gqsb/vegtot;
	}
	else
		YS1 = mean_gqsb/ruu_ch;

	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL mean_gqsb, YREAL ruu_ch, YREAL vegtot, YREAL mult1)
  {
	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 = YJ1I3 = 0;
		YJ1I4 = 1;		
		if(vegtot > 0)
		{
			YJ1I1 = 1/vegtot;
			YJ1I2 = 0;
			YJ1I3 =  -mean_gqsb/pow(vegtot,2);
			YJ1I4 = 0;
		}
	}
	else
	{
		YJ1I1 = 1/ruu_ch;
		YJ1I2 = -mean_gqsb/pow(ruu_ch,2);
		YJ1I3 = YJ1I4 = 0;
	}
  }

  //===========================================================================

//************************** End of MODULE H_soil_dsg.h ***********************


