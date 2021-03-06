  
/***************************************************************************
                 module classe H_veg_water2.h  -  description

!put it into reservoir of plant whose surface area has grown!
Default behaviour is a saturated soil (initialization).

We put the water to redistribute from the PFTs that "shrank" into the reservoirs of PFTs that "growed"
In doing so, water contents are kept constant in PFTs that shrank, and they increase in PFTs that growed
fra is the weight for that redistribution, scaled to vtr which is the total amount to redistribute

                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL vegchtot, YREAL vmr, YREAL gtr, YREAL btr, YREAL qstr, YREAL vtr, YREAL veget, YREAL resdist, YREAL gqsb, YREAL bqsb, YREAL qsintveg, YREAL dss, YREAL ruu_ch, YREAL qsdq, YREAL dsp1, YREAL dsg1, YREAL dpu_cste1)
  {
	double fra = 0, gqsb_m = 0, bqsb_m = 0, qsintveg_m = 0, dsg = 0, dsp = 0;

	if(Yt > 1)
	{
		if ((vegchtot > 0) && (abs(vtr) >  epsilon(1)) )
		{
			fra = vmr / vtr;  //Negative when vmr positive, thus in the condition below 
			if( vmr > 0)      //The PFT growed, thus its water contents must be updated
			{
				if(veget > 0) 
				{
					gqsb_m = (resdist*gqsb + fra*gtr)/veget;
					bqsb_m = (resdist*bqsb + fra*btr)/veget;
				}
				else
				{
					gqsb_m = gqsb;
					bqsb_m = bqsb;
				}			
		      
		      		qsintveg_m = qsintveg + fra* qstr;
			}
			else
			{
				qsintveg_m = qsintveg - qsdq;
				gqsb_m = gqsb;
				bqsb_m = bqsb;
			}
	
			//Then we update the soil layers depths.
		     	//But we do not  change dss, so that the redistribution does directly affect transpiration
			if(gqsb_m < min_sechiba) 
				dsg = 0;
			else
				dsg = (dss * ruu_ch + gqsb_m)/ ruu_ch;
		     
			dsp = dpu_cste1 - bqsb_m / ruu_ch;			
		}
		else
		{
			gqsb_m = gqsb;
			bqsb_m = bqsb;
			qsintveg_m = qsintveg;
			dsg = dsg1;
			dsp = dsp1;	
			
		}
	}
	
	YS1 = gqsb_m;
	YS2 = bqsb_m;
	YS3 = qsintveg_m;
	YS4 = dsg;
	YS5 = dsp;
  }
	

  //===========================================================================
  //                           backward
  backward (YREAL vegchtot, YREAL vmr, YREAL gtr, YREAL btr, YREAL qstr, YREAL vtr, YREAL veget, YREAL resdist, YREAL gqsb, YREAL bqsb, YREAL qsintveg, YREAL dss, YREAL ruu_ch, YREAL qsdq, YREAL dsp1, YREAL dsg1, YREAL dpu_cste1)
  {

	double fra = 0, gqsb_m = 0, bqsb_m = 0, qsintveg_m = 0, dsg = 0, dsp = 0;

	double d_fra_vmr=0;
	double d_fra_vtr=0;

	double d_gqsbm_resdist=0;
	double d_gqsbm_gqsb=0;
	double d_gqsbm_fra=0;
	double d_gqsbm_gtr=0;
	double d_gqsbm_veget=0;

	double d_bqsbm_resdist=0;
	double d_bqsbm_bqsb=0;
	double d_bqsbm_fra=0;
	double d_bqsbm_btr=0;
	double d_bqsbm_veget=0;

	double d_qsintm_qsintveg=0;
	double d_qsintm_fra=0;
	double d_qsintm_qstr=0;
	double d_qsintm_qsdq=0;
	
	double d_dsg_dss=0;
	double d_dsg_gqsbm=0;
	double d_dsg_ruu=0;
	double d_dsg_dsg1 = 0;

	double d_dsp_bqsbm=0;
	double d_dsp_ruu=0;
	double d_dsp_dsp1 = 0;
	double d_dsp_dpu = 0;

	if(Yt > 1)
	{
		if ((vegchtot > 0) && (abs(vtr) >  epsilon(1)) )
		{
			fra = vmr / vtr;  //Negative when vmr positive, thus in the condition below 

			d_fra_vmr= 1/vtr;
			d_fra_vtr = -vmr / pow(vtr,2);

			if( vmr > 0)      //The PFT growed, thus its water contents must be updated
			{
				if(veget > 0) 
				{
					gqsb_m = (resdist*gqsb + fra*gtr)/veget;
					bqsb_m = (resdist*bqsb + fra*btr)/veget;

					d_gqsbm_resdist = gqsb/veget;
					d_gqsbm_gqsb= resdist / veget;
					d_gqsbm_fra= gtr/veget;
					d_gqsbm_gtr= fra / veget;
					d_gqsbm_veget=  -(resdist*gqsb + fra*gtr)/pow(veget,2);
	
					d_bqsbm_resdist= bqsb / veget;
					d_bqsbm_bqsb= resdist / veget;
					d_bqsbm_fra= btr / veget;
					d_bqsbm_btr= fra / veget;
					d_bqsbm_veget= -(resdist*bqsb + fra*btr)/pow(veget,2);
				}
				else
				{
					gqsb_m = gqsb;
					bqsb_m = bqsb;

					d_gqsbm_gqsb= 1;
					d_bqsbm_bqsb= 1;
				}			
		      
		      		qsintveg_m = qsintveg + fra* qstr;

				d_qsintm_qsintveg= 1;
				d_qsintm_fra= qstr;
				d_qsintm_qstr= fra;
			}
			else
			{
				qsintveg_m = qsintveg - qsdq;
				gqsb_m = gqsb;
				bqsb_m = bqsb;

				d_qsintm_qsintveg= 1;
				d_qsintm_qsdq= -1;

			 	d_gqsbm_gqsb= 1;
				d_bqsbm_bqsb= 1;
			}
	
			//Then we update the soil layers depths.
		     	//But we do not  change dss, so that the redistribution does directly affect transpiration
			if(gqsb_m < min_sechiba) 
			{
				dsg = 0;
			}
			else
			{ 
				dsg = (dss * ruu_ch + gqsb_m)/ ruu_ch;

				d_dsg_dss = 1;
				d_dsg_gqsbm = 1/ruu_ch;
				d_dsg_ruu= -gqsb_m /(ruu_ch*ruu_ch);
			}
		     
			dsp = dpu_cste1 - bqsb_m / ruu_ch;

			d_dsp_bqsbm= -1 / ruu_ch;
			d_dsp_ruu= bqsb/(ruu_ch*ruu_ch);
			d_dsp_dpu = 1;

		}
		else
		{
			gqsb_m = gqsb;
			bqsb_m = bqsb;
			qsintveg_m = qsintveg;
			dsg = dsg1;
			dsp = dsp1;	

			d_qsintm_qsintveg= 1;
		 	d_gqsbm_gqsb= 1;
			d_bqsbm_bqsb= 1;
			d_dsg_dsg1 = 1;
			d_dsp_dsp1 = 1;		
			
		}
	}

	YJ1I1 = YJ1I4 =YJ1I5 =YJ1I10 =YJ1I11 = YJ1I12 = YJ1I13 = YJ1I14 = YJ1I15 = YJ1I16 =YJ1I17 =0;
	YJ1I2 = d_gqsbm_fra * d_fra_vmr;
	YJ1I3 = d_gqsbm_gtr;
	YJ1I6 = d_gqsbm_fra * d_fra_vtr;
	YJ1I7 = d_gqsbm_veget;
	YJ1I8 = d_gqsbm_resdist;
	YJ1I9 = d_gqsbm_gqsb;

	YJ2I1 = YJ2I3 =YJ2I5 =YJ2I9 =YJ2I11 = YJ2I12 = YJ2I13 = YJ2I14 = YJ2I15 = YJ2I16 = YJ2I17 =0;
	YJ2I2 = d_bqsbm_fra * d_fra_vmr;
	YJ2I4 = d_bqsbm_btr;
	YJ2I6 = d_bqsbm_fra * d_fra_vtr;
	YJ2I7 = d_bqsbm_veget;
	YJ2I8 = d_bqsbm_resdist;
	YJ2I10 = d_bqsbm_bqsb;

	YJ3I1 = YJ3I3 =YJ3I4 =YJ3I7 =YJ3I8 =YJ3I9 =YJ3I10 =YJ3I12 =YJ3I13 =YJ3I15 =YJ3I16 =YJ3I17 =0;

	YJ3I2 = d_qsintm_fra * d_fra_vmr;
	YJ3I5 = d_qsintm_qstr;
	YJ3I6 = d_qsintm_fra * d_fra_vtr;
	YJ3I11 = d_qsintm_qsintveg;
	YJ3I14 = d_qsintm_qsdq;

	YJ4I1 = YJ4I4 = YJ4I5 = YJ4I10 = YJ4I11 = YJ4I14 = YJ4I15 =  YJ4I17 =  0;
	YJ4I2 = d_dsg_gqsbm * d_gqsbm_fra * d_fra_vmr;
	YJ4I3 = d_dsg_gqsbm * d_gqsbm_gtr;
	YJ4I6 = d_dsg_gqsbm * d_gqsbm_fra * d_fra_vtr;
	YJ4I7 = d_dsg_gqsbm * d_gqsbm_veget;
	YJ4I8 = d_dsg_gqsbm * d_gqsbm_resdist;
	YJ4I9 = d_dsg_gqsbm * d_gqsbm_gqsb;
	YJ4I12 = d_dsg_dss;
	YJ4I13 = d_dsg_ruu;
	YJ4I16 = d_dsg_dsg1;

	YJ5I1 = YJ5I3 = YJ5I5 = YJ5I9 = YJ5I11 = YJ5I12 = YJ5I14 = YJ5I16 = 0;
	YJ5I2 = d_dsp_bqsbm * d_bqsbm_fra * d_fra_vmr;
	YJ5I4 = d_dsp_bqsbm * d_bqsbm_btr;
	YJ5I6 = d_dsp_bqsbm * d_bqsbm_fra * d_fra_vtr;
	YJ5I7 = d_dsp_bqsbm * d_bqsbm_veget;
	YJ5I8 = d_dsp_bqsbm * d_bqsbm_resdist;
	YJ5I10 = d_dsp_bqsbm * d_bqsbm_bqsb;
	YJ5I13 = d_dsp_ruu;
	YJ5I15 = d_dsp_dsp1;
	YJ5I17 = d_dsp_dpu;
	
  }

  //===========================================================================

//**************************End of MODULE H_veg_water2.h ********************************


