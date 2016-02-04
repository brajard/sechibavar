  
/***************************************************************************
                 module  T_coef_soil.h  -  description



Computation of cgrnd and dgrnd coefficient at the t time-step
//thermal conductivity at all levels


	YS1 = zdz1;       numerical constant (W/m2/K)	
	YS2 = zdz2;       numerical constant (W/m2/K)
	YS3 = pcapa_en;   capacity used for energy_incr calculation: snow heat capacity if snow is present in the layer; soil heat capacity otherwise. (J/K)


kpjindex - ngrnd

Parametres du modele
	so_capa_dry= 1.80e+6  ---> Dry soil Heat capacity of soils (J/m^3/K)
	so_capa_wet= 3.03e+6  ---> Wet soil Heat capacity of soils (J/m^3/K)

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL wetdiag, YREAL snow, YREAL so_capa_dry, YREAL so_capa_wet, YREAL dzz1, YREAL dzz2, YREAL zz_coeff, YREAL zz_coeff1, YREAL so_capa, YREAL so_cond)
  {
	double pcapa; //volumetric vertically discretized soil heat capacity (J/K/m3)
	double pkappa;//vertically discretized soil thermal conductivity (W/K/m)

	pcapa = so_capa * (so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry));
	pkappa = so_cond * (so_cond_dry + wetdiag*(so_cond_wet - so_cond_dry));
	YS3 = so_capa * (so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry));

	

      	//zdz1
        if(Yj < ngrnd -1) 
	{
	  	//YS1 = dz1[Yj] * pkappa;
		YS1 = dzz1 * pkappa;
	}
	else
		YS1 = 0;

	YS2= pcapa * dzz2/dtradia;
        
  }
	


 //===========================================================================
  //                         methodes  backward
  backward (YREAL wetdiag, YREAL snow, YREAL so_capa_dry, YREAL so_capa_wet, YREAL dzz1, YREAL dzz2, YREAL zz_coeff, YREAL zz_coeff1, YREAL so_capa, YREAL so_cond)
  {
	double pcapa; //volumetric vertically discretized soil heat capacity (J/K/m3)
	double pkappa;//vertically discretized soil thermal conductivity (W/K/m)
	
	double d_YS1_pkappa = 0;
	double d_YS1_dzz1 = 0;
	
	double d_YS2_pcapa = 0;
	double d_YS2_dzz2 = 0;
	
	double d_pcapa_capa =0;
	double d_pcapa_dry = 0;
	double d_pcapa_wet = 0;
	double d_pcapa_diag =0;

	double d_pkapa_cond = 0;
	double d_pkapa_diag =0;


	pcapa = so_capa * (so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry));
	pkappa = so_cond * (so_cond_dry + wetdiag*(so_cond_wet - so_cond_dry));
	YS3 = so_capa * (so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry));

	d_pcapa_capa = so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry);
	d_pcapa_dry = so_capa - wetdiag*so_capa	;
	d_pcapa_wet = wetdiag*so_capa;
	d_pcapa_diag = so_capa *(so_capa_wet - so_capa_dry);

	d_pkapa_cond = so_cond_dry + wetdiag*(so_cond_wet - so_cond_dry);
	d_pkapa_diag = so_cond *(so_cond_wet - so_cond_dry);

	YJ3I9 = so_capa_dry + wetdiag*(so_capa_wet - so_capa_dry);
	YJ3I3 = so_capa - wetdiag*so_capa;
	YJ3I4 = wetdiag*so_capa;
	YJ3I1 = so_capa *(so_capa_wet - so_capa_dry);


      	//zdz1
        if(Yj < ngrnd -1) 
	{
		d_YS1_pkappa = dzz1;
		d_YS1_dzz1 = pkappa;

		YJ1I1 = d_YS1_pkappa * d_pkapa_diag;
		YJ1I5 = d_YS1_dzz1;
		YJ1I10 = d_YS1_pkappa * d_pkapa_cond;		
	}
	else
	{
		YJ1I1 =YJ1I2 =YJ1I7 =YJ1I8 =YJ1I5 = YJ1I10 = YJ1I3 = YJ1I4 =YJ1I6 =YJ1I9 =0;
	}
	d_YS2_pcapa = dzz2/dtradia;
	d_YS2_dzz2 = pcapa /dtradia;


	YJ2I1 = d_YS2_pcapa * d_pcapa_diag;
	YJ2I3 = d_YS2_pcapa * d_pcapa_dry;
	YJ2I4 = d_YS2_pcapa *d_pcapa_wet;
	YJ2I6 = d_YS2_dzz2 ;
	YJ2I9 = d_YS2_pcapa * d_pcapa_capa;

	
  }

  //===========================================================================

//************************** MODULE T_coef_soil.h ********************************




