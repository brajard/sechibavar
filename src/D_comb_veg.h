  
/***************************************************************************
                 module classe D_comb_veg.h  -  description

-computes vbeta2: Beta for Interception lost
	  vebta : Total beta coefficient 
-Compute part of dew that can be intercepted by leafs (coeff_dew_veg)
-Then its integrated to vbeta2

ouputs: 
	YS1 = vbeta2;
	YS2 = vbeta;

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL toveg, YREAL tosnow, YREAL qsintmax, YREAL lai, YREAL vbeta, YREAL qsatt, YREAL qair, YREAL veget, YREAL vb2,YREAL vbm)
  {
	double coeff_dew_veg;
	double vbeta2;
	double vbeta_m;

	if(Yt > 1)
	{      	

		vbeta2 = vb2;


		if(Yj > 0)
		{
			vbeta_m = vbm;	
		}
		else
		{
			vbeta_m = vbeta;
		}

		
		if (toveg > 0.5)
		{
			if(qsintmax > min_sechiba) 
			{
 
				if ( lai > min_sechiba)
				{
					if(lai > 1.5)
						coeff_dew_veg= dew_veg_coeff[5]*pow(lai,5) - dew_veg_coeff[4]*pow(lai,4) + dew_veg_coeff[3]*pow(lai,3) - dew_veg_coeff[2]*pow(lai,2) + dew_veg_coeff[1]*lai + dew_veg_coeff[6];
					else
						coeff_dew_veg=1;
				}
				else
					coeff_dew_veg=0;
				       
				vbeta2 = coeff_dew_veg*veget;

				
			}
			else
			{
				vbeta2 = 0;
				
			}
			
			if(Yj == 0)		    	
				vbeta_m = vbeta + vbeta2;
			else
				vbeta_m = vbm + vbeta2;


		}
		if(tosnow > 0.5)
		{
			vbeta2 = 0;
			vbeta_m = vbeta ;
		}
		
 		
		if ( (Yj ==0) && (qsatt < qair) ) //In any case there is no interception loss on bare soil.
		{
			vbeta2 = 0;			
						
		}
		
	
	}
	else
	{
		vbeta2 = 0;
		vbeta_m = 0;
	}

		

	YS1 = vbeta2;
	YS2 = vbeta_m;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL toveg, YREAL tosnow, YREAL qsintmax, YREAL lai, YREAL vbeta, YREAL qsatt, YREAL qair, YREAL veget, YREAL vb2,YREAL vbm)
  {
	double coeff_dew_veg;
	double vbeta2;
	double vbeta_m ;

	//derivadas
	double d_coeff_dew_veg_lai= 0;	

	double d_vbeta2_coeff_dew= 0;
	double d_vbeta2_veget= 0;
	double d_vbeta2_lai= 0;
	double d_vbeta2_vb2= 0;

	double d_vbeta_m_vbeta = 0;
	double d_vbeta_m_vbeta2 = 0;
	double d_vbeta_m_vbm = 0;
	double d_vbeta_veget = 0;
	double d_vbeta_lai = 0;

	




	if(Yt > 1)
	{    	
		vbeta2 = vb2;
		d_vbeta2_vb2 = 1;

		if(Yj > 0)
		{
			vbeta_m = vbm;
			d_vbeta_m_vbm = 1;
	
		}
		else
		{
			vbeta_m = vbeta;
			d_vbeta_m_vbeta =1;
		}  

		if (toveg> 0.5)
		{
			d_vbeta2_vb2 = 0;
			if(qsintmax > min_sechiba) 
			{
 
				if ( lai > min_sechiba)
				{
					if(lai > 1.5)	
					{	
						coeff_dew_veg= 0.000017*pow(lai,5) - 0.000824*pow(lai,4) + 0.014843*pow(lai,3) - 0.110112*pow(lai,2) + 0.205673*lai + 0.887773;			
						d_coeff_dew_veg_lai = 5 * 0.000017*pow(lai,4) - 4* 0.000824*pow(lai,3) + 3* 0.014843*pow(lai,2) - 2 * 0.110112*lai + 0.205673;
					}
					else
					{
						coeff_dew_veg=1;
					}
				}
				else
				{
					coeff_dew_veg=0;
				}
				//vbeta2 y sus derivadas
				vbeta2 = coeff_dew_veg*veget;
				d_vbeta2_coeff_dew = veget;
				d_vbeta2_veget = coeff_dew_veg;
				d_vbeta2_lai =  d_vbeta2_coeff_dew * d_coeff_dew_veg_lai;

				//vbeta y sus derivadas
				
				if(Yj == 0)	
				{	    	
					vbeta_m = vbeta + vbeta2;
					d_vbeta_m_vbeta = 1;	
				}
				else
				{
					vbeta_m = vbm + vbeta2;
					d_vbeta_m_vbm = 1;					
				}
				
				d_vbeta_m_vbeta2 = 1;	
				d_vbeta_veget = d_vbeta_m_vbeta2 * d_vbeta2_veget;
				d_vbeta_lai = 	d_vbeta_m_vbeta2 * d_vbeta2_coeff_dew * d_coeff_dew_veg_lai; 	

						

			}
			else
			{	//vbeta2 y sus derivadas
				vbeta2 = 0;
				
				//vbeta y sus derivadas
				vbeta_m = vbeta + vbeta2;
				d_vbeta_m_vbeta = 1;
				d_vbeta_m_vbeta2 = 1;	
				d_vbeta_veget = d_vbeta_m_vbeta2 * d_vbeta2_veget;
				d_vbeta_lai = 	d_vbeta_m_vbeta2 * d_vbeta2_coeff_dew * d_coeff_dew_veg_lai; 		
			    
			}		
		}
		if(tosnow >0.5)
		{
			vbeta2 = 0;
			
			vbeta_m = vbeta ;			
			d_vbeta_m_vbeta = 1;	
			d_vbeta2_vb2 = 0;	
		}


 		//In any case there is no interception loss on bare soil.
		if ( (Yj ==0) && (qsatt < qair) )
		{
			vbeta2 = 0;
			d_vbeta2_vb2 = 0;
	
		}
	
	}

	else
	{
	}


	YJ1I1 = YJ1I2 = YJ1I3 =YJ1I5 =YJ1I6 =YJ1I7 =YJ1I10 = 0;
	YJ1I4 = d_vbeta2_lai;
	YJ1I8 = d_vbeta2_veget;
	YJ1I9 = d_vbeta2_vb2;

	YJ2I1 = YJ2I3 =YJ2I2 = YJ2I6 =YJ2I7 =YJ2I9 = 0;
	YJ2I5 =d_vbeta_m_vbeta;
	YJ2I4 = d_vbeta_lai;
	YJ2I8 = d_vbeta_veget;
	YJ2I10 =d_vbeta_m_vbm;

	
  }

  //===========================================================================

//************************** End of MODULE D_comb_veg.h ********************************














