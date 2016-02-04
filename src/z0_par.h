  
/***************************************************************************
                 module classe z0_par.h  -  description


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL z0_c)
  {
	/*if(sechiba_vegmax[0][0] == 1)
		YS1 = z0_c * z0_bare;
	else
		YS1 = z0_c * z0_par1;
	*/

	if(sechiba_vegmax[0][0] == 1)
		YS1 = z0_c * z0_bare;
	else
		YS1 = z0_c * z0_par1;


	//printf("Yt %d  z0_c %e     z0_par  %e  z0_bare %e   YS1 %e  \n",Yt,z0_c, z0_par1,z0_bare, YS1);
  }
	

  //===========================================================================
  //                         methodes  backward
 
backward (YREAL z0_c)
  {
	//YJ1I1 = z0_par1;
	if(sechiba_vegmax[0][0] == 1)
		YJ1I1 = z0_bare;
	else
		YJ1I1 = z0_par1;
	//printf("Yt %d  z0_c %e     z0_par  %e \n",Yt,z0_c, z0_par1);

	
  }



  //===========================================================================

//************************** FIN DU MODULE z0_par.h ********************************




