#ifndef StRHICfFMS_HH
#define StRHICfFMS_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfFMS : public TObject
{
    public: 
        StRHICfFMS();
        ~StRHICfFMS();

        void Clear(Option_t *option = "");

        void SetCategory(int idx);
        void SetFMSHitPos(double x, double y, double z);
        void SetMomentum(double px, double py, double pz);
        void SetEnergy(double e);
        
        Int_t GetCategory();
        Double_t GetFMSHitPosX();
        Double_t GetFMSHitPosY();
        Double_t GetFMSHitPosZ();
        Double_t GetPx();
        Double_t GetPy();
        Double_t GetPz();
        Double_t GetEnergy();

    private:
        Char_t mFmsCategory; // 0 = not fitted cluster, 1 = one photon, 2 = two photon in cluster, 3 = invalid
        Double_t mPosition[3];
        Double_t mMomentum[3];
        Double_t mEnergy;

    ClassDef(StRHICfFMS,1)
};

#endif
