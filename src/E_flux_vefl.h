  
/***************************************************************************
                 module  E_flux_vefl.h  -  description
                             -------------------
output:
	-YS1 = vevapp : Total of evaporation
	-YS2 = fluxlat : Latent heat flux
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL rau,YREAL qc,YREAL vbeta1,YREAL qsol_sat_new,YREAL qair,YREAL vbeta,YREAL valpha)
  {
	if(Yt == 1)
	{
		YS1 = 0;
		YS2 = 0;
		
	}
	else
	{
		YS1 = dtradia * rau * qc * vbeta1 * (qsol_sat_new - qair) + dtradia * rau * qc * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);
		YS2 = chalsu0 * rau * qc * vbeta1 * (qsol_sat_new - qair) +  chalev0 * rau * qc * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);		
	}


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL rau,YREAL qc,YREAL vbeta1,YREAL qsol_sat_new,YREAL qair,YREAL vbeta,YREAL valpha)
  {
	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4= YJ1I5 = YJ1I6= YJ1I7 =  0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4= YJ2I5 = YJ2I6= YJ2I7 =  0;
		
	}
	else
	{
		YJ1I1 = dtradia *  qc * vbeta1 * (qsol_sat_new - qair) + dtradia * qc * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);
		YJ1I2 = dtradia * rau * vbeta1 * (qsol_sat_new - qair) + dtradia * rau * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);
		YJ1I3 = dtradia * rau * qc * (qsol_sat_new - qair) - dtradia * rau * qc * vbeta * (valpha * qsol_sat_new - qair);
		YJ1I4 = dtradia * rau * qc * vbeta1 + dtradia * rau * qc * (1 - vbeta1) * vbeta * valpha ;
		YJ1I5 = -dtradia * rau * qc * vbeta1 - dtradia * rau * qc * (1 - vbeta1) * vbeta ;
		YJ1I6 = dtradia * rau * qc * (1 - vbeta1) * (valpha * qsol_sat_new - qair);
		YJ1I7 = dtradia * rau * qc * (1 - vbeta1) * vbeta * qsol_sat_new;

		YJ2I1 = chalsu0 * qc * vbeta1 * (qsol_sat_new - qair) +  chalev0 * qc * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);
		YJ2I2 = chalsu0 * rau * vbeta1 * (qsol_sat_new - qair) +  chalev0 * rau * (1 - vbeta1) * vbeta * (valpha * qsol_sat_new - qair);
		YJ2I3 = chalsu0 * rau * qc * (qsol_sat_new - qair) -  chalev0 * rau * qc * vbeta * (valpha * qsol_sat_new - qair);
		YJ2I4 = chalsu0 * rau * qc * vbeta1 +  chalev0 * rau * qc * (1 - vbeta1) * vbeta * valpha ;
		YJ2I5 = -chalsu0 * rau * qc * vbeta1 -  chalev0 * rau * qc * (1 - vbeta1) * vbeta ;
		YJ2I6 = chalev0 * rau * qc * (1 - vbeta1) * (valpha * qsol_sat_new - qair);
		YJ2I7 = chalev0 * rau * qc * (1 - vbeta1) * vbeta * qsol_sat_new ;
	}

	
  }

  //===========================================================================

//************************** End of MODULE E_flux_vefl.h ********************************














