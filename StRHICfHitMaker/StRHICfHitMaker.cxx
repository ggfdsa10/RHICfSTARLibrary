/***************************************************************************
 * Author: Akio Ogawa, Seunghwan Lee
 ***************************************************************************
 *
 * Description: This is the Hit maker for the RHICf data. 
 *
 ***************************************************************************
 * $Id: StRHICfHitMaker.cxx,v 1.4 2022/11/01 11:50:00 slee5 Exp $
 * $Log: StRHICfHitMaker.cxx,v $
 ***************************************************************************/

#include "StRHICfHitMaker.h"

#include "StRoot/StEvent/StEventTypes.h"
#include "St_base/StMessMgr.h"
#include "St_base/Stypes.h"
#include "StEvent/StEvent.h"

#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuRHICfCollection.h"
#include "StMuDSTMaker/COMMON/StMuRHICfUtil.h"

#include "StRoot/StRHICfDbMaker/StRHICfDbMaker.h"
#include "StRoot/StEvent/StRHICfCollection.h"
#include "StRoot/StEvent/StRHICfRawHit.h"
#include "StRoot/StEvent/StRHICfHit.h"
#include "StRoot/StEvent/StRHICfPoint.h"

StRHICfHitMaker::StRHICfHitMaker(const Char_t* name) : StMaker(name)
{
    mMuRHICfUtil = new StMuRHICfUtil();
}

StRHICfHitMaker::~StRHICfHitMaker()
{
}

int StRHICfHitMaker::InitRun(int runNumber)
{
    LOG_INFO << "StRHICfHitMaker::InitRun with run = "  << runNumber << endm;
    mRHICfDbMaker = static_cast<StRHICfDbMaker*>(GetMaker("rhicfDb"));
    if(!mRHICfDbMaker){
        LOG_ERROR  << "StRHICfHitMaker::InitRun Failed to get StRHICfDbMaker" << endm;
        return kStFatal;
    }

    checkRunTypeForRHICf2017(runNumber);
    genCrossTalkMatrix();

    // test
    // TRandom3* random = new TRandom3(0);
    // int idx = random -> Uniform(0, 1000000);
    // TString fileName = Form("%i_%i.txt", runNumber, idx);
    // txtFile.open(fileName.Data());

    return kStOk;
}

int StRHICfHitMaker::Make()
{
    StEvent* eventPtr = (StEvent*)GetDataSet("StEvent");
    StMuDst* muDst = (StMuDst*)GetDataSet("MuDst");

    // check the run type,
    // if runnumber is not tagged, this run doesn't worty for RHICf reconstruction and analysis
    // you can see the RHICf run type by run number list in StRHICfFunction.cxx
    if(getRunType() == 999){return kStFatal;}

    // first try to get collection from StEvent.
    if(eventPtr && !muDst->event()){
        LOG_INFO <<"StRHICfHitMaker::Make() -- Found StEvent" <<endm;
        mRHICfCollection = eventPtr -> rhicfCollection();
        if(!mRHICfCollection){return kStFatal;}
    }

    // 2nd try to get collection from StMuDst
    if(muDst && muDst->event()){
        LOG_INFO <<"StRHICfHitMaker::Make() -- Found StMuDst" <<endm;
        StMuRHICfCollection* muRHICfColl = muDst -> muRHICfCollection();
        if(!muRHICfColl){return kStFatal;}
        mRHICfCollection = mMuRHICfUtil -> getRHICf(muRHICfColl);
        
        muDst->setRHICfCollection(mRHICfCollection);
        mRHICfCollection -> setRunType(getRunType());
    }

    mRHICfCollection -> isAllSave();
    mRHICfRawHitColl = mRHICfCollection -> rawHitCollection();
    mRHICfHitColl = mRHICfCollection -> hitCollection();

    // test 
    // StMuRHICfCollection* muRHICfColl = muDst -> muRHICfCollection();
    // StMuRHICfRawHit* muRHICfRawHit = muRHICfColl->getRawHit();
    // txtFile << "RHICfRunNum  " << muRHICfRawHit -> getRHICfRunNumber() << " RHICfEventNum " << muRHICfRawHit -> getRHICfEventNumber() << endl;

    // Main calibration
    caliPlate();
    caliGSOBar();

    LOG_INFO <<"StRHICfHitMaker::Make() -- RHICf Hit calibration done" <<endm;
                    
    return kStOk;
}

int StRHICfHitMaker::Finish()
{
    return kStOk;
}

int StRHICfHitMaker::caliPlate()
{
    for(int it=0; it<kRHICfNtower; it++){
        for(int ip=0; ip<kRHICfNplate; ip++){
            float adc[2];
            adc[0] = mRHICfRawHitColl -> getPlateADC(it, ip, 0);
            adc[1] = mRHICfRawHitColl -> getPlateADC(it, ip, 1);

            // txtFile << "plate (" << it << " " << ip << ") " << adc[0] << " " << adc[1] << endl; // test

            if(adc[0] > mADCThreshold){
                float pactor0 = mRHICfDbMaker -> getPlateRangePar(it, ip, 0);
                float pactor1 = mRHICfDbMaker -> getPlateRangePar(it, ip, 1);
                adc[0] = pactor0 + pactor1*adc[1];
            }

            float pedestal = mRHICfDbMaker -> getPlatePedestal(it, ip);
            float plateGain = mRHICfDbMaker -> getPlateGain(it, ip);
            float plateEnergy = (adc[0] - pedestal)/plateGain;
            plateEnergy *= rescaleEnergyFactor(it, ip);
            mRHICfHitColl -> setPlateEnergy(it, ip, plateEnergy);
        }
    }
    return kStOk;
}

int StRHICfHitMaker::caliGSOBar()
{
    for(int it=0; it<kRHICfNtower; it++){
        for(int il=0; il<kRHICfNlayer; il++){
            for(int ixy=0; ixy<kRHICfNxy; ixy++){
                for(int ich=0; ich<checkGSOBarSize(it); ich++){
                    int adc = mRHICfRawHitColl -> getGSOBarADC(it, il, ixy, ich);
                    float pedeI = mRHICfDbMaker -> getBarPedestal(it, il, ixy, ich);
                    float pedeF = mRHICfDbMaker -> getBarPedestalF(it, il, ixy, ich);
                    float pedestal = pedeI + 0.000001*pedeF;

                    float gsobarGain = mRHICfDbMaker -> getBarGain(it, il, ixy, ich);
                    float gsobarEnergy = (adc - pedestal)/gsobarGain;

                    mRHICfHitColl -> setGSOBarEnergy(it, il, ixy, ich, gsobarEnergy);

                    // txtFile << "gsobar (" << it << " " << il << " " << ixy << " " << ich << ") " << adc << endl; // test
                }
            }
        }
    }
    caliCrossTalk();

    return kStOk;
}

int StRHICfHitMaker::genCrossTalkMatrix()
{
    mCrossTalkMatrix = new TMatrixD(60, 60);
    TArrayD crossTalkArray(3600);

    for(int i=0; i<60; i++){
        for(int j=0; j<60; j++){
            crossTalkArray[i*60+j] = mRHICfDbMaker -> getCrossTalk(i, j);
        }
    }
    mCrossTalkMatrix->SetMatrixArray(crossTalkArray.GetArray());

    return kStOk;
}

int StRHICfHitMaker::caliCrossTalk()
{
    double a4Vx[4][60];
    double a4Vy[4][60];
    for(int il=0; il<kRHICfNlayer; ++il){
        for(int m=0; m<60; m++){
            if(m<20){
                a4Vx[il][m] = (double)mRHICfHitColl -> getGSOBarEnergy(0, il, 0, m); 
                a4Vy[il][m] = (double)mRHICfHitColl -> getGSOBarEnergy(0, il, 1, m); 
            }
      else{
                a4Vx[il][m] = (double)mRHICfHitColl -> getGSOBarEnergy(1, il, 0, m-20); 
                a4Vy[il][m] = (double)mRHICfHitColl -> getGSOBarEnergy(1, il, 1, m-20); 
            }
        }

        TVectorD* Vx = new TVectorD(60,a4Vx[il]);
        TVectorD* Vy = new TVectorD(60,a4Vy[il]);

        (*Vy) *= (*mCrossTalkMatrix);
        (*Vx) *= (*mCrossTalkMatrix);

        for(int m=0; m<60; m++){
            if(m<20){
                mRHICfHitColl -> setGSOBarEnergy(0, il, 0, m, (*Vx)(m));
                mRHICfHitColl -> setGSOBarEnergy(0, il, 1, m, (*Vy)(m));
            }
      else{
                mRHICfHitColl -> setGSOBarEnergy(1, il, 0, m-20, (*Vx)(m)); 
                mRHICfHitColl -> setGSOBarEnergy(1, il, 1, m-20, (*Vy)(m)); 
            }
        }

        delete Vx;
        delete Vy;
        Vx = nullptr;
        Vy = nullptr;
    }
    return kStOk;
}

ClassImp(StRHICfHitMaker)