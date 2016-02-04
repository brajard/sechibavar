  
/***************************************************************************
                 module  D_aero.h  -  description

Computes the surface drag coefficient, for cases in which it is NOT provided by the coupled
atmospheric model LMDZ. The module first uses the meteorolgical input to calculate the Richardson
Number, which is an indicator of atmospheric stability in the surface layer. The formulation used
to find this surface drag coefficient is dependent on the stability determined. 

 geopotentiel  : zlev * cte_grav

q_cdrag : AERODYNAMIC TRANSFERT COEEFICIENT (Surface drag) 

Output : q-cdrag
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
 
  forward (YREAL wind, YREAL zri, YREAL z0)
  {
    
    double cd_neut, zscf;
    
    double temp2 = z0;
    
    
    if(Yt > 1)
      {
	
	
	/* 7. Computing the drag coefficient
	   We add the height of the vegetation to the level height to take into account
	   that the level seen by the vegetation is actually the top of the vegetation. Then we 
	   we can subtract the displacement height.*/
	cd_neut = pow(ct_karman / log(temp2),2); 
	
	//7.1 Stable case
	if(zri >= 0) 
	  {
	    //zscf = sqrt(1 + cd * abs(zri));
	    zscf = sqrt(1 + cd * zri);
	    YS1=cd_neut/(1 + 3 * cb * zri * zscf);
	    
	    //printf("(%d) zri = %e ***0\n",Yt,zri);
	    
	  }
	else
	  {          
	    //7.2 Unstable case
	    zscf = 1 / (1 + 3 * cb * cc * cd_neut * sqrt( -zri*temp2 ));
	    YS1=cd_neut * (1 - 3 * cb * zri * zscf);
	    
	    //printf("(%d) zri = %e ************1\n",Yt,zri);
	  }
	
	//If the Drag Coefficient becomes too small than the surface may uncouple from the atmosphere.
	//To prevent this, a minimum limit to the drag coefficient is defined as:
	if(YS1 < 1.e-4/wind)
	  YS1 = 1.e-4/wind;
	
	
      }
    else
      YS1 = 0;




}


//===========================================================================
//                           backward

backward (YREAL wind, YREAL zri, YREAL z0)
{
  
  double cd_neut, zscf;	
  double temp2 = z0;
  double d_temp2_z0 = 1;	
  
  double d_cdneut_temp2= 0;
  
  double d_zscf_zri= 0;
  double d_zscf_cdneut= 0;
  double d_zscf_temp2= 0;
  
  double d_YS1_cdneut = 0;
  double d_YS1_zri= 0;
  double d_YS1_zscf= 0;
  double d_YS1_wind = 0;
  
  
  if(Yt > 1)
    {
      
      
      /* 7. Computing the drag coefficient
	 We add the height of the vegetation to the level height to take into account
	 that the level seen by the vegetation is actually the top of the vegetation. Then we 
	 we can subtract the displacement height.*/
      
      
      cd_neut = pow(ct_karman / log(temp2),2); 
      
      d_cdneut_temp2=   -2 * pow(ct_karman,2) / ( temp2 * pow(log(temp2),3) );
      
      
      
      //7.1 Stable case
      if(zri >= 0) 
	{
	  zscf = sqrt(1 + cd * zri);
	  d_zscf_zri= cd / (2*sqrt(1 + cd * zri));
	  
	  d_YS1_cdneut = 1/(1 + 3 * cb * zri * zscf);
	  d_YS1_zri= -3*cd_neut * cb * zscf / pow(3 * cb * zri * zscf + 1,2);
	  d_YS1_zscf= -3*cd_neut * cb * zri / pow(3 * cb * zri * zscf + 1,2);
	  
	  
	  
	  
	}
      else
	{          
	  //7.2 Unstable case
	  zscf = 1 / (1 + 3 * cb * cc * cd_neut * sqrt( -zri*temp2) );
	  
	  
	  d_zscf_zri = 3*cb*cc*cd_neut*sqrt(temp2)/ (2*sqrt(-zri) * pow( 1 + 3 * cb * cc * cd_neut * sqrt( -zri*temp2) ,2));			
	  d_zscf_cdneut= -3 * cb * cc * sqrt( -zri*temp2) / pow(1 + 3 * cb * cc * cd_neut * sqrt( -zri*temp2),2 );
	  d_zscf_temp2 = -(3* sqrt(-zri) * cb * cc * cd_neut ) / ( 2* sqrt(temp2) * pow(3* sqrt(-zri) * cb * cc * cd_neut*sqrt(temp2) + 1  ,2) );
	  
	  d_YS1_cdneut = (1 - 3 * cb * zri * zscf);;
	  d_YS1_zri= -cd_neut * 3 * cb * zscf;
	  d_YS1_zscf= -cd_neut * 3 * cb * zri;
	  
	}
      
      //If the Drag Coefficient becomes too small than the surface may uncouple from the atmosphere.
      //To prevent this, a minimum limit to the drag coefficient is defined as:

      if(YS1 < 1.e-4/wind)
	d_YS1_wind = -1.e-4/(wind*wind);
      
      
      
      YJ1I1 = d_YS1_wind;
      YJ1I2 = d_YS1_zri + d_YS1_zscf * d_zscf_zri;
      YJ1I3 = d_YS1_cdneut * d_cdneut_temp2 * d_temp2_z0 + d_YS1_zscf * d_zscf_temp2 * d_temp2_z0 +  d_YS1_zscf * d_zscf_cdneut * d_cdneut_temp2 * d_temp2_z0;
      
		
      
    }
  else
    {
      YJ1I1 =YJ1I2 =YJ1I3 = 0;
    }
	
  
}



  //===========================================================================

//************************** End of MODULE D_aero.h ********************************








