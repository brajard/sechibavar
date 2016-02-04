  
/*****************************************************************************
                 module  H_soil_dsg1.h  -  description


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL resdist, YREAL dsg, YREAL mult1,YREAL dg1)
  {
	
	if(Yt==1)
	{
		if(Yj == 0)
		{
			YS1 = resdist * dsg ;
		}
		else
			YS1 = resdist* dsg + mult1;

	}
	else
	{
		YS1 = dg1;
	}
	
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL resdist, YREAL dsg, YREAL mult1,YREAL dg1)
  {
	if(Yt==1)
	{
		if(Yj == 0)
		{
			YJ1I1 = dsg;
			YJ1I2 = resdist;
			YJ1I3 = YJ1I4 = 0;
		}
		else
		{
			YJ1I1 = dsg;
			YJ1I2 = resdist;
			YJ1I3 = 1;
			YJ1I4 = 0;
		}
			YS1 = resdist* dsg + mult1;

	}
	else
	{
		YJ1I4 = 1;		
		YJ1I1 = YJ1I2 =YJ1I3 =  0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_dsg1.h ***********************


