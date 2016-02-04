#define nav_lon( i, j ) (nav_lon[(j)*(NX)+(i)])
#define nav_lat( i, j ) (nav_lat[(j)*(NX)+(i)])
#define e1u( i, j )  (e1u[(j)*(NX)+(i)])
#define e2u( i, j )  (e2u[(j)*(NX)+(i)])
#define e1v( i, j )  (e1v[(j)*(NX)+(i)])
#define e2v( i, j )  (e2v[(j)*(NX)+(i)])
#define e1t( i, j )  (e1t[(j)*(NX)+(i)])
#define e2t( i, j )  (e2t[(j)*(NX)+(i)])
#define e1f( i, j )  (e1f[(j)*(NX)+(i)])
#define e2f( i, j )  (e2f[(j)*(NX)+(i)])
#define gphit( i, j) (gphit[(j)*(NX)+(i)])
#define gphiu( i, j) (gphiu[(j)*(NX)+(i)])
#define gphiv( i, j) (gphiv[(j)*(NX)+(i)])
#define gphif( i, j) (gphif[(j)*(NX)+(i)])
#define tmask( i, j, k) (tmask[(k)*(NY*NX)+(j)*(NX)+(i)])
#define umask( i, j, k) (umask[(k)*(NY*NX)+(j)*(NX)+(i)])
#define vmask( i, j, k) (vmask[(k)*(NY*NX)+(j)*(NX)+(i)])
#define fmask( i, j, k) (fmask[(k)*(NY*NX)+(j)*(NX)+(i)])
#define tmask_i( i, j )  (tmask_i[(j)*(NX)+(i)])  //mohamed (18/03/09) voir domwri.F90
#define bmask( i, j )   (bmask[(j)*(NX)+(i)])  //mohamed (21/04/09) voir domwri.F90

// tous les indices commencent de 0
//domzgr_substitute.h90 key_zco
#   define  nav_lev( i , j , k ) nav_lev[k]
#   define  fsdept( i , j , k )  gdept_0[k]
#   define  fsdepw( i , j , k )  gdepw_0[k]
#   define  fsde3w( i , j , k )  gdepw_0[k]
#   define  fse3t( i , j , k )   e3t_0[k]
#   define  fse3u( i , j , k )   e3t_0[k]
#   define  fse3v( i , j , k )   e3t_0[k]
#   define  fse3f( i , j , k )   e3t_0[k]
#   define  fse3w( i , j , k )   e3w_0[k]
#   define  fse3uw( i , j , k )  e3w_0[k]
#   define  fse3vw( i , j , k )  e3w_0[k]
//
#   define  ff( i , j )   (ff[(j)*(NX)+(i)])
#   define  mbathy( i , j )   (mbathy[(j)*(NX)+(i)])

//!   Defautl option :                     avs = avt
#   define   fsavs(i,j,k)   avt(i,j,k)
//ldftra_substitute.h90
//substitute fsaht. the eddy diffusivity coeff
//!   Default option :             aht: Constant coefficient
#      define   fsahtt(i,j,k)   aht0
#      define   fsahtu(i,j,k)   aht0
#      define   fsahtv(i,j,k)   aht0
#      define   fsahtw(i,j,k)   aht0

//ldfdyn_substitute.h90
//the lateral eddy viscosity coeff.
//   default option :  Constant coefficient
#  define   fsahmt(i,j,k)   ahm0
#  define   fsahmf(i,j,k)   ahm0
#  define   fsahmu(i,j,k)   ahm0
#  define   fsahmv(i,j,k)   ahm0

double nav_lon[NY*NX]; // from mesh_mask.nc
double nav_lat[NY*NX]; // from mesh_mask.nc
double e1u[NY*NX];    // from mesh_mask.nc
double e2u[NY*NX];    // from mesh_mask.nc
double e1v[NY*NX];    // from mesh_mask.nc
double e2v[NY*NX];    // from mesh_mask.nc
double e1t[NY*NX];    // from mesh_mask.nc
double e2t[NY*NX];    // from mesh_mask.nc
double e1f[NY*NX];    // from mesh_mask.nc
double e2f[NY*NX];    // from mesh_mask.nc
double nav_lev[NZ];   // from mesh_mask.nc
double e3t_0[NZ];    // from mesh_mask.nc
double e3w_0[NZ];    // from mesh_mask.nc
double gphit[NY*NX]; // from mesh_mask.nc
double gphiu[NY*NX]; // from mesh_mask.nc
double gphiv[NY*NX]; // from mesh_mask.nc
double gphif[NY*NX]; // from mesh_mask.nc
double ff[NY*NX]; //
int mbathy[NY*NX];// from mesh_mask.nc
double gdept_0[NZ]; //  from mesh_mask.nc
double gdepw_0[NZ]; // from mesh_mask.nc
int tmask[NZ*NY*NX]; // from mesh_mask.nc
int umask[NZ*NY*NX]; // from mesh_mask.nc
int vmask[NZ*NY*NX]; // from mesh_mask.nc
int fmask[NZ*NY*NX]; // from mesh_mask.nc
int tmask_i[NY*NX]; // from mesh_mask.nc
int bmask[NY*NX]; // from mesh_mask.nc

typedef float netcdf_type;

char const    *mesh_mask_file    = PATH_NCFILES"/mesh_mask.nc";
int mesh_mask_file_id;
int nav_lon_id, nav_lat_id, nav_lev_id; //Utiliser juste pour info
int e1u_id,e2u_id,e1v_id,e2v_id,e1t_id,e2t_id,e1f_id,e2f_id,e3t_0_id,e3w_0_id;
int mbathy_id,gphit_id,gphiu_id,gphiv_id,gphif_id,gdept_0_id,gdepw_0_id;
int tmask_id,umask_id,vmask_id,fmask_id,tmask_i_id,bmask_id; // l'id des variables a lire
int status;

int neuler=0; //restart file

int nyear=1; // current year
int nmonth=1; //current month of the year nyear
int nday=1; //current day of the year nyear
double ndastp=nyear*10000 + nmonth*100 + nday; //date at the end of the current (previous) run
double adatrj=0; //date in days since the beginning of the run
double kt=0; //ocean time step


#   define        ze1ur(i,j)  e2u(i,j) / e1u(i,j)
#   define        ze2vr(i,j)  e1v(i,j) / e2v(i,j)
#   define        zbtr2(i,j)  1. / ( e1t(i,j) * e2t(i,j))

void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, double vect[]);
// lire volume 3D au temps t
void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, double vect[]);
void zonclire(int t,int znbval, int ncid, int varid, double vect[]);
// lire surface 2D au temps t
void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, int vect[]);
// lire volume 3D au temps t
void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, int vect[]);
void zonclire(int t,int znbval, int ncid, int varid, int vect[]);
// lire surface 2D au temps t
void xinit_mesh_mask_nc();// lire le fichier mesh_mask.nc
void xistate_rest_file();



//Ecriture du netcdf
void write_rst_global_att(int ncid); //Ecrit les attributs globaux du restart standard
void voncwrite(int t,int znbval,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);
void soncwrite(int t,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);
void zoncwrite(int t,int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[]);
void zncwrite(int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[]);
void sncwrite(int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[]);
void define_dim(int ncid, int dimid[]);
//dÃ©fini les dimensions en x,y,z, et t (unlimited),
//renvoie les id des dimensions dans le tableau dimid
//dimid={t_id,z_id,y_id,x_id};

void    xinit_mesh_mask_nc()
{
  //ouvrire le fichier mesh_mask.nc
  mesh_mask_file_id=Ouvre_nc(mesh_mask_file);

  nav_lon_id=Var_id(mesh_mask_file_id,"nav_lon");
  snclire(NY,NX, mesh_mask_file_id, nav_lon_id,nav_lon);
 
  nav_lat_id=Var_id(mesh_mask_file_id,"nav_lat");
  snclire(NY,NX, mesh_mask_file_id, nav_lat_id,nav_lat);

  e1u_id=Var_id(mesh_mask_file_id,"e1u");
  sonclire(0, NY,NX, mesh_mask_file_id, e1u_id,e1u);

  e2u_id=Var_id(mesh_mask_file_id,"e2u");
  sonclire(0, NY,NX, mesh_mask_file_id, e2u_id,e2u);

  e1v_id=Var_id(mesh_mask_file_id,"e1v");
  sonclire(0, NY,NX, mesh_mask_file_id, e1v_id,e1v);

  e2v_id=Var_id(mesh_mask_file_id,"e2v");
  sonclire(0, NY,NX, mesh_mask_file_id, e2v_id,e2v);

  e1t_id=Var_id(mesh_mask_file_id,"e1t");
  sonclire(0, NY,NX, mesh_mask_file_id, e1t_id,e1t);

  e2t_id=Var_id(mesh_mask_file_id,"e2t");
  sonclire(0, NY,NX, mesh_mask_file_id, e2t_id,e2t);

  e1f_id=Var_id(mesh_mask_file_id,"e1f");
  sonclire(0, NY,NX, mesh_mask_file_id, e1f_id,e1f);

  e2f_id=Var_id(mesh_mask_file_id,"e2f");
  sonclire(0, NY,NX, mesh_mask_file_id, e2f_id,e2f);

  e3t_0_id=Var_id(mesh_mask_file_id,"e3t_0");
  zonclire(0, NZ, mesh_mask_file_id, e3t_0_id,e3t_0);

  e3w_0_id=Var_id(mesh_mask_file_id,"e3w_0");
  zonclire(0, NZ, mesh_mask_file_id, e3w_0_id,e3w_0);

  mbathy_id=Var_id(mesh_mask_file_id,"mbathy");
  sonclire(0, NY,NX, mesh_mask_file_id, mbathy_id,mbathy);

  gphit_id=Var_id(mesh_mask_file_id,"gphit");
  sonclire(0, NY,NX, mesh_mask_file_id, gphit_id,gphit);

  gphiu_id=Var_id(mesh_mask_file_id,"gphiu");
  sonclire(0, NY,NX, mesh_mask_file_id, gphiu_id,gphiu);

  gphiv_id=Var_id(mesh_mask_file_id,"gphiv");
  sonclire(0, NY,NX, mesh_mask_file_id, gphiv_id,gphiv);

  gphif_id=Var_id(mesh_mask_file_id,"gphif");
  sonclire(0, NY,NX, mesh_mask_file_id, gphif_id,gphif);

  tmask_id=Var_id(mesh_mask_file_id,"tmask");
  vonclire(0, NZ, NY,NX, mesh_mask_file_id, tmask_id,tmask);

  umask_id=Var_id(mesh_mask_file_id,"umask");
  vonclire(0, NZ, NY,NX, mesh_mask_file_id, umask_id,umask);

  vmask_id=Var_id(mesh_mask_file_id,"vmask");
  vonclire(0, NZ, NY,NX, mesh_mask_file_id, vmask_id,vmask);

  fmask_id=Var_id(mesh_mask_file_id,"fmask");
  vonclire(0, NZ, NY,NX, mesh_mask_file_id, fmask_id,fmask);

  tmask_i_id=Var_id(mesh_mask_file_id,"tmask_i");
  sonclire(0, NY,NX, mesh_mask_file_id, tmask_i_id,tmask_i);

  bmask_id=Var_id(mesh_mask_file_id,"bmask");
  sonclire(0, NY,NX, mesh_mask_file_id, bmask_id,bmask);

  nav_lev_id=Var_id(mesh_mask_file_id,"nav_lev");
  znclire(NZ, mesh_mask_file_id, nav_lev_id,nav_lev);

  gdept_0_id=Var_id(mesh_mask_file_id,"gdept_0");
  zonclire(0, NZ, mesh_mask_file_id, gdept_0_id,gdept_0);

  gdepw_0_id=Var_id(mesh_mask_file_id,"gdepw_0");
  zonclire(0, NZ, mesh_mask_file_id, gdepw_0_id,gdepw_0);


  //gdept_0
  nc_close( mesh_mask_file_id);
}

void xistate_rest_file(int rest_file_id){
  double temp[NZ*NY*NX];
  int temp_id;

  //-----------un
  temp_id=Var_id(rest_file_id,"un");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++){
    for(int j=0;j<NY;j++){
      for(int i=0;i<NX;i++){
        YS_ua(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }
    }
  }
  //-----------ub
  temp_id=Var_id(rest_file_id,"ub");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++){
    for(int j=0;j<NY;j++){
      for(int i=0;i<NX;i++){
        YS_ub(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }
    }
  }
 
  //-----------vn
  temp_id=Var_id(rest_file_id,"vn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++){
    for(int j=0;j<NY;j++){
      for(int i=0;i<NX;i++){
        YS_va(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }
    }
  }
 
  //-----------vb
  temp_id=Var_id(rest_file_id,"vb");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_vb(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------wn
  //if( lk_vvl )
  /*  temp_id=Var_id(rest_file_id,"wn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++){
    for(int j=0;j<NY;j++){
      for(int i=0;i<NX;i++){
        YS_wa(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }
    }
    }*/
  //-----------
  temp_id=Var_id(rest_file_id,"tn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_ta(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"tb");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_tb(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"sn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_sa(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"sb");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_sb(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"sshn");
  sonclire(0, NY, NX, rest_file_id, temp_id,temp);
  for(int j=0;j<NY;j++)
    for(int i=0;i<NX;i++){
      YS_sshn(0,i,j,TU)=temp[(j)*(NX)+(i)];
    }

  //-----------
  temp_id=Var_id(rest_file_id,"sshb");
  sonclire(0, NY, NX, rest_file_id, temp_id,temp);
  for(int j=0;j<NY;j++)
    for(int i=0;i<NX;i++){
      YS_sshb(0,i,j,TU)=temp[(j)*(NX)+(i)];
    }

  //-----------
  temp_id=Var_id(rest_file_id,"hdivn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_hdivn(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"hdivb");// a revoir
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_hdivn(0,i,j,k,TU-1)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"rotn");
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_rotn(0,i,j,k,TU)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"rotb"); // a revoir
  vonclire(0, NZ, NY, NX, rest_file_id, temp_id,temp);
  for(int k=0;k<NZ;k++)
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_rotn(0,i,j,k,TU-1)=temp[(k)*(NY*NX)+(j)*(NX)+(i)];
      }

   //-----------
  temp_id=Var_id(rest_file_id,"gcx");
  sonclire(0, NY, NX, rest_file_id, temp_id,temp);
    for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        YS_gcx2(0,i,j,TU)=temp[(j)*(NX)+(i)];
      }

  //-----------
  temp_id=Var_id(rest_file_id,"gcxb");
  sonclire(0, NY, NX, rest_file_id, temp_id,temp);
  for(int j=0;j<NY;j++)
    for(int i=0;i<NX;i++){
      YS_gcx2(0,i,j,TU-1)=temp[(j)*(NX)+(i)];
    }
  // ndastp
  temp_id=Var_id(rest_file_id,"ndastp");
  scalarlire(rest_file_id,temp_id,&ndastp);

  //adatrj
  temp_id=Var_id(rest_file_id,"adatrj");
  scalarlire(rest_file_id,temp_id,&adatrj);

  //kt
temp_id=Var_id(rest_file_id,"kt");
  scalarlire(rest_file_id,temp_id,&kt);

}

void define_dim(int ncid, int dimid[])
//dÃ©fini les dimensions en x,y,z, et t (unlimited),
//renvoie les id des dimensions dans le tableau dimid
//dimid={t_id,z_id,y_id,x_id};
{ 
  nc_redef(ncid);
  nc_def_dim(ncid,"x",NX,&dimid[3]);
  nc_def_dim(ncid,"y",NY,&dimid[2]);
  nc_def_dim(ncid,"z",NZ,&dimid[1]);
  nc_def_dim(ncid,"t",NC_UNLIMITED,&dimid[0]);
  nc_enddef(ncid);
 
 
}

void write_rst_var(int t,   int ncid, int dimids[]) {
YREAL vect[NZ*NY*NX];
 
//nav_lon
 for(int j=0;j<NY;j++)
      for(int i=0;i<NX;i++){
        vect[(j)*(NX)+(i)]=nav_lon( i, j );
      }
 sncwrite(NY,NX,"nav_lon",ncid,dimids,vect);
 
 //nav_lat
 for(int j=0;j<NY;j++)
   for(int i=0;i<NX;i++){
     vect[(j)*(NX)+(i)]=nav_lat( i, j );
   }
 sncwrite(NY,NX,"nav_lat",ncid,dimids,vect);
 
 //nav_lev
 for(int k=0;k<NZ;k++){
  vect[k]=nav_lev[k]; 
}
zncwrite(NZ,"nav_lev",ncid,dimids,vect);

//time_counter
 twrite(0,"time_counter",ncid,dimids[0],kt);


//gcx
for(int j=0;j<NY;j++)
   for(int i=0;i<NX;i++){
     vect[(j)*(NX)+(i)]=YS_gcx2(0,i,j,t);
   }
 soncwrite(0,NY,NX,"gcx",ncid,dimids,vect);
 
//gcxb
for(int j=0;j<NY;j++)
   for(int i=0;i<NX;i++){
     vect[(j)*(NX)+(i)]=YS_gcx2(0,i,j,t-1);
   }
 soncwrite(0,NY,NX,"gcxb",ncid,dimids,vect);
 
//sshb
for(int j=0;j<NY;j++)
   for(int i=0;i<NX;i++){
     vect[(j)*(NX)+(i)]=YS_sshb(0,i,j,t);
   }
 soncwrite(0,NY,NX,"sshb",ncid,dimids,vect);
 
//sshn
for(int j=0;j<NY;j++)
   for(int i=0;i<NX;i++){
     vect[(j)*(NX)+(i)]=YS_sshn(0,i,j,t);
   }
 soncwrite(0,NY,NX,"sshn",ncid,dimids,vect);
 
 //rdt
 scalarwrite(ncid,"rdt",rdt);
 
//rdttra1
 scalarwrite(ncid,"rdttra1",rdt);

 //ub
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_ub(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"ub",ncid,dimids,vect);

//vb
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_vb(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"vb",ncid,dimids,vect);

//tb
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_tb(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"tb",ncid,dimids,vect);

//sb
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_sb(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"sb",ncid,dimids,vect);

//rotb
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_rotn(0,i,j,k,t-1);
     }
 voncwrite(0,NZ,NY,NX,"rotb",ncid,dimids,vect);

//hdivb
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_hdivn(0,i,j,k,t-1);
     }
 voncwrite(0,NZ,NY,NX,"hdivb",ncid,dimids,vect);

//un
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_ua(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"un",ncid,dimids,vect);

//vn
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_va(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"vn",ncid,dimids,vect);

//tn
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_ta(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"tn",ncid,dimids,vect);

//sn
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_sa(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"sn",ncid,dimids,vect);

//rotn
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_rotn(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"rotn",ncid,dimids,vect);

//hdivn
 for(int k=0;k<NZ;k++)
   for(int j=0;j<NY;j++)
     for(int i=0;i<NX;i++){
       vect[(k)*(NY*NX)+(j)*(NX)+(i)]=YS_hdivn(0,i,j,k,t);
     }
 voncwrite(0,NZ,NY,NX,"hdivn",ncid,dimids,vect);

 //kt
 scalarwrite(ncid,"kt",kt);

 //ndastp
 scalarwrite(ncid,"ndastp",ndastp);

 //adatrk
 scalarwrite(ncid,"adatrj",adatrj);


}



void write_rst_global_att(int ncid) {

  //:DOMAIN_number_total = 1 ;
  static int DOMAIN_number_total=1;
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_number_total",NC_INT,1,&DOMAIN_number_total);

  //:DOMAIN_number = 0 ;
  static int DOMAIN_number=0;
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_number",NC_INT,1,&DOMAIN_number);

  //:DOMAIN_dimensions_ids = 1, 2 ;
  static int DOMAIN_dimensions_ids[]={1,2};
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_dimensions_ids",NC_INT,2,DOMAIN_dimensions_ids);

  //:DOMAIN_size_global = 32, 22 ;
  static int DOMAIN_size_global[]={NX,NY};
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_size_global",NC_INT,2,DOMAIN_size_global);

  //:DOMAIN_size_local = 32, 22 ;
  static int DOMAIN_size_local[]={NX,NY};
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_size_local",NC_INT,2,DOMAIN_size_local);

  //:DOMAIN_position_first = 1, 1 ;
  static int DOMAIN_position_first[]={1,1};
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_position_first",NC_INT,2,DOMAIN_position_first);

  //:DOMAIN_position_last = 32, 22 ;
 static int DOMAIN_position_last[]={NX,NY};
 nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_position_last",NC_INT,2,DOMAIN_position_last);

 //:DOMAIN_halo_size_start = 0, 0 ;
 static int DOMAIN_halo_size_start[]={0,0};
 nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_halo_size_start",NC_INT,2,DOMAIN_halo_size_start);

 //:DOMAIN_halo_size_end = 0, 0 ;
 static int DOMAIN_halo_size_end[]={0,0};
  nc_put_att_int (ncid,NC_GLOBAL,"DOMAIN_halo_size_end",NC_INT,2,DOMAIN_halo_size_end);

  //DOMAIN_type = "BOX" ;
  nc_put_att_text(ncid,NC_GLOBAL,"DOMAIN_type",3,"BOX");

}

