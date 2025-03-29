#ifndef StRHICfBBC_HH
#define StRHICfBBC_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfBBC : public TObject
{
    public: 
        StRHICfBBC();
        ~StRHICfBBC();

        void Clear(Option_t *option = "");

        void SetBBCSmallADC(int dir, int pmt, int adc);
        void SetBBCSmallTDC(int dir, int pmt, int tdc);
        void SetBBCSmallTAC(int dir, int pmt, int tac);

        void SetBBCLargeADC(int dir, int pmt, int adc);
        void SetBBCLargeTDC(int dir, int pmt, int tdc);
        void SetBBCLargeTAC(int dir, int pmt, int tac);

        Int_t GetBBCSmallADC(int dir, int pmt);
        Int_t GetBBCSmallTDC(int dir, int pmt);
        Int_t GetBBCSmallTAC(int dir, int pmt);

        Int_t GetBBCLargeADC(int dir, int pmt);
        Int_t GetBBCLargeTDC(int dir, int pmt);
        Int_t GetBBCLargeTAC(int dir, int pmt);

        Int_t GetBBCSmallSumADC(int dir);
        Int_t GetBBCLargeSumADC(int dir);

    private:
        Short_t mBbcSmall[kBeamSideNum][kBBCSmallPmtNum][3]; // [east, west][pmt idx][adc, tdc, tac] 
        Short_t mBbcLarge[kBeamSideNum][kBBCLargePmtNum][3]; // [pmt idx][adc, tdc, tac] 

    ClassDef(StRHICfBBC,1)
};

#endif
