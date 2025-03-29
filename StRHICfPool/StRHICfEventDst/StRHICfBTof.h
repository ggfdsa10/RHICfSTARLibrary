#ifndef StRHICfBTof_HH
#define StRHICfBTof_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfBTof : public TObject
{
    public: 
        StRHICfBTof();
        ~StRHICfBTof();

        void Clear(Option_t *option = "");

        void SetIsVPD();
        void SetTray(int idx);
        void SetModule(int idx);
        void SetCell(int idx);

        Bool_t IsVPD(); // is VPD cell
        Int_t GetTray(); 
        Int_t GetModule(); 
        Int_t GetCell();
        Int_t GetGlobalCell(); // global cell index for all B-TOF tower

    private:
        Bool_t mIsVPD;
        UChar_t mTray; // StMuBTofHit tray()
        UChar_t mModule; // StMuBTofHit module()
        UChar_t mCell;  // StMuBTofHit cell()

    ClassDef(StRHICfBTof,1)
};

#endif
