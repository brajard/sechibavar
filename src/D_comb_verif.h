  
/***************************************************************************
                 module  D_comb_verif.h  -  description
                             -------------------

In any case there is no transpiration when air moisture is too high.


outputs:
	YS1 = vbeta3---> Beta for transpiration;
	YS2 = humrel---> Moisture stress


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL qsatt, YREAL qair, YREAL vbeta3, YREAL humrel)
  {

	if(Yt > 1)
	{
		if ( qsatt < qair )
		{
			YS1 = 0;
			YS2 = 0;		
		}
		else
		{
			YS1 = vbeta3;
			YS2 = humrel;
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
  backward (YREAL qsatt, YREAL qair, YREAL vbeta3, YREAL humrel)
  {
	if(Yt > 1)
	{
		if ( qsatt < qair )
		{
			YJ1I1 = 0;
			YJ1I2 = 0;
			YJ1I3 = 0;
			YJ1I4 = 0;

			YJ2I1 = 0;
			YJ2I2 = 0;
			YJ2I3 = 0;
			YJ2I4 = 0;
		}
		else
		{
			YJ1I1 = 0;
			YJ1I2 = 0;
			YJ1I3 = 1;
			YJ1I4 = 0;

			YJ2I1 = 0;
			YJ2I2 = 0;
			YJ2I3 = 0;
			YJ2I4 = 1;			
		}
	}
	else
	{	
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
		YJ2I4 = 0;	
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE D_comb_verif.h ********************************














