  
/***************************************************************************
                 module  H_veg_niv.h  -  description

WATER LEVEL ponderated BY VEGET VARIATION. In the grid-points with "veget" change, we define changes in water content

output
	YS1 = gdq  -->change in surface soil layer water content (kg m^{-2});
	YS2 = bdq  -->change in bottom soil layer water content (kg m^{-2});
	YS3 = qsdq  -->change in interception reservoir water content  (kg m^{-2};

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vmr, YREAL gqsb, YREAL bqsb, YREAL qsintveg2, YREAL vegchtot)
  {
	if(Yt > 1)
	{
		if(( vegchtot > 0 ) && (vmr != 0) )
		{
			if(vmr > 0)
			{			
				YS1 = vmr * gqsb;
				YS2 = vmr * bqsb;
				YS3 = vmr * qsintveg2;
			}
			else			
			{			
				YS1 = -vmr * gqsb;
				YS2 = -vmr * bqsb;
				YS3 = -vmr * qsintveg2;
			}
		}
		else
		{
			YS1 = 0;
			YS2 = 0;
			YS3 = 0;
		}
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
	}

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL vmr, YREAL gqsb, YREAL bqsb, YREAL qsintveg2, YREAL vegchtot)
  {
	//printf("%d   %e\n",Yt,vmr);
	if(Yt > 1)
	{
		if(( vegchtot > 0 ) && (vmr != 0) )
		{
			if(vmr > 0)
			{	
				YJ1I1 = gqsb;
				YJ1I2 = vmr;
				YJ1I3 = YJ1I4 = YJ1I5 = 0;

				YJ2I1 = bqsb;
				YJ2I3 = vmr;
				YJ2I2 = YJ1I4 = YJ1I5 = 0;

				YJ3I1 = qsintveg2;
				YJ3I4 = vmr;
				YJ3I2 = YJ1I3 = YJ1I5 = 0;
			}
			else			
			{			
				YJ1I1 = -gqsb;
				YJ1I2 = -vmr;
				YJ1I3 = YJ1I4 = YJ1I5 = 0;

				YJ2I1 = -bqsb;
				YJ2I3 = -vmr;
				YJ2I2 = YJ1I4 = YJ1I5 = 0;

				YJ3I1 = -qsintveg2;
				YJ3I4 = -vmr;
				YJ3I2 = YJ1I3 = YJ1I5 = 0;
			}

			
		}
		else
		{
			YJ1I1 = YJ1I2 = YJ1I5 = YJ1I3 = YJ1I4 = 0;
			YJ2I1 = YJ2I2 = YJ2I5 = YJ2I3 = YJ2I4 = 0;
			YJ3I1 = YJ3I2 = YJ3I5 = YJ3I3 = YJ3I4 = 0;
		}
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I5 = YJ1I3 = YJ1I4 = 0;
		YJ2I1 = YJ2I2 = YJ2I5 = YJ2I3 = YJ2I4 = 0;
		YJ3I1 = YJ3I2 = YJ3I5 = YJ3I3 = YJ3I4 = 0;
	}
	
  }

  //===========================================================================

//************************** End of  MODULE H_veg_niv.h  ********************************














