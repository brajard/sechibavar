  
/***************************************************************************
                 module classe max_drain.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL max_drain)
  {
	YS1 = max_drain * max_drain1;
  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL max_drain)
  {
	YJ1I1 = max_drain1;
	
  }



  //===========================================================================

//************************** FIN DU MODULE max_drain.h ********************************








