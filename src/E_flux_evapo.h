  
/***************************************************************************
                 module  E_flux_evapo.h  -  description

output
	-evapot: Soil Potential Evaporation

                             -------------------

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL rau,YREAL qc,YREAL qsol_sat_new,YREAL qair)
  {
	if(Yt > 0)
	{
		if(dtradia * rau * qc * (qsol_sat_new - qair) > 0)
		{
			YS1 = dtradia * rau * qc * (qsol_sat_new - qair);
		}
		else
		{
			YS1 = 0;
		}
 		
	}
	else
	{
		YS1 = 0;
	}

	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL rau,YREAL qc,YREAL qsol_sat_new,YREAL qair)
  {
	if(Yt > 0)
	{
		if(dtradia * rau * qc * (qsol_sat_new - qair) > 0)
		{
			YJ1I1 = dtradia * qc * (qsol_sat_new - qair);
			YJ1I2 = dtradia * rau * (qsol_sat_new - qair);
			YJ1I3 = dtradia * rau * qc;    
			YJ1I4 = -dtradia * rau * qc;
		}
		else
		{
			YJ1I1 = 0;
			YJ1I2 = 0;
			YJ1I3 = 0;    
			YJ1I4 = 0;
		}
 		
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;    
		YJ1I4 = 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_flux_evapo.h ********************************














