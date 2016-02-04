  
/***************************************************************************
                 module classe C_albcalc3.h  -  description

SALIDA : YS1 = albedo

calculo fibnal del albedo (Visible et infra rojo)

si impaze no es 1, buscar el otro calculo del albedo que esta en sechiba1

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward ()
  {
		if(Yi == 0)
	 	{	
			YS1 = albedo_scal[0];	
			//printf("%d : %e \n", Yt, albedo_scal[0]);
		}
		else
		{
			YS1 = albedo_scal[1];
			//printf("%d : %e \n", Yt, albedo_scal[1]);
		}
  }
	

  //===========================================================================
  //                         methodes  backward
  backward ()
  {

  }

  //===========================================================================

//************************** FIN DU MODULE C_albcalc3.h  ********************************














