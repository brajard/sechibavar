  
/***************************************************************************
                 module  E_grad_qsat.h  -  description
//

YS1 = grad_qsat;

computed with function dev_qsatcalc (sechiba.h)

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL tair, YREAL pb)
  {
	if(Yt > 1)

		YS1= dev_qsatcalc(tair, pb);	
	else
		YS1 = 0;

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL tair, YREAL pb)
  {
	if(Yt > 1)
	{
		YJ1I1 = d_dev_qsatcalc_tempsol(tair, pb);
		YJ1I2 = d_dev_qsatcalc_pb(tair, pb);
	}
	else
	{
		YJ1I1 = YJ1I2 = 0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE E_grad_qsat.h ********************************














