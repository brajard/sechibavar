  
/******************************************************************************
                 module classe H_soil_flxcorr.h  -  description

We need a test on the bare soil fraction because we can have bare soil evaporation even when 
there is no bare soil because of transfers (snow for instance). This should only apply if there
is vegetation but we do not test this case.

output:
	YS1 = zeflux_m--->Soil evaporation (modified)
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL veget1, YREAL zeflux, YREAL vevapnu, YREAL vegtot )
  {
	if(Yt > 1)
	{
		//case 1 - there is vegetation and bare soil (nvm = 0)
		if(Yj == 0)
		{
			if((vegtot > 0) && (veget1 > min_sechiba))
				YS1 = vevapnu/veget1;
			else
				YS1 = zeflux;

			
		}
		else
		{	
			//case 2 - there is vegetation but no bare soil (other pft)
			if( (vegtot > 0) && (veget1 <= min_sechiba) && (veget > min_sechiba))
				YS1 =  zeflux + vevapnu/vegtot;
			else
				YS1 = zeflux;
		}
	}
	else
		YS1 = 0;

  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL veget, YREAL veget1, YREAL zeflux, YREAL vevapnu, YREAL vegtot )
  {
	if(Yt > 1)
	{
		//case 1 - there is vegetation and bare soil (nvm = 0)
		if(Yj == 0)
		{
			if((vegtot > 0) && (veget1 > min_sechiba))
			{
				YJ1I1 = YJ1I3 = YJ1I5= 0;
				YJ1I2 = -vevapnu/pow(veget1,2);
				YJ1I4 = 1/veget1;			
				
			}
			else
			{
				YJ1I3 = 1;
				YJ1I1 =YJ1I2 = YJ1I4 =YJ1I5 = 0;
			}
		}
		else
		{	
			//case 2 - there is vegetation but no bare soil (other pft)
			if( (vegtot > 0) && (veget1 <= min_sechiba) && (veget > min_sechiba))
			{

				YJ1I1 =YJ1I2 = 0;
				YJ1I3 = 1;
				YJ1I4 = 1 / vegtot;
				YJ1I5 = -vevapnu/pow(vegtot,2);
				
			}
			else
			{
				YJ1I3 = 1;
				YJ1I1 =YJ1I2 = YJ1I4 =YJ1I5 = 0;
			}
		}
	}
	else
	{
		YJ1I1 =YJ1I2 =YJ1I3 = YJ1I4 =YJ1I5 = 0;
	}
	
  }

  //===========================================================================

//****************** End of MODULE H_soil_flxcorr.h ***************************


