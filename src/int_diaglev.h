  
/***************************************************************************
                 module classe init_diaglev.h  -  description


Sorties

YS1=intfactw_s
YS2 = lev_prog1   en j-1

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL dz2, YREAL lev_prog1, YREAL diagl, YREAL diaglev1, YREAL i1, YREAL l1 )
  {
	//rutina de thermosoil_diaglev
	double prev_diag = 0;
	double lev_prog = 0;
	double lev_diag = 0;
	double prev_prog = 0;

	double temp1,temp2;
	double temp3 = 0;


	if(Yt == 1)
	{
	
		lev_diag = diagl;


		//printf("diaglev %e   diagl %e     diaglev1 %e \n",diaglev[Yj],diagl, diaglev1);
	
		if(Yj == 0)
			prev_diag = 0;
		else
			prev_diag = diaglev1;
		if(Yi == 0)
			prev_prog = 0;
		else
			prev_prog = lev_prog1;



		if ( (Yi == ngrnd-1) && ((prev_prog + dz2) < lev_diag ) )
			lev_prog = lev_diag; //Just make sure we cover the deepest layers
		else
			lev_prog = prev_prog + dz2;
		
		//YS1 = MAX(MIN(lev_diag,lev_prog)-MAX(prev_diag, prev_prog), 0)/(lev_diag-prev_diag);
		if(lev_diag < lev_prog)
			temp1 = lev_diag;
		else
			temp1 = lev_prog;

		if(prev_diag > prev_prog)
			temp2 = prev_diag;
		else
			temp2 = prev_prog;

		if(temp1-temp2>0)
			temp3 = temp1-temp2;

		YS1 = temp3 / (lev_diag-prev_diag);
		//prev_prog = lev_prog;
		//prev_diag = lev_diag;


		YS2 = lev_prog;

		//printf("(%d,%d)diag dz2 %e \n",Yj,Yi,dz2); 
		//printf("(%d,%d)  prev_diag %e , prev_prog %e, lev_diag %e, levprog: %e  intfacts %e, dz2[Yi]: %e, diaglev[Yj]:%e  \n", Yj,Yi,prev_diag, prev_prog, lev_diag, lev_prog,YS1,dz2,diaglev[Yj]);



		//printf("( %d , %d  )  int_s_YS1 %e \n",Yj,Yi,YS1);
	}
	else
	{
		YS1 = i1;
		YS2 = l1;
	}

	//printf("%d(%d,%d)   %e     %e    \n",Yt,Yi,Yj,YS1,YS2);
        
  }
	


 //===========================================================================
  //                         methodes  backward
  backward (YREAL dz2, YREAL lev_prog1 , YREAL diagl , YREAL diaglev1, YREAL i1, YREAL l1 )
  {
	double prev_diag = 0;
	double lev_prog = 0;
	double lev_diag = 0;
	double prev_prog = 0;
	double temp1,temp2;
	double temp3 = 0;

	//double d_levdiag = 0;
	//double d_prevdiag = 0;

	double d_levprog_prevprog= 0;
	double d_levprog_dz2= 0;
	double d_levprog_levdiag = 0;
	
	double d_prevprog_lev_prog1= 0;

	double d_temp3_temp2= 0;
	double d_temp3_temp1= 0;
	double d_temp2_prevprog= 0;
	double d_temp2_prevdiag= 0;
	double d_temp1_levprog= 0;
	double d_temp1_levdiag= 0;

	
	double d_levdiag_diaglev = 0;
	
	double d_prevdiag_diaglev1 = 0;

	double d_YS1_temp3= 0;
	double d_YS1_levdiag = 0;
	double d_YS1_prevdiag = 0;


	if(Yt == 1)
	{

		lev_diag = diagl;

		d_levdiag_diaglev = 1;



	
		if(Yj == 0)
			prev_diag = 0;
		else
		{
			prev_diag = diaglev1;
			d_prevdiag_diaglev1 = 1;
		}

		if(Yi == 0)
		{
			prev_prog = 0;
		}
		else
		{
			prev_prog = lev_prog1;
			d_prevprog_lev_prog1 = 1;
		}



		if ( (Yi == ngrnd-1) && ((prev_prog + dz2) < lev_diag ) )
		{
			lev_prog = lev_diag; //Just make sure we cover the deepest layers
			d_levprog_levdiag = 1;
		}
		else
		{
			lev_prog = prev_prog + dz2;
			d_levprog_prevprog= 1;
			d_levprog_dz2 = 1;
		}
		
		//YS1 = MAX(MIN(lev_diag,lev_prog)-MAX(prev_diag, prev_prog), 0)/(lev_diag-prev_diag);
		if(lev_diag < lev_prog)
		{
			temp1 = lev_diag;
			d_temp1_levdiag = 1;
		}
		else
		{
			temp1 = lev_prog;
			d_temp1_levprog = 1;
		}

		if(prev_diag > prev_prog)
		{
			temp2 = prev_diag;
			d_temp2_prevdiag = 1;
		}
		else
		{
			temp2 = prev_prog;
			d_temp2_prevprog = 1;
		}

		if(temp1-temp2>0)
		{
			temp3 = temp1-temp2;
			d_temp3_temp2= -1;
			d_temp3_temp1= 1;
		}

		d_YS1_temp3 = 1 / (lev_diag-prev_diag);
		d_YS1_levdiag = -temp3 / pow(lev_diag-prev_diag,2);
		d_YS1_prevdiag = temp3 / pow(lev_diag-prev_diag,2);

		//sortie 1 
		YJ1I1 = d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_dz2;
		YJ1I2 = d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_prevprog * d_prevprog_lev_prog1 + d_YS1_temp3 * d_temp3_temp2 * d_temp2_prevprog * d_prevprog_lev_prog1;
		YJ1I3 = d_YS1_levdiag * d_levdiag_diaglev + d_YS1_temp3 * d_temp3_temp1 *d_temp1_levdiag * d_levdiag_diaglev + d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_levdiag * d_levdiag_diaglev;
		YJ1I4 = d_YS1_prevdiag * d_prevdiag_diaglev1 + d_YS1_temp3 * d_temp3_temp2 * d_temp2_prevdiag * d_prevdiag_diaglev1;
	
		//sortie 2
		YJ2I1 = d_levprog_dz2;
		YJ2I2 = d_levprog_prevprog * d_prevprog_lev_prog1 ;
		YJ2I3 = d_levprog_levdiag * d_levdiag_diaglev;
		YJ2I4 = 0;
	}
	else
	{
		YJ1I5 = YJ2I6 = 1;
		
	}
	

	
  }




  //===========================================================================

//************************** FIN DU MODULE init_diaglev.h ********************************











