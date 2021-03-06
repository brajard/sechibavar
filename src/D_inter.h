  
/***************************************************************************
                 module  D_inter.h  -  description
                             -------------------
beta coefficient for interception;  interception for each type of vegetation

output---> vbeta2: Beta for interception loss
           vbeta23: partial beta coefficient 


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL qair, YREAL qsatt, YREAL rau, YREAL wind, YREAL q_cdrag, YREAL veget, YREAL qsintveg, YREAL qsintmax)
  {
	double zqsvegrap, ziltest, zrapp;
	double vbeta2 = 0;
	
	if(Yt > 1)
	{
		
		if ((veget > min_sechiba) && (qsintveg >0.001))
		{ 			
			if (qsintmax > min_sechiba ) 
			{
				if(qsintveg / qsintmax > 0.001)  //zqsvegrap = MAX(0, qsintveg / qsintmax);
					zqsvegrap = qsintveg / qsintmax;
				else
					zqsvegrap = 0;					
				
			}
			else
			{
				zqsvegrap = 0;
			}
			 
	
				     
			// Interception loss: IL
			vbeta2 = veget * zqsvegrap  / (1 + wind * q_cdrag * rstruct[Yi][Yj] );
			 

			/* !  -- Limitation of IL by the water stored on the leaf. 
			!     A first approximation of IL is obtained with the old values of
			!     qair and qsol_sat: function of temp-sol and pb. (see call of qsatcalc)*/
			     
			ziltest = dtradia * vbeta2 * wind * q_cdrag * rau * ( qsatt - qair);
			if ( ziltest > 0.0001 )
			{
				zrapp = qsintveg/ ziltest;
				if( zrapp < 1 )
				{
					YS1 = vbeta2 * zrapp;


					if(vbeta2 - vbeta2 * zrapp > 0)  
					{
						YS2 = vbeta2 - vbeta2 * zrapp;
					}
					else
					{
						YS2 = 0;
					}					
					
				}
				else
				{	
					YS1 = vbeta2;
					YS2 = 0;
				}
				
			}
			else
			{
				YS1 = vbeta2;
				YS2 = 0;
			}

		}
		else
		{
			YS1 = 0;
			YS2 = 0;
		}

	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}
	

   }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL qair, YREAL qsatt, YREAL rau, YREAL wind, YREAL q_cdrag, YREAL veget, YREAL qsintveg, YREAL qsintmax)
  {

	double zqsvegrap, ziltest, zrapp;
	double vbeta2;//salidas

	//derivadas
	double d_zqsvegrap_qsintmax=0;
	double d_zqsvegrap_qsintveg=0;

	double d_vbeta2_veget=0;
	double d_vbeta2_zqsvegrap=0;
	double d_vbeta2_wind=0;
	double d_vbeta2_q_cdrag=0;

	double d_ziltest_vbeta2=0;
	double d_ziltest_wind=0;
	double d_ziltest_q_cdrag=0;
	double d_ziltest_rau=0;
	double d_ziltest_qsatt=0;
	double d_ziltest_qair=0;

	double d_zrapp_qsintveg=0;
	double d_zrapp_ziltest=0;

	double d_YS1_vbeta2=0;
	double d_YS1_zrapp=0;

	double d_YS2_vbeta2=0;
	double d_YS2_zrapp=0;

	if(Yt > 1)
	{
		if ((veget > min_sechiba) && (qsintveg >0.001))
		{ 			
			if (qsintmax > min_sechiba ) 
			{
				if(qsintveg / qsintmax > 0.001) 
				{
					zqsvegrap = qsintveg / qsintmax;
					d_zqsvegrap_qsintmax = -qsintveg / pow(qsintmax,2);
					d_zqsvegrap_qsintveg = 1/qsintmax;
				}
				else
				{
					zqsvegrap = 0;					
				}
				
			}
			else
			{
				zqsvegrap = 0;
			}
			 		     
			// Interception loss: IL
			vbeta2 = veget * zqsvegrap  / (1 + wind * q_cdrag * rstruct[Yi][Yj] );
			d_vbeta2_veget = zqsvegrap  / (1 + wind * q_cdrag * rstruct[Yi][Yj] );
			d_vbeta2_zqsvegrap = veget / (1 + wind * q_cdrag * rstruct[Yi][Yj] );
			d_vbeta2_wind = -q_cdrag * rstruct[Yi][Yj] * veget * zqsvegrap / pow(1 + wind * q_cdrag * rstruct[Yi][Yj],2);
			d_vbeta2_q_cdrag = -wind * rstruct[Yi][Yj] * veget * zqsvegrap / pow(1 + wind * q_cdrag * rstruct[Yi][Yj],2);
			 

			/* !  -- Limitation of IL by the water stored on the leaf. 
			!     A first approximation of IL is obtained with the old values of
			!     qair and qsol_sat: function of temp-sol and pb. (see call of qsatcalc)*/
			     
			ziltest = dtradia * vbeta2 * wind * q_cdrag * rau * ( qsatt - qair);
			d_ziltest_vbeta2 = dtradia * wind * q_cdrag * rau * ( qsatt - qair);
			d_ziltest_wind = dtradia * vbeta2 * q_cdrag * rau * ( qsatt - qair);
			d_ziltest_q_cdrag = dtradia * vbeta2 * wind * rau * ( qsatt - qair);
			d_ziltest_rau = dtradia * vbeta2 * wind * q_cdrag * ( qsatt - qair);
			d_ziltest_qsatt = dtradia * vbeta2 * wind * q_cdrag * rau;
			d_ziltest_qair = -dtradia * vbeta2 * wind * q_cdrag * rau;

				//printf("memememe %e\n",ziltest);

			if ( ziltest > 0.001 )
			{
				zrapp = qsintveg/ ziltest;
				d_zrapp_qsintveg = 1/ziltest;
				d_zrapp_ziltest = -qsintveg/ pow(ziltest,2);
				

				if( zrapp < 1 )
				{
					d_YS1_vbeta2 = zrapp;
					d_YS1_zrapp = vbeta2;

					YJ1I1 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_qair;
					YJ1I2 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_qsatt;
					YJ1I3 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_rau;
					YJ1I4 = d_YS1_vbeta2 * d_vbeta2_wind + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_wind + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_wind;
					YJ1I5 = d_YS1_vbeta2*d_vbeta2_q_cdrag + d_YS1_zrapp*d_zrapp_ziltest*d_ziltest_q_cdrag + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_q_cdrag;	
					YJ1I6 = d_YS1_vbeta2 * d_vbeta2_veget + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_veget;
					YJ1I7 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg + d_YS1_zrapp * d_zrapp_qsintveg + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;
					YJ1I8 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintmax + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;



					if(vbeta2 - vbeta2 * zrapp > 0)  //vbeta23 = MAX(vbeta2 - vbeta2 * zrapp, 0);
					{
						d_YS2_vbeta2 = 1 - zrapp;
						d_YS2_zrapp = -vbeta2;

						YJ2I1 = d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_qair;
						YJ2I2 = d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_qsatt;
						YJ2I3 = d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_rau;
						YJ2I4 = d_YS2_vbeta2 * d_vbeta2_wind + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_wind + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_wind;
						YJ2I5 = d_YS2_vbeta2*d_vbeta2_q_cdrag + d_YS2_zrapp*d_zrapp_ziltest*d_ziltest_q_cdrag + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_q_cdrag;	
						YJ2I6 = d_YS2_vbeta2 * d_vbeta2_veget + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_veget;
						YJ2I7 = d_YS2_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg + d_YS2_zrapp * d_zrapp_qsintveg + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;
						YJ2I8 = d_YS2_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintmax + d_YS2_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;
						
					}
					else
					{
						YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 = YJ2I8 =0;
					}					
					
				}
				else
				{

			
					d_YS1_vbeta2 = 1;
					d_YS1_zrapp = 0;

					YJ1I1 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_qair;
					YJ1I2 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_qsatt;
					YJ1I3 = d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_rau;
					YJ1I4 = d_YS1_vbeta2 * d_vbeta2_wind + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_wind + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_wind;
					YJ1I5 = d_YS1_vbeta2 * d_vbeta2_q_cdrag + d_YS1_zrapp*d_zrapp_ziltest*d_ziltest_q_cdrag + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_q_cdrag;	
					YJ1I6 = d_YS1_vbeta2 * d_vbeta2_veget + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_veget;
					YJ1I7 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg + d_YS1_zrapp * d_zrapp_qsintveg + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;
					YJ1I8 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintmax + d_YS1_zrapp * d_zrapp_ziltest * d_ziltest_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;

					YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 = YJ2I8 =0;
				}
				
			}
			else
			{
				d_YS1_vbeta2 = 1;
				d_YS1_zrapp = 0;

				YJ1I1 = YJ1I2 =YJ1I3 =0;
				YJ1I4 = d_YS1_vbeta2 * d_vbeta2_wind;
				YJ1I5 = d_YS1_vbeta2*d_vbeta2_q_cdrag;
				YJ1I6 = d_YS1_vbeta2 * d_vbeta2_veget;
				YJ1I7 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintveg;
				YJ1I8 = d_YS1_vbeta2 * d_vbeta2_zqsvegrap * d_zqsvegrap_qsintmax;

				YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 = YJ2I8 =0;
			}

		}
		else
		{
			YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 = YJ1I8 =0;
			YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 = YJ2I8 =0;

		}

	}
	else
	{
			YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 = YJ1I8 =0;
			YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 = YJ2I8 =0;
	}

  }

  //===========================================================================

//************************** End of MODULE D_inter.h ********************************


