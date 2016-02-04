  
/***************************************************************************
                 module  H_soil_shum.h  -  description

Relative soil moisture, Initial value of shumdiag (mean relative soil moisture in the different diagnostic levels, for use by thermosoil.f90): 
	YS1 = shumdiag;
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL mean_dsg,YREAL mean_bqsb,YREAL mean_gqsb, YREAL mx_eau_var, YREAL tmp_dl, YREAL tmp_dl1)
  {
	double gtr = 0, btr = 0, shumdiag = 0;
	double temp = 0;

	if ( tmp_dl < mean_dsg) 
	{
		shumdiag = mean_gqsb/mx_eau_var;
	}
	else
	{
		if ( tmp_dl1 < mean_dsg)
		{
			gtr = (mean_dsg-tmp_dl1)/(tmp_dl-tmp_dl1);
			btr = 1 - gtr;
			shumdiag = gtr*mean_gqsb/mx_eau_var + btr*mean_bqsb/mx_eau_var;
		}
		else
		{
			shumdiag = mean_bqsb/mx_eau_var;
		}
            	
	}

	if(shumdiag < 1)
	{
		temp = shumdiag;
	}
	else
	{
		temp = 1;
	}

	if(temp > 0)
		YS1 = temp;
	else
		YS1 = 0;	
  }	

  //===========================================================================
  //                           backward
  backward (YREAL mean_dsg,YREAL mean_bqsb,YREAL mean_gqsb, YREAL mx_eau_var, YREAL tmp_dl, YREAL tmp_dl1)
  {
	double gtr, btr, shumdiag, temp = 0;

	double d_gtr_mgsg=0;
	double d_gtr_tmp1 = 0;
	double d_gtr_tmp = 0;

	double d_btr_gtr=0;

	double d_shumdiag_gtr=0;
	double d_shumdiag_btr=0;
	double d_shumdiag_mbqsb=0;
	double d_shumdiag_mgqsb=0;
	double d_shumdiag_mx = 0;

	if ( tmp_dl < mean_dsg) 
	{
		shumdiag = mean_gqsb/mx_eau_var;

		d_shumdiag_mgqsb = 1/mx_eau_var;
		d_shumdiag_mx = -mean_gqsb/pow(mx_eau_var,2);
	}
	else
	{
		if ( tmp_dl1 < mean_dsg)
		{
			gtr = (mean_dsg-tmp_dl1)/(tmp_dl-tmp_dl1);
			btr = 1 - gtr;
			shumdiag = gtr*mean_gqsb/mx_eau_var + btr*mean_bqsb/mx_eau_var;

			d_gtr_mgsg= 1/(tmp_dl-tmp_dl1);
			d_gtr_tmp1 = -(mean_dsg-tmp_dl)/pow(tmp_dl-tmp_dl1,2);
			d_gtr_tmp = (mean_dsg-tmp_dl1)/pow(tmp_dl-tmp_dl1,2);

			d_btr_gtr = -1;

			d_shumdiag_gtr= mean_gqsb/mx_eau_var ;
			d_shumdiag_btr=mean_bqsb/mx_eau_var;
			d_shumdiag_mbqsb=btr/mx_eau_var;
			d_shumdiag_mgqsb=gtr/mx_eau_var;
			d_shumdiag_mx = -gtr*mean_gqsb/pow(mx_eau_var,2) -btr*mean_bqsb/pow(mx_eau_var,2);
		}
		else
		{
			shumdiag = mean_bqsb/mx_eau_var;

			d_shumdiag_mbqsb= 1/mx_eau_var;;
			d_shumdiag_mx = -mean_bqsb/pow(mx_eau_var,2);
		}
            	
	}

	if(shumdiag < 1)
	{
		temp = shumdiag;
	}
	else
	{
		temp = 1;
	}


	if(temp > 0)
	{
		YJ1I1 = d_shumdiag_gtr * d_gtr_mgsg  + d_shumdiag_btr *d_btr_gtr *  d_gtr_mgsg;
		YJ1I2 = d_shumdiag_mbqsb;
		YJ1I3 = d_shumdiag_mgqsb;
		YJ1I4 = d_shumdiag_mx;
		YJ1I5 = -(d_shumdiag_gtr * d_gtr_tmp + d_shumdiag_btr *d_btr_gtr *  d_gtr_tmp);
		YJ1I6= -(d_shumdiag_gtr * d_gtr_tmp1 + d_shumdiag_btr *d_btr_gtr *  d_gtr_tmp1);

	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6= 0;
	}
  }

  //===========================================================================

//************************** End of MODULE H_soil_shum.h ********************************


