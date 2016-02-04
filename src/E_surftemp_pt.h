  
/***************************************************************************
                    module  E_surftemp_pt.h  -  description
                             -------------------
computes 
	-psnew: New surface static energy
	-temp_sol_new: New soil temperature

	YS1 = psnew
	YS2 = temp_sol_new;

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL psold, YREAL dtheta)
  {

	if(Yt == 1)
	{
		YS1 = 0;	
		YS2 = 0;
	}
	else
	{
		YS1 =  psold + dtheta;	
		YS2 = (psold + dtheta) / cp_air;	
	}

	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL psold, YREAL dtheta)
  {
	if(Yt == 1)
	{
		YJ1I1 = 0;
		YJ1I2 = 0;

		YJ2I1 = 0;
		YJ2I2 = 0;
	}
	else
	{
		YJ1I1 = 1;
		YJ1I2 = 1;

		YJ2I1 = 1/cp_air;
		YJ2I2 = 1/cp_air;

	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp_pt.h ********************************














