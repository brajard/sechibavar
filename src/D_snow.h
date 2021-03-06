
/***************************************************************************
                 module  D_snow.h  -  description
                             -------------------

Description: This routine computes beta coefficient for snow sublimation, which
		integrates the snow on both vegetation and other surface types(ice, lakes,
		cities...) .\n
		The calculation of beta coefficients for each type is multiplied by a factor,
		which relates snow amounts to the atmospheric demand

output: vbeta1

 ***************************************************************************/

//===========================================================================
//                           forward
forward (YREAL totfrac_nobio, YREAL snow, YREAL speed, YREAL q_cdrag, YREAL rau, YREAL qsatt, YREAL qair)
{
  double subtest, zrapp;
  double temp, m;
  
  if(snow/snowcri < 1) 
    m = snow/snowcri;
  else
    m = 1;
  
  if(m > 0)
    temp = (1 - totfrac_nobio) * m;
  else
    temp = 0;
  
    //Limitation of sublimation in case of snow amounts smaller than the atmospheric demand. 
  subtest = dtradia * temp * speed * q_cdrag * rau * ( qsatt - qair );
  
  if( subtest > 0 ) 
    {
      zrapp = snow / subtest;
      if( zrapp < 1 ) 
	YS1 = temp * zrapp;
      else
	YS1 = temp;
    }
  else
    YS1 = temp;
  
  
  
}


  //===========================================================================
  //                           backward
backward (YREAL totfrac_nobio, YREAL snow, YREAL speed, YREAL q_cdrag, YREAL rau, YREAL qsatt, YREAL qair)
{
  double subtest;
  double temp, m;
  

  
  //derivadas
  double d_temp_totfrac=0;
  double d_temp_m=0;
  
  double d_m_snow=0;
  
  double d_subtest_speed=0;
  double d_subtest_q_cdrag=0;
  double d_subtest_rau=0;
  double d_subtest_qsatt=0;
  double d_subtest_qair=0;
  double d_subtest_temp=0;
  
  double d_zrapp_subtest=0;
  
  double d_YS1_temp=0;
  double d_YS1_zrapp=0;
  
  
  if(snow/snowcri < 1)
    {
      m = snow/snowcri;
		d_m_snow = 1/snowcri;
    }
  else
    {
      m = 1;
    }
  
  
  if(m > 0)
    {
      temp = (1 - totfrac_nobio) * m;
		d_temp_totfrac = -m ;
		d_temp_m = 1-totfrac_nobio;
		
		
    }
  else
    {
      temp = 0;
      
    }

  
  //Limitation of sublimation in case of snow amounts smaller than the atmospheric demand. 
  subtest = dtradia * temp * speed * q_cdrag * rau * ( qsatt - qair );
  
  d_subtest_speed = dtradia * temp * q_cdrag * rau * ( qsatt - qair );
  d_subtest_q_cdrag = dtradia * temp * speed  * rau * ( qsatt - qair );
  d_subtest_rau = dtradia * temp * speed * q_cdrag * ( qsatt - qair );
  d_subtest_qsatt = dtradia * temp * speed * q_cdrag * rau ;
  d_subtest_qair = -dtradia * temp * speed * q_cdrag * rau ;
  d_subtest_temp = dtradia *  speed * q_cdrag * rau * ( qsatt - qair );
  
  YJ1I1 = d_YS1_temp * d_temp_totfrac + d_YS1_zrapp * d_zrapp_subtest * d_subtest_temp * d_temp_totfrac;
  YJ1I2 = d_m_snow; 		
  
  YJ1I3 = d_YS1_zrapp * d_zrapp_subtest * d_subtest_speed;
  YJ1I4 = d_YS1_zrapp * d_zrapp_subtest * d_subtest_q_cdrag;
  YJ1I5 = d_YS1_zrapp * d_zrapp_subtest * d_subtest_rau;
  YJ1I6 = d_YS1_zrapp * d_zrapp_subtest * d_subtest_qsatt;
  YJ1I7 = d_YS1_zrapp * d_zrapp_subtest * d_subtest_qair;
  
  
}





  //===========================================================================

//************************** End of MODULE D_snow.h ********************************



