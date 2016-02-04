  
/***************************************************************************
                 module  SE_rau.h  -  description

rau: air density--->YS1 = rau;
las module to be computed

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_air, YREAL pb)
  {
	if(Yt == 1)
		YS1 = 0;
	else
		YS1 = pa_par_hpa * pb / (cte_molr*temp_air);

  }	

  //===========================================================================
  //                           backward
  backward(YREAL temp_air, YREAL pb)
  {
	if(Yt == 1)
		YJ1I1 = YJ1I2 =0;
	else
	{
		YJ1I2 = pa_par_hpa / (cte_molr*temp_air);
		YJ1I1 = -cte_molr*pa_par_hpa * pb / pow(cte_molr*temp_air,2);	
	}	
  }

  //===========================================================================

//************************** MODULE SE_rau.h  **************************



