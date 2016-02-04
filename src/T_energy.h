  
/***************************************************************************
                 module  T_energy.h  -  description

Compute Surface temperature at the beginning of the timestep

Sum up the energy content of all layers in the soil.
                             -------------------

output
	YS1 = surfheat_incr----->Change in soil heat;
	YS2 = coldcont_incr----->Change in snow cold content ;
	YS3= temp_sol_beg----->Surface temperature at the beginning of the timestep;

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol_new, YREAL pcapa_en, YREAL soilcap, YREAL temp_sol_beg1)
  {
	if(Yt == 1)
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = temp_sol_new;
		
	}
	else
	{	
		//Verify the energy conservation in the surface layer
		if(pcapa_en <= sn_capa)
		{
			YS1 = 0;			
			YS2 = soilcap * (temp_sol_new - temp_sol_beg1);			
		}
		else
		{			
			YS1 = soilcap * (temp_sol_new - temp_sol_beg1);
			YS2= 0;
		}	
		
		YS3 = temp_sol_new;
	}
	
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL temp_sol_new, YREAL pcapa_en, YREAL soilcap, YREAL temp_sol_beg1)
  {
	if(Yt == 1)
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
		YJ2I4 = 0;

		YJ3I1 = 1;
		YJ3I2 = 0;
		YJ3I3 = 0;
		YJ3I4 = 0;
	}
	else
	{	
		//Verify the energy conservation in the surface layer
		if(pcapa_en <= sn_capa)
		{
			YJ1I1 = 0;
			YJ1I2 = 0;
			YJ1I3 = 0;
			YJ1I4 = 0;

			YJ2I1 = soilcap;
			YJ2I2 = 0;
			YJ2I3 = temp_sol_new - temp_sol_beg1;
			YJ2I4 = -soilcap;	
		}
		else
		{		
			YJ1I1 = soilcap;
			YJ1I2 = 0;
			YJ1I3 = (temp_sol_new - temp_sol_beg1);
			YJ1I4 = -soilcap;

			YJ2I1 = 0;
			YJ2I2 = 0;
			YJ2I3 = 0;
			YJ2I4 = 0;

		}	
		
		YJ3I1 = 1;
		YJ3I2 = 0;
		YJ3I3 = 0;
		YJ3I4 = 0;
	}
	
	
  }

  //===========================================================================

//************************** MODULE T_energy.h *************************














