  
/***************************************************************************
                 module classe z0_over.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL z0_over)
  {
	YS1 = z0_over * z0_over1;

  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL z0_over)
  {
	YJ1I1 = z0_over1;
	
  }



  //===========================================================================

//************************** FIN DU MODULE z0_over.h ********************************





