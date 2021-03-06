  
/***************************************************************************
                 module classe E_beg_qsol.h  -  description


-psold:		Old surface dry static energy for the old timestep
		We here define the surface static energy for the 'old' timestep, in terms of the surface
		temperature and heat capacity.
-qsol_sat: 	Derivative of satured specific humidity at the old temperature


outputs
	YS1 = psold   ----> Old surface dry static energy;
	YS2 = qsol_sat----> Saturated specific humudity for old temperature ;
	YS3 = pdqsold ----> Derivative of satured specific humidity at the old temperature;

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol, YREAL pb, YREAL qsatt, YREAL dev_qsol)
  {
	if(Yt > 1)
	{
		//1. computes psold
		YS1 = temp_sol*cp_air;

		//2. computes qsol_sat
		YS2 = qsatt;

		//3. computes pdqsold
		YS3 = dev_qsol * pow(pb,kappa)/ cp_air;
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
  backward (YREAL temp_sol, YREAL pb, YREAL qsatt, YREAL dev_qsol)  
  {
	if(Yt > 1)
	{
		//1. computes psold
		YJ1I1 = cp_air;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;

		//2. computes qsol_sat
		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 1;
		YJ2I4 = 0;

		//3. computes pdqsold
		YJ3I1 = 0;
		YJ3I2 = kappa * dev_qsol * pow(pb,kappa-1)/ cp_air;
		YJ3I3 = 0;
		YJ3I4 = pow(pb,kappa)/ cp_air;

	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
		YJ2I4 = 0;

		YJ3I1 = 0;
		YJ3I2 = 0;
		YJ3I3 = 0;
		YJ3I4 = 0;
	}

	
	
  }

  //===========================================================================

//************************** End of MODULE E_beg_qsol.h ********************************

