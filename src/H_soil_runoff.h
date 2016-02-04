  
/***************************************************************************
                 module  H_soil_runoff.h  -  description

updating soil moisture

                             -------------------
 ***************************************************************************/

  //==========================================================================
  //                           forward
  forward (YREAL gqsb, YREAL bqsb, YREAL dsg, YREAL dsp, YREAL dss, YREAL zeflux, YREAL ruu_ch, YREAL mx_eau_var,YREAL dpu_cste1)
  {
	//sorties
	double 	bqsb_m = 0;
	double 	dsp_m = 0;
	double 	gqsb_m = 0;
	double 	dsg_m = 0;
	double 	dss_m = 0;
	double	runoff2D = 0;
 
	if( Yt > 1)
	{
		runoff2D =gqsb + bqsb - mx_eau_var;
			if (runoff2D<0) 
				runoff2D = 0;
	  //runoff2D = MAX(gqsb + bqsb - mx_eau_var, 0);

		if (mx_eau_var < (gqsb + bqsb)) 
		{
		    
		    /*1.4.1 Plus de reservoir de surface: le sol est sature d'eau. Le reservoir de surface est inexistant
		    !       Tout est dans le reservoir de fond. Le ruissellement correspond a ce qui deborde.*/
			gqsb_m = 0;
			dsg_m = 0;
			bqsb_m = mx_eau_var;
			dsp_m = 0;
			dss_m = 0;//dsp_m ;

			//printf("(%d,%d  *0\n",Yt,Yj);
			//printf("Me\n");
		}
		else if ((gqsb + bqsb)>= min_sechiba) 
		{

			if (gqsb > (dsg * ruu_ch))
			{
				/* 1.4.2 On agrandit le reservoir de surface car il n y a pas eu ruissellement et toute l'eau du 
				!       reservoir de surface tient dans un reservoir dont la taille est plus importante que l actuel. 
				!       La hauteur de sol sec dans le reservoir de surface est alors nulle*/
				dsg_m = gqsb / ruu_ch;
				dss_m = 0;
				//Le reste ne bouge pas
				gqsb_m = gqsb;
				bqsb_m = bqsb;
				dsp_m = dsp;

				//printf("(%d,%d  ****1\n",Yt,Yj);
			}
			else if (gqsb > min_sechiba )
			{
				/* 1.4.3 L eau tient dans le reservoir de surface tel qu il existe. Calcul de la nouvelle hauteur de sol sec
				        dans la couche de surface.*/
				//dss_m = ((dsg * ruu_ch) - gqsb) / ruu_ch ;			
				dss_m = dsg - gqsb/ruu_ch ;
				//Le reste ne bouge pas
				gqsb_m = gqsb;
				bqsb_m = bqsb;
				dsp_m = dsp;
				dsg_m = dsg;

				//printf("(%d,%d  *******2\n",Yt,Yj);
			}
			else
			{
				/*1.4.4 La quantite d eau du reservoir de surface est negative. Cela revient a enlever cette quantite au 
				!       reservoir profond. Le reservoir de surface est alors vide. (voir aussi 1.4.1)*/
				bqsb_m = bqsb + gqsb;
				dsp_m = dpu_cste1 - bqsb_m / ruu_ch;
				gqsb_m = 0;
				dsg_m = 0;
				dss_m = dsp_m;

			}

		}
		else
		{ 
		    /* 1.4.5 Le reservoir profond est aussi asseche. La quantite d eau a enlever depasse la quantite disponible dans 
		    !       le reservoir profond. Ceci ne devrait jamais arriver plus d'une fois par point. C-a-d une fois la valeur 
		    !       negative atteinte les flux doivent etre nuls. On ne signal que ce cas donc.*/
	    
			bqsb_m = gqsb + bqsb;
			dsp_m = dpu_cste1;
			gqsb_m = 0;
			dsg_m = 0;
			dss_m = dsp_m;
		}
	}
	YS1 = gqsb_m;
	YS2 = dsg_m;
	YS3 = bqsb_m ;
	YS4 = dsp_m ;
	YS5 = dss_m;
	YS6 = runoff2D;
  }
	

  //==========================================================================
  //                           backward
  backward (YREAL gqsb, YREAL bqsb, YREAL dsg, YREAL dsp, YREAL dss, YREAL zeflux, YREAL ruu_ch, YREAL mx_eau_var,YREAL dpu_cste1)
  {	   
	//sorties
	double bqsb_m = 0;
	double runoff2D = 0;

	double d_bqsbm_mx= 0;
	double d_bqsbm_bqsb= 0;
	double d_bqsbm_gqsb= 0;

	double d_gqsbm_gqsb= 0;

	double d_dspm_dsp= 0;
	double d_dspm_bqsbm= 0;
	double d_dspm_ruu= 0;
	double d_dspm_dpu= 0;
	
	double d_dsgm_gqsb= 0;
	double d_dsgm_ruu= 0;
	double d_dsgm_dsg= 0;
	
	double d_dssm_dspm= 0;
	double d_dssm_dsg= 0;
	double d_dssm_ruu= 0;
	double d_dssm_gqsb= 0;

	double d_runoff_gqsb= 0;
	double d_runoff_bqsb= 0;
	double d_runoff_mx=0;


	if( Yt > 1)
	{	
		runoff2D =gqsb + bqsb - mx_eau_var;
		d_runoff_gqsb= 1;
		d_runoff_bqsb= 1;
		d_runoff_mx=-1;

		if (runoff2D < 0) 
		{
			runoff2D = 0;
			d_runoff_gqsb = 0;
			d_runoff_bqsb = 0;
			d_runoff_mx = 0;			
		}
		

		if (mx_eau_var < (gqsb + bqsb)) 
		{	
			bqsb_m = mx_eau_var;  
			d_bqsbm_mx= 1;	
		}
		else if ((gqsb + bqsb)>= min_sechiba) 
		{

			if (gqsb > (dsg * ruu_ch))
			{
				d_dsgm_gqsb= 1/ruu_ch;
				d_dsgm_ruu= -gqsb / pow(ruu_ch,2);
				d_gqsbm_gqsb = 1;
				d_bqsbm_bqsb= 1;
				d_dspm_dsp= 1;							
	
			}
			else if (gqsb > min_sechiba )
			{
				/* 1.4.3 L eau tient dans le reservoir de surface tel qu il existe. Calcul de la nouvelle hauteur de sol sec
				        dans la couche de surface.*/
				d_dssm_dsg= 1;
				d_dssm_ruu= gqsb/pow(ruu_ch,2);
				d_dssm_gqsb= -1/ruu_ch;
		
				//Le reste ne bouge pas
				d_gqsbm_gqsb = 1;
				d_bqsbm_bqsb= 1;
				d_dspm_dsp= 1;
				d_dsgm_dsg= 1;
	
			}
			else
			{
				/*1.4.4 La quantite d eau du reservoir de surface est negative. Cela revient a enlever cette quantite au 
				!       reservoir profond. Le reservoir de surface est alors vide. (voir aussi 1.4.1)*/

				bqsb_m = bqsb + gqsb;

				d_bqsbm_bqsb= 1;
				d_bqsbm_gqsb= 1;

			 	d_dspm_bqsbm= -1/ruu_ch;
				d_dspm_ruu= bqsb_m / pow(ruu_ch,2);
				d_dspm_dpu= 1;

				d_dssm_dspm= 1;					
			}
		}
		else
		{ 
		    /* 1.4.5 Le reservoir profond est aussi asseche. La quantite d eau a enlever depasse la quantite disponible dans 
		    !       le reservoir profond. Ceci ne devrait jamais arriver plus d'une fois par point. C-a-d une fois la valeur 
		    !       negative atteinte les flux doivent etre nuls. On ne signal que ce cas donc.*/
	    		d_bqsbm_bqsb= 1;
			d_bqsbm_gqsb= 1;
			d_dssm_dspm= 1;
			d_dspm_dpu= 1;

		}
	}
	//YS1
	YJ1I1 = d_gqsbm_gqsb ;
	YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I8 =YJ1I9 =0;

	//YS2
	YJ2I1 = d_dsgm_gqsb;
	YJ2I3 = d_dsgm_dsg;
	YJ2I7 = d_dsgm_ruu;
	YJ2I2 =YJ2I4 =YJ2I5 =YJ2I6 =YJ2I8 =YJ2I9 =0;


	//YS3	
	YJ3I1 = d_bqsbm_gqsb;
	YJ3I2 = d_bqsbm_bqsb;
	YJ3I8 = d_bqsbm_mx;
	YJ3I3 =YJ3I4 =YJ3I5 =YJ3I6 =YJ3I7 =YJ3I9 =0;

	//YS4
	YJ4I1 = d_dspm_bqsbm * d_bqsbm_gqsb ;
	YJ4I2 = d_dspm_bqsbm * d_bqsbm_bqsb;
	YJ4I4 = d_dspm_dsp;
	YJ4I7 = d_dspm_ruu;
	YJ4I8 = d_dspm_bqsbm *d_bqsbm_mx;
	YJ4I9 = d_dspm_dpu;
	YJ4I3 =YJ4I5 =YJ4I6 =0;
		
	//YS5
	YJ5I1 = d_dssm_gqsb + d_dssm_dspm * d_dspm_bqsbm * d_bqsbm_gqsb;
	YJ5I2 = d_dssm_dspm * d_dspm_bqsbm * d_bqsbm_bqsb;
	YJ5I3 = d_dssm_dsg;
	YJ5I7 = d_dssm_ruu + d_dssm_dspm * d_dspm_ruu;
	YJ5I9 = d_dssm_dspm * d_dspm_dpu;
	YJ5I4 =YJ5I5 =YJ5I6 =YJ5I8 =0;	

	//YS6
	YJ6I1 = d_runoff_gqsb;
	YJ6I2 = d_runoff_bqsb;
	YJ6I8 = d_runoff_mx;
	YJ6I3 =YJ6I4 =YJ6I5 =YJ6I6 =YJ6I7 =YJ6I9 =0;	
  }

  //==========================================================================

//************************** End of MODULE H_soil_runoff.h *******************

