  
/***************************************************************************
                 module  S_qsintm.h  -  description

YS1 :qsintmax  Maximum water on vegetation for interception



                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL veget, YREAL lai)
  {

		
		
	if(Yj == 0)
		YS1 = 0;
	else
		YS1 = qsintcst * veget * lai ; 


	  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget, YREAL lai)
  {


	if(Yj == 0)
		YJ1I1 = YJ1I2 = 0;		
	else
	{

		YJ1I1 = qsintcst * lai;
		YJ1I2 = qsintcst * veget;
	}

	
	
  }

  //===========================================================================

//************************** MODULE S_qsintm.h  **********************



