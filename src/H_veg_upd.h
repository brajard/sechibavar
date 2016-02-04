  
/***************************************************************************
                 module  H_veg_upd.h  -  description

resdist: old vegetation map
We update resdist for the next "veget" change
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL veget1, YREAL resdist1)
  {

	if(Yt ==1)
		YS1 = veget1 ;
	else
		YS1 = veget;
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget, YREAL veget1, YREAL resdist1)
  {
	if(Yt ==1)
	{
		YJ1I1 = YJ1I3 = 0;
		YJ1I2 = 1;
	}
	else
	{
		YJ1I2 = YJ1I3 = 0;
		YJ1I1 = 1;
	}
	
  }

  //===========================================================================

//************************** End of  MODULE H_veg_upd.h ********************************














