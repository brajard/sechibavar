  
/***************************************************************************
                 module classe D_comb_vbsum.h  -  description
                             -------------------
diffuco_comb: combination of coefficient : alpha and beta coefficient; combines previous partial beta coeeficient and calculates alpha and complete beta coefficient
this routine is decomposed in 5 modules. They are called in this order:
1) D_comb_vbsum
2) D_comb_vb
3) D_comb_soil
4) D_comb_veget
5) D_comb_verif

output: vbeta2sum, vbeta3sum: sums of the current vbeta2,3 coefficients
	YS1 = vbeta2sum;
	YS2 = vbeta3sum;

 ***************************************************************************/


  //===========================================================================
  //                           forward  
  forward (YREAL vbeta2, YREAL vbeta3, YREAL vb2, YREAL vb3)
  {
	if(Yt > 1)
	{
		if(Yj == 0) 
		{
			YS1 =  vbeta2;
			YS2 =  vbeta3;
		}
		else
		{
			YS1 =  vb2 + vbeta2;
			YS2 =  vb3 + vbeta3;
		}
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}

	
  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL vbeta2, YREAL vbeta3, YREAL vb2, YREAL vb3)
  {
	if(Yt > 1)
	{
		if(Yj == 0) 
		{
			YJ1I1 = 1;
			YJ1I2 = YJ1I3 =YJ1I4 = 0;

			YJ2I1 = YJ2I3 =YJ2I4 = 0;
			YJ2I2 = 1;
		}
		else
		{	
			YJ1I1 = YJ1I3 = 1;
			YJ1I2 = YJ1I4 =0;

			YJ2I1 = YJ2I3 = 0;
			YJ2I2 = YJ2I4 = 1;
		}
	}
	else
	{
		YJ1I1 = YJ1I2 = YJ1I3 = YJ1I4 = 0;
		YJ2I1 = YJ2I2 = YJ2I3 = YJ2I4 = 0;
	}


  }

  //===========================================================================

//************************** End of MODULE  D_comb_vbsum.h ********************************














