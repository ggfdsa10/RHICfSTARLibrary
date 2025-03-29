#include "StRHICfBTof.h"

ClassImp(StRHICfBTof)

StRHICfBTof::StRHICfBTof()
{
    Clear();
}

StRHICfBTof::~StRHICfBTof()
{
}

void StRHICfBTof::Clear(Option_t *option)
{
    mIsVPD = false;
    mTray = 0; 
    mModule = 0; 
    mCell = 0; 
}

void StRHICfBTof::SetIsVPD(){mIsVPD = true;}
void StRHICfBTof::SetTray(int idx){mTray = idx;}
void StRHICfBTof::SetModule(int idx){mModule = idx;}
void StRHICfBTof::SetCell(int idx){mCell = idx;}

Bool_t StRHICfBTof::IsVPD(){return mIsVPD;}
Int_t StRHICfBTof::GetTray(){return mTray;}
Int_t StRHICfBTof::GetModule(){return mModule;}
Int_t StRHICfBTof::GetCell(){return mCell;}
Int_t StRHICfBTof::GetGlobalCell(){return (mTray-1)*192 + (mModule-1)*6 + (mCell-1);}
