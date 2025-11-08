#include "StRHICfEventDst.h"

ClassImp(StRHICfEventDst)

StRHICfEventDst::StRHICfEventDst()
: mOffTPCTrack(false), mOffBTof(false), mOffBBC(false), mOffVPD(false), 
  mOffZDC(false), mOffFMS(false), mOffRPS(false)
{
    InitClonesArray();
}

StRHICfEventDst::~StRHICfEventDst()
{
}

void StRHICfEventDst::Clear(Option_t* option)
{
    if(mEvent){mEvent -> Clear("C");}
    if(mRHICfDetHit){mRHICfDetHit -> Clear("C");}
    if(mRHICfDetPoint){mRHICfDetPoint -> Clear("C");}
    if(mTPCTrack){mTPCTrack -> Clear("C");}
    if(mBTof){mBTof -> Clear("C");}
    if(mBBC){mBBC -> Clear("C");}
    if(mVPD){mVPD -> Clear("C");}
    if(mZDC){mZDC -> Clear("C");}
    if(mFMS){mFMS -> Clear("C");}
    if(mRPS){mRPS -> Clear("C");}
}

void StRHICfEventDst::InitClonesArray()
{
    mEvent = 0;
    mRHICfDetHit = 0;
    mRHICfDetPoint = 0;
    mTPCTrack = 0;
    mBTof = 0;
    mBBC = 0;
    mVPD = 0;
    mZDC = 0;
    mFMS = 0;
    mRPS = 0;
}

Int_t StRHICfEventDst::CreateDstArray(TTree* tree)
{
    mEvent = new TClonesArray("StRHICfEvent");
    mRHICfDetHit = new TClonesArray("StRHICfDetHit");
    mRHICfDetPoint = new TClonesArray("StRHICfDetPoint");
    if(!mOffTPCTrack){mTPCTrack = new TClonesArray("StRHICfTPCTrack");}
    if(!mOffBTof){mBTof = new TClonesArray("StRHICfBTof");}
    if(!mOffBBC){mBBC = new TClonesArray("StRHICfBBC");}
    if(!mOffVPD){mVPD = new TClonesArray("StRHICfVPD");}
    if(!mOffZDC){mZDC = new TClonesArray("StRHICfZDC");}
    if(!mOffFMS){mFMS = new TClonesArray("StRHICfFMS");}
    if(!mOffRPS){mRPS = new TClonesArray("StRHICfRPS");}

    if(mEvent){tree -> Branch("StRHICfEvent", &mEvent);}
    if(mRHICfDetHit){tree -> Branch("StRHICfDetHit", &mRHICfDetHit);}
    if(mRHICfDetPoint){tree -> Branch("StRHICfDetPoint", &mRHICfDetPoint);}
    if(mTPCTrack){tree -> Branch("StRHICfTPCTrack", &mTPCTrack);}
    if(mBTof){tree -> Branch("StRHICfBTof", &mBTof);}
    if(mBBC){tree -> Branch("StRHICfBBC", &mBBC);}
    if(mVPD){tree -> Branch("StRHICfVPD", &mVPD);}
    if(mZDC){tree -> Branch("StRHICfZDC", &mZDC);}
    if(mFMS){tree -> Branch("StRHICfFMS", &mFMS);}
    if(mRPS){tree -> Branch("StRHICfRPS", &mRPS);}

    return 1;
}

Int_t StRHICfEventDst::ReadDstArray(TTree* tree)
{
    if(tree->GetBranchStatus("StRHICfEvent")){tree -> SetBranchAddress("StRHICfEvent", &mEvent);}
    if(tree->GetBranchStatus("StRHICfDetHit")){tree -> SetBranchAddress("StRHICfDetHit", &mRHICfDetHit);}
    if(tree->GetBranchStatus("StRHICfDetPoint")){tree -> SetBranchAddress("StRHICfDetPoint", &mRHICfDetPoint);}
    if(!mOffTPCTrack && tree->GetBranchStatus("StRHICfTPCTrack")){tree -> SetBranchAddress("StRHICfTPCTrack", &mTPCTrack);}
    if(!mOffBTof && tree->GetBranchStatus("StRHICfBTof")){tree -> SetBranchAddress("StRHICfBTof", &mBTof);}
    if(!mOffBBC && tree->GetBranchStatus("StRHICfBBC")){tree -> SetBranchAddress("StRHICfBBC", &mBBC);}
    if(!mOffVPD && tree->GetBranchStatus("StRHICfVPD")){tree -> SetBranchAddress("StRHICfVPD", &mVPD);}
    if(!mOffZDC && tree->GetBranchStatus("StRHICfZDC")){tree -> SetBranchAddress("StRHICfZDC", &mZDC);}
    if(!mOffFMS && tree->GetBranchStatus("StRHICfFMS")){tree -> SetBranchAddress("StRHICfFMS", &mFMS);}
    if(!mOffRPS && tree->GetBranchStatus("StRHICfRPS")){tree -> SetBranchAddress("StRHICfRPS", &mRPS);}

    return 1;
}

Int_t StRHICfEventDst::ReadDstArray(TChain* chain)
{
    if(chain->GetBranchStatus("StRHICfEvent")){chain -> SetBranchAddress("StRHICfEvent", &mEvent);}
    if(chain->GetBranchStatus("StRHICfDetHit")){chain -> SetBranchAddress("StRHICfDetHit", &mRHICfDetHit);}
    if(chain->GetBranchStatus("StRHICfDetPoint")){chain -> SetBranchAddress("StRHICfDetPoint", &mRHICfDetPoint);}
    if(!mOffTPCTrack && chain->GetBranchStatus("StRHICfTPCTrack")){chain -> SetBranchAddress("StRHICfTPCTrack", &mTPCTrack);}
    if(!mOffBTof && chain->GetBranchStatus("StRHICfBTof")){chain -> SetBranchAddress("StRHICfBTof", &mBTof);}
    if(!mOffBBC && chain->GetBranchStatus("StRHICfBBC")){chain -> SetBranchAddress("StRHICfBBC", &mBBC);}
    if(!mOffVPD && chain->GetBranchStatus("StRHICfVPD")){chain -> SetBranchAddress("StRHICfVPD", &mVPD);}
    if(!mOffZDC && chain->GetBranchStatus("StRHICfZDC")){chain -> SetBranchAddress("StRHICfZDC", &mZDC);}
    if(!mOffFMS && chain->GetBranchStatus("StRHICfFMS")){chain -> SetBranchAddress("StRHICfFMS", &mFMS);}
    if(!mOffRPS && chain->GetBranchStatus("StRHICfRPS")){chain -> SetBranchAddress("StRHICfRPS", &mRPS);}

    return 1;
}

void StRHICfEventDst::OffAllSTARDet()
{
    mOffTPCTrack = true;
    mOffBTof = true;
    mOffBBC = true;
    mOffVPD = true;
    mOffZDC = true;
    mOffFMS = true;
    mOffRPS = true;
}

void StRHICfEventDst::OffTPCTrack(){mOffTPCTrack = true;}
void StRHICfEventDst::OffBTof(){mOffBTof = true;}
void StRHICfEventDst::OffBBC(){mOffBBC = true;}
void StRHICfEventDst::OffVPD(){mOffVPD = true;}
void StRHICfEventDst::OffZDC(){mOffZDC = true;}
void StRHICfEventDst::OffFMS(){mOffFMS = true;}
void StRHICfEventDst::OffRPS(){mOffRPS = true;}

StRHICfEvent* StRHICfEventDst::GetEvent(){return (StRHICfEvent*)mEvent->ConstructedAt(0);}
StRHICfDetHit* StRHICfEventDst::GetRHICfDetHit(){return (StRHICfDetHit*)mRHICfDetHit->ConstructedAt(0);}
StRHICfDetPoint* StRHICfEventDst::GetRHICfDetPoint(int idx){return (StRHICfDetPoint*)mRHICfDetPoint->ConstructedAt(idx);}

StRHICfTPCTrack* StRHICfEventDst::GetTPCTrack(int idx)
{
    if(!mTPCTrack){return 0;}
    return (StRHICfTPCTrack*)mTPCTrack->ConstructedAt(idx);
}

StRHICfBTof* StRHICfEventDst::GetBTof(int idx)
{
    if(!mBTof){return 0;}
    return (StRHICfBTof*)mBTof->ConstructedAt(idx);
}

StRHICfBBC* StRHICfEventDst::GetBBC()
{
    if(!mBBC){return 0;}
    return (StRHICfBBC*)mBBC->ConstructedAt(0);
}

StRHICfVPD* StRHICfEventDst::GetVPD()
{
    if(!mVPD){return 0;}
    return (StRHICfVPD*)mVPD->ConstructedAt(0);
}

StRHICfZDC* StRHICfEventDst::GetZDC()
{
    if(!mZDC){return 0;}
    return (StRHICfZDC*)mZDC->ConstructedAt(0);
}

StRHICfFMS* StRHICfEventDst::GetFMS(int idx)
{
    if(!mFMS){return 0;}
    return (StRHICfFMS*)mFMS->ConstructedAt(idx);
}

StRHICfRPS* StRHICfEventDst::GetRPS(int idx)
{
    if(!mRPS){return 0;}
    return (StRHICfRPS*)mRPS->ConstructedAt(idx);
}

Int_t StRHICfEventDst::GetRHICfDetPointNum(){return mRHICfDetPoint->GetEntries();}

Int_t StRHICfEventDst::GetTPCTrackNum()
{
    if(!mTPCTrack){return 0;}
    return mTPCTrack->GetEntries();
}

Int_t StRHICfEventDst::GetBTofNum()
{
    if(!mBTof){return 0;}
    return mBTof->GetEntries();
}

Int_t StRHICfEventDst::GetFMSNum()
{
    if(!mFMS){return 0;}
    return mFMS->GetEntries();
}

Int_t StRHICfEventDst::GetRPSNum()
{
    if(!mRPS){return 0;}
    return mRPS->GetEntries();
}
