  
/***************************************************************************
                 module classe D_raerod.h  -  description
                             -------------------
output = raero : aerodynamic resistance

raero = 1 / (q_cdrag*speed)----> drag coefficient and the wind speed;
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL wind, YREAL q_cdrag)
  {
	if(Yt > 1)
	        YS1 = 1/(wind*q_cdrag);
	else
		YS1 = 0;

	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL wind, YREAL q_cdrag)
  {
	if(Yt > 1)
	{
		YJ1I1 = -1/(q_cdrag * wind * wind);
		YJ1I2 = -1/pow(q_cdrag,2) * (1/wind);    
	}
	
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
	}

	
	
  }

  //===========================================================================

//************************** End of MODULE d_raerod.h ********************************














