  
/***************************************************************************
                 module  D_comb_vb.h  -  description
                             -------------------
! 1. The beta and alpha coefficients are calculated.

-el qsatt de aqui es el calculado en el modulo D_qsatt
-
outputs:
	YS1 = vbeta
	YS2 = toveg  ; 
	YS3 = tosnow ;
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vbeta4,YREAL snow, YREAL vbeta2sum, YREAL vbeta3sum, YREAL qsatt, YREAL qair, YREAL temp_air)
  {


	if(Yt > 1)
	{
		if(snow < snowcri)
		{
			YS1 = vbeta4 + vbeta2sum + vbeta3sum;

			
			if (vbeta4 + vbeta2sum + vbeta3sum < min_sechiba) 
				YS1 = 0;
		}
		else
			YS1 = 1;

			
		if(qair > qsatt)
		{

			if (temp_air > tp_00)
			{
				//3.1.1  If it is not freezing dew is put into the interception reservoir and on the bare soil.
				YS2 = 1;  //toveg
				YS3 = 0;  //tosnow
			}
			else
			{
				//3.1.2  If it is freezing water is put into the snow reservoir.
				YS2 = 0; //toveg
				YS3 = 1; //tosnow
			}
		}
		else
		{
	
			YS2 = 0;
			YS3 = 0;	
		}
	}
	else
	{
		YS1 = 0;
		YS2= 0;
		YS3 = 0;
	}
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL vbeta4,YREAL snow, YREAL vbeta2sum, YREAL vbeta3sum, YREAL qsatt, YREAL qair, YREAL temp_air)
  {        
	
	if(Yt > 1)
	{
		if(snow < snowcri)
		{
			
			YJ1I1 = YJ1I3 = YJ1I4 =1;
			YJ1I2 = YJ1I5 = YJ1I6 =YJ1I7 =0;
	
			if (vbeta4 + vbeta2sum + vbeta3sum < min_sechiba)
				YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 =YJ1I6 = YJ1I7 =0;
		}
		else
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 =YJ1I6 = YJ1I7 =0;

	
		if(qair > qsatt)
		{
			if (temp_air > tp_00)
			{
				//3.1.1  If it is not freezing dew is put into the interception reservoir and on the bare soil.
				YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 = YJ2I7 =0;
				YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 =YJ3I6 = YJ3I7 =0;
			}
			else
			{
				//3.1.2  If it is freezing water is put into the snow reservoir.
				YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 = YJ2I7 =0;
				YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 =YJ3I6 = YJ3I7 =0;
			}
		}
		else
		{
			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 = YJ2I7 =0;
			YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 =YJ3I6 = YJ3I7 =0;	
		}
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 =YJ1I6 = YJ1I7 =0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 = YJ2I7 =0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 =YJ3I6 = YJ3I7 =0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE D_comb_vb.h ********************************







