  
/***************************************************************************
                 module classe int_lskin.h  -  description


Sorties
	YS1 = lskin --> the skin depth


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  //forward (YREAL so_cond, YREAL so_capa,YREAL lskin1)
  forward (YREAL lskin1)
  {
	double temp;

	if(Yt == 1)
	{

		temp  = so_cond_m / so_capa_m;

		YS1 = sqrt(temp * one_day / pi);	

	}
	else
		YS1 = lskin1;

	//printf("%d,%d   YS1 = %e \n",Yt,Yi,YS1);


	//printf("############################# 2 \n\n");

	//printf("\n\n****LSKIN****\n");
	//printf("Yt %d    so_cond %e     so_capa %e       temp %e      YS1 %e \n\n",Yt, so_cond,so_capa,temp,YS1);
	//printf("\n\n ********int_thermosoil*******\n");
        
  }
	


 //===========================================================================
  //                         methodes  backward
//  backward (YREAL so_cond, YREAL so_capa,YREAL lskin1)
  backward (YREAL lskin1)
  {

	if(Yt == 1)
	{

		YJ1I1 =	0;

	}
	else
	{
		YJ1I1 =	1;
	}

	


	/*double temp;

	double d_temp_cond = 0;
	double d_temp_capa = 0;
	
	double d_YS1_temp = 0;

	
		temp  = so_cond / so_capa;

		d_temp_cond = 1 / so_capa;
		d_temp_capa = -so_cond / (so_capa*so_capa);
	
		d_YS1_temp = one_day / (2*pi * sqrt(temp * one_day / pi)) ;


		YJ1I1 = d_YS1_temp * d_temp_cond;
		YJ1I2 = d_YS1_temp * d_temp_capa;*/


	
  }




  //===========================================================================

//************************** FIN DU MODULE int_lskin.h ********************************











