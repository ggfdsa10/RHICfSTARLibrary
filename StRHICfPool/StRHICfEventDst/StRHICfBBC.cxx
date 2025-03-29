#include "StRHICfBBC.h"

ClassImp(StRHICfBBC)

StRHICfBBC::StRHICfBBC()
{
    Clear();
}

StRHICfBBC::~StRHICfBBC()
{
}

void StRHICfBBC::Clear(Option_t *option)
{
   std::fill_n(&mBbcSmall[0][0][0], kBeamSideNum*kBBCSmallPmtNum*3, -1);
   std::fill_n(&mBbcLarge[0][0][0], kBeamSideNum*kBBCLargePmtNum*3, -1);
}

void StRHICfBBC::SetBBCSmallADC(int dir, int pmt, int adc){mBbcSmall[dir][pmt][0] = adc;}
void StRHICfBBC::SetBBCSmallTDC(int dir, int pmt, int tdc){mBbcSmall[dir][pmt][1] = tdc;}
void StRHICfBBC::SetBBCSmallTAC(int dir, int pmt, int tac){mBbcSmall[dir][pmt][2] = tac;}

void StRHICfBBC::SetBBCLargeADC(int dir, int pmt, int adc){mBbcLarge[dir][pmt][0] = adc;}
void StRHICfBBC::SetBBCLargeTDC(int dir, int pmt, int tdc){mBbcLarge[dir][pmt][1] = tdc;}
void StRHICfBBC::SetBBCLargeTAC(int dir, int pmt, int tac){mBbcLarge[dir][pmt][2] = tac;}

Int_t StRHICfBBC::GetBBCSmallADC(int dir, int pmt){return mBbcSmall[dir][pmt][0];}
Int_t StRHICfBBC::GetBBCSmallTDC(int dir, int pmt){return mBbcSmall[dir][pmt][1];}
Int_t StRHICfBBC::GetBBCSmallTAC(int dir, int pmt){return mBbcSmall[dir][pmt][2];}

Int_t StRHICfBBC::GetBBCLargeADC(int dir, int pmt){return mBbcLarge[dir][pmt][0];}
Int_t StRHICfBBC::GetBBCLargeTDC(int dir, int pmt){return mBbcLarge[dir][pmt][1];}
Int_t StRHICfBBC::GetBBCLargeTAC(int dir, int pmt){return mBbcLarge[dir][pmt][2];}

Int_t StRHICfBBC::GetBBCSmallSumADC(int dir)
{
    int sumADC = 0;
    for(int i=0; i<kBBCSmallPmtNum; i++){
        sumADC += mBbcSmall[dir][i][0];
    }
    return sumADC;
}

Int_t StRHICfBBC::GetBBCLargeSumADC(int dir)
{
    int sumADC = 0;
    for(int i=0; i<kBBCLargePmtNum; i++){
        sumADC += mBbcLarge[dir][i][0];
    }
    return sumADC;
}


