  
/***************************************************************************
                 module  H_soil_trans.h  -  description

Transpiration for each vegetation type

output:
	YS1 = gqsb_m--->water height in the surface reservoir
	YS2 = bqsb_m--->water height in the deep reservoir
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL gqsb,YREAL bqsb,YREAL zeflux,YREAL veget,YREAL zpreci,YREAL irrigation,YREAL returnflow,YREAL tot_melt, YREAL vegtot)
  {
	double temp1; 		
	double temp2; 		
	double temp3 = bqsb; 	

	double gqsb_m = 0, bqsb_m = 0;

	if(Yt > 1)
	{
		//Evaporated water is taken out of the ground.   
		temp1 = gqsb - zeflux;
		
		//Add snow and ice melt, troughfall from vegetation and irrigation.

		if(vegtot != 0)
		{
			//snow and ice melt and troughfall from vegetation
			temp2 = temp1 + zpreci  + tot_melt/vegtot;
		  
			//We take care to add the irrigation only to the vegetated part if possible
			if(abs(vegtot-veget) <= min_sechiba) 
				gqsb_m = temp2 + irrigation/vegtot;
		   	else
			{
				if( Yj > 0 )
				{
			
					//Only add the irrigation to the upper soil if there is a reservoir. Without this the water evaporates right away.
				 	if( temp2 > 0 ) 
				   		gqsb_m = temp2 + irrigation/(vegtot-veget);
				 	else
					{
				    		temp3 = bqsb + irrigation/(vegtot-veget) ;                   	
						gqsb_m = temp2;
					}
		        	}
				else
					gqsb_m = temp2;
			}
		        //We add the water returned from rivers to the lower reservoir.
		   	bqsb_m = temp3 + returnflow/vegtot;
		   
		}
		else
		{
			gqsb_m = temp1;
			bqsb_m = bqsb;
		}

	}

	YS1 = gqsb_m;
	YS2 = bqsb_m;

	
  }
	

  //===========================================================================
  //                           backward
  backward(YREAL gqsb,YREAL bqsb,YREAL zeflux,YREAL veget,YREAL zpreci,YREAL irrigation,YREAL returnflow,YREAL tot_melt, YREAL vegtot)	 
  {
	double temp1; 		
	double temp2; 		
	double temp3 = bqsb; 	
	double gqsb_m = 0, bqsb_m = 0;

	double d_temp1_gqsb= 0;
	double d_temp1_zeflux= 0;

	double d_temp2_temp1= 0;
	double d_temp2_zpreci= 0;
	double d_temp2_totmelt= 0;
	double d_temp2_vegtot= 0;

	double d_temp3_bqsb= 1;
	double d_temp3_irrigation= 0;
	double d_temp3_vegtot= 0;
	double d_temp3_veget= 0;

	double d_gqsbm_temp2= 0;
	double d_gqsbm_irrigation= 0;
	double d_gqsbm_vegtot= 0;
	double d_gqsbm_veget= 0;
	double d_gqsbm_temp1= 0;

	double d_bqsbm_temp3= 0;
	double d_bqsbm_return= 0;
	double d_bqsbm_vegtot= 0;
	double d_bqsbm_bqsb= 0;


	//sorties
	

	if(Yt > 1)
	{
		//Evaporated water is taken out of the ground.   
		temp1 = gqsb - zeflux;

		d_temp1_gqsb= 1;
		d_temp1_zeflux = -1;
		
		//Add snow and ice melt, troughfall from vegetation and irrigation.

		if(vegtot != 0)
		{
			//snow and ice melt and troughfall from vegetation
			temp2 = temp1 + zpreci  + tot_melt/vegtot;
	
			d_temp2_temp1= 1;
			d_temp2_zpreci= 1;
			d_temp2_totmelt= 1/vegtot;
			d_temp2_vegtot = -tot_melt/pow(vegtot,2);
		  
			//We take care to add the irrigation only to the vegetated part if possible
			if(abs(vegtot-veget) <= min_sechiba) 
			{
				gqsb_m = temp2 + irrigation/vegtot;
				
				d_gqsbm_temp2= 1;
				d_gqsbm_irrigation= 1/vegtot;
				d_gqsbm_vegtot= -irrigation/vegtot;
			}
		   	else
			{
				if( Yj > 0 )
				{
			
					//Only add the irrigation to the upper soil if there is a reservoir. Without this the water evaporates right away.
				 	if( temp2 > 0 ) 
					{
				   		gqsb_m = temp2 + irrigation/(vegtot-veget);

						d_gqsbm_temp2= 1;
						d_gqsbm_irrigation= 1/(vegtot-veget);
						d_gqsbm_vegtot= -irrigation/pow(vegtot-veget,2);
						d_gqsbm_veget= irrigation/pow(vegtot-veget,2);
					}
				 	else
					{
				    		temp3 = bqsb + irrigation/(vegtot-veget) ;                   	
						gqsb_m = temp2;
						
						d_temp3_bqsb= 1;
						d_temp3_irrigation= 1/(vegtot-veget);
						d_temp3_vegtot= -irrigation/pow(vegtot-veget,2);
						d_temp3_veget= irrigation/pow(vegtot-veget,2);

						d_gqsbm_temp2= 1;
					}
		        	}
				else
				{
					gqsb_m = temp2;

					d_gqsbm_temp2= 1;
				}
				
			}
		   

			//We add the water returned from rivers to the lower reservoir.
		   	bqsb_m = temp3 + returnflow/vegtot;

			d_bqsbm_temp3= 1;
			d_bqsbm_return= 1/vegtot;
			d_bqsbm_vegtot= -returnflow/pow(vegtot,2);

		   
		}
		else
		{
			gqsb_m = temp1;
			bqsb_m = bqsb;
			
			d_gqsbm_temp1= 1;			
			d_bqsbm_bqsb= 1;

		}

	}

	//sorties

	YJ1I1 = d_gqsbm_temp1 * d_temp1_gqsb + d_gqsbm_temp2 * d_temp2_temp1 *  d_temp1_gqsb;
	YJ1I3 = d_gqsbm_temp1 * d_temp1_zeflux + d_gqsbm_temp2 * d_temp2_temp1 *  d_temp1_zeflux;
	YJ1I4 = d_gqsbm_veget;
	YJ1I5 = d_gqsbm_temp2 * d_temp2_zpreci;
	YJ1I6 = d_gqsbm_irrigation;
	YJ1I8 = d_gqsbm_temp2 * d_temp2_totmelt;
	YJ1I9 = d_gqsbm_temp2 * d_temp2_vegtot;
	YJ1I2 = YJ1I7 =0;
	
	YJ2I1 = 0;
	YJ2I2 = d_bqsbm_bqsb + d_bqsbm_temp3 * d_temp3_bqsb;
	YJ2I3 = 0;
	YJ2I4 = d_bqsbm_temp3 * d_temp3_veget;
	YJ2I5 = 0;
	YJ2I6 = d_bqsbm_temp3 * d_temp3_irrigation;
 	YJ2I7 = d_bqsbm_return;
	YJ2I8 = 0;	
	YJ2I9 = d_bqsbm_vegtot + d_bqsbm_temp3 * d_temp3_vegtot;
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_trans.h *********************


