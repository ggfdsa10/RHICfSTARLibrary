#ifndef StRHICfParticle_HH
#define StRHICfParticle_HH

#include "TMath.h"
#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfParticle : public TObject
{
    public: 
        StRHICfParticle();
        ~StRHICfParticle();

        void Clear(Option_t *option = "");

        void SetPID(int pid);
        void SetPi0Type(int type);
        void SetTowerIdx(int idx);
        void SetRHICfHitPos(double x, double y, double z);
        void SetMomentum(double px, double py, double pz);
        void SetEnergy(double e);
        void SetMass(double mass);

        Int_t GetPID();
        Int_t GetPi0Type();
        Int_t GetTowerIdx();
        Double_t GetRHICfHitPosX();
        Double_t GetRHICfHitPosY();
        Double_t GetRHICfHitPosZ();
        Double_t GetPx();
        Double_t GetPy();
        Double_t GetPz();
        Double_t GetEnergy();
        Double_t GetMass();
        Double_t GetPt();
        Double_t GetXf();

    private:
        Char_t mPID;
        Char_t mPi0Type; // 1 = Pi0 type1, 2 = Pi0 type2, -1 =  Neutron case
        Char_t mTowerIdx; // 0 = TS, 1 = TL, -1 = type1 pi0 case, (type2 pi0 has tower idx)
        Double_t mPosition[3]; // [mm] at the RHICf detector surface 18000 mm
        Double_t mMomentum[3]; // [GeV/c]
        Double_t mEnergy; // [GeV]
        Double_t mMass; // invariant mass [MeV/c^2]

    ClassDef(StRHICfParticle,1)
};

#endif
