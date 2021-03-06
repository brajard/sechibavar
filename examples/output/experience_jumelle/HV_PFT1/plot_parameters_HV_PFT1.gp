


set key font ",12"
set multiplot layout 2,3 title "Parameters prior and after assimilation. FLUXNET Harvard Forest, 07-03 to 14-03-1996, Bare Soil (PFT 1)" font ",16"

set style fill solid border -1
set xrange [-0.25:0.5] # coord axe x
set yrange [0:2] # coord axe y
set format x ""
set title "Roughness height (Zo)" font ",14" offset 0,-1
set ylabel 'Parameter value (Normalized)'
plot './examples/output/experience_jumelle/HV_PFT1/param/z0_opt_HV.dat' using 2 with histograms  ti "Initial value",\
     './examples/output/experience_jumelle/HV_PFT1/param/z0_init_HV.dat' using 2 with histograms  ti "Perturbed",\
     './examples/output/experience_jumelle/HV_PFT1/param/z0_fin_HV.dat' using 2 with histograms ti "After Assimilation"
unset ylabel
####
####
set title "Soil Conductivity (so_cond)" font ",14" offset 0,-1
plot './examples/output/experience_jumelle/HV_PFT1/param/so_cond_opt_HV.dat' using 2 with histograms  ti "Initial value",\
     './examples/output/experience_jumelle/HV_PFT1/param/so_cond_init_HV.dat' using 2 with histograms  ti "Perturbed",\
     './examples/output/experience_jumelle/HV_PFT1/param/so_cond_fin_HV.dat' using 2 with histograms ti "After Assimilation"
####
####
set title "Soil Capacity (so_capa)" font ",14" offset 0,-1
plot './examples/output/experience_jumelle/HV_PFT1/param/so_capa_opt_HV.dat' using 2 with histograms  ti "Initial value",\
     './examples/output/experience_jumelle/HV_PFT1/param/so_capa_init_HV.dat' using 2 with histograms  ti "Perturbed",\
     './examples/output/experience_jumelle/HV_PFT1/param/so_capa_fin_HV.dat' using 2 with histograms ti "After Assimilation"
####
####
set ylabel 'Parameter value (Normalized)'
set title "Surface emissivity (emis)" font ",14" offset 0,-1
plot './examples/output/experience_jumelle/HV_PFT1/param/emis_opt_HV.dat' using 2 with histograms  ti "Initial value",\
     './examples/output/experience_jumelle/HV_PFT1/param/emis_init_HV.dat' using 2 with histograms  ti "Perturbed",\
     './examples/output/experience_jumelle/HV_PFT1/param/emis_fin_HV.dat' using 2 with histograms ti "After Assimilation"
unset ylabel
####
####
set title "Surface albedo (albvis)" font ",14" offset 0,-1
plot './examples/output/experience_jumelle/HV_PFT1/param/albvis_opt_HV.dat' using 2 with histograms  ti "Initial value",\
     './examples/output/experience_jumelle/HV_PFT1/param/albvis_init_HV.dat' using 2 with histograms  ti "Perturbed",\
     './examples/output/experience_jumelle/HV_PFT1/param/albvis_fin_HV.dat' using 2 with histograms ti "After Assimilation"






unset multiplot


pause -1 "Hit return to finish"




