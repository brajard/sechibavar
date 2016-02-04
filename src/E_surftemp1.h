  
/***************************************************************************
                    module  E_surftemp1.h  -  description

the routine surftemp computes the energy balance at the surface with an implicit scheme 
that is connected to the Richtmyer and Morton algorithm of the PBL.
This routines has been divided in several modules, for simplicity

-E_surftemp1
-E_surftemp3
-E_surftemp3
-E_surftemp4
-E_surftemp_pt
-E_surftemp_epot
-E_surftemp_qsol
-E_surftemp_qair
 
this module computes some intermediate variables
output: 
	YS1 = zikt

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL wind, YREAL q_cdrag, YREAL rau)
  {
	if (Yt > 1)
		YS1 = 1/(rau * wind * q_cdrag);
	else
		YS1 = 0;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL wind, YREAL q_cdrag, YREAL rau)
  {
	if (Yt > 1)
	{
		YJ1I1 = -(rau * q_cdrag)/pow(rau * wind * q_cdrag , 2);
		YJ1I2 = -(rau * wind) / pow(rau * wind * q_cdrag , 2);
		YJ1I3 = -(q_cdrag * wind) / pow(rau * wind * q_cdrag , 2);

	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE E_surftemp1.h ********************************














