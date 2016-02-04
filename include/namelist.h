//!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//!! NEMO/OPA  :  1 - run manager      (namrun)
//!! namelists    2 - Domain           (nam_zgr, nam_zgr_sco, namdom)
//!!              3 - Surface boundary (namsbc, namsbc_ana, namsbc_flx, namsbc_clio, namsbc_core
//!!                                    namsbc_cpl, namqsr, namsbc_rnf, namsbc_ssr, namalb)
//!!              4 - lateral boundary (namlbc, namcla, namobc, namagrif, nambdy, namtide)
//!!              5 - bottom  boundary (nambfr, nambbc, nambbl)
//!!              6 - Tracer           (nameos, nam_traadv, nam_traldf, namtdp)
//!!              7 - dynamics         (nam_dynadv, nam_dynvor, nam_dynhpg, namflg, nam_dynspg, nam_dynldf)
//!!              8 - Verical physics  (namzdf, namnpc, namric, namtke, namkpp, namddm)
//!!              9 - diagnostics      (namtrd, namgap, namspr, namflo, namptr)
//!!              9 - miscellaneous    (namsol, nam_mpp, nam_mpp_dyndist, namctl)
//!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//!  CAUTION: some scripts does not support CAPITALs for logical use .true./.false., not .TRUE./.FALSE.

//!-----------------------------------------------------------------------
//&namrun        !   parameters of the run
//!-----------------------------------------------------------------------
//   no          =       0   !  job number
//   cexper      =  "GYRE"   !  experience name
//   cn_ocerst_in  = "restart"   !  suffix of ocean restart name (input)
//   cn_ocerst_out = "restart"   !  suffix of ocean restart name (output)
//   ln_rstart   = .false.   !  start from rest (F) or from a restart file (T)
//   nrstdt      =       0   !  restart control = 0 nit000 is not compared to the restart file value
//                           !                  = 1 use ndate0 in namelist (not the value in the restart file)
//                           !                  = 2 calendar parameters read in the restart file
//   nit000      =       1   !  first time step
//   nitend      =    10 !4320   !  last  time step
int ndate0      =  010101;//   !  initial calendar date yymmdd (used if nrstdt=1)
//   nleapy      =      30   !  Leap year calendar (1) or not (0)
//   ninist      =       0   !  output the initial state (1) or not (0)
//   nstock      =    10 ! 4320   !  frequency of creation of a restart file (modulo referenced to 1)
//   nwrite      =      60   !  frequency of write in the output file   (modulo referenced to nit000)
//   ln_dimgnnn  = .false.   !  DIMG file format: 1 file for all processors (F) or by processor (T)
//   ln_mskland  = .false.   !  mask land points (1.e+20) in NetCDF outputs (costly: + ~15%)
///
//!-----------------------------------------------------------------------
//&namdom        !   space and time domain (bathymetry, mesh, timestep)
//!-----------------------------------------------------------------------
//   ntopo       =    0      !  compute (=0) or read(=1) the bathymetry file
double   e3zps_min   =    5.;//     !  the thickness of the partial step is set larger than the minimum
double   e3zps_rat   =    0.1;//    !  of e3zps_min and e3zps_rat * e3t   (N.B. 0<e3zps_rat<1)
//   nmsh        =    1      !  create (=1) a mesh file (coordinates, scale factors, masks) or not (=0)
//   nacc        =    0      !  =1 acceleration of convergence method used, rdt < rdttra(k)
//                           !  =0, no acceleration, rdt = rdttra
double   atfp        =    0.1;//    !  asselin time filter parameter
double   rdt         = 7200.; //    !  time step for the dynamics (and tracer if nacc=0)
double   rdtmin      = 7200.; //    !  minimum time step on tracers (used if nacc=1)
double   rdtmax      = 7200.; //    !  maximum time step on tracers (used if nacc=1)
double   rdth        =  800.; //    !  depth variation of tracer time step  (used if nacc=1)
     //   nn_baro     =   60      !  number of barotropic time step (for the split explicit algorithm) ("key_dynspg_ts")
//   nclosea     =    0      !  = 0 no closed sea in the model domain
//                           !  = 1 closed sea (Black Sea, Caspian Sea, Great US Lakes...)
//!-----------------------------------------------------------------------
//&namqsr        !   penetrative solar radiation
//!-----------------------------------------------------------------------
//   ln_traqsr   = .true.    !  penetrative solar radiation (T) or not (F)
double   rabs        =   0.58;//    !  fraction of qsr associated with xsi1
double   xsi1        =   0.35;//    !  first depth of extinction
double   xsi2        =   23.0;//    !  second depth of extinction

//!-----------------------------------------------------------------------
//&nambfr        !   bottom friction
//!-----------------------------------------------------------------------
//   nbotfr      =    2      !  type of bottom friction :   = 0 : no   slip,  = 2 : nonlinear friction
//                           !                              = 3 : free slip,  = 1 :    linear friction
double   bfri1       =    4.e-4;//  !  bottom drag coefficient (linear case)
double   bfri2       =    1.e-3;//  !  bottom drag coefficient (non linear case)
double   bfeb2       =    2.5e-3;// !  bottom turbulent kinetic energy background  (m^2/s^2)

//!-----------------------------------------------------------------------
//&nameos        !   ocean physical parameters
//!-----------------------------------------------------------------------
//   neos        =    2      !  type of equation of state and Brunt-Vaisala frequency
//                           !     = 0, UNESCO (formulation of Jackett and McDougall (1994) and of McDougall (1987) )
//                           !     = 1, linear: rho(T)   = rau0 * ( 1.028 - ralpha * T )
//                           !     = 2, linear: rho(T,S) = rau0 * ( rbeta * S - ralpha * T )
double   ralpha      =    2.e-4;//  !  thermal expension coefficient (neos= 1 or 2)
double   rbeta       =    7.7e-4;// !  saline  expension coefficient (neos= 2)
//!-----------------------------------------------------------------------
//&nam_traldf    !   lateral diffusion scheme for tracer
//!-----------------------------------------------------------------------
double   aht0        =  1000.;//         !     horizontal eddy diffusivity for tracers [m2/s]
double   ahtb0       =     0.;//         !     background eddy diffusivity for ldf_iso [m2/s]
double   aeiv0       =  1000.;//         !     eddy induced velocity coefficient [m2/s]    (require "key_traldf_eiv")

//!-----------------------------------------------------------------------
//&nam_dynhpg    !   Hydrostatic pressure gradient option
//!-----------------------------------------------------------------------
double   gamm        = 0.e0;//      !  weighting coefficient (wdj scheme)

//!-----------------------------------------------------------------------
//&nam_dynldf    !   lateral diffusion on momentum
//!-----------------------------------------------------------------------
double   ahm0    = 100000.;//            !     horizontal eddy viscosity   [m2/s]
double   ahmb0   =     0.;//             !     background eddy viscosity for ldf_iso [m2/s]

//!-----------------------------------------------------------------------
//&namzdf        !   vertical physics
//!-----------------------------------------------------------------------
double   avm0        =   1.2e-4;//  !  vertical eddy viscosity   [m2/s]          (background Kz if not "key_zdfcst")
double   avt0        =   1.2e-5;//  !  vertical eddy diffusivity [m2/s]          (background Kz if not "key_zdfcst")
double   avevd       = 100.;//      !  vertical coefficient for enhanced diffusion scheme [m2/s]

//!-----------------------------------------------------------------------
//&namsol        !   elliptic solver / island / free surface
//!-----------------------------------------------------------------------
//   nsolv       =      2    !  elliptic solver: =1 preconditioned conjugate gradient (pcg)
//                           !                   =2 successive-over-relaxation (sor)
//                           !                   =3 FETI (fet)                               ("key_feti")
//                           !                   =4 sor with extra outer halo
int   nsol_arp    =      0;//    !  absolute/relative (0/1) precision convergence test
int   nmin        =    210;//    !  minimum of iterations for the SOR solver
int   nmax        =    800;//    !  maximum of iterations for the SOR solver
int   nmod        =     10;//    !  frequency of test for the SOR solver
double   eps         =  1.e-6;//    !  absolute precision of the solver
double   resmax      =  1.e-10;//   !  absolute precision for the SOR solver
double   sor         =  1.96;//     !  optimal coefficient for SOR solver (to be adjusted with the domain)
double   epsisl      =  1.e-10;//   !  absolute precision on stream function solver
int   nmisl       =   4000;//    !  maximum pcg iterations for island                            ("key_islands")
double   rnu         =      1.;//   !  strength of the additional force used in filtered free surface

