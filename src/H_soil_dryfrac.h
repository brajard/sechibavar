/***************************************************************************
                 module  H_soil_dryfrac.h  -  description

outputs:
	YS1 = drysoil_frac-->The fraction of visibly dry soil (dry when dss = 0.1 m);
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL dss)
  {
	double temp = 0;

	
	if(dss > min_sechiba)
		temp = 10 * dss;

	if(temp < 1)
		YS1 = temp;
	else
		YS1 = 1;	
  }

  //===========================================================================
  //                           backward
  backward (YREAL dss)
  {
	double temp = 0;
	double d_temp_dss = 0;
	
	if(dss > min_sechiba)
	{
		d_temp_dss = 10;
		temp = 10 * dss;
	}

	if(temp < 1)
		YJ1I1 = d_temp_dss;
	else
		YJ1I1 = 0;
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_dryfrac.h *********************


