#include "StRHICfTPCTrack.h"

ClassImp(StRHICfTPCTrack)

StRHICfTPCTrack::StRHICfTPCTrack()
{
    Clear();
}

StRHICfTPCTrack::~StRHICfTPCTrack()
{
}

void StRHICfTPCTrack::Clear(Option_t *option)
{
    mIsPrimary = false;
    mChi2 = 9999;
    mStatus = 0;

    mOriginPosition[0] = -999.;
    mOriginPosition[1] = -999.;
    mOriginPosition[2] = -999.;
    mMomentum[0] = -999.;
    mMomentum[1] = -999.;
    mMomentum[2] = -999.;
    mNSigmaElectron = std::numeric_limits<short>::min();
    mNSigmaPion = std::numeric_limits<short>::min();
    mNSigmaKaon = std::numeric_limits<short>::min();
    mNSigmaProton = std::numeric_limits<short>::min();
    mProbElectron = -999.;
    mProbPion = -999.;
    mProbKaon = -999.;
    mProbProton = -999.;
    mDca3D = -999.;

    mDedx = -999.;
    mDndx = -999.;
    mNHitDedx = 0;
    mNHitFit = 0;
    mNHitsMax = 0;

    mIsBtofHit = false;
    mBTofMatchFlag = -1;  
    mBtofTOF = -999.;
    mBtofBeta = -999.;
    mBtofNSigmaElectron = std::numeric_limits<short>::min();
    mBtofNSigmaPion = std::numeric_limits<short>::min();
    mBtofNSigmaKaon = std::numeric_limits<short>::min();
    mBtofNSigmaProton = std::numeric_limits<short>::min();

    mIsEtofHit = false;
    mETofMatchFlag = -1;  
    mEtofTOF = -999.;
    mEtofBeta = -999.;

    mIsMtdHit = false;
    mMtdMatchFlag = -1;  
    mMtdDeltaTOF = -999.;
    mMtdBeta = -999.;
}

// track information
void StRHICfTPCTrack::SetIsPrimary(){mIsPrimary = true;}
void StRHICfTPCTrack::SetChi2(UShort_t chi2){mChi2 = chi2;}
void StRHICfTPCTrack::SetStatus(Char_t val){mStatus = val;}

void StRHICfTPCTrack::SetOriginPos(Float_t x, Float_t y, Float_t z)
{
    mOriginPosition[0]= x;
    mOriginPosition[1]= y;
    mOriginPosition[2]= z;
}

void StRHICfTPCTrack::SetMomentum(Float_t px, Float_t py, Float_t pz)
{
    mMomentum[0] = px;  
    mMomentum[1] = py;  
    mMomentum[2] = pz;  
}

void StRHICfTPCTrack::SetSigmaElectron(Float_t sigma)
{
    mNSigmaElectron = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ? 
        ( (sigma > 0) ? std::numeric_limits<short>::max() : 
        std::numeric_limits<short>::min() ) : 
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetSigmaPion(Float_t sigma)
{
    mNSigmaPion = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}
void StRHICfTPCTrack::SetSigmaKaon(Float_t sigma)
{
    mNSigmaKaon = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetSigmaProton(Float_t sigma)
{
    mNSigmaProton = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetProbElectron(Float_t prob){mProbElectron = prob;}
void StRHICfTPCTrack::SetProbPion(Float_t prob){mProbPion = prob;}
void StRHICfTPCTrack::SetProbKaon(Float_t prob){mProbKaon = prob;}
void StRHICfTPCTrack::SetProbProton(Float_t prob){mProbProton = prob;}
void StRHICfTPCTrack::SetDca3D(Float_t val){mDca3D = val;}

Bool_t StRHICfTPCTrack::IsPrimary(){return mIsPrimary;}
UShort_t StRHICfTPCTrack::GetChi2(){return mChi2;}
Char_t StRHICfTPCTrack::GetStatus(){return mStatus;}
Float_t StRHICfTPCTrack::GetOriginPosX(){return mOriginPosition[0];}
Float_t StRHICfTPCTrack::GetOriginPosY(){return mOriginPosition[1];}
Float_t StRHICfTPCTrack::GetOriginPosZ(){return mOriginPosition[2];}
TVector3 StRHICfTPCTrack::GetOriginPos()
{
    TVector3 oritinPos(mOriginPosition[0], mOriginPosition[1], mOriginPosition[2]);
    return oritinPos;
}

Float_t StRHICfTPCTrack::GetMomX(){return mMomentum[0];}
Float_t StRHICfTPCTrack::GetMomY(){return mMomentum[1];}
Float_t StRHICfTPCTrack::GetMomZ(){return mMomentum[2];}
TVector3 StRHICfTPCTrack::GetMomentum()
{
    TVector3 momentum(mMomentum[0], mMomentum[1], mMomentum[2]);
    return momentum;
}

Char_t StRHICfTPCTrack::GetPID()
{
    // test method
    Char_t pid = -1;
    Float_t tmpSigma = (Float_t)std::numeric_limits<short>::max() / 1000.f;
    if(fabs(GetSigmaElectron()) < fabs(tmpSigma)){
        tmpSigma = GetSigmaElectron();
        if(GetCharge() > 0){pid = kPositron;}
        if(GetCharge() < 0){pid = kElectron;}
    }
    if(fabs(GetSigmaPion()) < fabs(tmpSigma)){
        tmpSigma = GetSigmaPion();
        if(GetCharge() > 0){pid = kPlusPion;}
        if(GetCharge() < 0){pid = kMinusPion;}
    }
    if(fabs(GetSigmaKaon()) < fabs(tmpSigma)){
        tmpSigma = GetSigmaKaon();
        if(GetCharge() > 0){pid = kPlusKaon;}
        if(GetCharge() < 0){pid = kMinusKaon;}
    }
    if(fabs(GetSigmaProton()) < fabs(tmpSigma)){
        tmpSigma = GetSigmaProton();
        if(GetCharge() > 0){pid = kProton;}
        if(GetCharge() < 0){pid = kAntiProton;}
    }

    return pid;
}

Float_t StRHICfTPCTrack::GetPt(){return sqrt(mMomentum[0]*mMomentum[0] + mMomentum[1]*mMomentum[1]);}
Float_t StRHICfTPCTrack::GetEta()
{
    double ptot = sqrt(mMomentum[0]*mMomentum[0] + mMomentum[1]*mMomentum[1] + mMomentum[2]*mMomentum[2]);
    double cosTheta = (ptot == 0.0) ? 1.0 : mMomentum[2]/ptot;
    if(cosTheta*cosTheta < 1){return -0.5* TMath::Log( (1.0-cosTheta)/(1.0+cosTheta) );}
    if(mMomentum[2] == 0){return 0;}
    if(mMomentum[2] > 0){return -999.;}
    else{return -999.;}
}

Char_t StRHICfTPCTrack::GetCharge(){return (mNHitFit > 0) ? 1 : -1;}
Float_t StRHICfTPCTrack::GetSigmaElectron(){return (Float_t)mNSigmaElectron / 1000.f;}
Float_t StRHICfTPCTrack::GetSigmaPion(){return (Float_t)mNSigmaPion / 1000.f;}
Float_t StRHICfTPCTrack::GetSigmaKaon(){return (Float_t)mNSigmaKaon / 1000.f;}
Float_t StRHICfTPCTrack::GetSigmaProton(){return (Float_t)mNSigmaProton / 1000.f;}
Float_t StRHICfTPCTrack::GetProbElectron(){return mProbElectron;}
Float_t StRHICfTPCTrack::GetProbPion(){return mProbPion;}
Float_t StRHICfTPCTrack::GetProbKaon(){return mProbKaon;}
Float_t StRHICfTPCTrack::GetProbProton(){return mProbProton;}
Float_t StRHICfTPCTrack::GetDca3D(){return mDca3D;}

// hit information in track
void StRHICfTPCTrack::SetDedx(Float_t val){mDedx = val;}
void StRHICfTPCTrack::SetDndx(Float_t val){mDndx = val;}
void StRHICfTPCTrack::SetnHitDedx(Short_t val){mNHitDedx = val;}
void StRHICfTPCTrack::SetnHitFit(Short_t val){mNHitFit = val;}
void StRHICfTPCTrack::SetnHitMax(Short_t val){mNHitsMax = val;}

Float_t StRHICfTPCTrack::GetDedx(){return mDedx;}
Float_t StRHICfTPCTrack::GetDndx(){return mDndx;}
Short_t StRHICfTPCTrack::GetnHitDedx(){return mNHitDedx;}
Short_t StRHICfTPCTrack::GetnHitFit(){return mNHitFit;}
Short_t StRHICfTPCTrack::GetnHitMax(){return mNHitsMax;}

void StRHICfTPCTrack::SetIsBtofHit(){mIsBtofHit = true;}
void StRHICfTPCTrack::SetBtofMatchFlag(Char_t flag){mBTofMatchFlag = flag;}
void StRHICfTPCTrack::SetBtofTOF(Float_t val){mBtofTOF = val;}
void StRHICfTPCTrack::SetBtofBeta(Float_t val){mBtofBeta = val;}
void StRHICfTPCTrack::SetBtofSigmaElectron(Float_t sigma)
{
    mBtofNSigmaElectron = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetBtofSigmaPion(Float_t sigma)
{
    mBtofNSigmaPion = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetBtofSigmaKaon(Float_t sigma)
{
    mBtofNSigmaKaon = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

void StRHICfTPCTrack::SetBtofSigmaProton(Float_t sigma)
{
    mBtofNSigmaProton = ( fabs(sigma * 1000.) > std::numeric_limits<short>::max() ?
        ( (sigma > 0) ? std::numeric_limits<short>::max() :
        std::numeric_limits<short>::min() ) :
        (Short_t)( TMath::Nint( sigma * 1000. ) ) );
}

Bool_t StRHICfTPCTrack::IsBtofHit(){return mIsBtofHit;}
Char_t StRHICfTPCTrack::GetBtofMatchFlag(){return mBTofMatchFlag;}
Float_t StRHICfTPCTrack::GetBtofTOF(){return mBtofTOF;}
Float_t StRHICfTPCTrack::GetBtofBeta(){return mBtofBeta;}
Float_t StRHICfTPCTrack::GetBtofSigmaElectron(){return (Float_t)mBtofNSigmaElectron / 1000.f;}
Float_t StRHICfTPCTrack::GetBtofSigmaPion(){return (Float_t)mBtofNSigmaPion / 1000.f;}
Float_t StRHICfTPCTrack::GetBtofSigmaKaon(){return (Float_t)mBtofNSigmaKaon / 1000.f;}
Float_t StRHICfTPCTrack::GetBtofSigmaProton(){return (Float_t)mBtofNSigmaProton / 1000.f;}

// etof traits
void StRHICfTPCTrack::SetIsEtofHit(){mIsEtofHit = true;}
void StRHICfTPCTrack::SetEtofMatchFlag(Char_t flag){mETofMatchFlag =flag;}
void StRHICfTPCTrack::SetEtofTOF(Float_t val){mEtofTOF = val;}
void StRHICfTPCTrack::SetEtofBeta(Float_t val){mEtofBeta = val;}

Bool_t StRHICfTPCTrack::IsEtofHit(){return mIsEtofHit;}
Char_t StRHICfTPCTrack::GetEtofMatchFlag(){return mETofMatchFlag;}
Float_t StRHICfTPCTrack::GetEtofTOF(){return mEtofTOF;}
Float_t StRHICfTPCTrack::GetEtofBeta(){return mEtofBeta;}

// mtd traits
void StRHICfTPCTrack::SetIsMtdHit(){mIsMtdHit = true;}
void StRHICfTPCTrack::SetMtdMatchFlag(Char_t flag){mMtdMatchFlag = flag;}
void StRHICfTPCTrack::SetMtdDeltaTOF(Float_t val){mMtdDeltaTOF = val;}
void StRHICfTPCTrack::SetMtdBeta(Float_t val){mMtdBeta = val;}

Bool_t StRHICfTPCTrack::IsMtdHit(){return mIsMtdHit;}
Char_t StRHICfTPCTrack::GetMtdMatchFlag(){return mMtdMatchFlag;}
Float_t StRHICfTPCTrack::GetMtdDeltaTOF(){return mMtdDeltaTOF;}
Float_t StRHICfTPCTrack::GetMtdBeta(){return mMtdBeta;}
