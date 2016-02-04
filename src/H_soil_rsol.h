  
/***************************************************************************
                 module  H_soil_dryfrac.h  -  description

outputs:
	YS1 = rsol---> Resistance to bare soil evaporation.
	YS2 = litterhumdiag--->litter humidity;
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL hdry, YREAL veget,YREAL mean_dsg, YREAL rsol_cste, YREAL dpu_cste)
  {
	double temp= 0;

              // rsol=hdry*rsol_cste pour hdry < 1m70
		if(veget > min_sechiba) 
		{
			//YS1 =  ( hdry + 1/pow( 10*(dpu_cste-hdry) + 1e-10, 2 ) ) * rsol_cste;
			temp = dpu_cste-hdry + 1e-10;

			//YS1 =  ( hdry + 1/pow( dpu_cste-hdry + 1e-10, 2 ) ) * rsol_cste;
			YS1 =  ( hdry + 1/(temp*temp)) * rsol_cste;
		}
		else
		  YS1 = -1;
		
		
		/*special case: it has just been raining a few drops. The upper soil
		  reservoir is ridiculously small, but the dry soil height is 0.
		  Don't take it into acco1t.*/
		
		if( ( hdry < min_sechiba ) && ( mean_dsg > min_sechiba ) && ( mean_dsg < 5.E-4 ) )
		  YS2 = 0;
		else
		  YS2 = exp( - hdry/ hcrit_litter );
		
		
  }


  //===========================================================================
  //                           backward
  backward (YREAL hdry, YREAL veget,YREAL mean_dsg, YREAL rsol_cste, YREAL dpu_cste)
  {
	
	
	double temp = 0;
		if(veget > min_sechiba) 
		{
			
			temp = dpu_cste-hdry + 1e-10;

						
			YJ1I1 = rsol_cste + 2*(rsol_cste)/(temp*temp*temp);
			YJ1I2 = YJ1I3 = 0;
			YJ1I4 =  hdry + 1/(temp*temp);	
			YJ1I5 = -2*rsol_cste/(temp*temp*temp) ;

			
		}
		else
		{
			YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = YJ1I5 =0;
		}

	
		if( ( hdry < min_sechiba ) && ( mean_dsg > min_sechiba ) && ( mean_dsg < 5.E-4 ) )
		{
	      		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 =YJ2I5 = 0;
		}
		else
		{
			YJ2I1 = (-1/hcrit_litter) * exp( - hdry/ hcrit_litter );
			YJ2I2 = YJ2I3 = YJ2I4 =YJ2I5 =0;

		}
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_dryfrac.h *********************


