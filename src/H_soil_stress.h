  
/***************************************************************************
                 module  H_soil_stress.h  -  description

Description:
 	Compute the moisture stress on vegetation and  relative surface humidity
	Only use the standard formulas if total soil moisture is larger than 0.
	else stress functions are set to 0. This will avoid that large negative 
	soil moisture accumulate over time by the the creation of small skin reservoirs which evaporate quickly.


:
	humrel y vegstress---> HYDROL_HUMR = 1

Sorties
	YS1 = humrel
	YS2 = vegstress;
	YS3 = a_subgrd;


                             -------------------
 ***************************************************************************/


  //===========================================================================
  //                           forward
  forward (YREAL gqsb,YREAL dsg,YREAL bqsb,YREAL dsp,YREAL dss, YREAL ruu_ch, YREAL h1,YREAL h2,YREAL h3,YREAL h4,YREAL h5,YREAL h6,YREAL h7,YREAL h8,YREAL h9,YREAL h10,YREAL h11,YREAL h12,YREAL h13 , YREAL dpu_cste)
  {

	double zhumrel_lo, zhumrel_up;

	double a_subgrd = 0;
	double humrel, vegstress;
	double dsg_m = dsg;
	double temp = 0;
	double hum_cste = 0;

	if(Yt > 1)
	{	
		//Choosing the hum_cste corresponding to the current vegetation fraction
		switch(Yj)
		{
			case 0:  { hum_cste = h1; break;}
			case 1:  { hum_cste = h2; break;}
			case 2:  { hum_cste = h3; break;}
			case 3:  { hum_cste = h4; break;}
			case 4:  { hum_cste = h5; break;}
			case 5:  { hum_cste = h6; break;}
			case 6:  { hum_cste = h7; break;}
			case 7:  { hum_cste = h8; break;}
			case 8:  { hum_cste = h9; break;}
			case 9:  { hum_cste = h10; break;}
			case 10: { hum_cste = h11; break;}
			case 11: { hum_cste = h12; break;}
			case 12: { hum_cste = h13; break;}			
		}


		/*Only use the standard formulas if total soil moisture is larger than zero.
         	Else stress functions are set to zero. 
         	This will avoid that large negative soil moisture accumulate over time by the
         	the creation of small skin reservoirs which evaporate quickly  */
		if ( gqsb+bqsb  >  0 ) 
		{
		    
			if ((dsg == 0) || (gqsb == 0)) 
			{
			       //humrel = exp( - hum_cste * dpu_cste * (dsp/dpu_cste) );
				humrel = exp( - hum_cste *dsp );

			       dsg_m = 0;
			       /*!
			       if the dry soil height is larger than the one corresponding
			       to the wilting point, or negative lower soil moisture : humrel is 0.0
			       !*/
			       if (dsp > (dpu_cste - (qwilt / ruu_ch)) || bqsb  < 0) 
				  humrel = 0;

			       //In this case we can take for vegetation growth the same values for humrel and vegstress
			       vegstress = humrel;

			}
			else
			{
				//As we need a slower variable for vegetation growth the stress is computed differently than in humrel.
				//zhumrel_lo = exp( - humcste[Yj] * dsp);
				//zhumrel_up = exp( - humcste[Yj] * dss);
				zhumrel_lo = exp( - hum_cste * dsp);
				zhumrel_up = exp( - hum_cste * dss);

				//a_subgrd=MIN(MAX(dsg-dss,0)/dsg_min,1);
				if ((dsg-dss)>0)
					temp = (dsg-dss)/dsg_min;

				if(temp < 1)
					a_subgrd = temp;
				else
					a_subgrd = 1;		

				humrel=a_subgrd*zhumrel_up+(1-a_subgrd)*zhumrel_lo;
		
				vegstress = zhumrel_lo + zhumrel_up - exp( - hum_cste * dsg ) ;				
			}
		
		}
		else
		{
			humrel = 0;
			vegstress = 0;			
		  
		}
	}
	else
	{	 
		//1 PDT, inicializacion de las variables
		humrel = 0;
		vegstress = 1;
		dsg_m = dsg;

		if( Yj == 0)
		{
			if( gqsb+bqsb > 0 )
			{
		        	if( (dsg != 0)  ||  (gqsb ==0))
				{
					//a_subgrd=MIN(MAX(dsg-dss,0)/dsg_min,1);
					if ((dsg-dss)>0)
						temp = (dsg-dss)/dsg_min;

					if(temp < 1)
						a_subgrd = temp;
					else
						a_subgrd = 1;				

				}					
			}
          	}
		else
			a_subgrd = 0;
	
	}
	YS1 = humrel;
	YS2 = vegstress;
	YS3 = a_subgrd;
	YS4 = dsg_m;

  }
	

  //===========================================================================
  //                           backward
  backward (YREAL gqsb,YREAL dsg,YREAL bqsb,YREAL dsp,YREAL dss, YREAL ruu_ch, YREAL h1,YREAL h2,YREAL h3,YREAL h4,YREAL h5,YREAL h6,YREAL h7,YREAL h8,YREAL h9,YREAL h10,YREAL h11,YREAL h12,YREAL h13 , YREAL dpu_cste)
  {
	double zhumrel_lo, zhumrel_up; 
	//sorties
	double a_subgrd = 0;
	double humrel, vegstress;
	double dsg_m = dsg;
	double temp=0;
	double hum_cste=0;

	double d_hum_h1 = 0;
	double d_hum_h2 = 0;
	double d_hum_h3 = 0;
	double d_hum_h4 = 0;
	double d_hum_h5 = 0;
	double d_hum_h6 = 0;
	double d_hum_h7 = 0;
	double d_hum_h8 = 0;
	double d_hum_h9 = 0;
	double d_hum_h10 = 0;
	double d_hum_h11 = 0;
	double d_hum_h12 = 0;
	double d_hum_h13 = 0;

	//derivees
	double d_zlo_dsp= 0;
	double d_zlo_hum = 0;
	
	double d_zup_dss= 0;
	double d_zup_hum = 0;
	//a_subgrd
	double d_asub_temp= 0;
	//temp
	double d_temp_dss= 0;
	double d_temp_dsg= 0;
	//humrel
	double d_humrel_asub= 0;
	double d_humrel_zup= 0;
	double d_humrel_zlo= 0;
	double d_humrel_dsp= 0;
	double d_humrel_hum = 0;
	//vegstress
	double d_veg_humrel= 0;
	double d_veg_zlo= 0;
	double d_veg_zup= 0;
	double d_veg_dsg= 0;
	double d_veg_hum = 0;
	//dsg
	double d_dsgm_dsg = 1;

	

	if(Yt > 1)
	{
		//Choosing the hum_cste corresponding to the current vegetation fraction
		switch(Yj)
		{
			case 0:  { hum_cste = h1; d_hum_h1 = 1; break;}
			case 1:  { hum_cste = h2; d_hum_h2 = 1; break;}
			case 2:  { hum_cste = h3; d_hum_h3 = 1; break;}
			case 3:  { hum_cste = h4; d_hum_h4 = 1; break;}
			case 4:  { hum_cste = h5; d_hum_h5 = 1; break;}
			case 5:  { hum_cste = h6; d_hum_h6 = 1; break;}
			case 6:  { hum_cste = h7; d_hum_h7 = 1; break;}
			case 7:  { hum_cste = h8; d_hum_h8 = 1; break;}
			case 8:  { hum_cste = h9; d_hum_h9 = 1; break;}
			case 9:  { hum_cste = h10;d_hum_h10 = 1;  break;}
			case 10: { hum_cste = h11; d_hum_h11 = 1; break;}
			case 11: { hum_cste = h12; d_hum_h12 = 1; break;}
			case 12: { hum_cste = h13; d_hum_h13 = 1; break;}			
		}


		/*Only use the standard formulas if total soil moisture is larger than zero.
         	Else stress functions are set to zero. 
         	This will avoid that large negative soil moisture accumulate over time by the
         	the creation of small skin reservoirs which evaporate quickly  */
		if ( gqsb+bqsb  >  0 ) 
		{
		    
			if ((dsg == 0) || (gqsb == 0)) 
			{
				humrel = exp( - hum_cste * dsp );
				d_humrel_dsp= exp( - hum_cste * dsp ) * (- hum_cste);
				d_humrel_hum = exp( - hum_cste * dsp ) * (- dsp);

				dsg_m = 0;
				d_dsgm_dsg = 0;

			       /*!
			       ! if the dry soil height is larger than the one corresponding
			       ! to the wilting point, or negative lower soil moisture : humrel is 0.0
			       !*/
				if (dsp > (dpu_cste - (qwilt / ruu_ch)) || bqsb  < 0) 
				{
					humrel = 0;
				
					d_humrel_dsp= 0;
					d_humrel_hum = 0;
				}

			       //In this case we can take for vegetation growth the same values for humrel and vegstress
				vegstress = humrel;
				
				d_veg_humrel= 1;
		
			}
			else
			{
				//As we need a slower variable for vegetation growth the stress is computed differently than in humrel.
				zhumrel_lo = exp( - hum_cste * dsp);
				zhumrel_up = exp( - hum_cste * dss);
				d_zlo_dsp= exp( - hum_cste* dsp) * ( - hum_cste  );	
				d_zup_dss= exp( - hum_cste * dss) * ( - hum_cste  );
				d_zlo_hum = exp( - hum_cste * dsp) * (-dsp); 
				d_zup_hum = exp( - hum_cste * dss) * (-dss); 
				
				if ((dsg-dss)>0)
				{
					temp = (dsg-dss)/dsg_min;
			
					d_temp_dss= -1/dsg_min;
					d_temp_dsg= 1/dsg_min;
				}

				if(temp < 1)
				{
					a_subgrd = temp;

					d_asub_temp = 1;
				}
				else
					a_subgrd = 1;	


				humrel=a_subgrd*zhumrel_up+(1-a_subgrd)*zhumrel_lo;
				vegstress = zhumrel_lo + zhumrel_up - exp( - hum_cste * dsg ) ;
				d_humrel_asub= zhumrel_up - zhumrel_lo;
				d_humrel_zup= a_subgrd;
				d_humrel_zlo= (1-a_subgrd);
				d_veg_zlo= 1;
				d_veg_zup= 1;
				d_veg_dsg= exp( - hum_cste * dsg ) *  hum_cste;	
				d_veg_hum = exp( - hum_cste * dsg ) *  dsg;		

			}
			
		}
		else
		{
			humrel = 0;
			vegstress = 0;			
		  
		}
	}
	else
	{	 
		humrel = 0;
		vegstress = 1;
		dsg_m = dsg;

		d_dsgm_dsg = 1;

		if( Yj == 0)
		{
			if( gqsb+bqsb > 0 )
			{
		        	if( (dsg != 0)  ||  (gqsb ==0))
				{
					if ((dsg-dss)>0)
					{
						temp = (dsg-dss)/dsg_min;
			
						d_temp_dss= -1/dsg_min;
						d_temp_dsg= 1/dsg_min;
					}

					if(temp < 1)
					{
						a_subgrd = temp;

						d_asub_temp = 1;
					}
					else
						a_subgrd = 1;				

				}					
			}
          	}
		else
			a_subgrd = 0;
	
	}

	//YS1
	YJ1I1 = YJ1I3 = YJ1I6 = 0;
	YJ1I2 = d_humrel_asub * d_asub_temp * d_temp_dsg;
	YJ1I4 = d_humrel_dsp +  d_humrel_zlo * d_zlo_dsp;
	YJ1I5 = d_humrel_asub * d_asub_temp * d_temp_dss + d_humrel_zup * d_zup_dss ;

	YJ1I7 = d_humrel_hum * d_hum_h1 + d_humrel_zlo * d_zlo_hum * d_hum_h1 +  d_humrel_zup * d_zup_hum * d_hum_h1;
	YJ1I8 = d_humrel_hum * d_hum_h2 + d_humrel_zlo * d_zlo_hum * d_hum_h2 +  d_humrel_zup * d_zup_hum * d_hum_h2;
	YJ1I9 = d_humrel_hum * d_hum_h3 + d_humrel_zlo * d_zlo_hum * d_hum_h3 +  d_humrel_zup * d_zup_hum * d_hum_h3;
	YJ1I10 =d_humrel_hum * d_hum_h4 + d_humrel_zlo * d_zlo_hum * d_hum_h4 +  d_humrel_zup * d_zup_hum * d_hum_h4;
	YJ1I11 =d_humrel_hum * d_hum_h5 + d_humrel_zlo * d_zlo_hum * d_hum_h5 +  d_humrel_zup * d_zup_hum * d_hum_h5;
	YJ1I12 =d_humrel_hum * d_hum_h6 + d_humrel_zlo * d_zlo_hum * d_hum_h6 +  d_humrel_zup * d_zup_hum * d_hum_h6;
	YJ1I13 =d_humrel_hum * d_hum_h7 + d_humrel_zlo * d_zlo_hum * d_hum_h7 +  d_humrel_zup * d_zup_hum * d_hum_h7;
	YJ1I14 =d_humrel_hum * d_hum_h8 + d_humrel_zlo * d_zlo_hum * d_hum_h8 +  d_humrel_zup * d_zup_hum * d_hum_h8;
	YJ1I15 =d_humrel_hum * d_hum_h9 + d_humrel_zlo * d_zlo_hum * d_hum_h9 +  d_humrel_zup * d_zup_hum * d_hum_h9;
	YJ1I16 =d_humrel_hum * d_hum_h10 + d_humrel_zlo * d_zlo_hum * d_hum_h10 +  d_humrel_zup * d_zup_hum * d_hum_h10;
	YJ1I17 =d_humrel_hum * d_hum_h11 + d_humrel_zlo * d_zlo_hum * d_hum_h11 +  d_humrel_zup * d_zup_hum * d_hum_h11;
	YJ1I18 =d_humrel_hum * d_hum_h12 + d_humrel_zlo * d_zlo_hum * d_hum_h12 +  d_humrel_zup * d_zup_hum * d_hum_h12;
	YJ1I19 =d_humrel_hum * d_hum_h13 + d_humrel_zlo * d_zlo_hum * d_hum_h13 +  d_humrel_zup * d_zup_hum * d_hum_h13;

	//YS2
	YJ2I1 = YJ2I3 = YJ2I6 =0;
	YJ2I2 =	d_veg_dsg + d_veg_humrel * d_humrel_asub * d_asub_temp * d_temp_dsg;
	YJ2I4 = d_veg_zlo * d_zlo_dsp + d_veg_humrel * YJ1I4;
	YJ2I5 = d_veg_zup * d_zup_dss + d_veg_humrel * YJ1I5; 

	YJ2I7 = d_veg_hum * d_hum_h1 + d_veg_zlo * d_zlo_hum * d_hum_h1 +  d_veg_zup * d_zup_hum * d_hum_h1;
	YJ2I8 = d_veg_hum * d_hum_h2 + d_veg_zlo * d_zlo_hum * d_hum_h2 +  d_veg_zup * d_zup_hum * d_hum_h2;
	YJ2I9 = d_veg_hum * d_hum_h3 + d_veg_zlo * d_zlo_hum * d_hum_h3 +  d_veg_zup * d_zup_hum * d_hum_h3;
	YJ2I10 = d_veg_hum * d_hum_h4 + d_veg_zlo * d_zlo_hum * d_hum_h4 +  d_veg_zup * d_zup_hum * d_hum_h4;
	YJ2I11 = d_veg_hum * d_hum_h5 + d_veg_zlo * d_zlo_hum * d_hum_h5 +  d_veg_zup * d_zup_hum * d_hum_h5;
	YJ2I12 = d_veg_hum * d_hum_h6 + d_veg_zlo * d_zlo_hum * d_hum_h6 +  d_veg_zup * d_zup_hum * d_hum_h6;
	YJ2I13 = d_veg_hum * d_hum_h7 + d_veg_zlo * d_zlo_hum * d_hum_h7 +  d_veg_zup * d_zup_hum * d_hum_h7;
	YJ2I14 = d_veg_hum * d_hum_h8 + d_veg_zlo * d_zlo_hum * d_hum_h8 +  d_veg_zup * d_zup_hum * d_hum_h8;
	YJ2I15 = d_veg_hum * d_hum_h9 + d_veg_zlo * d_zlo_hum * d_hum_h9 +  d_veg_zup * d_zup_hum * d_hum_h9;
	YJ2I16 = d_veg_hum * d_hum_h10 + d_veg_zlo * d_zlo_hum * d_hum_h10 +  d_veg_zup * d_zup_hum * d_hum_h10;
	YJ2I17 = d_veg_hum * d_hum_h11 + d_veg_zlo * d_zlo_hum * d_hum_h11 +  d_veg_zup * d_zup_hum * d_hum_h11;
	YJ2I18 = d_veg_hum * d_hum_h12 + d_veg_zlo * d_zlo_hum * d_hum_h12 +  d_veg_zup * d_zup_hum * d_hum_h12;
	YJ2I9 = d_veg_hum * d_hum_h13 + d_veg_zlo * d_zlo_hum * d_hum_h13 +  d_veg_zup * d_zup_hum * d_hum_h13;

	//YS3
	YJ3I1 = YJ3I3 = YJ3I6 = YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I11 =YJ3I12 =YJ3I13 =YJ3I14 =YJ3I15 =YJ3I16 =YJ3I17 =YJ3I18 =YJ3I19 =0;
	YJ3I2 =	d_asub_temp * d_temp_dsg;
	YJ3I4 = 0;
	YJ3I5 = d_asub_temp * d_temp_dss;
	
	//YS4
	YJ4I1 = YJ4I3 = YJ4I4 = YJ4I5 = YJ4I6 = YJ4I7 =YJ4I8 =YJ4I9 =YJ4I10 =YJ4I11 =YJ4I12 =YJ4I13 =YJ4I14 =YJ4I15 =YJ4I16 =YJ4I17 =YJ4I18 =YJ4I19  = 0;
	YJ4I2 = d_dsgm_dsg;

  }

  //===========================================================================

//************************** End of MODULE H_soil_stress.h ********************************




