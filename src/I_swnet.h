  
/***************************************************************************
                 module  I_swnet.h  -  description

!calculo de swnet = : Net surface short-wave flux

for_swnet(i,j) = (1.-(albedo(i,j,1)+albedo(i,j,2))/2.)*swdown(i,j)
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward  
  forward (YREAL albedo, YREAL albedo1, YREAL swdown)
  {
	
	if(Yt == 1)	
	{
		YS1 = (1.-(0.13+0.13)/2.)*swdown; //inicializacion de albedos 0.13 para VIs y NIR

	}
	else		
		YS1 = (1.-(albedo+albedo1)/2.)*swdown;


  }
	

  //===========================================================================
  //                           backward 
  backward (YREAL albedo, YREAL albedo1, YREAL swdown)
  {
	if(Yt == 1)	
	{
 		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = (1.-(0.13+0.13)/2.);
	}
	else
	{
		YJ1I1 = -swdown/2;
		YJ1I2 = -swdown/2;
		YJ1I3 = (1.-(albedo+albedo1)/2.);
	}	
  }

  //===========================================================================

//************************** MODULE H_veg_water2.h ********************************







