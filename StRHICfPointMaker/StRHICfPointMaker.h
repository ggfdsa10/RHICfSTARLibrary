/***************************************************************************
 * Author: Seunghwan Lee
 ***************************************************************************
 *
 * Description: This is the Point maker for the RHICf data. 
 *
 ***************************************************************************
 * $Id: StRHICfPointMaker.h,v 1.0 2022/08/25 2:51:00 SeunghwanLee Exp $
 * $Log: StRHICfPointMaker.h,v $
 ***************************************************************************/

#ifndef StRHICfPointMaker_H
#define StRHICfPointMaker_H

#include <string>

#include "StMaker.h"
#include "StRoot/StRHICfUtil/StRHICfFunction.h"
#include "StRoot/StRHICfUtil/StRHICfPID.h"
#include "StRoot/StRHICfUtil/StRHICfRecoPos.h"
#include "StRoot/StRHICfUtil/StRHICfRecoEnergy.h"

class StEnumerations;
class StRHICfDbMaker;
class StRHICfCollection;
class StRHICfHit;
class StRHICfPoint;
class StMuRHICfUtil;

class StRHICfPointMaker : public StMaker, StRHICfFunction
{
  public: 
    StRHICfPointMaker(const Char_t* name="RHICfPoint");
    ~StRHICfPointMaker();

    int InitRun(int runNumber);
    int Make();
    int Finish();

    void saveDetailInfo(bool isSave){mIsSaveDetailInfo = isSave;} // RHICf detailed Hit information on-off

    void setMCReco(){mMCRecoFlag = true;}
    void setMCCollection(StRHICfCollection* rhicfcoll){mRHICfCollection = rhicfcoll;}

  private:
    int init();
    int recoPID();
    int recoHitPosition();
    int recoEnergy();

    int fillRHICfCollection();

    bool mMCRecoFlag;
    Int_t mEventNumber;

    StRHICfDbMaker* mRHICfDbMaker = 0; 
    StMuRHICfUtil* mMuRHICfUtil = 0;
    StRHICfCollection* mRHICfCollection = 0; 
    StRHICfHit* mLocalRHICfHitColl = 0; 
    StRHICfHit* mGlobalRHICfHitColl = 0; 
    std::vector<StRHICfPoint*> mRHICfPointColl;

    // Reconstrcution tool
    StRHICfPID* mRecoPID = 0;
    StRHICfRecoPos* mRecoPos = 0;
    StRHICfRecoEnergy* mRecoEnergy = 0;
    
    int mPID[kRHICfNtower];
    int mResultHitNum[kRHICfNtower];
    int mGSOMaxLayer[kRHICfNtower][kRHICfNorder];
    bool mWorthy[kRHICfNtower];
    bool mOverlap[kRHICfNtower][kRHICfNxy];
    bool mIsSaveDetailInfo; 

  ClassDef(StRHICfPointMaker,0);
};

#endif
