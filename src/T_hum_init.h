  
/***************************************************************************
                 module  T_hum_init.h  -  description

-Put soil wetness on the temperature levels
-Computes intfactw, which is used to compute wetdiag (Soil weetness on the thermodynamical levels) 
-It is only computed once, so the trajectory is Tcontr

outputs:
	YS1 = prev_diag;
	YS2 = prev_prog;
	YS3 = lev_diag;
	YS4 = intfactw;
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL prev_diag1, YREAL prev_prog1, YREAL lev_diag1)
  {
	double lev_prog;
	//sorties
	double prev_diag, lev_diag, prev_prog,  intfactw;
	
	if((Yi == 0 ) && (Yj == 0))
		prev_diag = 0;

	if(Yj == 0)	
	{
		lev_diag = prev_diag1 + dz2[Yi];
		prev_prog = 0;
	}
	else
		lev_diag = lev_diag1;



	if( (Yj == nbdl-1) &&  (diaglev[Yj] < lev_diag ) )
                lev_prog = lev_diag;
	else
		lev_prog = diaglev[Yj];


	intfactw = MAX(MIN(lev_diag,lev_prog)-MAX(prev_diag1, prev_prog1), 0)/(lev_diag-prev_diag1);
	prev_prog = lev_prog;

	//prev_diag	
	if(Yj == nbdl-1)
		prev_diag = lev_diag;	
	if((Yj > 0) && (Yj < nbdl-1))
		prev_diag = prev_diag1;

	YS1 = prev_diag;
	YS2 = prev_prog;
	YS3 = lev_diag;
	YS4 = intfactw;


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL prev_diag1, YREAL prev_prog1, YREAL lev_diag1)
  {
	
	
  }

  //===========================================================================

//************************** MODULE T_hum_init.h ***********************














