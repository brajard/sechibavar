  
/***************************************************************************
                 module  H_veg_var.h  -  description



output: 
	YS1 = vmr--->VARIATION OF VEGET
	YS2 = qsintveg2--->Water on vegetation due to interception over old veget!

qsintveg2 is the intercepted water in mm if the total volume was redistributed over the previous "veget" fractions 
This the distribution of intercepted water that needs to be chnaged if "veget" changes

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL resdist, YREAL qsintveg)
  {

	if(Yt > 1)
	{
		//YS1-->VARIATION OF VEGET
		if (abs(veget-resdist) > 1e-20) 
			YS1 = veget-resdist;
		else
			YS1 = 0;		

		//YS2-->Water on vegetation due to interception over old veget
		if(resdist > 0)
			YS2 = qsintveg/resdist;
		else
			YS2 = 0;	
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget, YREAL resdist, YREAL qsintveg)
  {
	if(Yt > 1)
	{
		//YS1-->VARIATION OF VEGET
		if (abs(veget-resdist) >1e-20) 
		{			
			YJ1I1 = 1;
			YJ1I2 = -1;
			YJ1I3 = 0;
		}
		else
		{
			YJ1I1 = YJ1I2 = YJ1I3 =0;
		}

		//YS2-->Water on vegetation due to interception over old veget
		if(resdist >0)
		{
			YJ2I1 = 0;
			YJ2I2 = -qsintveg/pow(resdist,2);
			YJ2I3 = 1/resdist;

			
		}
		else
		{
			YJ2I1 = YJ2I2 = YJ2I3 =0;
		}

	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 =0;
		YJ2I1 = YJ2I2 = YJ2I3 =0;
	}
	
  }

  //===========================================================================

//************************** End of  MODULE H_veg_var.h  ********************************

