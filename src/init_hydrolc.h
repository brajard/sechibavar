  
/***************************************************************************
                 module classe init_hydrolc.h  -  description

-inicializa -vegtot: Total  vegetation; Solo la ultima columna tiene el valor total d la suma por cada point de grille ()
	    -ruu_ch: Quantite d'eau maximum (mm)
	    -mx_eau_var: Profondeur du reservoir contenant le maximum d'eau (mm) 


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL veget,YREAL vegtot1,YREAL mx_eau_var1, YREAL v, YREAL r, YREAL m, YREAL mx_eau_eau1)
  {
 
	double vegtot, mx_eau_var;

	if(Yt == 1)
	{
		if (Yj == 0)
		{
			vegtot = veget;
			mx_eau_var = veget*wmax_veg[Yj]*dpu_cste;
		}	
		else if(Yj == nvm-1)
		{
			vegtot = vegtot1 + veget;
			if (vegtot1 > 0) 
				mx_eau_var = mx_eau_var1/vegtot;
			else
				mx_eau_var = mx_eau_eau1*dpu_cste;
			
    		}
		else
		{
			vegtot = vegtot1 + veget;	
			mx_eau_var = mx_eau_var1 + veget*wmax_veg[Yj]*dpu_cste;
			//ruu_ch = mx_eau_var / dpu_cste;
		}		

		YS1 = vegtot;
		YS2 = mx_eau_var / dpu_cste;
		YS3 = mx_eau_var;


	//	printf("-------------YS1 %e    yS2 %e     YS3 %e \n",YS1,YS2,YS3);



		//printf("%d(%d,%d) veget %e  vegtot: %e   ruu_ch %e   mx_eau %e  \n",Yt,Yi,Yj,veget,YS1,YS2,YS3);
	}
	else
	{
		YS1 = v;
		YS2 = r;
		YS3 = m;

		
	}

	//printf("%d-%d   YS1 %e      YS2 %e      YS3 %e     mx_eau_eau %e \n  ",Yt,Yj,YS1,YS2,YS3,mx_eau_eau);


  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL veget,YREAL vegtot1,YREAL mx_eau_var1, YREAL v, YREAL r, YREAL m, YREAL mx_eau_eau1)
  {
	double vegtot, mx_eau_var;

	//derivadas
	double dvegtot_veget;
	double d_vetot_vegtot1;

	double dmx_eau_veget;
	double dmx_eau_mx_eau_var1;
	double dmx_eau_eau = 0;
	double dmx_eau_vegtot;

	if(Yt == 1)
	{
		if (Yj == 0)
		{
			mx_eau_var = veget*wmax_veg[Yj]*dpu_cste;
			dmx_eau_veget = wmax_veg[Yj]*dpu_cste;
			dmx_eau_mx_eau_var1 = 0;
			dmx_eau_vegtot = 0;

			vegtot = veget;
			dvegtot_veget = 1;
			d_vetot_vegtot1 = 0;
		}	
		else if(Yj == nvm-1)
		{
			vegtot = vegtot1 + veget;
			dvegtot_veget = 1;
			d_vetot_vegtot1 = 1;

			if (vegtot1 > 0) 
			{
				mx_eau_var = mx_eau_var1/vegtot;	
				dmx_eau_veget = 0;
				dmx_eau_mx_eau_var1 = 1/vegtot;
				dmx_eau_vegtot = -mx_eau_var1/pow(vegtot,2);
			}
			else
			{
				mx_eau_var = mx_eau_eau1*dpu_cste;
				dmx_eau_veget = dmx_eau_mx_eau_var1 = dmx_eau_vegtot = 0;
				dmx_eau_eau = dpu_cste;
			}			
    		}
		else
		{
			vegtot = vegtot1 + veget;	
			dvegtot_veget = 1;
			d_vetot_vegtot1 = 1;

			mx_eau_var = mx_eau_var1 + veget*wmax_veg[Yj]*dpu_cste;
			dmx_eau_veget = wmax_veg[Yj]*dpu_cste;
			dmx_eau_mx_eau_var1 = 1;
			dmx_eau_vegtot = 1;

			//ruu_ch = mx_eau_var / dpu_cste;
		}		

		YJ1I1 = dvegtot_veget;
		YJ1I2 = d_vetot_vegtot1;
		YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 =YJ1I7 =0;		

		YJ2I1 = (1/dpu_cste) * dmx_eau_vegtot * dvegtot_veget + (1/dpu_cste) * dmx_eau_veget ;
		YJ2I2 = (1/dpu_cste) * dmx_eau_vegtot * d_vetot_vegtot1;
		YJ2I3 = (1/dpu_cste) * dmx_eau_mx_eau_var1;
		YJ2I4 = 0;
		YJ2I5 = 0;
		YJ2I6 = 0;
		YJ2I7 = (1/dpu_cste) * dmx_eau_eau;

		YJ3I1 = dmx_eau_vegtot * dvegtot_veget + dmx_eau_veget ;
		YJ3I2 = dmx_eau_vegtot * d_vetot_vegtot1;
		YJ3I3 = dmx_eau_mx_eau_var1;
		YJ3I4 = 0;
		YJ3I5 = 0;
		YJ3I6 = 0;
		YJ3I7 =  dmx_eau_eau;

		//printf("%d(%d,%d) veget %e  vegtot: %e   ruu_ch %e   mx_eau %e  \n",Yt,Yi,Yj,veget,YS1,YS2,YS3);
	}
	else
	{
		YJ1I4 = 1;
		YJ1I2 =YJ1I3 = YJ1I1 = YJ1I5 = YJ1I6 =YJ1I7 =0;
		
		YJ2I5 = 1;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I6 = YJ2I7 = 0;		
				
		YJ3I6 = 1;
		YJ3I1 = YJ3I2 =YJ3I3 = YJ3I4 = YJ3I5 = YJ3I7 =0;
	}	
  }

  //===========================================================================

//************************** FIN DU MODULE init_hydrolc.h *********************














