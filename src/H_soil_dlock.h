  
/***************************************************************************
                 module  H_soil_dlock.h  -  description

Very large upper reservoirs or very close upper and lower reservoirs can be deadlock situations for Choisnel!

outputs:
	YS1 = gqsb_m;
	YS2 = dsg_m;
	YS3 = bqsb_m ;
	YS4 = dsp_m ;
	YS5 = dss_m;
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward


  forward (YREAL gqsb, YREAL dsg, YREAL bqsb, YREAL dsp, YREAL dss, YREAL ruu_ch, YREAL min_drain, YREAL max_drain,YREAL exp_drain, YREAL dpu_cste1)
  {
	double gqseuil, eausup, wd1;
	double gdrainage; 	//Drainage between reservoirs;
	double temp2 = 0;
	double tempd; 		//variable temporal drainage
	//sorties
	double bqsb_m, gqsb_m, dsp_m, dsg_m, dss_m;

	if(Yt >1)
	{
		

		bqsb_m = bqsb;
		dsp_m = dsp;
		gqsb_m = gqsb;
		dsg_m = dsg;
		dss_m = dss;

		//the two reservoirs are very close to each other!!
		if ( abs(dsp-dsg) < min_resdis ) 
		{
			bqsb_m = bqsb + gqsb;
			dsp_m = dpu_cste1 - bqsb_m / ruu_ch;
			gqsb_m = 0;
			dsg_m = 0;
			dss_m = dsp_m;		
			//printf("%d pase\n",Yt);
		}
		
		//Draine some water from the upper to the lower reservoir!
		gqseuil = min_resdis * 2 * ruu_ch;
		eausup = dsg_m * ruu_ch;
		wd1 = 0.75*eausup;

		if (eausup > gqseuil) 
		{
				
			if ((gqsb_m >= wd1) && (dsg_m > 0.10)) 
				tempd =  min_drain * (gqsb_m/eausup) + (max_drain-min_drain)* pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain );
			else
			{
				tempd = min_drain *  (gqsb_m/eausup);				
			}

			if(gqsb_m > 0) 
				temp2 = gqsb_m;


			if(temp2 < tempd)
				gdrainage = temp2;
			else
				gdrainage = tempd;			

		}
		else
			gdrainage=0;
		

		YS1 = gqsb_m -  gdrainage;		
		YS2 = dsg_m -  gdrainage / ruu_ch;
		YS3 = bqsb_m + gdrainage;
		YS4 = dpu_cste1 - (bqsb_m+gdrainage)/ruu_ch;
		YS5 = dss_m;

	}
	else
		YS1 = YS2 =YS3 =YS4 = YS5 = 0;
		
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL gqsb, YREAL dsg, YREAL bqsb, YREAL dsp, YREAL dss, YREAL ruu_ch, YREAL min_drain, YREAL max_drain,YREAL exp_drain, YREAL dpu_cste1)
  {
	double gqseuil, eausup, wd1;
	double gdrainage = 0; 	//Drainage between reservoirs;
	double temp2 = 0;
	double tempd; 		//variable temporal drainage
	//sorties
	double bqsb_m, gqsb_m, dsp_m, dsg_m,dss_m;

	//derivees
	double d_gseuil_ruu= 0;

	double d_eausup_dsgm= 0;
	double d_eausup_ruu= 0;

	double d_wd1_eausup= 0;

	double d_gdrainage_temp2= 0;
	double d_gdrainage_tempd= 0;

	double d_temp2_gqsbm= 0;

	double d_tempd_gqsbm= 0;
	double d_tempd_eausup= 0;
	double d_tempd_wd1= 0;
	double d_temp_min_drain = 0;
	double d_temp_max_drain = 0;
	double d_temp_exp_drain = 0;
	
	double d_bqsbm_bqsb= 0;
	double d_bqsbm_gqsb= 0;

	double d_dspm_bqsbm= 0;
	double d_dspm_ruu= 0;
	double d_dspm_dsp= 0;
	double d_dspm_dpu = 0;
	
	double d_gqsbm_gqsb= 0;
	
	double d_dssm_dss= 0;
	double d_dssm_dspm= 0;
	
	double d_dsgm_dsg= 0;

	double d_YS1_gqsbm= 0;
	double d_YS1_gdrainage= 0;

	double d_YS2_dsgm= 0;
	double d_YS2_gdrainage= 0;
	double d_YS2_ruu= 0;

	double d_YS3_bqsbm= 0;
	double d_YS3_gdrainage= 0;
	
	double d_YS4_bqsbm= 0;
	double d_YS4_gdrainage= 0;
	double d_YS4_ruu= 0;
	double d_YS4_dpu= 0;
	

	if(Yt >1)
	{


		bqsb_m = bqsb;
		dsp_m = dsp;
		gqsb_m = gqsb;
		dsg_m = dsg;
		dss_m = dss;	
			
		d_bqsbm_bqsb= 1;
		d_dspm_dsp= 1;
		d_gqsbm_gqsb= 1;
		d_dsgm_dsg= 1;
		d_dssm_dss= 1;

		//the two reservoirs are very close to each other!!
		if ( abs(dsp-dsg) < min_resdis ) 
		{
			bqsb_m = bqsb + gqsb;
			d_bqsbm_bqsb= 1;
			d_bqsbm_gqsb= 1;

			dsp_m = dpu_cste1 - bqsb_m / ruu_ch;
			d_dspm_bqsbm= - 1/ ruu_ch;
			d_dspm_ruu=  bqsb_m / (ruu_ch*ruu_ch);
			d_dspm_dpu = 1;
			d_dspm_dsp= 0;

			gqsb_m = 0;
			d_gqsbm_gqsb= 0;

			dsg_m = 0;
			d_dsgm_dsg= 0;

			dss_m = dsp_m;
			d_dssm_dspm= 1;
			d_dssm_dss= 0;				
			
			//printf("pase 1 \n");
		
		}
		
		//Draine some water from the upper to the lower reservoir!
		gqseuil = min_resdis * 2 * ruu_ch;
		eausup = dsg_m * ruu_ch;
		wd1 = 0.75*eausup;


		d_gseuil_ruu= min_resdis * 2;
		d_eausup_dsgm= ruu_ch;
		d_eausup_ruu= dsg_m;
		d_wd1_eausup= 0.75;


		if (eausup > gqseuil) 
		{ 
	
			if ((gqsb_m >= wd1) && (dsg_m > 0.10)) 
			{
				tempd =  min_drain * (gqsb_m/eausup) + (max_drain-min_drain)* pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain );

				d_tempd_gqsbm=  min_drain/eausup  + ((max_drain-min_drain)/ pow(eausup-wd1,exp_drain )) * exp_drain * pow( gqsb_m-wd1, exp_drain -1);
				d_tempd_eausup= -min_drain * gqsb_m/(eausup*eausup)  +  ((max_drain-min_drain)* pow( gqsb_m-wd1, exp_drain )) * (-exp_drain / pow(eausup-wd1,exp_drain +1 ));		
				d_tempd_wd1=  (max_drain-min_drain) * exp_drain * pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain-1 ) * (gqsb -eausup)/pow(eausup-wd1,2);
				
				d_temp_min_drain = (gqsb_m/eausup) - pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain );
				d_temp_max_drain = pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain );
				d_temp_exp_drain = pow( (gqsb_m-wd1)/(eausup-wd1),exp_drain ) * log((gqsb_m-wd1)/(eausup-wd1));
			}
			else
			{
				tempd = min_drain* gqsb_m/eausup;

				d_tempd_gqsbm= min_drain /eausup;
				d_tempd_eausup= -min_drain * gqsb_m/(eausup*eausup);
				d_temp_min_drain = gqsb_m/eausup;
			}

			if(gqsb_m > 0) 
			{
				temp2 = gqsb_m;

				d_temp2_gqsbm= 1;				
			}
			
			if(temp2 < tempd)
			{
				gdrainage = temp2;
				d_gdrainage_temp2= 1;
			}
			else
			{
				gdrainage = tempd;
				d_gdrainage_tempd= 1;							
			}
		}
		else
		{
					
			gdrainage=0;
		}
	
		d_YS1_gqsbm= 1;
		d_YS1_gdrainage= -1;

		d_YS2_dsgm= 1;
		d_YS2_gdrainage= -1/ruu_ch;
		d_YS2_ruu= gdrainage/(ruu_ch*ruu_ch);

		d_YS3_bqsbm= 1;
		d_YS3_gdrainage= 1;
	
		d_YS4_bqsbm= -1/ruu_ch;
		d_YS4_gdrainage= -1/ruu_ch;
		d_YS4_ruu= (bqsb_m+gdrainage)/(ruu_ch*ruu_ch);
		d_YS4_dpu= 1;	
		
		//YS1
		YJ1I1 = d_YS1_gqsbm * d_gqsbm_gqsb + d_YS1_gdrainage * d_gdrainage_temp2 * d_temp2_gqsbm * d_gqsbm_gqsb + d_YS1_gdrainage * d_gdrainage_tempd  * d_tempd_gqsbm * d_gqsbm_gqsb;
		YJ1I2 = d_YS1_gdrainage * d_gdrainage_tempd * d_tempd_eausup * d_eausup_dsgm * d_dsgm_dsg + d_YS1_gdrainage * d_gdrainage_tempd * d_tempd_wd1 * d_wd1_eausup * d_eausup_dsgm * d_dsgm_dsg;
		YJ1I6 = d_YS1_gdrainage * d_gdrainage_tempd * d_tempd_wd1 * d_wd1_eausup * d_eausup_ruu  +  d_YS1_gdrainage * d_gdrainage_tempd * d_tempd_eausup * d_eausup_ruu;
		YJ1I7 = d_YS1_gdrainage * d_gdrainage_tempd * d_temp_min_drain;
		YJ1I8 = d_YS1_gdrainage * d_gdrainage_tempd * d_temp_max_drain;
		YJ1I9 = d_YS1_gdrainage * d_gdrainage_tempd * d_temp_exp_drain;
		YJ1I3 = YJ1I4 =YJ1I5 =0;


		

		//YS2
		YJ2I1 = d_YS2_gdrainage * d_gdrainage_temp2 * d_temp2_gqsbm * d_gqsbm_gqsb + d_YS2_gdrainage * d_gdrainage_tempd * d_tempd_gqsbm * d_gqsbm_gqsb;
		YJ2I2 = d_YS2_dsgm*d_dsgm_dsg + d_YS2_gdrainage*d_gdrainage_tempd*d_tempd_eausup*d_eausup_dsgm*d_dsgm_dsg + d_YS2_gdrainage*d_gdrainage_tempd*d_tempd_wd1*d_wd1_eausup*d_eausup_dsgm* d_dsgm_dsg;
		
		YJ2I6 =d_YS2_ruu + d_YS2_gdrainage * d_gdrainage_tempd * d_tempd_eausup * d_eausup_ruu  + d_YS2_gdrainage * d_gdrainage_tempd * d_tempd_wd1 * d_wd1_eausup * d_eausup_ruu;
		YJ2I7 = d_YS2_gdrainage * d_gdrainage_tempd * d_temp_min_drain;
		YJ2I8 = d_YS2_gdrainage * d_gdrainage_tempd * d_temp_max_drain;
		YJ2I9 = d_YS2_gdrainage * d_gdrainage_tempd * d_temp_exp_drain;

		YJ2I3 = YJ2I4 = YJ2I5 =0;

		//YS3
		YJ3I1 = d_YS3_bqsbm*d_bqsbm_gqsb + d_YS3_gdrainage * d_gdrainage_temp2 * d_temp2_gqsbm * d_gqsbm_gqsb + d_YS3_gdrainage * d_gdrainage_tempd  * d_tempd_gqsbm * d_gqsbm_gqsb;
		YJ3I2 = d_YS3_gdrainage*d_gdrainage_tempd*d_tempd_eausup*d_eausup_dsgm*d_dsgm_dsg + d_YS3_gdrainage*d_gdrainage_tempd*d_tempd_wd1*d_wd1_eausup*d_eausup_dsgm* d_dsgm_dsg;
		YJ3I3 = d_YS3_bqsbm * d_bqsbm_bqsb;
		YJ3I4 = 0;
		YJ3I5 = 0;
		YJ3I6 = d_YS3_gdrainage * d_gdrainage_tempd * d_tempd_eausup * d_eausup_ruu  + d_YS3_gdrainage * d_gdrainage_tempd * d_tempd_wd1 * d_wd1_eausup * d_eausup_ruu;
		YJ3I7 = d_YS3_gdrainage * d_gdrainage_tempd * d_temp_min_drain;
		YJ3I8 = d_YS3_gdrainage * d_gdrainage_tempd * d_temp_max_drain;
		YJ3I9 = d_YS3_gdrainage * d_gdrainage_tempd * d_temp_exp_drain;

		//YS4
		YJ4I1 = d_YS4_bqsbm * d_bqsbm_gqsb + d_YS4_gdrainage * d_gdrainage_temp2 * d_temp2_gqsbm * d_gqsbm_gqsb + d_YS4_gdrainage * d_gdrainage_tempd  * d_tempd_gqsbm * d_gqsbm_gqsb;
		YJ4I2 = d_YS4_gdrainage*d_gdrainage_tempd*d_tempd_eausup*d_eausup_dsgm*d_dsgm_dsg + d_YS4_gdrainage*d_gdrainage_tempd*d_tempd_wd1*d_wd1_eausup*d_eausup_dsgm* d_dsgm_dsg;
		YJ4I3 = d_YS4_bqsbm * d_bqsbm_bqsb;
		YJ4I4 = 0;
		YJ4I5 = 0;
		YJ4I6 = d_YS4_ruu + d_YS4_gdrainage * d_gdrainage_tempd * d_tempd_eausup * d_eausup_ruu  + d_YS4_gdrainage * d_gdrainage_tempd * d_tempd_wd1 * d_wd1_eausup * d_eausup_ruu;
		YJ4I7 = d_YS4_gdrainage * d_gdrainage_tempd * d_temp_min_drain;
		YJ4I8 = d_YS4_gdrainage * d_gdrainage_tempd * d_temp_max_drain;
		YJ4I9 = d_YS4_gdrainage * d_gdrainage_tempd * d_temp_exp_drain;
		YJ4I10 = 1;

		//YS5
		YJ5I1 = d_dssm_dspm * d_dspm_bqsbm * d_bqsbm_gqsb;
		YJ5I2 = 0;
		YJ5I3 = d_dssm_dspm * d_dspm_bqsbm * d_bqsbm_bqsb;
		YJ5I4 = d_dssm_dspm * d_dspm_dsp;
		YJ5I5 = d_dssm_dss;
		YJ5I6 = d_dssm_dspm * d_dspm_ruu;
		YJ5I7 = YJ5I8 =YJ5I9 =0;
		YJ5I10 = d_dssm_dspm * d_dspm_dpu;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_dlock.h ********************************



