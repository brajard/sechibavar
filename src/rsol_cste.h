  
/***************************************************************************
                 module classe rsol_cste.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL rsol)
  {
	YS1 = rsol * rsol_cste1;

  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL rsol)
  {
	YJ1I1 = rsol_cste1;

	//printf("%d YG_rsol_cste(0,0) %ev\n",Yt,YG_rsol_cste(0,0));
	
  }



  //===========================================================================

//************************** FIN DU MODULE rsol_cste.h ********************************








