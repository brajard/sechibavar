  
/******************************************************************************
                 module  H_soil_cond.h  -  description



output:
	YS1 = bqsb
	YS2 = gqsb
	YS3 = dsg	
	YS4 = dsp
	YS5 = dss
	YS6 = runoff
	YS7 = mean_bqsb
	YS8 = mean_gqsb

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward ()
  {
	 
	bool OnceMore = true;
	int niter = 0;
	bool lbad_ij[kjindex]= {true};
	bool lbad[kjindex][nvm];

	int count_lbad = 0;
	int count_kjindex_bad = 0;

	double bqsb[kjindex][nvm];
	double gqsb[kjindex][nvm]; 
	double dsg[kjindex][nvm];
	double dsp[kjindex][nvm]; 
	double dss[kjindex][nvm]; 
	double runoff[kjindex][nvm]; 
	double mean_bqsb[kjindex][nvm]; 
	double mean_gqsb[kjindex][nvm];
	double veget[kjindex][nvm];
	double ruu_ch[kjindex];
	double mx_var[kjindex];
	double vegtot[kjindex];


	if((Yi == 0) && (Yj == 0))
		
	{
	
		for(int i = 0; i < kjindex ; i++)
		{
			for(int j = 0 ; j < nvm ; j++)
			{
				bqsb[i][j] = YS_H_soil_dlock(2,i,j,Yt);
				gqsb[i][j]= YS_H_soil_dlock(0,i,j,Yt);
				dsg[i][j]= YS_H_soil_dlock(1,i,j,Yt);
				dsp[i][j]= YS_H_soil_dlock(3,i,j,Yt);
				dss[i][j]= YS_H_soil_dlock(4,i,j,Yt);
				runoff[i][j] = YS_H_soil_runoff(5,i,j,Yt);
				mean_bqsb[i][j] = YS_H_soil_diff(0,i,nvm-1,Yt);
				mean_gqsb[i][j] = YS_H_soil_diff(1,i,nvm-1,Yt);
				veget[i][j] = YS_S_veget(0,i,j,Yt-1);
				ruu_ch[i] = YS_init_hydrolc(1,i,nvm-1,1);
				mx_var[i] = YS_init_hydrolc(2,i,nvm-1,1);
				vegtot[i] = YS_init_hydrolc(0,i,nvm-1,1);				
			}
		}

	    	if(Yt > 1)
		{

	 	//nitermax prevents infinite loops (should actually never occur)
		do
	      	{
			niter++;	
		
			for(int ji = 0; ji < kjindex ; ji++)
			{
				for(int jv = 0 ; jv < nvm ; jv++)
				{
					if(lbad_ij[ji])
					{
						if( veget[ji][jv] > 0 )
						{
							bqsb[ji][jv] = mean_bqsb[ji][nvm-1];
							dsp[ji][jv] = dpu_cste - bqsb[ji][jv]/ruu_ch[ji];
						}
					}	

					 //where do we have to do something?
					if( ( dsp[ji][jv] < dsg[ji][jv] ) && ( dsg[ji][jv] > 0 ) && ( veget[ji][jv] > 0 ))
					{
						lbad[ji][jv] = true;
						count_lbad++;
						count_kjindex_bad++;

			

					}
					else
						lbad[ji][jv] = false;

				 }



				if( count_kjindex_bad == 0 ) 
					lbad_ij[ji]= false;
				else
				{
					lbad_ij[ji]= true;
					count_kjindex_bad = 0;
				}
				
			}
			if (count_lbad == 0)
				OnceMore = false;
			else
				count_lbad = 0;

			
			for(int ji = 0; ji < kjindex ; ji++)
			{
				for(int jv = 0 ; jv < nvm ; jv++)
				{

					if(lbad[ji][jv])
					{					    
						runoff[ji][jv] = runoff[ji][jv] + MAX( bqsb[ji][jv] + gqsb[ji][jv] - mx_var[ji], 0);
						bqsb[ji][jv] = MIN( bqsb[ji][jv] + gqsb[ji][jv], mx_var[ji]);
						gqsb[ji][jv] = 0;
						dsp[ji][jv] = dpu_cste - bqsb[ji][jv]/ruu_ch[ji];
						dss[ji][jv] = dsp[ji][jv];
						dsg[ji][jv] = 0;
					}
				}
			}

			mean_bqsb[0][0] = 0;
			mean_gqsb[0][0] = 0;


			for(int ji = 0; ji < kjindex ; ji++)		
			{
				for(int jv = 0 ; jv < nvm ; jv++)
				{
					if( vegtot[ji] >0 )
					{
					    mean_bqsb[ji][jv] = mean_bqsb[ji][jv] + (veget[ji][jv]/vegtot[ji])*bqsb[ji][jv];
					    mean_gqsb[ji][jv] = mean_gqsb[ji][jv] + (veget[ji][jv]/vegtot[ji])*gqsb[ji][jv];
					}
				}
			}

		}  while( OnceMore && ( niter < nitermax ) ) ;  //end-do while

		}


		for(int ji = 0; ji < kjindex ; ji++)		
		{
			for(int jv = 0 ; jv < nvm ; jv++)
			{
			

				YS_H_soil_cond(0,ji,jv,Yt) = bqsb[ji][jv];
				YS_H_soil_cond(1,ji,jv,Yt) = gqsb[ji][jv];
				YS_H_soil_cond(2,ji,jv,Yt) = dsg[ji][jv];
				YS_H_soil_cond(3,ji,jv,Yt) = dsp[ji][jv];
				YS_H_soil_cond(4,ji,jv,Yt) = dss[ji][jv];
				YS_H_soil_cond(5,ji,jv,Yt) = runoff[ji][jv] ;
				YS_H_soil_cond(6,ji,jv,Yt) = mean_bqsb[ji][jv] ;
				YS_H_soil_cond(7,ji,jv,Yt) = mean_gqsb[ji][jv] ;
		
			}
		}



	} 

	
  }
	

  //===========================================================================
  //                           backward
  backward ()
  {
	
	
  }

  //===========================================================================

//****************** End of MODULE H_soil_cond.h    ***************************














