
/***************************************************************************
                 module classe derivfunc.h  -  description
                             -------------------

Description: contains derivative of functions declared in sechiba.h



===========================================================================*/

float qsfrict[370];
float d_qsfrict[370];

//this function interpolates value between ztempmin and ztempmax used for lai detection.
//________________________________________________________________________________________________________________
double d_tempfunc(double temp_in)
{
	double tempfunc_result; // Temperature in degre Kelvin
	double ztempmin=273;    // Temperature for laimin
	double ztempmax=293;    // Temperature for laimax
	double zfacteur;        // Interpolation factor

	zfacteur = 1/pow(ztempmax-ztempmin,2); //0.0025
	if (temp_in > ztempmax)
		tempfunc_result = 1;
	else if(temp_in < ztempmin) 
		tempfunc_result = 0;

	else 
		tempfunc_result = 2*zfacteur*(ztempmax-temp_in);	

	//printf("Yt %d       zfacteur %e    tempfunc %e    temp_in %e \n",Yt,zfacteur,tempfunc_result,temp_in);
   
	return tempfunc_result;
}




//________________________________________________________________________________________________________________
//---------initialises qsfrict array to store precalculated values for qsat by using Goff-Gratch equation---------
void init_qsfrict()
{
	float zrapp,zcorr,ztemperature,zqsat, d_zqsat;

	//The qsfrict_init routine initialises qsfrict array to store precalculated value for qsat

	zrapp = msmlr_h2o/msmlr_air;	
  	zcorr = 0.00320991;

	//computes saturated humidity one time and store in qsfrict local array
	for(int i = 99 ; i < max_temp ; i++)
	{
		ztemperature = float(i);
		if(ztemperature < 273)
		{
			// Equilibrium water vapor - solid
			zqsat = zrapp*pow( 10.0, 2.07023-zcorr*ztemperature -2484.896/ztemperature + 3.56654*log10(ztemperature) );
			//derivee zqsat = u^x --> x' * u^x * ln(u)
			d_zqsat = (-zcorr + 2484.896/pow(ztemperature,2) + 3.56654/(ztemperature * log(10)) ) * zqsat * log(10);
		}
		else
		{
			//Equilibrium water vapor - liquid
			zqsat = zrapp*pow(10.0, 23.8319 - (2948.964/ztemperature) - 5.028*log10(ztemperature) -29810.16*exp(-0.0699382*ztemperature) + 25.21935*exp(-2999.924/ztemperature)); 
			//derivee
			d_zqsat = (   2948.964/pow(ztemperature,2) - 5.028 / (ztemperature* log(10)) + 0.0699382*29810.16*exp(-0.0699382*ztemperature) + 25.21935*2999.924*exp(-2999.924/ztemperature)/pow(ztemperature,2)  ) * zqsat * log(10);
		}

		   			
		qsfrict[i] = zqsat;
		d_qsfrict[i] = d_zqsat;
	}
		
	for(int j = 0; j<100 ; j++)
  		d_qsfrict[j] = qsfrict[j] = 0;		


	
}





//________________________________________________________________________________________________________________
//------------computes  de derivative of qsat with respect to temperature, into successive temperature ------------
double d_dev_qsatcalc_tempsol(double temp_in, double pres_in)
{	
	float d_dev_qsat_out; //output

	float jt, zz_a, zz_b, zz_c, zz_f;

	
	//1. computes qsat interpolation into two successive temperature
	jt = int(float(temp_in)+0.5);

	//Pixels where the temperature is too high 
	if(jt >= max_temp-1)   
		jt = max_temp-1;

	//Pixels where the temperature is too low
	if(jt <= min_temp)   
		jt = min_temp;

	//Temporary variables needed for interpolation
	zz_f = float(temp_in) +0.5 - float(jt);
	zz_a = d_qsfrict[int(jt-1)];
	zz_b = d_qsfrict[int(jt)];
	zz_c = d_qsfrict[int(jt+1)];

	//2. interpolates between this two values to obtain de derivative of qsat
	d_dev_qsat_out = ( (zz_c-2*zz_b+zz_a) * (zz_f-1) + zz_c-zz_b)/pres_in;

	return d_dev_qsat_out;	
}




//________________________________________________________________________________________________________________
//------------computes  de derivative of qsat with respect to pb, into successive temperature ------------
double d_dev_qsatcalc_pb(double temp_in, double pres_in)
{
	
	float d_dev_qsat_out; //output

	float jt, zz_a, zz_b, zz_c, zz_f;

	
	//1. computes qsat interpolation into two successive temperature
	jt = int(float(temp_in)+0.5);

	//Pixels where the temperature is too high 
	if(jt >= max_temp-1)   
		jt = max_temp-1;

	//Pixels where the temperature is too low
	if(jt <= min_temp)   
		jt = min_temp;

	//Temporary variables needed for interpolation
	zz_f = float(temp_in) +0.5 - float(jt);
	zz_a = qsfrict[int(jt-1)];
	zz_b = qsfrict[int(jt)];
	zz_c = qsfrict[int(jt+1)];

	//2. interpolates between this two values
	d_dev_qsat_out = -( (zz_c-2*zz_b+zz_a) * (zz_f-1) + zz_c-zz_b)/pow(pres_in,2);

	//printf("Yt %d     tempin %e    jt %e    jt %d     dev %e   zzf %e    zza %e   zzb %e   zzc %e  \n",Yt,temp_in,jt,int(jt),dev_qsat_out,zz_f,zz_a,zz_b,zz_c );

	return d_dev_qsat_out;	
}




//________________________________________________________________________________________________________________
//------------computes  de derivative of qsat with respect to temperature, into successive temperature ------------
double d_qsatcalc_temp(double temp_in,double pres_in)
{
	float d_qsat_out; //sortie

	float zz_a, zz_b, zz_f, jt;

	//1. computes qsat interpolation into two successive temperature

	//Diagnostic pixels where the temperature is too high 
	jt = int(temp_in);
	if(jt >= max_temp-1)   
		jt = max_temp-1;
	//Diagnostic pixels where the temperature is too low
	if (jt <= min_temp) 
		jt = min_temp;


	zz_f = float(temp_in) -float(jt);
	zz_a = d_qsfrict[int(jt)];
	zz_b = d_qsfrict[int(jt+1)];

	//2. interpolates between this two values
    	d_qsat_out = ((zz_b-zz_a)*zz_f+zz_a)/pres_in;
	
	return d_qsat_out;	
}

//________________________________________________________________________________________________________________
//------------computes  de derivative of qsat with respect to pressure, into successive temperature ------------
double d_qsatcalc_pres_in(double temp_in,double pres_in)
{
	float d_qsat_out; //sortie

	float zz_a, zz_b, zz_f, jt;

	//1. computes qsat interpolation into two successive temperature

	//Diagnostic pixels where the temperature is too high 
	jt = int(temp_in);
	if(jt >= max_temp-1)   
		jt = max_temp-1;
	//Diagnostic pixels where the temperature is too low
	if (jt <= min_temp) 
		jt = min_temp;


	zz_f = float(temp_in) -float(jt);
	zz_a = qsfrict[int(jt)];
	zz_b = qsfrict[int(jt+1)];

	//2. interpolates between this two values
    	d_qsat_out = -((zz_b-zz_a)*zz_f+zz_a)/pow(pres_in,2);
	
	return d_qsat_out;	
}









