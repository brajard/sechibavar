    
/***************************************************************************
                 module classe init_thermosoil.h  -  description


Sorties



                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
//  forward (YREAL so_cond, YREAL so_capa)
  forward (YREAL lskin, YREAL dz11, YREAL dz22, YREAL zz11, YREAL l1)
  {
	//sorties
	double dz1;
	double dz2;
	double zz_coef;
	double lambda;
	double zz1;
	double zz2;


	/*Computing the depth of the Temperature level, using a non dimentional 
	variable x = z/lskin, lskin beeing the skin depth*/
	//double sum = 0;
	//if(Yt == 1)	
	//{
		dz2 = lskin * (fz(Yi+1) - fz(Yi) ) / cstgrnd;

		zz_coef = lskin * fz(Yi+1) / cstgrnd  ;

		//The undimentional depth is computed. Converting to meters.

		zz1 = lskin * ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd;//
		zz2 = lskin * ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd;//
	     	
		//dz1
		if(Yi == ngrnd-1)
			dz1 = 0;
		else
			dz1  = 1 / (zz2 - zz1);
	

		//lambda
		if(Yi ==0)
		    	lambda = zz1 * dz1;
		else
			lambda = 0;
	
	
		YS1 = dz1;
		YS2 = dz2;
		YS3 = zz_coef;
		YS4 = lambda;
	//}
	//else
	//{
	//	YS1 = dz11;
	//	YS2 = dz22;
	//	YS3 = zz11;
	//	YS4 = l1;
	//}

	//printf("%d,%d   %e     %e    %e     %e",Yt,Yi,YS1,YS2,YS3,YS4);

	

	//printf("Yt %d   dz1 %e          dz2 %e       zz_coef %e   lambda %e    \n",Yt,dz1, dz2, zz_coef,lambda);
	//printf("X1 %e    X2 %e  \n", (fz(Yi+1) - fz(Yi) ),  1/ ( ( fz( (double)(Yi+2) - 0.5) ) - ( fz( (double)(Yi+1) - 0.5) ) )   );
	        
  }
	


 //===========================================================================
  //                         methodes  backward
  //backward (YREAL so_cond, YREAL so_capa)
  backward (YREAL lskin, YREAL dz11, YREAL dz22, YREAL zz11, YREAL l1)
  {

	//double cstgrnd;
	//double lskin;
	//double fz1;
	//double zalph;

	//sorties
	double dz1;
	double dz2;
	double zz_coef;
	double lambda;
	double zz1;
	double zz2;

	double d_dz1_zz1 = 0;
	double d_dz1_zz2= 0;

	double d_zz1_lskin= 0;
	double d_zz2_lskin= 0;

	double d_dz2_lskin= 0;

	double d_zzcoef_lskin= 0;

	double d_lambda_zz1= 0;
	double d_lambda_dz1= 0;

	//double d_lskin_socapa= 0;
	//double d_lskin_socond= 0;


	//lskin = sqrt(so_cond / so_capa * one_day / pi);
	//d_lskin_socapa = 0.5* sqrt ( (one_day/pi) / (so_cond /so_capa)  ) * (-so_cond/pow(so_capa,2));
	//d_lskin_socond = 0.5* sqrt ( (one_day/pi) / (so_cond /so_capa)  ) * (1/so_capa);
	

	//cstgrnd =sqrt(one_day / pi);
	//fz1 = 0.3 * cstgrnd;
	//zalph = 2;


	/*Computing the depth of the Temperature level, using a non dimentional 
	variable x = z/lskin, lskin beeing the skin depth*/
	//double sum = 0;
	

	//if(Yt == 1)	
	//{
		dz2 = lskin * (fz(Yi+1) - fz(Yi) ) / cstgrnd ;
		d_dz2_lskin=(fz(Yi+1) - fz(Yi) ) / cstgrnd;

		zz_coef = lskin * fz(Yi+1) / cstgrnd ;
		d_zzcoef_lskin = fz(Yi+1) / cstgrnd ;
		//The undimentional depth is computed. Converting to meters.

		zz1 = lskin * ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd;//
		zz2 = lskin * ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd;//
		d_zz1_lskin=  ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd ;
		d_zz2_lskin=  ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd ;

		if(Yi == ngrnd-1)
		{
			dz1 = 0;
		}
		else
		{
			dz1  = 1 / (zz2 - zz1);
			d_dz1_zz1 = 1/((zz2 - zz1)*(zz2 - zz1));
			d_dz1_zz2= -1/((zz2 - zz1)*(zz2 - zz1));
		}

		if(Yi ==0)
		{
		    	lambda = zz1 * dz1;
			d_lambda_zz1= dz1;
			d_lambda_dz1= zz1;
		}
		else
		{
			lambda = 0;
			d_lambda_zz1= 0;
			d_lambda_dz1= 0;
		}

		//dz1
		YJ1I1 = d_dz1_zz1 * d_zz1_lskin + d_dz1_zz2 * d_zz2_lskin ;	
		//dz2
		YJ2I1 = d_dz2_lskin;
		//zz_coef
		YJ3I1 = d_zzcoef_lskin;
		//lambda
		YJ4I1= d_lambda_zz1 * d_zz1_lskin + d_lambda_dz1 * YJ1I1;
		//printf("1 %e     2  %e\n",d_lambda_zz1 * d_zz1_lskin , d_lambda_dz1 * YJ1I1);

	/*}
	else
	{
		YJ1I2 = YJ2I3 = YJ3I4 = YJ4I5 = 1;
	}*/


	
  }




  //===========================================================================

//************************** FIN DU MODULE init_thermosoil.h ********************************


































































/*

                   methodes forward
  forward (YREAL so_cond, YREAL so_capa)
  {
	
	//double zz_temp[7] = {1.785279754032999E-002, 7.880606102882565E-002, 0.200712588005817, 0.444525641959800, 0.932151749867765, 1.90740396568370, 3.85790839731556};
	//double zz; 

	//double cstgrnd;
	double lskin;
	//double fz1;
	//double zalph;

	//sorties
	double dz1;
	double dz2;
	double zz_coef;
	double lambda;
	double zz1;
	double zz2;


	//cstgrnd =sqrt(one_day / pi);
	lskin = sqrt(so_cond / so_capa * one_day / pi);
	//fz1 = 0.3 * cstgrnd;
	//zalph = 2;

	//printf("cstgrnd  %e  fz1 %e   zalph %e  \n\n ", cstgrnd   ,  fz1   , zalph);


	
		
	

	zz_coef = fz(Yi+1) / cstgrnd * lskin;

	//The undimentional depth is computed. Converting to meters.

	zz1 = ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd * lskin;//
	zz2 = ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd * lskin;//
     	
	//dz1
	if(Yi == ngrnd-1)
		dz1 = 0;
	else
		dz1  = 1 / (zz2 - zz1);
	

	//lambda
	if(Yi ==0)
	    	lambda = zz1 * dz1;
	else
		lambda = 0;
	

	
	YS1 = dz1;
	YS2 = dz2;
	YS3 = zz_coef;
	YS4 = lambda;

	//printf(" dz1 %e          dz2 %e       zz_coef %e   lambda %e    \n",dz1, dz2, zz_coef,lambda);

	
        
  }
	


 //===========================================================================
  //                         methodes  backward
  backward (YREAL so_cond, YREAL so_capa)
  {

	//double cstgrnd;
	double lskin;
	//double fz1;
	//double zalph;

	//sorties
	double dz1;
	double dz2;
	double zz_coef;
	double lambda;
	double zz1;
	double zz2;

	double d_dz1_zz1 = 0;
	double d_dz1_zz2= 0;

	double d_zz1_lskin= 0;
	double d_zz2_lskin= 0;

	double d_dz2_lskin= 0;

	double d_zzcoef_lskin= 0;

	double d_lambda_zz1= 0;
	double d_lambda_dz1= 0;

	double d_lskin_socapa= 0;
	double d_lskin_socond= 0;


	lskin = sqrt(so_cond / so_capa * one_day / pi);
	d_lskin_socapa = 0.5* sqrt ( (one_day/pi) / (so_cond /so_capa)  ) * (-so_cond/pow(so_capa,2));
	d_lskin_socond = 0.5* sqrt ( (one_day/pi) / (so_cond /so_capa)  ) * (1/so_capa);
	

	//cstgrnd =sqrt(one_day / pi);
	//fz1 = 0.3 * cstgrnd;
	//zalph = 2;



		
	
	d_dz2_lskin=(fz(Yi+1) - fz(Yi) ) / cstgrnd;

	zz_coef = fz(Yi+1) / cstgrnd * lskin;
	d_zzcoef_lskin = fz(Yi+1) / cstgrnd ;
	//The undimentional depth is computed. Converting to meters.

	zz1 = ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd * lskin;//
	zz2 = ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd * lskin;//
	d_zz1_lskin= ( fz( (double)(Yi+1) - 0.5) )/ cstgrnd ;
	d_zz2_lskin= ( fz( (double)(Yi+2) - 0.5) )/ cstgrnd ;

     	



	if(Yi == ngrnd-1)
	{
		dz1 = 0;
		d_dz1_zz1 = 0;
		d_dz1_zz2= 0;
	}
	else
	{
		dz1  = 1 / (zz2 - zz1);
		d_dz1_zz1 = 1/pow(zz2 - zz1,2);
		d_dz1_zz2= -1/pow(zz2 - zz1,2);
	}


	if(Yi ==0)
	{
	    	lambda = zz1 * dz1;
		d_lambda_zz1= dz1;
		d_lambda_dz1= zz1;
	}
	else
	{
		lambda = 0;
		d_lambda_zz1= 0;
		d_lambda_dz1= 0;
	}


	//dz1
	YJ1I1 = d_dz1_zz1 * d_zz1_lskin * d_lskin_socond + d_dz1_zz2 * d_zz2_lskin * d_lskin_socond ;
	YJ1I2 = d_dz1_zz1 * d_zz1_lskin * d_lskin_socapa + d_dz1_zz2 * d_zz2_lskin * d_lskin_socapa ;
	//dz2
	YJ2I1 = d_dz2_lskin * d_lskin_socond; 
	YJ2I2 = d_dz2_lskin * d_lskin_socapa; 
	//zz_coef
	YJ3I1 = d_zzcoef_lskin* d_lskin_socond; 
	YJ3I2 = d_zzcoef_lskin* d_lskin_socapa; 
	//lambda
	YJ4I1= d_lambda_zz1 * d_zz1_lskin * d_lskin_socond + d_lambda_dz1 * YJ1I1;

	printf( "primero  %e     2do %e     \n" , d_lambda_zz1 * d_zz1_lskin + d_lskin_socond,d_lambda_dz1 * YJ1I1  );	

	YJ4I2= d_lambda_zz1 * d_zz1_lskin * d_lskin_socapa + d_lambda_dz1 * YJ1I2;


	
  }

*/


  //===========================================================================

//************************** FIN DU MODULE init_thermosoil.h ********************************











