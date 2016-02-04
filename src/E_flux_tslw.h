  
/***************************************************************************
                 module  E_flux_tslw.h  -  description
                             -------------------
We first of all calculate the radiation as a result of the Stefan-Boltzmann equation,
which is the sum of the calculated values at the surface temperature  at the 'old' 
temperature and the value that corresponds to the difference between the old temperature
and the temperature at the 'new' timestep.

Computes:
	YS1 = lwup : Long-wave up radiation (W/m^2)
	YS2 = tsol_rad: Radiative surface temperature
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL emis,YREAL temp_sol,YREAL temp_sol_new,YREAL lwdown)
  {
	double lwup;

	if(Yt == 1)
	{
		YS1 = 0;
		YS2 = temp_sol_new;
	}
	else
	{
		lwup = emis * c_stefan * pow(temp_sol,4) +  4 * emis * c_stefan * pow(temp_sol,3) * (temp_sol_new - temp_sol) +  (1 - emis) * lwdown;

		YS1 = lwup;
		YS2 = emis * c_stefan * pow(temp_sol,4) + lwup;
	}


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL emis,YREAL temp_sol,YREAL temp_sol_new,YREAL lwdown)
  {
	
	double d_lwup_emis;
	double d_lwup_temp_sol;
	double d_lwup_temp_sol_new;
	double d_lwup_lwdown;

	double d_YS1_lwup;
	double d_YS2_lwup;

	if(Yt == 1)
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 1;
		YJ2I4 = 0;
	}
	else
	{
		

		d_lwup_emis = c_stefan * pow(temp_sol,4)  + 4 * c_stefan * pow(temp_sol,3) * (temp_sol_new - temp_sol) - lwdown ;
		d_lwup_temp_sol =  4 * emis * c_stefan * pow(temp_sol,3) + 4 * emis * c_stefan * ( 3*pow(temp_sol,2) * temp_sol_new - 4 * pow(temp_sol,3));

		
		d_lwup_temp_sol_new = 4 * emis * c_stefan * pow(temp_sol,3);
		d_lwup_lwdown = (1 - emis);

		d_YS1_lwup = 1;
		d_YS2_lwup = 1;


		YJ1I1 = d_YS1_lwup * d_lwup_emis;
		YJ1I2 = d_YS1_lwup * d_lwup_temp_sol;
		YJ1I3 = d_YS1_lwup * d_lwup_temp_sol_new;
		YJ1I4 = d_YS1_lwup * d_lwup_lwdown;

		YJ2I1 = c_stefan * pow(temp_sol,4) + d_YS2_lwup * d_lwup_emis;
		YJ2I2 = 4 * emis * c_stefan * pow(temp_sol,3) + d_YS2_lwup * d_lwup_temp_sol ;
		YJ2I3 = d_YS2_lwup * d_lwup_temp_sol_new;
		YJ2I4 = d_YS2_lwup * d_lwup_lwdown;

	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_flux_calcul.h ********************************














