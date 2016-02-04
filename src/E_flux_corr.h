  
/***************************************************************************
                 module  E_flux_corr.h  -  description
                             -------------------

evapot correction

YS1 = evapot_corr;
 ***************************************************************************/

  //===========================================================================
  //                           forward

  forward ( YREAL qc, YREAL rau, YREAL emis, YREAL psnew, YREAL evapot, YREAL epot_air, YREAL tair, YREAL grad_qsat, YREAL vevapp)
  {

	double temp_correction = 0;
	double correction = 0;
	double correc1 = 0;

	if(Yt > 1)
	{
		if ((evapot > 0) && ((psnew - epot_air) != 0 ))
		{
			correc1 =  (4 * emis * c_stefan * pow(tair,3) + rau * qc * cp_air + chalev0 * rau * qc * grad_qsat * vevapp / evapot );
			if (abs(correc1) > min_sechiba) 
				correction = chalev0 * rau * qc * grad_qsat * (1 - vevapp/evapot) / correc1;
			else
				correction = correc1;	
		}

	       if(correction > 0)
			temp_correction = correction;        
		
		YS1 = (evapot / (1 + temp_correction));
	}
	else
		YS1 = 0;

  }
	

  //===========================================================================
  //                           backward
  backward ( YREAL qc, YREAL rau, YREAL emis, YREAL psnew, YREAL evapot, YREAL epot_air, YREAL tair, YREAL grad_qsat, YREAL vevapp)
  {
	double temp_correction = 0;
	double correction = 0;
	double correc1 = 0;

	double d_correc1_emis = 0;
	double d_correc1_tair = 0;
	double d_correc1_rau = 0;
	double d_correc1_qc = 0;
	double d_correc1_grad = 0;
	double d_correc1_vevapp = 0;
	double d_correc1_evapot = 0;

	double d_correc_rau = 0;
	double d_correc_qc = 0;
	double d_correc_grad = 0;
	double d_correc_vevapp = 0;
	double d_correc_evapot = 0;
	double d_correc_correc1 = 0;

	double d_temp_corr = 0;

	double d_YS1_evapot = 0;
	double d_YS1_temp = 0;


	if(Yt > 1)
	{
		if ((evapot > 0) && ((psnew - epot_air) != 0 ))
		{
			correc1 =  4 * emis * c_stefan * pow(tair,3) + rau * qc * cp_air + chalev0 * rau * qc * grad_qsat * vevapp / evapot ;
				
			d_correc1_emis = 4  * c_stefan * pow(tair,3);
			d_correc1_tair = 12 * emis * c_stefan * pow(tair,2);
			d_correc1_rau = qc * cp_air + chalev0 * qc * grad_qsat * vevapp / evapot;
			d_correc1_qc = rau * cp_air+ chalev0 * rau * grad_qsat * vevapp / evapot;
			d_correc1_grad = chalev0 * rau * qc * vevapp / evapot;
			d_correc1_vevapp = chalev0 * rau * qc * grad_qsat / evapot;
			d_correc1_evapot = -chalev0 * rau * qc * grad_qsat * vevapp / pow(evapot,2);
			

			if (abs(correc1) > min_sechiba) 
			{
				correction = chalev0 * rau * qc * grad_qsat * (1 - vevapp/evapot) / correc1;
				
				d_correc_rau = chalev0 * qc * grad_qsat * (1 - vevapp/evapot) / correc1;
				d_correc_qc = chalev0 * rau * grad_qsat * (1 - vevapp/evapot) / correc1;
				d_correc_grad = chalev0 * rau * qc * (1 - vevapp/evapot) / correc1;
				d_correc_vevapp = chalev0 * rau * qc * grad_qsat * (-1/evapot) / correc1;
				d_correc_evapot = chalev0 * rau * qc * grad_qsat * (vevapp/pow(evapot,2)) / correc1;
				d_correc_correc1 = chalev0 * rau * qc * grad_qsat * (-1 + vevapp/evapot) / pow(correc1,2);
			}	
			else
			{
				correction = correc1;
				d_correc_correc1 = 1;
			}
		}

	       if(correction > 0)
		{
			temp_correction = correction; 
			d_temp_corr = 1;  
		     
		}


		d_YS1_evapot = (1 + temp_correction);
		d_YS1_temp = (-evapot / pow(1 + temp_correction,2));


		YJ1I1 = d_YS1_temp * d_temp_corr * d_correc_qc + d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_qc;
		YJ1I2 =  d_YS1_temp * d_temp_corr * d_correc_rau + d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_rau;
		YJ1I3 = d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_emis;
		YJ1I4 = 0;
		YJ1I5 =  d_YS1_temp * d_temp_corr * d_correc_evapot + d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_evapot + d_YS1_evapot;
		YJ1I6 = 0;
		YJ1I7 = d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_tair;
		YJ1I8 = d_YS1_temp * d_temp_corr * d_correc_grad + d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_grad;
		YJ1I9 = d_YS1_temp * d_temp_corr * d_correc_vevapp + d_YS1_temp * d_temp_corr *d_correc_correc1 *d_correc1_vevapp;

	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = YJ1I7 = YJ1I8 = YJ1I9 = 0;

	}	
  }

  //===========================================================================

//************************** End of MODULE E_flux_corr.h ********************************


