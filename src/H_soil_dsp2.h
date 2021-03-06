  
/******************************************************************************
                 module  H_soil_dsp2.h  -  description



output:
	YS1 = 
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL dsp, YREAL mean_bqsb, YREAL veget, YREAL ruu_ch, YREAL bqsb, YREAL bad, YREAL dpu_cste)
  {
	if(Yt > 1)
	{
		if((veget > min_sechiba) && (bad ==1))
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
		YS1 = dsp;  
		YS2 = bqsb;   		
         }

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL dsp, YREAL mean_bqsb, YREAL veget, YREAL ruu_ch, YREAL bqsb, YREAL bad, YREAL dpu_cste)
  {
	if(Yt > 1)
	{
		if((veget > min_sechiba) && (bad ==1))
		{
			YJ1I1 = YJ1I3 = YJ1I5 = YJ1I6 = 0;
			YJ1I2 = -1/ruu_ch;			
			YJ1I4 = mean_bqsb/pow(ruu_ch,2);	
			YJ1I7 = 1;					

			YJ2I1 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = 0;
			YJ2I2 = 1;
		}
		else
		{
			YJ1I1 = 1;
			YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 = 0;

			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =  YJ2I6 = YJ2I7 =0;
			YJ2I5 = 1;
		}
	}
	else
	{		
		YJ1I1 =YJ2I5 =1;
		YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 =YJ1I7 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I6 =YJ2I7 = 0; 
         }
	
  }

  //===========================================================================

//****************** End of MODULE H_soil_dsp2.h    ***************************


