  
/***************************************************************************
                 module classe E_emis_planck.h  -  description
                             -------------------
Energy emmited by the surface in the 8-14 microns band

output---> emis_s


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol, YREAL emis_s1)
  {
	double a;
	double b;
	double rad;
	double lambda;

		if(Yj == 0)	
			YS1 = 0;
		else
		{
			lambda = lambda1 + (Yj - 1) * dlambda;
			a = 2 * hc * ch * ch * pow(lambda,-5);
			b = exp( hc * ch / ( lambda * kb * temp_sol)) - 1;
			rad = pi * a / b;
			YS1 = emis_s1 + rad * dlambda;
		}

}
	

  //===========================================================================
  //                           backward
  backward (YREAL temp_sol, YREAL emis_s1)
  {

	double a;
	double b;
	double rad;
	double lambda;

	double d_b_tempsol = 0;
	double d_rad_b = 0;


		if(Yj == 0)	
			YJ1I1 = YJ1I2 = 0;
		else
		{
			lambda = lambda1 + (Yj - 1) * dlambda;
			a = 2 * hc * ch * ch * pow(lambda,-5);

			b = exp( hc * ch / ( lambda * kb * temp_sol)) - 1;
			d_b_tempsol = -(hc * ch / ( lambda * kb * temp_sol * temp_sol)) * exp( hc * ch / ( lambda * kb * temp_sol));
					
			rad = pi * a / b;
			d_rad_b = -pi * a / (b*b);

			
			YJ1I1 = 1;
			YJ1I2 = dlambda * d_rad_b * d_b_tempsol;

		}

	
	

  }

  //===========================================================================

//************************** End of MODULE E_emis_planck.h ********************************


