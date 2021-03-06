  
/******************************************************************************
                 module  H_soil_verif.h  -  description



output:
	YS1 = runoff;
	YS2 = gqsb;
	YS3 = dsg;
	YS4 = bqsb;
	YS5 = dsp;
	YS6 = dss;
	YS7 = marcador
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward

  forward (YREAL runoff1, YREAL gqsb1, YREAL  dsg1, YREAL bqsb1, YREAL dsp1, YREAL  dss1, YREAL veget, YREAL ruu_ch, YREAL mx_eau_var, YREAL mx_eau_eau1, YREAL dpu_cste,YREAL bqsb_c, YREAL gqsb_c, YREAL bqsb_init, YREAL gqsb_init )
  {	
	double temp1 = 0;
	double tempYS4 = 0;
	

	if(Yt > 1)
	{
		if((dsp1 < dsg1) && (dsg1 > min_sechiba) && (veget > min_sechiba) )
		{
			
			//pour YS1
			if(bqsb1 + gqsb1 - mx_eau_var > min_sechiba)
				temp1 =  bqsb1 + gqsb1 - mx_eau_var;

			//sortie YS4
			tempYS4 = mx_eau_var;
			if(bqsb1 + gqsb1 < mx_eau_var)
			{
				tempYS4 = bqsb1 + gqsb1;
			}
				

			YS1 = runoff1 + temp1;
			YS2 = 0;  
			YS3 = 0;  
			YS4 = tempYS4;
            		YS5 = dpu_cste - tempYS4/ruu_ch;
            		YS6 = dpu_cste - tempYS4/ruu_ch;           		
			YS7 = 1;			
		}
		else
		{
			YS1 = runoff1;   
			YS2 = gqsb1;   
			YS3 = dsg1;
            		YS4 = bqsb1;            		
            		YS5 = dsp1;
            		YS6 = dss1;            		
			YS7 = 0;
		}

		
		if(YS2 < min_sechiba)
			YS3 = 0;
	}
	else
	{   	
	
		YS2 = gqsb_c * gqsb_init;
		YS4 = bqsb_c * bqsb_init;

		YS1 = 0;
		YS5 = 0;
		YS3 = 0;
		YS6 = 0;
		YS7 = 0;               	
	}

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL runoff1, YREAL gqsb1, YREAL  dsg1, YREAL bqsb1, YREAL dsp1, YREAL  dss1, YREAL veget, YREAL ruu_ch, YREAL mx_eau_var, YREAL mx_eau_eau1, YREAL dpu_cste,YREAL bqsb_c, YREAL gqsb_c, YREAL bqsb_init, YREAL gqsb_init )
  {
	double tempYS4 = 0;

	double d_temp1_bqsb1= 0;
	double d_temp1_gqsb1 = 0;
	double d_temp1_mx = 0;

	double d_temp4_mxeau = 0;
	double d_temp4_bqsb1 = 0;
	double d_temp4_gqsb1 = 0;
	

	//double d_YS5_YS4;
	double d_YS5_temp4=0;
	double d_YS5_ruu=0;
	//double d_YS5_mx_eau = 0;

	double tempYS2 = 0;


	if(Yt > 1)
	{
		if((dsp1 < dsg1) && (dsg1 > min_sechiba) && (veget > min_sechiba) )
		{
			
			//pour YS1
			if(bqsb1 + gqsb1 - mx_eau_var > min_sechiba)
			{
				d_temp1_bqsb1 = 1;
				d_temp1_gqsb1 = 1;
				d_temp1_mx = -1;
			}

			
			//sortie YS1	
			YJ1I1 = 1;
			YJ1I2 = d_temp1_gqsb1;
			YJ1I4 = d_temp1_bqsb1;
			YJ1I9 = d_temp1_mx;
			YJ1I3 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I10 =YJ1I11 = 0;

			//sortie YS2 et YS3
			YJ2I1 =YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 =YJ2I10 =YJ2I11 =0;
			YJ3I1 =YJ3I2 =YJ3I3 =YJ3I4 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I11 =0;

			//YS4
			tempYS4 = mx_eau_var;
			d_temp4_mxeau = 1;
			
			if(bqsb1 + gqsb1 < mx_eau_var)
			{
				tempYS4 = bqsb1 + gqsb1;
				d_temp4_bqsb1 = 1;
				d_temp4_gqsb1 = 1;
				d_temp4_mxeau = 0;			
			}
			YJ4I2 =d_temp4_gqsb1;
			YJ4I4 = d_temp4_bqsb1;
			YJ4I9 = d_temp4_mxeau;
			YJ4I1 =YJ4I3 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 = YJ4I10 =YJ4I11 =0;


			//sortie YS5 et YS6
			d_YS5_temp4 = - 1/ruu_ch;
			d_YS5_ruu = tempYS4/(ruu_ch*ruu_ch);
		
			
	           	YJ5I2 = d_YS5_temp4 * d_temp4_gqsb1;
			YJ5I4 = d_YS5_temp4 * d_temp4_bqsb1;
			YJ5I8 = d_YS5_ruu;
			YJ5I9 = d_YS5_temp4 * d_temp4_mxeau;
			YJ5I11 = 1;	           	
			YJ5I1 = YJ5I3 =YJ5I5 =YJ5I6 =YJ5I7 = YJ5I10 =0;

			YJ6I2 = d_YS5_temp4 * d_temp4_gqsb1;
			YJ6I4 = d_YS5_temp4 * d_temp4_bqsb1;
			YJ6I8 = d_YS5_ruu;
			YJ6I9 = d_YS5_temp4 * d_temp4_mxeau;
			YJ6I11 = 1;
			YJ6I1 = YJ6I3 =YJ6I5 =YJ6I6 =YJ6I7 =YJ6I10 =0;
		
			//sortie YS7			
			YJ7I1 =YJ7I2 =YJ7I3 =YJ7I4 =YJ7I5 =YJ7I6 =YJ7I7 =YJ7I8 =YJ7I9 =YJ7I10 =YJ7I11 =0;

			tempYS2 = 0;	
	
		}
		else
		{			
			YJ1I1 = YJ2I2 =YJ3I3 = YJ4I4 =YJ5I5 =YJ6I6 =1;
			YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 =YJ1I10 =YJ1I11 =0;
			YJ2I1 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 =YJ2I10 =YJ2I11 =0;
			YJ3I1 =YJ3I2 =YJ3I4 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I11 =0;
			YJ4I1 =YJ4I2 =YJ4I3 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 =YJ4I9 =YJ4I10 =YJ4I11 =0;
			YJ5I1 =YJ5I2 =YJ5I3 =YJ5I4 =YJ5I6 =YJ5I7 =YJ5I8 =YJ5I9 =YJ5I10 =YJ5I11 =0;
			YJ6I1 =YJ6I2 =YJ6I3 =YJ6I4 =YJ6I5 =YJ6I7 =YJ6I8 =YJ6I9 =YJ6I10 =YJ6I11 =0;
			YJ7I1 =YJ7I2 =YJ7I3 =YJ7I4 =YJ7I5 =YJ7I6 =YJ7I7 =YJ7I8 =YJ7I9 =YJ7I10 =YJ7I11 =0;	

			tempYS2 = gqsb1;
		}


		//if(YS2 < min_sechiba)
		if(tempYS2 < min_sechiba)
		{
			YJ3I1 = YJ3I2 =YJ3I3 =YJ3I4 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I11 =0;
		}

	}
	else
	{   	
		
		YS2 = gqsb_c * gqsb_init;
		YS4 = bqsb_c * bqsb_init;


		YJ2I13 = gqsb_init;
		YJ2I15 = gqsb_c;
		YJ4I12 = bqsb_init;
		YJ4I14 = bqsb_c;




		YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 =YJ1I10 =YJ1I11 =0;
		YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I7 =YJ2I8 =YJ2I9 =YJ2I10 =YJ2I11 =0;
		YJ3I1 = YJ3I2 =YJ3I3 =YJ3I4 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I11 =0;
		YJ4I1 = YJ4I2 =YJ4I3 =YJ4I4 =YJ4I5 =YJ4I6 =YJ4I7 =YJ4I8 =YJ4I9 =0;
		YJ5I1 = YJ5I2 =YJ5I3 =YJ5I4 =YJ5I5 =YJ5I6 =YJ5I7 =YJ5I8 =YJ5I9 =YJ5I10 =YJ5I11 =0;
		YJ6I1 = YJ6I2 =YJ6I3 =YJ6I4 =YJ6I5 =YJ6I6 =YJ6I7 =YJ6I8 =YJ6I9 =YJ6I10 =YJ6I11 =0;
		YJ7I1 = YJ7I2 =YJ7I3 =YJ7I4 =YJ7I5 =YJ7I6 =YJ7I7 =YJ7I8 =YJ7I9 =YJ7I10 =YJ7I11 =0;

		YJ4I10 =YJ4I11 = 0;
		
		//YS4 = mx_eau_eau1*dpu_cste;
		//YJ4I10 = dpu_cste;
		//YJ4I11 = mx_eau_eau1;

            	
	}	
	
  }

  //===========================================================================

//****************** FIN DU MODULE H_soil_verif.h    ***************************

