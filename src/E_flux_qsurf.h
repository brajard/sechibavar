  
/***************************************************************************
                 module  E_flux_qsurf.h  -  description
                             -------------------
output: qsurf----> Surface specific humidity


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vbeta1,YREAL qsol_sat_new,YREAL vbeta,YREAL valpha,YREAL qair ,YREAL qair_i)
  {
	if(Yt == 1)
		YS1 = qair_i;
	else
	{	
		if( vbeta1 * qsol_sat_new + (1 - vbeta1) * vbeta * valpha * qsol_sat_new > qair)
			YS1 =  vbeta1 * qsol_sat_new + (1 - vbeta1) * vbeta * valpha * qsol_sat_new;
		else
			YS1 = qair;

	}

	
  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL vbeta1,YREAL qsol_sat_new,YREAL vbeta,YREAL valpha,YREAL qair,YREAL qair_i)
  {      
	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I14= YJ1I5 = 0;
		YJ1I6 = 1;
	}
	else
	{	
		if( vbeta1 * qsol_sat_new + (1 - vbeta1) * vbeta * valpha * qsol_sat_new > qair)
		{
			

			YJ1I1 = qsol_sat_new - vbeta * valpha * qsol_sat_new;
			YJ1I2 = vbeta1 + (1 - vbeta1) * vbeta * valpha;
			YJ1I3 = (1 - vbeta1) * valpha * qsol_sat_new;        
			YJ1I4=  (1 - vbeta1) * vbeta * qsol_sat_new;
			YJ1I5 = YJ1I6 =0;
		}
		else
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4= YJ1I6 =  0;
			YJ1I5 = 1;
		}

	}
	
  }

  //===========================================================================

//************************** End of MODULE E_flux__qsurf.h ********************************














