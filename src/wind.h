  
/***************************************************************************
                 module classe wind.h  -  description

calculo del viento
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL u, YREAL v)
  {

        if(sqrt(u*u + v*v)< min_wind) 
		YS1 = min_wind;
        else
                YS1 = sqrt(u*u + v*v);
	//printf("u: %e  v: %e YS1: %e \n", u, v, YS1);	
  }
	

  //===========================================================================
  //                         methodes  backward
  backward(YREAL u, YREAL v)
  {
	if(sqrt(u*u + v*v)< min_wind) 
		YJ1I1 = YJ1I2 =0;
        else
	{
		YJ1I1 = u / sqrt(u*u + v*v);
		YJ1I2 = v / sqrt(u*u + v*v);
	}

  }

  //===========================================================================

//************************** FIN DU MODULE T_profile.h ********************************














