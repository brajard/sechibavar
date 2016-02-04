  
/***************************************************************************
                 module  C_albcalc2.h  -  description

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          forward
  forward (YREAL veget, YREAL temp11, YREAL temp22)
  {
	
	double temp1, temp2;

	if(Yj == 0)
	{
		temp1 = veget * alb_leaf_tmp[Yj][0];
		temp2 = veget * alb_leaf_tmp[Yj][1];
	}
	else
	{
		temp1 = temp11 + veget * alb_leaf_tmp[Yj][0];
		temp2 = temp22 + veget * alb_leaf_tmp[Yj][1];
	}

	YS1 = temp1;
	YS2 = temp2;

 


  }
	

  //===========================================================================
  //                          backward
  backward (YREAL veget1, YREAL veget)
  {
	
	
  }

  //===========================================================================

//************************** End of MODULE C_albcalc2.h  ********************************














