  
/***************************************************************************
                 module classe E_beg_netrad.h  -  description

computes netrad and absorbed LW radiation absorbed at the surface 
Long wave radiation absorbed by the surface is the product of emissivity and downwelling LW radiation


outputs: 
       YS1 = lwabs-----> LW radiation absorbed by the surface;
       YS2 = netrad ---> Net radiation;                      
				-------------------

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL emis, YREAL lwdown, YREAL swnet, YREAL temp_sol)  
  {

	if(Yt > 1)
	{

		YS1 = emis * lwdown;
		YS2 = lwdown + swnet  - (emis * c_stefan * pow(temp_sol,4) + (1 - emis) * lwdown) ;

	}
	else
	{
		YS1 = 0;
		YS2 = 0;
	}


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL emis, YREAL lwdown, YREAL swnet, YREAL temp_sol)   
  {

	if(Yt > 1)
	{
		YJ1I1 = lwdown;
		YJ1I2 = emis;
		YJ1I3 = 0 ;
		YJ1I4 = 0 ;

		YJ2I1 = -c_stefan * pow(temp_sol,4) + lwdown;
		YJ2I2 = emis;
		YJ2I3 = 1 ;
		YJ2I4 = - 4 * emis * c_stefan * pow(temp_sol,3) ;

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

//************************** End of MODULE E_beg_netrad.h ********************************

