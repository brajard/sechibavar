  
/***************************************************************************
                 module classe int_dz2.h  -  description


Sorties


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
 // forward (YREAL dz1, YREAL dz2, YREAL zz_coef)
forward ()
  {


	//YS1 = dz1;
	//YS2 = dz2;
	//YS3 = zz_coef;

	YS1 = Yting[Yj];
	YS2 = Yting[Yj+7];
	YS3 = Yting[Yj+14];
	

	//if(Yj == 0)
	//	printf("%d,%d   dz1 %e dz2 %e YS3 %e \n",Yi,Yj,YS1,YS2,YS3);
	
        
  }
	


 //===========================================================================
  //                         methodes  backward
 // backward (YREAL dz2, YREAL dz22, YREAL zz_coef)
 backward ()
  {
	Yjac[0][Yj] = 1;
	Yjac[1][Yj+7] = 1;
	Yjac[2][Yj+14] = 1;

	//Yjac[0][Yj+17] = 1;
	//Yjac[0][Yj+14] = 1;
  }




  //===========================================================================

//************************** FIN DU MODULE int_dz2.h ********************************











