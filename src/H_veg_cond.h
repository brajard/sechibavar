  
/***************************************************************************
                 module classe H_veg_cond.h  -  description

where vegetation fraction is zero, set water to that of bare soil
This does not create any additional water.
	----> veget < EPS1 

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL gqsb, YREAL bqsb, YREAL  dsg, YREAL dss, YREAL dsp, YREAL gqsb1, YREAL bqsb1, YREAL dsg1, YREAL dss1, YREAL dsp1, YREAL veget)  
  {
	if(Yt > 1)
	{	

		if ( veget < epsilon(1) )
		{
			YS1 = gqsb1;
			YS2 = bqsb1;
			YS3 = dsg1;
			YS4 = dss1;
			YS5 = dsp1;
		}
		else
		{
			YS1 = gqsb;
			YS2 = bqsb;
			YS3 = dsg;
			YS4 = dss;
			YS5 = dsp;
		}
	}
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL gqsb, YREAL bqsb, YREAL  dsg, YREAL dss, YREAL dsp, YREAL gqsb1, YREAL bqsb1, YREAL dsg1, YREAL dss1, YREAL dsp1, YREAL veget)  
  {
	if(Yt > 1)
	{	

		if ( veget < epsilon(1) )
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I7 = YJ1I8 = YJ1I9 = YJ1I10 = YJ1I11 = 0;
			YJ1I6 = 1 ; 

			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = YJ2I8 = YJ2I9 = YJ2I10 = YJ2I11 = 0;
			YJ2I7 = 1 ; 

			YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = YJ3I7 = YJ3I9 = YJ3I10 = YJ3I11 = 0;
			YJ3I8 = 1 ; 

			YJ4I1 = YJ4I2 = YJ4I3 = YJ4I4 = YJ4I5 = YJ4I6 = YJ4I7 = YJ4I8 =  YJ4I10 = YJ4I11 = 0;
			YJ4I9 = 1 ; 
			
			YJ5I1 = YJ5I2 = YJ5I3 = YJ5I4 = YJ5I5 = YJ5I6 = YJ5I7 = YJ5I8 = YJ5I9 =  YJ5I11 = 0;
			YJ5I10 = 1 ; 
		}
		else
		{
			YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 = YJ1I8 = YJ1I9 = YJ1I10 = YJ1I11 = 0;
			YJ1I1 = 1 ; 

			YJ2I1 =  YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = YJ2I7 =YJ2I8 = YJ2I9 = YJ2I10 = YJ2I11 = 0;
			YJ2I2 = 1 ; 

			YJ3I1 = YJ3I2 = YJ3I4 = YJ3I5 = YJ3I6 = YJ3I7 = YJ3I8 = YJ3I9 = YJ3I10 = YJ3I11 = 0;
			YJ3I3 = 1 ; 

			YJ4I1 = YJ4I2 = YJ4I3 =  YJ4I5 = YJ4I6 = YJ4I7 = YJ4I8 = YJ4I9 = YJ4I10 = YJ4I11 = 0;
			YJ4I4 = 1 ; 
			
			YJ5I1 = YJ5I2 = YJ5I3 = YJ5I4 =  YJ5I6 = YJ5I7 = YJ5I8 = YJ5I9 = YJ5I10 = YJ5I11 = 0;
			YJ5I5 = 1 ; 
		}
	}
	
	
  }

  //===========================================================================

//************************** End of FIN DU MODULE H_veg_cond.h ********************************














