  
/***************************************************************************
                 module  H_veg_sum.h  -  description

SUM of vegetation variation
vegchtot is the total change of "veget" in the grid-points, integrated over the PFTs (vegchtot in kg m^{-2})
It is the sum of the absolute values of changes, it may thus be larger than 1 : it serves as a flag of change in each grid-point

output:
	YS1=vegchtot--> 
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vmr, YREAL vt)
  {
	if(Yt > 1)
	{
		if(Yj == 0)
			YS1 = abs(vmr);
		else
			YS1 = vt + abs(vmr);
	}
	else
		YS1 = 0;
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL vmr, YREAL vt)
  {
	if(Yt > 1)
	{
		if(Yj == 0)
		{
			YJ1I1 = 1;
			YJ1I2 = 0;
		}
		else
		{

			YJ1I1 = 1;
			YJ1I2 = 1;
		}
	}
	else
	{
		YJ1I1 =YJ1I2 = 0;
		
	}
	
  }

  //===========================================================================

//************************** End of  MODULE H_veg_sum.h  ********************************














