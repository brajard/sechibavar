  
/***************************************************************************
                    module  E_surftemp_qair.h  -  description
                             -------------------

output: qair_new ---- > New air moisture

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL zikq,YREAL qair,YREAL lareva_old,YREAL larsub_old,YREAL lareva_sns,YREAL  larsub_sns,YREAL dtheta,YREAL vbeta1,YREAL vbeta,YREAL valpha,YREAL qsol_sat_new)
  {
	double fevap;		

	if(Yt > 1)
	{
		fevap = (lareva_old - lareva_sns * dtheta) + (larsub_old - larsub_sns * dtheta);

		if(abs(fevap) < epsilon(1))
			YS1 = qair;	

		else
			YS1 = (zikq * 1 / (chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1)) * fevap + qsol_sat_new;
	}
	else
		YS1 = 0;

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL zikq,YREAL qair,YREAL lareva_old,YREAL larsub_old,YREAL lareva_sns,YREAL  larsub_sns,YREAL dtheta,YREAL vbeta1,YREAL vbeta,YREAL valpha,YREAL qsol_sat_new) 
  {
	double fevap;	

	double d_fevap_lareva_old;
	double d_fevap_larsub_old;
	double d_fevap_lareva_sns;
	double d_fevap_larsub_sns;
	double d_fevap_dtheta;

	double d_YS1_fevap;


	if(Yt > 1)
	{
		fevap = (lareva_old - lareva_sns * dtheta) + (larsub_old - larsub_sns * dtheta);

		d_fevap_lareva_old = 1;
		d_fevap_larsub_old = 1;
		d_fevap_lareva_sns = -dtheta;
		d_fevap_larsub_sns = -dtheta;
		d_fevap_dtheta = - lareva_sns- larsub_sns ;

		d_YS1_fevap = (zikq * 1 / (chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1)) ; 
		

		if(abs(fevap) < epsilon(1))
		{
			YJ1I1 = 0;
			YJ1I2 = 1;
			YJ1I3 = 0;
			YJ1I4 = 0;
			YJ1I5 = 0;
			YJ1I6 = 0;
			YJ1I7 = 0;
			YJ1I8 = 0;
			YJ1I9 = 0;
			YJ1I10 = 0;	
			YJ1I11 = 0;

		}
		else
		{
			YJ1I1 = 1 / (chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1) * fevap;
			YJ1I2 = 0;
			YJ1I3 = d_YS1_fevap * d_fevap_lareva_old;
			YJ1I4 = d_YS1_fevap * d_fevap_larsub_old;
			YJ1I5 = d_YS1_fevap * d_fevap_lareva_sns;
			YJ1I6 = d_YS1_fevap * d_fevap_larsub_sns;
			YJ1I7 = d_YS1_fevap * d_fevap_dtheta;
			YJ1I8 = -(zikq * (- chalev0 * vbeta * valpha + chalsu0 ) * fevap) / pow( chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1 , 2);
			YJ1I9 = -(zikq * chalev0 * (1 - vbeta1) * valpha * fevap)  / pow( chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1  , 2);
			YJ1I10 = -(zikq * chalev0 * (1 - vbeta1) * vbeta * fevap)  / pow( chalev0 * (1 - vbeta1) * vbeta * valpha + chalsu0 *  vbeta1  , 2);	
			YJ1I11 = 1
;

		}


	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 1;
		YJ1I3 = 0;
		YJ1I4 = 0;
		YJ1I5 = 0;
		YJ1I6 = 0;
		YJ1I7 = 0;
		YJ1I8 = 0;
		YJ1I9 = 0;
		YJ1I10 = 0;	
		YJ1I11 = 0;	
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp_qair.h ********************************














