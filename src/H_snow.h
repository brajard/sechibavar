  
/***************************************************************************
                 module classe H_snow.h  -  description
                             -------------------

Description: In this routine the snow-related processes accumulation, sublimation, melting and ageing are
		computed separately on vegetated and nobio areas

output: YS1 = snow
	YS2 = subsnownobio;
	YS3 = vevapnu

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL snow, YREAL frac_nobio,YREAL vevapsno, YREAL vevapnu)
  {

	double subsnownobio = 0 , subsnowveg = 0;

	//1. On vegetation
	//1.1. It is snowing
	//snow = snow1 + (1 - totfrac_nobio)*precip_snow;

	//1.2. Sublimation - separate between vegetated and no-veget fractions Care has to be taken as we might have sublimation from the
	//the frac_nobio while there is no snow on the rest of the grid.
      
	if ( snow > snowcri )
	{
		subsnownobio = frac_nobio*vevapsno;
		subsnowveg = vevapsno - subsnownobio;
		
	}
      	else
	{
        
		if( frac_nobio > min_sechiba) 
		{
			subsnownobio = vevapsno;
			subsnowveg = 0;
		}
         	else
		{ 
			subsnownobio = 0;
			subsnowveg = vevapsno;
		}         
      	}
    
	//1.2.1 Check that sublimation on the vegetated fraction is possible.

	if(subsnowveg > snow) //What could not be sublimated goes into bare soil evaporation
	{
   		

		if( frac_nobio > min_sechiba) 
		{			
			YS2 = subsnownobio + (subsnowveg - snow);			
			YS3 = vevapnu;
		}
		else  
		{			
			YS2 = subsnownobio; 
			YS3 = vevapnu + (subsnowveg - snow);
		}

		YS1 = 0;
	
	}
	else 
	{
        	YS1 = snow - subsnowveg;
		YS2 = subsnownobio;
		YS3 = vevapnu;
		
	}
	

	
	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL snow, YREAL frac_nobio,YREAL vevapsno, YREAL vevapnu)
  {

	double subsnownobio = 0 , subsnowveg = 0;

	//derivadas
	double d_subsnownobio_frac_nobio;
	double d_subsnownobio_vevapsno;

	double d_subsnowveg_vevapsno;
	double d_subsnowveg_subsnownobio;

	double d_YS2_subsnownobio;
	double d_YS2_subsnowveg;
	double d_YS3_subsnowveg;


	if ( snow > snowcri )
	{
		subsnownobio = frac_nobio*vevapsno;
		subsnowveg = vevapsno - subsnownobio;

		d_subsnownobio_frac_nobio = vevapsno;
		d_subsnownobio_vevapsno = frac_nobio;

		d_subsnowveg_vevapsno = 1;
		d_subsnowveg_subsnownobio = -1;
		
	}
      	else
	{
        	if( frac_nobio > min_sechiba) 
		{
			subsnownobio = vevapsno;
			subsnowveg = 0;

			d_subsnownobio_frac_nobio = 0;
			d_subsnownobio_vevapsno = 1;

			d_subsnowveg_vevapsno = 0;
			d_subsnowveg_subsnownobio = 0;
		}
         	else
		{ 
			subsnownobio = 0;
			subsnowveg = vevapsno;

			d_subsnownobio_frac_nobio = 0;
			d_subsnownobio_vevapsno = 0;

			d_subsnowveg_vevapsno = 1;
			d_subsnowveg_subsnownobio = 0;
		}         
      	}
	
	if(subsnowveg > snow) //What could not be sublimated goes into bare soil evaporation
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = 0;

		if( frac_nobio > min_sechiba) 
		{					

			d_YS2_subsnownobio = 1;
			d_YS2_subsnowveg = 1;
			d_YS3_subsnowveg = 0;
			
			YJ2I1 = -1; 
			YJ2I2 = d_YS2_subsnownobio * d_subsnownobio_frac_nobio + d_YS2_subsnowveg * d_subsnowveg_subsnownobio * d_subsnownobio_frac_nobio; 
			YJ2I3 = d_YS2_subsnownobio * d_subsnownobio_vevapsno + d_YS2_subsnowveg * d_subsnowveg_subsnownobio * d_subsnownobio_vevapsno +  d_YS2_subsnowveg *d_subsnowveg_vevapsno;
			YJ2I4 = 0 ;

			YJ3I1 = YJ3I2 = YJ3I3 = 0;			
			YJ3I4 = 1;
		}
		else  
		{			
			d_YS2_subsnownobio = 1;
			d_YS2_subsnowveg = 0;
			d_YS3_subsnowveg = 1;

			YJ2I1 = 0; 
			YJ2I2 = d_YS2_subsnownobio * d_subsnownobio_frac_nobio;
			YJ2I3 = d_YS2_subsnownobio * d_subsnownobio_vevapsno;
			YJ2I4 = 0 ;

			YJ3I1 = -1;
			YJ3I2 = d_YS3_subsnowveg * d_subsnowveg_subsnownobio * d_subsnownobio_frac_nobio ;
			YJ3I3 = d_YS3_subsnowveg * d_subsnowveg_subsnownobio * d_subsnownobio_vevapsno + d_YS3_subsnowveg * d_subsnowveg_vevapsno ;
			YJ3I4 = 1;
			

		}

	}
	else 
	{
		YJ1I1 = 1;
		YJ1I2 = 1 * d_subsnowveg_subsnownobio * d_subsnownobio_frac_nobio;
		YJ1I3 = -1 * d_subsnowveg_subsnownobio * d_subsnownobio_vevapsno;
		YJ1I4 = 0;

		YJ2I1 = 0;
		YJ2I2 = 1 * d_subsnownobio_frac_nobio;
		YJ2I3 = 1 * d_subsnownobio_vevapsno;
		YJ2I4 = 0;

		YJ3I1 = YJ3I2 = 0;
		YJ3I3 = 1;
		YJ3I4 = 1;
		
	}
		
	
  }

  //===========================================================================

//************************** End of MODULE H_snow.h ********************************














