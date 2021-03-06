  
/***************************************************************************
                 module  D_pre_trans2.h  -  description
                             -------------------
output: rveget, resistance de la vegetaction. 

 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL swnet, YREAL zdefconc, YREAL lai)
  {
	if( ( lai > min_sechiba  ) &&  ( kzero[Yj] > min_sechiba ) && ( swnet > min_sechiba ) )
		YS1 =   (( swnet + rayt_cste ) / swnet )  * ((defc_plus + (defc_mult * zdefconc )) / kzero[Yj]) * (1 / lai);
	else
		YS1 = 0;

	

  }
// [ ]	

  //===========================================================================
  //                           backward
  backward (YREAL swnet, YREAL zdefconc, YREAL lai)
  {
	if( ( lai > min_sechiba  ) &&  ( kzero[Yj] > min_sechiba ) && ( swnet > min_sechiba ) )
	{
		YJ1I1 = (-rayt_cste / pow(swnet,2) )* ((defc_plus + (defc_mult * zdefconc )) / kzero[Yj]) * (1 / lai);
		YJ1I2 =   (defc_mult /kzero[Yj]) *  (( swnet + rayt_cste ) / swnet ) * (1 / lai);
		YJ1I3 =  -(( swnet + rayt_cste ) / swnet )  * ((defc_plus + (defc_mult * zdefconc )) / kzero[Yj]) * (1 / pow(lai,2));
	}
	else
		YJ1I1 = YJ1I2 = YJ1I3 = 0;

		
  }

  //===========================================================================

//************************** End of MODULE D_pre_trans2.h ********************************

