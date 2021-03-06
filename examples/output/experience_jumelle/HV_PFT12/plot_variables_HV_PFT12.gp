




set multiplot layout 3, 1 title "Fluxes variation prior and after assimilation. FLUXNET Harvard Forest, 07-03 to 14-03-1996, Agricultural C3 (PFT 12)" font ",16"
set tmargin 2
set key font ",10"



#
set title "Land Surface Temperature" font ",14" offset 0,-1
set xlabel "Step time (30 min)" offset 0,0.5
set ylabel "LST (K)" offset 2,0
plot '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_obs_HV.dat' using 1:3 with points   ti "Observations",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_init_HV.dat' using 1:3 with lines   ti "First Guest",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_tempsol_fin_HV.dat' using 1:3 with lines  ti "After assimilation"
unset ylabel
unset xlabel
unset title

#
set title "Sensible Heat Flux" font ",14" offset 0,-1
set xlabel 'Step time (30 min)' offset 0,0.5
set ylabel 'H (W/m^2)' offset 2,0
plot '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_obs_HV.dat' using 1:3 with points  ti "Observations",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_init_HV.dat' using 1:3 with lines  ti "First Guest",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxsens_fin_HV.dat' using 1:3 with lines ti "After assimilation"
unset ylabel
unset xlabel
unset title
#
set title "Latent Heat Flux" font ",14" offset 0,-1
set xlabel 'Step time (30 min)' offset 0,0.5
set ylabel 'LE (W/m^2)' offset 2,0
plot '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_obs_HV.dat' using 1:3 with points  ti "Observations",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_init_HV.dat' using 1:3 with lines  ti "First Guest",\
     '< tail -336 ./examples/output/experience_jumelle/HV_PFT1/fluxes/E_fluxlat_fin_HV.dat' using 1:3 with lines ti "After assimilation"

#
unset ylabel
unset xlabel
unset title
unset multiplot





pause -1 "Hit return to finish"
