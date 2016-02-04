  
/***************************************************************************
                 module  H_soil_runofft.h  -  description

computes total runoff2D and estimates some drainage
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL tot_melt, YREAL irrigation,YREAL run_off_tot1,YREAL runoff2D,YREAL veget, YREAL vegtot)
  {

	//sorties
	double drainage = 0;
	double runtemp = 0;

	if(Yt > 1)
	{
		if(Yj==0)
		{

			if( vegtot > 0 ) 
				runtemp = runoff2D*veget;
		      	else
				runtemp  = tot_melt + irrigation;
		}
		else
		{
			if( vegtot > 0 ) 
				runtemp = run_off_tot1 + (runoff2D*veget);

		      	else
				runtemp = tot_melt + irrigation;	    	
		}

		drainage = 0.95 * runtemp  ;

		if(Yj == nvm -1)
		{
			YS1 = (runtemp - drainage); 
		}
		else
		{	
			YS1 = runtemp;
		}
		
		YS2 = drainage;	

	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}	
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL tot_melt, YREAL irrigation,YREAL run_off_tot1,YREAL runoff2D,YREAL veget, YREAL vegtot)
  {
	//sorties
	double d_drainage_runtemp= 0;
	
	double d_runtemp_runoff= 0;
	double d_runtemp_veget= 0;
	double d_runtemp_totmelt = 0;
	double d_runtemp_irri= 0;
	double d_runtemp_runoff1= 0;
	
	double d_YS1_drainage = 0;

	if(Yt > 1)
	{
		if(Yj==0)
		{

			if( vegtot > 0 ) 
			{
				d_runtemp_runoff=veget;
				d_runtemp_veget=runoff2D;			
			}
		      	else
			{
				d_runtemp_totmelt = 1;
				d_runtemp_irri= 1;
			}
		}
		else
		{
			if( vegtot > 0 ) 
			{
				d_runtemp_runoff=veget;
				d_runtemp_veget= runoff2D;
				d_runtemp_runoff1=1;
			}
		      	else
			{
				d_runtemp_totmelt = 1;
				d_runtemp_irri= 1;
			}
		}

		d_drainage_runtemp = 0.95;

		//sorties
		if(Yj == nvm -1)
		{
			YJ1I1 = d_runtemp_totmelt + d_YS1_drainage * d_drainage_runtemp * d_runtemp_totmelt ;
			YJ1I2 = d_runtemp_irri + d_YS1_drainage * d_drainage_runtemp * d_runtemp_irri;
			YJ1I3 = d_runtemp_runoff1 + d_YS1_drainage * d_drainage_runtemp * d_runtemp_runoff1;
			YJ1I4 = d_runtemp_runoff + d_YS1_drainage * d_drainage_runtemp * d_runtemp_runoff;
			YJ1I5 = d_runtemp_veget + d_YS1_drainage * d_drainage_runtemp * d_runtemp_veget ;
			YJ1I6 = 0;
		}
		else
		{	
			YJ1I1 = d_runtemp_totmelt;
			YJ1I2 = d_runtemp_irri;
			YJ1I3 = d_runtemp_runoff1;
			YJ1I4 = d_runtemp_runoff;
			YJ1I5 = d_runtemp_veget;
			YJ1I6 = 0;
		}		

		YJ2I1 = d_drainage_runtemp * d_runtemp_totmelt ;
		YJ2I2 = d_drainage_runtemp * d_runtemp_irri;
		YJ2I3 = d_drainage_runtemp * d_runtemp_runoff1;
		YJ2I4 = d_drainage_runtemp * d_runtemp_runoff;
		YJ2I5 = d_drainage_runtemp * d_runtemp_veget ;
		YJ2I6 = 0;

	}
	else
	{
		YJ1I1 = YJ1I2 =YJ1I3 = YJ1I4 = YJ1I5 =YJ1I6 =0;
		YJ2I1 = YJ2I2 =YJ2I3 = YJ2I4 = YJ2I5 =YJ2I6 =0;
	}
	
  }

  //===========================================================================

//************************** End of MODULE H_soil_runofft.h *******************

