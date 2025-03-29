#ifndef StRHICfRPS_HH
#define StRHICfRPS_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfRPS : public TObject
{
    public: 
        StRHICfRPS();
        ~StRHICfRPS();

        void Clear(Option_t *option = "");

        void SetUsedPlane(int plane);
        void SetTrkType(int type);
        void SetTrkBranch(int idx);
        void SetRPSHitPos(int point, double x, double y, double z);
        void SetMomentum(double px, double py, double pz);


        Int_t GetUsedPlane();
        Int_t GetTrkType();
        Int_t GetTrkBranch();
        Double_t GetRPSHitPosX(int point);
        Double_t GetRPSHitPosY(int point);
        Double_t GetRPSHitPosZ(int point);
        Double_t GetPx();
        Double_t GetPy();
        Double_t GetPz();

    private:
        Char_t mRpsUsedPlane; // for track quality
        Char_t mRpsTrkType; // 0 = reco. only one point (local) , 1 = reco. using two point (global) , 2 = undefined
        Char_t mRpsTrkBranch; // Rps branch detector, 0 = EU, 1 = ED, 2 = WU, 3 = WD 
        Double_t mPosition[kRPSPointNum][3]; // [point][x, y, z]
        Double_t mMomentum[3];

    ClassDef(StRHICfRPS,1)
};

#endif
