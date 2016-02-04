  
/***************************************************************************
                 module  H_snow_age.h  -  description
                             -------------------

Description: computes snow age on veg and ice (for albedo)
		Age of snow on ice: a little bit different because in cold regions, we really
		cannot negect the effect of cold temperatures on snow metamorphism any more.

output: YS1 = snow_age
	YS2 = snow_nobio_age
	Ys3 = snowdepth

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL snow, YREAL snow_nobio, YREAL precip_snow, YREAL snow_age1, YREAL snow_nobio_age1, YREAL temp_sol_new)
  {
	double d_age;
	double temp = 0;
	double xx;


	if(Yt == 1)	
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
	}
	else
	{
		//1
		if(snow<0)
		{
			YS1 = 0;
		}
		else
		{
			YS1 = (snow_age1 + (1 - snow_age1/max_snow_age) * (dtradia) /one_day) * exp(-precip_snow / snow_trans);
		}
	      
		//2
		//computes snow_nobio_age
		if(snow_nobio < 0 ) 
		{
			YS2= 0;
		}
		else
		{
			temp = ( snow_nobio_age1 + (1 - snow_nobio_age1/max_snow_age) * (dtradia)/one_day ) * exp(-precip_snow / snow_trans) - snow_nobio_age1;
			//3			
			if(temp >0 ) 
			{
				//4
				if(tp_00 - temp_sol_new > 0)
				{
					xx = tp_00 - temp_sol_new;
				}
				else
				{
					xx = 0;
				}
			
				d_age = temp / (1 + pow(xx/7,4) ); //modificado por mi

			}
			else
			{
				d_age = temp;
			}

			//5
			if(snow_nobio_age1 + d_age > 0)
			{
				YS2 = snow_nobio_age1 + d_age;	
			}
			else 
			{
				YS2 = 0;
			}
		}
	      

		//snowdepth: Diagnose the depth of the snow layer
		YS3 = snow /sn_dens;
	}	
	
  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL snow, YREAL snow_nobio, YREAL precip_snow, YREAL snow_age1, YREAL snow_nobio_age1, YREAL temp_sol_new )
  {

	double d_age;
	double temp = 0;
	double xx;

	//derivadas
	double d_age_temp;
	double d_age_xx;

	double d_temp_snow_nobio_age1;
	double d_temp_precip_snow;

	double d_xx_tempsol;
	
	double d_YS2_age;
	
	if(Yt == 1)	
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = 0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = 0;

	}
	else
	{
		if(snow <0)
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = 0;
			
		}
		else
		{
			YJ1I1 = 0;
			YJ1I2 = 0;
			YJ1I3 = (snow_age1 + (1 - snow_age1/max_snow_age) * (dtradia) /one_day) * exp(-precip_snow / snow_trans) * (-1 / snow_trans);
			YJ1I4 = exp(-precip_snow / snow_trans) - (dtradia /one_day) * (1/max_snow_age);
			YJ1I5 = 0;
			YJ1I6 = 0;
		}
	      

		//computes snow_nobio_age
		if(snow_nobio < 0 ) 
		{
			YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = 0;
		}
		else
		{
			temp = ( snow_nobio_age1 + (1 - snow_nobio_age1/max_snow_age) * (dtradia)/one_day ) * exp(-precip_snow / snow_trans) - snow_nobio_age1;
			d_temp_snow_nobio_age1 = exp(-precip_snow / snow_trans) - (dtradia /one_day) * (1/max_snow_age) - 1;
			d_temp_precip_snow = (snow_age1 + (1 - snow_age1/max_snow_age) * (dtradia) /one_day) * exp(-precip_snow / snow_trans) * (-1 / snow_trans);

			
			if(temp >0 ) 
			{
				//xx = MAX(tp_00 - temp_sol_new, 0);
				if(tp_00 - temp_sol_new > 0)
				{
					xx = tp_00 - temp_sol_new;
					d_xx_tempsol = -1;
				}
				else
				{
					xx = 0;
					d_xx_tempsol = 0;
				}
			
				d_age = temp / (1 + pow(xx/7,4) ); //modificado por mi
				d_age_temp= 1 / (1 + pow(xx/7,4) );
				d_age_xx = 4* temp * pow(xx,3) * pow(1/7,4)    / pow(1 + pow(xx/7,4),2 );
				
			}
			else
			{
				d_age = temp;
				d_age_temp= 1;
				d_age_xx = 0;
			}


			//YS2 = MAX( snow_nobio_age1 + d_age, 0 );
			if(snow_nobio_age1 + d_age > 0)
			{
				d_YS2_age = 1;

				YJ2I1 = 0;
				YJ2I2 = 0;
				YJ2I3 = d_YS2_age * d_age_temp * d_temp_precip_snow;
				YJ2I4 = 0;
				YJ2I5 = 1 + d_YS2_age * d_age_temp * d_temp_snow_nobio_age1;
				YJ2I6 = d_YS2_age * d_age_xx * d_xx_tempsol;
			}
			else 
			{
				YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = 0;
			}
		}
	      

		//snowdepth: Diagnose the depth of the snow layer
		YJ3I1 = 1/sn_dens;
		YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = 0;
	}

	
  }

  //===========================================================================

//************************** End of MODULE H_snow_age.h ********************************














