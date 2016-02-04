  
/***************************************************************************
                 module classe E_flux_net.h  -  description

 output                            -------------------
	YS1 = netrad: net radiation, recomoputed after being coputed in E_beg_netrad
	YS2 = lwnet: Net Long-wave radiation

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL lwdown, YREAL swnet, YREAL lwup)
  {
   	if(Yt > 1)  
	{	
		YS1 = lwdown + swnet - lwup;
		YS2 = lwdown - lwup;
	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}

	

  }

  //===========================================================================
  //                           backward
  backward (YREAL lwdown, YREAL swnet, YREAL lwup)
  {
	if(Yt > 1)  
	{	
		

		YJ1I1 = 1;
		YJ1I2 = 1;  
		YJ1I3 = -1;   

		YJ2I1 = 1;
		YJ2I2 = 0;
		YJ2I3 = -1;
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;  
		YJ1I3 = 0;   

		YJ2I1 = 0;
		YJ2I2 = 0;
		YJ2I3 = 0;
	}
	
	
  }

  //===========================================================================

//************************** End of MODULE E_flux_net.h ********************************














