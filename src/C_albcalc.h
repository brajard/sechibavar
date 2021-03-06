  
/***************************************************************************
                 module  C_albcalc.h  -  description
SALIDA : q-cdrag
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          forward
  forward ( YREAL drysoil_frac) 
  {
	double alb_bare, 
	//sortie
	double albedo;

	if(impaze==1) 
	{
		if(Yi == 0)
			albedo = albedo_scal[0];
		else
			albedo = albedo_scal[1];
	}
	else
	{
		if(alb_bare_model == 1 )
			alb_bare = soilalb_wet[Yi][Yj] + drysoil_frac * (soilalb_dry[Yi][Yj] -  soilalb_wet[Yi][Yj]);
		else
			alb_bare = soilalb_moy[Yi][Yj];
		

		albedo = veget1 * alb_bare;		
	}    

	YS1 = albedo;

  }
	

  //===========================================================================
  //                         backward
  backward (
  {
	
	
  }

  //===========================================================================

//************************** end of MODULE C_albcalc.h ********************************












