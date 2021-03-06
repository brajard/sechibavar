  
/***************************************************************************
                 module  H_snow_melt.h  -  description
        

Descripcion: snow melt only if temperature positive
                     -------------------
outputs:
	YS1=snow_m;       Snow mass (kg/m^2)
	YS2= snow_nobio;  Snowmass on nobio areas (kg/m^2)
	YS3 = tot_melt;   Total melt (kg/m^2)

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL temp_sol_new, YREAL snow, YREAL frac_nobio, YREAL soilcap, YREAL sn )
  {

	double snowmelt ,snowmelt_t;
	double snowmelt_m =0 ;
	double snow_m = snow;
	double snowmelt_tmp;
	double icemelt = 0;
	double snow_nobio =0 ;
	//1.3. snow melt only if temperature positive

	if(Yt == 1)
	{
		YS1 = 0;
		YS2 = 0;
		YS3 = 0;
	}
	else
	{
		if(temp_sol_new > tp_00)
		{

			if(snow > sneige)
			{
		    		//1.3.1.1 enough snow for melting or not

				if( (1 - frac_nobio)*(temp_sol_new - tp_00) * soilcap / chalfu0 < snow)  
				{			       		
					snowmelt = (1 - frac_nobio)*(temp_sol_new - tp_00) * soilcap / chalfu0;       //agregado por mi
					snow_m = snow - snowmelt;	
				}
			    	else 
				{
			       		snowmelt = snow;
			       		snow_m = 0;
				}
			    
			}
			else if (snow >= 0)
			{ 
			   
				//1.3.2 not enough snow
				snowmelt = snow;
				snow_m = 0;
			}
			else
			{  
				//1.3.3 negative snow - now snow melt
				snow_m = 0;
				snowmelt  = 0;
			}
		}
		else
		{
			snow_m = snow;
			snowmelt = 0;	
		}


		//1.4. Ice melt only if there is more than a given mass : maxmass_glacier i.e. only weight melts glaciers !
		if( snow_m > maxmass_glacier )
		{
			snowmelt_m = snowmelt + (snow_m - maxmass_glacier);
			snow_m = maxmass_glacier;
		}
		else
			snowmelt_m = snowmelt;   
	
		
		//2.3. snow melt only for continental ice fraction
		if (temp_sol_new > tp_00) 
		{ 
			//2.3.1 If there is snow on the ice-fraction it can melt 
			snowmelt_tmp = frac_nobio*(temp_sol_new - tp_00) * soilcap / chalfu0;
		 	if( snowmelt_tmp > sn ) {
			  if (sn>0) snowmelt_tmp=sn ;
			  else snowmelt_tmp=0;
			}
			
	    
		 	snowmelt_t = snowmelt_m + snowmelt_tmp;
		 	snow_nobio = sn - snowmelt_tmp;
			
		 
		}
		else
		{
			snow_nobio = 0;          
			snowmelt_t = snowmelt_m;  
		}	 

		//2.4 Ice melt only if there is more than a given mass : maxmass_glacier, i.e. only weight melts glaciers 
		if( snow_nobio >  maxmass_glacier ) 
		{
			icemelt = snow_nobio - maxmass_glacier;
			snow_nobio = maxmass_glacier;
		}
		else
			icemelt = 0;
		
		//4. computes total melt (snow and ice)		
		YS1=snow_m;
		YS2 = snow_nobio ;
		YS3 = icemelt + snowmelt_t;	
	}		
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL temp_sol_new, YREAL snow, YREAL frac_nobio, YREAL soilcap, YREAL sn )
  {
	double snowmelt ,snowmelt_t;
	double snowmelt_m;
	double snow_m = snow;
	double snowmelt_tmp;
	double icemelt;
	double snow_nobio=0;
	//1.3. snow melt only if temperature positive

	//derivadas
	double dsnowmelt_frac_nobio;
	double dsnowmelt_tempsol =0;
	double dsnowmelt_soilcap = 0;	
	double dsnowmelt_snow;

	double dsnow_m_snow;
	double dsnow_m_snowmelt;

	double dsnowmelt_m_snowmelt;
	double dsnowmelt_m_snow_m;

	double dsnowmelt_tmp_frac_nobio;
	double dsnowmelt_tmp_tempsol=0;
	double dsnowmelt_tmp_soilcap=0;	
	double dsnowmelt_tmp_snow;
	double dsnowmelt_tmp_sn;

	double dsnowmelt_t_snowmelt_tmp;
	double dsnowmelt_t_snowmelt_m;	
	
	double dsnow_nobio_sn;
	double dsnow_nobio_snowmelt_tmp;

	double dicemelt_snownobio;

	if(Yt == 1)
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 = YJ1I6 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = YJ2I5 = YJ2I6 = 0;
		YJ3I1 = YJ3I2 = YJ3I3 = YJ3I4 = YJ3I5 = YJ3I6 = 0;
	}
	else
	{
		if(temp_sol_new > tp_00)
		{

			if(snow > sneige)
			{
		    		//1.3.1.1 enough snow for melting or not

				if( (1 - frac_nobio)*(temp_sol_new - tp_00) * soilcap / chalfu0 < snow)  
				{
			       		snowmelt = (1 - frac_nobio)*(temp_sol_new - tp_00) * soilcap / chalfu0;       
					dsnowmelt_frac_nobio = - (temp_sol_new - tp_00) * soilcap / chalfu0;  ;
					dsnowmelt_tempsol = (1 - frac_nobio)* soilcap / chalfu0;
					dsnowmelt_soilcap = (1 - frac_nobio)*(temp_sol_new - tp_00) / chalfu0;
					dsnowmelt_snow = 0;

					snow_m = snow - snowmelt;
					dsnow_m_snow = 1;
					dsnow_m_snowmelt = -1;
				}
			    	else 
				{
			       		snowmelt = snow;		
					dsnowmelt_frac_nobio = dsnowmelt_tempsol = dsnowmelt_soilcap = 0;
					dsnowmelt_snow = 1;

			       		snow_m = 0;
					dsnow_m_snow = dsnow_m_snowmelt = 0;
				}
			    
			}
			else if (snow >= 0)
			{ 			   
				//1.3.2 not enough snow
				snowmelt = snow;
				dsnowmelt_frac_nobio = 0;
				dsnowmelt_tempsol = 0;
				dsnowmelt_soilcap = 0;
				dsnowmelt_snow = 1;

			       	snow_m = 0;
				dsnow_m_snow = dsnow_m_snowmelt = 0;
			}
			else
			{  
				//1.3.3 negative snow - now snow melt
				snowmelt = 0;
				dsnowmelt_frac_nobio = dsnowmelt_tempsol = dsnowmelt_soilcap = dsnowmelt_snow = 0;

			       	snow_m = 0;
				dsnow_m_snow = dsnow_m_snowmelt = 0;
			}
		}
		else
		{	
			snow_m = snow;
			snowmelt = 0;

			dsnowmelt_frac_nobio = 0;
			dsnowmelt_tempsol= 0;
			dsnowmelt_soilcap= 0;	
			dsnowmelt_snow= 0;

			dsnow_m_snow = 1;
			dsnow_m_snowmelt = 0;	

		}

		//1.4. Ice melt only if there is more than a given mass : maxmass_glacier i.e. only weight melts glaciers !
		if( snow_m > maxmass_glacier )
		{
			snowmelt_m = snowmelt + (snow_m - maxmass_glacier);
			dsnowmelt_m_snowmelt = 1;
			dsnowmelt_m_snow_m = 1;			
			
			snow_m = maxmass_glacier;
			dsnow_m_snow = dsnow_m_snowmelt = 0;
		}
		else
		{
			snowmelt_m = snowmelt;   
			dsnowmelt_m_snowmelt = 1;
			dsnowmelt_m_snow_m = 0;	

		}
	      
		//2.3. snow melt only for continental ice fraction
		if (temp_sol_new > tp_00) 
		{ 
			//2.3.1 If there is snow on the ice-fraction it can melt 
			snowmelt_tmp = frac_nobio*(temp_sol_new - tp_00) * soilcap / chalfu0;
			dsnowmelt_tmp_frac_nobio = (temp_sol_new - tp_00) * soilcap / chalfu0;;
			dsnowmelt_tmp_tempsol = frac_nobio*soilcap / chalfu0;;
			dsnowmelt_tmp_soilcap = frac_nobio*(temp_sol_new - tp_00) / chalfu0;;	
			dsnowmelt_tmp_snow = 0;

		 	if( snowmelt_tmp > sn ) 
			{
		     		if( sn>0 )
				{
					snowmelt_tmp = sn;
					dsnowmelt_tmp_frac_nobio = dsnowmelt_tmp_tempsol = dsnowmelt_tmp_soilcap = dsnowmelt_tmp_snow = 0;
					dsnowmelt_tmp_sn = 1;
				}
				else
				{
					snowmelt_tmp = dsnowmelt_tmp_frac_nobio = dsnowmelt_tmp_tempsol =0;
					dsnowmelt_tmp_soilcap = dsnowmelt_tmp_snow = dsnowmelt_tmp_sn =0;
				}
			}
	    
		 	snowmelt_t = snowmelt_m + snowmelt_tmp;		 	
			dsnowmelt_t_snowmelt_tmp = dsnowmelt_t_snowmelt_m = 1;
		
			snow_nobio = sn - snowmelt_tmp;
			dsnow_nobio_sn = 1;
			dsnow_nobio_snowmelt_tmp = -1;
		 
		}
		else
		{			
			dsnow_nobio_sn = 0;
			dsnow_nobio_snowmelt_tmp = 0;

			snowmelt_t = snowmelt_m;  
			dsnowmelt_t_snowmelt_tmp = 0;
			dsnowmelt_t_snowmelt_m = 1;
		}	 

		//2.4 Ice melt only if there is more than a given mass : maxmass_glacier, i.e. only weight melts glaciers 

		if( snow_nobio >  maxmass_glacier ) 
		{
			icemelt = snow_nobio - maxmass_glacier;
			dicemelt_snownobio = 1;

			snow_nobio = maxmass_glacier;
			dsnow_nobio_sn = dsnow_nobio_snowmelt_tmp =  0;
		}
		else
		{
			icemelt = 0;
			dicemelt_snownobio = 0;
		}
		
		//4. computes total melt (snow and ice)

		YJ1I1 = dsnow_m_snowmelt * dsnowmelt_tempsol;
		YJ1I2 = dsnow_m_snow + dsnow_m_snowmelt * dsnowmelt_snow ;
		YJ1I3 = dsnow_m_snowmelt * dsnowmelt_frac_nobio;
		YJ1I4 = dsnow_m_snowmelt * dsnowmelt_soilcap;
		YJ1I5 = 0;

		YJ2I1 = dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_tempsol;
		YJ2I2 = dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_snow;
		YJ2I3 = 0;
		YJ2I4 = dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_soilcap;
		YJ2I5 = 1;


		YJ3I1 = dicemelt_snownobio * dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_tempsol + dsnowmelt_t_snowmelt_tmp * dsnowmelt_tmp_tempsol + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snowmelt * dsnowmelt_tempsol + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snow_m * dsnow_m_snowmelt * dsnowmelt_tempsol;
		YJ3I2 = dicemelt_snownobio * dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_snow + dsnowmelt_t_snowmelt_tmp * dsnowmelt_tmp_snow + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snowmelt * dsnowmelt_snow + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snow_m * dsnow_m_snowmelt * dsnowmelt_snow + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snow_m * dsnow_m_snow;
		YJ3I3 = 0;
		YJ3I4 = dicemelt_snownobio * dsnow_nobio_snowmelt_tmp * dsnowmelt_tmp_soilcap + dsnowmelt_t_snowmelt_tmp * dsnowmelt_tmp_soilcap + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snowmelt * dsnowmelt_soilcap + dsnowmelt_t_snowmelt_m * dsnowmelt_m_snow_m * dsnow_m_snowmelt * dsnowmelt_soilcap;
		YJ3I5 = 0;
	
	}
	
  }

  //===========================================================================

//************************** End of MODULE H_snow_melt.h ********************************

