#==========================================================
#        Instruction file assim_HV_PFT1.i
# 
# This script run a twin experiment, introducing a random error 
# in the control parameters. Type of PFT : bare soil (PFT 1)
#====================================#====================== 



###########################################
# 0) MANDATORY FUNCTIONS
###########################################


################################ PDF Inizialization #############################
xPFT 1 0 0 0 0 0 0 0 0 0 0 0 0 

######## Select the day of the beginning of the experiment (in step time) ######
xbypass_jour 3216 

############################### Forcing file ###################################
xinit_forcing ./examples/forcing/ 1996US-Ha1.nc 

######### Initial values of the model parameters  (don't modify) ################
#---------- rsol   socapa_wet   socapa_dry  min_drain  max_drain  exp_drain   so_capa   so_cond  dpu_cste   mx_eau_eau  rveg   z0_p  z0_over_height  albvis   albnir  emissitive     
xparam_init 33000   3.03e+06     1.80e+6      0.001       0.2        1.5         1          1        2          150       1      1       0.046          1        1	  0.96      

############# Initials humcste values, one for each PFT (don't modify)###########
#------------ h1  h2  h3  h4  h5  h6  h7  h8  h9  h10  h11  h12  h13
xparam_humrel 5  0.8  0.8 1  0.8  0.8  1  1  0.8   4    4    4   4  





###############################################################################
########################## 1) MODEL INITIALIZATION   ##########################
###############################################################################

##################   Variables initial Conditions   #############################
setstate temp_sol	273
setstate netrad 	0
setstate fluxsens 	0
setstate fluxlat 	0
setstate evapnu 	0
setstate transpir 	0
setstate ptn 		273
setstate qsintveg 	0 


#########################  Hydrological variables  ###############################
setstate h_c_1	1
setstate h_c_2	0	
setstate h_c_3 	0
setstate h_c_4 	0
setstate h_c_5 	0
setstate h_c_6 	0
setstate h_c_7 	0
setstate h_c_8 	0
setstate h_c_9 	0
setstate h_c_10 0
setstate h_c_11 0 
setstate h_c_12 0
setstate h_c_13 0

setstate bqsb_init 300
setstate gqsb_init 0.001


################# Initialization of controllable parameters #####################
setstate rsol_c		1
setstate min_drain_c	1
setstate rveg_c		1
setstate z0_c		1
setstate emis_c		1

setstate so_capa_c	1
setstate so_cond_c	1
setstate mx_eau_c 	1
setstate dpu_c		1
setstate albvis_c 	1
setstate hum_c_12	1

setstate hum_c_1	1
setstate hum_c_2	1
setstate hum_c_3	1
setstate hum_c_4	1
setstate hum_c_5	1
setstate hum_c_6	1
setstate hum_c_7	1
setstate hum_c_8	1
setstate hum_c_9	1
setstate hum_c_10	1
setstate hum_c_11	1
setstate hum_c_13	1

setstate bqsb_c		1
setstate gqsb_c		1

setstate so_dry_c	1
setstate so_wet_c	1
setstate mx_drain_c	1
setstate e_drain_c	1
setstate z0_over_c	1
setstate albnir_c	1

setstate eaumax		208.46     
setstate eaumin		175.6886  

setstate hummax		0.24431       
setstate hummin		0.14463

    
############################### Controled parameters #########################
xset_control z0_c	  	
xset_control so_capa_c  	 
xset_control so_cond_c  	  
xset_control emis_c      	
xset_control albvis_c


###############################################################################
################################# 2) FORWARD   ################################
###############################################################################
set_modeltime 0
FORWARD
set_modeltime 0

############ Saving variables and parameters after execution of the direct model ##########
savestate E_tempsol	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_obs_HV.dat
savestate E_tempsol	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_obs2_HV.dat
savestate E_fluxsens	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_obs_HV.dat
savestate E_fluxlat	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_obs_HV.dat
savestate H_soil_humrel1	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_humrel_obs_HV.dat
savestate T_coef_diff	2 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/T_soilflx_obs_HV.dat
savestate E_netrad	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_netrad_obs_HV.dat
savestate H_soil_mean	1 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_bqsb_obs_HV.dat
savestate H_soil_mean	2 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_gqsb_obs_HV.dat

savestate z0_c 1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/z0_opt_HV.dat
savestate so_cond_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_cond_opt_HV.dat
savestate so_capa_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_capa_opt_HV.dat
savestate emis_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/emis_opt_HV.dat
savestate albvis_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/albvis_opt_HV.dat

############### Load Observations for the twin experiments #################
LOADOBS E_tempsol 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_obs_HV.dat D




#-----------------------------------------------------------------------------------------------------------------------


###############################################################################
############## 3) MINIMIZATION M1QN3  (without constrains) ####################
###############################################################################

############## Adding random noise to control parameters ##################

setstate z0_c	  	0.7182 
setstate so_capa_c  	0.5501 
setstate so_cond_c  	0.5102 
setstate emis_c     	0.8531 
setstate albvis_c	0.6175 


################### Before Optimization (first guess) #####################

set_modeltime 0
FORWARD
set_modeltime 0


################################# Saving First Guess #######################

savestate z0_c 		1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/z0_init_HV.dat
savestate so_cond_c 	1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_cond_init_HV.dat
savestate so_capa_c 	1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_capa_init_HV.dat
savestate emis_c 	1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/emis_init_HV.dat
savestate albvis_c 	1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/albvis_init_HV.dat

savestate rsol_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/rsol_c_init_HV.dat
savestate so_capa_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/so_capa_init_HV.dat
savestate so_cond_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/so_cond_init_HV.dat	
savestate mx_eau_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/mx_eau_init_HV.dat	
savestate dpu_c		1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/dpu_init_HV.dat	
savestate hum_c_12	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/hum_init_HV.dat	

savestate E_tempsol	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_init_HV.dat
savestate E_fluxsens	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_init_HV.dat
savestate E_fluxlat	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_init_HV.dat
savestate H_soil_humrel1	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_humrel_init_HV.dat
savestate T_coef_diff	2 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/T_soilflx_init_HV.dat
savestate E_netrad	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_netrad_init_HV.dat
savestate H_soil_mean	1 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_bqsb_init_HV.dat
savestate H_soil_mean	2 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_gqsb_init_HV.dat



##########################    Minimization    ##############################
print_cost ON
setm_impres 	0
setm_io 	6
setm_mode 	0
set_nbiter    100
setm_nsim     200
setm_dxmin    2.0e-30
setm_epsg     2.0e-20
setm_ddf1     1.0
#set_qs_parts 4
runm


###################### After optimization ######################
set_modeltime 0
FORWARD
set_modeltime 0


###############################################################################
######################### 4) Savings parameters ###############################
###############################################################################

################################# Saving Parameters after assimilation #######################
savestate z0_c 1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/z0_fin_HV.dat
savestate so_cond_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_cond_fin_HV.dat
savestate so_capa_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/so_capa_fin_HV.dat
savestate emis_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/emis_fin_HV.dat
savestate albvis_c 1 i 0 A 1 ./examples/output/experience_jumelle/HV_PFT1/param/albvis_fin_HV.dat

savestate rsol_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/rsol_c_fin_HV.dat
savestate so_capa_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/so_capa_fin_HV.dat
savestate so_cond_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/so_cond_fin_HV.dat	
savestate mx_eau_c 	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/mx_eau_fin_HV.dat	
savestate dpu_c		1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/dpu_fin_HV.dat	
savestate hum_c_12	1 i 0 A 1  ./examples/output/experience_jumelle/HV_PFT1/param/hum_fin_HV.dat	


################################# Saving Model state after assimilation #######################
savestate E_tempsol	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_fin_HV.dat
savestate E_fluxsens	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_fin_HV.dat
savestate E_fluxlat	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_fin_HV.dat
savestate H_soil_humrel1	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_humrel_fin_HV.dat
savestate T_coef_diff	2 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/T_soilflx_fin_HV.dat
savestate E_netrad	1 i  0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_netrad_fin_HV.dat
savestate H_soil_mean	1 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_bqsb_fin_HV.dat
savestate H_soil_mean	2 ij 0 A  3  ./examples/output/experience_jumelle/HV_PFT1/fluxes/H_gqsb_fin_HV.dat

################################# Plotting all outputs #######################

####some model variables
!gnuplot ./examples/output/experience_jumelle/HV_PFT1/plot_variables_HV_PFT1.gp

####control paremeter variation
!gnuplot ./examples/output/experience_jumelle/HV_PFT1/plot_parameters_HV_PFT1.gp






