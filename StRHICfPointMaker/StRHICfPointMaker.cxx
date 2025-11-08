/***************************************************************************
 * Author: Seunghwan Lee
 ***************************************************************************
 *
 * Description: This is the Point maker for the RHICf data. 
 *
 ***************************************************************************
 * $Id: StRHICfPointMaker.cxx,v 1.0 2022/08/25 2:51:00 SeunghwanLee Exp $
 * $Log: StRHICfPointMaker.cxx,v $
 ***************************************************************************/

#include "StRHICfPointMaker.h"

#include "StRoot/StEvent/StEventTypes.h"
#include "St_base/StMessMgr.h"
#include "St_base/Stypes.h"

#include "StEvent/StEvent.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuRHICfCollection.h"
#include "StMuDSTMaker/COMMON/StMuRHICfRawHit.h"
#include "StMuDSTMaker/COMMON/StMuRHICfHit.h"
#include "StMuDSTMaker/COMMON/StMuRHICfUtil.h"

#include "StRoot/StRHICfDbMaker/StRHICfDbMaker.h"
#include "StRoot/StEvent/StRHICfCollection.h"
#include "StRoot/StEvent/StRHICfHit.h"
#include "StRoot/StEvent/StRHICfPoint.h"

StRHICfPointMaker::StRHICfPointMaker(const Char_t* name) : StMaker(name), mMCRecoFlag(false), mEventNumber(-1)
{
  mMuRHICfUtil = new StMuRHICfUtil();
  mRecoPID = new StRHICfPID();
  mRecoPos = new StRHICfRecoPos();
  mRecoEnergy = new StRHICfRecoEnergy();
  mLocalRHICfHitColl = new StRHICfHit();
  mLocalRHICfHitColl -> initDataArray(); 
  mIsSaveDetailInfo = true;
}

StRHICfPointMaker::~StRHICfPointMaker()
{
}

int StRHICfPointMaker::InitRun(int runNumber)
{
  if(mMCRecoFlag && runNumber == -1){return kStOk;}
  LOG_INFO << "StRHICfPointMaker::InitRun with run = "  << runNumber << endm;

  mRHICfDbMaker = static_cast<StRHICfDbMaker*>(GetMaker("rhicfDb")); 
  if(!mRHICfDbMaker){
    LOG_ERROR  << "StRHICfPointMaker::InitRun Failed to get StRHICfDbMaker" << endm;
    return kStFatal;
  }

  for(int it=0; it<kRHICfNtower; it++){
    for(int il=0; il<kRHICfNlayer; il++){
      for(int ixy=0; ixy<kRHICfNxy; ixy++){
        for(int ich=0; ich<checkGSOBarSize(it); ich++){
          float posTable = mRHICfDbMaker -> getBarPos(it, il, ixy, ich);
          mRecoPos -> setGSOBarTable(it, il, ixy, ich, posTable);
        }
      }
    }

    mRecoEnergy -> setLeakageOutTableNeutron(it, mRHICfDbMaker -> getLeakOutNeutron(it));
    for(int ip=0; ip<kRHICfNplate; ip++){
      mRecoEnergy -> setLeakageInTable(it, ip, mRHICfDbMaker -> getLeakInPhoton(it, ip));
      mRecoEnergy -> setLeakageOutTablePhoton(it, ip, mRHICfDbMaker -> getLeakOutPhoton(it, ip));
    }
  }

  return kStOK;
}

int StRHICfPointMaker::Make()
{
  StEvent* eventPtr = (StEvent*)GetDataSet("StEvent");
  StMuDst* muDst = (StMuDst*)GetDataSet("MuDst");

  // first try to get collection from StEvent.
  if(eventPtr && !muDst->event()){
    LOG_INFO <<"StRHICfPointMaker::Make() -- Found StEvent" <<endm;
    mRHICfCollection = eventPtr -> rhicfCollection();
    if(!mRHICfCollection){return kStFatal;}
    if(mIsSaveDetailInfo){mRHICfCollection->hitCollection()->initDataArray();}
  }

  // 2nd try to get collection from StMuDst
  if(muDst && muDst->event()){
    LOG_INFO <<"StRHICfPointMaker::Make() -- Found StMuDst" <<endm;
    mRHICfCollection = muDst -> rhicfCollection();
    if(!mRHICfCollection){return kStFatal;}
  }

  // new collection for MC
  if(mMCRecoFlag && !muDst && !eventPtr){
    if(mEventNumber == GetNumber()){return kStOk;}
    LOG_INFO <<"StRHICfPointMaker::Make() -- Make RHICf collection for MC" <<endm;

    if(!mRHICfCollection){
      LOG_ERROR << "StRHICfPointMaker::Make() -- Can not found the StRHICfCollection input" << endm;
      return kStFatal;
    }
  }
  mGlobalRHICfHitColl = mRHICfCollection -> hitCollection();

  init();

  // main reconstruction
  recoPID();
  recoHitPosition();
  if(mWorthy[0] || mWorthy[1]){recoEnergy();}
  
  // Fill the MuDst
  if(muDst && muDst->event()){
    StMuRHICfHit* muRHICfHit = muDst -> muRHICfCollection() -> addHit();
    if(mIsSaveDetailInfo){muRHICfHit->initDataArray();}
    mMuRHICfUtil->fillMuRHICfHit(muRHICfHit, mLocalRHICfHitColl);

    for(unsigned int i=0; i<mRHICfPointColl.size(); i++){
      StMuRHICfPoint* muRHICfPoint = muDst -> muRHICfCollection() -> addPoint();
      mMuRHICfUtil->fillMuRHICfPoint(muRHICfPoint, mRHICfPointColl[i]);
    }
  }
  if(mMCRecoFlag && !muDst){
    mEventNumber = GetNumber();
    fillRHICfCollection();
  }

  return kStOK;
}

int StRHICfPointMaker::Finish()
{

  return kStOK;
}

int StRHICfPointMaker::init()
{
  mRecoPID -> init();
  mRecoPos -> init();
  mRecoEnergy -> init();
  mLocalRHICfHitColl -> clear(); 
  for(unsigned int i=0; i<mRHICfPointColl.size(); i++){
    delete mRHICfPointColl[i];
    mRHICfPointColl[i] = nullptr;
  }
  mRHICfPointColl.clear();

  return kStOk;
}

int StRHICfPointMaker::recoPID()
{
  for(int it=0; it<kRHICfNtower; it++){
    for(int ip=0; ip<kRHICfNplate; ip++){
      float plateEnergy =  mGlobalRHICfHitColl -> getPlateEnergy(it, ip);
      mRecoPID -> setPlateEnergy(it, ip, plateEnergy);
      mLocalRHICfHitColl -> setPlateEnergy(it, ip, plateEnergy);
    }
  }

  mRecoPID -> calculate();

  for(int it=0; it<kRHICfNtower; it++){
    float l20 = mRecoPID -> getL20(it);
    float l90 = mRecoPID -> getL90(it);
    int pid = mRecoPID -> getPID(it);

    mLocalRHICfHitColl -> setL20(it, l20);
    mLocalRHICfHitColl -> setL90(it, l90);
    mPID[it] = pid;
  }
  return kStOK;
}

int StRHICfPointMaker::recoHitPosition()
{
  // recoPosition setup
  for(int it=0; it<kRHICfNtower; it++){
    for(int il=0; il<kRHICfNlayer; il++){
      for(int ixy=0; ixy<kRHICfNxy; ixy++){
        for(int ich=0; ich<checkGSOBarSize(it); ich++){
          float gsoBarEnergy = mGlobalRHICfHitColl -> getGSOBarEnergy(it, il, ixy, ich);
          mRecoPos -> setGSOBarEnergy(it, il, ixy, ich, gsoBarEnergy);
          mLocalRHICfHitColl -> setGSOBarEnergy(it, il, ixy, ich, gsoBarEnergy);
        }
      }
    }
  }

  mRecoPos -> fillData();

  for(int it=0; it<kRHICfNtower; it++){
    mResultHitNum[it] = 0;
    mGSOMaxLayer[it][0] = mRecoPos -> getGSOMaxLayer(it, 0);
    mGSOMaxLayer[it][1] = mRecoPos -> getGSOMaxLayer(it, 1);
    mLocalRHICfHitColl -> setGSOMaxLayer(it, 0, mGSOMaxLayer[it][0]);
    mLocalRHICfHitColl -> setGSOMaxLayer(it, 1, mGSOMaxLayer[it][1]);
    mWorthy[it] = mRecoPos -> getWorthy(it);
  }

  if(!mWorthy[0] && !mWorthy[1]){
    for(int it=0; it<kRHICfNtower; it++){mResultHitNum[it] = 0;}
  }
  else{
    // Position reconstruction
    mRecoPos -> calculate();

    // Fill the result data
    for(int it=0; it<kRHICfNtower; it++){
      mResultHitNum[it] = mRecoPos -> getEvalHitNum(it);
      if(mResultHitNum[it] > 1){
        mRecoPos -> separateMultiFit(it);
        mLocalRHICfHitColl -> setMultiHitNum(it, 2);
      }

      for(int il=0; il<kRHICfNlayer; il++){
        for(int ixy=0; ixy<kRHICfNxy; ixy++){
          // For single hits
          int evalHitNum = mRecoPos -> getEvalHitNum(it, il, ixy);
          int maxPeakBin = mRecoPos -> getMaximumBin(it, il, ixy);
          float singlePos = mRecoPos -> getSinglePeakPos(it, il, ixy);
          float singlePeakHeight = mRecoPos -> getSinglePeakHeight(it, il, ixy);
          float singleChi2 = mRecoPos -> getSingleChi2(it, il, ixy);
          float multiChi2 = mRecoPos -> getMultiChi2(it, il, ixy);

          mLocalRHICfHitColl -> setSingleHitNum(it, il, ixy, evalHitNum);
          mLocalRHICfHitColl -> setMaxPeakBin(it, il, ixy, maxPeakBin);
          mLocalRHICfHitColl -> setSingleHitPos(it, il, ixy, singlePos);
          mLocalRHICfHitColl -> setSinglePeakHeight(it, il, ixy, singlePeakHeight);
          mLocalRHICfHitColl -> setSingleFitChi2(it, il, ixy, singleChi2);
          mLocalRHICfHitColl -> setMultiFitChi2(it, il, ixy, multiChi2);

          // For multi hits
          if(mResultHitNum[it] > 1){
            for(int io=0; io<2; io++){
              float multiPos = mRecoPos -> getMultiPeakPos(it, il, ixy, io);
              float multiPeakRaw = mRecoPos -> getMultiPeakRaw(it, il, ixy, io);
              float multiEvalPeak = mRecoPos -> getEvalPeakHeight(it, il, ixy, io);
              float multiEnergySum = mRecoPos -> getMultiEnergySum(it, il, ixy, io);

              mLocalRHICfHitColl -> setMultiHitPos(it, il, ixy, io, multiPos);
              mLocalRHICfHitColl -> setMultiPeakRaw(it, il, ixy, io, multiPeakRaw);
              mLocalRHICfHitColl -> setMultiPeakHeight(it, il, ixy, io, multiEvalPeak);
              mLocalRHICfHitColl -> setMultiEnergySum(it, il, ixy, io, multiEnergySum);
            }
          }
        }
      }
      mOverlap[it][0] =  mRecoPos -> getOverlap(it, 0);
      mOverlap[it][1] =  mRecoPos -> getOverlap(it, 1);
    }
  }
  return kStOk;
}

int StRHICfPointMaker::recoEnergy()
{   
  // recoEnergy setup
  mRecoEnergy -> setRunType(mRHICfCollection -> getRunType());

  for(int it=0; it<kRHICfNtower; it++){
    mRecoEnergy -> setResultHitNum(it, mResultHitNum[it]);

    for(int ip=0; ip<kRHICfNplate; ip++){
      float plateEnergy = mGlobalRHICfHitColl -> getPlateEnergy(it, ip);
      mRecoEnergy -> setPlateEnergy(it, ip, plateEnergy);
    }

    for(int ixy=0; ixy<kRHICfNxy; ixy++){
      float resultHitPos = mLocalRHICfHitColl -> getSingleHitPos(it, mGSOMaxLayer[it][0], ixy); 
      mRecoEnergy -> setResultHitPos(it, ixy, resultHitPos);
      mRecoEnergy -> setOverlap(it, ixy, mOverlap[it][ixy]);

      for(int io=0; io<2; io++){
        float multiHitPos1 = mLocalRHICfHitColl -> getMultiHitPos(it, mGSOMaxLayer[it][0], ixy, io);
        float multiHitPos2 = mLocalRHICfHitColl -> getMultiHitPos(it, mGSOMaxLayer[it][1], ixy, io);
        float multiPeak1 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][0], ixy, io);
        float multiPeak2 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][1], ixy, io);
        mRecoEnergy -> setMultiHitPos(it, 0, ixy, io, multiHitPos1);
        mRecoEnergy -> setMultiHitPos(it, 1, ixy, io, multiHitPos2);
        mRecoEnergy -> setMultiPeakHeight(it, 0, ixy, io, multiPeak1);
        mRecoEnergy -> setMultiPeakHeight(it, 1, ixy, io, multiPeak2);
      }
    }
  }

  // Energy reconstruction
  mRecoEnergy -> calculate();

  // Save the result
  for(int it=0; it<kRHICfNtower; it++){
    int resultHitNum = mRecoEnergy -> getResultHitNum(it);
    if(resultHitNum == 0) continue;

    float sumEnergyAll = mRecoEnergy -> getPlateSumEnergy(it, true);
    float sumEnergyPart = mRecoEnergy -> getPlateSumEnergy(it, false);

    // Type-1 
    if(resultHitNum == 1){
      float peakHeightX = mLocalRHICfHitColl -> getSinglePeakHeight(it, mGSOMaxLayer[it][0], 0);
      float peakHeightY = mLocalRHICfHitColl -> getSinglePeakHeight(it, mGSOMaxLayer[it][0], 1);
      if(peakHeightX < 0.035 || peakHeightY < 0.035){continue;}

      StRHICfPoint* pointColl = new StRHICfPoint();
      pointColl -> setTowerIdx(it);
      pointColl -> setPID(mPID[it]);
      pointColl -> setTowerSumEnergy(sumEnergyAll, sumEnergyPart);

      float posX = mLocalRHICfHitColl -> getSingleHitPos(it, mGSOMaxLayer[it][0], 0);
      float posY = mLocalRHICfHitColl -> getSingleHitPos(it, mGSOMaxLayer[it][0], 1);
      float resultPhotonEnergy = mRecoEnergy -> getResultEnergy(it, kRHICfPhoton);
      float resultHadronEnergy = mRecoEnergy -> getResultEnergy(it, kRHICfHadron);

      pointColl -> setPointPos(posX, posY);
      pointColl -> setPointEnergy(resultPhotonEnergy, resultHadronEnergy);
      mRHICfPointColl.push_back(pointColl);

    }
    else if(resultHitNum == 2){ // Type-2
      float peakHeightX1 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][0], 0, 0);
      float peakHeightY1 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][0], 1, 0);
      float peakHeightX2 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][0], 0, 1);
      float peakHeightY2 = mLocalRHICfHitColl -> getMultiPeakHeight(it, mGSOMaxLayer[it][0], 1, 1);
      if(peakHeightX1 < 0.035 || peakHeightY1 < 0.035 || peakHeightX2 < 0.035 || peakHeightY2 < 0.035){continue;}

      for(int points=0; points<resultHitNum; points++){
        StRHICfPoint* pointColl = new StRHICfPoint();
        pointColl -> setTowerIdx(it);
        pointColl -> setPID(mPID[it]);
        pointColl -> setTowerSumEnergy(sumEnergyAll, sumEnergyPart);

        float posX = mLocalRHICfHitColl -> getMultiHitPos(it, mGSOMaxLayer[it][0], 0, points);
        float posY = mLocalRHICfHitColl -> getMultiHitPos(it, mGSOMaxLayer[it][0], 1, points);
        pointColl -> setPointPos(posX, posY);
        float resultPhotonEnergy = mRecoEnergy -> getResultEnergy(it, kRHICfPhoton);
        float resultHadronEnergy = mRecoEnergy -> getResultEnergy(it, kRHICfHadron);

        pointColl -> setPointEnergy(resultPhotonEnergy * mRecoEnergy -> getEnergyRatio(it, points), resultHadronEnergy * mRecoEnergy -> getEnergyRatio(it, points));
        mRHICfPointColl.push_back(pointColl);
      }
    }
  }

  return kStOk;
}

int StRHICfPointMaker::fillRHICfCollection()
{
  for(int it=0; it<kRHICfNtower; it++){
    for(int io=0; io<2; io++){
      Int_t gsoMaxLayer = mLocalRHICfHitColl -> getGSOMaxLayer(it, io);
      mGlobalRHICfHitColl -> setGSOMaxLayer(it, io, gsoMaxLayer);
    }

    Float_t l20 = mLocalRHICfHitColl -> getL20(it);
    Float_t l90 = mLocalRHICfHitColl -> getL90(it);
    mGlobalRHICfHitColl -> setL20(it, l20);
    mGlobalRHICfHitColl -> setL90(it, l90);
      
    Int_t multiHitNum = mLocalRHICfHitColl -> getMultiHitNum(it);
    mGlobalRHICfHitColl -> setMultiHitNum(it, multiHitNum);
      
    for(int il=0; il<kRHICfNlayer; il++){
      for(int ixy=0; ixy<kRHICfNxy; ixy++){

        Int_t singleHitNum = mLocalRHICfHitColl -> getSingleHitNum(it, il, ixy);
        Int_t maxPeakBin = mLocalRHICfHitColl -> getMaxPeakBin(it, il, ixy);
        Float_t singleHitPos = mLocalRHICfHitColl -> getSingleHitPos(it, il, ixy);
        Float_t singlePeakHeight = mLocalRHICfHitColl -> getSinglePeakHeight(it, il, ixy);
        Float_t singleChi2 = mLocalRHICfHitColl -> getSingleFitChi2(it, il, ixy);
        Float_t multiChi2 = mLocalRHICfHitColl -> getMultiFitChi2(it, il, ixy);

        mGlobalRHICfHitColl -> setSingleHitNum(it, il, ixy, singleHitNum);
        mGlobalRHICfHitColl -> setMaxPeakBin(it, il, ixy, maxPeakBin);
        mGlobalRHICfHitColl -> setSingleHitPos(it, il, ixy, singleHitPos);
        mGlobalRHICfHitColl -> setSinglePeakHeight(it, il, ixy, singlePeakHeight);
        mGlobalRHICfHitColl -> setSingleFitChi2(it, il, ixy, singleChi2);
        mGlobalRHICfHitColl -> setMultiFitChi2(it, il, ixy, multiChi2);

        for(int io=0; io<2; io++){
          Float_t multiHitPos = mLocalRHICfHitColl -> getMultiHitPos(it, il, ixy, io);
          Float_t multiPeakRaw = mLocalRHICfHitColl -> getMultiPeakRaw(it, il, ixy, io);
          Float_t multiPeakHeight = mLocalRHICfHitColl -> getMultiPeakHeight(it, il, ixy, io);
          Float_t multiEnergySum = mLocalRHICfHitColl -> getMultiEnergySum(it, il, ixy, io);
            
          mGlobalRHICfHitColl -> setMultiHitPos(it, il, ixy, io, multiHitPos);
          mGlobalRHICfHitColl -> setMultiPeakRaw(it, il, ixy, io, multiPeakRaw);
          mGlobalRHICfHitColl -> setMultiPeakHeight(it, il, ixy, io, multiPeakHeight);
          mGlobalRHICfHitColl -> setMultiEnergySum(it, il, ixy, io, multiEnergySum);
        }
      }
    }
  }

  for(unsigned int i=0; i<mRHICfPointColl.size(); i++){
    Int_t towerIdx = mRHICfPointColl[i] -> getTowerIdx();
    Int_t pid = mRHICfPointColl[i] -> getPID();
    Float_t posX = mRHICfPointColl[i] -> getPointPos(0);
    Float_t posY = mRHICfPointColl[i] -> getPointPos(1);
    Float_t photonE = mRHICfPointColl[i] -> getPointEnergy(0);
    Float_t hadronE = mRHICfPointColl[i] -> getPointEnergy(1);
    Float_t towerEAll = mRHICfPointColl[i] -> getTowerSumEnergy(0);
    Float_t towerEPart = mRHICfPointColl[i] -> getTowerSumEnergy(1);

    StRHICfPoint* pointColl = new StRHICfPoint();
    pointColl -> setTowerIdx(towerIdx);
    pointColl -> setPID(pid);
    pointColl -> setPointPos(posX, posY);
    pointColl -> setPointEnergy(photonE, hadronE);
    pointColl -> setTowerSumEnergy(towerEAll, towerEPart);
    
    mRHICfCollection -> addPoint(pointColl);
  }

  return  kStOk;
}