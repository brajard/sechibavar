  
/***************************************************************************
                 module classe humr_c_8.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL hum8)
  {
	YS1 = hum8 * hum_c8;
  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL hum8)
  {
	YJ1I1 =  hum_c8;
	
  }



  //===========================================================================

//************************** FIN DU MODULE humr_c_8.h ********************************








