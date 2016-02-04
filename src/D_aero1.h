  
/***************************************************************************
                 module classe D_aero1.h  -  description

Computes the surface drag coefficient, for cases in which it is NOT provided by the coupled
atmospheric model LMDZ. The module first uses the meteorolgical input to calculate the Richardson
Number, which is an indicator of atmospheric stability in the surface layer. The formulation used
to find this surface drag coefficient is dependent on the stability determined. 

 geopotentiel  : zlev * cte_grav

q_cdrag : AERODYNAMIC TRANSFERT COEEFICIENT (Surface drag) 

Output : q-cdrag
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL wind, YREAL temp_sol, YREAL temp_air, YREAL qsurf, YREAL qair)
  {
	double ztvd, ztvs, zdu2;

	if(Yt > 1)
        {

		
		//! 3. virtual air temperature at the surface        
		ztvd = (temp_air + (zlev * cte_grav /cp_air) / (1 + rvtmp2 * qair)) * (1 + retv * qair);

		//! 4. virtual surface temperature
		ztvs = temp_sol * (1 + retv * qsurf);
		
		//! 5. squared wind shear   zdu2 = MAX(cepdu2,pow(wind,2));  
		if(pow(wind,2)> cepdu2)   
		{
			zdu2 = pow(wind,2);
		}
		else
		{
			zdu2 = cepdu2;		
		}		
		
		//! 6. Richardson number
		if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) < 5) 
		{
			if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) > -5)
				YS1 = zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd); // geopotentiel  : zlev * cte_grav
			else
				YS1 = -5;


		
		}		
		else 
		{

		}


	}
	else
		YS1 = 0;
  }


 backward (YREAL wind, YREAL temp_sol, YREAL temp_air, YREAL qsurf, YREAL qair)
  {
	double ztvd, ztvs, zdu2;

	double d_ztvd_temp_air=0;
	//double d_ztvd_zlev=0;
	double d_ztvd_qair=0;

	double d_ztvs_temp_sol=0;
	double d_ztvs_qsurf=0;

	double d_zdu2_wind=0;

	//double d_YS1_zlev=0;
	double d_YS1_ztvd=0;
	double d_YS1_ztvs=0;
	double d_YS1_zdu2=0;


	if(Yt > 1)
        {
		
		
		//! 3. virtual air temperature at the surface        
		ztvd = (temp_air + (zlev * cte_grav /cp_air) / (1 + rvtmp2 * qair)) * (1 + retv * qair);
		d_ztvd_temp_air= (1 + retv * qair) ;
		
		d_ztvd_qair = cte_grav*zlev*(retv-rvtmp2)/(cp_air*pow(1 + rvtmp2 * qair,2))  + retv*temp_air ;    

		//! 4. virtual surface temperature
		ztvs = temp_sol * (1 + retv * qsurf);
		d_ztvs_temp_sol = (1 + retv * qsurf);
	 	d_ztvs_qsurf = temp_sol * retv;

		//! 5. squared wind shear   zdu2 = MAX(cepdu2,pow(wind,2));  
		if(pow(wind,2)> cepdu2)   
		{
			zdu2 = pow(wind,2);
			d_zdu2_wind = 2 * wind;
		}
		else
		{
			zdu2 = cepdu2;		
		}		
		
		//! 6. Richardson number
		
		if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) < 5) 
		{
			if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) > -5)
			{
				
				d_YS1_ztvd = ztvs*cte_grav*zlev/(zdu2*ztvd*ztvd);      
					
				d_YS1_ztvs = -zlev * cte_grav /(zdu2 * ztvd);
				d_YS1_zdu2 =  -(zlev * cte_grav * (ztvd - ztvs) / ztvd) * (1/(zdu2*zdu2));  


				
				YJ1I1 = d_YS1_zdu2 * d_zdu2_wind;
				YJ1I2 = d_YS1_ztvs * d_ztvs_temp_sol ;
				YJ1I3 = d_YS1_ztvd * d_ztvd_temp_air;
				YJ1I4 = d_YS1_ztvs * d_ztvs_qsurf;	
				YJ1I5 = d_YS1_ztvd * d_ztvd_qair;


			}
			else
				YJ1I1 =YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =  0;
		}		
		else 
		{
			YJ1I1 =YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =  0;
		}


	}
	else
		YJ1I1 =YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =  0;
  }

  //===========================================================================

//************************** End of MODULE D_aero1.h ********************************


