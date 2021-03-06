#ifndef __NESTEROV_BASE__
#define __NESTEROV_BASE__

#include <vector>
#include <memory>
#include <unordered_map>

#include "point.h"

namespace replace {

class Instance;
class Die;
class PlacerBase;

class Instance;
class Pin;
class Net;

class GPin;
class FFT;

class Logger;

class GCell {
public:
  GCell();

  // instance cells
  GCell(Instance* inst);
  GCell(std::vector<Instance*>& insts);

  // filler cells
  GCell(int cx, int cy, int dx, int dy);
  ~GCell();

  Instance* instance() const;
  const std::vector<Instance*> & insts() const { return insts_; }
  const std::vector<GPin*> & gPins() const { return gPins_; }

  void addGPin(GPin* gPin);

  void setClusteredInstance(std::vector<Instance*>& insts);
  void setInstance(Instance* inst);
  void setFiller();
  void setMacroInstance();
  void setStdInstance();

  // normal coordinates
  int lx() const;
  int ly() const;
  int ux() const;
  int uy() const;
  int cx() const;
  int cy() const;
  int dx() const;
  int dy() const;

  // virtual density coordinates
  int dLx() const;
  int dLy() const;
  int dUx() const;
  int dUy() const;
  int dCx() const;
  int dCy() const;
  int dDx() const;
  int dDy() const;


  void setLocation(int lx, int ly);
  void setCenterLocation(int cx, int cy);
  void setSize(int dx, int dy);

  void setDensityLocation(int dLx, int dLy);
  void setDensityCenterLocation(int dCx, int dCy);
  void setDensitySize(int dDx, int dDy);

  void setDensityScale(float densityScale);
  void setGradientX(float gradX);
  void setGradientY(float gradY);

  float gradientX() const { return gradientX_; }
  float gradientY() const { return gradientY_; }
  float densityScale() const { return densityScale_; }

  bool isInstance() const;
  bool isClusteredInstance() const;
  bool isFiller() const;
  bool isMacroInstance() const;
  bool isStdInstance() const;


private:
  std::vector<Instance*> insts_;
  std::vector<GPin*> gPins_;
  int lx_;
  int ly_;
  int ux_;
  int uy_;

  int dLx_;
  int dLy_;
  int dUx_;
  int dUy_;

  float densityScale_;
  float gradientX_;
  float gradientY_;

  // need to be stored for
  // MS replace
  bool isMacroInstance_:1;
};

inline int
GCell::lx() const {
  return lx_;
}
inline int
GCell::ly() const {
  return ly_; 
}

inline int
GCell::ux() const { 
  return ux_; 
}

inline int
GCell::uy() const {
  return uy_; 
}

inline int
GCell::cx() const {
  return (lx_ + ux_)/2;
}

inline int
GCell::cy() const {
  return (ly_ + uy_)/2;
}

inline int 
GCell::dx() const {
  return ux_ - lx_; 
}

inline int
GCell::dy() const {
  return uy_ - ly_;
}

inline int
GCell::dLx() const {
  return dLx_;
}

inline int
GCell::dLy() const {
  return dLy_;
}

inline int
GCell::dUx() const {
  return dUx_;
}

inline int
GCell::dUy() const {
  return dUy_;
}

inline int
GCell::dCx() const {
  return (dUx_ + dLx_)/2;
}

inline int
GCell::dCy() const {
  return (dUy_ + dLy_)/2;
}

inline int
GCell::dDx() const {
  return dUx_ - dLx_; 
}

inline int
GCell::dDy() const {
  return dUy_ - dLy_;
}

class GNet {
  public:
    GNet();
    GNet(Net* net);
    GNet(std::vector<Net*>& nets);
    ~GNet();

    Net* net() const;
    const std::vector<Net*> & nets() const { return nets_; }
    const std::vector<GPin*> & gPins() const { return gPins_; }

    int lx() const;
    int ly() const;
    int ux() const;
    int uy() const;

    void setCustomWeight( float customWeight );
    float customWeight() const { return customWeight_; }
    float netWeight() const { return weight_; }

    void addGPin(GPin* gPin);
    void updateBox();
    int64_t hpwl();

    void setDontCare();
    bool isDontCare();

    // clear WA(Weighted Average) variables.
    void clearWaVars();

    void addWaExpMinSumX(float waExpMinSumX);
    void addWaXExpMinSumX(float waExpXMinSumX);

    void addWaExpMinSumY(float waExpMinSumY);
    void addWaYExpMinSumY(float waExpXMinSumY);

    void addWaExpMaxSumX(float waExpMaxSumX);
    void addWaXExpMaxSumX(float waExpXMaxSumX);

    void addWaExpMaxSumY(float waExpMaxSumY);
    void addWaYExpMaxSumY(float waExpXMaxSumY);

    float waExpMinSumX() const; 
    float waXExpMinSumX() const;

    float waExpMinSumY() const;
    float waYExpMinSumY() const;

    float waExpMaxSumX() const;
    float waXExpMaxSumX() const;

    float waExpMaxSumY() const;
    float waYExpMaxSumY() const;


  private:
    std::vector<GPin*> gPins_;
    std::vector<Net*> nets_;
    int lx_;
    int ly_;
    int ux_;
    int uy_;

    float customWeight_;
    float weight_;

    //
    // weighted average WL model stor for better indexing
    // Please check the equation (4) in the ePlace-MS paper.
    //
    // WA: weighted Average
    // saving four variable will be helpful for
    // calculating the WA gradients/wirelengths.
    //
    // gamma: modeling accuracy.
    //
    // X forces.
    //
    // waExpMinSumX_: store sigma {exp(x_i/gamma)}
    // waXExpMinSumX_: store signa {x_i*exp(e_i/gamma)}
    // waExpMaxSumX_ : store sigma {exp(-x_i/gamma)}
    // waXExpMaxSumX_: store sigma {x_i*exp(-x_i/gamma)}
    //
    float waExpMinSumX_;
    float waXExpMinSumX_;

    float waExpMaxSumX_;
    float waXExpMaxSumX_;

    //
    // Y forces.
    //
    // waExpMinSumY_: store sigma {exp(y_i/gamma)}
    // waYExpMinSumY_: store signa {y_i*exp(e_i/gamma)}
    // waExpMaxSumY_ : store sigma {exp(-y_i/gamma)}
    // waYExpMaxSumY_: store sigma {y_i*exp(-y_i/gamma)}
    //
    float waExpMinSumY_;
    float waYExpMinSumY_;

    float waExpMaxSumY_;
    float waYExpMaxSumY_;

    unsigned char isDontCare_:1;
};

inline int 
GNet::lx() const {
  return lx_; 
}

inline int
GNet::ly() const {
  return ly_;
}

inline int
GNet::ux() const {
  return ux_;
}

inline int
GNet::uy() const {
  return uy_;
}

// eight add functions
inline void
GNet::addWaExpMinSumX(float waExpMinSumX) {
  waExpMinSumX_ += waExpMinSumX;
}

inline void
GNet::addWaXExpMinSumX(float waXExpMinSumX) {
  waXExpMinSumX_ += waXExpMinSumX;
}

inline void
GNet::addWaExpMinSumY(float waExpMinSumY) {
  waExpMinSumY_ += waExpMinSumY;
}

inline void
GNet::addWaYExpMinSumY(float waYExpMinSumY) {
  waYExpMinSumY_ += waYExpMinSumY;
}

inline void
GNet::addWaExpMaxSumX(float waExpMaxSumX) {
  waExpMaxSumX_ += waExpMaxSumX;
}

inline void
GNet::addWaXExpMaxSumX(float waXExpMaxSumX) {
  waXExpMaxSumX_ += waXExpMaxSumX;
}

inline void
GNet::addWaExpMaxSumY(float waExpMaxSumY) {
  waExpMaxSumY_ += waExpMaxSumY;
}

inline void
GNet::addWaYExpMaxSumY(float waYExpMaxSumY) {
  waYExpMaxSumY_ += waYExpMaxSumY;
}

inline float
GNet::waExpMinSumX() const {
  return waExpMinSumX_;
}

inline float
GNet::waXExpMinSumX() const {
  return waXExpMinSumX_;
}

inline float
GNet::waExpMinSumY() const {
  return waExpMinSumY_;
}

inline float
GNet::waYExpMinSumY() const {
  return waYExpMinSumY_;
}

inline float
GNet::waExpMaxSumX() const {
  return waExpMaxSumX_;
}

inline float
GNet::waXExpMaxSumX() const {
  return waXExpMaxSumX_; 
}

inline float
GNet::waExpMaxSumY() const {
  return waExpMaxSumY_; 
}

inline float
GNet::waYExpMaxSumY() const {
  return waYExpMaxSumY_;
}


class GPin {
  public:
    GPin();
    GPin(Pin* pin);
    GPin(std::vector<Pin*>& pins);
    ~GPin();

    Pin* pin() const;
    const std::vector<Pin*> & pins() const { return pins_; }

    GCell* gCell() const { return gCell_; }
    GNet* gNet() const { return gNet_; }

    void setGCell(GCell* gCell);
    void setGNet(GNet* gNet);

    int cx() const { return cx_; }
    int cy() const { return cy_; }
    
    // clear WA(Weighted Average) variables.
    void clearWaVars();

    void setMaxExpSumX(float maxExpSumX);
    void setMaxExpSumY(float maxExpSumY);
    void setMinExpSumX(float minExpSumX);
    void setMinExpSumY(float minExpSumY);

    float maxExpSumX() const { return maxExpSumX_; }
    float maxExpSumY() const { return maxExpSumY_; }
    float minExpSumX() const { return minExpSumX_; }
    float minExpSumY() const { return minExpSumY_; }

    bool hasMaxExpSumX() const { return (hasMaxExpSumX_ == 1); }
    bool hasMaxExpSumY() const { return (hasMaxExpSumY_ == 1); }
    bool hasMinExpSumX() const { return (hasMinExpSumX_ == 1); }
    bool hasMinExpSumY() const { return (hasMinExpSumY_ == 1); }

    void setCenterLocation(int cx, int cy);
    void updateLocation(const GCell* gCell);
    void updateDensityLocation(const GCell* gCell);

  private:
    GCell* gCell_;
    GNet* gNet_;
    std::vector<Pin*> pins_;

    int offsetCx_;
    int offsetCy_;
    int cx_;
    int cy_;

    // weighted average WL vals stor for better indexing
    // Please check the equation (4) in the ePlace-MS paper.
    //
    // maxExpSum_: holds exp(x_i/gamma)
    // minExpSum_: holds exp(-x_i/gamma)
    // the x_i is equal to cx_ variable.
    //
    float maxExpSumX_;
    float maxExpSumY_;

    float minExpSumX_;
    float minExpSumY_;

    // flag variables
    //
    // check whether
    // this pin is considered in a WA models.
    unsigned char hasMaxExpSumX_:1;
    unsigned char hasMaxExpSumY_:1;

    unsigned char hasMinExpSumX_:1;
    unsigned char hasMinExpSumY_:1;
};

class Bin {
public:
  Bin();
  Bin(int x, int y, int lx, int ly, int ux, int uy, float targetDensity);

  ~Bin();

  int x() const;
  int y() const;

  int lx() const;
  int ly() const;
  int ux() const;
  int uy() const;
  int cx() const;
  int cy() const;
  int dx() const;
  int dy() const;

  float electroPhi() const;
  float electroForceX() const;
  float electroForceY() const;
  float targetDensity() const;
  float density() const;

  void setDensity(float density);
  void setTargetDensity(float density);
  void setElectroForce(float electroForceX, float electroForceY);
  void setElectroPhi(float phi);

  void setNonPlaceArea(int64_t area);
  void setInstPlacedArea(int64_t area);
  void setFillerArea(int64_t area);

  void addNonPlaceArea(int64_t area);
  void addInstPlacedArea(int64_t area);
  void addFillerArea(int64_t area);

  const int64_t binArea() const;
  const int64_t nonPlaceArea() const { return nonPlaceArea_; }
  const int64_t instPlacedArea() const { return instPlacedArea_; }
  const int64_t fillerArea() const { return fillerArea_; }

private:
  // index
  int x_;
  int y_;

  // coordinate
  int lx_;
  int ly_;
  int ux_;
  int uy_;

  int64_t nonPlaceArea_;
  int64_t instPlacedArea_;
  int64_t fillerArea_;

  float density_;
  float targetDensity_;  // will enable bin-wise density screening
  float electroPhi_;
  float electroForceX_;
  float electroForceY_;
};

inline int
Bin::x() const {
  return x_;
}

inline int
Bin::y() const { 
  return y_;
}

inline int 
Bin::lx() const { 
  return lx_; 
}

inline int
Bin::ly() const { 
  return ly_;
}

inline int
Bin::ux() const { 
  return ux_;
}

inline int
Bin::uy() const { 
  return uy_;
}

inline int
Bin::cx() const { 
  return (ux_ + lx_)/2;
}

inline int
Bin::cy() const { 
  return (uy_ + ly_)/2;
}

inline int
Bin::dx() const { 
  return (ux_ - lx_);
} 

inline int
Bin::dy() const { 
  return (uy_ - ly_);
}

inline void
Bin::setNonPlaceArea(int64_t area) {
  nonPlaceArea_ = area;
}

inline void
Bin::setInstPlacedArea(int64_t area) {
  instPlacedArea_ = area;
}

inline void
Bin::setFillerArea(int64_t area) {
  fillerArea_ = area;
}

inline void
Bin::addNonPlaceArea(int64_t area) {
  nonPlaceArea_ += area;
}

inline void
Bin::addInstPlacedArea(int64_t area) {
  instPlacedArea_ += area;
}

inline void
Bin::addFillerArea(int64_t area) {
  fillerArea_ += area;
}

//
// The bin can be non-uniform because of
// "integer" coordinates
//
class BinGrid {
public:
  BinGrid();
  BinGrid(Die* die);
  ~BinGrid();

  void setPlacerBase(std::shared_ptr<PlacerBase> pb);
  void setLogger(std::shared_ptr<Logger> log);
  void setCoordi(Die* die);
  void setBinCnt(int binCntX, int binCntY);
  void setBinCntX(int binCntX);
  void setBinCntY(int binCntY);
  void setTargetDensity(float density);
  void updateBinsGCellDensityArea(std::vector<GCell*>& cells);


  void initBins();

  // lx, ly, ux, uy will hold coreArea
  int lx() const;
  int ly() const;
  int ux() const;
  int uy() const;
  int cx() const;
  int cy() const;
  int dx() const;
  int dy() const;

  int binCntX() const;
  int binCntY() const;
  int binSizeX() const;
  int binSizeY() const;

  int64_t overflowArea() const;

  // return bins_ index with given gcell
  std::pair<int, int> getDensityMinMaxIdxX(GCell* gcell);
  std::pair<int, int> getDensityMinMaxIdxY(GCell* gcell);

  std::pair<int, int> getMinMaxIdxX(Instance* inst);
  std::pair<int, int> getMinMaxIdxY(Instance* inst);

  const std::vector<Bin*> & bins() const;

private:
  std::vector<Bin> binStor_;
  std::vector<Bin*> bins_;
  std::shared_ptr<PlacerBase> pb_;
  std::shared_ptr<Logger> log_;
  int lx_;
  int ly_;
  int ux_;
  int uy_;
  int binCntX_;
  int binCntY_;
  int binSizeX_;
  int binSizeY_;
  float targetDensity_;
  int64_t overflowArea_;
  unsigned char isSetBinCntX_:1;
  unsigned char isSetBinCntY_:1;

  void updateBinsNonPlaceArea();
};

inline const std::vector<Bin*> & BinGrid::bins() const {
  return bins_; 
}

class NesterovBaseVars {
public:
  float targetDensity;
  float minAvgCut;
  float maxAvgCut;
  int binCntX;
  int binCntY;
  float minWireLengthForceBar;
  unsigned char isSetBinCntX:1;
  unsigned char isSetBinCntY:1;

  NesterovBaseVars();
  void reset();
};

class NesterovBase {
public:
  NesterovBase();
  NesterovBase(NesterovBaseVars nbVars, std::shared_ptr<PlacerBase> pb,
      std::shared_ptr<Logger> log);
  ~NesterovBase();

  const std::vector<GCell*> & gCells() const { return gCells_; }
  const std::vector<GCell*> & gCellInsts() const { return gCellInsts_; }
  const std::vector<GCell*> & gCellFillers() const { return gCellFillers_; }

  const std::vector<GNet*> & gNets() const { return gNets_; }
  const std::vector<GPin*> & gPins() const { return gPins_; }

  //
  // placerBase To NesterovBase functions
  //
  GCell* placerToNesterov(Instance* inst);
  GPin* placerToNesterov(Pin* pin);
  GNet* placerToNesterov(Net* net);

  // update gCells with lx, ly
  void updateGCellLocation(
      std::vector<FloatPoint>& points);

  // update gCells with cx, cy
  void updateGCellCenterLocation(
      std::vector<FloatPoint>& points);

  void updateGCellDensityCenterLocation(
      std::vector<FloatPoint>& points);

  int binCntX() const;
  int binCntY() const;
  int binSizeX() const;
  int binSizeY() const;
  
  const std::vector<Bin*> & bins() const; 

  int64_t overflowArea() const;
  float sumPhi() const;

  void updateDensityCoordiLayoutInside(GCell* gcell);

  float getDensityCoordiLayoutInsideX(GCell* gCell, float cx);
  float getDensityCoordiLayoutInsideY(GCell* gCell, float cy);

  // WL force update based on WeightedAverage model
  // wlCoeffX : WireLengthCoefficient for X.
  //            equal to 1 / gamma_x
  // wlCoeffY : WireLengthCoefficient for Y.
  //            equal to 1 / gamma_y
  //
  // Gamma is described in the ePlaceMS paper.
  //
  void updateWireLengthForceWA(
      float wlCoeffX,
      float wlCoeffY);

  FloatPoint
    getWireLengthGradientPinWA(GPin* gPin,
        float wlCoeffX, float wlCoeffY);

  FloatPoint
    getWireLengthGradientWA(GCell* gCell,
        float wlCoeffX, float wlCoeffY);

  // for preconditioner
  FloatPoint
    getWireLengthPreconditioner(GCell* gCell);

  FloatPoint
    getDensityPreconditioner(GCell* gCell);

  FloatPoint
    getDensityGradient(GCell* gCell);

  int64_t getHpwl();

  // update electrostatic forces within Bin
  void updateDensityForceBin();

private:
  NesterovBaseVars nbVars_;
  std::shared_ptr<PlacerBase> pb_;
  std::shared_ptr<Logger> log_;

  BinGrid bg_;
  std::unique_ptr<FFT> fft_;

  std::vector<GCell> gCellStor_;
  std::vector<GNet> gNetStor_;
  std::vector<GPin> gPinStor_;

  std::vector<GCell*> gCells_;
  std::vector<GCell*> gCellInsts_;
  std::vector<GCell*> gCellFillers_;

  std::vector<GNet*> gNets_;
  std::vector<GPin*> gPins_;

  std::unordered_map<Instance*, GCell*> gCellMap_;
  std::unordered_map<Pin*, GPin*> gPinMap_;
  std::unordered_map<Net*, GNet*> gNetMap_;

  float sumPhi_;

  void init();
  void initFillerGCells();
  void initBinGrid();

  void reset();
};

inline const std::vector<Bin*> & NesterovBase::bins() const {
  return bg_.bins();
}

}

#endif
