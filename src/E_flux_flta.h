  
/***************************************************************************
                 module  E_flux_flta.h  -  description
                             -------------------
outputs
	-YS1 = fluxsens: sensible heat flux
	-YS2 = tair: air potential energy divided by the Specific heat of air (cp-air)
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL rau, YREAL qc, YREAL psnew, YREAL epot_air)
  {

	if (Yt == 1)
	{
		YS1 = 0;
		YS2 = 0;
		
	}
	else
	{
		YS1 =  rau * qc * (psnew - epot_air);
		YS2  =  epot_air / cp_air; 		
	}
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL rau, YREAL qc, YREAL psnew, YREAL epot_air) 
  {
	if (Yt == 1)
	{
		
		YJ1I1 = 0;
		YJ1I2 = 0;  
		YJ1I3 = 0;   
		YJ1I4 = 0; 

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
		YJ2I4 = 0;
		
	}
	else
	{
		YJ1I1 = qc * (psnew - epot_air);
		YJ1I2 = rau * (psnew - epot_air);  
		YJ1I3 = rau * qc ;   
		YJ1I4 = -rau * qc; 

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
		YJ2I4 = 1/cp_air;

	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_flux_flta.h ********************************














