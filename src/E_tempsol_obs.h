  
/***************************************************************************
                    module  E_tempsol_obs.h  -  description
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL tempsol)
  {
	YS1 = tempsol;
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL tempsol)
  {
	YJ1I1 = 1;
  }

  //===========================================================================

//************************** End of MODULE E_tempsol_obs.h ********************************














