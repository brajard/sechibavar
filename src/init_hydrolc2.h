  
/***************************************************************************
                 module classe init_hydrolc.h  -  description

-inicializa -vegtot: Total  vegetation; Solo la ultima columna tiene el valor total d la suma por cada point de grille ()
	    -ruu_ch: Quantite d'eau maximum (mm)
	    -mx_eau_var: Profondeur du reservoir contenant le maximum d'eau (mm) 


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL veget,YREAL vegtot1,YREAL mx_eau_var1,YREAL mx_eau_eau1, YREAL dpu_cste1, YREAL vt, YREAL ruu, YREAL eau_var)
  {      
 
	double vegtot, mx_eau_var;


	if(Yt == 1)
	{
		if (Yj == 0)
		{
			vegtot = veget;
			mx_eau_var = veget*mx_eau_eau1*dpu_cste1;
			//mx_eau_var = veget*wmax_veg[Yj]*dpu_cste1;
		}	
		else if(Yj == nvm-1)
		{
			vegtot = vegtot1 + veget;
			if (vegtot1 > min_sechiba) 
				mx_eau_var = mx_eau_var1/vegtot;
			else
			{
				mx_eau_var = mx_eau_eau1*dpu_cste1;
			}

			
		
	   	}
		else
		{
			vegtot = vegtot1 + veget;	
			mx_eau_var = mx_eau_var1 + veget*mx_eau_eau1*dpu_cste1;
			//mx_eau_var = mx_eau_var1 + veget*wmax_veg[Yj]*dpu_cste1;
		}	

	//	printf("%d  vegtot %e ,  mx_eau_var %e,  veget %e  , mx_eau_eau1 %e ,   dpu_cste1 %e  \n    ",Yt,vegtot  ,  mx_eau_var ,  veget   , mx_eau_eau1  ,   dpu_cste1 );	
		
		YS1 = vegtot;
		YS2 = mx_eau_var / dpu_cste1;
		YS3 = mx_eau_var;

		//printf("%d     YS1 %e    YS2 %e    YS3 %e \n",Yt,YS1 ,YS2,YS3 );	
	}
	else
	{
		YS1 = vt;
		YS2 = ruu;
		YS3 = eau_var;

	}

	//printf("ruu %e \n",YS2);
	//printf("%d(%d,%d)YS1 %e    yS2 %e     YS3 %e    mx_eau_eau %e veget %e \n",Yt,Yi,Yj,YS1,YS2,YS3,mx_eau_eau1,veget);


  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL veget,YREAL vegtot1,YREAL mx_eau_var1,YREAL mx_eau_eau1, YREAL dpu_cste1, YREAL vt, YREAL ruu, YREAL eau_var)
  {
	double vegtot=0, mx_eau_var=0;

	//derivadas
	double d_vegtot_veget=0;
	double d_vetot_vegtot1=0;

	double d_mxeau_veget = 0;
	double d_mxeau_mxeau_var1 =0 ;
	double d_mxeau_eau = 0;
	double d_mxeau_vegtot =0;
	double d_mxeau_dpu = 0;

	double d_YS2_dpu = 0;
	double d_YS2_mxeau = 0;

	if(Yt == 1)
	{

		if (Yj == 0)
		{
			vegtot = veget;
			d_vegtot_veget = 1;
			
			mx_eau_var = veget*mx_eau_eau1*dpu_cste1;
			d_mxeau_veget = mx_eau_eau1*dpu_cste1;
			d_mxeau_dpu = veget*mx_eau_eau1;
			d_mxeau_eau = veget*dpu_cste1;		
		}	
		else if(Yj == nvm-1)
		{
			vegtot = vegtot1 + veget;
			d_vegtot_veget = 1;
			d_vetot_vegtot1 = 1;

			if (vegtot1 > min_sechiba) 
			{
				mx_eau_var = mx_eau_var1/vegtot;				
				d_mxeau_mxeau_var1 = 1/vegtot;
				d_mxeau_vegtot = -mx_eau_var1/pow(vegtot,2);

				//printf("Pase 1\n\n");
			}
			else
			{
				mx_eau_var = mx_eau_eau1*dpu_cste1;
				d_mxeau_eau = dpu_cste1;
				d_mxeau_dpu = mx_eau_eau1;
			
				
			}

					
	   	}
		else
		{
			vegtot = vegtot1 + veget;	
			d_vegtot_veget = 1;
			d_vetot_vegtot1 = 1;

			mx_eau_var = mx_eau_var1 + veget*mx_eau_eau1*dpu_cste1;
			d_mxeau_veget = mx_eau_eau1 * dpu_cste1;
			d_mxeau_mxeau_var1 = 1;
			d_mxeau_dpu = veget * mx_eau_eau1;
			d_mxeau_eau = veget * dpu_cste1;
			
		}	

		
		d_YS2_dpu = -mx_eau_var / (dpu_cste1*dpu_cste1);
		d_YS2_mxeau = 1 / dpu_cste1;
		
		//YS1 = vegtot;
		YJ1I1 = d_vegtot_veget;
		YJ1I2 = d_vetot_vegtot1;
		YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 =YJ1I7 = YJ1I8 =   0;	
		

		//YS2 = mx_eau_var / dpu_cste1;
		YJ2I1 = d_YS2_mxeau * d_mxeau_vegtot * d_vegtot_veget + d_YS2_mxeau * d_mxeau_veget ;
		YJ2I2 = d_YS2_mxeau * d_mxeau_vegtot * d_vetot_vegtot1;
		YJ2I3 = d_YS2_mxeau * d_mxeau_mxeau_var1;
		YJ2I4 = d_YS2_mxeau * d_mxeau_eau;
		YJ2I5 = d_YS2_mxeau * d_mxeau_dpu  + d_YS2_dpu;
		//printf("d_mxeau_dpu %e\n",d_mxeau_dpu);
		YJ2I6 = YJ2I7 = YJ2I8 = 0;


		//YS3 = mx_eau_var;
		YJ3I1 = d_mxeau_vegtot * d_vegtot_veget + d_mxeau_veget ;
		YJ3I2 = d_mxeau_vegtot * d_vetot_vegtot1;
		YJ3I3 = d_mxeau_mxeau_var1;
		YJ3I4 = d_mxeau_eau;
		YJ3I5 =  d_mxeau_dpu;
		YJ3I6 = YJ3I7 = YJ3I8 = 0;


		//printf("%d,%d YJ3I5 %e    YJ2I5 %e\n",Yt,Yj,YJ3I5,YJ3I5);

	}
	else
	{
		YJ1I6 = YJ2I7 = YJ3I8 = 1;
	

	}
	
		//printf("%d(%d,%d) veget %e  vegtot: %e   ruu_ch %e   mx_eau %e  \n",Yt,Yi,Yj,veget,YS1,YS2,YS3);
}
  //===========================================================================
//************************** FIN DU MODULE init_hydrolc.h *********************







