  
/***************************************************************************
                 module classe E_evap_veva.h  -  description
                             -------------------
//computes
	YS1 = vevapsno; Snow evaporation
	YS2 = vevapnu; Bare soil evaporation
	YS3 = xx; preliminar variable to compute transpir (transpiration) and vevapwet (interception)
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ( YREAL speed , YREAL vbeta1, YREAL vbeta4, YREAL rau, YREAL q_cdrag, YREAL qsol_sat_new, YREAL qair) 
  {
     	if(Yt>1)
	{
		// snow sublimation
		YS1 = vbeta1 * dtradia * rau * speed * q_cdrag * (qsol_sat_new - qair) ;

		// bare soil evaporation
		YS2 = (1 - vbeta1) * vbeta4 * dtradia  * rau * speed * q_cdrag * (qsol_sat_new - qair);

	
		YS3 = dtradia * (1-vbeta1) * (qsol_sat_new-qair) * rau * speed * q_cdrag;
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
	}

  }
	

  //===========================================================================
  //                           backward
  backward ( YREAL speed , YREAL vbeta1, YREAL vbeta4, YREAL rau, YREAL q_cdrag, YREAL qsol_sat_new, YREAL qair)
  {
	if(Yt>1)
	{
		// snow sublimation
		YJ1I1 = vbeta1 * dtradia * rau * q_cdrag * (qsol_sat_new - qair) ;
		YJ1I2 = dtradia * rau * speed * q_cdrag * (qsol_sat_new - qair) ;
		YJ1I3 = 0;  
		YJ1I4 = vbeta1 * dtradia * speed * q_cdrag * (qsol_sat_new - qair) ;
		YJ1I5 = vbeta1 * dtradia * rau * speed * (qsol_sat_new - qair) ;
		YJ1I6 = vbeta1 * dtradia * rau * speed * q_cdrag ; 
  		YJ1I7 = -vbeta1 * dtradia * rau * speed * q_cdrag ;

		// bare soil evaporation
		YJ2I1 = (1 - vbeta1) * vbeta4 * dtradia  * rau * q_cdrag * (qsol_sat_new - qair);
		YJ2I2 = - vbeta4 * dtradia  * rau * speed * q_cdrag * (qsol_sat_new - qair);
		YJ2I3 = (1 - vbeta1) * dtradia  * rau * speed * q_cdrag * (qsol_sat_new - qair); 
		YJ2I4 = (1 - vbeta1) * vbeta4 * dtradia * speed * q_cdrag * (qsol_sat_new - qair);
		YJ2I5 = (1 - vbeta1) * vbeta4 * dtradia  * rau * speed * (qsol_sat_new - qair);
		YJ2I6 = (1 - vbeta1) * vbeta4 * dtradia  * rau * speed * q_cdrag;
  		YJ2I7 = -(1 - vbeta1) * vbeta4 * dtradia  * rau * speed * q_cdrag ; 

		YJ3I1 = dtradia * (1-vbeta1) * (qsol_sat_new-qair) * rau * q_cdrag;
		YJ3I2 = -dtradia * (qsol_sat_new-qair) * rau * speed * q_cdrag;
		YJ3I3 = 0;  
		YJ3I4 = dtradia * (1-vbeta1) * (qsol_sat_new-qair) * speed * q_cdrag;
		YJ3I5 = dtradia * (1-vbeta1) * (qsol_sat_new-qair) * rau * speed;
		YJ3I6 = dtradia * (1-vbeta1) * rau * speed * q_cdrag; 
  		YJ3I7 = -dtradia * (1-vbeta1) * rau * speed * q_cdrag;
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;    
		YJ1I4 = 0;
		YJ1I5 = 0;
		YJ1I6 = 0; 
  		YJ1I7 = 0; 

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;  
		YJ2I4 = 0;
		YJ2I5 = 0;
		YJ2I6 = 0; 
  		YJ2I7 = 0; 

		YJ3I1 = 0;
		YJ3I2 = 0;
		YJ3I3 = 0;  
		YJ3I4 = 0;
		YJ3I5 = 0;
		YJ3I6 = 0; 
  		YJ3I7 = 0; 
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_evap_veva.h ********************************


