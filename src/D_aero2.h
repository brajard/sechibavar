  
/***************************************************************************
                 module  D_aero.h  -  description

Computes the surface drag coefficient, for cases in which it is NOT provided by the coupled
atmospheric model LMDZ. The module first uses the meteorolgical input to calculate the Richardson
Number, which is an indicator of atmospheric stability in the surface layer. The formulation used
to find this surface drag coefficient is dependent on the stability determined. 

 geopotentiel  : zlev * cte_grav

q_cdrag : AERODYNAMIC TRANSFERT COEEFICIENT (Surface drag) 

output : q-cdrag
                             -------------------
 ***************************************************************************/

  //===========================================================================
  //                           forward
  forward (YREAL wind, YREAL zlev, YREAL z0, YREAL roughheight, YREAL temp_sol, YREAL temp_air, YREAL qsurf, YREAL qair)
  {
	double ztvd, ztvs, zdu2;
	double zri, cd_neut, zscf, cd_tmp;

	if(Yt > 1)
        {
			
		//! 3. virtual air temperature at the surface        
		ztvd = (temp_air + (zlev * cte_grav /cp_air) / (1 + rvtmp2 * qair)) * (1 + retv * qair);

		//! 4. virtual surface temperature
		ztvs = temp_sol * (1 + retv * qsurf);
		
		//! 5. squared wind shear   zdu2 = MAX(cepdu2,pow(wind,2));  
		if(pow(wind,2)> cepdu2)   
		{
			zdu2 = pow(wind,2);
		}
		else
		{
			zdu2 = cepdu2;		
		}		
		
		//! 6. Richardson number
		if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) < 5) 
		{
			if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) > -5)
				zri = zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd); // geopotentiel  : zlev * cte_grav
			else
				zri = -5;
		}		
		else 
		{
			zri = 5;
		}

		
		
		/* 7. Computing the drag coefficient
		    We add the height of the vegetation to the level height to take into account
		    that the level seen by the vegetation is actually the top of the vegetation. Then we 
		    we can subtract the displacement height.*/
		cd_neut = pow(ct_karman / log((zlev + roughheight) / z0),2); 
	       
		//7.1 Stable case
		if(zri >= 0) 
		{
			zscf = sqrt(1 + cd * abs(zri));
			cd_tmp=cd_neut/(1 + 3 * cb * zri * zscf);
			
		}
		else
		{          
		 //7.2 Unstable case
			zscf = 1 / (1 + 3 * cb * cc * cd_neut * sqrt(abs(zri)*( (zlev + roughheight) / z0 )));
			cd_tmp=cd_neut * (1 - 3 * cb * zri * zscf);
		}
		
		//If the Drag Coefficient becomes too small than the surface may uncouple from the atmosphere.
		//To prevent this, a minimum limit to the drag coefficient is defined as:		
		if(cd_tmp > 1.e-4/wind)
		{
			YS1 = cd_tmp;
		}
		else
		{
			YS1 = 1.e-4/wind;
		}
		
	}
	else
	{
		YS1 = 0;
	}

	
  }
	

  //===========================================================================
  //                           backward
 
backward (YREAL wind, YREAL zlev, YREAL z0, YREAL roughheight, YREAL temp_sol, YREAL temp_air, YREAL qsurf, YREAL qair)
  {

	double ztvd, ztvs, zdu2;
	double zri, cd_neut, zscf, cd_tmp;
	double temp = 0;

	double d_ztvd_temp_air=0;
	double d_ztvd_zlev=0;
	double d_ztvd_qair=0;

	double d_ztvs_temp_sol=0;
	double d_ztvs_qsurf=0;

	double d_zdu2_wind=0;

	double d_zri_zlev=0;
	double d_zri_ztvd=0;
	double d_zri_ztvs=0;
	double d_zri_zdu2=0;

	double d_cd_neut_zlev=0;
	double d_cd_neut_roughheight=0;
	double d_cd_neut_z0=0;

	double d_zscf_zlev=0;
	double d_zscf_roughheight=0;
	double d_zscf_z0=0;
	double d_zscf_zri=0;
	double d_zscf_cd_neut=0;

	double d_cd_tmp_cdneut=0;
	double d_cd_tmp_zri=0;
	double d_cd_tmp_zscf=0;

	if(Yt > 1)
        {			
		//! 3. virtual air temperature at the surface        
		ztvd = (temp_air + (zlev * cte_grav /cp_air) / (1 + rvtmp2 * qair)) * (1 + retv * qair);
		d_ztvd_temp_air= ( 1 / (1 + rvtmp2 * qair)) * (1 + retv * qair);
		d_ztvd_zlev = ( (cte_grav /cp_air) / (1 + rvtmp2 * qair)) * (1 + retv * qair);
		d_ztvd_qair = (retv*temp_air) + (zlev * cte_grav /cp_air) * (  (retv*(1 + rvtmp2 * qair)  - rvtmp2*(1 + retv * qair))/pow(1 + rvtmp2 * qair,2) ) ;


		//! 4. virtual surface temperature
		ztvs = temp_sol * (1 + retv * qsurf);
		d_ztvs_temp_sol = (1 + retv * qsurf);
	 	d_ztvs_qsurf = temp_sol * retv;

		//! 5. squared wind shear   zdu2 = MAX(cepdu2,pow(wind,2));  
		if(pow(wind,2)> cepdu2)   
		{
			zdu2 = pow(wind,2);
			d_zdu2_wind = 2 * wind;
		}
		else
			zdu2 = cepdu2;	
		
		//! 6. Richardson number		
		if(zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) < 5) 
		{
			if( (zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd)) > -5)
			{
				zri = zlev * cte_grav * (ztvd - ztvs) / (zdu2 * ztvd); // geopotentiel  : zlev * cte_grav
				d_zri_ztvd = zlev * cte_grav *  ( zdu2 * ztvd - zdu2*(ztvd - ztvs) ) / pow(zdu2 * ztvd,2);
				d_zri_zlev = cte_grav * (ztvd - ztvs) / (zdu2 * ztvd) + d_zri_ztvd * d_ztvd_zlev;				
				d_zri_ztvs = -zlev * cte_grav *  (1/ (zdu2 * ztvd));
				d_zri_zdu2 =  -(zlev * cte_grav * (ztvd - ztvs) / ztvd) * (1/(zdu2*zdu2));  
			}
			else
			{
				zri = -5;	
			}
		}		
		else 
		{
			zri = 5;
		}

		
		// 7. Computing the drag coefficient
		//    We add the height of the vegetation to the level height to take into account
		//    that the level seen by the vegetation is actually the top of the vegetation. Then we 
		//    we can subtract the displacement height.

		cd_neut = pow(ct_karman / log((zlev + roughheight) / z0),2); 
		d_cd_neut_zlev = 2 * (ct_karman / log((zlev + roughheight) / z0)) * ( -ct_karman / pow( log((zlev + roughheight) / z0),2)  ) *  (z0/(zlev + roughheight));
		d_cd_neut_roughheight = 2 * (ct_karman / log((zlev + roughheight) / z0)) * ( -ct_karman / pow( log((zlev + roughheight) / z0),2)  ) *  (1/(zlev + roughheight));
		d_cd_neut_z0 = 2 * (ct_karman / log((zlev + roughheight) / z0)) * ( ct_karman / pow( log((zlev + roughheight) / z0),2)  ) * (1/z0);
	       
		//7.1 Stable case
		if(zri >= 0) 
		{
			zscf = sqrt(1 + cd * abs(zri));
			cd_tmp=cd_neut/(1 + 3 * cb * zri * zscf);

			d_zscf_zri = cd/(2*sqrt(1+cd*abs(zri)));
			d_zscf_zlev = d_zscf_zri * d_zri_zlev ;
		
			d_cd_tmp_cdneut = 1/(1 + 3 * cb * zri * zscf);
			d_cd_tmp_zri = (-3 * cb * cd_neut * zscf)/pow(1 + 3 * cb * zri * zscf,2);
			d_cd_tmp_zscf = (-3 * cb * cd_neut * zri)/pow(1 + 3 * cb * zri * zscf,2);
		}
		else
		{          
		 //7.2 Unstable case
			zscf = 1 / (1 + 3 * cb * cc * cd_neut * sqrt(abs(zri)*( (zlev + roughheight) / z0 )));
			cd_tmp=cd_neut * (1 - 3 * cb * zri * zscf);

			//derivada de cd_temp regla de la cadena para z0, zlev y rough zri
			temp = -1 /pow(1 + 3 * cb * cc * cd_neut * sqrt(abs(zri)*((zlev + roughheight)/ z0 )),2) * (0.5)*3*cc*cb*cd_neut*pow(-(zri)*((zlev + roughheight)/z0 ),-0.5 );

			d_zscf_zri = temp * (-(zlev + roughheight)/z0) ;

			d_zscf_cd_neut = -1 /pow(1 + 3 * cb * cc * cd_neut * sqrt(abs(zri)*((zlev + roughheight)/ z0 )),2)   *    3 * cb * cc * sqrt(-(zri)*( (zlev + roughheight) / z0 ));
			d_zscf_zlev = temp * (zri/z0) + d_zscf_zri* d_zri_zlev +  d_zscf_cd_neut * d_cd_neut_zlev ;
			d_zscf_roughheight = temp * (zri/z0) +  d_zscf_cd_neut * d_cd_neut_roughheight  ;
			d_zscf_z0 = -temp * (zlev+roughheight)/pow(z0,2) +  d_zscf_cd_neut * d_cd_neut_z0 ;			
		
			d_cd_tmp_cdneut = 1 - 3 * cb * zri * zscf;
			d_cd_tmp_zri = -cd_neut * 3 * cb * zscf;
			d_cd_tmp_zscf = -cd_neut * 3 * cb * zri;

		}
		
		//If the Drag Coefficient becomes too small than the surface may uncouple from the atmosphere.
		//To prevent this, a minimum limit to the drag coefficient is defined as:
		if(cd_tmp > 1.e-4/wind)
		{
			YJ1I1 = d_cd_tmp_zscf * d_zscf_zri * d_zri_zdu2 * d_zdu2_wind + d_cd_tmp_zri * d_zri_zdu2 * d_zdu2_wind;//
			YJ1I2 = d_cd_tmp_cdneut * d_cd_neut_zlev + d_cd_tmp_zri * d_zri_zlev +  d_cd_tmp_zri * d_zri_ztvd * d_ztvd_zlev;/
			YJ1I3 = d_cd_tmp_cdneut * d_cd_neut_z0 + d_cd_tmp_zscf * d_zscf_z0;
			YJ1I4 = d_cd_tmp_cdneut * d_cd_neut_roughheight + d_cd_tmp_zscf * d_zscf_roughheight;
			YJ1I5 = d_cd_tmp_zri * d_zri_ztvs * d_ztvs_temp_sol + d_cd_tmp_zscf * d_zscf_zri * d_zri_ztvs * d_ztvs_temp_sol ;	
			YJ1I6 = d_cd_tmp_zri * d_zri_ztvd * d_ztvd_temp_air + d_cd_tmp_zscf * d_zscf_zri * d_zri_ztvd * d_ztvd_temp_air ;
			YJ1I7 = d_cd_tmp_zri * d_zri_ztvs * d_ztvs_qsurf + d_cd_tmp_zscf * d_zscf_zri * d_zri_ztvs * d_ztvs_qsurf ;
			YJ1I8 = d_cd_tmp_zri * d_zri_ztvd * d_ztvd_qair + d_cd_tmp_zscf * d_zscf_zri * d_zri_ztvd * d_ztvd_qair ;
		}
		else
		{
			YJ1I1 = -1.e-4/pow(wind,2);
			YJ1I2 = 0;
			YJ1I3 = 0;
			YJ1I4 = 0;
			YJ1I5 = 0;	
			YJ1I6 = 0;
			YJ1I7 = 0;
			YJ1I8 = 0;
		}
		
	}
	else
	{
		YJ1I1 = 0;
		YJ1I2 = 0;
		YJ1I3 = 0;
		YJ1I4 = 0;
		YJ1I5 = 0;	
		YJ1I6 = 0;
		YJ1I7 = 0;
		YJ1I8 = 0;
	}


	
	
  }


