  
/***************************************************************************
                    module  E_surftemp2.h  -  description
                             -------------------
 Compute the fluxes for the old surface conditions, As a reference, we first 
calculate the sensible and latent heat for the 'old' timestep.
 The equation here is in terms of surface static energy


outputs: 
	YS1 = sensfl_old ---> sensible heat for the old time step
	YS2 = larsub_old --->  Latent heat for the 'old' timestep (sublimation process)
	YS3= lareva_old--->  Latent heat for the 'old' timestep (evaporation process)
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL petBcoef,YREAL psold, YREAL zikt, YREAL petAcoef, YREAL vbeta1, YREAL qsol_sat, YREAL vbeta, YREAL  valpha, YREAL peqBcoef, YREAL peqAcoef)
  {
	if (Yt > 1)
	{
		YS1 = (petBcoef -  psold) / (zikt -  petAcoef);
		YS2 = chalsu0 * vbeta1 * (peqBcoef -  qsol_sat) / (zikt - peqAcoef);
		YS3 = chalev0 * (1 - vbeta1) * vbeta * (peqBcoef -  valpha * qsol_sat) / (zikt - peqAcoef);
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
  backward (YREAL petBcoef,YREAL psold, YREAL zikt, YREAL petAcoef, YREAL vbeta1, YREAL qsol_sat, YREAL vbeta, YREAL  valpha, YREAL peqBcoef, YREAL peqAcoef)
  {        
	if (Yt > 1)
	{
		//derivees de sensfl_old 
		YJ1I1 = 1 / (zikt -  petAcoef);
		YJ1I2 = -1 / (zikt -  petAcoef);
		YJ1I3 = -(petBcoef -  psold) / pow(zikt -  petAcoef , 2);
		YJ1I4 = (petBcoef -  psold) / pow(zikt -  petAcoef , 2);
		YJ1I5 = YJ1I6 =YJ1I7 = YJ1I8 =YJ1I9 = YJ1I10 = 0;

		//derivees de larsub_old
		YJ2I3 = -chalsu0 * vbeta1 * (peqBcoef -  qsol_sat) / pow(zikt - peqAcoef , 2);
		YJ2I5 = chalsu0 * (peqBcoef -  qsol_sat) / (zikt - peqAcoef);
		YJ2I6 = -chalsu0 * vbeta1 / (zikt - peqAcoef);
		YJ2I9 = chalsu0 * vbeta1 / (zikt - peqAcoef);
		YJ2I10 = chalsu0 * vbeta1 * (peqBcoef -  qsol_sat) / pow(zikt - peqAcoef, 2);
		YJ2I1 = YJ2I2 = YJ2I4 = YJ2I7 = YJ2I8 = 0;

		//derivees de lareva_old
		YJ3I1 = 0;
		YJ3I2 = 0;
		YJ3I3 = -chalev0 * (1 - vbeta1) * vbeta * (peqBcoef -  valpha * qsol_sat) / pow(zikt - peqAcoef , 2);
		YJ3I4 = 0;
		YJ3I5 = -chalev0 * vbeta * (peqBcoef -  valpha * qsol_sat) / (zikt - peqAcoef);
		YJ3I6 = -chalev0 * (1 - vbeta1) * vbeta *  valpha  / (zikt - peqAcoef);
		YJ3I7 = chalev0 * (1 - vbeta1) * (peqBcoef -  valpha * qsol_sat) / (zikt - peqAcoef);
		YJ3I8 = -chalev0 * ((1 - vbeta1) * vbeta * qsol_sat) / (zikt - peqAcoef);
		YJ3I9 = chalev0 * (1 - vbeta1) * vbeta   / (zikt - peqAcoef);
		YJ3I10 = chalev0 * (1 - vbeta1) * vbeta * (peqBcoef -  valpha * qsol_sat) / pow(zikt - peqAcoef , 2);

	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 = YJ1I8 = YJ1I9 = YJ1I10 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = YJ2I7 = YJ2I8 = YJ2I9 = YJ1I10 = 0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = YJ3I7 = YJ3I8 = YJ3I9 = YJ1I10 = 0;

	}

	
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp2.h ********************************














