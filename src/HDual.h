#ifndef HDUAL_H_
#define HDUAL_H_

#include "HModel.h"
#include "HMatrix.h"
#include "HFactor.h"
#include "HVector.h"
#include "HDualRow.h"
#include "HDualRHS.h"
#include "HCrash.h"

#include <set>
#include <string>
#include <vector>
using namespace std;

const int HSOL_THREAD_LIMIT = 32;
const int HSOL_SLICED_LIMIT = 100;

//Define the EdWt modes
const int EdWt_Mode_DSE = 0;
const int EdWt_Mode_Dvx = 1;
const int EdWt_Mode_Dan = 2;

//Define the Price modes
const int Price_Mode_Row = 0;
const int Price_Mode_Col = 1;

//Define the Crash modes
const int Crash_Mode_No = 0;
const int Crash_Mode_LTSSF_k = 1;
const int Crash_Mode_LTSSF_pri = 2;
const int Crash_Mode_LTSF_k = 3;
const int Crash_Mode_LTSF_pri = 4;
const int Crash_Mode_LTSF = 5;
const int Crash_Mode_Bixby = 6;
const int Crash_Mode_Dev = 7;
const int Crash_Mode_Bs = 8;
#ifdef HiGHSDEV
const int Crash_Mode_TsSing = 9;
#endif
const int Crash_Mode_Df = Crash_Mode_LTSSF_pri;

//Define the Presolve modes
const int Presolve_Mode_Off = 0;
const int Presolve_Mode_On = 1;

//Define the Devex status flags
//Using 1 and 0 allows dvx_ix to be used as a multiplier to save a conditional branch
const int dvx_in_R = 1;
const int dvx_not_in_R = 0;

//Define the parameters controlling number of Devex iterations
const int mn_n_dvx_it = 25;
const double nw_dvx_fwk_fq = 1e-2;
const double tl_dvx_wt = 3.0;

const double runningAverageMu = 0.05;

const bool noInvertWhenOptimal = false;

//Set limits on problem size for reporting
const bool rp_hsol = false;
const bool rp_dvx = false;
//const bool rp_hsol = true;
//const bool rp_dvx = true;
const int mx_rp_numTot = 20;

enum HDUAL_VARIANT
{
  HDUAL_VARIANT_PLAIN = 0,
  HDUAL_VARIANT_TASKS,
  HDUAL_VARIANT_MULTI,
};

class HDual
{
public:
  void solve(HModel *model, int variant = 0, int num_threads = 1);

public:
  void init(int num_threads);
  void init_slice(int init_sliced_num);

  void solve_phase1();
  void solve_phase2();

  void rebuild();
  void cleanup();

  void iterate();
  void iterate_tasks();
  void iterate_multi(); // in HDualMulti.cpp
  void iterateIzAn();
  void iterateAn();
  void iterateRp();
  void iterateRpFull(bool header);
  void iterateRpBrief(bool header);
  void iterateRpInvert(int i_v);
  void iterateRpIterPh(bool header);
  void iterateRpDuObj(bool header);
  double uOpRsDensityRec(double lc_OpRsDensity, double *opRsDensity, double *opRsAvDensity, double *opRsAvLog10Density);

  void chooseRow();

  void chooseColumn(HVector *row_ep);
  void chooseColumn_slice(HVector *row_ep);

  void updateFtran();
  void updateFtranBFRT();
  void updateFtranDSE(HVector *DSE_Vector);
  void updateVerify();
  void updateDual();
  void updatePrimal(HVector *DSE_Vector);
  void updatePivots();

  void handleRankDeficiency();
  void iz_dvx_fwk();
  void setCrash(const char *CrashMode);
  void setPrice(const char *PriceMode);
  void setEdWt(const char *EdWtMode);
  void setTimeLimit(double TimeLimit_ArgV);
  void setPresolve(const char *PresolveMode);

  // Utility to get a row of the inverse of B for SCIP
  int util_getBasisInvRow(int r, double *coef, int *inds, int *ninds);

  double an_bs_cond(HModel *ptr_model);

  void major_chooseRow();
  void major_chooseRowBtran();
  void minor_chooseRow();

  void minor_update();
  void minor_updateDual();
  void minor_updatePrimal();
  void minor_updatePivots();
  void minor_updateRows();

  void major_update();
  void major_updateFtranPrepare();
  void major_updateFtranParallel();
  void major_updateFtranFinal();
  void major_updatePrimal();
  void major_updateFactor();

  void major_rollback();

#ifdef HiGHSDEV
  void iterateRpIterDa(bool header);
  void iterateRpDsty(bool header);
  int intLog10(double v);
  void iterateOpRecBf(int opTy, HVector &vector, double hist_dsty);
  void iterateOpRecAf(int opTy, HVector &vector);
  void iterateRpAn();
  void rp_hsol_da_str();
  //void rp_hsol_si_it();
  void rp_hsol_pv_c(HVector *column) const;
  void rp_hsol_sol(HModel *ptr_model);
  void an_iz_vr_v();
#endif

  // Variant choice
  int dual_variant = 0;
  int Price_Mode = 0;    //Row-wise PRICE
  int EdWt_Mode = 0;     //DSE
  int Crash_Mode = 0;    //No crash
  int Presolve_Mode = 0; //No presolve

  bool SolveBailout;
  double TimeLimitValue = 0;

#ifdef HiGHSDEV
  // Analysis of rebuilds
  const bool anRebuildTime = false;
  int totalRebuilds;
  double totalRebuildTime;
#endif

  // Devex scalars
  int n_dvx_fwk;
  int n_dvx_it;
  bool nw_dvx_fwk;
  // Devex vector
  vector<int> dvx_ix;

  // Price scalars
  bool alw_price_by_col_sw = true; //By default allow switch to column PRICE if results sufficiently dense
  bool alw_price_by_row_sw = true; //By default allow switch to standard row-wise PRICE if result is sufficiently dense
  bool alw_price_ultra = false;    //By default don't allow ultra-sparse PRICE
  const double dstyColPriceSw = 0.75;

  // DSE scalars
  bool iz_DSE_wt;
  bool alw_DSE2Dvx_sw = true;
  int n_wg_DSE_wt;
  int AnIterNumCostlyDseIt;
  int AnIterPrevRpNumCostlyDseIt;
  double AnIterCostlyDseFq;
  const double AnIterCostlyDseMeasureLimit = 1000.0;
  const double AnIterCostlyDseMnDensity = 0.01;
  const double AnIterFracNumTot_ItBfSw = 0.1;
  const double AnIterFracNumCostlyDseItbfSw = 0.05;
  double AnIterCostlyDseMeasure;

  // Model
  HModel *model;
  double Tp; // Tolerance for primal
  double Td; // Tolerance for dual

  int numCol;
  int numRow;
  int numTot;
  const HMatrix *matrix;
  const HFactor *factor;

  const int *jMove;
  const double *workRange;
  const double *baseLower;
  const double *baseUpper;
  double *baseValue;
  double *workDual;
  //    JAJH: Only because I can't get these from HModel.h
  double *workValue;
  double *colLower;
  double *colUpper;
  double *rowLower;
  double *rowUpper;
  int *nonbasicFlag;

  vector<double> bs_cond_x;
  vector<double> bs_cond_y;
  vector<double> bs_cond_z;
  vector<double> bs_cond_w;

  int solvePhase;
  int invertHint;

  HVector row_ep;
  HVector row_ap;
  HVector column;
  HVector columnBFRT;
  HVector columnDSE;
  double columnDensity;
  double row_epDensity;
  double row_apDensity;
  double rowdseDensity;
  double columnAvLog10Density;
  double row_epAvLog10Density;
  double row_apAvLog10Density;
  double rowdseAvLog10Density;
  double columnAvDensity;
  double row_epAvDensity;
  double row_apAvDensity;
  double rowdseAvDensity;

  HDualRow dualRow;

  // Solving related buffers
  int dualInfeasCount;

  HDualRHS dualRHS;

  // Simplex pivotal information
  int rowOut;
  int columnOut;
  int sourceOut; // -1 from small to lower, +1 to upper
  int columnIn;
  double deltaPrimal;
  double thetaDual;
  double thetaPrimal;
  double alpha;
  double alphaRow;
  double numericalTrouble;

  // Iteration counts
  int n_ph1_du_it;
  int n_ph2_du_it;
  int n_pr_it;
  // Partitioned coefficient matrix
  int slice_num;
  int slice_PRICE;
  int slice_start[HSOL_SLICED_LIMIT + 1];
  HMatrix slice_matrix[HSOL_SLICED_LIMIT];
  HVector slice_row_ap[HSOL_SLICED_LIMIT];
  HDualRow slice_dualRow[HSOL_SLICED_LIMIT];

  // Multiple price data
  struct MChoice
  {
    int rowOut;
    double baseValue;
    double baseLower;
    double baseUpper;
    double infeasValue;
    double infeasEdWt;
    double infeasLimit;
    HVector row_ep;
    HVector column;
    HVector columnBFRT;
  };

  struct MFinish
  {
    int moveIn;
    double shiftOut;
    vector<int> flipList;

    int rowOut;
    int columnOut;
    int columnIn;
    double alphaRow;
    double thetaPrimal;
    double basicBound;
    double basicValue;
    double EdWt;
    HVector_ptr row_ep;
    HVector_ptr column;
    HVector_ptr columnBFRT;
  };

  int multi_num;
  int multi_iChoice;
  int multi_nFinish;
  int multi_iteration;
  int multi_chooseAgain;
  MChoice multi_choice[HSOL_THREAD_LIMIT];
  MFinish multi_finish[HSOL_THREAD_LIMIT];

  double total_fake;
  double total_INVERT_TICK;
  double total_FT_inc_TICK;

  int AnIterIt0;
#ifdef HiGHSDEV
  const bool AnIterLg = true;
  int AnIterPrevIt;
  //Major operation analysis struct
  enum AnIterOpTy
  {
    AnIterOpTy_Btran = 0,
    AnIterOpTy_Price,
    AnIterOpTy_Ftran,
    AnIterOpTy_FtranBFRT,
    AnIterOpTy_FtranDSE,
    NumAnIterOpTy,
  };

  struct AnIterOpRec
  {
    double AnIterOpLog10RsDsty;
    double AnIterOpSuLog10RsDsty;
    double AnIterOpHyperCANCEL;
    double AnIterOpHyperTRAN;
    int AnIterOpRsDim;
    int AnIterOpNumCa;
    int AnIterOpNumHyperOp;
    int AnIterOpNumHyperRs;
    int AnIterOpRsMxNNZ;
    int AnIterOpSuNumCa;
    int AnIterOpSuNumHyperOp;
    int AnIterOpSuNumHyperRs;
    string AnIterOpName;
  };
  AnIterOpRec AnIterOp[NumAnIterOpTy];

  struct AnIterTraceRec
  {
    double AnIterTraceTime;
    double AnIterTraceDsty[NumAnIterOpTy];
    double AnIterTraceAux0;
    int AnIterTraceIter;
    int AnIterTraceEdWt_Mode;
  };

  const int AnIterTraceMxNumRec = 20;
  int AnIterTraceNumRec;
  int AnIterTraceIterDl;
  AnIterTraceRec AnIterTrace[22]; //How can this be 1+AnIterTraceMxNumRec+1;

  const int AnIterNumInvertHint = 7;
  int AnIterNumInvert[8]; //How can this be AnIterNumInvertHint+1
  int AnIterNumColPrice;
  int AnIterNumRowPrice;
  int AnIterNumRowPriceWSw;
  int AnIterNumRowPriceUltra;
  int AnIterNumPrDgnIt;
  int AnIterNumDuDgnIt;
  int AnIterNumEdWtIt[3]; //How can this be EdWt_Mode_Dan+1
#endif
};

#endif /* HDUAL_H_ */
