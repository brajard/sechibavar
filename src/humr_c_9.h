  
/***************************************************************************
                 module classe humr_c_9.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL hum9)
  {
	YS1 = hum9 * hum_c9;
  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL hum9)
  {
	YJ1I1 =  hum_c9;
	
  }



  //===========================================================================

//************************** FIN DU MODULE humr_c_9.h ********************************








