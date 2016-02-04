  
/***************************************************************************
                 module  D_trans.h  -  description
                             -------------------
output: vbeta3 : Beta for Transpiration. resistance stomatique

partial beta coefficient : transpiration for each type of vegetation

1 PDT : humrel y qsintveg equal to zero

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ( YREAL veget, YREAL lai, YREAL swnet, YREAL humrel, YREAL wind, YREAL q_cdrag, YREAL rveget, YREAL vbeta23, YREAL zqsvegrap, YREAL rveg_par)
  {
	double vbeta3;

	double rveget_p = rveget * rveg_par;
	
	if(Yt > 1)
	{					  
		if( ( veget*lai > min_sechiba  ) &&  ( kzero[Yj] > min_sechiba ) && ( swnet > min_sechiba ) )
		{              
		
			vbeta3 = veget * (1 - zqsvegrap) * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))));
			
			if(  vbeta23 < veget * zqsvegrap * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))))  )
			{				
				YS1 = vbeta3 + vbeta23;					

			}
			else
			{				
				YS1 = vbeta3 +  veget * zqsvegrap * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))));				
			}
		}
		else
			YS1 = 0;	
		
	}
	else
	{
		YS1 = 0;
	}
}

  //===========================================================================
  //                           backward
  backward( YREAL veget, YREAL lai, YREAL swnet, YREAL humrel, YREAL wind, YREAL q_cdrag, YREAL rveget, YREAL vbeta23, YREAL zqsvegrap, YREAL rveg_par)
  {
	//double vbeta3;
	double rveget_p = rveget * rveg_par;

	double d_rvegetp_rveg = 0;
	double d_rvegetp_rveget = 0;

	double d_vbeta3_veget =0;
	double d_vbeta3_zqsvegrap = 0;
	double d_vbeta3_humrel = 0;
	double d_vbeta3_wind =0;
	double d_vbeta3_qcdrag = 0;
	double d_vbeta3_rveget = 0;
	
	double d_YS1_vbeta3 = 0;
	double d_YS1_vbeta23 = 0;
	
	double d_YS1_veget = 0;
	double d_YS1_zqsvegrap = 0;
	double d_YS1_humrel =0;
	double d_YS1_wind = 0;
	double d_YS1_qcdrag =0;
	double d_YS1_rveget = 0;


	d_rvegetp_rveg =  rveget;
	d_rvegetp_rveget = rveg_par;


	if(Yt > 1)
	{					  
		if( ( veget*lai > min_sechiba  ) &&  ( kzero[Yj] > min_sechiba ) && ( swnet > min_sechiba ) )
		{  			
			d_vbeta3_veget = (1 - zqsvegrap) * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))));
			d_vbeta3_zqsvegrap = -veget * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))));
			d_vbeta3_humrel = veget * (1 - zqsvegrap) * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))));
			d_vbeta3_wind = veget*(1 - zqsvegrap) * humrel * (-q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])) / pow(1 + wind * q_cdrag *(rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])),2));			
			d_vbeta3_qcdrag =  - veget* (1 - zqsvegrap) * humrel * wind * rveg_pft[Yj]* (rveget_p + rstruct[Yi][Yj] ) / pow( 1 + wind * q_cdrag * rveg_pft[Yj] * (rveget_p + rstruct[Yi][Yj]),2);
			d_vbeta3_rveget = -veget * (1 - zqsvegrap) * humrel * wind * q_cdrag * rveg_pft[Yj]/ pow (1 + wind * q_cdrag * rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]) ,2);
			
			if(  vbeta23 < veget * zqsvegrap * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]))))  )
			{				
				d_YS1_vbeta3 = 1;
				d_YS1_vbeta23 = 1;

				YJ1I1 = d_YS1_vbeta3 * d_vbeta3_veget;
				YJ1I2 = 0;
				YJ1I3 = 0;
				YJ1I4 = d_YS1_vbeta3 * d_vbeta3_humrel;
				YJ1I5 = d_YS1_vbeta3 * d_vbeta3_wind;
				YJ1I6 = d_YS1_vbeta3 * d_vbeta3_qcdrag;				
				YJ1I7 = d_YS1_vbeta3 * d_vbeta3_rveget * d_rvegetp_rveget;
				YJ1I8 = d_YS1_vbeta23;
				YJ1I9 = d_YS1_vbeta3 * d_vbeta3_zqsvegrap;			
				YJ1I10 = d_YS1_vbeta3 * d_vbeta3_rveget * d_rvegetp_rveg  ;

							
			}
			else
			{				
				d_YS1_vbeta3 = 1;
			
				d_YS1_veget = (zqsvegrap * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])))));
				d_YS1_zqsvegrap = (veget * humrel * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])))));
				d_YS1_humrel = (zqsvegrap * veget * (1 / (1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])))));
				d_YS1_wind = (veget * zqsvegrap * humrel) * ( - q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])) / pow(1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])),2));
				d_YS1_qcdrag = (veget * zqsvegrap * humrel) * ( -wind * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])) / pow(1 + wind * q_cdrag * (rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj])),2));
				d_YS1_rveget = d_vbeta3_rveget -( veget * zqsvegrap * humrel * wind * q_cdrag * rveg_pft[Yj]) / pow (1 + wind * q_cdrag * rveg_pft[Yj]*(rveget_p + rstruct[Yi][Yj]),2);
				
	
				
				YJ1I1 = d_YS1_vbeta3 * d_vbeta3_veget + d_YS1_veget;
				YJ1I2 = 0;
				YJ1I3 = 0;
				YJ1I4 = d_YS1_vbeta3 * d_vbeta3_humrel+d_YS1_humrel;
				YJ1I5 = d_YS1_vbeta3 * d_vbeta3_wind+d_YS1_wind;
				YJ1I6 = d_YS1_vbeta3 * d_vbeta3_qcdrag+d_YS1_qcdrag;				
				YJ1I7 = d_YS1_vbeta3 * d_vbeta3_rveget * d_rvegetp_rveget + d_YS1_rveget * d_rvegetp_rveget;
				YJ1I8 = d_YS1_vbeta23;
				YJ1I9 = d_YS1_vbeta3 * d_vbeta3_zqsvegrap; 
				YJ1I10 =  d_YS1_vbeta3 * d_vbeta3_rveget * d_rvegetp_rveg + d_YS1_rveget * d_rvegetp_rveg;


			}
		}
		else
			YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 =YJ1I10 =0;

	
		
	}
	else
	{
		YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 =YJ1I10 =0;
	}

	
		
  }

  //===========================================================================

//************************** End of MODULE D_trans.h ********************************

