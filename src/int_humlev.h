  
/***************************************************************************
                 module classe init_humlev.h  -  description


Sorties

YS1 =intfactw_w 
YS2 = lev_prog
YS3 = lev_diag

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                          methodes forward
  forward (YREAL dzz2, YREAL lev_prog1, YREAL lev_diag1, YREAL diagle, YREAL i1, YREAL l1, YREAL s1 )
  {
	double prev_diag = 0;
	double lev_prog = 0;
	double lev_diag = 0;
	double prev_prog = 0;
	double temp1,temp2;
	double temp3 = 0;

	if(Yt == 1)
	{

		if(Yj == 0)
		{
			prev_diag = 0;
			lev_diag = dzz2;
		}
		else
		{
			prev_diag = lev_diag1;
			lev_diag = prev_diag + dzz2;
		}


		if(Yi == 0)
			prev_prog = 0;
		else
			prev_prog = lev_prog1;

				      
		if((Yi == nbdl-1) && (diagle < lev_diag) )
			lev_prog = lev_diag;
		else
			lev_prog = diagle;

		//printf("diaglev %e     diagle %e  MEMEMEMe \n",diaglev[Yi],diagle);
		


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
		


		//YS1 =intfactw_w; 
		YS2 = lev_prog;
		YS3 = lev_diag;
		//printf("(%d,%d)humlev dzz2 %e \n",Yi,Yj,dzz2); 
		//printf("(%d,%d)  prev_diag %e , prev_prog %e, lev_diag %e, levprog: %e  intfactw %e, dzz2[Yi]: %e, diaglev[Yj]:%e  \n", Yi,Yj,prev_diag, prev_prog, lev_diag, lev_prog,YS1,dzz2,diaglev[Yi]);

		//printf("%d(%d,%d)----YS1 %e   int_o %e      dzz2 %e  diaglev %e \n",Yt,Yi,Yj,YS1, intfactw_w[Yj][Yi], dzz2,diaglev[Yi]);
		//printf("( %d , %d  )  int_w_YS1 %e\n",Yi,Yj,YS1);
		//printf("%d( %d , %d  )   YS1 %e\n",Yt,Yi,Yj,YS1);
		//if((Yi == ngrnd -1) && (Yj == nbdl -1))
		//	printf("\n\n ------------------------------------------------------------------- \n\n");
	}
	else
	{
		YS1 = i1;
		YS2 = l1;
		YS3 = s1;
	}
	//printf("%d(%d,%d)   %e     %e    \n",Yt,Yi,Yj,YS1,YS2);   
  }
	


 //===========================================================================
  //                         methodes  backward
  backward (YREAL dzz2, YREAL lev_prog1, YREAL lev_diag1, YREAL diagle, YREAL i1, YREAL l1, YREAL s1)
  {
	double prev_diag = 0;
	double lev_prog = 0;
	double lev_diag = 0;
	double prev_prog = 0;
	double temp1,temp2;
	double temp3 = 0;

	double d_temp3_temp2 = 0;
	double d_temp3_temp1 = 0;

	double d_temp1_levdiag = 0;
	double d_temp1_levprog = 0;
	
	double d_temp2_prevdiag = 0;
	double d_temp2_prevprog = 0;

	double d_levdiag_dzz2 = 0;
	double d_levdiag_prevdiag = 0;
	double d_levdiag_diaglev = 0;
	
	double d_levprog_levdiag = 0;
	
	double d_prevdiag_levdiag1 = 0;

	double d_prevprog_levprog1 = 0;

	double d_YS1_temp3 = 0;
	double d_YS1_levdiag = 0;
	double d_YS1_prevdiag = 0;
	

	if(Yt == 1)
	{
	
		if(Yj == 0)
		{
			prev_diag = 0;
			lev_diag = dzz2;

			d_levdiag_dzz2 = 1;	
		}
		else
		{
			prev_diag = lev_diag1;
			lev_diag = prev_diag + dzz2;

			d_prevdiag_levdiag1 = 1;
			d_levdiag_dzz2 = 1;
			d_levdiag_prevdiag = 1;		
		}


		if(Yi == 0)
			prev_prog = 0;
		else
		{
			prev_prog = lev_prog1;
			d_prevprog_levprog1 = 1;
		}

				      
		if((Yi == nbdl-1) && (diagle < lev_diag) )
		{
			lev_prog = lev_diag;
			d_levprog_levdiag = 1;
		}
		else
		{
			lev_prog = diagle;
			d_levdiag_diaglev = 1;
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
			d_temp3_temp2 = -1;
			d_temp3_temp1 = 1;
		}

		//YS1 = temp3 / (lev_diag-prev_diag);

		d_YS1_temp3 = 1 / (lev_diag-prev_diag);
		d_YS1_levdiag = -temp3 / pow(lev_diag-prev_diag,2);
		d_YS1_prevdiag = temp3 / pow(lev_diag-prev_diag,2);



		YJ1I1 = d_YS1_temp3 * d_temp3_temp1 * d_temp1_levdiag * d_levdiag_dzz2 + d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_levdiag * d_levdiag_dzz2 + d_YS1_levdiag * d_levdiag_dzz2;
		YJ1I2 = d_YS1_temp3 * d_temp3_temp2 * d_temp2_prevprog * d_prevprog_levprog1;
		YJ1I3 = d_YS1_temp3 * d_temp3_temp1 * d_temp1_levdiag * d_levdiag_prevdiag * d_prevdiag_levdiag1 + d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_levdiag * d_levdiag_dzz2 + d_YS1_temp3 * d_temp3_temp2 * d_temp2_prevdiag * d_prevdiag_levdiag1;
		YJ1I4 = d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levdiag_diaglev;//d_YS1_levdiag * d_levdiag_diaglev + d_YS1_temp3 * d_temp3_temp1 * d_temp1_levdiag * d_levdiag_diaglev +  d_YS1_temp3 * d_temp3_temp1 * d_temp1_levprog * d_levprog_levdiag * d_levdiag_diaglev;

	//printf("d_YS1_levdiag %e  d_levdiag_diaglev %e   d_YS1_temp3 %e    d_temp3_temp1 %e   d_temp1_levdiag %e   d_levdiag_diaglev %e \n\n",d_YS1_levdiag , d_levdiag_diaglev,d_YS1_temp3 , d_temp3_temp1 , d_temp1_levdiag , d_levdiag_diaglev);



		YJ2I1 = d_levprog_levdiag * d_levdiag_dzz2;
		YJ2I2 = 0;
		YJ2I3 = d_levprog_levdiag * d_levdiag_prevdiag * d_prevdiag_levdiag1;
		YJ2I4 = d_levdiag_diaglev;

		YJ3I1 = d_levdiag_dzz2;
		YJ3I2 = 0;
		YJ3I3 = d_levdiag_prevdiag * d_prevdiag_levdiag1;
		YJ3I4 = d_levprog_levdiag * d_levdiag_diaglev;

	}
	else
	{
		YJ1I5 = YJ2I6 = YJ3I7 = 1;
	}

  }




  //===========================================================================

//************************** FIN DU MODULE init_humlev.h ********************************











