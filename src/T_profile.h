  
/***************************************************************************
                 module  T_profile.h  -  description

!Computation of : the ground temperature evolution
   !   objet:  computation of : the ground temperature evolution
    !   ------
    !   Method: implicit time integration
    !   -------
    !   Consecutives ground temperatures are related by:
    !           T(k+1) = C(k) + D(k)*T(k)  (1)
    !   the coefficients C and D are computed at the t-dt time-step.
    !   Routine structure:
    !   -new temperatures are computed  using (1)
    !    surface temperature

output:
	YS1 = ptn--->Different levels soil temperature
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL dgrnd,YREAL cgrnd,YREAL temp_sol_new,YREAL ptn1,YREAL dgrnd1,YREAL cgrnd1,YREAL ptn_init, YREAL lam)
  {
	
	if(Yt == 1)
		YS1 = ptn_init;
	else
	{
		if(Yj == 0)
		{
			//poner inicializacion de cgrnd y dgrnd
			YS1 = (lam * cgrnd1 + temp_sol_new) / (lam * (1 - dgrnd1) + 1);

		}
		else
			YS1 = cgrnd + dgrnd * ptn1;	
	}

  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL dgrnd,YREAL cgrnd,YREAL temp_sol_new,YREAL ptn1,YREAL dgrnd1,YREAL cgrnd1,YREAL ptn_init, YREAL lam)
  {
	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 = YJ1I8 = 0;
		YJ1I7 = 1;
	}
	else
	{
		if(Yj == 0)
		{
			YJ1I3 = 1 / (lam * (1 - dgrnd1) + 1);
			YJ1I5 = (lam * cgrnd1 + temp_sol_new)*lam / pow(lam * (1 - dgrnd1) + 1 , 2);
			YJ1I6 = lam / (lam * (1 - dgrnd1) + 1);
			YJ1I8 = ( cgrnd1+   (dgrnd1-1)*temp_sol_new  )/ pow(lam * (1 - dgrnd1) + 1 , 2);
			YJ1I1 =YJ1I2 = YJ1I4 =YJ1I7 = 0;
		}
		else
		{
			YJ1I1 = ptn1;
			YJ1I2 = 1;
			YJ1I4 = dgrnd;
			YJ1I3 =YJ1I5 = YJ1I6 =YJ1I7 =YJ1I8 =0;

		}
			
	}
	
	
  }

  //===========================================================================

//************************** MODULE T_profile.h ********************************














