  
  
/***************************************************************************
                 module  T_hum_wet.h  -  description

multipluies shumdiag(ji,jg)*intfactw(jd,jg) to obtain the soil wetness on the temperature levels


output: wetdiag:   Soil weetness on the thermodynamical levels (dimensionless) 
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ()
  {

	double x1 = 0;
	double p;
	double w;

	
		for(int k = 0 ; k < nbdl ; k++)
		{

			p= Yting[k];



			w = Yting[k+nbdl];
			x1 += w * p;

		}




	YS1 = x1;
	
	

  }
	

  //===========================================================================
  //                           backward
  backward()
  {
	double w, p;

		for (int k=0 ; k < nbdl *2 ; k++)
		{
			w = Yting[k+nbdl];
			p= Yting[k];

			if(k < 12)
				Yjac[0][k] = Yting[k+nbdl];
			else
				Yjac[0][k] = Yting[k-nbdl];


		}

	
  }

  //===========================================================================

//**************************  MODULE T_hum_wet.h ************************




























/*

  forward (YREAL shumdiag, YREAL intfactw, YREAL wetdiag1)
  {
	//double temp[11];
	
	//for(int i = 0 ; i < nbdl ; i++)	
	//	temp[i] =  Yting[i]*Yting[i+11];

	//YS1 = temp[Yi-1];
	
	YS1 = wetdiag1 + shumdiag*intfactw;

  }

  //===========================================================================
  //                         methodes  backward
  backward(YREAL shumdiag, YREAL intfactw, YREAL wetdiag1)
  {
	
	
  }

*/











