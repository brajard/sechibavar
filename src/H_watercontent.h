  
/***************************************************************************
                 module  H_watercontent.h  -  description
                             -------------------

output---> Nhum

intput
	a--->  min(humidite observe)
	b--->  max(humiditer observe)
	c--->  min(humidite simule)
	d--->  max(humiditer simule)


 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL bqsb, YREAL gqsb, YREAL a,YREAL b,YREAL c,YREAL d )
  {
	double eau_total;
	double Neau_total;



	//if(Yt > 1)
	//{
		if((d == 0)||(c == 0))
		{
			d = 1;
			c = 0;
		}
		//eau_total = (bqsb+gqsb)/300;
		eau_total = (bqsb+gqsb);
		Neau_total = (eau_total-c)/(d-c);
		
		YS1 = a + Neau_total*(b-a);
   }
	

  //===========================================================================
  //                           backward
  backward (YREAL bqsb, YREAL gqsb, YREAL a,YREAL b,YREAL c,YREAL d )
  {
	double eau_total;
	double Neau_total;

	double d_eau_bqsb = 0;
	double d_eau_gqsb = 0;

	double d_neau_c = 0;
	double d_neau_d = 0;
	double d_neau_eau = 0;

	double d_YS1_neau = 0;

		if((d == 0)||(c == 0))
		{
			d = 1;
			c = 0;
		}

		eau_total = (bqsb+gqsb);
		d_eau_bqsb = 1;
		d_eau_gqsb = 1;

		Neau_total = (eau_total-c)/(d-c);
		d_neau_c = eau_total-d / pow(d-c,2);
		d_neau_d = -(eau_total-c)/pow(d-c,2);
		d_neau_eau = 1/(d-c);
	

		d_YS1_neau = (b-a);

		YJ1I1 = d_YS1_neau * d_neau_eau *d_eau_bqsb ;
		YJ1I2 = d_YS1_neau * d_neau_eau *d_eau_gqsb ;
		YJ1I3 = 1 -Neau_total ;
		YJ1I4 = Neau_total;
		YJ1I5 = d_YS1_neau * d_neau_c;
		YJ1I6 = d_YS1_neau * d_neau_d;



  }

  //===========================================================================

//************************** End of MODULE H_watercontent.h  ********************************


