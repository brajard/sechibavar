  
/***************************************************************************
                 module  E_flux1.h  -  description
                             -------------------

temporal variables to compute  energy fluxes


output: YS1 = qc ---> drag coefficient multiplied by the wind

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL speed, YREAL q_cdrag)
  {
	if(Yt > 1)
		YS1 = speed * q_cdrag; 
	else
		YS1 = 0;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL speed, YREAL q_cdrag)
  {
	if(Yt > 1)
	{
		YJ1I1 = q_cdrag;
		YJ1I2 = speed;
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
	
	}

	
  }

  //===========================================================================

//************************** End of MODULE E_flux1.h ********************************














