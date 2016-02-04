  
/***************************************************************************
                 module  C_z0_rug.h  -  description

    	YS1 = z0		Roughness height (m)	
	YS2 = roughheight;	Roughness
	YS3 = sumveg		Vegetation sum
	YS4 = ave_height	Mean height

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward


  forward (YREAL veget1, YREAL veget, YREAL veget_max, YREAL height, YREAL totfrac_nobio, YREAL z01, YREAL sumveg1, YREAL ave_height1,YREAL z0_over,YREAL z0_c)
  {

	double z0;
	double sumveg;
	double ave_height;
	double roughheight = 0;
	double temp;
	double temp1, temp2;
	double zhdispl;
	double d_veg ;

		if(Yj == 0)
		{
			z0 = veget1* log(z0_bare);
			sumveg = veget1;
			ave_height = 0;

			
		}
		else
		{
			
			if(is_tree[Yj] )  //tree trunks influence the atmosphere even when there are no leaves
				d_veg = veget_max;
			else		  //grasses only have an influence if they are really there!
				d_veg = veget;

			
			if(height*z0_over > z0_bare)
			{
				temp = height*z0_over;
				
			}
			else
			{
				temp = z0_bare;
			}

			
			temp1 = z01 + d_veg * log(temp);
			sumveg = sumveg1 + d_veg;
			ave_height = ave_height1 + veget_max*height;

			

			z0 = temp1;

			if(Yj == nvm-1)
			{
				
				if( sumveg > 0 )
					temp2 = temp1 / sumveg;
				else	
					temp2 = temp1;

				z0 = z0_c * exp((1 - totfrac_nobio) * temp2);

				//Temporarily we compute the zero plane displacement height

				zhdispl = ave_height * height_displacement;

				//In order to get a variable independent of the height of the
				//vegetation we compute what we call the effective roughness height.
				//This is the height over which the roughness acts. It combines the
				//zero plane displacement height and the vegetation height.

				roughheight = ave_height - zhdispl;
			}	

		}

	    	YS1 = z0;	
		YS2 = roughheight;
		YS3 = sumveg;
		YS4 = ave_height;




  }
	

  //===========================================================================
  //                           backward
  backward (YREAL veget1, YREAL veget, YREAL veget_max, YREAL height, YREAL totfrac_nobio, YREAL z01, YREAL sumveg1, YREAL ave_height1,YREAL z0_over,YREAL z0_c)
  {
	double z0;
	double sumveg;
	double ave_height;
	double roughheight;
	double temp;
	double temp1, temp2;
	double zhdispl;
	double d_veg ;

	//derivees
	double d_z0_veget1 = 0;	
	double d_z0_z0c = 0;
	double d_z0_temp2 = 0;
	double d_z0_tot = 0;
	double d_z0_temp1 = 0;

	double d_sumveg_veget1 = 0;
	double d_sumveg_sumveg1 = 0;
	double d_sumveg_dveg = 0;

	double d_dveg_vegetmax = 0;
	double d_dveg_veget = 0;

	double d_temp_z0over = 0;
	double d_temp_height = 0;

	double d_temp1_z01 =0;
	double d_temp1_dveg = 0;
	double d_temp1_temp =0;

	double d_temp2_temp1 = 0;
	double d_temp2_sumveg = 0;

	double d_ave_ave1 =0;
	double d_ave_vegetmax =0;
	double d_ave_height =0;

	double d_zhdispl_ave = 0;

	double d_rug_ave = 0;
	double d_rug_zhdispl = 0;

	//if(Yt > 1)
	//{
		if(Yj == 0)
		{
			z0 = veget1* log(z0_bare);
			sumveg = veget1;
			ave_height = 0;

			d_z0_veget1 = log(z0_bare);
			d_sumveg_veget1 = 1;



		}
		else
		{
			if(is_tree[Yj] )  //tree trunks influence the atmosphere even when there are no leaves
			{
				d_veg = veget_max;
				d_dveg_vegetmax = 1;
				
			}
			else		  //grasses only have an influence if they are really there!
			{
				d_veg = veget;
				d_dveg_veget = 1;
			}

			if(height*z0_over > z0_bare)
			{
				temp = height*z0_over;

				d_temp_z0over = height;
				d_temp_height = z0_over;
			}
			else
				temp = z0_bare;

			

			temp1 = z01 + d_veg * log(temp);
			sumveg = sumveg1 + d_veg;
			ave_height = ave_height1 + veget_max*height;

			d_temp1_z01 =1;
			d_temp1_dveg =  log(temp);
			d_temp1_temp = d_veg /temp;

			d_sumveg_sumveg1 = 1;
			d_sumveg_dveg = 1;

			d_ave_ave1 = 1;
			d_ave_vegetmax = height;
			d_ave_height = veget_max;

						

			z0 = temp1;
			d_z0_temp1 = 1;

			if(Yj == nvm-1)
			{
				
				if( sumveg > 0 )
				{
					temp2 = temp1 / sumveg;

					d_temp2_temp1 = 1/ sumveg;
					d_temp2_sumveg = -temp1 / pow(sumveg,2);
				}
				else	
				{
					temp2 = temp1;

					d_temp2_temp1 = 1;
				}

				z0 = z0_c * exp((1 - totfrac_nobio) * temp2);
	
				d_z0_z0c = exp((1 - totfrac_nobio) * temp2);
				d_z0_temp2 = z0_c *exp((1 - totfrac_nobio) * temp2) * (1 - totfrac_nobio) ;
				d_z0_tot = z0_c * exp((1 - totfrac_nobio) * temp2) * (-temp2);

				//Temporarily we compute the zero plane displacement height

				zhdispl = ave_height * height_displacement;
	
				d_zhdispl_ave = height_displacement;

				/*In order to get a variable independent of the height of the
				vegetation we compute what we call the effective roughness height.
				This is the height over which the roughness acts. It combines the
				zero plane displacement height and the vegetation height.*/

				roughheight = ave_height - zhdispl;

				d_rug_ave = 1;
				d_rug_zhdispl = -1;

				
			}

		

		}
	    	
		//YS1 = z0
		YJ1I1 = d_z0_veget1;
		YJ1I8 = 0;
		YJ1I2 = d_z0_temp2 * d_temp2_temp1 * d_temp1_dveg * d_dveg_veget + d_z0_temp2 * d_temp2_sumveg * d_sumveg_dveg * d_dveg_veget + d_z0_temp1 * d_temp1_dveg * d_dveg_veget ;
		YJ1I3 = d_z0_temp2 * d_temp2_temp1 * d_temp1_dveg * d_dveg_vegetmax + d_z0_temp2 * d_temp2_sumveg * d_sumveg_dveg * d_dveg_vegetmax + d_z0_temp1 * d_temp1_dveg * d_dveg_vegetmax;
		YJ1I4 = d_z0_temp2 * d_temp2_temp1 * d_temp1_temp * d_temp_height + d_z0_temp1 * d_temp1_temp * d_temp_height;
		YJ1I5 = d_z0_tot;
		YJ1I6 = d_z0_temp2 * d_temp2_temp1 * d_temp1_z01 + d_z0_temp1 * d_temp1_z01;
		YJ1I7 = d_z0_temp2 * d_temp2_sumveg * d_sumveg_sumveg1;
		YJ1I9 = d_z0_temp1 * d_temp1_temp * d_temp_z0over;
		YJ1I10 = d_z0_z0c;

		//YS2 =roughheight
		YJ2I3 = d_rug_ave * d_ave_vegetmax + d_rug_zhdispl * d_zhdispl_ave * d_ave_vegetmax;
		YJ2I4 = d_rug_ave * d_ave_height + d_rug_zhdispl * d_zhdispl_ave * d_ave_height;
		YJ2I8 = d_rug_ave * d_ave_ave1 + d_rug_zhdispl * d_zhdispl_ave * d_ave_ave1;

		//YS3 =sumveg
		YJ3I1 =  d_sumveg_veget1;
		YJ3I2 = d_sumveg_dveg * d_dveg_veget;
		YJ3I3 = d_sumveg_dveg * d_dveg_vegetmax;
		YJ3I7 = d_sumveg_sumveg1;
	
		//YS4 = ave_height
		YJ4I3 = d_ave_vegetmax;
		YJ4I4 = d_ave_height;
		YJ4I8 = d_ave_ave1;
	
	
  }

  //===========================================================================

//************************** End of MODULE C_z0_rug.h  **********************



































/*

  forward (YREAL veget1, YREAL veget, YREAL veget_max, YREAL height, YREAL totfrac_nobio, YREAL z01, YREAL sumveg1, YREAL ave_height1)
  {
	double z0, sumveg;
	double ave_height;
	double temp = z0_bare;
	double temp_z0, temp_z02;
	double roughheight ;
	double d_veg = 0, zhdispl = 0;
	
	if(Yt>1)
	{
		if(Yj == 0)
		{
			z0 = veget1 * log(z0_bare);
			//z0 = veget1 * pow(ct_karman/log(10/z0_bare),2);
			sumveg = veget1;
			ave_height = 0;
		}
		else if (Yj == nvm -1)
		{

			if(is_tree[Yj] == 1 ) //tree trunks influence the atmosphere even when there are no leaves
				d_veg = veget_max;
			else		  //grasses only have an influence if they are really there!
				d_veg = veget;
	
			if(height* z0_over > z0_bare)	
			{
				temp = height* z0_over ;
			}

			//temp_z0 = z01 + d_veg * pow(ct_karman/log(10/temp) ,2);

			temp_z0 = z01 + d_veg * log(temp);

			sumveg = sumveg1 + d_veg;

			ave_height = ave_height1 + veget_max*height;




			temp_z02 = temp_z0;
			if(sumveg > 0 )
				temp_z02 = temp_z0 / sumveg;			


			 //z0 = 10/ exp(ct_karman/sqrt(temp_z02));
			z0 = exp((1 - totfrac_nobio) * temp_z02);

			

			     
			// Temporarily we compute the zero plane displacement height     
			zhdispl = ave_height * height_displacement;


			    
			roughheight = ave_height - zhdispl;


			
		
		}
		else
		{
			if(is_tree[Yj] == 1 ) //tree trunks influence the atmosphere even when there are no leaves
				d_veg = veget_max;
			else		  //grasses only have an influence if they are really there!
				d_veg = veget;
	
			if(height* z0_over > z0_bare)	
			{
				temp = height* z0_over ;
			}

			//z0 = z01 + d_veg * pow(ct_karman/log(10/temp) ,2);

			z0 = z01 + d_veg * log(temp);

			sumveg = sumveg1 + d_veg;

			ave_height = ave_height1 + veget_max*height;

		}
	}


	YS1 = z0;	
	YS2 = roughheight;
	YS3 = sumveg;
	YS4 = ave_height;

printf("%d  veget %e     veget_max %e     d_veg %e     height %e      z0 %e     z01 %e     ave_height %e     sumveg %e\n ",Yt, veget, veget_max,d_veg, height, z0,z01,ave_height,sumveg );	
 

  }
	

  //===========================================================================
  //                         methodes  backward
  backward (YREAL veget1, YREAL veget, YREAL veget_max, YREAL height, YREAL totfrac_nobio, YREAL z01, YREAL sumveg1, YREAL ave_height1)
  {
	
	
  }

*/




