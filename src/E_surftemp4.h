  
/***************************************************************************
                    module  E_surftemp4.h  -  description
                             -------------------
//Now we are solving the energy balancen, We calculate the total flux for the 'old' timestep.

output:
	YS1= dtheta --> the change in the surface static energy over the timestep.


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL netrad, YREAL netrad_sns, YREAL sensfl_sns,YREAL larsub_sns,YREAL lareva_sns,YREAL sensfl_old,YREAL larsub_old,YREAL lareva_old,YREAL soilflx,YREAL soilcap) 
  {
	double sum_old, sum_sns;

	if(Yt > 1)
	{
		sum_old = netrad + sensfl_old + larsub_old + lareva_old + soilflx;
		sum_sns = netrad_sns + sensfl_sns + larsub_sns + lareva_sns;

		YS1 =  dtradia * sum_old / (zicp *  soilcap + dtradia * sum_sns);

	}
	else
		YS1 = 0;
	

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL netrad, YREAL netrad_sns, YREAL sensfl_sns,YREAL larsub_sns,YREAL lareva_sns,YREAL sensfl_old,YREAL larsub_old,YREAL lareva_old,YREAL soilflx,YREAL soilcap) 
  {
	double sum_old, sum_sns;


	double d_sum_old_netrad = 0;
	double d_sum_old_sensfl_old = 0;
	double d_sum_old_larsub_old = 0;
	double d_sum_old_lareva_old = 0;
	double d_sum_old_soilflx = 0;

	double d_sum_sns_netrad_sns = 0;
	double d_sum_sns_sensfl_sns = 0;
	double d_sum_sns_larsub_sns = 0;
	double d_sum_sns_lareva_sns = 0;

	double d_YS1_sum_old = 0;
	double d_YS1_soilcap = 0;
	double d_YS1_sum_sns = 0;
	


	if(Yt > 1)
	{
		sum_old = netrad + sensfl_old + larsub_old + lareva_old + soilflx;
		sum_sns = netrad_sns + sensfl_sns + larsub_sns + lareva_sns;

		d_sum_old_netrad = 1;
		d_sum_old_sensfl_old = 1;
		d_sum_old_larsub_old = 1;
		d_sum_old_lareva_old = 1;
		d_sum_old_soilflx = 1;

		d_sum_sns_netrad_sns = 1;
		d_sum_sns_sensfl_sns = 1;
		d_sum_sns_larsub_sns = 1;
		d_sum_sns_lareva_sns = 1;

		d_YS1_sum_old = dtradia / (zicp *  soilcap + dtradia * sum_sns);
	   	d_YS1_soilcap = -zicp * dtradia * sum_old / pow(zicp *  soilcap + dtradia * sum_sns, 2);
		d_YS1_sum_sns = -(pow(dtradia,2) * sum_old) / pow(zicp *  soilcap + dtradia * sum_sns, 2);

		YJ1I1 = d_YS1_sum_old * d_sum_old_netrad ;
		YJ1I2 = d_YS1_sum_sns * d_sum_sns_netrad_sns;
		YJ1I3 = d_YS1_sum_sns * d_sum_sns_sensfl_sns;
		YJ1I4 = d_YS1_sum_sns * d_sum_sns_larsub_sns;
		YJ1I5 = d_YS1_sum_sns * d_sum_sns_lareva_sns;
		YJ1I6 = d_YS1_sum_old * d_sum_old_sensfl_old;
		YJ1I7 = d_YS1_sum_old * d_sum_old_larsub_old;
		YJ1I8 = d_YS1_sum_old * d_sum_old_lareva_old;
		YJ1I9 = d_YS1_sum_old * d_sum_old_soilflx;
		YJ1I10 = d_YS1_soilcap;

	}
	else
	{

		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;
		YJ1I5 = 0;
		YJ1I6 = 0;
		YJ1I7 = 0;
		YJ1I8 = 0;
		YJ1I9 = 0;
		YJ1I10 = 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp4.h ********************************














