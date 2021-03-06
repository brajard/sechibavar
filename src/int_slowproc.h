  
/***************************************************************************
                 module classe int_slowproc.h  -  description

Description:
	Module pour initialiser diaglev: Vertical diagnostic levels

Output:
	YS1 = diaglev
	YS2 = tem
	YS3 = minval

YS2 en el ultrimo point de grille representa lcanop, que es usado en el modulo S_lai


                             -------------------
 ***************************************************************************/


  //===========================================================================
  //                          methodes forward
  forward (YREAL dpu_cste, YREAL temp1, YREAL minval1, YREAL diag1, YREAL d1, YREAL l1, YREAL m1)
  {

	
	
	double zsoil1, minva = 9999;
	int tem = 0;
	double diag;
//	int lca;

	if(Yt == 1)
	{

		if(Yj ==0)
		{
			minva = 9999;
			tem = 0;
		}
		else
		{
			minva = minval1;
			tem = temp1;	
		}

		//first 2.0 is dpu 
		//second 2.0 is average
		//for(int jv = 1 ; jv <= nbdl-1 ; jv++)
		//{
			//diag = dpu_cste/(pow(2,nbdl-1) -1) * ( ( pow(2,Yj) -1) + ( pow(2,Yj+1) -1) ) / 2;	
			//printf("diag(%d) %e \n",jv-1,diaglev[jv-1]);
		//}
	    	if(Yj == nbdl -1)
			diag = dpu_cste;
		else
			diag = dpu_cste/(pow(2,nbdl-1) -1) * ( ( pow(2,Yj) -1) + ( pow(2,Yj+1) -1) ) / 2;	

	

		//for(int l = 0 ; l< nbdl ; l++)
		//{
			//depth at center of the levels
		//	zsoil1 = (Yj==0)? diag / 2 : ( diag + diag1 ) / 2;
		if (Yj==0) 
			zsoil1=diag / 2 ;
		else  
			zsoil1=( diag + diag1 ) / 2;

		if(abs ( zcanop - zsoil1 ) < minva)
		{
			tem = Yj;	
			minva = abs ( zcanop - zsoil1 );
		}
		
			//printf("%d--> zsoil= %e  temp = %d   minval= %e  \n",l,zsoil[l],temp,minval);
		//}


		//if(Yj == nbdl -1)
		//	lca = tem;

		YS1 = diag;
		YS2 = tem;
		YS3 = minva;

		//printf("      tem %d    minval %e    zsoil1 %e  zsoil(Yj) %e\n",tem,minva,zsoil1,zsoil[Yj]);
	}
	else
	{
		YS1 = d1;
		YS2 = l1;
		YS3 = m1;
	
	}

	//printf("%d,%d   %e     %e    %e    \n",Yt,Yi,YS1,YS2,YS3);

  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL dpu_cste, YREAL temp1, YREAL minval1, YREAL diag1, YREAL d1, YREAL l1, YREAL m1)
  {

	double zsoil1;
	double minva;
	int tem;
	double diag;

	double d_zsoil_diag = 0;
	double d_zsoil_diag1 = 0;

	double d_minva_zsoil1 = 0;
	double d_minva_minval1 = 0;

	double d_tem_temp1 = 0;

	double d_diag_dpu = 0;


	if(Yt == 1)
	{
	
		if(Yj ==0)
		{
			minva = 9999;
			tem = 0;
		}
		else
		{
			minva = minval1;
			tem = temp1;	

			d_minva_minval1 = 1;
			d_tem_temp1 = 1;
		}


	    	if(Yj == nbdl -1)
		{
			diag = dpu_cste;
			d_diag_dpu = 1;
		}
		else
		{
			diag = dpu_cste/(pow(2,nbdl-1) -1) * ( ( pow(2,Yj) -1) + ( pow(2,Yj+1) -1) ) / 2;	
			d_diag_dpu =  1/(pow(2,nbdl-1) -1) * ( ( pow(2,Yj) -1) + ( pow(2,Yj+1) -1) ) / 2;	
		}

	
		if(Yj == 0)
		{
			zsoil1 =  diag / 2;
			d_zsoil_diag = 0.5;
		}
		else
		{
			zsoil1 = ( diag + diag1 ) / 2;
			d_zsoil_diag = 0.5;
			d_zsoil_diag1 = 0.5;
		}

		if(abs ( zcanop - zsoil1 ) < minva)
		{
			tem = Yj;	
			minva = abs ( zcanop - zsoil1 );

			d_minva_minval1 = 0;
			d_tem_temp1 = 0;

			if(zsoil1 > zcanop )
				d_minva_zsoil1 = 1;
			else
				d_minva_zsoil1 = -1;

		}
		
		//YS1 = diag;
		//YS2 = tem;
		//YS3 = minva;

		//YS1
		YJ1I1 = d_diag_dpu;
		YJ1I2 = YJ1I3 = YJ1I4 = 0;
		//YS2
		YJ2I1 = YJ2I3 = YJ2I4 = 0;
		YJ2I2 = d_tem_temp1;
		//YS3
		YJ3I1 = d_minva_zsoil1 * d_zsoil_diag * d_diag_dpu; 
		YJ3I2 = 0;
		YJ3I3 = d_minva_minval1;
		YJ3I4 = d_minva_zsoil1 * d_zsoil_diag1;

		//printf("%d,%d YJ1I1 %e    YJ3I1 %e\n",Yt,Yj,YJ1I1,YJ3I1);

	}
	else
	{
		YJ1I5 = YJ2I6 = YJ3I7 = 1;
	}


  }

  //===========================================================================

//************************** FIN DU MODULE int_slowproc.h ********************************














