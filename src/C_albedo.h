  
/***************************************************************************
                 module C_albedo.h  -  description

outputs:
	YS1 = albedo VIS y NIR
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL albedo1, YREAL alb_veget1, YREAL albvis, YREAL albnir)
  {
	double alb_bare = 0;
	double albedo = 0;
	double alb_veget = 0;

	if(Yt > 1)
	{
	      /* If 'alb_bare_model' is set to TRUE,  the soil albedo calculation differentiates between
	       wet and dry soil albedo
	       if 'alb_bare_model' is set to FALSE, the mean of wet and dry soil albedo is used*/
	      // if( alb_bare_model ==1 ) 
		  	//alb_bare = soilalb_wet + drysoil_frac * (soilalb_dry -  soilalb_wet);
	       //else
		 alb_bare = soilalb_moy[0][Yi];
	       
		if(Yj == 0)       
		{
			//Soil albedo is weighed by fraction of bare soil          
		       albedo = veget * alb_bare;
		}
		else
		{ 
			//Mean albedo of grid cell for visible and near-infrared range
			albedo = albedo1 + veget * alb_leaf[Yi][Yj];

			//Mean albedo of vegetation for visible and near-infrared range
			alb_veget = alb_veget1 + veget * alb_leaf[Yi][Yj];

			

			
		}	

			
			
	}
	else
	{
		albedo = 0.13;
		alb_veget = 0.13;
		
	}

	
	
	if(Yj < nvm -1)
	{
		YS1 = albedo;
		YS2 = alb_veget;
	}
	else
	{
		if(Yi == 0)
			YS1 = albvis * albedo;
		if(Yi == 1)
			YS1 = albnir * albedo;

		//printf("%d,%d   YS1 %e\n",Yi,Yj,YS1);
	}


	//printf("%d(%d,%d) veget %e     albedo1 %e     alb %e     albedo: %e\n",Yt,Yi,Yj,veget,YS1,YS2,albedo);
		

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget, YREAL albedo1, YREAL alb_veget1, YREAL albvis, YREAL albnir)
  {
	double alb_bare = 0;
	double albedo = 0;
	double alb_veget = 0;

	double d_albedo_veget =0;
//	double d_albedo_alb = 0 ;
	double d_albedo_alb1 = 0;

	double d_albveget_albv1 = 0;
	double d_albveget_veget = 0;

	if(Yt > 1)
	{
	        alb_bare = soilalb_moy[0][Yi];
	       
		if(Yj == 0)       
		{
			//Soil albedo is weighed by fraction of bare soil          
		        albedo = veget * alb_bare;
			d_albedo_veget = alb_bare;
		}
		else
		{ 
			//Mean albedo of grid cell for visible and near-infrared range
			albedo = albedo1 + veget * alb_leaf[Yj][Yi];
			d_albedo_veget = alb_leaf[Yj][Yi];
			d_albedo_alb1 = 1;
			

			//Mean albedo of vegetation for visible and near-infrared range
			alb_veget = alb_veget1 + veget * alb_leaf[Yj][Yi];
			d_albveget_albv1 = 1;
			d_albveget_veget = alb_leaf[Yj][Yi];

			
		}		
	}
	else
	{
		albedo = 0.13;
		alb_veget = 0.13;
		
	}


	if(Yj < nvm -1)
	{
		YS1 = albedo;
		YS2 = alb_veget;

		YJ1I1 = d_albedo_veget ;
		YJ1I2 = d_albedo_alb1;
		YJ1I3 =YJ1I4 = YJ1I5 =0;

		YJ2I1 = d_albveget_veget ;		
		YJ2I3 = d_albveget_albv1; 
		YJ2I2 = YJ2I4 =YJ2I5 =0;
		

	}
	else
	{
		if(Yi == 0)
		{
			YJ1I1 = albvis * d_albedo_veget;
			YJ1I2 = albvis * d_albedo_alb1;
			YJ1I3 =0;
			YJ1I4 = albedo ;
			YJ1I5 =0;

			YJ2I1 = d_albveget_veget ;		
			YJ2I3 = d_albveget_albv1; 
			YJ2I2 = YJ2I4 =YJ2I5 =0;

		}
		if(Yi == 1)
		{
			YJ1I1 = albnir * d_albedo_veget;
			YJ1I2 = albnir * d_albedo_alb1;
			YJ1I3 =0;
			YJ1I4 = 0 ;
			YJ1I5 = albedo;
		}

		//printf("%d,%d   YS1 %e\n",Yi,Yj,YS1);
	}

	//printf("%d(%d,%d) veget %e     albedo1 %e     alb %e     albedo: %e\n",Yt,Yi,Yj,veget,albedo1,alb,albedo);

	
  }

  //===========================================================================

//************************** End of MODULE C_albedo.h *********************














