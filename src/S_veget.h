  
/***************************************************************************
                 module  S_veget.h  -  description

Fraction of vegetation type - fraction de surface effectivement couverte par un PFT

veget = vegetmax(1-exp(v*lai)) 

v---> extinction coefficient of the Monsi&Seaki relationship (1953)



                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget_max, YREAL lai, YREAL veget_diff1 )
  {

		if(Yj ==0)
			YS1 = veget_max + veget_diff1;
		else
		{
			YS1 = veget_max * ( 1 - exp(-lai*ext_coef[Yj]));	
			YS2 = veget_diff1 + veget_max - (veget_max * ( 1 - exp(-lai*ext_coef[Yj])) );
		}



  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget_max, YREAL lai, YREAL veget_diff1 )
  {
		if(Yj ==0)
		{
			YJ1I1 = YJ1I3 = 1;
			YJ1I2 = 0;
		}
		else
		{
			
			YJ1I1 = ( 1 - exp(-lai*ext_coef[Yj]));
			YJ1I2 = veget_max * exp(-lai*ext_coef[Yj]) * ext_coef[Yj];
			YJ1I3 = 0;

			YJ2I1 = 1 - ( 1 - exp(-lai*ext_coef[Yj]));
			YJ2I2 = -veget_max * exp(-lai*ext_coef[Yj]) * ext_coef[Yj];
			YJ2I3 = 1;
		}
	
  }

  //===========================================================================

//************************** MODULE S_veget.h  **********************


