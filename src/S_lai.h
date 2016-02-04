  
/***************************************************************************
                 module  S_lai.h  -  description

leaf area index
I only do the interpolation between laimax and laimin

I use a temperature compute by stemdiag to do the interpolation
The initial temperature 280, it can be found in the run.def file "SLOWPROC_LAI_TEMPDIAG = 280"

-lcanop: soil level used for LAI


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL lai1, YREAL d1, YREAL d2,YREAL d3,YREAL d4,YREAL d5,YREAL d6,YREAL d7,YREAL d8,YREAL d9,YREAL d10,YREAL d11, YREAL lcan,YREAL temp_init)
  {
		
	double temp = 0;

	if((bypass_lai_active == 1) && (sechiba_vegmax[Yi][Yj] != 0))
	{
		if(Yj ==0)
			YS1 = 0;
		else
		YS1 = bypass_lai;
	}
	else
	{
		if(Yt ==1)
		{
			temp =tempfunc(temp_init);

			if(Yj ==0)
				YS1 = 0;
			else
			{
				if((lai_imp == 1) && (Yj!= PFT_c))	
				{		
					YS1 = llaimin[Yj] + temp * (llaimax[Yj] - llaimin[Yj]);	
				}
				
				
				if(lai_imp == 0)
					YS1 = llaimin[Yj] + temp * (llaimax[Yj] - llaimin[Yj]);	
			}


		}
		else if (((Yt)% (int) (one_day_pdt))==0) 
		{

			switch((int)lcan)
			{
				case 0: { temp =tempfunc(d1);
					  break;}	
				case 1: { temp =tempfunc(d2);
					  break;}
				case 2: { temp =tempfunc(d3);
					  break;}	
				case 3: { temp =tempfunc(d4);
					  break;}
				case 4: { temp =tempfunc(d5);
					  break;}	
				case 5: { temp =tempfunc(d6);
					  break;}
				case 6: { temp =tempfunc(d7);
					  break;}	
				case 7: { temp =tempfunc(d8);
					  break;}
				case 8: { temp =tempfunc(d9);
					  break;}	
				case 9: { temp =tempfunc(d10);
					  break;}
				case 10: { temp =tempfunc(d11);
					  break;}	
			}			

			
			if(Yj ==0)
				YS1 = 0;
			else
			{
				if((lai_imp == 1) && (Yj!= PFT_c))	
				{		
					YS1 = llaimin[Yj] + temp * (llaimax[Yj] - llaimin[Yj]);	
				}		
				
				
				if(lai_imp == 0)
					YS1 = llaimin[Yj] + temp * (llaimax[Yj] - llaimin[Yj]);	
			}
		}
		else
		{
			if((lai_imp == 1) && (Yj!= PFT_c))
			{			
				YS1 = lai1;
			}
			
				
			if(lai_imp == 0)
				YS1 = lai1;
		}

	}


  }
	

  //===========================================================================
  //                           backward
  backward (YREAL lai1, YREAL d1, YREAL d2,YREAL d3,YREAL d4,YREAL d5,YREAL d6,YREAL d7,YREAL d8,YREAL d9,YREAL d10,YREAL d11, YREAL lcan,YREAL temp_init)
  {
	YJ1I1 = 1;

	
  }

  //===========================================================================

//************************** MODULE S_lai.h  **********************


