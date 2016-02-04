  
/***************************************************************************
                 module  E_netrad.h  -  description

 output                            -------------------
	YS1 = netrad: net radiation, recomoputed after being coputed in E_beg_netrad


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL netrad, YREAL netrad_init)
  {
   	if(Yt == 1)  
	{	
		YS1 = netrad_init * netr;

	}
	else
	{
		YS1 = netrad;		
	}


  }
	

  //===========================================================================
  //                           backward
  backward  (YREAL netrad, YREAL netrad_init)
  {
	if(Yt == 1)  
	{	
		YJ1I1 = 0;
		YJ1I2 = netr;  
	}
	else
	{
		YJ1I1 = 1;
		YJ1I2 = 0;  
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_netrad.h ********************************


