  
/***************************************************************************
                 module classe int_diaghum.h  -  description

Module pour avoir diaglev seulmenet dans une seule dimension


Sorties


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward

forward ()
  {


	//YS1 = dz1;
	//YS2 = dz2;
	//YS3 = zz_coef;

	YS1 = Yting[Yi];

	//printf(" Yt %d diag %e\n",Yt,YS1);


	//if(Yj == 0)
	//	printf("%d,%d   dz1 %e dz2 %e YS3 %e \n",Yi,Yj,YS1,YS2,YS3);
	
        
  }
	


 //===========================================================================
  //                         methodes  backward
 backward ()
  {
	Yjac[0][Yi] = 1;


	//Yjac[0][Yj+17] = 1;
	//Yjac[0][Yj+14] = 1;
  }




  //===========================================================================

//************************** FIN DU MODULE  int_diaghum.h  ********************************











