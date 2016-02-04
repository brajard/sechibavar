load /.autofs/neuro/com/hbplod/DOCTORADO/yao_projects/SECHIBA/sechiba4/output/real/assimilations/todos_1sem_ete_SMOS2006/tempsol_fin.dat  

load  /.autofs/neuro/com/hbplod/DOCTORADO/yao_projects/SECHIBA/sechiba4/input/real/SMOSREX/observations/SMOSREX2005_1sem__tempsol.dat    

load /.autofs/neuro/com/hbplod/DOCTORADO/yao_projects/SECHIBA/sechiba4/output/real/sim_humidite/tempsol_obs.dat    




 plot(tempsol_fin(2:334,3),'r-')    
hold on
plot(SMOSREX2005_1sem__tempsol(:,3))   
plot(tempsol_obs(2:334,3),'k-')       





prior = tempsol_obs(2:334,3);     
after = tempsol_fin(2:334,3);                                                           
obs = SMOSREX2005_1sem__tempsol(:,3);     



resPrior = prior - obs;
resAfter = after -obs;

RMSEPrior = sqrt(sum(resPrior.*resPrior)/size(resPrior,1)  );
RMSEAfter = sqrt(sum(resAfter.*resAfter)/size(resAfter,1)  );
