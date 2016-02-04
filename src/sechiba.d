#==========================================================
# 	       fichier de DESCRIPTION sechiba.d
#
#       RESPECTER L'ORDRE DE DECLARATION DES DIRECTIVE S
#==========================================================
#DEFVAL pour definir des trucs ------------------------------------------DEFVAL

#Variations
defval dT 1800. | 30 min en seeg
defval dZ 1. 

#Uptimes
defval Utc 0
defval Utm 1
defval Utp 1 

#Steps time
defval STm   337  |  1 an:17520  |  hiver: 0-4320   |   printemp: 4321 - 8760  |  ete: 8761-13140  | automne : 13140-17520
defval saison 4320   |  d uration en PDT d une saison
#defval modo  1

#Spaces
defval nbdl     11 |  diagnostic levels soil
defval kjindex  1  |  domain size
defval nnobio   1  |  other surfaces: ice, lakes, cities
defval nstm     3  |  soil types
defval nvm      13 |  number of PFT
defval ngrnd    7  |  soil levels

#varios
defval nvm1     10 | nvm-1
defval ngrnd1   6  | ngrnd-1
defval douze    12

defval nbdl1    12 | nbdl + 1
defval nbdl11   13 | nbdl + 2
defval nbdl2    22 | nbdl + nbdl1
defval nbdl3    23 | nbdl + nbdl1+1
defval ST       22 | input array to T_hum_wet = 11*2


#NAME ---------------------------------------------------------------HAT_NAME
hat_name sechiba



#OPTION --------------------------------------------------------------OPTION
#option  O_GRADTEST
#option  O_M1QN3      
option  O_M1QN3 M2QN1
#option  O_M1QN3
option  O_REAL double 
option  O_VARINCR
option  O_EXTOBJ "./include/ncutil.o"
#option  O_DBG_NANF
#option O_CADNA


#TRAJECTOIRES ------(en secondes)-------------------------------------TRAJ
#traj  nom        type upt   offt dt stept
traj  Tcontr       M   Utc            1
traj  Tmodul       M   Utm    0   1  STm





#ESPACES -------------------------------------------------------------SPACE

#espacio modulo target
#space  nom       type    SZX     SZY    SZY    Traj
space  Scontr       M   kjindex                Tcontr
space  SParam       M   kjindex                Tcontr
space  Scontr2      M      2                   Tcontr
space  ScontrN      M   kjindex   nvm          Tcontr
space  ScontrNG     M   kjindex  ngrnd         Tcontr 
space  ScontrN1     M   kjindex   nvm1         Tcontr

space  SParamT       M   kjindex                Tmodul



#Espacios de varios
#space  nom       type  SZX     SZY    SZY      Traj
space  Snvm_init    M   kjindex  nvm           Tmodul     |  init_hydrolc2
space  SkjindexW    M   kjindex                Tmodul     |  wind
#space  Sngrnd_init  M   ngrnd                  Tcontr     |  int_thermosoil
#space  Skjindexn    M   kjindex                Tcontr     |  lskin
#space  Sn1_init     M   nbdl     ngrnd         Tcontr     |  init_humlev
#space  Sn2_init     M   ngrnd    nbdl          Tcontr     |  init_diaglev
#space  Sngrnd_22    M   kjindex  ngrnd         Tcontr     |  int_dz2
#space  Snbdl_hum    M   nbdl     kjindex       Tcontr     |  int_diaghum
#space  Snbdl_init   M   kjindex  nbdl          Tcontr     |  init_slowproc

space  Sngrnd_init  M   ngrnd                  Tmodul     |  int_thermosoil
space  Skjindexn    M   kjindex                Tmodul     |  int_thermosoil
space  Sn1_init     M   nbdl     ngrnd         Tmodul     |  init_humlev
space  Sn2_init     M   ngrnd    nbdl          Tmodul     |  init_diaglev
space  Sngrnd_22    M   kjindex  ngrnd         Tmodul     |  int_dz2
space  Snbdl_hum    M   nbdl     kjindex       Tmodul     |  int_diaghum
space  Snbdl_init   M   kjindex  nbdl          Tmodul     |  init_slowproc





#Espacios de Forçage
#space  nom       type  SZX     SZY    SZY      Traj
space  SkjindexFn  M   kjindex                Tmodul


#Espacios de intersurf
#space nom       type  SZX      SZY    SZY      Traj
space  SkjindexIn M    kjindex                 Tcontr
space  SkjindexIi M    kjindex                 Tmodul


#Espacios de SECHIBA
#space  nom        type  SZX      SZY    SZY      Traj
space  SkjindexSEn  M   kjindex                 Tcontr
space  SkjindexSEi  M   kjindex                 Tmodul
#space  SkjindexSEi1 M   kjindex                 Tmodul

#Espacios de slowproc
#space  nom      type  SZX      SZY    SZY      Traj
space  SnnobioSn   M   kjindex  nnobio         Tcontr
space  SkjindexSn  M   kjindex                 Tcontr
space  SnvmSn      M   kjindex  nvm            Tcontr
space  SnvmSi      M   kjindex  nvm            Tmodul


#Espacios de diffuco
#space  nom      type  SZX      SZY    SZY      Traj
space  SnvmDpre    M   kjindex        	       Tmodul
space  SkjindexDi  M   kjindex                 Tmodul
space  SkjindexD1i M   kjindex                 Tmodul
space  SnvmDi      M   kjindex  nvm            Tmodul
space  SnvmD1i     M   kjindex  nvm            Tmodul


#Espacios de Enerbil
#space  nom      type  SZX      SZY    SZY      Traj
space  SkjindexEi  M   kjindex                 Tmodul
space  SkjindexE1i M   kjindex                 Tcontr
space  SnvmEi      M   kjindex  nvm            Tmodul
space  SdouzeEi    M   kjindex  douze          Tmodul
space  SkjindexE2i M   kjindex                 Tmodul


#Espacios de Hydrolc
space  SnvmHi       M   kjindex  nvm            Tmodul
space  SnvmH1i      M   kjindex  nvm            Tmodul
space  SnvmH2i      M   kjindex  nvm            Tmodul
space  SkjindexHi   M   kjindex                 Tmodul
space  SkjindexH1i  M   kjindex                 Tmodul
space  SkjindexH2i  M   kjindex                 Tmodul
space  SkjindexHis  M   kjindex                 Tmodul
space  SnbdlHi      M   kjindex  nbdl           Tmodul
space  SnvmH1n      M   kjindex  nvm            Tmodul


#Espacios de Thermosoil
#space  nom      type    SZX     SZY    SZZ     Traj
space  SngrndTi     M   kjindex  ngrnd          Tmodul 
space  SngrndTTT    M   kjindex  ngrnd          Tmodul
space  SngrndT2i    M   kjindex  ngrnd          Tmodul
space  SnbdlTi      M   kjindex  nbdl           Tmodul
space  SkjindexTi   M   kjindex                 Tmodul
space  SkjindexT1i  M   kjindex                 Tmodul
#space  S3D          M   kjindex  ngrnd  nbdl    Tmodul
space  S3D2         M   kjindex  ngrnd  nbdl    Tmodul


#Espacios de Condveg
#space  nom        type    SZX     SZY    SZY     Traj
space  Skjindex2Cn  M   kjindex    2             Tcontr
#space  SkjindexCi   M   kjindex                  Tcontr
space  SnvmCi	    M   kjindex    nvm           Tmodul
space  SnvmC2	    M   2	   nvm	         Tmodul

#MODULES -------------------------------------------------------------MODULES	
#######################################################################################################################################
#target modul

#parametres
modul  humr_c_1        space SParam       input 1  output  1         |  target 
modul  humr_c_2        space SParam       input 1  output  1         |  target 
modul  humr_c_3        space SParam       input 1  output  1         |  target 
modul  humr_c_4        space SParam       input 1  output  1         |  target 
modul  humr_c_5        space SParam       input 1  output  1         |  target 
modul  humr_c_6        space SParam       input 1  output  1         |  target 
modul  humr_c_7        space SParam       input 1  output  1         |  target 
modul  humr_c_8        space SParam       input 1  output  1         |  target 
modul  humr_c_9        space SParam       input 1  output  1         |  target 
modul  humr_c_10       space SParam       input 1  output  1         |  target 
modul  humr_c_11       space SParam       input 1  output  1         |  target 
modul  humr_c_12       space SParam       input 1  output  1         |  target 
modul  humr_c_13       space SParam       input 1  output  1         |  target 

modul  rsol_cste      space SParam       input 1  output  1        |  target
modul  so_capa_wet    space SParam       input 1  output  1        |  target 
modul  so_capa_dry    space SParam       input 1  output  1	   |  target 
modul  min_drain      space SParam       input 1  output  1        |  target 
modul  max_drain      space SParam       input 1  output  1	   |  target 
modul  exp_drain      space SParam       input 1  output  1	   |  target 
modul  so_cond        space SParam       input 1  output  1	   |  target 
modul  so_capa        space SParam       input 1  output  1	   |  target 
modul  dpu_cste1      space SParam       input 1  output  1	   |  target 
modul  mx_eau_eau     space SParam       input 1  output  1	   |  target 
modul  rveg_par       space SParam       input 1  output  1	   |  target
modul  z0_par         space SParam       input 1  output  1	   |  target
modul  z0_over        space SParam       input 1  output  1	   |  target
modul  albedovis      space SParam       input 1  output  1	   |  target
modul  albedonir      space SParam       input 1  output  1	   |  target
modul  emis_par       space SParam       input 1  output  1	   |  target

#A controller
modul  emis_c	      space SParam       noward  output  1	      target
modul  rveg_c         space SParam       noward  output  1	      target
modul  z0_c           space SParam       noward  output  1	     target
modul  so_cond_c      space SParam       noward  output  1	      target 
modul  so_capa_c      space SParam       noward  output  1	     target 
modul  albvis_c	      space SParam       noward  output  1	     target
modul  hum_c_12       space SParam       noward  output  1          target 

modul  rsol_c         space SParam       noward  output  1       |    target
modul  dpu_c          space SParam       noward  output  1	 |    target 
modul  mx_eau_c       space SParam       noward  output  1	 |    target


modul  hum_c_1        space SParam       noward  output  1       |    target 
modul  hum_c_2        space SParam       noward  output  1       |    target 
modul  hum_c_3        space SParam       noward  output  1       |    target 
modul  hum_c_4        space SParam       noward  output  1       |    target 
modul  hum_c_5        space SParam       noward  output  1       |    target 
modul  hum_c_6        space SParam       noward  output  1       |    target 
modul  hum_c_7        space SParam       noward  output  1       |    target 
modul  hum_c_8        space SParam       noward  output  1       |    target 
modul  hum_c_9        space SParam       noward  output  1       |    target 
modul  hum_c_10       space SParam       noward  output  1       |    target 
modul  hum_c_11       space SParam       noward  output  1       |    target 
modul  hum_c_13       space SParam       noward  output  1       |    target 




# conditions initiales target
#modul  h_init         space Scontr       noward  output  1  | target
modul  bqsb_c         space Scontr       noward  output  1  |  target
modul  gqsb_c         space Scontr       noward  output  1   | target



#parametros no controlados
modul  z0_over_c      space Scontr       noward  output  1	     
modul  so_wet_c       space Scontr       noward  output  1       
modul  so_dry_c       space Scontr       noward  output  1
#modul  z0_dummy       space Scontr       noward  output  1
modul  e_drain_c      space Scontr       noward  output  1	    
modul  mx_drain_c     space Scontr       noward  output  1	
modul  min_drain_c    space SParam       noward  output  1  
modul  albnir_c	      space SParam       noward  output  1

#otros parametros
modul  eaumax      space Scontr       noward  output  1	     
modul  eaumin      space Scontr       noward  output  1       
modul  hummax      space Scontr       noward  output  1
modul  hummin      space Scontr       noward  output  1
	     



#cout modules
modul  E_tempsol       space SkjindexEi  input  2  output 1   tempo   cout
modul  E_emis_tempsol  space SkjindexE2i input  1  output 1   tempo    | cout

modul  H_watercontent  space SkjindexH1i input  6  output 1   tempo   | cout
modul  H_soil_humrel1  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel2  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel3  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel4  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel5  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel6  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel7  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel8  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel9  space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel10 space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel11 space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel12 space SkjindexH2i input  2  output 1   tempo  | cout
modul  H_soil_humrel13 space SkjindexH2i input  2  output 1   tempo  | cout

modul  E_netrad	       space SkjindexEi  input  2  output 1   tempo   |cout	
modul  E_fluxsens      space SkjindexEi  input  2  output 1   tempo  |  cout 
modul  E_fluxlat       space SkjindexEi  input  2  output 1   tempo   | cout 
modul  E_evapnu        space SkjindexEi  input  2  output 1   tempo  | cout 
modul  E_transpir      space SnvmEi      input  2  output 1   tempo  | cout 


#modules pour initialiser
modul  fluxsens		space Scontr       noward  output  1         	
modul  fluxlat		space Scontr       noward  output  1               
modul  temp_sol		space Scontr       noward  output  1	 	 
modul  qsintveg		space ScontrN      noward  output  1    
modul  ptn		space ScontrNG     noward  output  1     
modul  netrad		space Scontr       noward  output  1     
modul  evapnu		space Scontr       noward  output  1      	
modul  transpir		space ScontrN      noward  output  1   
modul  bqsb_init	space ScontrN      noward  output  1    
modul  gqsb_init	space ScontrN      noward  output  1  


#conditions init humidite
modul  h_c_1          space Scontr       noward  output  1      
modul  h_c_2          space Scontr       noward  output  1        
modul  h_c_3          space Scontr       noward  output  1        
modul  h_c_4          space Scontr       noward  output  1        
modul  h_c_5          space Scontr       noward  output  1        
modul  h_c_6          space Scontr       noward  output  1         
modul  h_c_7          space Scontr       noward  output  1          
modul  h_c_8          space Scontr       noward  output  1          
modul  h_c_9          space Scontr       noward  output  1         
modul  h_c_10         space Scontr       noward  output  1         
modul  h_c_11         space Scontr       noward  output  1          
modul  h_c_12         space Scontr       noward  output  1          
modul  h_c_13         space Scontr       noward  output  1    


###############################################################################################################################

#Operaciones varias
modul  wind            space SkjindexW   input  2  output 1   tempo 
modul  init_hydrolc2   space Snvm_init   input  8  output 3   tempo
modul  int_lskin       space Skjindexn    input  1  output 1  tempo
modul  int_thermosoil  space Sngrnd_init input  5  output 4   tempo 
modul  int_diaglev     space Sn2_init    input  6  output 2   tempo
modul  int_humlev      space Sn1_init    input  7  output 3   tempo
modul  int_dz2         space Sngrnd_22   input  array 21   output 3  tempo
modul  int_slowproc    space Snbdl_init  input  7  output 3   tempo
modul  int_diaghum     space Snbdl_hum   input array 11    output 1   tempo 



#donnes ficher entree
modul  F_u            space SkjindexFn  input 0    output 1   tempo 
modul  F_v            space SkjindexFn  input 0    output 1   tempo 
modul  F_qair         space SkjindexFn  input 0    output 1   tempo 
modul  F_pb           space SkjindexFn  input 0    output 1   tempo 
modul  F_lwdown       space SkjindexFn  input 0    output 1   tempo 
modul  F_precip       space SkjindexFn  input 0    output 1   tempo 
modul  F_temp_air     space SkjindexFn  input 0    output 1   tempo 
modul  F_swdown       space SkjindexFn  input 0    output 1   tempo
modul  F_precip_snow  space SkjindexFn  input 0    output 1   tempo

#intersurf
#modul  I_zlev         space SkjindexIn  input 0    output 1             
#modul  I_contfrac     space SkjindexIn  noward    output 1       
#modul  I_ccanopy      space SkjindexIn  noward    output 1        
#modul  I_comb         space SkjindexIi  input  3  output 1   tempo   

modul  I_petAcoef     space SkjindexIn  input 0    output 1  
modul  I_peqAcoef     space SkjindexIn  input 0    output 1 
modul  I_swnet        space SkjindexDi  input  3  output 1   tempo 
modul  I_epot_air     space SkjindexIi  input  1  output 1   tempo 
modul  I_petBcoef     space SkjindexIi  input  1  output 1   tempo 
modul  I_peqBcoef     space SkjindexIi  input  1  output 1   tempo 


#SECHIBA
modul  SE_rau         space SkjindexSEi  input  2  output 1   tempo 
#modul  SE_evaplim     space SkjindexSEn  input 0    output 1     
modul  SE_routing     space SkjindexSEn  input 0    output 4    

#SLOWPROC
modul  S_fracnobio    space SkjindexSn	input	0	output 2    
modul  S_height       space SnvmSn	input	0	output 1  
modul  S_vegetmax     space SnvmSn	input	0	output 1    
#modul  S_deadleaf     space SkjindexSn  noward		output 1   
modul  S_veget        space SnvmSi	input	3	output 2   tempo 
modul  S_qsintm       space SnvmSi	input	2	output 1   tempo 
modul  S_lai          space SnvmSi	input	14	output 1   tempo 


#DIFFUCO
modul  D_pre_aero     space SnvmDpre    input  2  output 1   tempo
modul  D_aero 	      space SkjindexDi  input  3  output 1   tempo 
modul  D_aero1 	      space SkjindexDi  input  5  output 1   tempo 
#modul  D_raerod      space SkjindexDi  input  2  output 1   tempo 
modul  D_qsatt        space SkjindexDi  input  2  output 1   tempo 
modul  D_snow         space SkjindexDi  input  7  output 1   tempo
modul  D_bare	      space SkjindexDi  input  4  output 1   tempo 
modul  D_inter	      space SnvmDi      input  8  output 2   tempo 
modul  D_pre_trans    space SnvmDi      input  6  output 2   tempo 
modul  D_pre_trans2   space SnvmDi      input  3  output 1   tempo 
modul  D_trans        space SnvmDi      input  10 output 1   tempo 
modul  D_comb_vbsum   space SnvmDi      input  4  output 2   tempo 
modul  D_comb_vb      space SkjindexD1i input  7  output 3   tempo 
modul  D_comb_soil    space SkjindexD1i input  6  output 4   tempo 
modul  D_comb_veg     space SnvmD1i     input  10 output 2   tempo 
modul  D_comb_verif   space SnvmD1i     input  4  output 2   tempo 


#ENERBIL
modul  E_dev_qsol      space SkjindexEi  input  2  output 1   tempo 
modul  E_beg_qsol      space SkjindexEi  input  4  output 3   tempo 
modul  E_beg_netrad    space SkjindexEi  input  4  output 2   tempo 
modul  E_surftemp1     space SkjindexEi  input  3  output 1   tempo 
modul  E_surftemp2     space SkjindexEi  input  10 output 3   tempo 
modul  E_surftemp3     space SkjindexEi  input  9  output 4   tempo 
modul  E_surftemp4     space SkjindexEi  input  10 output 1   tempo 
modul  E_surftemp_pt   space SkjindexEi  input  2  output 2   tempo
modul  E_surftemp_qsol space SkjindexEi  input  3  output 1   tempo
modul  E_surftemp_epot space SkjindexEi  input  5  output 1   tempo
modul  E_surftemp_qair space SkjindexEi  input  11 output 1   tempo
modul  E_flux1         space SkjindexEi  input  2  output 1   tempo 
modul  E_flux_tslw     space SkjindexEi  input  4  output 2   tempo 
modul  E_flux_qsurf    space SkjindexEi  input  6  output 1   tempo 
modul  E_flux_net      space SkjindexEi  input  3  output 2   tempo 	
modul  E_flux_vefl     space SkjindexEi  input  7  output 2   tempo 
modul  E_flux_flta     space SkjindexEi  input  4  output 2   tempo
modul  E_flux_evapo    space SkjindexEi  input  4  output 1   tempo 
modul  E_flux_corr     space SkjindexEi  input  9  output 1   tempo 
modul  E_grad_qsat     space SkjindexEi  input  2  output 1   tempo  
modul  E_evap_veva     space SkjindexEi  input  7  output 3   tempo 
modul  E_evap_trans    space SnvmEi      input  3  output 3   tempo 
modul  E_emis_planck   space SdouzeEi    input  2  output 1   tempo 
modul  E_emis_rad      space SkjindexE2i  input  3  output 1   tempo 




#modul  E_totmelt       space SkjindexE1i noward    output 1    


#HYDROLC
modul  H_snow_i       space SkjindexHis input  3  output 1   tempo 
modul  H_snow         space SkjindexHis input  4  output 3   tempo 
modul  H_snow_melt_i  space SkjindexHis input  5  output 1   tempo 
modul  H_snow_melt    space SkjindexHis input  5  output 3   tempo 
modul  H_snow_age     space SkjindexHis input  6  output 3   tempo 
modul  H_veg_var      space SnvmHi      input  3  output 2   tempo 
modul  H_veg_sum      space SnvmHi      input  2  output 1   tempo 
modul  H_veg_niv      space SnvmHi      input  5  output 3   tempo 	
modul  H_veg_water    space SnvmHi      input  9  output 4   tempo 
modul  H_veg_water2   space SnvmHi      input 17  output 5   tempo 
modul  H_veg_upd      space SnvmHi      input  3  output 1   tempo 
modul  H_veg_cond     space SnvmHi      input 11  output 5   tempo 
modul  H_canop_init   space SnvmHi      input  4  output 1   tempo 
modul  H_canop_interp space SnvmHi      input  5  output 2   tempo 
modul  H_soil_flx     space SnvmHi      input  3  output 2   tempo 
modul  H_soil_flxcorr space SnvmHi      input  5  output 1   tempo 
modul  H_soil_trans   space SnvmHi      input  9  output 2   tempo 
modul  H_soil_runoff  space SnvmHi      input  9  output 7   tempo 
modul  H_soil_dlock   space SnvmHi      input  10 output 5   tempo 
modul  H_soil_diff    space SnvmHi      input  7  output 3   tempo 
modul  H_soil_dsp     space SnvmHi      input  6  output 2   tempo 
modul  H_soil_verif   space SnvmHi      input  15 output 7   tempo 
modul  H_soil_mean    space SnvmHi      clonol H_soil_diff  
modul  H_soil_dsp2    space SnvmHi      input  7  output 2   tempo 	
modul  H_soil_runofft space SnvmHi      input  6  output 2   tempo 
modul  H_soil_dsg1    space SnvmHi      input  4  output 1   tempo
modul  H_soil_dsg     space SkjindexHi  input  4  output 1   tempo 
modul  H_soil_stress  space SnvmH1i     input  20 output 4   tempo
modul  H_soil_humrel  space SnvmH2i     input  13 output 1   tempo  
modul  H_soil_shum    space SnbdlHi     input  6  output 1   tempo 
modul  H_soil_dryfrac space SkjindexH1i input  1  output 1   tempo 
modul  H_soil_hdry    space SkjindexH1i input  3  output 1   tempo 
modul  H_soil_rsol    space SkjindexH1i input  5  output 2   tempo 



#------------------------------------------------------------------------------
#modul  H_soil_cond    space SnvmHi      input  0  output 8   tempo 
#------------------------------------------------------------------------------


#THERMOSOIL
modul T_hum_wet       space SngrndTi      input  array nbdl2  output 1   tempo |hidjac
modul T_profile       space SngrndTi      input  8  output 1   tempo 
modul T_diag_stemp    space SnbdlTi       input  array 14  output 1   tempo |hidjac
modul  T_energy       space SkjindexTi    input  4  output 3   tempo     
modul  T_coef_soil    space SngrndTTT     input  10 output 3   tempo  
modul  T_coef_soil2   space SngrndT2i     input  6  output 2   tempo   	
modul  T_coef_diff    space SkjindexT1i   input  7  output 2   tempo   	


#CONDVEG
modul  C_albcalc3     space Skjindex2Cn  input 0  output 1    
#modul  C_zol_init     space SkjindexCi   input 0  output 2    
#modul  C_zol_calc     space SkjindexCi   input 0  output 1    

modul  C_z0_rug       space SnvmCi       input 10 output 4      tempo  	| mismo espacio que S_veget
modul  C_albedo       space SnvmC2       input 5  output 2      tempo  	|



#CONNECTIONS ------------------------------------------------------CTIN-M

#--------------------
#---INITIALISATION---
#--------------------

#---|------modul-----|# in|---modul from-----|#out|space|pdt|nom in | sortie du module


ctin I_epot_air         1  from F_temp_air     1  i          t    | tair	|
#ctin I_epot_air         2  from I_zlev         1  i               | zlev	|-->epot_air

ctin I_petBcoef         1  from I_epot_air     1  i          t    | epot_air	|-->petBcoef

ctin I_peqBcoef         1  from F_qair         1  i          t    | qair	|-->peqBcoef

#ctin I_comb             1  from I_zlev         1  i               | zlev	|
#ctin I_comb             2  from C_zol_init     1  i               | z0		|--> temp
#ctin I_comb             3  from C_zol_init     2  i               | roughheight	|



#--------------------
#---PARAMETRES DE CONTROLE---
#--------------------
#---|--modul-----------|# in|--modul from----|#out|space|pdt     |nom in | sortie du module

ctin rsol_cste            1  from rsol_c        1  1             | rsol_cste	 |--> rsol_cste
ctin so_capa_wet          1  from so_wet_c      1  1             | so_capa_wet 	 |--> so_capa_wet 
ctin so_capa_dry          1  from so_dry_c      1  1             | so_capa_dry 	 |--> so_capa_dry
ctin min_drain            1  from min_drain_c   1  1             | min_drain 	 |--> min_drain
ctin max_drain            1  from mx_drain_c    1  1             | max_drain 	 |--> max_drain
ctin exp_drain            1  from e_drain_c     1  1             | exp_drain 	 |--> exp_drain
ctin so_capa              1  from so_capa_c     1  1             | so_capa 	 |--> so_capa
ctin so_cond              1  from so_cond_c     1  1             | so_cond 	 |--> so_cond
ctin dpu_cste1            1  from dpu_c         1  1             | dpu_cste1 	 |--> dpu_cste1 
ctin mx_eau_eau           1  from mx_eau_c      1  1             | mx_eau_eau 	 |--> mx_eau_eau
ctin rveg_par             1  from rveg_c        1  1             | rveg_c	 |--> rveg_par
ctin z0_par               1  from z0_c          1  1             | z0_c	         |--> z0_par
ctin z0_over              1  from z0_over_c     1  1             | z0_over_c     |--> z0_over
ctin albedovis            1  from albvis_c      1  1             | albvis_c      |--> albedovis
ctin albedonir            1  from albnir_c      1  1             | albnir_c      |--> albedonir
ctin emis_par             1  from emis_c        1  1             | emis_c        |--> emis_par

ctin humr_c_1             1  from hum_c_1        1  1             | hum_c_1    |--> humr_c_1
ctin humr_c_2             1  from hum_c_2        1  1             | hum_c_2    |--> humr_c_2
ctin humr_c_3             1  from hum_c_3        1  1             | hum_c_3    |--> humr_c_3
ctin humr_c_4             1  from hum_c_4        1  1             | hum_c_4    |--> humr_c_4
ctin humr_c_5             1  from hum_c_5        1  1             | hum_c_5    |--> humr_c_5
ctin humr_c_6             1  from hum_c_6        1  1             | hum_c_6    |--> humr_c_6
ctin humr_c_7             1  from hum_c_7        1  1             | hum_c_7    |--> humr_c_7
ctin humr_c_8             1  from hum_c_8        1  1             | hum_c_8    |--> humr_c_8
ctin humr_c_9             1  from hum_c_9        1  1             | hum_c_9    |--> humr_c_9
ctin humr_c_10             1  from hum_c_10      1  1             | hum_c_10   |--> humr_c_10
ctin humr_c_11             1  from hum_c_11      1  1             | hum_c_11   |--> humr_c_11
ctin humr_c_12             1  from hum_c_12      1  1             | hum_c_12   |--> humr_c_12
ctin humr_c_13             1  from hum_c_13      1  1             | hum_c_13   |--> humr_c_13


#--------------------
#------SLOWPROC------
#--------------------

ctin S_lai           1  from S_lai        1    i   j    t-1		| lai		|
ctin S_lai           2  from T_diag_stemp 1    i   1    t-1   		| diagstemp	|
ctin S_lai           3  from T_diag_stemp 1    i   2    t-1  		| diagstemp	|
ctin S_lai           4  from T_diag_stemp 1    i   3    t-1   		| diagstemp	|
ctin S_lai           5  from T_diag_stemp 1    i   4    t-1   		| diagstemp	|
ctin S_lai           6  from T_diag_stemp 1    i   5    t-1   		| diagstemp	|
ctin S_lai           7  from T_diag_stemp 1    i   6    t-1   		| diagstemp	|--> lai
ctin S_lai           8  from T_diag_stemp 1    i   7    t-1   		| diagstemp	|
ctin S_lai           9  from T_diag_stemp 1    i   8    t-1   		| diagstemp	|
ctin S_lai           10 from T_diag_stemp 1    i   9    t-1   		| diagstemp	|
ctin S_lai           11 from T_diag_stemp 1    i   10   t-1   		| diagstemp	|
ctin S_lai           12 from T_diag_stemp 1    i   11   t-1   		| diagstemp	|
ctin S_lai           13 from int_slowproc 2    i   nbdl   t    		| lcanop	|
ctin S_lai           14 from temp_sol     1  i	             		| temp_sol_init	|


ctin S_veget         1  from S_vegetmax   1    i   j          		| vegetmax	|
ctin S_veget         2  from S_lai        1    i   j    t     		| lai 		|--> veget
ctin S_veget         3  from S_veget      2    i   j+1  t     		| veget_diff 	|

ctin S_qsintm        1  from S_veget      1    i   j    t-1     	| veget		|--> qsintmax
ctin S_qsintm        2  from S_lai        1    i   j    t-1     	| lai 		|

ctin C_z0_rug        1  from S_veget      1    i   1    t   		| veget1	|
ctin C_z0_rug        2  from S_veget      1    i   j    t    		| veget		|
ctin C_z0_rug        3  from S_vegetmax   1    i   j           		| vegetmax	|--> 1) z0
ctin C_z0_rug        4  from S_height     1    i   j           		| height	|--> 2) roughheight
ctin C_z0_rug        5  from S_fracnobio  2    i    		  	| totfrac_nobio	|--> 3) sumveg1
ctin C_z0_rug        6  from C_z0_rug     1    i   j-1  t      		| z01		|--> 4) ave_height1
ctin C_z0_rug        7  from C_z0_rug     3    i   j-1  t      		| sumveg1	|
ctin C_z0_rug        8  from C_z0_rug     4    i   j-1  t      		| ave_height1	|
ctin C_z0_rug        9  from z0_over	  1    i               		| z0_over	|
ctin C_z0_rug        10 from z0_par	  1    i               		| z0		|

ctin C_albedo        1  from S_veget      1    1   j    t    		| veget		|
ctin C_albedo        2  from C_albedo     1    i   j-1  t      		| albedo	|--> 1) albedo (nis y vir)
ctin C_albedo        3  from C_albedo     2    i   j-1  t      		| alb_veget	|--> 2) alb_veget
ctin C_albedo        4  from albedovis    1    1               		| albvir_c	|
ctin C_albedo        5  from albedonir    1    1              		| albnir_c	|


ctin I_swnet            1  from C_albedo  1  1   nvm    t-1       	| albedo vis	|
ctin I_swnet            2  from C_albedo  1  2   nvm    t-1       	| albedo nir	|--> swnet
ctin I_swnet            3  from F_swdown  1  i          t    		| swdown	|
#ctin I_swnet            1  from F_swdown 1  i          t   		| swdown	|--> swnet


#--------------------
#------SECHIBA ------
#--------------------

ctin SE_rau          1  from F_temp_air   1    i        t-1     | temp_air	|--> rau
ctin SE_rau          2  from F_pb         1    i        t-1     | pb		|


#--------------------
#------VARIAS--------
#--------------------

ctin wind            1  from F_u            1  i            t    | u
ctin wind            2  from F_v            1  i            t    | v

#ctin int_lskin      1  from so_cond         1  1          	| so_cond	|
#ctin int_lskin      2  from so_capa         1  1          	| so_capa	|--> 1)lskin
ctin int_lskin      1  from int_lskin       1  i	t-1    	| lskin		|




ctin int_thermosoil 1  from int_lskin       1  1        t  	| lskin   	|
ctin int_thermosoil 2  from int_thermosoil  1  i        t-1  	| dz1   	|--> 1) dz1     2) dz2
ctin int_thermosoil 3  from int_thermosoil  2  i        t-1 	| dz2   	|--> 3) zzcoef  4) lambda
ctin int_thermosoil 4  from int_thermosoil  3  i        t-1  	| zzcoef   	|
ctin int_thermosoil 5  from int_thermosoil  4  i        t-1  	| lambda   	|


ctin int_slowproc   1  from dpu_cste1       1  i	     	| dpu_cste1	|
ctin int_slowproc   2  from int_slowproc    2  i  j-1	  t   	| temp1		|--> 1) diaglev
ctin int_slowproc   3  from int_slowproc    3  i  j-1	  t   	| min_val1	|--> 2) lcanop
ctin int_slowproc   4  from int_slowproc    1  i  j-1	  t   	| diaglev1	|
ctin int_slowproc   5  from int_slowproc    1  i  j	  t-1  	| diaglev	|
ctin int_slowproc   6  from int_slowproc    2  i  j 	  t-1  	| lcanop	|
ctin int_slowproc   7  from int_slowproc    3  i  j 	  t-1  	| minval	|

ctin int_diaghum 1..11 from int_slowproc   1   1  1..11    t     | diaglev	|--> 1) diaglev

ctin int_dz2     1..7   from int_thermosoil  1  1..7       t     | dz1		|--> 1) dz1
ctin int_dz2     8..14  from int_thermosoil  2  1..7       t     | dz2		|--> 2) dz2
ctin int_dz2     15..21 from int_thermosoil  3  1..7       t     | zz_coef	|--> 3) zz_coef

ctin int_diaglev    1  from int_thermosoil  2  i       	t	| dz2		|
ctin int_diaglev    2  from int_diaglev     2  i-1  j   t   	| lev_prog1	|--> 1) intfactw_s
ctin int_diaglev    3  from int_slowproc    1  1    j   t   	| diaglev	|--> 2) lev_prog1
ctin int_diaglev    4  from int_slowproc    1  1    j-1 t     	| diaglev	|
ctin int_diaglev    5  from int_diaglev     1  i    j   t-1   	| intfactw_s	|
ctin int_diaglev    6  from int_diaglev     2  i    j   t-1   	| lev_prog1	|


ctin int_humlev     1  from int_dz2         2  1    j    t	| dz2		|
ctin int_humlev     2  from int_humlev      2  i-1    j  t    	| lev_prog1	|
ctin int_humlev     3  from int_humlev      3  i  j-1    t  	| lev_diag1	|--> 1) intfactw_s
ctin int_humlev     4  from int_diaghum     1  i  1      t	| diaglev	|--> 2) lev_prog1
ctin int_humlev     5  from int_humlev      1  i  j     t-1 	| intfactw_s	|
ctin int_humlev     6  from int_humlev      2  i  j     t-1 	| lev_prog1	|
ctin int_humlev     7  from int_humlev      3  i  j     t-1 	| lev_prog1	|

#ctin init_hydrolc2  1  from S_vegetmax     1  i  j         	| vegetmax	|
ctin init_hydrolc2   1  from S_veget        1  i  j        t    | veget		|
ctin init_hydrolc2   2  from init_hydrolc2  1  i  j-1      t 	| vegtot1	|
ctin init_hydrolc2   3  from init_hydrolc2  3  i  j-1      t 	| mx_eau_var1	|--> 1) vegtot
ctin init_hydrolc2   4  from mx_eau_eau     1  i           	| mx_eau_eau	|--> 2) ruu_ch
ctin init_hydrolc2   5  from dpu_cste1      1  i            	| dpu_cste	|--> 3) mx_eau_var
ctin init_hydrolc2   6  from init_hydrolc2  1  i  j        t-1	| vegtot_t1	|
ctin init_hydrolc2   7  from init_hydrolc2  2  i  j        t-1 	| ruu_ch_t1	|
ctin init_hydrolc2   8  from init_hydrolc2  3  i  j        t-1 	| mx_eau_var_t1	|




#--------------------
#------DIFFUCO-------
#--------------------

#ctin D_pre_aero      1  from z0_par	    1  i            | z0_par		|--> temp (para D_aero)	
ctin D_pre_aero      1  from C_z0_rug	    1  i   nvm   t  | z0		|--> temp (para D_aero)	
ctin D_pre_aero      2  from C_z0_rug	    2  i   nvm   t  | rought		|

ctin D_aero1         1  from wind           1  i      t-1    | wind		|
ctin D_aero1         2  from E_tempsol      1  i      t-1    | temp_sol_new	|
ctin D_aero1         3  from F_temp_air     1  i      t-1    | temp_air		|-->zri
ctin D_aero1         4  from E_flux_qsurf   1  i      t-1    | qsurf		|
ctin D_aero1         5  from F_qair         1  i      t-1    | qair		|

ctin D_aero          1  from wind           1  i      t-1    | wind		|-->q_cdrag	
ctin D_aero          2  from D_aero1	    1  i      t      | zri		|
ctin D_aero          3  from D_pre_aero     1  i      t      | z0+rug		|
#ctin D_aero         3  from z0_par	    1  i             | z0		|

#ctin D_raerod        1  from wind           1  i      t-1    | wind		|
#ctin D_raerod        2  from D_aero         1  i      t      | q_cdrag		|--> raero

ctin D_qsatt         1  from E_tempsol      1  i      t-1    | temp_sol_new	|
ctin D_qsatt         2  from F_pb           1  i      t-1    | pb		|--> qsatt

ctin D_snow          1  from S_fracnobio    2  i             | totfrac_nobio	|
ctin D_snow          2  from H_snow_melt    1  i      t-1    | snow		|
ctin D_snow          3  from wind           1  i      t-1    | wind		|
ctin D_snow          4  from D_aero         1  i      t      | q_cdrag		|--> vbeta1
ctin D_snow          5  from SE_rau         1  i      t      | rau		|
ctin D_snow          6  from D_qsatt        1  i      t      | qsatt		|
ctin D_snow          7  from F_qair         1  i      t-1    | qair		|

ctin D_inter         1  from F_qair         1  i      t-1    | qair		|
ctin D_inter         2  from D_qsatt        1  i      t      | qsatt		|
ctin D_inter         3  from SE_rau         1  i      t      | rau		|
ctin D_inter         4  from wind           1  i      t-1    | wind		|--> 1) vbeta2
ctin D_inter         5  from D_aero         1  i      t      | q_cdrag		|--> 2) vbeta23
ctin D_inter         6  from S_veget        1  i  j   t-1    | veget		|
ctin D_inter         7  from H_canop_interp 2  i  j   t-1    | qsintveg		|
ctin D_inter         8  from S_qsintm       1  i  j   t-1    | qsintmax		|

ctin D_bare          1  from D_aero         1  i      t      | q_cdrag		|
ctin D_bare          2  from wind           1  i      t-1    | wind		|
ctin D_bare          3  from H_soil_rsol    1  i      t-1    | rsol		|--> vbeta4
ctin D_bare          4  from S_veget        1  i  1   t-1    | veget		|

ctin D_pre_trans     1  from F_temp_air     1  i      t-1    | temp_air		|
ctin D_pre_trans     2  from F_pb           1  i      t-1    | pb		|
ctin D_pre_trans     3  from F_qair         1  i      t-1    | qair		|--> 1) zdefconc
ctin D_pre_trans     4  from SE_rau         1  i      t      | rau		|--> 2) zqsvegrap
ctin D_pre_trans     5  from H_canop_interp 2  i  j   t-1    | qsintveg		|
ctin D_pre_trans     6  from S_qsintm       1  i  j   t-1    | qsintmax		|

ctin D_pre_trans2    1  from I_swnet        1  i      t-1    | swnet		|
ctin D_pre_trans2    2  from D_pre_trans    1  i  j   t      | zdefconc		|-- > rveget
ctin D_pre_trans2    3  from S_lai          1  i  j   t-1    | lai		|

ctin D_trans         1  from S_veget       1  i  j   t-1    | veget		|
ctin D_trans         2  from S_lai         1  i  j   t-1    | lai		|
ctin D_trans         3  from I_swnet       1  i      t-1    | swnet		|
ctin D_trans         4  from H_soil_humrel 1  i  j   t-1    | humrel		|
ctin D_trans         5  from wind          1  i      t-1    | wind		|
ctin D_trans         6  from D_aero        1  i      t      | q_cdrag		|--> vbeta3
ctin D_trans         7  from D_pre_trans2  1  i  j   t      | rveget		|
ctin D_trans         8 from D_inter        2  i  j   t      | vbeta23		|
ctin D_trans         9  from D_pre_trans   2  i  j   t      | zqsvegrap		|
ctin D_trans         10 from rveg_par      1  i             | rveg_par		|

#-------------------------------------------------------------------------------------------------------------------------------------
#ctin D_trans         1  from I_swnet        1  i      t-1    | swnet		|
#ctin D_trans         2  from F_temp_air     1  i      t-1    | temp_air	|
#ctin D_trans         3  from F_pb           1  i      t-1    | pb		|
#ctin D_trans         4  from F_qair         1  i      t-1    | qair		|
#ctin D_trans         5  from SE_rau         1  i      t      | rau		|
#ctin D_trans         6  from wind           1  i      t-1    | wind		|
#ctin D_trans         7  from D_aero         1  i      t      | q_cdrag		|--> vbeta3
#ctin D_trans         8  from H_soil_humrel  1  i  j   t-1    | humrel		|
#ctin D_trans         9  from S_veget        1  i  j   t-1    | veget		|
#ctin D_trans         10 from S_lai          1  i  j   t-1    | lai		|
#ctin D_trans         11 from H_canop_interp 2  i  j   t-1    | qsintveg	|
#ctin D_trans         12 from S_qsintm       1  i  j   t-1    | qsintmax	|
#ctin D_trans         13 from D_inter        2  i  j   t      | vbeta23		|
#ctin D_trans         14 from rveg_par       1  i             | rveg_par	|
#-------------------------------------------------------------------------------------------------------------------------------------


ctin D_comb_vbsum    1  from D_inter        1  i    j    t   | vbeta2		|
ctin D_comb_vbsum    2  from D_trans        1  i    j    t   | vbeta3		|--> 1) vbeta2sum
ctin D_comb_vbsum    3  from D_comb_vbsum   1  i    j-1  t   | acum de vbeta2	|--> 2) vbeta3sum
ctin D_comb_vbsum    4  from D_comb_vbsum   2  i    j-1  t   | acum de vbeta3	|

ctin D_comb_vb       1  from D_bare         1  i         t   | vbeta4		|
ctin D_comb_vb       2  from H_snow_melt    1  i         t   | snow		|
ctin D_comb_vb       3  from D_comb_vbsum   1  i   nvm   t   | vbeta2sum	|
ctin D_comb_vb       4  from D_comb_vbsum   2  i   nvm   t   | vbeta3sum	|--> 1) vbeta
ctin D_comb_vb       5  from D_qsatt        1  i         t   | qsatt		|--> 2) toveg
ctin D_comb_vb       6  from F_qair         1  i         t-1 | qair		|--> 3) tosnow
ctin D_comb_vb       7  from F_temp_air     1  i         t-1 | temp_air		|

ctin D_comb_soil     1  from D_comb_vb      2  i         t   | toveg		|
ctin D_comb_soil     2  from S_veget        1  i    1    t-1 | veget		|
ctin D_comb_soil     3  from D_comb_vb      1  i         t   | vbeta		|--> 1) vbeta
ctin D_comb_soil     4  from D_bare         1  i         t   | vbeta4		|--> 2) vbeta1
ctin D_comb_soil     5  from D_snow         1  i         t   | vbeta1		|--> 3) vbeta4
ctin D_comb_soil     6  from D_comb_vb      3  i         t   | tosnow		|--> 4) valpha

ctin D_comb_veg      1  from D_comb_vb      2  i         t   | toveg		|
ctin D_comb_veg      2  from D_comb_vb      3  i         t   | tosnow		|
ctin D_comb_veg      3  from S_qsintm       1  i    j   t-1  | qsintmax		|
ctin D_comb_veg      4  from S_lai          1  i    j   t-1  | lai		|--> 1) vbeta2
ctin D_comb_veg      5  from D_comb_soil    1  i         t   | vbeta		|--> 2) vbeta
ctin D_comb_veg      6  from D_qsatt        1  i         t   | qsatt		|
ctin D_comb_veg      7  from F_qair         1  i         t-1 | qair		|
ctin D_comb_veg      8  from S_veget        1  i    j    t-1 | veget		|
ctin D_comb_veg      9  from D_inter        1  i    j    t   | vbeta2		|
ctin D_comb_veg      10 from D_comb_veg     2  i    j-1  t   | vbeta(j-1)	|

ctin D_comb_verif    1  from D_qsatt        1  i         t   | qsatt		|
ctin D_comb_verif    2  from F_qair         1  i         t-1 | qair		|--> 1) vbeta3
ctin D_comb_verif    3  from D_trans        1  i     j   t   | vbeta3		|--> 2) humrel
ctin D_comb_verif    4  from H_soil_humrel  1  i     j   t-1 | humrel		|


#--------------------
#------ENERBIL ------
#--------------------

ctin E_dev_qsol      1  from E_tempsol      1  i      t-1    | temp_sol_new	|--> dev_qsol
ctin E_dev_qsol      2  from F_pb           1  i      t-1    | pb		|
		
ctin E_beg_qsol      1  from E_tempsol      1  i      t-1    | temp_sol_new	|
ctin E_beg_qsol      2  from F_pb           1  i      t-1    | pb		|--> 1) psold
ctin E_beg_qsol      3  from D_qsatt        1  i      t      | qsatt		|--> 2) qsol_sat
ctin E_beg_qsol      4  from E_dev_qsol     1  i      t      | dev_qsol		|--> 3) pdqsold

ctin E_beg_netrad    1  from emis_par       1  i             | emis		|
ctin E_beg_netrad    2  from F_lwdown       1  i      t-1    | lwdown		|--> 1)lwabs
ctin E_beg_netrad    3  from I_swnet        1  i      t-1    | swnet		|--> 2) netrad
ctin E_beg_netrad    4  from E_tempsol      1  i      t-1    | temp_sol_new	|

ctin E_surftemp1     1  from wind           1  i      t-1    | speed		|
ctin E_surftemp1     2  from D_aero         1  i      t      | q_cdrag		|--> 1) zikt
ctin E_surftemp1     3  from SE_rau         1  i      t      | rau		|

ctin E_surftemp2     1  from I_petBcoef     1  i      t-1    | petBcoef		|
ctin E_surftemp2     2  from E_beg_qsol     1  i      t      | psold		|
ctin E_surftemp2     3  from E_surftemp1    1  i      t      | zikt		|
ctin E_surftemp2     4  from I_petAcoef     1  i             | petACoef		|--> 1)sensfl_old
ctin E_surftemp2     5  from D_comb_soil    2  i      t      | vbeta1		|--> 2)larsub_old
ctin E_surftemp2     6  from E_beg_qsol     2  i      t      | qsol_sat		|--> 3)lareva_old
ctin E_surftemp2     7  from D_comb_veg     2  i nvm  t      | vbeta		|
ctin E_surftemp2     8  from D_comb_soil    4  i      t      | valpha		|
ctin E_surftemp2     9  from I_peqBcoef     1  i      t-1    | peqBcoef		|
ctin E_surftemp2     10 from I_peqAcoef     1  i             | peqACoef		|

ctin E_surftemp3     1  from emis_par       1  i             | emis		|
ctin E_surftemp3     2  from E_beg_qsol     1  i      t      | psold		|
ctin E_surftemp3     3  from E_surftemp1    1  i      t      | zikt		|--> 1) netrad_sns 
ctin E_surftemp3     4  from I_petAcoef     1  i             | petACoef		|--> 2) sensfl_sns
ctin E_surftemp3     5  from D_comb_soil    2  i      t      | vbeta1		|--> 3) larsub_sns 
ctin E_surftemp3     6  from E_beg_qsol     3  i      t      | pdqsold		|--> 4) lareva_sns
ctin E_surftemp3     7  from D_comb_soil    4  i      t      | valpha		|
ctin E_surftemp3     8  from D_comb_veg     2  i  nvm t      | vbeta		|
ctin E_surftemp3     9  from I_peqAcoef     1  i             | peqACoef		|

ctin E_surftemp4     1  from E_beg_netrad   2  i      t      | netrad		|
ctin E_surftemp4  2..5  from E_surftemp3 1..4  i      t      | netrad_sns sensfl_sns larsub_sns lareva_sns
ctin E_surftemp4  6..8  from E_surftemp2 1..3  i      t      | sensfl_old larsub_old lareva_old
ctin E_surftemp4     9  from T_coef_diff    2  i      t-1    | soilflx		|
ctin E_surftemp4     10 from T_coef_diff    1  i      t-1    | soilcap		|

ctin E_surftemp_pt   1  from E_beg_qsol     1  i      t      | psold		|--> 1)psnew			i
ctin E_surftemp_pt   2  from E_surftemp4    1  i      t      | dtheta		|--> 2)temp_sol_new 

ctin E_tempsol       1  from E_surftemp_pt 2  i       t      | temp_sol_new	|--> 1)temp_sol_new		i
ctin E_tempsol       2  from temp_sol      1  i	             | temp_sol_init	|	
		
ctin E_surftemp_qsol 1  from E_beg_qsol     2  i      t      | qsol_sat		|				
ctin E_surftemp_qsol 2  from E_beg_qsol     3  i      t      | pdqsold		|-->qsol_sat_new
ctin E_surftemp_qsol 3  from E_surftemp4    1  i      t      | dtheta		|

ctin E_surftemp_epot 1  from E_surftemp1    1  i      t      | zikt		|
ctin E_surftemp_epot 2  from E_surftemp2    1  i      t      | sensfl_old	|
ctin E_surftemp_epot 3  from E_surftemp3    2  i      t      | sensfl_sns	|--> epot_air_new
ctin E_surftemp_epot 4  from E_surftemp4    1  i      t      | dtheta		|
ctin E_surftemp_epot 5  from E_surftemp_pt  1  i      t      | psnew		|

ctin E_surftemp_qair 1  from E_surftemp1     1  i      t     | zikt		|
ctin E_surftemp_qair 2  from F_qair          1  i      t-1   | qair		|
ctin E_surftemp_qair 3  from E_surftemp2     3  i      t     | lareva_old	|
ctin E_surftemp_qair 4  from E_surftemp2     2  i      t     | larsub_old	|
ctin E_surftemp_qair 5  from E_surftemp3     4  i      t     | lareva_sns	|
ctin E_surftemp_qair 6  from E_surftemp3     3  i      t     | larsub_sns	|--> qair_new
ctin E_surftemp_qair 7  from E_surftemp4     1  i      t     | dtheta		|
ctin E_surftemp_qair 8  from D_comb_soil     2  i      t     | vbeta1		|
ctin E_surftemp_qair 9  from D_comb_veg      2  i  nvm t     | vbeta		|
ctin E_surftemp_qair 10 from D_comb_soil     4  i      t     | valpha		|
ctin E_surftemp_qair 11 from E_surftemp_qsol 1  i      t     | qsol_sat_new	|

ctin E_flux1         1  from wind            1  i      t-1   | wind		|--> qc
ctin E_flux1         2  from D_aero          1  i      t     | q_cdrag		|

ctin E_flux_tslw     1  from emis_par       1  i             | emis		|				i
ctin E_flux_tslw     2  from E_tempsol      1  i      t-1    | temp_sol_new old	|--> 1) lwup
ctin E_flux_tslw     3  from E_tempsol      1  i      t      | temp_sol_new	|--> 2) tsol_rad
ctin E_flux_tslw     4  from F_lwdown       1  i      t-1    | lwdown 		|

ctin E_flux_qsurf    1  from D_comb_soil     2  i      t     | vbeta1		|				i
ctin E_flux_qsurf    2  from E_surftemp_qsol 1  i      t     | qsol_sat_new 	|
ctin E_flux_qsurf    3  from D_comb_veg      2  i nvm  t     | vbeta		|--> qsurf
ctin E_flux_qsurf    4  from D_comb_soil     4  i      t     | valpha		|
ctin E_flux_qsurf    5  from E_surftemp_qair 1  i      t     | qair_new		|
ctin E_flux_qsurf    6  from F_qair          1  i      t     | qair		|

ctin E_flux_net      1  from F_lwdown        1  i      t-1   | lwdown		|
ctin E_flux_net      2  from I_swnet         1  i      t-1   | swnet		|--> 1) netrad
ctin E_flux_net      3  from E_flux_tslw     1  i      t     | lwup		|--> 2) lwnet

ctin E_netrad        1  from E_beg_netrad    2  i      t     | netrad		|
ctin E_netrad        2  from netrad          1  i            | netrad_init	|--> 1) netrad

ctin E_flux_vefl     1  from SE_rau          1  i      t     | rau		|				i
ctin E_flux_vefl     2  from E_flux1         1  i      t     | qc		|
ctin E_flux_vefl     3  from D_comb_soil     2  i      t     | vbeta1		|
ctin E_flux_vefl     4  from E_surftemp_qsol 1  i      t     | qsol_sat_new 	|--> 1) vevapp
ctin E_flux_vefl     5  from E_surftemp_qair 1  i      t     | qair_new		|--> 2) fluxlat
ctin E_flux_vefl     6  from D_comb_veg      2  i  nvm t     | vbeta		|
ctin E_flux_vefl     7  from D_comb_soil     4  i      t     | valpha		|

ctin E_fluxlat       1  from fluxlat         1  i            | fluxlat initial  |--> 1)fluxlat	
ctin E_fluxlat       2  from E_flux_vefl     2  i      t     | fluxlat		|

ctin E_flux_flta     1  from SE_rau          1  i      t     | rau		|				i
ctin E_flux_flta     2  from E_flux1         1  i      t     | qc		|--> 1)fluxsens
ctin E_flux_flta     3  from E_surftemp_pt   1  i      t     | psnew		|--> 2)tair 
ctin E_flux_flta     4  from E_surftemp_epot 1  i      t     | epot_air_new	|

ctin E_fluxsens      1  from fluxsens        1  i            | fluxsens initial |--> 1)fluxsens	
ctin E_fluxsens      2  from E_flux_flta     1  i      t     | fluxsens		|	

ctin E_flux_evapo    1  from SE_rau          1  i     t      | rau		|
ctin E_flux_evapo    2  from E_flux1         1  i     t      | qc		|
ctin E_flux_evapo    3  from E_surftemp_qsol 1  i     t      | qsol_sat_new 	|--> evapot
ctin E_flux_evapo    4  from E_surftemp_qair 1  i     t      | qair_new		|

ctin E_grad_qsat     1  from E_flux_flta    2  i      t      | tair		|
ctin E_grad_qsat     2  from F_pb           1  i      t-1    | pb		|-->grad_qsat

ctin E_flux_corr     1  from E_flux1         1  i      t     | qc		|
ctin E_flux_corr     2  from SE_rau          1  i      t     | rau		|
ctin E_flux_corr     3  from emis_par        1  i            | emis		|
ctin E_flux_corr     4  from E_surftemp_pt   1  i      t     | psnew		|
ctin E_flux_corr     5  from E_flux_evapo    1  i      t     | evapot		|--> evapot_corr
ctin E_flux_corr     6  from E_surftemp_epot 1  i      t     | epot_air_new	|
ctin E_flux_corr     7  from E_flux_flta     2  i      t     | tair		|
ctin E_flux_corr     8  from E_grad_qsat     1  i      t     | grad_qsat	|
ctin E_flux_corr     9  from E_flux_vefl     1  i      t     | vevapp		|

ctin E_evap_veva     1  from wind            1  i     t-1    | speed		|
ctin E_evap_veva     2  from D_comb_soil     2  i     t      | vbeta1		|
ctin E_evap_veva     3  from D_comb_soil     3  i     t      | vbeta4		|--> 1) vevapsno
ctin E_evap_veva     4  from SE_rau          1  i     t      | rau		|--> 2) vevapnu
ctin E_evap_veva     5  from D_aero          1  i     t      | q_cdrag		|--> 3) xx (transpir & vevapwet)
ctin E_evap_veva     6  from E_surftemp_qsol 1  i     t      | qsol_sat_new 	|
ctin E_evap_veva     7  from E_surftemp_qair 1  i     t      | qair_new		|

ctin E_evapnu        1  from evapnu          1  i            | evapnu init	|--> 1) vevapnu
ctin E_evapnu        2  from E_evap_veva     2  i     t      | evapnu		|

ctin E_evap_trans    1  from D_comb_veg     1  i   j  t      | vbeta2		|--> 1) vevapwet
ctin E_evap_trans    2  from D_comb_verif   1  i   j  t      | vbeta3		|--> 2) transpir
ctin E_evap_trans    3  from E_evap_veva    3  i      t      | xx		|--> 3) gpp

ctin E_transpir      1  from transpir       1  i   j         | transpir init	|--> 1) transpir
ctin E_transpir      2  from E_evap_trans   2  i   j  t      | transpir		|

ctin E_emis_planck   1  from E_tempsol      1  i       t    | temp_sol_new	|--> 1) emis_s
ctin E_emis_planck   2  from E_emis_planck  1  i   j-1 t      | emis j-1	|

ctin E_emis_rad      1  from emis_par       1  i              | emis    	|
ctin E_emis_rad      2  from E_emis_planck  1  i   12  t      | emis j-1	|--> 1) rad
ctin E_emis_rad      3  from F_lwdown       1  i       t    | lwdown	 	|

ctin E_emis_tempsol  1  from E_emis_rad     1  i       t      | rad      	|--> 1) tempsol_b









#--------------------
#------HYDROLC ------
#--------------------

ctin H_snow_i        1  from H_snow_melt    1  i      t-1   | snow1		|
ctin H_snow_i        2  from S_fracnobio    2  i            | totfrac_nobio	|--> 1)snow
ctin H_snow_i        3  from F_precip_snow  1  i      t-1   | precip_snow	|

ctin H_snow          1  from H_snow_i       1  i      t     | snow		|--> 1)snow
ctin H_snow          2  from S_fracnobio    1  i            | frac_nobio	|--> 2)subsnownobio
ctin H_snow          3  from E_evap_veva    1  i      t     | vevapsno		|--> 2)vevapnu
ctin H_snow          4  from E_evapnu       1  i      t     | vevapnu		|

ctin H_snow_melt_i   1  from H_snow_melt    1  i      t-1   | snow_nobio1	|
ctin H_snow_melt_i   2  from S_fracnobio    1  i            | frac_nobio	|
ctin H_snow_melt_i   3  from F_precip_snow  1  i      t-1   | precip_snow	|--> sn
ctin H_snow_melt_i   4  from F_precip       1  i      t-1   | precip_rain	|
ctin H_snow_melt_i   5  from H_snow         1  i      t     | sunsnownobio	|

ctin H_snow_melt     1  from E_tempsol      1  i      t | temp_sol_new	|				i
ctin H_snow_melt     2  from H_snow         1  i      t     | snow		|
ctin H_snow_melt     3  from S_fracnobio    1  i            | frac_nobio	|--> 1)snow
ctin H_snow_melt     4  from T_coef_diff    1  i      t-1   | soilcap		|--> 2)snow_nobio	|
ctin H_snow_melt     5  from H_snow_melt_i  1  i      t     | sn		|--> 3) tot_melt

ctin H_snow_age      1  from H_snow_melt    1  i      t     | snow		|				i
ctin H_snow_age      2  from H_snow_melt    2  i      t     | snow_nobio	|--> 1) snow_age
ctin H_snow_age      3  from F_precip_snow  1  i      t-1   | precip_snow	|--> 2) snow_nobio_age
ctin H_snow_age      4  from H_snow_age     1  i      t-1   | snow_age		|--> 3) snowdepth
ctin H_snow_age      5  from H_snow_age     2  i      t-1   | snow_nobio_age1	|
ctin H_snow_age      6  from E_tempsol      1  i      t     | temp_sol_new	|


ctin H_veg_var       1  from S_veget        1  i  j   t-1   | veget		|--> 1) vmr
ctin H_veg_var       2  from H_veg_upd      1  i  j   t-1   | resdist		|--> 2) qsintveg2
ctin H_veg_var       3  from H_canop_interp 2  i  j   t-1   | qsintveg		|

ctin H_veg_sum       1  from H_veg_var      1  i   j   t    | vmr		|-->vegchtot
ctin H_veg_sum       2  from H_veg_sum      1  i   j-1 t    | vegchtot		|

ctin H_veg_niv       1  from H_veg_var      1  i  j    t    | vmr		|
ctin H_veg_niv       2  from H_soil_verif   2  i  j    t-1  | gqsb		|--> 1)gdq
ctin H_veg_niv       3  from H_soil_dsp2    2  i  j    t-1  | bqsb		|--> 2)bdq
ctin H_veg_niv       4  from H_veg_var      2  i  j    t    | qsintveg2		|--> 3)qsdq
ctin H_veg_niv       5  from H_veg_sum      1  i nvm   t    | vegchtot		|

ctin H_veg_water     1  from H_veg_sum      1  i nvm   t    | vegchtot		|
ctin H_veg_water     2  from H_veg_var      1  i  j    t    | vmr		|--> gtr btr qstr vtr
ctin H_veg_water  3..5  from H_veg_niv   1..3  i  j    t    | gdq bdq qsdq	|
ctin H_veg_water  6..9  from H_veg_water 1..4  i  j-1  t    | gtr btr qstr vtr	|

ctin H_veg_water2    1  from H_veg_sum      1  i nvm   t    | vegchtot		|
ctin H_veg_water2    2  from H_veg_var      1  i  j    t    | vmr		|
ctin H_veg_water2 3..6  from H_veg_water 1..4  i nvm   t    | gtr btr qstr vtr	|
ctin H_veg_water2    7  from S_veget        1  i  j    t-1  | veget		|
ctin H_veg_water2    8  from H_veg_upd      1  i  j    t-1  | resdist		|--> 1) gqsb
ctin H_veg_water2    9  from H_soil_verif   2  i  j    t-1  | gqsb		|--> 2) bqsb
ctin H_veg_water2    10 from H_soil_dsp2    2  i  j    t-1  | bqsb		|--> 3) qsintveg
ctin H_veg_water2    11 from H_canop_interp 2  i  j    t-1  | qsintveg		|--> 4) dsg
ctin H_veg_water2    12 from H_soil_verif   6  i  j    t-1  | dss		|--> 5) dsp                        
ctin H_veg_water2    13 from init_hydrolc2  2  i nvm   t    | ruu_ch		|
ctin H_veg_water2    14 from H_veg_niv      3  i  j    t    | qsdq		|
ctin H_veg_water2    15 from H_soil_dsp2    1  i  j    t-1  | dsp		|
ctin H_veg_water2    16 from H_soil_stress  4  i  j    t-1  | dsg		|
ctin H_veg_water2    17 from dpu_cste1      1  i            | dpu_cste		|


ctin H_veg_upd       1  from S_veget        1  i  j    t-1  | veget		|--> 1)resdist			i
ctin H_veg_upd       2  from S_veget        1  i  j    t    | veget1		|
ctin H_veg_upd       3  from H_veg_upd      1  i  j    t-1  | resdist1		|

ctin H_veg_cond      1  from H_veg_water2   1  i  j    t    | gqsb		|
ctin H_veg_cond      2  from H_veg_water2   2  i  j    t    | bqsb		|
ctin H_veg_cond      3  from H_veg_water2   4  i  j    t    | dsg		|
ctin H_veg_cond      4  from H_soil_verif   6  i  j    t-1  | dss		|--> 1) gqsb
ctin H_veg_cond      5  from H_veg_water2   5  i  j    t    | dsp		|--> 2) bqsb
ctin H_veg_cond      6  from H_veg_water2   1  i  1    t    | gqsb1		|--> 3) dsg
ctin H_veg_cond      7  from H_veg_water2   2  i  1    t    | bqsb1		|--> 4) dss
ctin H_veg_cond      8  from H_veg_water2   4  i  1    t    | dsg1		|--> 5) dsp
ctin H_veg_cond      9  from H_soil_verif   6  i  1    t-1  | dss1		|
ctin H_veg_cond      10 from H_veg_water2   5  i  1    t    | dsp1		|
ctin H_veg_cond      11 from S_veget        1  i  j    t-1  | veget		|

ctin H_canop_init    1  from H_veg_water2   3  i  j    t    | qsintveg		|
ctin H_canop_init    2  from E_evap_trans   1  i  j    t    | vevapwet		|--> 1) qsintveg
ctin H_canop_init    3  from S_veget        1  i  j    t-1  | veget		|
ctin H_canop_init    4  from F_precip       1  i       t-1  | precip_rain	|

ctin H_canop_interp  1  from H_canop_init   1  i  j    t    | qsintveg		|				i
ctin H_canop_interp  2  from S_qsintm       1  i  j    t-1  | qsintmax		|--> 1) precisol
ctin H_canop_interp  3  from S_veget        1  i  j    t-1  | veget		|--> 2) qsintveg
ctin H_canop_interp  4  from F_precip       1  i       t-1  | precip_rain	|
ctin H_canop_interp  5  from qsintveg       1  i  j         | qsintveg_init	|

ctin H_soil_flx      1  from S_veget        1  i  j    t-1  | veget		|--> 1)zeflux 
ctin H_soil_flx      2  from E_transpir     1  i  j    t    | transpir		|--> 2)zepreci
ctin H_soil_flx      3  from H_canop_interp 1  i  j    t    | precisol		|

ctin H_soil_flxcorr  1  from S_veget        1  i  j    t-1  | veget		|
ctin H_soil_flxcorr  2  from S_veget        1  i  1    t-1  | veget1		|
ctin H_soil_flxcorr  3  from H_soil_flx     1  i  j    t    | zeflux		|--> 1)zeflux 
ctin H_soil_flxcorr  4  from H_snow         3  i       t    | vevapnu		|
ctin H_soil_flxcorr  5  from init_hydrolc2  1  i nvm   t     | vegtot		|

ctin H_soil_trans    1  from H_veg_cond     1  i  j    t    | gqsb		|
ctin H_soil_trans    2  from H_veg_cond     2  i  j    t    | bqsb		|
ctin H_soil_trans    3  from H_soil_flxcorr 1  i  j    t    | zeflux		|
ctin H_soil_trans    4  from S_veget        1  i  1    t-1  | veget		|
ctin H_soil_trans    5  from H_soil_flx     2  i  j    t    | zpreci		|--> 1)gqsb
ctin H_soil_trans    6  from SE_routing     4  i            | irrigation	|--> 2)bqsb	
ctin H_soil_trans    7  from SE_routing     3  i            | returnflow	|
ctin H_soil_trans    8  from H_snow_melt    3  i       t    | tot_melt		|
ctin H_soil_trans    9  from init_hydrolc2  1  i nvm   t     | vegtot		|

ctin H_soil_runoff   1  from H_soil_trans   1  i  j    t    | gqsb		|
ctin H_soil_runoff   2  from H_soil_trans   2  i  j    t    | bqsb		|
ctin H_soil_runoff   3  from H_veg_cond     3  i  j    t    | dsg		|
ctin H_soil_runoff   4  from H_veg_cond     5  i  j    t    | dsp		|--> gqsb dsg bqsb dsp dss runoff
ctin H_soil_runoff   5  from H_veg_cond     4  i  j    t    | dss		|
ctin H_soil_runoff   6  from H_soil_flxcorr 1  i  j    t    | zeflux		|
ctin H_soil_runoff   7  from init_hydrolc2  2  i nvm   t    | ruu_ch		|
ctin H_soil_runoff   8  from init_hydrolc2  3  i nvm   t    | mx_eau_var	|
ctin H_soil_runoff   9  from dpu_cste1      1  i            | dpu_cste		|

ctin H_soil_dlock    1  from H_soil_runoff   1  i  j   t     | gqsb		|
ctin H_soil_dlock    2  from H_soil_runoff   2  i  j   t     | dsg		|
ctin H_soil_dlock    3  from H_soil_runoff   3  i  j   t     | bqsb		|
ctin H_soil_dlock    4  from H_soil_runoff   4  i  j   t     | dsp		|
ctin H_soil_dlock    5  from H_soil_runoff   5  i  j   t     | dss		|---> gqsb dsg bqsb dsp dss	i
ctin H_soil_dlock    6  from init_hydrolc2   2  i nvm  t     | ruu_ch		|
ctin H_soil_dlock    7  from min_drain       1  i   	     | min_drain	|
ctin H_soil_dlock    8  from max_drain       1  i            | max_drain	|
ctin H_soil_dlock    9  from exp_drain       1  i            | exp_drain	|
ctin H_soil_dlock    10 from dpu_cste1       1  i            | dpu_cste		|

ctin H_soil_diff     1  from H_soil_dlock    1  i  j    t    | gqsb		|					i
ctin H_soil_diff     2  from H_soil_dlock    3  i  j    t    | bqsb		|
ctin H_soil_diff     3  from S_veget         1  i  j    t-1  | veget		|--> 1) mean_bqsb
ctin H_soil_diff     4  from H_soil_diff     1  i  j-1  t    | mean_bqsb1	|--> 2) mean_gqsb
ctin H_soil_diff     5  from H_soil_diff     2  i  j-1  t    | mean_gqsb1	|
ctin H_soil_diff     6  from init_hydrolc2   1  i nvm   t    | vegtot		|
ctin H_soil_diff     7  from H_veg_upd       1  i  j    t    | resdist 		|


ctin H_soil_dsp      1  from H_soil_dlock    4  i  j    t    | dsp		|
ctin H_soil_dsp      2  from H_soil_diff     1  i  nvm  t    | mean_bqsb	|--> 1)dsp
ctin H_soil_dsp      3  from S_veget         1  i  j    t-1  | veget		|--> 2)bqsb
ctin H_soil_dsp	     4  from init_hydrolc2   2  i nvm   t    | ruu_ch		|
ctin H_soil_dsp	     5  from H_soil_dlock    3  i  j    t    | bqsb		|
ctin H_soil_dsp	     6  from dpu_cste1       1  i            | dpu_cste		|

ctin H_soil_verif    1  from H_soil_runoff   6  i  j    t    | runoff2D		|
ctin H_soil_verif    2  from H_soil_dlock    1  i  j    t    | gqsb    		|
ctin H_soil_verif    3  from H_soil_dlock    2  i  j    t    | dsg   		|
ctin H_soil_verif    4  from H_soil_dsp      2  i  j    t    | bqsb      	|
ctin H_soil_verif    5  from H_soil_dsp      1  i  j    t    | dsp      	|
ctin H_soil_verif    6  from H_soil_dlock    5  i  j    t    | dss		|
ctin H_soil_verif    7  from S_veget         1  i  j    t-1  | veget		|
ctin H_soil_verif    8  from init_hydrolc2   2  i nvm   t    | ruu_ch   	|--> runoff  gqsb dsg bqsb dsp dss bad
ctin H_soil_verif    9  from init_hydrolc2   3  i nvm   t    | mx_eau_var	|
ctin H_soil_verif    10 from mx_eau_eau      1  i            | mx_eau_eau	|
ctin H_soil_verif    11 from dpu_cste1       1  i            | dpu_cste		|
ctin H_soil_verif    12 from bqsb_c	     1  1            | bqsb_c 		|
ctin H_soil_verif    13 from gqsb_c	     1  1            | gqsb_c		|
ctin H_soil_verif    14 from bqsb_init	     1  i  j         | bqsb_init	|
ctin H_soil_verif    15 from gqsb_init	     1  i  j         | gqsb_init	|



ctin H_soil_mean     1  from H_soil_verif    2  i  j    t    | gqsb		|					i
ctin H_soil_mean     2  from H_soil_verif    4  i  j    t    | bqsb		|
ctin H_soil_mean     3  from S_veget         1  i  j    t-1  | veget		|--> 1) mean_bqsb
ctin H_soil_mean     4  from H_soil_mean     1  i  j-1  t    | mean_bqsb1	|--> 2) mean_gqsb
ctin H_soil_mean     5  from H_soil_mean     2  i  j-1  t    | mean_gqsb1	|
ctin H_soil_mean     6  from init_hydrolc2   1  i nvm   t     | vegtot		|
ctin H_soil_mean     7  from H_veg_upd       1  i  j    t    | resdist 		|

ctin H_soil_dsp2     1  from H_soil_verif    5  i  j    t    | dsp		|
ctin H_soil_dsp2     2  from H_soil_mean     1  i  nvm  t    | mean_bqsb	|--> 1)dsp
ctin H_soil_dsp2     3  from S_veget         1  i  j    t-1  | veget		|--> 2)bqsb
ctin H_soil_dsp2     4  from init_hydrolc2   2  i nvm   t     | ruu_ch		|
ctin H_soil_dsp2     5  from H_soil_verif    4  i  j    t    | bqsb		|
ctin H_soil_dsp2     6  from H_soil_verif    7  i  j    t    | bad		|
ctin H_soil_dsp2     7  from dpu_cste1       1  i            | dpu_cste		|


ctin H_soil_runofft  1  from H_snow_melt     3  i       t    | tot_melt		|
ctin H_soil_runofft  2  from SE_routing      4  i            | irrigation	|
ctin H_soil_runofft  3  from H_soil_runofft  1  i  j-1  t    | run_off_tot1	|--> 1) run_off_tot
ctin H_soil_runofft  4  from H_soil_verif    1  i  j    t    | runoff2D		|--> 2) drainage
ctin H_soil_runofft  5  from S_veget         1  i  j    t-1  | veget		|
ctin H_soil_runofft  6  from init_hydrolc2   1  i nvm   t     | vegtot		|

ctin H_soil_dsg1     1  from H_veg_upd       1  i  j    t    | resdist 			|				i
ctin H_soil_dsg1     2  from H_soil_verif    3  i  j    t    | dsg			|
ctin H_soil_dsg1     3  from H_soil_dsg1     1  i  j-1  t    | = salida decalada 1 en j	|--> init mean_dsg
ctin H_soil_dsg1     4  from H_soil_dsg1     1  i  j    t-1  | = salida decalada 1 en t	|

ctin H_soil_dsg      1  from H_soil_mean     2  i  nvm  t    | mean_gqsb	|
ctin H_soil_dsg      2  from init_hydrolc2   2  i  nvm  t     | ruu_ch		|--> mean_dsg
ctin H_soil_dsg      3  from init_hydrolc2   1  i  nvm  t     | vegtot		|
ctin H_soil_dsg      4  from H_soil_dsg1     1  i  nvm  t    | init mean_dsg	|

ctin H_soil_stress   1  from H_soil_verif    2  i  j    t    | gqsb 		| 
ctin H_soil_stress   2  from H_soil_verif    3  i  j    t    | dsg		| 
ctin H_soil_stress   3  from H_soil_dsp2     2  i  j    t    | bqsb 		| 
ctin H_soil_stress   4  from H_soil_dsp2     1  i  j    t    | dsp		|
ctin H_soil_stress   5  from H_soil_verif    6  i  j    t    | dss		|
ctin H_soil_stress   6  from init_hydrolc2   2  i  nvm  t    | ruu_ch		|
ctin H_soil_stress   7  from humr_c_1	     1  i            | humr_c_1	        |     
ctin H_soil_stress   8  from humr_c_2	     1  i            | humr_c_2   	|     
ctin H_soil_stress   9  from humr_c_3	     1  i            | humr_c_3	        |     
ctin H_soil_stress   10 from humr_c_4	     1  i            | humr_c_4	        |--> 1) humrel      
ctin H_soil_stress   11 from humr_c_5	     1  i            | humr_c_5 	|--> 2) vegstress  
ctin H_soil_stress   12 from humr_c_6	     1  i            | humr_c_6	        |--> 3) a_subgrd   
ctin H_soil_stress   13 from humr_c_7	     1  i            | humr_c_7	        |--> 4) dsg     
ctin H_soil_stress   14 from humr_c_8	     1  i            | humr_c_8 	|     
ctin H_soil_stress   15 from humr_c_9	     1  i            | humr_c_9 	|     
ctin H_soil_stress   16 from humr_c_10	     1  i            | humr_c_10	|     
ctin H_soil_stress   17 from humr_c_11	     1  i            | humr_c_11	|     
ctin H_soil_stress   18 from humr_c_12	     1  i            | humr_c_12	|   
ctin H_soil_stress   19 from humr_c_13	     1  i            | humr_c_13	|       
ctin H_soil_stress   20 from dpu_cste1       1  i            | dpu_cste		|

ctin H_soil_humrel1  1  from H_soil_stress   1  i  1    t    | humrel1		| --> humrel
ctin H_soil_humrel1  2  from h_c_1	     1  i            | humrel_c_1       |   

ctin H_soil_humrel2  1  from H_soil_stress   1  i  2    t    | humrel2		| --> humrel
ctin H_soil_humrel2  2  from h_c_2	     1  i            | humrel_c_2       | 

ctin H_soil_humrel3  1  from H_soil_stress   1  i  3    t    | humrel3		| --> humrel
ctin H_soil_humrel3  2  from h_c_3	     1  i            | humrel_c_3       | 

ctin H_soil_humrel4  1  from H_soil_stress   1  i  4    t    | humrel4		| --> humrel
ctin H_soil_humrel4  2  from h_c_4	     1  i            | humrel_c_4       | 

ctin H_soil_humrel5  1  from H_soil_stress   1  i  5    t    | humrel5		| --> humrel
ctin H_soil_humrel5  2  from h_c_5	     1  i            | humrel_c_5       | 

ctin H_soil_humrel6  1  from H_soil_stress   1  i  6    t    | humrel6		| --> humrel
ctin H_soil_humrel6  2  from h_c_6	     1  i            | humrel_c_6       | 

ctin H_soil_humrel7  1  from H_soil_stress   1  i  7    t    | humrel1		| --> humrel
ctin H_soil_humrel7  2  from h_c_7	     1  i            | humrel_c_7       | 

ctin H_soil_humrel8  1  from H_soil_stress   1  i  8    t    | humrel8		| --> humrel
ctin H_soil_humrel8  2  from h_c_8	     1  i            | humrel_c_8       | 

ctin H_soil_humrel9  1  from H_soil_stress   1  i  9    t    | humrel9		| --> humrel
ctin H_soil_humrel9  2  from h_c_9	     1  i            | humrel_c_9       | 

ctin H_soil_humrel10 1  from H_soil_stress   1  i  10   t    | humrel10		| --> humrel
ctin H_soil_humrel10 2  from h_c_10	     1  i            | humrel_c_10      | 

ctin H_soil_humrel11 1  from H_soil_stress   1  i  11   t    | humrel11		| --> humrel
ctin H_soil_humrel11 2  from h_c_11	     1  i            | humrel_c_11      | 

ctin H_soil_humrel12 1  from H_soil_stress   1  i  12   t    | humrel12		| --> humrel
ctin H_soil_humrel12 2  from h_c_12	     1  i            | humrel_c_12      | 

ctin H_soil_humrel13 1  from H_soil_stress   1  i  13   t    | humrel13		| --> humrel
ctin H_soil_humrel13 2  from h_c_13	     1  i            | humrel_c_13      | 

ctin H_soil_humrel   1  from H_soil_humrel1  1  i       t    | h_soil_humrel1   |  
ctin H_soil_humrel   2  from H_soil_humrel2  1  i       t    | h_soil_humrel2   |  
ctin H_soil_humrel   3  from H_soil_humrel3  1  i       t    | h_soil_humrel3   |  
ctin H_soil_humrel   4  from H_soil_humrel4  1  i       t    | h_soil_humrel4   |  
ctin H_soil_humrel   5  from H_soil_humrel5  1  i       t    | h_soil_humrel5   |  
ctin H_soil_humrel   6  from H_soil_humrel6  1  i       t    | h_soil_humrel6   |  
ctin H_soil_humrel   7  from H_soil_humrel7  1  i       t    | h_soil_humrel7   |  --> humrel
ctin H_soil_humrel   8  from H_soil_humrel8  1  i       t    | h_soil_humrel8   |  
ctin H_soil_humrel   9  from H_soil_humrel9  1  i       t    | h_soil_humrel9   |  
ctin H_soil_humrel   10 from H_soil_humrel10 1  i       t    | h_soil_humrel10  |  
ctin H_soil_humrel   11 from H_soil_humrel11 1  i       t    | h_soil_humrel11  |  
ctin H_soil_humrel   12 from H_soil_humrel12 1  i       t    | h_soil_humrel12  |  
ctin H_soil_humrel   13 from H_soil_humrel13 1  i       t    | h_soil_humrel13  |    
	
ctin H_soil_shum     1  from H_soil_dsg      1  i       t    | mean_dsg		|					i
ctin H_soil_shum     2  from H_soil_mean     1  i nvm   t    | mean_bqsb	|
ctin H_soil_shum     3  from H_soil_mean     2  i nvm   t    | mean_gqsb	|--> shumdiag
ctin H_soil_shum     4  from init_hydrolc2   3  i nvm   t    | mx_eau_var	|
ctin H_soil_shum     5  from int_slowproc    1  i j     t     | diaglev   	|
ctin H_soil_shum     6  from int_slowproc    1  i j-1   t     | diaglev1		|

		
ctin H_soil_hdry     1  from H_soil_stress   3  i  1    t    | a_subgrd		|					i
ctin H_soil_hdry     2  from H_soil_verif    6  i  1    t    | dss		|--> 1) hdry
ctin H_soil_hdry     3  from H_soil_dsp2     1  i  1    t    | dsp		|

ctin H_soil_dryfrac  1  from H_soil_verif    6  i  1    t    | dss		|--> 1) drysoil_frac			i
		
ctin H_soil_rsol     1  from H_soil_hdry     1  i       t    | hdry		|--> 1)rsol					i
ctin H_soil_rsol     2  from S_veget         1  i  1    t    | veget		|--> 2) litterhumdiag
ctin H_soil_rsol     3  from H_soil_dsg      1  i       t    | mean_dsg		|
ctin H_soil_rsol     4  from rsol_cste       1  i            | rsol_cste	|
ctin H_soil_rsol     5  from dpu_cste1       1  i            | dpu_cste		|

ctin H_watercontent     1  from H_soil_mean     1  i  nvm  t    | mean_bqsb	|
ctin H_watercontent     2  from H_soil_mean     2  i  nvm  t    | mean_gqsb	|
ctin H_watercontent     3  from hummin          1  1            | a		| --> 1) Nhumsim 
ctin H_watercontent     4  from hummax          1  1            | b		|  
ctin H_watercontent     5  from eaumin          1  1            | c		|  
ctin H_watercontent     6  from eaumax          1  1            | d		|  





#--------------------
#------THERMOSOIL----
#--------------------

ctin T_profile       1  from T_coef_soil2  2    i   j-1  t-1 | dgrnd		|					i
ctin T_profile       2  from T_coef_soil2  1    i   j-1  t-1 | cgrnd		|
ctin T_profile       3  from E_tempsol     1    i        t   | temp_sol_new	|
ctin T_profile       4  from T_profile     1    i   j-1  t   | ptn1		|--> 1)ptn
ctin T_profile       5  from T_coef_soil2  2    i   1    t-1 | dgrnd1		|
ctin T_profile       6  from T_coef_soil2  1    i   1    t-1 | cgrnd1		|
ctin T_profile       7  from ptn           1    i   j        | ptn init		|
ctin T_profile       8  from int_thermosoil 4   1	 t-1 | lambda

ctin T_energy        1  from E_tempsol     1   i        t    | temp_sol_new	|					i
ctin T_energy        2  from T_coef_soil   3   i    1  t-1   | pcapa_en		|--> 1) surfheat_incr
ctin T_energy        3  from T_coef_diff   1   i       t-1   | soilcap		|--> 2) coldcont_incr
ctin T_energy        4  from T_energy      3   i       t-1   | temp_sol_beg1	|--> 3) temp_sol_beg

ctin T_coef_soil     1  from T_hum_wet    1    i   j    t    | wetdiag		|			
ctin T_coef_soil     2  from H_snow_melt  1    i        t    | snow		|
ctin T_coef_soil     3  from so_capa_dry  1    i             | so_capa_dry	|		 			i
ctin T_coef_soil     4  from so_capa_wet  1    i             | so_capa_wet	|
ctin T_coef_soil     5  from int_dz2	  1    i   j 	t     | dz1		|--> 1) zdz1 2) zdz2
ctin T_coef_soil     6  from int_dz2	  2    i   j	t     | dz2		|--> 3) pcapa_en
ctin T_coef_soil     7  from int_dz2	  3    i   j	t     | zz_coef		|
ctin T_coef_soil     8  from int_dz2	  3    i   j-1	t     | zz_coef1		|
ctin T_coef_soil     9  from so_capa	  1    i   	     | so capa		|
ctin T_coef_soil     10 from so_cond	  1    i   	     | so_cond		|


ctin T_coef_soil2    1  from T_coef_soil  1    i   j+1  t    | zdz1		|					i
ctin T_coef_soil2    2  from T_coef_soil  2    i   j+1  t    | zdz2		|
ctin T_coef_soil2    3  from T_coef_soil  1    i   j    t    | zdz11		|--> 1) cgrnd
ctin T_coef_soil2    4  from T_profile    1    i   j+1  t    | ptn		|--> 2) dgrnd
ctin T_coef_soil2    5  from T_coef_soil2 1    i   j+1  t    | cgrnd1		|
ctin T_coef_soil2    6  from T_coef_soil2 2    i   j+1  t    | dgrnd1		|

ctin T_coef_diff     1  from T_coef_soil   1   i   1    t    | zdz1		|					i
ctin T_coef_diff     2  from T_coef_soil   2   i   1    t    | zdz2		|
ctin T_coef_diff     3  from T_coef_soil2  1   i   1    t    | cgrnd		|--> 1) soilcap
ctin T_coef_diff     4  from T_coef_soil2  2   i   1    t    | dgrnd		|--> 2) soilflx
ctin T_coef_diff     5  from T_profile     1   i   1    t    | ptn		|
ctin T_coef_diff     6  from E_tempsol 1   i        t    | temp_sol_new	|
ctin T_coef_diff     7  from int_thermosoil 4 1	    t-1	     | lambda		|

#ctin T_hum_wet   1  		from h_init         1   1		   | h_init   |
#ctin T_hum_wet   2..nbdl1        from H_soil_shum    1   i       1..nbdl  t | shumdiag |-->wetdiag
#ctin T_hum_wet   nbdl11..nbdl3   from int_humlev     1   1..nbdl j    t	   | int_w    |

ctin T_hum_wet    1..nbdl        from H_soil_shum   1   i       1..nbdl  t | shumdiag |-->wetdiag
ctin T_hum_wet   nbdl1..nbdl2   from int_humlev     1   1..nbdl j    t	   | int_w    |


ctin T_diag_stemp 1..ngrnd  	from T_profile     1   i  1..ngrnd t | ptn		|-->stempdiag
ctin T_diag_stemp 8..14  	from int_diaglev   1   1..ngrnd j   t| 			|



#ORDER ---------------------------------------------------------------ORDER

#--------------------
#---MODULES-ESPACE---
#--------------------


#---------------------------------
#---------SECHIBA------------
#----------------------------------
order modinspace SkjindexSEi
	order YA1  SE_rau            forder
forder


#modulos noward
order modinspace SkjindexSEn
#	order YA1  SE_evaplim        forder
	order YA1  SE_routing        forder
forder


#---------------------------------
#---------INTERSURF------------
#----------------------------------
order modinspace SkjindexIi
#	order YA1  I_swnet           forder
	order YA1  I_epot_air        forder
	order YA1  I_petBcoef        forder
	order YA1  I_peqBcoef        forder
	#order YA1  I_comb            forder
forder

#modulos noward
order modinspace SkjindexIn
#	order YA1  I_zlev            forder
#	order YA1  I_contfrac        forder
	order YA1  I_petAcoef        forder
	order YA1  I_peqAcoef        forder
#	order YA1  I_ccanopy         forder
forder


#---------------------------------
#---------VARIAS------------------
#----------------------------------


order modinspace SParam 
	order YA1  rsol_cste  so_capa_wet  so_capa_dry  min_drain   max_drain  exp_drain  so_capa  so_cond  dpu_cste1  mx_eau_eau rveg_par  z0_par    z0_over   albedovis albedonir emis_par forder
	order YA1  humr_c_1   humr_c_2     humr_c_3     humr_c_4    humr_c_5   humr_c_6   humr_c_7 humr_c_8 humr_c_9   humr_c_10  humr_c_11 humr_c_12 humr_c_13 forder
forder



order modinspace Skjindexn
	order YA1  int_lskin         forder
forder

order modinspace SkjindexW
	order YA1  wind             forder
forder



order modinspace Sngrnd_init
	order YA1 int_thermosoil forder
forder


order modinspace Snbdl_init 
	order YA1 YA2  int_slowproc    forder
forder

order modinspace Snbdl_hum
	order YA2 YA1 int_diaghum    forder
forder




order modinspace Sngrnd_22
	order YA1 YA2 int_dz2     forder
forder

order modinspace Sn2_init
#	order YA1 YA2  int_humlev     forder
	order YA2 YA1  int_diaglev    forder
forder

order modinspace Sn1_init
	order YA2 YA1  int_humlev    forder
forder




#---------------------------------
#---------SLOWPROC------------
#----------------------------------
#modulos noward

#order modinspace SkjindexSn
#	order YA1      S_deadleaf    forder 
#forder

order modinspace SkjindexSn
	order YA1   S_fracnobio    forder 
forder

order modinspace SnvmSn
	order YA1 YA2  S_height      forder 
	order YA1 YA2  S_vegetmax    forder
#	order YA1 YA2  init_hydrolc2 forder
forder

order modinspace SnvmSi
	order YA1 YA2  S_lai         forder
	order YA1 YB2  S_veget       forder 	
	order YA1 YA2  S_qsintm      forder
#	order YA1 YA2  init_hydrolc  forder 	
forder

order modinspace Snvm_init
	order YA1 YA2  init_hydrolc2 forder
forder



#--------------------------------
#---------CONDVEG-------------
#---------------------------------

order modinspace SnvmCi
	order YA1 YA2  C_z0_rug      forder
forder

order modinspace SnvmC2
	order YA1 YA2  C_albedo      forder
forder






#--------------------------------
#---------DIFFUCO-------------
#---------------------------------

order modinspace SnvmDpre
	order YA1  D_pre_aero     forder
forder


order modinspace SkjindexDi
	order YA1  I_swnet  forder
	order YA1  D_aero1  forder
	order YA1  D_aero   forder
#	order YA1  D_raerod forder
	order YA1  D_qsatt  forder
	order YA1  D_snow   forder
	order YA1  D_bare   forder
forder

order modinspace SnvmDi
	order YA1 YA2  D_inter	    forder 
	order YA1 YA2  D_pre_trans  forder 
	order YA1 YA2  D_pre_trans2 forder 
	order YA1 YA2  D_trans	    forder 
	order YA1 YA2  D_comb_vbsum forder 

forder

order modinspace SkjindexD1i
	order YA1  D_comb_vb   forder
	order YA1  D_comb_soil forder
forder

order modinspace SnvmD1i
	order YA1 YA2  D_comb_veg    forder
	order YA1 YA2  D_comb_verif  forder
forder


#---------------------------------
#---------ENERBIL-------------
#---------------------------------
order modinspace SkjindexEi
	order YA1  E_dev_qsol      forder
	order YA1  E_beg_qsol      forder
	order YA1  E_beg_netrad    forder
	order YA1  E_surftemp1     forder
	order YA1  E_surftemp2     forder
	order YA1  E_surftemp3     forder
	order YA1  E_surftemp4     forder
	order YA1  E_surftemp_pt   forder
	order YA1  E_tempsol 	   forder
	order YA1  E_surftemp_qsol forder
	order YA1  E_surftemp_epot forder
	order YA1  E_surftemp_qair forder
	order YA1  E_flux1         forder
	order YA1  E_flux_tslw     forder
	order YA1  E_flux_qsurf    forder
	order YA1  E_flux_net      forder
	order YA1  E_netrad        forder
	order YA1  E_flux_vefl     forder
	order YA1  E_fluxlat       forder
	order YA1  E_flux_flta     forder
	order YA1  E_fluxsens      forder
	order YA1  E_flux_evapo    forder
	order YA1  E_grad_qsat     forder
	order YA1  E_flux_corr     forder
	order YA1  E_evap_veva     forder
	order YA1  E_evapnu        forder
forder

order modinspace SnvmEi
	order YA1 YA2  E_evap_trans	 forder 
	order YA1 YA2  E_transpir	 forder 	
forder



order modinspace SdouzeEi
	order YA1 YA2  E_emis_planck	 forder 
forder


order modinspace SkjindexE2i
	order YA1  E_emis_rad      forder
	order YA1  E_emis_tempsol      forder
forder



#---------------------------------
#---------HYDROLC-------------
#---------------------------------

order modinspace SkjindexHis
	order YA1  H_snow_i              forder
	order YA1  H_snow                forder
	order YA1  H_snow_melt_i         forder
	order YA1  H_snow_melt           forder
	order YA1  H_snow_age            forder
forder

order modinspace SnvmHi
	order YA1 YA2  H_veg_var       forder 
	order YA1 YA2  H_veg_sum       forder 
	order YA1 YA2  H_veg_niv       forder 
	order YA1 YA2  H_veg_water     forder 
	order YA1 YA2  H_veg_water2    forder 
	order YA1 YA2  H_veg_upd       forder 
	order YA1 YA2  H_veg_cond      forder 
	order YA1 YA2  H_canop_init    forder 
	order YA1 YA2  H_canop_interp  forder 
	order YA1 YA2  H_soil_flx      forder 
	order YA1 YA2  H_soil_flxcorr  forder 
	order YA1 YA2  H_soil_trans    forder 
	order YA1 YA2  H_soil_runoff   forder 
	order YA1 YA2  H_soil_dlock    forder 
	order YA1 YA2  H_soil_diff     forder 
#----------------------------------------------------------------------
	#order YA1 YA2  H_soil_cond     forder 
#----------------------------------------------------------------------
	order YA1 YA2  H_soil_dsp      forder 
	order YA1 YA2  H_soil_verif    forder 
	order YA1 YA2  H_soil_mean     forder 
	order YA1 YA2  H_soil_dsp2     forder 
	order YA1 YA2  H_soil_runofft  forder 
	order YA1  YA2 H_soil_dsg1     forder
forder

order modinspace SkjindexHi
	order YA1  H_soil_dsg           forder
forder

order modinspace SnvmH1i
	order YA1 YA2  H_soil_stress    forder
forder

order modinspace SkjindexH2i
	order YA1  H_soil_humrel1 H_soil_humrel2 H_soil_humrel3 H_soil_humrel4
		   H_soil_humrel5 H_soil_humrel6 H_soil_humrel7 H_soil_humrel8
		   H_soil_humrel9 H_soil_humrel10 H_soil_humrel11 H_soil_humrel12 
		   H_soil_humrel13 
      forder
forder

order modinspace SnvmH2i
	order YA1 YA2  H_soil_humrel    forder
forder

#order modinspace SnvmH1i
#	order YA1 YA2  H_soil_stress    forder 
#	order YA1 YA2  H_soil_humrel    forder
#forder

order modinspace SnbdlHi
	order YA1 YA2  H_soil_shum      forder 
forder

order modinspace SkjindexH1i
	order YA1  H_soil_hdry         forder
	order YA1  H_soil_dryfrac      forder
	order YA1  H_soil_rsol         forder
	order YA1  H_watercontent        forder
forder

#order modinspace SnvmH1n
#	order YA1  YA2 H_soil_dsg1       forder
#forder


#---------------------------------
#---------THERMO-------------
#---------------------------------

order modinspace SngrndTi
	order YA1 YA2 T_hum_wet      forder
	order YA1 YA2 T_profile      forder
forder

order modinspace SnbdlTi
	order YA1 YA2 T_diag_stemp   forder
forder

order modinspace SkjindexTi
	order YA1  T_energy          forder
forder

order modinspace SngrndTTT
	order YA1 YA2 T_coef_soil    forder
forder

order modinspace SngrndT2i
	#order YA1 YA2 T_coef_soil    forder
	order YA1 YB2 T_coef_soil2   forder
forder

order modinspace SkjindexT1i
	order YA1  T_coef_diff       forder
forder



#---------------------------------
#---------CONDVEG------------
#----------------------------------

order modinspace Skjindex2Cn
	order YA1 YA2  C_albcalc3        forder
forder


#order modinspace SkjindexCi
#	order YA1  C_zol_init    forder 
#	order YA1  C_zol_calc    forder
#forder


 
#---------------------------------
#---------SECHIBA AGAIN-----------
#---------------------------------

#order modinspace SkjindexSEi1                   
#	order YA1  SE_tempsol        forder         | se ejecuta del ultimo
#forder




#--------------------
#-ESPACE-TRAJECTOIRE-
#--------------------

order spaceintraj Tmodul
#--> forçage
#	SkjindexFn
#--> varios Intersuft
	Skjindexn   
	Sngrnd_init  
	Snbdl_init   
	Snbdl_hum   
	Sngrnd_22   
	Sn2_init    
	Sn1_init    
	SkjindexW
	
#--> intersurf
	SkjindexIi
#--> sechiba
	SkjindexSEi
#--> slowproc
	SnvmSi
	SnvmCi
	SnvmC2
	Snvm_init	| init_hydrolc2
#--> diffuco
	SnvmDpre
	SkjindexDi
	SnvmDi
	SkjindexD1i
	SnvmD1i
#--> enerbil
	SkjindexEi
	SnvmEi
	SdouzeEi
	SkjindexE2i 

#--> hydro
	SkjindexHis
	SnvmHi
#	SnvmH1n
	SkjindexHi
	SnvmH1i		| soil_tress
	SkjindexH2i	| humrel separadas
	SnvmH2i		| humrel juntas
	SnbdlHi
	SkjindexH1i
#--> thermo
	SngrndTi 
	SnbdlTi  
	SkjindexTi  
	SngrndTTT 
	SngrndT2i    
	SkjindexT1i
#--> Condveg
#	SkjindexSEi1   | de sechiba
forder


#modulos noward
order spaceintraj Tcontr	 
	SParam  
	#Skjindexn
	#Sngrnd_init
	#Sngrnd_22
#--> Condveg
	Skjindex2Cn
#	SkjindexCi
#--> intersurf
	SkjindexIn
#--> sechiba
	SkjindexSEn
#--> slowproc
#	SnnobioSn
	SnvmSn
#	SkjindexSn
#-->enerbil
#	SkjindexE1i
#-->hydro
#	SnvmH1n
#--> thermo
#	Scomb1Ti 
#--> varios
	#Snbdl_init 
	#Snbdl_hum 
	#Sn1_init
	#Sn2_init	
forder










	
#USER FUNCTIONS	---------------------------------INSERT_FCT
insert_fct      xdisplay
insert_fct arg  xinit_forcing
insert_fct arg  xinit_forcing_SMOSREX
insert_fct arg  xforcing_lai
insert_fct arg  xsaison
insert_fct arg  xparam_init
insert_fct arg  xparam_humrel
insert_fct arg  xsensib
insert_fct arg  xPFT
insert_fct arg  xbypass_jour
insert_fct arg  xbypass_lai
insert_fct arg  xgrad_fichier
insert_fct arg  xobs_pdt
insert_fct arg xset_control 
#USER FUNCTIONS	---------------------------------INSERT_FCT



