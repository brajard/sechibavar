  
/***************************************************************************
                 module  H_soil_diff.h  -  description

Diffusion of water between the reservoirs of the different plants

output: 
	YS1 = mean_bqsb: deep reservoir
	YS2 = mean_gqsb: surface reservoir
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL gqsb,YREAL bqsb, YREAL veget, YREAL  mean_bqsb1, YREAL mean_gqsb1, YREAL vegtot, YREAL resdist)
  {

	if(Yt == 1)
	{

		if(Yj == 0)
		{
			YS1 =  resdist*bqsb;
			YS2 =  resdist*gqsb;
		}
		else
		{
			YS1 =  mean_bqsb1 + resdist*bqsb;
			YS2 =  mean_gqsb1 + resdist*gqsb;
		}
	}
	else
	{
		
		if(vegtot > 0)
		{
			if(Yj == 0)
			{
				YS1 =  bqsb * veget/vegtot;
				YS2 =  gqsb * veget/vegtot;
			}
			else
			{
				YS1 =  mean_bqsb1 + bqsb * veget/vegtot;
				YS2 =  mean_gqsb1 + gqsb * veget/vegtot;
			}
		}
		else
		{
			YS1 = 0;
			YS2 = 0;
		}
	}	
  }
	

  //===========================================================================
  //                           backward
  backward(YREAL gqsb,YREAL bqsb, YREAL veget, YREAL  mean_bqsb1, YREAL mean_gqsb1, YREAL vegtot, YREAL resdist)
  {
	if(Yt == 1)
	{

		if(Yj == 0)
		{
			YJ1I2 = resdist;
			YJ1I7 = bqsb;
			YJ1I1 =  YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = 0;

			YJ2I1 =resdist;
			YJ2I7 =gqsb;
			YJ2I2 = YJ2I3 = YJ2I4 =  YJ2I5 = YJ2I6 = 0; 
		}
		else
		{
			YJ1I2 = resdist;
			YJ1I4 = 1 ;
			YJ1I7 = bqsb;
			YJ1I1 =  YJ1I3 =  YJ1I5 = YJ1I6 = 0;

			YJ2I1 =resdist;
			YJ2I5 =1;
			YJ2I7 =gqsb;
			YJ2I2 = YJ2I3 = YJ2I4 =   YJ2I6 = 0; 
		}
	}
	else
	{
		
		if(vegtot > 0)
		{
			if(Yj == 0)
			{
				YJ1I2 = veget/vegtot;
				YJ1I3 = bqsb/vegtot;
				YJ1I6 = -veget*bqsb/pow(vegtot,2);
				YJ1I1 =   YJ1I4 = YJ1I5 =  YJ1I7 = 0;

				YJ2I1 = veget/vegtot;
				YJ2I3 = gqsb/vegtot;
				YJ2I6 =-veget*gqsb/pow(vegtot,2);
				YJ2I2 = YJ2I4 =  YJ2I5 = YJ2I7 =0; 				

			}
			else
			{
				YJ1I2 = veget/vegtot;
				YJ1I3 = bqsb/vegtot;
				YJ1I4 = 1;
				YJ1I6 = -veget*bqsb/pow(vegtot,2);
				YJ1I1 =  YJ1I5 =  YJ1I7 = 0;

				YJ2I1 = veget/vegtot;
				YJ2I3 = gqsb/vegtot;
				YJ2I5 = 1;
				YJ2I6 =-veget*gqsb/pow(vegtot,2);				
				YJ2I2 = YJ2I4 = YJ2I7 =0; 	
			}
		}
		else
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 =0;
			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =  YJ2I5 = YJ2I6 = YJ1I7 =0; 
		}
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_diff.h ********************************


