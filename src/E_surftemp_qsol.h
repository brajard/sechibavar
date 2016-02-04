  
/***************************************************************************
                    module  E_surftemp_qsol.h  -  description
                             -------------------

output
	YS1 = qsol_sat_new---> New saturated surface air moisture
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL qsol_sat,YREAL pdqsold,YREAL dtheta)
  { 
	
	if(Yt > 1)
		YS1 = qsol_sat + zicp * pdqsold * dtheta;
	else
		YS1 = 0;

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL qsol_sat,YREAL pdqsold,YREAL dtheta)  
  {
	if(Yt > 1)
	{
		YJ1I1 = 1;
		YJ1I2 = zicp * dtheta;
		YJ1I3 = zicp * pdqsold;
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = 0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp_qsol.h ********************************














