  
/***************************************************************************
                 module  D_qsatt.h  -  description

output : qsatt : An estimation of the satturated humidity at the surface

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol, YREAL pb)
  {	
	if(Yt > 1)
	{
		YS1 = qsatcalc(temp_sol, pb);
	}
	else
		YS1 = 0;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL temp_sol, YREAL pb)
  {
	if(Yt > 1)
	{
		YJ1I1= d_qsatcalc_temp(temp_sol, pb);
		YJ1I2= d_qsatcalc_pres_in(temp_sol, pb);
	}
	else
	{
		YJ1I1=0;
		YJ1I2= 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE D_qsatt.h ********************************














