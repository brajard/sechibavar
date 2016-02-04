#include "../include/ncutil.h"//netcdf

//--------------------- fonction d'erreur -------------------------------------
using namespace std;

inline void     handle_error (int status, char *txt)
{
        if (status != NC_NOERR)
        { printf ("netcdf error: %s: netcdf statut: ", txt);
        cout << nc_strerror (status) << endl;
        exit (-1);
        }
}

int Ouvre_nc_write(char const *var_file)
{
  ///* ouvre un fichier nc pour Ã©criture et renvoie son id var_file_id
  int var_file_id;
  int status = nc_create(var_file, NC_CLOBBER, &var_file_id);
  if (status != NC_NOERR) {
  char file_error[100];
    sprintf(file_error,"open %s",var_file);
    handle_error (status, file_error);
  }
  return(var_file_id);
}

int Ouvre_nc(char const *var_file ) 
{
///* Ouvre un fichier nc et renvoie son id var_file_id
  int var_file_id;
  int status = nc_open (var_file, NC_NOWRITE | NC_SHARE,&var_file_id);
  if (status != NC_NOERR){
    char file_error[100];
    sprintf(file_error,"open %s",var_file);
    handle_error (status, file_error);
  }
  return(var_file_id);
}

int Var_id(int  var_file_id,char* varn) //initial traceurs actifs
{// avoir l id d un variable a partir de l id var_file_id
  int var_id;
  int status = nc_inq_varid (var_file_id,varn,&var_id);
  if (status != NC_NOERR){
    char var_id_error[100];
    sprintf(var_id_error,"%s_id error",varn);
    handle_error (status, var_id_error);
  }
  return(var_id);
}

void twrite(int t, char *var_name,int ncid,int tid,int var)
{
  //Ecrire un scalaire au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name au point (t)
  int status;

  int var_id;
  static size_t start[] = {0}; //3D+t
  static size_t count[] = {1}; //3D+t
  count[0]= (size_t) t+1;
  start[0]=t; //temps commence de 0;

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_INT,1,&tid,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_int(ncid,var_id,start,count,&var);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}

void twrite(int t, char *var_name,int ncid,int tid,float var)
{
  //Ecrire un scalaire au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name au point (t)
  int status;

  int var_id;
  static size_t start[] = {0}; //3D+t
  static size_t count[] = {1}; //3D+t
  count[0]= (size_t) t+1;
  start[0]=t; //temps commence de 0;

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_FLOAT,1,&tid,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_float(ncid,var_id,start,count,&var);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}

void twrite(int t, char *var_name,int ncid,int tid,double var)
{
  //Ecrire un scalaire au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name au point (t)
  int status;

  int var_id;
  static size_t start[] = {0}; //3D+t
  static size_t count[] = {1}; //3D+t
  count[0]= (size_t) t+1;
  start[0]=t; //temps commence de 0;

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_DOUBLE,1,&tid,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_double(ncid,var_id,start,count,&var);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}


void voncwrite(int t,int znbval,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[])
{ //Ecrrire le volume 3D au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name aux points (t, 1:znbval, 1:ynbval, 1:xnbval)
  int status;

  int var_id;
  int local_dimids[4];
  static size_t start[] = {0,0,0,0}; //3D+t
  static size_t count[] = {1,1,1,1}; //3D+t
  count[0]= (size_t) t+1;
  count[1]= (size_t) znbval;
  count[2]= (size_t) ynbval;
  count[3]= (size_t) xnbval;
  start[0]=t; //temps commence de 0;

  local_dimids[0]=dimids[0]; //t
  local_dimids[1]=dimids[1]; //z
  local_dimids[2]=dimids[2]; //y
  local_dimids[3]=dimids[3]; //x

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_REAL,4,local_dimids,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_real(ncid,var_id,start,count,vect);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}

void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, double vect[])
{// lire volume 3D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,znbval,ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (t,1:znbval,1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim znbval*ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0, 0, 0 };  // 3D+t
  static size_t   count[] = { 1, 1, 1, 1 }; // 3D+t
  count[0] =(size_t)  t+1;
  count[1] =(size_t)  znbval;
  count[2] =(size_t)  ynbval;
  count[3] =(size_t)  xnbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_double (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::vonclire");
    }
}

void soncwrite(int t,int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[])
{ //Ecrrire le volume 2D au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name aux points (t, 1:ynbval, 1:xnbval)
  int status;

  int var_id;
  int local_dimids[3];
  static size_t start[] = {0,0,0}; //2D+t
  static size_t count[] = {1,1,1}; //2D+t
  count[0]= (size_t) t+1;
  count[1]= (size_t) ynbval;
  count[2]= (size_t) xnbval;
  start[0]=t; //temps commence de 0;

  local_dimids[0]=dimids[0]; //t
  local_dimids[1]=dimids[2]; //y
  local_dimids[2]=dimids[3]; //x

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_REAL,3,local_dimids,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_real(ncid,var_id,start,count,vect);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}


void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, double vect[])
{// lire surface 2D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (t,1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0, 0 };  // 2D+t
  static size_t   count[] = { 1, 1, 1 }; // 2D+t
  count[0] =(size_t)  1;
  count[1] =(size_t)  ynbval;
  count[2] =(size_t)  xnbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_double (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::sonclire");
    }
}

void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, float vect[])
{// lire surface 2D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (t,1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0, 0 };  // 2D+t
  static size_t   count[] = { 1, 1, 1 }; // 2D+t
  count[0] =(size_t)  t+1;
  count[1] =(size_t)  ynbval;
  count[2] =(size_t)  xnbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_float (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::sonclire");
    }
}

void snclire(int ynbval, int xnbval, int ncid, int varid, double vect[])
{// lire surface 2D
  // (ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0 };  // 2D
  static size_t   count[] = { 1, 1 }; // 2D
  count[0] =(size_t)  ynbval;
  count[1] =(size_t)  xnbval;
 
  // Read an array of values
  status = nc_get_vara_double (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::snclire");
    }
}

void snclire(int ynbval, int xnbval, int ncid, int varid, float vect[])
{// lire surface 2D
  // (ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0 };  // 2D
  static size_t   count[] = { 1, 1 }; // 2D
  count[0] =(size_t)  ynbval;
  count[1] =(size_t)  xnbval;
 
  // Read an array of values
  status = nc_get_vara_float (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::snclire");
    }
}

void zoncwrite(int t,int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[])
{ //Ecrrire la variable 1D au temps t
  //le parametre t commence de 0.
  // ecrit la variable var_name aux points (t, 1:znbval)
  int status;

  int var_id;
  int local_dimids[2];
  static size_t start[] = {0,0}; //1D+t
  static size_t count[] = {1,1}; //1D+t
  count[0]= (size_t) t+1;
  count[1]= (size_t) znbval;
 
  start[0]=t; //temps commence de 0;

  local_dimids[0]=dimids[0]; //t
  local_dimids[1]=dimids[1]; //z
 

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_REAL,2,local_dimids,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_real(ncid,var_id,start,count,vect);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}


void zonclire(int t,int znbval, int ncid, int varid, double vect[])
{// lire surface 1D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,znbval,ncid, varid):
  // lit la variable varid aux points (t,1:znbval) et
  // renvoie un vecteur de dim znbval
 
  int           status;
  static size_t   start[] = { 0, 0 };  // t+1D
  static size_t   count[] = { 1, 1 }; // t+1D
  count[0] =(size_t)  1;
  count[1] =(size_t)  znbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_double (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::zonclire");
    }
}


void vonclire(int t,int znbval, int ynbval, int xnbval, int ncid, int varid, int vect[])
{// lire volume 3D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,znbval,ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (t,1:znbval,1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim znbval*ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0, 0, 0 };  // 3D+t
  static size_t   count[] = { 1, 1, 1, 1 }; // 3D+t
  count[0] =(size_t)  t+1;
  count[1] =(size_t)  znbval;
  count[2] =(size_t)  ynbval;
  count[3] =(size_t)  xnbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_int (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::vonclire");
    }
}

void sonclire(int t,int ynbval, int xnbval, int ncid, int varid, int vect[])
{// lire surface 2D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,ynbval,xnbval,ncid, varid):
  // lit la variable varid aux points (t,1:ynbval,1:xnbval) et
  // renvoie un vecteur de dim ynbval*xnbval
 
  int           status;
  static size_t   start[] = { 0, 0, 0 };  // 2D+t
  static size_t   count[] = { 1, 1, 1 }; // 2D+t
  count[0] =(size_t)  t+1;
  count[1] =(size_t)  ynbval;
  count[2] =(size_t)  xnbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_int (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::sonclire");
    }
}

void zonclire(int t,int znbval, int ncid, int varid, int vect[])
{// lire surface 1D au temps t
  // le parametre t commence de 0 et pas de 1
  // (t,znbval,ncid, varid):
  // lit la variable varid aux points (t,1:znbval) et
  // renvoie un vecteur de dim znbval
 
  int           status;
  static size_t   start[] = { 0, 0 };  // t+1D
  static size_t   count[] = { 1, 1 }; // t+1D
  count[0] =(size_t)  t+1;
  count[1] =(size_t)  znbval;
  start[0] = (size_t) t;   // temps commence de 0:
 
  // Read an array of values
  status = nc_get_vara_int (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::zonclire");
    }
}

void znclire(int znbval, int ncid, int varid, double vect[])
{// lire surface 1D
  // (znbval,ncid, varid):
  // lit la variable varid aux points (1:znbval) et
  // renvoie un vecteur de dim znbval
 
  int           status;
  static size_t   start[] = { 0 };  //1D
  static size_t   count[] = { 1 }; // 1D
  count[0] =(size_t)  znbval;
 
 
  // Read an array of values
  status = nc_get_vara_double (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::zonclire");
    }
}


void znclire(int znbval, int ncid, int varid, float vect[])
{// lire surface 1D
  // (znbval,ncid, varid):
  // lit la variable varid aux points (1:znbval) et
  // renvoie un vecteur de dim znbval
 
  int           status;
  static size_t   start[] = { 0 };  //1D
  static size_t   count[] = { 1 }; // 1D
  count[0] =(size_t)  znbval;
 
 
  // Read an array of values
  status = nc_get_vara_float (ncid, varid, start, count, vect);
 
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::zonclire");
    }
}

void scalarlire (int ncid,int varid, int *var) {
  int status;
  status=nc_get_var_int (ncid,varid,var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::scalarlire");
    }
}

void scalarlire (int ncid,int varid, float *var) {
  int status;
  status=nc_get_var_float (ncid,varid,var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::scalarlire");
    }
}

void scalarlire (int ncid,int varid, double *var) {
  int status;
  status=nc_get_var_double (ncid,varid,var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur lecture fichier NetCDF::scalarlire");
    }
}

void scalarwrite (int ncid,char const *var_name,int var) {
  int status;
  int var_id;
  status=nc_redef(ncid);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::scalarwrite");
    }
 
  status=nc_def_var(ncid,var_name,NC_INT,0,NULL,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::scalarwrite");
    }
  nc_enddef(ncid);
  status=nc_put_var_int(ncid,var_id,&var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::scalarwrite");
    }
}
void scalarwrite (int ncid,char const *var_name,float var) {
  int status;
  int var_id;
  status=nc_redef(ncid);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::scalarwrite");
    }
 
  status=nc_def_var(ncid,var_name,NC_FLOAT,0,NULL,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::scalarwrite");
    }
  nc_enddef(ncid);
  status=nc_put_var_float(ncid,var_id,&var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::scalarwrite");
    }
}
void scalarwrite (int ncid,char const *var_name,double var) {
  int status;
  int var_id;
  status=nc_redef(ncid);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::scalarwrite");
    }
 
  status=nc_def_var(ncid,var_name,NC_DOUBLE,0,NULL,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::scalarwrite");
    }
  nc_enddef(ncid);
  status=nc_put_var_double(ncid,var_id,&var);
  if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::scalarwrite");
    }
}

void sncwrite(int ynbval, int xnbval, char const *var_name,int ncid,int dimids[], YREAL vect[])
{ //Ecrrire le volume 2D
  // ecrit la variable var_name aux points (1:ynbval, 1:xnbval)
  int status;

  int var_id;
  int local_dimids[2];
  static size_t start[] = {0,0}; //2D
  static size_t count[] = {1,1}; //2D
  count[0]= (size_t) ynbval;
  count[1]= (size_t) xnbval;
 
  local_dimids[0]=dimids[2]; //y
  local_dimids[1]=dimids[3]; //x

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_REAL,2,local_dimids,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_real(ncid,var_id,start,count,vect);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}


void zncwrite(int znbval,char const *var_name,int ncid,int dimids[], YREAL vect[])
{ //Ecrrire la variable 1D
  // ecrit la variable var_name aux points (1:znbval)
  int status;

  int var_id;
  int local_dimids[1];
  static size_t start[] = {0}; //1D
  static size_t count[] = {1}; //1D

  count[0]= (size_t) znbval;
 

  local_dimids[0]=dimids[1]; //z
 

  //Define the netCDF variable
  status=nc_redef(ncid);
if (status != NC_NOERR)
    {
      handle_error (status, "Erreur change to def mode NetCDF ::voncwrite");
    }
 
 status=nc_def_var(ncid,var_name,NC_REAL,1,local_dimids,&var_id);
 
  if (status != NC_NOERR)
    {
      printf("variable %s:",var_name);
      handle_error (status, "Erreur define NetCDF variable ::voncwrite");
    }
 nc_enddef(ncid);
  status=nc_put_vara_real(ncid,var_id,start,count,vect);
 if (status != NC_NOERR)
    {
      handle_error (status, "Erreur writing NetCDF variable ::voncwrite");
    }
}

