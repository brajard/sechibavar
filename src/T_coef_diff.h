  
/***************************************************************************
                 module  T_coef_diff.h  -  description


    !   computation of the surface diffusive flux from ground and
    !   calorific capacity of the ground:

//sorties
	YS1 = soilcap---->Soil capacity;
	YS2= soilflx-----> surface diffusive flux;


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL zdz1,YREAL zdz2,YREAL cgrnd,YREAL dgrnd,YREAL ptn,YREAL temp_sol_new, YREAL lam)
  {
	double temp_flx, temp_cap;
	double z1; //numerical constant (W/m2/K)


	temp_flx = zdz1 * (cgrnd + (dgrnd-1) * ptn);

	temp_cap = zdz2 * dtradia  +   dtradia * (1 - dgrnd) * zdz1;
	
	z1= lam * (1 - dgrnd) + 1;

	YS1 = temp_cap / z1;

	YS2 = temp_flx + (temp_cap/z1) * (ptn * z1 - lam * cgrnd - temp_sol_new) / dtradia ;
}
	

  //===========================================================================
  //                           backward
  backward (YREAL zdz1,YREAL zdz2,YREAL cgrnd,YREAL dgrnd,YREAL ptn,YREAL temp_sol_new, YREAL lam)
  {	
	double temp_flx, temp_cap;
	double z1; //numerical constant (W/m2/K)

	//derivadas de YS1 y YS2
	double d_YS1_temp_cap;
	double d_YS1_z1;

	double d_YS2_temp_flx;
	double d_YS2_temp_cap;
	double d_YS2_z1;
	double d_YS2_lam;

	//derivadas de temp_flx
	double d_temp_flx_zdz1;
	double d_temp_flx_cgrnd;
	double d_temp_flx_dgrnd;
	double d_temp_flux_ptn;

	//derivadas de temp_cap
	double d_temp_cap_zdz1;
	double d_temp_cap_zdz2;
	double d_temp_cap_dgrnd;
	
	// derivadas de z1
	double d_z1_dgrnd;
	double d_z1_lam;

	


	temp_flx = zdz1 * (cgrnd + (dgrnd-1) * ptn);
	temp_cap = zdz2 * dtradia  +   dtradia * (1 - dgrnd) * zdz1;

	z1 = lam * (1 - dgrnd) + 1;
	//derivadas de temp_flx
	d_temp_flx_zdz1 = (cgrnd + (dgrnd-1) * ptn);
	d_temp_flx_cgrnd = zdz1 ;
	d_temp_flx_dgrnd = zdz1 * ptn;
	d_temp_flux_ptn = zdz1 * (dgrnd -1);

	//derivadas de temp_cap
	d_temp_cap_zdz1 = dtradia * (1 - dgrnd);
	d_temp_cap_zdz2 = dtradia;
	d_temp_cap_dgrnd = -dtradia * zdz1;
	
	// derivadas de z1
	//d_z1_dgrnd = -lambda;
	d_z1_dgrnd = -lam;
	d_z1_lam = (1 - dgrnd);

	//derivada de YS1 y YS2
	d_YS1_temp_cap = 1/z1;
	d_YS1_z1 = -temp_cap / (z1 * z1);
	d_YS2_temp_flx = 1;
	d_YS2_temp_cap = (1/z1) * (ptn * z1 - lam * cgrnd - temp_sol_new) / dtradia ;
	d_YS2_z1 = (1/dtradia) * (temp_cap * lam * cgrnd  +  temp_cap * temp_sol_new)/(z1*z1);
	d_YS2_lam = -(cgrnd * temp_cap)/( z1 * dtradia) ;


	YJ1I1 = d_YS1_temp_cap * d_temp_cap_zdz1;
	YJ1I2 = d_YS1_temp_cap * d_temp_cap_zdz2;
	YJ1I3 = 0;
	YJ1I4 = d_YS1_temp_cap * d_temp_cap_dgrnd + d_YS1_z1 * d_z1_dgrnd;
	YJ1I5 = 0;
	YJ1I6 = 0;
	YJ1I7 = d_YS1_z1 * d_z1_lam;

	YJ2I1 = d_YS2_temp_flx * d_temp_flx_zdz1  +   d_YS2_temp_cap * d_temp_cap_zdz1;
	YJ2I2 = d_YS2_temp_cap * d_temp_cap_zdz2;
	YJ2I3 = d_YS2_temp_flx * d_temp_flx_cgrnd  - (temp_cap * lam) / (z1 * dtradia);
	YJ2I4 = d_YS2_temp_flx * d_temp_flx_dgrnd + d_YS2_temp_cap * d_temp_cap_dgrnd + d_YS2_z1 *  d_z1_dgrnd;
	YJ2I5 = d_YS2_temp_flx * d_temp_flux_ptn + temp_cap/dtradia;
	YJ2I6 = -temp_cap / (z1 * dtradia);	
	YJ2I7 = d_YS2_lam + d_YS2_z1 * d_z1_lam;

	
  }

  //===========================================================================

//**************************  MODULE T_coef_diff.h ***********************














