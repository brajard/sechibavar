  
/***************************************************************************
                 module classe H_veg_water.h  -  description

calculate water mass that we have to redistribute
We calculate the total water mass that we have to redistribute by grid-point, for each water reservoir
We sum up the water changes from PFTs where "veget" decreases : this is the water that needs to be redistributed
vtr is the total fraction  over which "veget" has decreased (> 0)
By construction, it is balanced by the total fraction where "veget" has increased
                             -------------------

outputs:
	YS1 = gtr ;
	YS2 = btr ;
	YS3 = qstr;
	YS4 = vtr;

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vegchtot, YREAL vmr, YREAL gdq, YREAL bdq, YREAL qsdq, YREAL g, YREAL b,YREAL  qs, YREAL vt)
  {
	if(Yt > 1)
	{
		if(Yj == 0)
		{
			if (( vegchtot > 0 ) && (vmr < 0) ) 
			{
				YS1 = gdq;
				YS2 = bdq;
				YS3 = qsdq ;
				YS4 = -vmr;
			}
			else
			{
				YS1 = 0;
				YS2 = 0;
				YS3 = 0 ;
				YS4 = 0;	
			}
		}
		else
		{
			if (( vegchtot > 0 ) && (vmr < 0) ) 
			{
				
				YS1 = g + gdq;
				YS2 = b + bdq;
				YS3 = qs + qsdq ;
				YS4 = vt - vmr;
			}
			else
			{
				YS1 = g;
				YS2 = b;
				YS3 = qs;
				YS4 = vt;	
			}
		}
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
		YS4 = 0;
	}
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL vegchtot, YREAL vmr, YREAL gdq, YREAL bdq, YREAL qsdq, YREAL g, YREAL b,YREAL  qs, YREAL vt)
  {
	if(Yt > 1)
	{
		if(Yj == 0)
		{
			if (( vegchtot > 0 ) && (vmr < 0) ) 
			{
				YJ1I3 = 1;
				YJ1I1 =YJ1I2 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 = 0;	

				YJ2I4 = 1;
				YJ2I1 =YJ2I2 =YJ2I3 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 = 0;	

				YJ3I5 = 1;
				YJ3I1 =YJ3I2 =YJ3I4 =YJ3I3 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 = 0;	

				YJ4I2 = -1;
				YJ4I1 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 =YJ4I9 = 0;	
			}
			else
			{
				YJ1I1 =YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 = 0;	
				YJ2I1 =YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 = 0;	
				YJ3I1 =YJ3I2 =YJ3I4 =YJ3I3 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 = 0;	
				YJ4I1 =YJ4I2 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 =YJ4I9 = 0;	
			}
		}
		else
		{
			if (( vegchtot > 0 ) && (vmr < 0) ) 
			{
				YJ1I3 = YJ1I6 =1;
				YJ1I1 =YJ1I2 =YJ1I4 =YJ1I5 =YJ1I7 =YJ1I8 =YJ1I9 = 0;	

				YJ2I4 = YJ2I7 =1;
				YJ2I1 =YJ2I2 =YJ2I3 =YJ2I5 =YJ2I6 =YJ2I8 =YJ2I9 = 0;	

				YJ3I5 = YJ3I8 =1;
				YJ3I1 =YJ3I2 =YJ3I4 =YJ3I3 =YJ3I6 =YJ3I7 =YJ3I9 = 0;	

				YJ4I2 = -1;
				YJ4I9 = 1;	
				YJ4I1 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 = 0;	
			}
			else
			{
				YJ1I6 =1;
				YJ1I1 =YJ1I2 =YJ1I3 = YJ1I4 =YJ1I5 =YJ1I7 =YJ1I8 =YJ1I9 = 0;	

				YJ2I7 =1;
				YJ2I1 =YJ2I2 =YJ2I3 =YJ2I5 =YJ2I6 =YJ2I4 = YJ2I8 =YJ2I9 = 0;	

				YJ3I8 =1;
				YJ3I1 =YJ3I2 =YJ3I4 =YJ3I3 =YJ3I5 = YJ3I6 =YJ3I7 =YJ3I9 = 0;	

				YJ4I9 = 1;	
				YJ4I1 =YJ4I2 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 = 0;	
			}
		}
	}
	else
	{
		YJ1I1 =YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 = 0;	
		YJ2I1 =YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 = 0;	
		YJ3I1 =YJ3I2 =YJ3I4 =YJ3I3 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 = 0;	
		YJ4I1 =YJ4I2 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 =YJ4I9 = 0;	
	}

	
  }

  //===========================================================================

//************************** End of MODULE H_veg_water.h ********************************

