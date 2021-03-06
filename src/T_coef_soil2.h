  
/***************************************************************************
                 module  T_coef_soil2.h  -  description



Computation of cgrnd and dgrnd coefficient at the t time-step

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL zdz1,YREAL zdz2,YREAL zdz11,YREAL ptn,YREAL cgrnd1, YREAL dgrnd1)
  {

	double z1;


	if(Yj == ngrnd -1)
	{
		YS1 = 0;
		YS2 = 0;	
	}
	else if(Yj == ngrnd -2)
	{
		z1 = zdz2 + zdz11;
		YS1 = zdz2 * ptn / z1;
		YS2 = zdz11 / z1;
	}
	else
	{
		z1 = 1 / (zdz2 + zdz11 + zdz1 * (1 - dgrnd1));
		YS1 = (ptn * zdz2 + zdz1 * cgrnd1) * z1;
		YS2 = zdz11 * z1;
		
	}



  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL zdz1,YREAL zdz2,YREAL zdz11,YREAL ptn,YREAL cgrnd1, YREAL dgrnd1)
  {
	double z1= 0;

	//derivadas de z1
	double d_z1_zdz2= 0;
	double d_z1_zdz11= 0;
	double d_z1_zdz1= 0;
	double d_z1_dgrnd1= 0;

	//derivadas YS1 y YS2
	double d_YS1_z1= 0;
	double d_YS2_z1= 0;


	if(Yj == ngrnd -1)
	{
		YJ1I1 = YJ1I2 =YJ1I3 =YJ1I4 =YJ1I5 =YJ1I6 = 0;
		YJ2I1 = YJ2I2 =YJ2I3 =YJ2I4 =YJ2I5 =YJ2I6 =0;
	}
	else if(Yj == ngrnd -2)
	{		
		z1 = zdz2 + zdz11;

		d_z1_zdz2 = 1 ;
		d_z1_zdz11 = 1;


		YJ1I1 = YJ1I5 =YJ1I6 =0;
		YJ1I2 = ptn/z1 -(zdz2*ptn/(z1*z1)) * d_z1_zdz2 ;
		YJ1I3 = -(zdz2*ptn/(z1*z1)) * d_z1_zdz11;
		YJ1I4 = zdz2/z1;

		YJ2I1 = YJ2I4 =YJ2I5 =YJ2I6 = 0;
		YJ2I2 = - (zdz11/(z1*z1)) * d_z1_zdz2;
		YJ2I3 = 1/z1 - (zdz11/(z1*z1)) * d_z1_zdz11 ;
	}
	else
	{
		
		z1 = 1 / (zdz2 + zdz11 + zdz1 * (1 - dgrnd1));

		d_z1_zdz2 = -1/  pow(zdz2 + zdz11 + zdz1 * (1 - dgrnd1) , 2);
		d_z1_zdz11 = -1/  pow(zdz2 + zdz11 + zdz1 * (1 - dgrnd1) , 2);
		d_z1_zdz1 =  -(1 - dgrnd1)/  pow(zdz2 + zdz11 + zdz1 * (1 - dgrnd1) , 2);;
		d_z1_dgrnd1 = zdz1/  pow(zdz2 + zdz11 + zdz1 * (1 - dgrnd1) , 2);

		d_YS1_z1 = (ptn * zdz2 + zdz1 * cgrnd1) ;
		d_YS2_z1 = zdz11;


		YJ1I1 = cgrnd1 * z1 + d_YS1_z1 * d_z1_zdz1;
		YJ1I2 = ptn * z1 + d_YS1_z1 * d_z1_zdz2;
		YJ1I3 = d_YS1_z1 * d_z1_zdz11;
		YJ1I4 = zdz2 * z1;
		YJ1I5 = zdz1 * z1;
		YJ1I6 = d_YS1_z1 * d_z1_dgrnd1;

		YJ2I1 = d_YS2_z1 * d_z1_zdz1 ;
		YJ2I2 = d_YS2_z1 * d_z1_zdz2;
		YJ2I3 = z1 + d_YS2_z1 * d_z1_zdz11;
		YJ2I4 = 0;
		YJ2I5 = 0;
		YJ2I6 = d_YS2_z1 * d_z1_dgrnd1;


		
	}

	
	
  }

  //===========================================================================

//**************************  MODULE T_coef_soil2.h ********************************





