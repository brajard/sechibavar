  
/******************************************************************************
                 module  H_soil_dsp.h  -  description


output:
	YS1 = dsp --> modified taking mean_bqsb to the computing of this parameter
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL dsp, YREAL mean_bqsb, YREAL veget, YREAL ruu_ch, YREAL bqsb, YREAL dpu_cste)
  {
	if(Yt > 1)
	{
		if(veget > 0)
		{
			YS1 = dpu_cste - mean_bqsb/ruu_ch;
			YS2 = mean_bqsb;
		}
		else
		{
			YS1 = dsp;
			YS2 = bqsb;
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
  backward (YREAL dsp, YREAL mean_bqsb, YREAL veget, YREAL ruu_ch, YREAL bqsb, YREAL dpu_cste)
  {
	if(Yt > 1)
	{
		if(veget > 0)
		{	YJ1I2 = -1/ruu_ch;
			YJ1I4 = mean_bqsb/(ruu_ch*ruu_ch);
			YJ1I6 = 1;
			YJ1I1 =  YJ1I3 =  YJ1I5 = 0;

			YJ2I2 = 1;
			YJ2I1 =  YJ2I3 = YJ2I4 =  YJ2I5 =YJ2I6 = 0; 
		}
		else
		{
			YJ1I1 = 1;
			YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5  = YJ1I6 =0;

			YJ2I5 = 1;
			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =YJ2I6 =  0; 
		}
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 =YJ1I6 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =  YJ2I5 = YJ2I6 =0; 
	}
	
  }

  //===========================================================================

//****************** End of MODULE H_soil_dsp.h    ***************************

