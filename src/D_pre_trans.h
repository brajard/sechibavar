  
/***************************************************************************
                 module  D__pre_trans.h  -  description
                             -------------------
output: Variables temporelles pour le calculo de vbeta3 : resistance stomatique

YS1 = zdefconc   --> Moisture concentration at the leaf level.    
YS2 = zqsvegrap  --> Rate between the cAnopy intercepted water and max water quantity 

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_air, YREAL pb, YREAL qair, YREAL rau, YREAL qsintveg, YREAL qsintmax)
  {
	double qsatt_a; //Moisture concentration at the leaf level.

	
	if((Yt > 1) && (Yj > 0))
	{
		//1.  Moisture concentration at the leaf level.
		qsatt_a = qsatcalc(temp_air, pb);

		if(qsatt_a - qair > 0)
			YS1 = rau * (qsatt_a - qair);
		else
			YS1 = 0;

		//2. beta coefficient for vegetation transpiration	 
		if((qsintmax > min_sechiba) && ( (qsintveg / qsintmax) > 0 ))
		{

			YS2 = qsintveg / qsintmax;

		}
		else
			YS2 = 0;
		  
		
		
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}


  }
// [ ]	

  //===========================================================================
  //                           backward
  backward (YREAL temp_air, YREAL pb, YREAL qair, YREAL rau, YREAL qsintveg, YREAL qsintmax)
  {
	double qsatt_a; //Moisture concentration at the leaf level.

	double d_qsatt_tempair = 0;
	double d_qsatt_pb = 0;
	
	double d_YS1_qsatt = 0;
	

	
	if((Yt > 1) && (Yj > 0))
	{
		//1.  Moisture concentration at the leaf level.
		qsatt_a = qsatcalc(temp_air, pb);

		d_qsatt_tempair = d_qsatcalc_temp(temp_air, pb);
		d_qsatt_pb = d_qsatcalc_pres_in(temp_air, pb);

		if(qsatt_a - qair > 0)
		{
			d_YS1_qsatt = rau;
			YJ1I1 = d_YS1_qsatt * d_qsatt_tempair;
			YJ1I2 = d_YS1_qsatt * d_qsatt_pb;
			YJ1I3 = -rau;
			YJ1I4 = (qsatt_a - qair);
			YJ1I5 = YJ1I6 = 0;
		}
		else
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 =YJ1I5 = YJ1I6 = 0;


		//2. beta coefficient for vegetation transpiration	 
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = 0;

		if((qsintmax > min_sechiba) && ( (qsintveg / qsintmax) > 0 ))
		{
	
			//YS2 = qsintveg / qsintmax;
			YJ2I5 = 1 / qsintmax;
			YJ2I6 = -qsintveg / pow(qsintmax,2);

		}
		else
			YJ2I5 = YJ2I6 = 0;
		  
		
		
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 =YJ1I5 = YJ1I6 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =YJ2I5 = YJ2I6 =0;
	}

	
		
  }

  //===========================================================================

//************************** End of MODULE D_trans.h ********************************

