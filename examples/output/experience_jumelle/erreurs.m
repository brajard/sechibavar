

rel_var=zeros(3,4);

tobs = load('./HV_PFT1/fluxes/E_tempsol_obs_HV.dat');
tinit = load('./HV_PFT1/fluxes/E_tempsol_init_HV.dat');
tfin = load('./HV_PFT1/fluxes/E_tempsol_fin_HV.dat');

rel_var(1,1) = mean(   abs(tobs(3:end,3)-tinit(3:end,3) )./ tobs(3:end,3) );
rel_var(1,3) = sqrt( sum(  (tobs(3:end,3) - tinit(3:end,3)).*(tobs(3:end,3) - tinit(3:end,3))         )    )/335;
rel_var(1,2) = mean(   abs( tobs(3:end,3) - tfin(3:end,3) )./ tobs(3:end,3) );
rel_var(1,4) = sqrt( sum(  (tobs(3:end,3) - tfin(3:end,3)).*(tobs(3:end,3) - tinit(3:end,3))         )    )/335;



hobs = load('./HV_PFT1/fluxes/E_fluxsens_obs_HV.dat');
hinit = load('./HV_PFT1/fluxes/E_fluxsens_init_HV.dat');
hfin = load('./HV_PFT1/fluxes/E_fluxsens_fin_HV.dat');

rel_var(2,1) = mean(   abs( hobs(3:end,3) - hinit(3:end,3) )./ hobs(3:end,3) );
rel_var(2,3) = sqrt( sum(  (hobs(3:end,3) - hinit(3:end,3)).*(hobs(3:end,3) - hinit(3:end,3))         )    )/335;
rel_var(2,2) = mean(   abs( hobs(3:end,3) - hfin(3:end,3) )./ hobs(3:end,3) );
rel_var(2,4) = sqrt( sum(  (hobs(3:end,3) - hfin(3:end,3)).*(hobs(3:end,3) - hinit(3:end,3))         )    )/335;



LEobs = load('./HV_PFT1/fluxes/E_fluxlat_obs_HV.dat');
LEinit = load('./HV_PFT1/fluxes/E_fluxlat_init_HV.dat');
LEfin = load('./HV_PFT1/fluxes/E_fluxlat_fin_HV.dat');

rel_var(3,1) = mean(   abs( LEobs(3:end,3) - LEinit(3:end,3) )./ LEobs(3:end,3) );
rel_var(3,3) = sqrt( sum(  (LEobs(3:end,3) - LEinit(3:end,3)).*(LEobs(3:end,3) - LEinit(3:end,3))         )    )/335;
rel_var(3,2) = mean(   abs( LEobs(3:end,3) - LEfin(3:end,3) )./ LEobs(3:end,3) );
rel_var(3,4) = sqrt( sum(  (LEobs(3:end,3) - LEfin(3:end,3)).*(LEobs(3:end,3) - LEinit(3:end,3))         )    )/335;



for i = 1 : 3
	fprintf('%2.6e    %2.6e    %2.6e    %2.6e \n',rel_var(i,1),rel_var(i,2),rel_var(i,3),rel_var(i,4) )
end 

%fprintf('Valeurs initiales: tempe %2.6e  Cpinit %2.6e   Ktinit %2.6e   Veginit %2.6e   emis_i %2.6e  alb_i %2.6e \n\n',err_z0_init(y),err_capa_init(y), err_cond_init//(y),err_rveg_init(y),err_emis_init(y), err_albvis_init(y));



rel_var2=zeros(3,4);

tobs = load('./HV_PFT12/fluxes/E_tempsol_obs_HV.dat');
tinit = load('./HV_PFT12/fluxes/E_tempsol_init_HV.dat');
tfin = load('./HV_PFT12/fluxes/E_tempsol_fin_HV.dat');

rel_var2(1,1) = mean(   abs(tobs(3:end,3)-tinit(3:end,3) )./ tobs(3:end,3) );
rel_var2(1,3) = sqrt( sum(  (tobs(3:end,3) - tinit(3:end,3)).*(tobs(3:end,3) - tinit(3:end,3))         )    )/335;
rel_var2(1,2) = mean(   abs( tobs(3:end,3) - tfin(3:end,3) )./ tobs(3:end,3) );
rel_var2(1,4) = sqrt( sum(  (tobs(3:end,3) - tfin(3:end,3)).*(tobs(3:end,3) - tinit(3:end,3))         )    )/335;



hobs = load('./HV_PFT12/fluxes/E_fluxsens_obs_HV.dat');
hinit = load('./HV_PFT12/fluxes/E_fluxsens_init_HV.dat');
hfin = load('./HV_PFT12/fluxes/E_fluxsens_fin_HV.dat');

rel_var2(2,1) = mean(   abs( hobs(3:end,3) - hinit(3:end,3) )./ hobs(3:end,3) );
rel_var2(2,3) = sqrt( sum(  (hobs(3:end,3) - hinit(3:end,3)).*(hobs(3:end,3) - hinit(3:end,3))         )    )/335;
rel_var2(2,2) = mean(   abs( hobs(3:end,3) - hfin(3:end,3) )./ hobs(3:end,3) );
rel_var2(2,4) = sqrt( sum(  (hobs(3:end,3) - hfin(3:end,3)).*(hobs(3:end,3) - hinit(3:end,3))         )    )/335;



LEobs = load('./HV_PFT12/fluxes/E_fluxlat_obs_HV.dat');
LEinit = load('./HV_PFT12/fluxes/E_fluxlat_init_HV.dat');
LEfin = load('./HV_PFT12/fluxes/E_fluxlat_fin_HV.dat');

rel_var2(3,1) = mean(   abs( LEobs(3:end,3) - LEinit(3:end,3) )./ LEobs(3:end,3) );
rel_var2(3,3) = sqrt( sum(  (LEobs(3:end,3) - LEinit(3:end,3)).*(LEobs(3:end,3) - LEinit(3:end,3))         )    )/335;
rel_var2(3,2) = mean(   abs( LEobs(3:end,3) - LEfin(3:end,3) )./ LEobs(3:end,3) );
rel_var2(3,4) = sqrt( sum(  (LEobs(3:end,3) - LEfin(3:end,3)).*(LEobs(3:end,3) - LEinit(3:end,3))         )    )/335;


fprintf('\n\n\n')

for i = 1 : 3
	fprintf('%2.6e    %2.6e    %2.6e    %2.6e \n',rel_var2(i,1),rel_var2(i,2),rel_var2(i,3),rel_var2(i,4) )
end 



emisobs =load('./HV_PFT12/param/emis_opt_HV.dat' );
emisinit =load('./HV_PFT12/param/emis_init_HV.dat' );
emisfin =load('./HV_PFT12/param/emis_fin_HV.dat');
rel_var3(1,1) = mean(   abs( emisobs(2) - emisinit(2) )./ emisobs(2) );
rel_var3(1,2) = mean(   abs( emisobs(2) - emisfin(2) )./ emisobs(2) );

zobs =load('./HV_PFT12/param/z0_opt_HV.dat' );
zinit =load('./HV_PFT12/param/z0_init_HV.dat' );
zfin =load('./HV_PFT12/param/z0_fin_HV.dat');
rel_var3(2,1) = mean(   abs( zobs(2) - zinit(2) )./ zobs(2) );
rel_var3(2,2) = mean(   abs( zobs(2) - zfin(2) )./ zobs(2) );

so_capaobs =load('./HV_PFT12/param/so_capa_opt_HV.dat' );so_capainit =load('./HV_PFT12/param/so_capa_init_HV.dat' );so_capafin =load('./HV_PFT12/param/so_capa_fin_HV.dat');rel_var3(3,1) = mean(   abs( so_capaobs(2) - so_capainit(2) )./ so_capaobs(2) );rel_var3(3,2) = mean(   abs( so_capaobs(2) - so_capafin(2) )./ so_capaobs(2) );so_condobs =load('./HV_PFT12/param/so_cond_opt_HV.dat' );so_condinit =load('./HV_PFT12/param/so_cond_init_HV.dat' );so_condfin =load('./HV_PFT12/param/so_cond_fin_HV.dat');rel_var3(4,1) = mean(   abs( so_condobs(2) - so_condinit(2) )./ so_condobs(2) );rel_var3(4,2) = mean(   abs( so_condobs(2) - so_condfin(2) )./ so_condobs(2) );rvegobs =load('./HV_PFT12/param/rveg_opt_HV.dat' );rveginit =load('./HV_PFT12/param/rveg_init_HV.dat' );rvegfin =load('./HV_PFT12/param/rveg_fin_HV.dat');rel_var3(5,1) = mean(   abs(rvegobs(2) - rveginit(2) )./ rvegobs(2) );rel_var3(5,2) = mean(   abs(rvegobs(2) - rvegfin(2) )./ rvegobs(2) );

fprintf('\n\n\n')

for i = 1 : 5
	fprintf('%2.6e    %2.6e  \n',rel_var3(i,1),rel_var3(i,2) )
end 




emisobs =load('./HV_PFT1/param/emis_opt_HV.dat' );emisinit =load('./HV_PFT1/param/emis_init_HV.dat' );emisfin =load('./HV_PFT1/param/emis_fin_HV.dat');rel_var4(1,1) = mean(   abs( emisobs(2) - emisinit(2) )./ emisobs(2) );rel_var4(1,2) = mean(   abs( emisobs(2) - emisfin(2) )./ emisobs(2) );zobs =load('./HV_PFT1/param/z0_opt_HV.dat' );zinit =load('./HV_PFT1/param/z0_init_HV.dat' );zfin =load('./HV_PFT1/param/z0_fin_HV.dat');rel_var4(2,1) = mean(   abs( zobs(2) - zinit(2) )./ zobs(2) );rel_var4(2,2) = mean(   abs( zobs(2) - zfin(2) )./ zobs(2) );so_capaobs =load('./HV_PFT1/param/so_capa_opt_HV.dat' );so_capainit =load('./HV_PFT1/param/so_capa_init_HV.dat' );so_capafin =load('./HV_PFT1/param/so_capa_fin_HV.dat');rel_var4(3,1) = mean(   abs( so_capaobs(2) - so_capainit(2) )./ so_capaobs(2) );rel_var4(3,2) = mean(   abs( so_capaobs(2) - so_capafin(2) )./ so_capaobs(2) );so_condobs =load('./HV_PFT1/param/so_cond_opt_HV.dat' );so_condinit =load('./HV_PFT1/param/so_cond_init_HV.dat' );so_condfin =load('./HV_PFT1/param/so_cond_fin_HV.dat');rel_var4(4,1) = mean(   abs( so_condobs(2) - so_condinit(2) )./ so_condobs(2) );rel_var4(4,2) = mean(   abs( so_condobs(2) - so_condfin(2) )./ so_condobs(2) );rvegobs =load('./HV_PFT1/param/albvis_opt_HV.dat' );rveginit =load('./HV_PFT1/param/albvis_init_HV.dat' );rvegfin =load('./HV_PFT1/param/albvis_fin_HV.dat');rel_var4(5,1) = mean(   abs(rvegobs(2) - rveginit(2) )./ rvegobs(2) );rel_var4(5,2) = mean(   abs(rvegobs(2) - rvegfin(2) )./ rvegobs(2) );fprintf('\n\n\n')for i = 1 : 5	fprintf('%2.6e    %2.6e  \n',rel_var4(i,1),rel_var4(i,2) )end



