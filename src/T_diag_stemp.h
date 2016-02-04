/*  
  ***************************************************************************
                 module  T_diag_stemp.h  -  description

output : YS1 = stempdiag; temperature profile(stempdiag)
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ()
  {
	double x = 0;
	double x1 = 0;
	double p;
	double w;
	for(int k = 0 ; k < ngrnd ; k++)
	{
		p = Yting[k]; 
		x+= intfactw_s[Yj][k] * p;

		w = Yting[k+7];
		x1+= w * p;

	}

	YS1 = x1;
  }
	

  //===========================================================================
  //                           backward
  backward ()
  {
	
	double w, p;


	for (int k=0 ; k < ngrnd *2 ; k++)
	{
		w = Yting[k+ngrnd];
		p= Yting[k];

		if(k < 7)
			Yjac[0][k] = Yting[k+ngrnd];
		else
			Yjac[0][k] = w = Yting[k-ngrnd];

	}

	
  }

  //===========================================================================

//************************** MODULE T_diag_stemp.h ************************






