#ifndef StRHICfTPCTrack_HH
#define StRHICfTPCTrack_HH

#include "TObject.h"
#include "TVector3.h"
#include "TMath.h"
#include "StRHICfParameters.h"

class StRHICfTPCTrack : public TObject
{
    public: 
        StRHICfTPCTrack();
        ~StRHICfTPCTrack();

        virtual void Clear(Option_t *option = "");

        // track information
        void SetIsPrimary();
        void SetChi2(UShort_t chi2);
        void SetStatus(Char_t val);
        void SetOriginPos(Float_t x, Float_t y, Float_t z);
        void SetMomentum(Float_t px, Float_t py, Float_t pz);
        void SetSigmaElectron(Float_t sigma);
        void SetSigmaPion(Float_t sigma);
        void SetSigmaKaon(Float_t sigma);
        void SetSigmaProton(Float_t sigma);
        void SetProbElectron(Float_t prob);
        void SetProbPion(Float_t prob);
        void SetProbKaon(Float_t prob);
        void SetProbProton(Float_t prob);
        void SetDca3D(Float_t val);

        Bool_t IsPrimary();
        UShort_t GetChi2();
        Char_t GetStatus();
        Float_t GetOriginPosX();
        Float_t GetOriginPosY();
        Float_t GetOriginPosZ();
        TVector3 GetOriginPos();
        Float_t GetMomX();
        Float_t GetMomY();
        Float_t GetMomZ();
        TVector3 GetMomentum();

        Char_t GetPID();
        Float_t GetPt();
        Float_t GetEta();

        Char_t GetCharge();
        Float_t GetSigmaElectron();
        Float_t GetSigmaPion();
        Float_t GetSigmaKaon();
        Float_t GetSigmaProton();
        Float_t GetProbElectron();
        Float_t GetProbPion();
        Float_t GetProbKaon();
        Float_t GetProbProton();
        Float_t GetDca3D();

        // hit information in track
        void SetDedx(Float_t val);
        void SetDndx(Float_t val);
        void SetnHitDedx(Short_t val);
        void SetnHitFit(Short_t val);
        void SetnHitMax(Short_t val);

        Float_t GetDedx();
        Float_t GetDndx();
        Short_t GetnHitDedx();
        Short_t GetnHitFit();
        Short_t GetnHitMax();

        // btof traits
        void SetIsBtofHit();
        void SetBtofMatchFlag(Char_t flag);
        void SetBtofTOF(Float_t val);
        void SetBtofBeta(Float_t val);
        void SetBtofSigmaElectron(Float_t val);
        void SetBtofSigmaPion(Float_t val);
        void SetBtofSigmaKaon(Float_t val);
        void SetBtofSigmaProton(Float_t val);

        Bool_t IsBtofHit();
        Char_t GetBtofMatchFlag();
        Float_t GetBtofTOF();
        Float_t GetBtofBeta();
        Float_t GetBtofSigmaElectron();
        Float_t GetBtofSigmaPion();
        Float_t GetBtofSigmaKaon();
        Float_t GetBtofSigmaProton();

        // etof traits
        void SetIsEtofHit();
        void SetEtofMatchFlag(Char_t flag);
        void SetEtofTOF(Float_t val);
        void SetEtofBeta(Float_t val);

        Bool_t IsEtofHit();
        Char_t GetEtofMatchFlag();
        Float_t GetEtofTOF();
        Float_t GetEtofBeta();

        // etof traits
        void SetIsMtdHit();
        void SetMtdMatchFlag(Char_t flag);
        void SetMtdDeltaTOF(Float_t val);
        void SetMtdBeta(Float_t val);

        Bool_t IsMtdHit();
        Char_t GetMtdMatchFlag();
        Float_t GetMtdDeltaTOF();
        Float_t GetMtdBeta();

    private:
        Bool_t mIsPrimary;
        UShort_t mChi2;  // Chi2 of the track (encoding = chi2*1000)
        Char_t mStatus; // Checks if the track was fitted to any vertex, 0 = not fitted, 1 = fitted
        
        Float_t mOriginPosition[3];
        Float_t mMomentum[3];

        Short_t mNSigmaElectron;
        Short_t mNSigmaPion;
        Short_t mNSigmaKaon;
        Short_t mNSigmaProton;

        Float_t mProbElectron;
        Float_t mProbPion;
        Float_t mProbKaon;
        Float_t mProbProton;

        Float_t mDca3D;
        Float_t mDedx;
        Float_t mDndx;

        Short_t mNHitDedx;
        Short_t mNHitFit;
        Short_t mNHitsMax;

        Bool_t mIsBtofHit;
        Char_t mBTofMatchFlag;  /// 0 - no match, 1 - one-to-one, 2 - one-to-multiple
        Float_t mBtofTOF;
        Float_t mBtofBeta;
        Short_t mBtofNSigmaElectron;
        Short_t mBtofNSigmaPion;
        Short_t mBtofNSigmaKaon;
        Short_t mBtofNSigmaProton;

        Bool_t mIsEtofHit;
        Char_t mETofMatchFlag;  /// 0 - no match, 1 - one-to-one, 2 - one-to-multiple
        Float_t mEtofTOF;
        Float_t mEtofBeta;
 
        Bool_t mIsMtdHit;
        Char_t mMtdMatchFlag;  /// 0 - no match, 1 - one-to-one, 2 - one-to-multiple
        Float_t mMtdDeltaTOF;
        Float_t mMtdBeta;

    ClassDef(StRHICfTPCTrack,1);
};

#endif
