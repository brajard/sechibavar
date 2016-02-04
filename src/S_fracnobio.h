  
/***************************************************************************
                 module  S_fracnobio.h  -  description

fracnobio : Fraction of other surface types within the mesh
For the moment, there is only ice

-Value for frac_nobio for tests in 0-dim simulations is zero
-laisser ca tant qu'il n'y a que de la glace (pas de lacs)


                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ()
  {
	double fracnobio,tot_fracnobio;
		
	fracnobio = 0;
	tot_fracnobio = 0;
	
	YS1 = fracnobio;
	YS2 =  tot_fracnobio;

  }
	

  //===========================================================================
  //                           backward
  backward ()
  {

	
  }

  //===========================================================================

//**************************  MODULE S_fracnobio.h  **********************














