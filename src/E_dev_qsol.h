  
/***************************************************************************
                 module classe E_dev_qsol.h  -  description

1. computes qsat interpolation into two successive temperature

output : dev_qsol
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol, YREAL pb)
  {

	if(Yt > 1)
		YS1 = dev_qsatcalc(temp_sol, pb);
	else
		YS1 = 0;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL temp_sol, YREAL pb)
  {
	if(Yt > 1)
	{
		YJ1I1 = d_dev_qsatcalc_tempsol(temp_sol, pb);
		YJ1I2 = d_dev_qsatcalc_pb(temp_sol, pb);
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
	}

	
	
  }

  //===========================================================================

//************************** End of MODULE E_dev_qsol.h ********************************



