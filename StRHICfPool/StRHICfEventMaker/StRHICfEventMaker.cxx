// ============================================================ 
// | The StRHICfEvemtMaker class                              | 
// | for merging the RHICf+STAR data                          | 
// |                                                          | 
// |                                                          | 
// | Author: Seunghwan Lee                                    | 
// ============================================================ 

#include "StRHICfEventMaker.h"

// DB headers
#include "StRHICfDbMaker/StRHICfDbMaker.h"

// STAR headers
#include "StChain/StChain.h"
#include "StChain/StChainOpt.h"

// StMuEvents headers
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"

// rhicf
#include "StMuDSTMaker/COMMON/StMuRHICfCollection.h"
#include "StMuDSTMaker/COMMON/StMuRHICfRawHit.h"
#include "StMuDSTMaker/COMMON/StMuRHICfHit.h"
#include "StMuDSTMaker/COMMON/StMuRHICfPoint.h"

// fms
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsCluster.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"

// rps
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"

// StEvent headers
#include "StEvent/StEvent.h"
#include "StEvent/StRHICfPoint.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerDetectorCollection.h"
#include "StEvent/StTriggerIdCollection.h"
#include "StEvent/StTriggerId.h"
#include "StEvent/StBbcTriggerDetector.h"
#include "StEvent/StVpdTriggerDetector.h"
#include "StEvent/StL0Trigger.h"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StDcaGeometry.h"

// other
#include <unordered_map>
#include <bitset>
#include <vector>
#include "StMessMgr.h"
#include "Stypes.h"
#include "StEventTypes.h"
#include "StEnumerations.h"

StRHICfEventMaker::StRHICfEventMaker(int ioMode, const char* fileName, StMuDstMaker* muDstMaker, const char* name) : 
    StMaker(name), mIoMode(ioMode), mInputFileName(fileName), muDstMaker(muDstMaker)
{
    mOnAllEventSave = false;

    mOffTPCTrack = false;
    mOffBTof = false;
    mOffBBC = false;
    mOffVPD = false;
    mOffZDC = false;
	mOffFMS = false;
	mOffRPS = false;
}

StRHICfEventMaker::~StRHICfEventMaker()
{
}

Int_t StRHICfEventMaker::Init() 
{  
    fillNumArray[0] = 21142;
    fillNumArray[1] = 21145;
    fillNumArray[2] = 21148;
    fillNumArray[3] = 21149;
    fillNumArray[4] = 21150;

    mTpcVpdVzDiffCut = 6.; // default picoDst vpd-tpc vertex diff cut
    mNFiles = -1;
    iEvent = 0;

    if(mIoMode==ioRead){
        LOG_INFO << "StRHICfEventMaker::StRHICfEventMaker() -- begin StRHICfEventMaker construction for Read mode" << endm;
        if(openRead() == kStErr){return kStErr;}
    }
    else if(mIoMode==ioWrite){
        LOG_INFO << "StRHICfEventMaker::StRHICfEventMaker() -- begin StRHICfEventMaker construction for Write mode" << endm;

        spinPatternData();
        if(openWrite() == kStErr){return kStErr;}
    }

    return kStOK;
}

Int_t StRHICfEventMaker::Make() 
{
    // DST set-up
    muDst = (StMuDst*) GetInputDS("MuDst"); // from DST
    if(!muDst) {LOG_ERROR << "StRHICfEventMaker::Make() -- no StMuDst" << endm;  return kStErr;};

    muEvent = muDst->event(); // from muDST
    if(!muEvent) {LOG_ERROR << "StRHICfEventMaker::Make() -- no StMuEvent" << endm;  return kStErr;};

    muRHICfColl = muDst->muRHICfCollection();
    muRHICfRawHit = muRHICfColl->getRawHit();
    muRHICfHit = muRHICfColl->getHit();
    muFmsColl = muDst->muFmsCollection();

    if(!muRHICfColl) {LOG_ERROR << "StRHICfEventMaker::Make() -- no MuRHICfCollection"<< endm; return kStErr;}
    if(!muRHICfRawHit){LOG_ERROR << "StRHICfEventMaker::Make() -- RHICfRawHit not including " << endm; return kStErr;}
    if(!muRHICfHit){LOG_ERROR << "StRHICfEventMaker::Make() -- RHICfHit not including " << endm; return kStErr;}
    if(!muFmsColl){LOG_ERROR << "StRHICfEventMaker::Make() -- MuFmsCollection not including " << endm; return kStErr;}

    cout << " start event " << endl; // test


    // Initialize a data structure
    clear();
    cout << "after clear " << endl; // test

    // Main data Saving part
    eventData();
    cout << "after event " << endl; // test
    rhicfData();
    cout << "after rhicf " << endl; // test
    vertex();
    trackData();
    btofData();
    bbcData();
    vpdData();
    zdcData();
    fmsData();
    rpsData();

    // Fill data
    if(mOnAllEventSave){
        fillData();
        eventSummary();
    }
    else{
        // save data if RHICf detector has point
        int detPointNum = mRHICfEventDst -> GetRHICfDetPointNum();
        if(detPointNum != 0){
            fillData();
            eventSummary();
        }
    }

    return kStOK;
}

Int_t StRHICfEventMaker::Finish() 
{
    if(mIoMode==ioWrite){
        LOG_INFO << Form("StRHICfEventMaker::Finish() -- Writing and closing %s",mOutputFileName.Data()) << endm;
        mTFile -> cd();
        mTree -> Write();
        mTFile -> Close();
    }

    return kStOK;
}

Int_t StRHICfEventMaker::clear()
{
    mVertexVec.SetXYZ(-999., -999., -999.);
    mRHICfEventDst -> Clear();

    return kStOk;
}

Int_t StRHICfEventMaker::GetEventNum()
{
    if(mIoMode==ioRead){return iEvent;}
    return -1;
}

StRHICfEventDst* StRHICfEventMaker::GetEventDst(int event)
{
    if(mIoMode==ioRead){
        if(event < iEvent){
            mChain -> GetEntry(event);
            return mRHICfEventDst;
        }
        LOG_ERROR << "StRHICfEventMaker::GetEventDst() -- Event index: " << event << " larger than total event num (" << GetEventNum() << ")" << endl;
        return 0;
    }
    return 0;
}

Int_t StRHICfEventMaker::openRead()
{
    mChain = new TChain("RHICfEventDst");
    mNFiles = 0;
    TString fileName = "";

    if(mInputFileName.Length() == 0){
        // No input file
        LOG_ERROR << "StRHICfEventMaker::openRead() -- Input file is not a existing ... " << endm;
        return kStErr;
    }
    else{
        std::string const dirFile = mInputFileName.Data();
        if( dirFile.find(".list") != std::string::npos || dirFile.find(".lis") != std::string::npos ){
            std::ifstream inputStream( dirFile.c_str() );
            if(!inputStream) {LOG_ERROR << "StRHICfEventMaker::openRead() --  Cannot open list file " << dirFile << endm;}

            std::string file;
            size_t pos;
            while(getline(inputStream, file)){
                pos = file.find_first_of(" ");
                if (pos != std::string::npos ) file.erase(pos,file.length()-pos);
                if(file.find("RHICfEventDst") != std::string::npos) {
                    fileName = file;
                    mChain -> Add(fileName);
                    mNFiles++;
                }       
            }
        }
        else if(dirFile.find("RHICfEventDst") != std::string::npos){
            fileName = dirFile;
            mChain -> Add(fileName);
            mNFiles = 1;
        }
    }

    if(mNFiles == 1){
        LOG_INFO << Form("StRHICfEventMaker::openRead() -- Input file name: %s", fileName.Data()) << endm;
    }
    else{
        LOG_INFO << Form("StRHICfEventMaker::openRead() -- number of Files in .list: %i", mNFiles) << endm;
    }

    mRHICfEventDst = new StRHICfEventDst();
    if(mOffTPCTrack){mChain -> SetBranchStatus("StRHICfTPCTrack*", 0);}
    if(mOffBTof){mChain -> SetBranchStatus("StRHICfBTof*", 0);}
    if(mOffBBC){mChain -> SetBranchStatus("StRHICfBBC*", 0);}
    if(mOffVPD){mChain -> SetBranchStatus("StRHICfVPD*", 0);}
    if(mOffZDC){mChain -> SetBranchStatus("StRHICfZDC*", 0);}
    if(mOffFMS){mChain -> SetBranchStatus("StRHICfFMS*", 0);}
    if(mOffRPS){mChain -> SetBranchStatus("StRHICfRPS*", 0);}
	mRHICfEventDst -> ReadDstArray(mChain);
    iEvent = mChain -> GetEntries();

    return kStOk;
}

Int_t StRHICfEventMaker::openWrite()
{
    TString streamTypeName;
    TString runNumber;
    TString daqNumber;
    mNFiles = 0;
    TString rootFileName;

    if(mInputFileName.Length() == 0){
        // No input file
        LOG_ERROR << "StRHICfEventMaker::openWrite() -- Input file is not a existing ... " << endm;
        return kStErr;
    }
    else{
        std::string const dirFile = mInputFileName.Data();
        if( dirFile.find(".list") != std::string::npos || dirFile.find(".lis") != std::string::npos ){
            std::ifstream inputStream( dirFile.c_str() );
            if(!inputStream) {LOG_ERROR << "StRHICfEventMaker::openWrite() -- ERROR: Cannot open list file " << dirFile << endm;}

            std::string file;
            size_t pos;
            while(getline(inputStream, file)){
                pos = file.find_first_of(" ");
                if (pos != std::string::npos ) file.erase(pos,file.length()-pos);
                if(file.find("MuDst") != std::string::npos) {
                    
                    std::stringstream sstreamPath(file);
                    std::string wordPath;
                    std::vector<string> wordPaths;
                    while(getline(sstreamPath, wordPath, '/')){
                        wordPaths.push_back(wordPath);
                    }
                    int wordSize = wordPaths.size();

                    std::stringstream sstreamWord(wordPaths[wordSize-1]);
                    rootFileName = wordPaths[wordSize-1];
                    std::string word;
                    int nIdx = 0;
                    while(getline(sstreamWord, word, '_')){
                        // find a production stream type 
                        if(nIdx==1){streamTypeName = word;}
                        if(streamTypeName == "physics"){
                            LOG_ERROR << "StRHICfEventMaker::openWrite() -- Files has not RHICf stream!!! (" << wordPaths[wordSize-1] << ")" << endm;
                            return kStErr;
                        }
                        // find a runnumber 
                        if(nIdx==2){
                            if(word!="adc"){runNumber = word;}
                        }
                        if(nIdx==4){
                            std::stringstream sstreamWord2(word);
                            while(getline(sstreamWord2, word, '.')){
                                daqNumber = word;
                                break;
                            }
                        }
                        nIdx++;
                    }
                    mNFiles++;
                } 

            } 
            LOG_INFO << "StRHICfEventMaker::openWrite() -- Total " << mNFiles << " files have been read in. " << endm;
        }
        else if(dirFile.find("MuDst") != std::string::npos){
            std::stringstream sstreamPath(dirFile);
            std::string wordPath;
            std::vector<string> wordPaths;
            while(getline(sstreamPath, wordPath, '/')){
                wordPaths.push_back(wordPath);
            }
            int wordSize = wordPaths.size();

            std::stringstream sstreamWord(wordPaths[wordSize-1]);
            rootFileName = wordPaths[wordSize-1];
            std::string word;
            int nIdx = 0;
            while(getline(sstreamWord, word, '_')){
                // find a production stream type 
                if(nIdx==1){streamTypeName = word;}
                if(streamTypeName == "physics"){
                    LOG_ERROR << "StRHICfEventMaker::openWrite() -- Files has not RHICf stream!!! (" << wordPaths[wordSize-1] << ")" << endm;
                    return kStErr;
                }
                // find a runnumber 
                if(nIdx==2){
                    if(word!="adc"){runNumber = word;}
                }
                if(nIdx==4){
                    std::stringstream sstreamWord2(word);
                    while(getline(sstreamWord2, word, '.')){
                        daqNumber = word;
                        break;
                    }
                }
                nIdx++;
            }
            mNFiles = 1;
        }
    }

    if(mNFiles == 1){
        mOutputFileName = "st_" + streamTypeName + "_" + runNumber + "_pp500_" + daqNumber + "_RHICfEventDst.root";
        LOG_INFO << Form("StRHICfEventMaker::openWrite() -- Input file name: %s", rootFileName.Data()) << endm;
    }
    else{
        mOutputFileName = "st_" + streamTypeName + "_" + runNumber + "_pp500_RHICfEventDst.root";
        LOG_INFO << Form("StRHICfEventMaker::openWrite() -- number of Files in .list: %i", mNFiles) << endm;
    }
    
    LOG_INFO << Form("StRHICfEventMaker::openWrite() -- Output file name: %s",mOutputFileName.Data()) << endm;

    mTFile = new TFile(mOutputFileName.Data(), "recreate");
    mTree = new TTree("RHICfEventDst","RHICfEventDst");

    mRHICfEventDst = new StRHICfEventDst();
    if(mOffTPCTrack){mRHICfEventDst -> OffTPCTrack();}
    if(mOffBTof){mRHICfEventDst -> OffBTof();}
    if(mOffBBC){mRHICfEventDst -> OffBBC();}
    if(mOffVPD){mRHICfEventDst -> OffVPD();}
    if(mOffZDC){mRHICfEventDst -> OffZDC();}
    if(mOffFMS){mRHICfEventDst -> OffFMS();}
    if(mOffRPS){mRHICfEventDst -> OffRPS();}

    mRHICfEventDst -> CreateDstArray(mTree);
    return kStOk;
}

void StRHICfEventMaker::fillData()
{
    mTree -> Fill();
    iEvent++;
}

Int_t StRHICfEventMaker::eventData()
{
    const StTriggerData* trigData =  muEvent->triggerData();
    if(!trigData){LOG_WARN << "StRHICfEventMaker::eventData() -- no found StTriggerData" << endm; return kStWarn;}

    // General event information
    StRunInfo runInfo = muEvent->runInfo(); 
    Int_t fillNum = runInfo.beamFillNumber(blue); // star fill number
    Int_t runNum = muEvent->runNumber();
    Int_t eventNum = muEvent->eventId();
    Int_t bunch7Bit = trigData->bunchId7Bit();
    Double_t beamEnergy = runInfo.beamEnergy(StBeamDirection::blue); // [GeV] at COM system
    Int_t spinBit = 0;

    int fillIdx = -1;
    for(int fill=0; fill<5; fill++){
        if(fillNumArray[fill] == fillNum){fillIdx = fill;}
    }
    if(bunch7Bit<31 || (40<=bunch7Bit && bunch7Bit <= 110)){
        int bluePatt = spinPatterns[fillIdx][bunch7Bit][0];
        int yellowPatt = spinPatterns[fillIdx][bunch7Bit][1];

        if(bluePatt == 1 && yellowPatt == 1){spinBit = 5;}
        if(bluePatt == 1 && yellowPatt == -1){spinBit = 6;}
        if(bluePatt == -1 && yellowPatt == 1){spinBit = 9;}
        if(bluePatt == -1 && yellowPatt == -1){spinBit = 10;}
    }

    // =================== Trigger information ======================
    Bool_t isRHICf = muEvent->triggerIdCollection().nominal().isTrigger(590901);
    Bool_t isDiffractive = muEvent->triggerIdCollection().nominal().isTrigger(590902);
    Bool_t isVPD = muEvent->triggerIdCollection().nominal().isTrigger(590903);
    Bool_t isTPC = muEvent->triggerIdCollection().nominal().isTrigger(590904);

    mRHICfEvent = mRHICfEventDst -> GetEvent();
    mRHICfEvent -> SetRunNumber(runNum);
    mRHICfEvent -> SetEventNumber(eventNum);
    mRHICfEvent -> SetFillNumber(fillNum);
    mRHICfEvent -> SetBunchNumber(bunch7Bit);
    mRHICfEvent -> SetSpinBit(spinBit);
    mRHICfEvent -> SetBeamEnergy(beamEnergy);
    mRHICfEvent -> SetRHICfTrig(isRHICf);
    mRHICfEvent -> SetDiffractiveTrig(isDiffractive);
    mRHICfEvent -> SetVPDMB30Trig(isVPD);
    mRHICfEvent -> SetTPCTrig(isTPC);

    mRHICfEvent -> SetRHICfRunNumber(muRHICfRawHit -> getRHICfRunNumber());
    mRHICfEvent -> SetRHICfEventNumber(muRHICfRawHit -> getRHICfEventNumber());
    mRHICfEvent -> SetRHICfTriggerNumber(muRHICfRawHit -> getTriggerNumber());
    mRHICfEvent -> SetRHICfRunType(muRHICfRawHit -> getRunType());

    mRHICfEvent -> SetTofMult(trigData->tofMultiplicity()); // B-TOF mult in DAQ level
    mRHICfEvent -> SetBTofMult(muEvent->btofTrayMultiplicity()); // correctly B-TOF hit multiplicity in MuDst level
    mRHICfEvent -> SetRefMult(muEvent->refMult());
    mRHICfEvent -> SetGRefMult(muEvent->grefmult());

    for(int it=0; it<kTowerNum; it++){
        double l20 = muRHICfHit->getL20(it);
        double l90 = muRHICfHit->getL90(it);
        mRHICfEvent -> SetRHICfL20(it, l20);
        mRHICfEvent -> SetRHICfL90(it, l90);
    }

    return kStOk;
}

Int_t StRHICfEventMaker::rhicfData()
{
    // store all photon data in temporary structure
    Int_t rhicfPointNum = muRHICfColl->numberOfPoints();
    for(int classNum = 0; classNum<rhicfPointNum; classNum++){
        StMuRHICfPoint* muRHICfPoint = muRHICfColl->getPoint(classNum);

        // After event cut for analysis
        if(!eventCut(muRHICfPoint)){continue;}

        int rhicfDetPointNum = mRHICfEventDst -> GetRHICfDetPointNum();
        mRHICfDetPoint = mRHICfEventDst -> GetRHICfDetPoint(rhicfDetPointNum);
        mRHICfDetPoint -> SetTowerIdx(muRHICfPoint -> getTowerIdx());
        mRHICfDetPoint -> SetPID(muRHICfPoint -> getPID());
        mRHICfDetPoint -> SetPointPos(muRHICfPoint -> getPointPos(0), muRHICfPoint -> getPointPos(1));
        mRHICfDetPoint -> SetPointEnergy(muRHICfPoint -> getPointEnergy(0), muRHICfPoint -> getPointEnergy(1));  
        mRHICfDetPoint -> SetTowerSumEnergy(muRHICfPoint -> getTowerSumEnergy(0) , muRHICfPoint -> getTowerSumEnergy(1));
    }

    if(mRHICfEventDst -> GetRHICfDetPointNum() > 0){
        convertIPSystem(true, false); // Convert the coordinate system from RHICf tower origin to Collision Point
        kinematics();                 // Calculate the Kinematics and various values for Pi0
    }

    return kStOk;
}

Int_t StRHICfEventMaker::vertex()
{
    // Same as PicoVtxMode::VpdOrDefault
    muDst->setVertexIndex(0);
    StMuPrimaryVertex* pVertex = muDst->primaryVertex();
    if(!pVertex){return kStOk;}

    mVertexVec.SetXYZ(pVertex->position().x(), pVertex->position().y(), pVertex->position().z());

    StBTofHeader const* mBTofHeader = muDst->btofHeader();
    if (mBTofHeader && fabs(mBTofHeader->vpdVz()) < 200.){
        float vzVpd = mBTofHeader->vpdVz();
        for (unsigned int iVtx=0; iVtx<muDst->numberOfPrimaryVertices(); ++iVtx){
            StMuPrimaryVertex* vtx = muDst->primaryVertex(iVtx);
            if (!vtx) continue;
            if (fabs(vzVpd - vtx->position().z()) < mTpcVpdVzDiffCut){
                muDst->setVertexIndex(iVtx);
                mVertexVec.SetXYZ(muDst->primaryVertex()->position().x(), muDst->primaryVertex()->position().y(), muDst->primaryVertex()->position().z());
                break;
            }
        }
    }
    mRHICfEvent -> SetPrimaryVertex(mVertexVec.X(), mVertexVec.Y(), mVertexVec.Z());

    return kStOk;
}

Int_t StRHICfEventMaker::trackData()
{
    if(!muDst->primaryVertex()){return kStOk;}

    Double_t mBField = muEvent -> magneticField();

    std::unordered_map<unsigned int, unsigned int> index2Primary;

    // Primary tracks that used vertex fitting
    StMuTrack* pTrk;
    UShort_t nPrimaryTracks = muDst->numberOfPrimaryTracks();
    for(int i=0; i<nPrimaryTracks; i++){
        pTrk = (StMuTrack*)muDst->primaryTracks(i);
        if(!pTrk){continue;}
        index2Primary[pTrk->id()] = i;
    }

    UShort_t primaryTrkNum = 0;
    UShort_t globalTrkNum = 0;

    // Global tracks that all of the tracks in TPC
    StMuTrack* gTrk;
    UShort_t nGlobalTracks = muDst->numberOfGlobalTracks();
    for(int i=0; i<nGlobalTracks; i++){
        gTrk = (StMuTrack*)muDst->globalTracks(i);
        if(!gTrk){continue;}
        if(gTrk->type() != global){continue;}
        StMuTrack const* const pTrk = index2Primary.find(gTrk->id()) != index2Primary.end() ? (StMuTrack*)muDst->primaryTracks(index2Primary[gTrk->id()]) : nullptr;

        Bool_t isPrimary = false;
        if(pTrk){
            if (pTrk->type() != primary) continue;
            if (pTrk->id() != gTrk->id()) continue;
            isPrimary = true;
        }
        if(gTrk->index2Cov() < 0){continue;}

        StDcaGeometry* dcaG = muDst->covGlobTracks(gTrk->index2Cov());
        if(!dcaG){
            LOG_WARN << "No dca Geometry for this track !!! " << i << endm;
            continue;
        }

        if(isPrimary){primaryTrkNum++;}
        else{globalTrkNum++;}

        if(isPrimary == false){continue;}

        if(mOffTPCTrack){continue;}
    
        int tpcTrackNum = mRHICfEventDst -> GetTPCTrackNum();
        mRHICfTPCTrack = mRHICfEventDst -> GetTPCTrack(tpcTrackNum);

        mRHICfTPCTrack -> SetChi2(gTrk->chi2());
        if(gTrk->primaryTrack()){mRHICfTPCTrack -> SetStatus(1);}
        else{mRHICfTPCTrack -> SetStatus(0);}

        THelixTrack t = dcaG->thelix();
        StThreeVectorD V(muDst->primaryVertex()->position());

        mRHICfTPCTrack -> SetDca3D(t.Dca(V.xyz())); // distance b.t.w global and vertex position in 3D.
        mRHICfTPCTrack -> SetDedx(gTrk->probPidTraits().dEdxFit());
        mRHICfTPCTrack -> SetDndx(gTrk->probPidTraits().dNdxFit());

        // Fill track's hit information
        UShort_t flag = gTrk->flag();
        UShort_t nHitsFit = 0;
        UShort_t nHitsMax = 0;

        if(flag / 100 < 7){ // TPC tracks
            nHitsFit = gTrk->nHitsFit(kTpcId) * gTrk->charge();
            nHitsMax = gTrk->nHitsPoss(kTpcId);
        }
        else { // FTPC tracks
            if(gTrk->helix().momentum(mBField * kilogauss).pseudoRapidity() > 0.){
                //West FTPC
                nHitsFit = gTrk->nHitsFit(kFtpcWestId) * gTrk->charge();
                nHitsMax = gTrk->nHitsPoss(kFtpcWestId);
            }
            else{
                // East FTPC
                nHitsFit = gTrk->nHitsFit(kFtpcEastId) * gTrk->charge();
                nHitsMax = gTrk->nHitsPoss(kFtpcEastId);
            }
        }
        mRHICfTPCTrack -> SetnHitDedx(gTrk->nHitsDedx());
        mRHICfTPCTrack -> SetnHitFit(nHitsFit);
        mRHICfTPCTrack -> SetnHitMax(nHitsMax);

        // position and momentum
        const StThreeVectorF &pvert =  muDst->primaryVertex()->position();
        Double_t vtx[3] = {pvert[0],pvert[1],pvert[2]};
        THelixTrack thelix =  dcaG->thelix();
        thelix.Move(thelix.Path(vtx));
        const Double_t *pos = thelix.Pos();
        const Double_t *mom = thelix.Dir();
        StThreeVectorF momAtDca(mom[0], mom[1], mom[2]);
        momAtDca *= dcaG->momentum().mag();

        mRHICfTPCTrack -> SetOriginPos(pos[0], pos[1], pos[2]);

        if(isPrimary){
            mRHICfTPCTrack -> SetMomentum(pTrk->momentum().x(), pTrk->momentum().y(), pTrk->momentum().z());
            mRHICfTPCTrack -> SetIsPrimary();
        }
        else{
            mRHICfTPCTrack -> SetMomentum(momAtDca[0], momAtDca[1], momAtDca[2]);
        }

        // nSigmaPID information
        mRHICfTPCTrack -> SetSigmaElectron(gTrk->nSigmaElectron());
        mRHICfTPCTrack -> SetSigmaPion(gTrk->nSigmaPion());
        mRHICfTPCTrack -> SetSigmaKaon(gTrk->nSigmaKaon());
        mRHICfTPCTrack -> SetSigmaProton(gTrk->nSigmaProton());

        // prob PID information
        mRHICfTPCTrack -> SetProbElectron(gTrk->pidProbElectron());
        mRHICfTPCTrack -> SetProbPion(gTrk->pidProbPion());
        mRHICfTPCTrack -> SetProbKaon(gTrk->pidProbKaon());
        mRHICfTPCTrack -> SetProbProton(gTrk->pidProbProton());

        // pid traits 
        if(gTrk->tofHit()){
            mRHICfTPCTrack -> SetBtofMatchFlag( (UChar_t)(gTrk->btofPidTraits().matchFlag()) );
            mRHICfTPCTrack -> SetBtofTOF( gTrk->btofPidTraits().timeOfFlight() );
            mRHICfTPCTrack -> SetBtofBeta( gTrk->btofPidTraits().beta() );
            mRHICfTPCTrack -> SetBtofSigmaElectron( gTrk->btofPidTraits().sigmaElectron() );
            mRHICfTPCTrack -> SetBtofSigmaPion( gTrk->btofPidTraits().sigmaPion() );
            mRHICfTPCTrack -> SetBtofSigmaKaon( gTrk->btofPidTraits().sigmaKaon() );
            mRHICfTPCTrack -> SetBtofSigmaProton( gTrk->btofPidTraits().sigmaProton() );
        }
        if(gTrk->etofHit()){
            mRHICfTPCTrack -> SetEtofMatchFlag( (UChar_t)(gTrk->etofPidTraits().matchFlag()) );
            mRHICfTPCTrack -> SetEtofTOF(  gTrk->etofPidTraits().timeOfFlight() );
            mRHICfTPCTrack -> SetEtofBeta( gTrk->etofPidTraits().beta() );
        }
        if(gTrk->mtdHit()) {
            mRHICfTPCTrack -> SetMtdMatchFlag( (Char_t)gTrk->mtdPidTraits().matchFlag() );
            mRHICfTPCTrack -> SetMtdDeltaTOF( gTrk->mtdPidTraits().timeOfFlight() - gTrk->mtdPidTraits().expTimeOfFlight() );
            const double c_light = 2.99792458e+8;
            mRHICfTPCTrack -> SetMtdBeta( gTrk->mtdPidTraits().pathLength() / gTrk->mtdPidTraits().expTimeOfFlight() * 1e9 / c_light );
        }
    }
    mRHICfEvent -> SetPrimaryTrkNum(primaryTrkNum);
    mRHICfEvent -> SetGlobalTrkNum(globalTrkNum);

    return kStOk;
}

Int_t StRHICfEventMaker::btofData()
{
    if(mOffBTof){return kStOk;}
    Int_t bTofHitNum = muDst -> GetNBTofHit(); // used in picoDstMaker (same as numberOfBTofHit() ) it must be included VPD hit index
    for (int i=0; i<bTofHitNum; i++) {
        StMuBTofHit* btofHit = (StMuBTofHit*)muDst->btofHit(i);
        if (!btofHit) continue;
        Int_t trayId = btofHit->tray();
        Int_t moduleId = btofHit->module();
        Int_t cell = btofHit -> cell();
        
        int btofNum = mRHICfEventDst -> GetBTofNum();
        mRHICfBTof = mRHICfEventDst -> GetBTof(btofNum);
        
        if(trayId > 120){mRHICfBTof -> SetIsVPD();} // BTof has only 120 trays, over the 120 index will be VPD hit info
        mRHICfBTof -> SetTray(trayId);
        mRHICfBTof -> SetModule(moduleId);
        mRHICfBTof -> SetCell(cell);
    }

    return kStOk;
}

Int_t StRHICfEventMaker::bbcData()
{
    if(mOffBBC){return kStOk;}
    const StTriggerData* trigData = muEvent->triggerData();
    mRHICfBBC = mRHICfEventDst -> GetBBC();

    for(int dir=0; dir<kBeamSideNum; dir++){
        StBeamDirection direction = (dir==0)? StBeamDirection::east : StBeamDirection::west;
        // BBC Small
        for(int pmt=0; pmt<kBBCSmallPmtNum; pmt++){
            // Note: trigData::bbc small data index started from 1
            mRHICfBBC -> SetBBCSmallADC(dir, pmt, trigData->bbcADC(direction, pmt+1));
            mRHICfBBC -> SetBBCSmallTDC(dir, pmt, trigData->bbcTDC5bit(direction, pmt+1));
            mRHICfBBC -> SetBBCSmallTAC(dir, pmt, trigData->bbcTDC(direction, pmt+1));
        }

        // BBC Large 
        for(int pmt=0; pmt<kBBCLargePmtNum; pmt++){
            // Note: trigData::bbc small data index started from 17
            mRHICfBBC -> SetBBCLargeADC(dir, pmt, trigData->bbcADC(direction, pmt+17));
            mRHICfBBC -> SetBBCLargeTDC(dir, pmt, trigData->bbcTDC5bit(direction, pmt+17));
            mRHICfBBC -> SetBBCLargeTAC(dir, pmt, trigData->bbcTDC(direction, pmt+17));
        }
    }

    return kStOk;
}

Int_t StRHICfEventMaker::vpdData()
{
    if(mOffVPD){return kStOk;}
    const StTriggerData* trigData = muEvent->triggerData();
    mRHICfVPD = mRHICfEventDst -> GetVPD();

    for(int dir=0; dir<kBeamSideNum; dir++){
        StBeamDirection direction = (dir==0)? StBeamDirection::east : StBeamDirection::west;
        for(int pmt=0; pmt<kVPDPmtNum; pmt++){
            // Note: trigData::vpd data index started from 1
            mRHICfVPD -> SetVPDADC(dir, pmt, trigData->vpdADC(direction, pmt+1));
            mRHICfVPD -> SetVPDTDC(dir, pmt, trigData->vpdTDC(direction, pmt+1));
            mRHICfVPD -> SetVPDHighThrADC(dir, pmt, trigData->vpdADCHighThr(direction, pmt+1));
            mRHICfVPD -> SetVPDHighThrTDC(dir, pmt, trigData->vpdTDCHighThr(direction, pmt+1));
        }
        mRHICfVPD -> SetEarliestTDC(dir, trigData->vpdEarliestTDC(direction));
        mRHICfVPD -> SetEarliestHighThrTDC(dir, trigData->vpdEarliestTDCHighThr(direction));
    }
    mRHICfVPD -> SetVPDTimeDiff(trigData->vpdTimeDifference());

    return kStOk;
}

Int_t StRHICfEventMaker::zdcData()
{
    if(mOffZDC){return kStOk;}
    const StTriggerData* trigData = muEvent->triggerData();
    mRHICfZDC = mRHICfEventDst -> GetZDC();

    for(int dir=0; dir<kBeamSideNum; dir++){
        StBeamDirection direction = (dir==0)? StBeamDirection::east : StBeamDirection::west;
        for(int pmt=0; pmt<kZDCPmtNum; pmt++){
            // Note: trigData::zdc pmt data index started from 1
            mRHICfZDC -> SetZDCPmtADC(dir, pmt, trigData->zdcADC(direction, pmt+1));
            mRHICfZDC -> SetZDCPmtTDC(dir, pmt, trigData->zdcPmtTDC(direction, pmt+1));
        }
        mRHICfZDC -> SetZDCPmtAttenuatedSumADC(dir, trigData->zdcAttenuated(direction));
        mRHICfZDC -> SetZDCPmtUnAttenuatedSumADC(dir, trigData->zdcUnAttenuated(direction));
        mRHICfZDC -> SetZDCPmtSumTDC(dir, trigData->zdcTDC(direction));

        for(int axis=0; axis<2; axis++){
            // Note: axis=0 for vertical, axis=1 for horizontal
            for(int smd=0; smd<kSMDNum; smd++){
                // Note: trigData::zdc smd data index started from 1
                mRHICfZDC -> SetZDCSmdADC(dir, axis, smd, trigData->zdcSMD(direction, axis, smd+1));
            }
        }
    }
    mRHICfZDC -> SetZDCVtxZ(trigData->zdcVertexZ());

    return kStOk;
}

Int_t StRHICfEventMaker::fmsData()
{
    if(mOffFMS){return kStOk;}
    int fmsClusterNum = muFmsColl -> numberOfClusters();
    for(int i=0; i<fmsClusterNum; i++){
        StMuFmsCluster* cluster = muFmsColl->getCluster(i);

        Int_t category = cluster -> category();
        if(category == 0 || category == 3){continue;} // not fitted

        // photon in Cluster
        auto photon = cluster->photons();
        Int_t numerOfPoints = photon->GetEntries();

        Double_t x = 0.;
        Double_t y = 0.;
        Double_t z = 0.;
        for(int i=0; i<numerOfPoints; i++){
            StMuFmsPoint* point = (StMuFmsPoint*)photon -> At(i);
            x += point->xyz().x()*point->energy()/cluster->energy();
            y += point->xyz().y()*point->energy()/cluster->energy();
            z += point->xyz().z()*point->energy()/cluster->energy();
        }

        Double_t r = sqrt(x*x + y*y + z*z);
        Double_t e = cluster->energy();
        Double_t px = x/r*e;
        Double_t py = y/r*e;
        Double_t pz = z/r*e;

        int fmsNum = mRHICfEventDst -> GetFMSNum();
        mRHICfFMS = mRHICfEventDst -> GetFMS(fmsNum);
        mRHICfFMS -> SetCategory(category);
        mRHICfFMS -> SetFMSHitPos(x, y, z);
        mRHICfFMS -> SetMomentum(px, py, pz);
        mRHICfFMS -> SetEnergy(e);
    }

    return kStOk;
}

Int_t StRHICfEventMaker::rpsData()
{
    if(mOffRPS){return kStOk;}
    mMuRpsColl = muDst->RpsCollection();
    int rpsTrackNum = mMuRpsColl->numberOfTracks();

    if(rpsTrackNum > 5){
        LOG_INFO << "StRHICfEventMaker::rpsData() --- Not physical event... continue Roman Pots data" << endl;
        return kStOk;
    }

    for(int iTrack=0; iTrack<rpsTrackNum; iTrack++){
        StMuRpsTrack* rpsTrack = mMuRpsColl->track(iTrack);
        if(rpsTrack){
            Int_t trackBranch = rpsTrack -> branch();
            Int_t trackType = (Int_t)(rpsTrack -> type()); // 0 == reco. only one point (local) , 1 == reco. using two point (global) , 2 == undefined
            Int_t trackUsedPlane = rpsTrack->planesUsed();
            Float_t trackP = rpsTrack -> p();
            Float_t trackPt = rpsTrack -> pt();
            Float_t trackEta = rpsTrack -> eta();
            Float_t trackT = rpsTrack -> t( muEvent->runInfo().beamEnergy(rpsTrack->branch()<2 ? StBeamDirection::east : StBeamDirection::west) );
            Float_t trackXi = rpsTrack -> xi( muEvent->runInfo().beamEnergy(rpsTrack->branch()<2 ? StBeamDirection::east : StBeamDirection::west) );

            Bool_t isTrackBad = false;
            if(trackBranch<0 || trackBranch>3){isTrackBad=true;}
            if(trackType<0 || trackType>1){isTrackBad=true;}
            if(trackUsedPlane<0 || trackUsedPlane>8){isTrackBad=true;}
            if(trackP<0 || trackP>300){isTrackBad=true;}
            if(trackPt<0 || trackPt>300){isTrackBad=true;}
            if(trackEta<-30 || trackEta>30){isTrackBad=true;}
            if(trackXi<-3 || trackXi>3){isTrackBad=true;}
            if(trackT<-1000 || trackT>0){isTrackBad=true;}

            if(isTrackBad == false){
                int rpsNum = mRHICfEventDst -> GetRPSNum();
                mRHICfRPS = mRHICfEventDst -> GetRPS(rpsNum);
                mRHICfRPS -> SetUsedPlane(trackUsedPlane);
                mRHICfRPS -> SetTrkType(trackType);
                mRHICfRPS -> SetTrkBranch(trackBranch);

                for(int iPoint=0; iPoint<2; iPoint++){
                    const StMuRpsTrackPoint* trackPoint = rpsTrack->trackPoint(iPoint);
                    if(trackPoint){
                        TVector3 pos = trackPoint->positionVec();
                        mRHICfRPS -> SetRPSHitPos(iPoint, pos.X(), pos.Y(), pos.Z());
                    }
                }

                mRHICfRPS -> SetMomentum(rpsTrack->pVec().X(), rpsTrack->pVec().Y(), rpsTrack->pVec().Z());
            }
        }
    }

    return kStOk;
}

Bool_t StRHICfEventMaker::eventCut(StMuRHICfPoint* point)
{
    const Float_t geometryCut = 2.; // [mm]
    const Float_t energyCut = 20.; // [GeV]

    if(point->getPointEnergy(0) < energyCut){return false;} // Energy cut, if energy lower than 20 GeV
    double towerSize = double(checkGSOBarSize(point->getTowerIdx())); 

    Double_t x = point -> getPointPos(0);
    Double_t y = point -> getPointPos(1);
    if(x > towerSize-geometryCut || x < geometryCut){return false;} // Position cut, if x is out of cutting range 
    if(y > towerSize-geometryCut || y < geometryCut){return false;} // Position cut, if y is out of cutting range 

    return true;
}

void StRHICfEventMaker::convertIPSystem(bool metOpt, bool referOpt)
{
    // for convert Collision Point system, define the system in TS origin
    Int_t TowerIdxByRunType = -999;
    Double_t distOriginToOrigin = distTStoTL - TMath::Sqrt(2.)*geoCenterTL + TMath::Sqrt(2.)*geoCenterTS;
    Double_t beamCenterX = beamCenterPosition(0, metOpt, referOpt);
    Double_t beamCenterY = beamCenterPosition(1, metOpt, referOpt);

    int runType = mRHICfEvent -> GetRHICfRunType();
    if(runType == kRHICfTL){
        TowerIdxByRunType = 1;
        beamCenterY = beamCenterY + TMath::Sqrt(2.)*geoCenterTL;
    }
    if(runType == kRHICfTS || runType == kRHICfTOP){
        TowerIdxByRunType = 0;
        beamCenterY = beamCenterY + TMath::Sqrt(2.)*geoCenterTS;
    }

    int detPointNum = mRHICfEventDst -> GetRHICfDetPointNum();
    for(int i=0; i<detPointNum; i++){
        mRHICfDetPoint = mRHICfEventDst -> GetRHICfDetPoint(i);
        double posX = mRHICfDetPoint -> GetPointPos(0);
        double posY = mRHICfDetPoint -> GetPointPos(1);

        // the Hit Position convert to one tower coordinate
        if(TowerIdxByRunType == 0 && mRHICfDetPoint -> GetTowerIdx() == 1){
            posX += distOriginToOrigin/TMath::Sqrt(2.);
            posY += distOriginToOrigin/TMath::Sqrt(2.);
        }
        if(TowerIdxByRunType == 1 && mRHICfDetPoint -> GetTowerIdx() == 0){
            posX -= distOriginToOrigin/TMath::Sqrt(2.);
            posY -= distOriginToOrigin/TMath::Sqrt(2.);
        }

        // Hit position rotate to -45 degree.
        double tmpPosX = (posX - posY)/TMath::Sqrt(2.);
        double tmpPosY = (posX + posY)/TMath::Sqrt(2.);

        // convert to beam center and IP system [m]
        posX = (tmpPosX - beamCenterX)*0.001;
        posY = (tmpPosY - beamCenterY)*0.001;

        mRHICfDetPoint -> SetPointPos(posX, posY);
    }
}

void StRHICfEventMaker::kinematics()
{
    // Note: Lambda and mixing point selection method will be updated. 2025/Jan/28
    // for Pi0
    if(mRHICfEventDst -> GetRHICfDetPointNum() == 2){
        const Float_t l90LowerCut = 8.;
        const Float_t l90UpperCut = 18.;

        mRHICfDetPoint = mRHICfEventDst -> GetRHICfDetPoint(0);
        int towerIdx1 = mRHICfDetPoint -> GetTowerIdx();
        double x1 = mRHICfDetPoint -> GetPointPos(0); // [m]
        double y1 = mRHICfDetPoint -> GetPointPos(1); // [m]
        double e1 = mRHICfDetPoint -> GetPointEnergy(0); // [GeV]

        mRHICfDetPoint = mRHICfEventDst -> GetRHICfDetPoint(1);
        int towerIdx2 = mRHICfDetPoint -> GetTowerIdx();
        double x2 = mRHICfDetPoint -> GetPointPos(0); // [m]
        double y2 = mRHICfDetPoint -> GetPointPos(1); // [m]
        double e2 = mRHICfDetPoint -> GetPointEnergy(0); // [GeV]

        double l90_tower1 = mRHICfEvent -> GetRHICfL90(towerIdx1);
        double l90_tower2 = mRHICfEvent -> GetRHICfL90(towerIdx2);

        if((l90LowerCut <= l90_tower1 && l90_tower1 <= l90UpperCut) && (l90LowerCut <= l90_tower2 && l90_tower2 <= l90UpperCut)){
            int towerIdx = -1;
            int pi0Type = -1;
            if(towerIdx1 == towerIdx2){
                if(x1 != x2 && y1 != y2){
                    pi0Type = 2;
                    towerIdx = towerIdx1;
                }
            }
            else{pi0Type = 1;}

            if(pi0Type > 0){
                Double_t z = distZatIP*0.001;
                double r1 = sqrt(x1*x1 + y1*y1 + z*z);
                double r2 = sqrt(x2*x2 + y2*y2 + z*z);

                double e = e1 + e2;
                
                double px = (x1/r1)*e1 + (x2/r2)*e2; 
                double py = (y1/r1)*e1 + (y2/r2)*e2; 
                double pz = (z/r1)*e1 + (z/r2)*e2; 
                
                double mass = sqrt(e*e - px*px - py*py - pz*pz)*1000.;

                int particleNum = mRHICfEventDst -> GetRHICfParticleNum();
                mRHICfParticle = mRHICfEventDst -> GetRHICfParticle(particleNum);
                mRHICfParticle -> SetPID(kPi0);
                mRHICfParticle -> SetPi0Type(pi0Type);
                mRHICfParticle -> SetTowerIdx(towerIdx);
                mRHICfParticle -> SetRHICfHitPos(x1+x2, y1+y2, z); // [m]
                mRHICfParticle -> SetMomentum(px, py, pz); // [GeV/c]
                mRHICfParticle -> SetEnergy(e); // [GeV]
                mRHICfParticle -> SetMass(mass); // [MeV/c^2]
            }
        }
    }
    // for Neutron
    if(mRHICfEventDst -> GetRHICfDetPointNum() == 1){
        const Float_t l90Cut = 20.; 

        mRHICfDetPoint = mRHICfEventDst -> GetRHICfDetPoint(0);
        int towerIdx = mRHICfDetPoint -> GetTowerIdx();
        double x = mRHICfDetPoint -> GetPointPos(0); // [m]
        double y = mRHICfDetPoint -> GetPointPos(1); // [m]
        double e = mRHICfDetPoint -> GetPointEnergy(1); // [GeV]

        double l90 = mRHICfEvent -> GetRHICfL90(towerIdx);
        if(l90 > l90Cut){
            Double_t z = distZatIP*0.001;
            double r = sqrt(x*x + y*y + z*z); 

            double mass = 0.939565; 
            double p = sqrt(e*e - mass*mass); 
            double px = p*(x/r); 
            double py = p*(y/r); 
            double pz = p*(z/r); 

            int particleNum = mRHICfEventDst -> GetRHICfParticleNum();
            mRHICfParticle = mRHICfEventDst -> GetRHICfParticle(particleNum);
            mRHICfParticle -> SetPID(kNeutron);
            mRHICfParticle -> SetPi0Type(-1);
            mRHICfParticle -> SetTowerIdx(towerIdx);
            mRHICfParticle -> SetRHICfHitPos(x, y, z); // [m]
            mRHICfParticle -> SetMomentum(px, py, pz); // [GeV/c]
            mRHICfParticle -> SetEnergy(e); // [GeV]
            mRHICfParticle -> SetMass(mass); // [MeV/c^2]
        }
    }
}

void StRHICfEventMaker::spinPatternData()
{
    TString dataPath = "StRoot/StRHICfPool/StRHICfEventMaker/SpinPatternData/";
    memset(spinPatterns, 0, sizeof(spinPatterns));

    for(int fill=0; fill<5; fill++){
        for(int beam=0; beam<2; beam++){
            TString beamName = (beam==0)? "blue" : "yellow";
            TString spinDataName = dataPath+beamName+Form("_patterns_%i.dat",fillNumArray[fill]);

            std::ifstream spinDataFile;
            spinDataFile.open(spinDataName, ios::in);
            if(!spinDataFile.is_open()){continue;}
            LOG_INFO << "StRHICfEventMaker::spinPatternData() --- find a " << spinDataName << endm;

            string lines;
            while(getline(spinDataFile, lines)){
                TString line = lines;
                TObjArray *tokens = line.Tokenize(" ");
                TString bunchNum = ((TObjString *) tokens -> At(1)) -> GetString();
                TString pattern = ((TObjString *) tokens -> At(2)) -> GetString();

                spinPatterns[fill][bunchNum.Atoi()-1][beam] = pattern.Atoi();
            }
            spinDataFile.close();
        }
    }
}

Double_t StRHICfEventMaker::beamCenterPosition( int xy, bool metOpt, bool referOpt)
{
    if(mRHICfEvent -> GetRHICfRunType() == kRHICfTL){
        if(mRHICfEvent -> GetFillNumber() == fillNumArray[0]){
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return 2.21;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.66;}
                else if(xy==1){return 1.37;}
            }
        }
        else if(mRHICfEvent -> GetFillNumber() == fillNumArray[1]){
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return 2.31;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.22;}
                else if(xy==1){return 1.55;}
            }
        }
    }
    else if(mRHICfEvent -> GetRHICfRunType() == kRHICfTS){
        if(mRHICfEvent -> GetFillNumber() == fillNumArray[2]){
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return 2.36;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.22;}
                else if(xy==1){return 1.37;}
            }
        }
        else if(mRHICfEvent -> GetFillNumber() == fillNumArray[4]){
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return 2.33;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.22;}
                else if(xy==1){return 1.55;}
            }
        }
    }
    else if(mRHICfEvent -> GetRHICfRunType() == kRHICfTOP){
        if(referOpt == 0){ // refer to fill 21148
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return -21.67;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.22;}
                else if(xy==1){return -22.66;}
            }
        }
        if(referOpt == 1){ // refer to fill 21150
            if(metOpt==0){
                if(xy==0){return 0.;}
                else if(xy==1){return -21.71;}
            }
            else if(metOpt==1){
                if(xy==0){return 0.22;}
                else if(xy==1){return -22.48;}
            }
        }
    }
    return 0.;
}

Int_t  StRHICfEventMaker::eventSummary()
{
    LOG_INFO << "StRHICfEventMaker::eventSummary() -- Saved Event number: " << iEvent << endm;

    // RHICf parts
    int pi0Num = 0;
    int neuNum = 0;
    for(int i=0; i<mRHICfEventDst->GetRHICfParticleNum(); i++){
        mRHICfParticle = mRHICfEventDst->GetRHICfParticle(i);
        int pid = mRHICfParticle -> GetPID();
        if(pid == kPi0){pi0Num++;}
        if(pid == kNeutron){neuNum++;}
    }
    
    LOG_INFO << "* Number of RHICf Detector point: " << mRHICfEventDst->GetRHICfDetPointNum() << endm;
    LOG_INFO << "* RHICf particles, Pi0: " << pi0Num << ", Neutron: " << neuNum << endm;

    // Vertex
    LOG_INFO << "* Primary vertex: (" << mRHICfEvent->GetPrimaryVtxX() << ", " << mRHICfEvent->GetPrimaryVtxY() << ", " <<  mRHICfEvent->GetPrimaryVtxZ() << ") " << endm;

    // Tracks
    LOG_INFO << "* Number of primary track : " << mRHICfEvent->GetPrimaryTrkNum() << ", global track : " << mRHICfEvent->GetGlobalTrkNum() << endm;

    // BTOF
    if(!mOffBTof){LOG_INFO << "* Number of B-TOF Hit: " << mRHICfEventDst -> GetBTofNum() << endm;}

    // BBC
    if(!mOffBBC){
        LOG_INFO << "* BBC Sum of ADC, East(" << mRHICfBBC->GetBBCSmallSumADC(kBeamEast)<< ", " << mRHICfBBC->GetBBCLargeSumADC(kBeamWest) << ") " 
            << "West(" << mRHICfBBC->GetBBCSmallSumADC(1)<< ", " << mRHICfBBC->GetBBCLargeSumADC(1) << ") "<< endm;
    }
    
    // VPD
    if(!mOffVPD){LOG_INFO << "* VPD Sum of ADC, East: " << mRHICfVPD->GetVPDSumADC(kBeamEast) << ", West: " << mRHICfVPD->GetVPDSumADC(kBeamWest) << ", Time Difference: " << mRHICfVPD->GetVPDTimeDiff() << endm;}

    // ZDC
    if(!mOffZDC){LOG_INFO << "* ZDC Sum of ADC, East: " << mRHICfZDC->GetZDCPmtSumADC(kBeamEast) << ", West: " << mRHICfZDC->GetZDCPmtSumADC(kBeamWest) <<", ZDC vtx: "<< mRHICfZDC->GetZDCVtxZ() << endm;}

    // FPS
    if(!mOffFMS){LOG_INFO << "* Number of FMS cluster: " << mRHICfEventDst -> GetFMSNum() << endm;}
    
    // RPS
    if(!mOffRPS){LOG_INFO << "* Number of RPS track: " << mRHICfEventDst -> GetRPSNum() << endm;}

    return 1;
}