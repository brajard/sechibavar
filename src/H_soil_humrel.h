  
/***************************************************************************
                 module  H_soil_humrel.h  -  description

Description:
 	Compute the moisture stress on vegetation and  relative surface humidity
	Only use the standard formulas if total soil moisture is larger than 0.
	else stress functions are set to 0. This will avoid that large negative 
	soil moisture accumulate over time by the the creation of small skin reservoirs which evaporate quickly.


	humrel y vegstress---> HYDROL_HUMR = 1

Sorties
	YS1 = humrel
                             -------------------
 ***************************************************************************/


  //===========================================================================
  //                           forward
forward(YREAL hum_c1, YREAL hum_c2,YREAL hum_c3,YREAL hum_c4,YREAL hum_c5,YREAL hum_c6,YREAL hum_c7,YREAL hum_c8,YREAL hum_c9,YREAL hum_c10,YREAL hum_c11,YREAL hum_c12,YREAL hum_c13 )
  {
	switch(Yj)
	{
		case 0:
		{ YS1 = hum_c1; break;}
		case 1:
		{ YS1 = hum_c2; break;}
		case 2:
		{ YS1 = hum_c3; break;}
		case 3:
		{ YS1 = hum_c4; break;}
		case 4:
		{ YS1 = hum_c5; break;}
		case 5:
		{ YS1 = hum_c6; break;}
		case 6:
		{ YS1 = hum_c7; break;}
		case 7:
		{ YS1 = hum_c8; break;}
		case 8:
		{ YS1 = hum_c9; break;}
		case 9:
		{ YS1 = hum_c10; break;}
		case 10:
		{ YS1 = hum_c11; break;}
		case 11:
		{ YS1 = hum_c12; break;}
		case 12:
		{ YS1 = hum_c13; break;}
	}
  }
	

  //===========================================================================
  //                           backward
backward(YREAL hum_c1, YREAL hum_c2,YREAL hum_c3,YREAL hum_c4,YREAL hum_c5,YREAL hum_c6,YREAL hum_c7,YREAL hum_c8,YREAL hum_c9,YREAL hum_c10,YREAL hum_c11,YREAL hum_c12,YREAL hum_c13 )
  {
	switch(Yj)
	{
		case 0:
		{ YJ1I1 = 1; break;}
		case 1:
		{ YJ1I2 = 1; break;}
		case 2:
		{ YJ1I3 = 1; break;}
		case 3:
		{ YJ1I4 = 1; break;}
		case 4:
		{ YJ1I5 = 1; break;}
		case 5:
		{ YJ1I6 = 1; break;}
		case 6:
		{ YJ1I7 = 1; break;}
		case 7:
		{ YJ1I8 = 1; break;}
		case 8:
		{ YJ1I9 = 1; break;}
		case 9:
		{ YJ1I10 = 1; break;}
		case 10:
		{ YJ1I11 = 1; break;}
		case 11:
		{ YJ1I12 = 1; break;}
		case 12:
		{ YJ1I13 = 1; break;}
	}
  }

  //===========================================================================

//************************** End of MODULE H_soil_humrel.h ********************************














