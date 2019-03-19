#define CountingTests_cxx
#include "CountingTests.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <TMatrixT.h>

#include <iostream>

typedef TMatrixT<double>        TMatrixD;

void CountingTests::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L CountingTests.C
//      Root > CountingTests t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   
   double epsilonMu[3] = {0.85,0.9,0.95};
   double epsilonH1[3] = {0.35,0.5,0.65};
   double epsilonH2[3] = {0.35,0.5,0.65};
   
   TFile* outfile = new TFile("CountingTests.root", "RECREATE");
   
   TH1D* hMassBarrel = new TH1D("hMassBarrel","hMassBarrel",25,4.9,5.9);
   TH1D* hMassEndcap = new TH1D("hMassEndcap","hMassEndcap",25,4.9,5.9);
   TH1D* hMassMVABarrel = new TH1D("hMassMVABarrel","hMassMVABarrel",25,4.9,5.9);
   TH1D* hMassMVAEndcap = new TH1D("hMassMVAEndcap","hMassMVAEndcap",25,4.9,5.9);
   TH1D* hMassFail1Barrel = new TH1D("hMassFail1Barrel","hMassFail1Barrel",25,4.9,5.9);
   TH1D* hMassFail1Endcap = new TH1D("hMassFail1Endcap","hMassFail1Endcap",25,4.9,5.9);
   TH1D* hMassFail2Barrel = new TH1D("hMassFail2Barrel","hMassFail2Barrel",25,4.9,5.9);
   TH1D* hMassFail2Endcap = new TH1D("hMassFail2Endcap","hMassFail2Endcap",25,4.9,5.9);

   TH1D* hMassBarrelCat1 = new TH1D("hMassBarrelCat1","hMassBarrelCat1",25,4.9,5.9);
   TH1D* hMassEndcapCat1 = new TH1D("hMassEndcapCat1","hMassEndcapCat1",25,4.9,5.9);
   TH1D* hMassMVABarrelCat1 = new TH1D("hMassMVABarrelCat1","hMassMVABarrelCat1",25,4.9,5.9);
   TH1D* hMassMVAEndcapCat1 = new TH1D("hMassMVAEndcapCat1","hMassMVAEndcapCat1",25,4.9,5.9);
   TH1D* hMassFail1BarrelCat1 = new TH1D("hMassFail1BarrelCat1","hMassFail1BarrelCat1",25,4.9,5.9);
   TH1D* hMassFail1EndcapCat1 = new TH1D("hMassFail1EndcapCat1","hMassFail1EndcapCat1",25,4.9,5.9);
   TH1D* hMassFail2BarrelCat1 = new TH1D("hMassFail2BarrelCat1","hMassFail2BarrelCat1",25,4.9,5.9);
   TH1D* hMassFail2EndcapCat1 = new TH1D("hMassFail2EndcapCat1","hMassFail2EndcapCat1",25,4.9,5.9);

   TH1D* hMassBarrelCat2 = new TH1D("hMassBarrelCat2","hMassBarrelCat2",25,4.9,5.9);
   TH1D* hMassEndcapCat2 = new TH1D("hMassEndcapCat2","hMassEndcapCat2",25,4.9,5.9);
   TH1D* hMassMVABarrelCat2 = new TH1D("hMassMVABarrelCat2","hMassMVABarrelCat2",25,4.9,5.9);
   TH1D* hMassMVAEndcapCat2 = new TH1D("hMassMVAEndcapCat2","hMassMVAEndcapCat2",25,4.9,5.9);
   TH1D* hMassFail1BarrelCat2 = new TH1D("hMassFail1BarrelCat2","hMassFail1BarrelCat2",25,4.9,5.9);
   TH1D* hMassFail1EndcapCat2 = new TH1D("hMassFail1EndcapCat2","hMassFail1EndcapCat2",25,4.9,5.9);
   TH1D* hMassFail2BarrelCat2 = new TH1D("hMassFail2BarrelCat2","hMassFail2BarrelCat2",25,4.9,5.9);
   TH1D* hMassFail2EndcapCat2 = new TH1D("hMassFail2EndcapCat2","hMassFail2EndcapCat2",25,4.9,5.9);

   TH1D* hMassBarrelCat3 = new TH1D("hMassBarrelCat3","hMassBarrelCat3",25,4.9,5.9);
   TH1D* hMassEndcapCat3 = new TH1D("hMassEndcapCat3","hMassEndcapCat3",25,4.9,5.9);
   TH1D* hMassMVABarrelCat3 = new TH1D("hMassMVABarrelCat3","hMassMVABarrelCat3",25,4.9,5.9);
   TH1D* hMassMVAEndcapCat3 = new TH1D("hMassMVAEndcapCat3","hMassMVAEndcapCat3",25,4.9,5.9);
   TH1D* hMassFail1BarrelCat3 = new TH1D("hMassFail1BarrelCat3","hMassFail1BarrelCat3",25,4.9,5.9);
   TH1D* hMassFail1EndcapCat3 = new TH1D("hMassFail1EndcapCat3","hMassFail1EndcapCat3",25,4.9,5.9);
   TH1D* hMassFail2BarrelCat3 = new TH1D("hMassFail2BarrelCat3","hMassFail2BarrelCat3",25,4.9,5.9);
   TH1D* hMassFail2EndcapCat3 = new TH1D("hMassFail2EndcapCat3","hMassFail2EndcapCat3",25,4.9,5.9);

   TH1D* hMassBarrelCat4 = new TH1D("hMassBarrelCat4","hMassBarrelCat4",25,4.9,5.9);
   TH1D* hMassEndcapCat4 = new TH1D("hMassEndcapCat4","hMassEndcapCat4",25,4.9,5.9);
   TH1D* hMassMVABarrelCat4 = new TH1D("hMassMVABarrelCat4","hMassMVABarrelCat4",25,4.9,5.9);
   TH1D* hMassMVAEndcapCat4 = new TH1D("hMassMVAEndcapCat4","hMassMVAEndcapCat4",25,4.9,5.9);
   TH1D* hMassFail1BarrelCat4 = new TH1D("hMassFail1BarrelCat4","hMassFail1BarrelCat4",25,4.9,5.9);
   TH1D* hMassFail1EndcapCat4 = new TH1D("hMassFail1EndcapCat4","hMassFail1EndcapCat4",25,4.9,5.9);
   TH1D* hMassFail2BarrelCat4 = new TH1D("hMassFail2BarrelCat4","hMassFail2BarrelCat4",25,4.9,5.9);
   TH1D* hMassFail2EndcapCat4 = new TH1D("hMassFail2EndcapCat4","hMassFail2EndcapCat4",25,4.9,5.9);
   
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     
     if(fabs(m1eta)<1.4 && fabs(m2eta)<1.4)
     {
       if(bdt>0.36)
       {
         hMassBarrel->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVABarrel->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1Barrel->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2Barrel->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       if(bdt>0.1&&bdt<=0.23)
       {
         hMassBarrelCat1->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVABarrelCat1->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1BarrelCat1->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2BarrelCat1->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>0.23&&bdt<=0.33)
       {
         hMassBarrelCat2->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVABarrelCat2->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1BarrelCat2->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2BarrelCat2->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>0.33&&bdt<=0.44)
       {
         hMassBarrelCat3->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVABarrelCat3->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1BarrelCat3->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2BarrelCat3->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>0.44&&bdt<=1)
       {
         hMassBarrelCat4->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVABarrelCat4->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1BarrelCat4->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2BarrelCat4->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
     }
     else
     {
       if(bdt>0.38)
       {
         hMassEndcap->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVAEndcap->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1Endcap->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2Endcap->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       if(bdt>0.1&&bdt<=0.22)
       {
         hMassEndcapCat1->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVAEndcapCat1->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1EndcapCat1->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2EndcapCat1->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>0.22&&bdt<0.33)
       {
         hMassEndcapCat2->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVAEndcapCat2->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1EndcapCat2->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2EndcapCat2->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>=0.33&&bdt<0.45)
       {
         hMassEndcapCat3->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVAEndcapCat3->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1EndcapCat3->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2EndcapCat3->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
       else if(bdt>=0.45&&bdt<1)
       {
         hMassEndcapCat4->Fill(m);
         if(m1bdt>0.36&&m2bdt>0.36)
         {
           hMassMVAEndcapCat4->Fill(m);
         }
         else if((m1bdt>0.36&&m2bdt<=0.36)||(m1bdt<=0.36&&m2bdt>0.36))
         {
           hMassFail1EndcapCat4->Fill(m);
         }
         else if(m1bdt<=0.36&&m2bdt<=0.36)
         {
           hMassFail2EndcapCat4->Fill(m);
         }
         else
         {
           std::cout << "We should never be here!\n";
         }
       }
     }
   }
   
   int minN0BarrelAll=99999;
   int minN1BarrelAll=99999;
   int minN2BarrelAll=99999;
   double minnMuMuBarrelAll = 99999;
   double minnMuHBarrelAll = 99999;
   double minnHHBarrelAll = 99999;
   double minnMuMuMVABarrelAll = 99999;
   double minnMuHMVABarrelAll = 99999;
   double minnHHMVABarrelAll = 99999;
   int maxN0BarrelAll=-99999;
   int maxN1BarrelAll=-99999;
   int maxN2BarrelAll=-99999;
   double maxnMuMuBarrelAll = -99999;
   double maxnMuHBarrelAll = -99999;
   double maxnHHBarrelAll = -99999;
   double maxnMuMuMVABarrelAll = -99999;
   double maxnMuHMVABarrelAll = -99999;
   double maxnHHMVABarrelAll = -99999;
   
   int minN0BarrelLSB=99999;
   int minN1BarrelLSB=99999;
   int minN2BarrelLSB=99999;
   double minnMuMuBarrelLSB = 99999;
   double minnMuHBarrelLSB = 99999;
   double minnHHBarrelLSB = 99999;
   double minnMuMuMVABarrelLSB = 99999;
   double minnMuHMVABarrelLSB = 99999;
   double minnHHMVABarrelLSB = 99999;
   int maxN0BarrelLSB=-99999;
   int maxN1BarrelLSB=-99999;
   int maxN2BarrelLSB=-99999;
   double maxnMuMuBarrelLSB = -99999;
   double maxnMuHBarrelLSB = -99999;
   double maxnHHBarrelLSB = -99999;
   double maxnMuMuMVABarrelLSB = -99999;
   double maxnMuHMVABarrelLSB = -99999;
   double maxnHHMVABarrelLSB = -99999;
   
   int minN0BarrelSig=99999;
   int minN1BarrelSig=99999;
   int minN2BarrelSig=99999;
   double minnMuMuBarrelSig = 99999;
   double minnMuHBarrelSig = 99999;
   double minnHHBarrelSig = 99999;
   double minnMuMuMVABarrelSig = 99999;
   double minnMuHMVABarrelSig = 99999;
   double minnHHMVABarrelSig = 99999;
   int maxN0BarrelSig=-99999;
   int maxN1BarrelSig=-99999;
   int maxN2BarrelSig=-99999;
   double maxnMuMuBarrelSig = -99999;
   double maxnMuHBarrelSig = -99999;
   double maxnHHBarrelSig = -99999;
   double maxnMuMuMVABarrelSig = -99999;
   double maxnMuHMVABarrelSig = -99999;
   double maxnHHMVABarrelSig = -99999;
   
   int minN0BarrelRSB=99999;
   int minN1BarrelRSB=99999;
   int minN2BarrelRSB=99999;
   double minnMuMuBarrelRSB = 99999;
   double minnMuHBarrelRSB = 99999;
   double minnHHBarrelRSB = 99999;
   double minnMuMuMVABarrelRSB = 99999;
   double minnMuHMVABarrelRSB = 99999;
   double minnHHMVABarrelRSB = 99999;
   int maxN0BarrelRSB=-99999;
   int maxN1BarrelRSB=-99999;
   int maxN2BarrelRSB=-99999;
   double maxnMuMuBarrelRSB = -99999;
   double maxnMuHBarrelRSB = -99999;
   double maxnHHBarrelRSB = -99999;
   double maxnMuMuMVABarrelRSB = -99999;
   double maxnMuHMVABarrelRSB = -99999;
   double maxnHHMVABarrelRSB = -99999;
   
   int minN0EndcapAll=99999;
   int minN1EndcapAll=99999;
   int minN2EndcapAll=99999;
   double minnMuMuEndcapAll = 99999;
   double minnMuHEndcapAll = 99999;
   double minnHHEndcapAll = 99999;
   double minnMuMuMVAEndcapAll = 99999;
   double minnMuHMVAEndcapAll = 99999;
   double minnHHMVAEndcapAll = 99999;
   int maxN0EndcapAll=-99999;
   int maxN1EndcapAll=-99999;
   int maxN2EndcapAll=-99999;
   double maxnMuMuEndcapAll = -99999;
   double maxnMuHEndcapAll = -99999;
   double maxnHHEndcapAll = -99999;
   double maxnMuMuMVAEndcapAll = -99999;
   double maxnMuHMVAEndcapAll = -99999;
   double maxnHHMVAEndcapAll = -99999;
   
   int minN0EndcapLSB=99999;
   int minN1EndcapLSB=99999;
   int minN2EndcapLSB=99999;
   double minnMuMuEndcapLSB = 99999;
   double minnMuHEndcapLSB = 99999;
   double minnHHEndcapLSB = 99999;
   double minnMuMuMVAEndcapLSB = 99999;
   double minnMuHMVAEndcapLSB = 99999;
   double minnHHMVAEndcapLSB = 99999;
   int maxN0EndcapLSB=-99999;
   int maxN1EndcapLSB=-99999;
   int maxN2EndcapLSB=-99999;
   double maxnMuMuEndcapLSB = -99999;
   double maxnMuHEndcapLSB = -99999;
   double maxnHHEndcapLSB = -99999;
   double maxnMuMuMVAEndcapLSB = -99999;
   double maxnMuHMVAEndcapLSB = -99999;
   double maxnHHMVAEndcapLSB = -99999;
   
   int minN0EndcapSig=99999;
   int minN1EndcapSig=99999;
   int minN2EndcapSig=99999;
   double minnMuMuEndcapSig = 99999;
   double minnMuHEndcapSig = 99999;
   double minnHHEndcapSig = 99999;
   double minnMuMuMVAEndcapSig = 99999;
   double minnMuHMVAEndcapSig = 99999;
   double minnHHMVAEndcapSig = 99999;
   int maxN0EndcapSig=-99999;
   int maxN1EndcapSig=-99999;
   int maxN2EndcapSig=-99999;
   double maxnMuMuEndcapSig = -99999;
   double maxnMuHEndcapSig = -99999;
   double maxnHHEndcapSig = -99999;
   double maxnMuMuMVAEndcapSig = -99999;
   double maxnMuHMVAEndcapSig = -99999;
   double maxnHHMVAEndcapSig = -99999;
   
   int minN0EndcapRSB=99999;
   int minN1EndcapRSB=99999;
   int minN2EndcapRSB=99999;
   double minnMuMuEndcapRSB = 99999;
   double minnMuHEndcapRSB = 99999;
   double minnHHEndcapRSB = 99999;
   double minnMuMuMVAEndcapRSB = 99999;
   double minnMuHMVAEndcapRSB = 99999;
   double minnHHMVAEndcapRSB = 99999;
   int maxN0EndcapRSB=-99999;
   int maxN1EndcapRSB=-99999;
   int maxN2EndcapRSB=-99999;
   double maxnMuMuEndcapRSB = -99999;
   double maxnMuHEndcapRSB = -99999;
   double maxnHHEndcapRSB = -99999;
   double maxnMuMuMVAEndcapRSB = -99999;
   double maxnMuHMVAEndcapRSB = -99999;
   double maxnHHMVAEndcapRSB = -99999;
   
   int minN0BarrelAllCat1=99999;
   int minN1BarrelAllCat1=99999;
   int minN2BarrelAllCat1=99999;
   double minnMuMuBarrelAllCat1 = 99999;
   double minnMuHBarrelAllCat1 = 99999;
   double minnHHBarrelAllCat1 = 99999;
   double minnMuMuMVABarrelAllCat1 = 99999;
   double minnMuHMVABarrelAllCat1 = 99999;
   double minnHHMVABarrelAllCat1 = 99999;
   int maxN0BarrelAllCat1=-99999;
   int maxN1BarrelAllCat1=-99999;
   int maxN2BarrelAllCat1=-99999;
   double maxnMuMuBarrelAllCat1 = -99999;
   double maxnMuHBarrelAllCat1 = -99999;
   double maxnHHBarrelAllCat1 = -99999;
   double maxnMuMuMVABarrelAllCat1 = -99999;
   double maxnMuHMVABarrelAllCat1 = -99999;
   double maxnHHMVABarrelAllCat1 = -99999;
   
   int minN0BarrelLSBCat1=99999;
   int minN1BarrelLSBCat1=99999;
   int minN2BarrelLSBCat1=99999;
   double minnMuMuBarrelLSBCat1 = 99999;
   double minnMuHBarrelLSBCat1 = 99999;
   double minnHHBarrelLSBCat1 = 99999;
   double minnMuMuMVABarrelLSBCat1 = 99999;
   double minnMuHMVABarrelLSBCat1 = 99999;
   double minnHHMVABarrelLSBCat1 = 99999;
   int maxN0BarrelLSBCat1=-99999;
   int maxN1BarrelLSBCat1=-99999;
   int maxN2BarrelLSBCat1=-99999;
   double maxnMuMuBarrelLSBCat1 = -99999;
   double maxnMuHBarrelLSBCat1 = -99999;
   double maxnHHBarrelLSBCat1 = -99999;
   double maxnMuMuMVABarrelLSBCat1 = -99999;
   double maxnMuHMVABarrelLSBCat1 = -99999;
   double maxnHHMVABarrelLSBCat1 = -99999;
   
   int minN0BarrelSigCat1=99999;
   int minN1BarrelSigCat1=99999;
   int minN2BarrelSigCat1=99999;
   double minnMuMuBarrelSigCat1 = 99999;
   double minnMuHBarrelSigCat1 = 99999;
   double minnHHBarrelSigCat1 = 99999;
   double minnMuMuMVABarrelSigCat1 = 99999;
   double minnMuHMVABarrelSigCat1 = 99999;
   double minnHHMVABarrelSigCat1 = 99999;
   int maxN0BarrelSigCat1=-99999;
   int maxN1BarrelSigCat1=-99999;
   int maxN2BarrelSigCat1=-99999;
   double maxnMuMuBarrelSigCat1 = -99999;
   double maxnMuHBarrelSigCat1 = -99999;
   double maxnHHBarrelSigCat1 = -99999;
   double maxnMuMuMVABarrelSigCat1 = -99999;
   double maxnMuHMVABarrelSigCat1 = -99999;
   double maxnHHMVABarrelSigCat1 = -99999;
   
   int minN0BarrelRSBCat1=99999;
   int minN1BarrelRSBCat1=99999;
   int minN2BarrelRSBCat1=99999;
   double minnMuMuBarrelRSBCat1 = 99999;
   double minnMuHBarrelRSBCat1 = 99999;
   double minnHHBarrelRSBCat1 = 99999;
   double minnMuMuMVABarrelRSBCat1 = 99999;
   double minnMuHMVABarrelRSBCat1 = 99999;
   double minnHHMVABarrelRSBCat1 = 99999;
   int maxN0BarrelRSBCat1=-99999;
   int maxN1BarrelRSBCat1=-99999;
   int maxN2BarrelRSBCat1=-99999;
   double maxnMuMuBarrelRSBCat1 = -99999;
   double maxnMuHBarrelRSBCat1 = -99999;
   double maxnHHBarrelRSBCat1 = -99999;
   double maxnMuMuMVABarrelRSBCat1 = -99999;
   double maxnMuHMVABarrelRSBCat1 = -99999;
   double maxnHHMVABarrelRSBCat1 = -99999;

   int minN0EndcapAllCat1=99999;
   int minN1EndcapAllCat1=99999;
   int minN2EndcapAllCat1=99999;
   double minnMuMuEndcapAllCat1 = 99999;
   double minnMuHEndcapAllCat1 = 99999;
   double minnHHEndcapAllCat1 = 99999;
   double minnMuMuMVAEndcapAllCat1 = 99999;
   double minnMuHMVAEndcapAllCat1 = 99999;
   double minnHHMVAEndcapAllCat1 = 99999;
   int maxN0EndcapAllCat1=-99999;
   int maxN1EndcapAllCat1=-99999;
   int maxN2EndcapAllCat1=-99999;
   double maxnMuMuEndcapAllCat1 = -99999;
   double maxnMuHEndcapAllCat1 = -99999;
   double maxnHHEndcapAllCat1 = -99999;
   double maxnMuMuMVAEndcapAllCat1 = -99999;
   double maxnMuHMVAEndcapAllCat1 = -99999;
   double maxnHHMVAEndcapAllCat1 = -99999;
   
   int minN0EndcapLSBCat1=99999;
   int minN1EndcapLSBCat1=99999;
   int minN2EndcapLSBCat1=99999;
   double minnMuMuEndcapLSBCat1 = 99999;
   double minnMuHEndcapLSBCat1 = 99999;
   double minnHHEndcapLSBCat1 = 99999;
   double minnMuMuMVAEndcapLSBCat1 = 99999;
   double minnMuHMVAEndcapLSBCat1 = 99999;
   double minnHHMVAEndcapLSBCat1 = 99999;
   int maxN0EndcapLSBCat1=-99999;
   int maxN1EndcapLSBCat1=-99999;
   int maxN2EndcapLSBCat1=-99999;
   double maxnMuMuEndcapLSBCat1 = -99999;
   double maxnMuHEndcapLSBCat1 = -99999;
   double maxnHHEndcapLSBCat1 = -99999;
   double maxnMuMuMVAEndcapLSBCat1 = -99999;
   double maxnMuHMVAEndcapLSBCat1 = -99999;
   double maxnHHMVAEndcapLSBCat1 = -99999;
   
   int minN0EndcapSigCat1=99999;
   int minN1EndcapSigCat1=99999;
   int minN2EndcapSigCat1=99999;
   double minnMuMuEndcapSigCat1 = 99999;
   double minnMuHEndcapSigCat1 = 99999;
   double minnHHEndcapSigCat1 = 99999;
   double minnMuMuMVAEndcapSigCat1 = 99999;
   double minnMuHMVAEndcapSigCat1 = 99999;
   double minnHHMVAEndcapSigCat1 = 99999;
   int maxN0EndcapSigCat1=-99999;
   int maxN1EndcapSigCat1=-99999;
   int maxN2EndcapSigCat1=-99999;
   double maxnMuMuEndcapSigCat1 = -99999;
   double maxnMuHEndcapSigCat1 = -99999;
   double maxnHHEndcapSigCat1 = -99999;
   double maxnMuMuMVAEndcapSigCat1 = -99999;
   double maxnMuHMVAEndcapSigCat1 = -99999;
   double maxnHHMVAEndcapSigCat1 = -99999;
   
   int minN0EndcapRSBCat1=99999;
   int minN1EndcapRSBCat1=99999;
   int minN2EndcapRSBCat1=99999;
   double minnMuMuEndcapRSBCat1 = 99999;
   double minnMuHEndcapRSBCat1 = 99999;
   double minnHHEndcapRSBCat1 = 99999;
   double minnMuMuMVAEndcapRSBCat1 = 99999;
   double minnMuHMVAEndcapRSBCat1 = 99999;
   double minnHHMVAEndcapRSBCat1 = 99999;
   int maxN0EndcapRSBCat1=-99999;
   int maxN1EndcapRSBCat1=-99999;
   int maxN2EndcapRSBCat1=-99999;
   double maxnMuMuEndcapRSBCat1 = -99999;
   double maxnMuHEndcapRSBCat1 = -99999;
   double maxnHHEndcapRSBCat1 = -99999;
   double maxnMuMuMVAEndcapRSBCat1 = -99999;
   double maxnMuHMVAEndcapRSBCat1 = -99999;
   double maxnHHMVAEndcapRSBCat1 = -99999;
   
   int minN0BarrelAllCat2=99999;
   int minN1BarrelAllCat2=99999;
   int minN2BarrelAllCat2=99999;
   double minnMuMuBarrelAllCat2 = 99999;
   double minnMuHBarrelAllCat2 = 99999;
   double minnHHBarrelAllCat2 = 99999;
   double minnMuMuMVABarrelAllCat2 = 99999;
   double minnMuHMVABarrelAllCat2 = 99999;
   double minnHHMVABarrelAllCat2 = 99999;
   int maxN0BarrelAllCat2=-99999;
   int maxN1BarrelAllCat2=-99999;
   int maxN2BarrelAllCat2=-99999;
   double maxnMuMuBarrelAllCat2 = -99999;
   double maxnMuHBarrelAllCat2 = -99999;
   double maxnHHBarrelAllCat2 = -99999;
   double maxnMuMuMVABarrelAllCat2 = -99999;
   double maxnMuHMVABarrelAllCat2 = -99999;
   double maxnHHMVABarrelAllCat2 = -99999;
   
   int minN0BarrelLSBCat2=99999;
   int minN1BarrelLSBCat2=99999;
   int minN2BarrelLSBCat2=99999;
   double minnMuMuBarrelLSBCat2 = 99999;
   double minnMuHBarrelLSBCat2 = 99999;
   double minnHHBarrelLSBCat2 = 99999;
   double minnMuMuMVABarrelLSBCat2 = 99999;
   double minnMuHMVABarrelLSBCat2 = 99999;
   double minnHHMVABarrelLSBCat2 = 99999;
   int maxN0BarrelLSBCat2=-99999;
   int maxN1BarrelLSBCat2=-99999;
   int maxN2BarrelLSBCat2=-99999;
   double maxnMuMuBarrelLSBCat2 = -99999;
   double maxnMuHBarrelLSBCat2 = -99999;
   double maxnHHBarrelLSBCat2 = -99999;
   double maxnMuMuMVABarrelLSBCat2 = -99999;
   double maxnMuHMVABarrelLSBCat2 = -99999;
   double maxnHHMVABarrelLSBCat2 = -99999;
   
   int minN0BarrelSigCat2=99999;
   int minN1BarrelSigCat2=99999;
   int minN2BarrelSigCat2=99999;
   double minnMuMuBarrelSigCat2 = 99999;
   double minnMuHBarrelSigCat2 = 99999;
   double minnHHBarrelSigCat2 = 99999;
   double minnMuMuMVABarrelSigCat2 = 99999;
   double minnMuHMVABarrelSigCat2 = 99999;
   double minnHHMVABarrelSigCat2 = 99999;
   int maxN0BarrelSigCat2=-99999;
   int maxN1BarrelSigCat2=-99999;
   int maxN2BarrelSigCat2=-99999;
   double maxnMuMuBarrelSigCat2 = -99999;
   double maxnMuHBarrelSigCat2 = -99999;
   double maxnHHBarrelSigCat2 = -99999;
   double maxnMuMuMVABarrelSigCat2 = -99999;
   double maxnMuHMVABarrelSigCat2 = -99999;
   double maxnHHMVABarrelSigCat2 = -99999;
   
   int minN0BarrelRSBCat2=99999;
   int minN1BarrelRSBCat2=99999;
   int minN2BarrelRSBCat2=99999;
   double minnMuMuBarrelRSBCat2 = 99999;
   double minnMuHBarrelRSBCat2 = 99999;
   double minnHHBarrelRSBCat2 = 99999;
   double minnMuMuMVABarrelRSBCat2 = 99999;
   double minnMuHMVABarrelRSBCat2 = 99999;
   double minnHHMVABarrelRSBCat2 = 99999;
   int maxN0BarrelRSBCat2=-99999;
   int maxN1BarrelRSBCat2=-99999;
   int maxN2BarrelRSBCat2=-99999;
   double maxnMuMuBarrelRSBCat2 = -99999;
   double maxnMuHBarrelRSBCat2 = -99999;
   double maxnHHBarrelRSBCat2 = -99999;
   double maxnMuMuMVABarrelRSBCat2 = -99999;
   double maxnMuHMVABarrelRSBCat2 = -99999;
   double maxnHHMVABarrelRSBCat2 = -99999;
   
   int minN0EndcapAllCat2=99999;
   int minN1EndcapAllCat2=99999;
   int minN2EndcapAllCat2=99999;
   double minnMuMuEndcapAllCat2 = 99999;
   double minnMuHEndcapAllCat2 = 99999;
   double minnHHEndcapAllCat2 = 99999;
   double minnMuMuMVAEndcapAllCat2 = 99999;
   double minnMuHMVAEndcapAllCat2 = 99999;
   double minnHHMVAEndcapAllCat2 = 99999;
   int maxN0EndcapAllCat2=-99999;
   int maxN1EndcapAllCat2=-99999;
   int maxN2EndcapAllCat2=-99999;
   double maxnMuMuEndcapAllCat2 = -99999;
   double maxnMuHEndcapAllCat2 = -99999;
   double maxnHHEndcapAllCat2 = -99999;
   double maxnMuMuMVAEndcapAllCat2 = -99999;
   double maxnMuHMVAEndcapAllCat2 = -99999;
   double maxnHHMVAEndcapAllCat2 = -99999;
   
   int minN0EndcapLSBCat2=99999;
   int minN1EndcapLSBCat2=99999;
   int minN2EndcapLSBCat2=99999;
   double minnMuMuEndcapLSBCat2 = 99999;
   double minnMuHEndcapLSBCat2 = 99999;
   double minnHHEndcapLSBCat2 = 99999;
   double minnMuMuMVAEndcapLSBCat2 = 99999;
   double minnMuHMVAEndcapLSBCat2 = 99999;
   double minnHHMVAEndcapLSBCat2 = 99999;
   int maxN0EndcapLSBCat2=-99999;
   int maxN1EndcapLSBCat2=-99999;
   int maxN2EndcapLSBCat2=-99999;
   double maxnMuMuEndcapLSBCat2 = -99999;
   double maxnMuHEndcapLSBCat2 = -99999;
   double maxnHHEndcapLSBCat2 = -99999;
   double maxnMuMuMVAEndcapLSBCat2 = -99999;
   double maxnMuHMVAEndcapLSBCat2 = -99999;
   double maxnHHMVAEndcapLSBCat2 = -99999;
   
   int minN0EndcapSigCat2=99999;
   int minN1EndcapSigCat2=99999;
   int minN2EndcapSigCat2=99999;
   double minnMuMuEndcapSigCat2 = 99999;
   double minnMuHEndcapSigCat2 = 99999;
   double minnHHEndcapSigCat2 = 99999;
   double minnMuMuMVAEndcapSigCat2 = 99999;
   double minnMuHMVAEndcapSigCat2 = 99999;
   double minnHHMVAEndcapSigCat2 = 99999;
   int maxN0EndcapSigCat2=-99999;
   int maxN1EndcapSigCat2=-99999;
   int maxN2EndcapSigCat2=-99999;
   double maxnMuMuEndcapSigCat2 = -99999;
   double maxnMuHEndcapSigCat2 = -99999;
   double maxnHHEndcapSigCat2 = -99999;
   double maxnMuMuMVAEndcapSigCat2 = -99999;
   double maxnMuHMVAEndcapSigCat2 = -99999;
   double maxnHHMVAEndcapSigCat2 = -99999;
   
   int minN0EndcapRSBCat2=99999;
   int minN1EndcapRSBCat2=99999;
   int minN2EndcapRSBCat2=99999;
   double minnMuMuEndcapRSBCat2 = 99999;
   double minnMuHEndcapRSBCat2 = 99999;
   double minnHHEndcapRSBCat2 = 99999;
   double minnMuMuMVAEndcapRSBCat2 = 99999;
   double minnMuHMVAEndcapRSBCat2 = 99999;
   double minnHHMVAEndcapRSBCat2 = 99999;
   int maxN0EndcapRSBCat2=-99999;
   int maxN1EndcapRSBCat2=-99999;
   int maxN2EndcapRSBCat2=-99999;
   double maxnMuMuEndcapRSBCat2 = -99999;
   double maxnMuHEndcapRSBCat2 = -99999;
   double maxnHHEndcapRSBCat2 = -99999;
   double maxnMuMuMVAEndcapRSBCat2 = -99999;
   double maxnMuHMVAEndcapRSBCat2 = -99999;
   double maxnHHMVAEndcapRSBCat2 = -99999;
   
   int minN0BarrelAllCat3=99999;
   int minN1BarrelAllCat3=99999;
   int minN2BarrelAllCat3=99999;
   double minnMuMuBarrelAllCat3 = 99999;
   double minnMuHBarrelAllCat3 = 99999;
   double minnHHBarrelAllCat3 = 99999;
   double minnMuMuMVABarrelAllCat3 = 99999;
   double minnMuHMVABarrelAllCat3 = 99999;
   double minnHHMVABarrelAllCat3 = 99999;
   int maxN0BarrelAllCat3=-99999;
   int maxN1BarrelAllCat3=-99999;
   int maxN2BarrelAllCat3=-99999;
   double maxnMuMuBarrelAllCat3 = -99999;
   double maxnMuHBarrelAllCat3 = -99999;
   double maxnHHBarrelAllCat3 = -99999;
   double maxnMuMuMVABarrelAllCat3 = -99999;
   double maxnMuHMVABarrelAllCat3 = -99999;
   double maxnHHMVABarrelAllCat3 = -99999;
   
   int minN0BarrelLSBCat3=99999;
   int minN1BarrelLSBCat3=99999;
   int minN2BarrelLSBCat3=99999;
   double minnMuMuBarrelLSBCat3 = 99999;
   double minnMuHBarrelLSBCat3 = 99999;
   double minnHHBarrelLSBCat3 = 99999;
   double minnMuMuMVABarrelLSBCat3 = 99999;
   double minnMuHMVABarrelLSBCat3 = 99999;
   double minnHHMVABarrelLSBCat3 = 99999;
   int maxN0BarrelLSBCat3=-99999;
   int maxN1BarrelLSBCat3=-99999;
   int maxN2BarrelLSBCat3=-99999;
   double maxnMuMuBarrelLSBCat3 = -99999;
   double maxnMuHBarrelLSBCat3 = -99999;
   double maxnHHBarrelLSBCat3 = -99999;
   double maxnMuMuMVABarrelLSBCat3 = -99999;
   double maxnMuHMVABarrelLSBCat3 = -99999;
   double maxnHHMVABarrelLSBCat3 = -99999;
   
   int minN0BarrelSigCat3=99999;
   int minN1BarrelSigCat3=99999;
   int minN2BarrelSigCat3=99999;
   double minnMuMuBarrelSigCat3 = 99999;
   double minnMuHBarrelSigCat3 = 99999;
   double minnHHBarrelSigCat3 = 99999;
   double minnMuMuMVABarrelSigCat3 = 99999;
   double minnMuHMVABarrelSigCat3 = 99999;
   double minnHHMVABarrelSigCat3 = 99999;
   int maxN0BarrelSigCat3=-99999;
   int maxN1BarrelSigCat3=-99999;
   int maxN2BarrelSigCat3=-99999;
   double maxnMuMuBarrelSigCat3 = -99999;
   double maxnMuHBarrelSigCat3 = -99999;
   double maxnHHBarrelSigCat3 = -99999;
   double maxnMuMuMVABarrelSigCat3 = -99999;
   double maxnMuHMVABarrelSigCat3 = -99999;
   double maxnHHMVABarrelSigCat3 = -99999;
   
   int minN0BarrelRSBCat3=99999;
   int minN1BarrelRSBCat3=99999;
   int minN2BarrelRSBCat3=99999;
   double minnMuMuBarrelRSBCat3 = 99999;
   double minnMuHBarrelRSBCat3 = 99999;
   double minnHHBarrelRSBCat3 = 99999;
   double minnMuMuMVABarrelRSBCat3 = 99999;
   double minnMuHMVABarrelRSBCat3 = 99999;
   double minnHHMVABarrelRSBCat3 = 99999;
   int maxN0BarrelRSBCat3=-99999;
   int maxN1BarrelRSBCat3=-99999;
   int maxN2BarrelRSBCat3=-99999;
   double maxnMuMuBarrelRSBCat3 = -99999;
   double maxnMuHBarrelRSBCat3 = -99999;
   double maxnHHBarrelRSBCat3 = -99999;
   double maxnMuMuMVABarrelRSBCat3 = -99999;
   double maxnMuHMVABarrelRSBCat3 = -99999;
   double maxnHHMVABarrelRSBCat3 = -99999;
   
   int minN0EndcapAllCat3=99999;
   int minN1EndcapAllCat3=99999;
   int minN2EndcapAllCat3=99999;
   double minnMuMuEndcapAllCat3 = 99999;
   double minnMuHEndcapAllCat3 = 99999;
   double minnHHEndcapAllCat3 = 99999;
   double minnMuMuMVAEndcapAllCat3 = 99999;
   double minnMuHMVAEndcapAllCat3 = 99999;
   double minnHHMVAEndcapAllCat3 = 99999;
   int maxN0EndcapAllCat3=-99999;
   int maxN1EndcapAllCat3=-99999;
   int maxN2EndcapAllCat3=-99999;
   double maxnMuMuEndcapAllCat3 = -99999;
   double maxnMuHEndcapAllCat3 = -99999;
   double maxnHHEndcapAllCat3 = -99999;
   double maxnMuMuMVAEndcapAllCat3 = -99999;
   double maxnMuHMVAEndcapAllCat3 = -99999;
   double maxnHHMVAEndcapAllCat3 = -99999;
   
   int minN0EndcapLSBCat3=99999;
   int minN1EndcapLSBCat3=99999;
   int minN2EndcapLSBCat3=99999;
   double minnMuMuEndcapLSBCat3 = 99999;
   double minnMuHEndcapLSBCat3 = 99999;
   double minnHHEndcapLSBCat3 = 99999;
   double minnMuMuMVAEndcapLSBCat3 = 99999;
   double minnMuHMVAEndcapLSBCat3 = 99999;
   double minnHHMVAEndcapLSBCat3 = 99999;
   int maxN0EndcapLSBCat3=-99999;
   int maxN1EndcapLSBCat3=-99999;
   int maxN2EndcapLSBCat3=-99999;
   double maxnMuMuEndcapLSBCat3 = -99999;
   double maxnMuHEndcapLSBCat3 = -99999;
   double maxnHHEndcapLSBCat3 = -99999;
   double maxnMuMuMVAEndcapLSBCat3 = -99999;
   double maxnMuHMVAEndcapLSBCat3 = -99999;
   double maxnHHMVAEndcapLSBCat3 = -99999;
   
   int minN0EndcapSigCat3=99999;
   int minN1EndcapSigCat3=99999;
   int minN2EndcapSigCat3=99999;
   double minnMuMuEndcapSigCat3 = 99999;
   double minnMuHEndcapSigCat3 = 99999;
   double minnHHEndcapSigCat3 = 99999;
   double minnMuMuMVAEndcapSigCat3 = 99999;
   double minnMuHMVAEndcapSigCat3 = 99999;
   double minnHHMVAEndcapSigCat3 = 99999;
   int maxN0EndcapSigCat3=-99999;
   int maxN1EndcapSigCat3=-99999;
   int maxN2EndcapSigCat3=-99999;
   double maxnMuMuEndcapSigCat3 = -99999;
   double maxnMuHEndcapSigCat3 = -99999;
   double maxnHHEndcapSigCat3 = -99999;
   double maxnMuMuMVAEndcapSigCat3 = -99999;
   double maxnMuHMVAEndcapSigCat3 = -99999;
   double maxnHHMVAEndcapSigCat3 = -99999;
   
   int minN0EndcapRSBCat3=99999;
   int minN1EndcapRSBCat3=99999;
   int minN2EndcapRSBCat3=99999;
   double minnMuMuEndcapRSBCat3 = 99999;
   double minnMuHEndcapRSBCat3 = 99999;
   double minnHHEndcapRSBCat3 = 99999;
   double minnMuMuMVAEndcapRSBCat3 = 99999;
   double minnMuHMVAEndcapRSBCat3 = 99999;
   double minnHHMVAEndcapRSBCat3 = 99999;
   int maxN0EndcapRSBCat3=-99999;
   int maxN1EndcapRSBCat3=-99999;
   int maxN2EndcapRSBCat3=-99999;
   double maxnMuMuEndcapRSBCat3 = -99999;
   double maxnMuHEndcapRSBCat3 = -99999;
   double maxnHHEndcapRSBCat3 = -99999;
   double maxnMuMuMVAEndcapRSBCat3 = -99999;
   double maxnMuHMVAEndcapRSBCat3 = -99999;
   double maxnHHMVAEndcapRSBCat3 = -99999;
   
   int minN0BarrelAllCat4=99999;
   int minN1BarrelAllCat4=99999;
   int minN2BarrelAllCat4=99999;
   double minnMuMuBarrelAllCat4 = 99999;
   double minnMuHBarrelAllCat4 = 99999;
   double minnHHBarrelAllCat4 = 99999;
   double minnMuMuMVABarrelAllCat4 = 99999;
   double minnMuHMVABarrelAllCat4 = 99999;
   double minnHHMVABarrelAllCat4 = 99999;
   int maxN0BarrelAllCat4=-99999;
   int maxN1BarrelAllCat4=-99999;
   int maxN2BarrelAllCat4=-99999;
   double maxnMuMuBarrelAllCat4 = -99999;
   double maxnMuHBarrelAllCat4 = -99999;
   double maxnHHBarrelAllCat4 = -99999;
   double maxnMuMuMVABarrelAllCat4 = -99999;
   double maxnMuHMVABarrelAllCat4 = -99999;
   double maxnHHMVABarrelAllCat4 = -99999;
   
   int minN0BarrelLSBCat4=99999;
   int minN1BarrelLSBCat4=99999;
   int minN2BarrelLSBCat4=99999;
   double minnMuMuBarrelLSBCat4 = 99999;
   double minnMuHBarrelLSBCat4 = 99999;
   double minnHHBarrelLSBCat4 = 99999;
   double minnMuMuMVABarrelLSBCat4 = 99999;
   double minnMuHMVABarrelLSBCat4 = 99999;
   double minnHHMVABarrelLSBCat4 = 99999;
   int maxN0BarrelLSBCat4=-99999;
   int maxN1BarrelLSBCat4=-99999;
   int maxN2BarrelLSBCat4=-99999;
   double maxnMuMuBarrelLSBCat4 = -99999;
   double maxnMuHBarrelLSBCat4 = -99999;
   double maxnHHBarrelLSBCat4 = -99999;
   double maxnMuMuMVABarrelLSBCat4 = -99999;
   double maxnMuHMVABarrelLSBCat4 = -99999;
   double maxnHHMVABarrelLSBCat4 = -99999;
   
   int minN0BarrelSigCat4=99999;
   int minN1BarrelSigCat4=99999;
   int minN2BarrelSigCat4=99999;
   double minnMuMuBarrelSigCat4 = 99999;
   double minnMuHBarrelSigCat4 = 99999;
   double minnHHBarrelSigCat4 = 99999;
   double minnMuMuMVABarrelSigCat4 = 99999;
   double minnMuHMVABarrelSigCat4 = 99999;
   double minnHHMVABarrelSigCat4 = 99999;
   int maxN0BarrelSigCat4=-99999;
   int maxN1BarrelSigCat4=-99999;
   int maxN2BarrelSigCat4=-99999;
   double maxnMuMuBarrelSigCat4 = -99999;
   double maxnMuHBarrelSigCat4 = -99999;
   double maxnHHBarrelSigCat4 = -99999;
   double maxnMuMuMVABarrelSigCat4 = -99999;
   double maxnMuHMVABarrelSigCat4 = -99999;
   double maxnHHMVABarrelSigCat4 = -99999;
   
   int minN0BarrelRSBCat4=99999;
   int minN1BarrelRSBCat4=99999;
   int minN2BarrelRSBCat4=99999;
   double minnMuMuBarrelRSBCat4 = 99999;
   double minnMuHBarrelRSBCat4 = 99999;
   double minnHHBarrelRSBCat4 = 99999;
   double minnMuMuMVABarrelRSBCat4 = 99999;
   double minnMuHMVABarrelRSBCat4 = 99999;
   double minnHHMVABarrelRSBCat4 = 99999;
   int maxN0BarrelRSBCat4=-99999;
   int maxN1BarrelRSBCat4=-99999;
   int maxN2BarrelRSBCat4=-99999;
   double maxnMuMuBarrelRSBCat4 = -99999;
   double maxnMuHBarrelRSBCat4 = -99999;
   double maxnHHBarrelRSBCat4 = -99999;
   double maxnMuMuMVABarrelRSBCat4 = -99999;
   double maxnMuHMVABarrelRSBCat4 = -99999;
   double maxnHHMVABarrelRSBCat4 = -99999;
   
   int minN0EndcapAllCat4=99999;
   int minN1EndcapAllCat4=99999;
   int minN2EndcapAllCat4=99999;
   double minnMuMuEndcapAllCat4 = 99999;
   double minnMuHEndcapAllCat4 = 99999;
   double minnHHEndcapAllCat4 = 99999;
   double minnMuMuMVAEndcapAllCat4 = 99999;
   double minnMuHMVAEndcapAllCat4 = 99999;
   double minnHHMVAEndcapAllCat4 = 99999;
   int maxN0EndcapAllCat4=-99999;
   int maxN1EndcapAllCat4=-99999;
   int maxN2EndcapAllCat4=-99999;
   double maxnMuMuEndcapAllCat4 = -99999;
   double maxnMuHEndcapAllCat4 = -99999;
   double maxnHHEndcapAllCat4 = -99999;
   double maxnMuMuMVAEndcapAllCat4 = -99999;
   double maxnMuHMVAEndcapAllCat4 = -99999;
   double maxnHHMVAEndcapAllCat4 = -99999;
   
   int minN0EndcapLSBCat4=99999;
   int minN1EndcapLSBCat4=99999;
   int minN2EndcapLSBCat4=99999;
   double minnMuMuEndcapLSBCat4 = 99999;
   double minnMuHEndcapLSBCat4 = 99999;
   double minnHHEndcapLSBCat4 = 99999;
   double minnMuMuMVAEndcapLSBCat4 = 99999;
   double minnMuHMVAEndcapLSBCat4 = 99999;
   double minnHHMVAEndcapLSBCat4 = 99999;
   int maxN0EndcapLSBCat4=-99999;
   int maxN1EndcapLSBCat4=-99999;
   int maxN2EndcapLSBCat4=-99999;
   double maxnMuMuEndcapLSBCat4 = -99999;
   double maxnMuHEndcapLSBCat4 = -99999;
   double maxnHHEndcapLSBCat4 = -99999;
   double maxnMuMuMVAEndcapLSBCat4 = -99999;
   double maxnMuHMVAEndcapLSBCat4 = -99999;
   double maxnHHMVAEndcapLSBCat4 = -99999;
   
   int minN0EndcapSigCat4=99999;
   int minN1EndcapSigCat4=99999;
   int minN2EndcapSigCat4=99999;
   double minnMuMuEndcapSigCat4 = 99999;
   double minnMuHEndcapSigCat4 = 99999;
   double minnHHEndcapSigCat4 = 99999;
   double minnMuMuMVAEndcapSigCat4 = 99999;
   double minnMuHMVAEndcapSigCat4 = 99999;
   double minnHHMVAEndcapSigCat4 = 99999;
   int maxN0EndcapSigCat4=-99999;
   int maxN1EndcapSigCat4=-99999;
   int maxN2EndcapSigCat4=-99999;
   double maxnMuMuEndcapSigCat4 = -99999;
   double maxnMuHEndcapSigCat4 = -99999;
   double maxnHHEndcapSigCat4 = -99999;
   double maxnMuMuMVAEndcapSigCat4 = -99999;
   double maxnMuHMVAEndcapSigCat4 = -99999;
   double maxnHHMVAEndcapSigCat4 = -99999;
   
   int minN0EndcapRSBCat4=99999;
   int minN1EndcapRSBCat4=99999;
   int minN2EndcapRSBCat4=99999;
   double minnMuMuEndcapRSBCat4 = 99999;
   double minnMuHEndcapRSBCat4 = 99999;
   double minnHHEndcapRSBCat4 = 99999;
   double minnMuMuMVAEndcapRSBCat4 = 99999;
   double minnMuHMVAEndcapRSBCat4 = 99999;
   double minnHHMVAEndcapRSBCat4 = 99999;
   int maxN0EndcapRSBCat4=-99999;
   int maxN1EndcapRSBCat4=-99999;
   int maxN2EndcapRSBCat4=-99999;
   double maxnMuMuEndcapRSBCat4 = -99999;
   double maxnMuHEndcapRSBCat4 = -99999;
   double maxnHHEndcapRSBCat4 = -99999;
   double maxnMuMuMVAEndcapRSBCat4 = -99999;
   double maxnMuHMVAEndcapRSBCat4 = -99999;
   double maxnHHMVAEndcapRSBCat4 = -99999;
   
   for(unsigned int i = 0; i < 3; i++)
   {
     for(unsigned int j = 0; j < 3; j++)
     {
       for(unsigned int k = 0; k < 3; k++)
       {
         double array[9];
         array[0] = (1-epsilonMu[i])*(1-epsilonMu[i]);
         array[1] = (1-epsilonMu[i])*(1-epsilonH1[j]);
         array[2] = (1-epsilonH2[k])*(1-epsilonH2[k]);
         array[3] = 2*(1-epsilonMu[i])*epsilonMu[i];
         array[4] = ((1-epsilonMu[i])*epsilonH1[j] + (1-epsilonH1[j])*epsilonMu[i]);
         array[5] = 2*(1-epsilonH2[k])*epsilonH2[k];
         array[6] = epsilonMu[i]*epsilonMu[i];
         array[7] = epsilonMu[i]*epsilonH1[j];
         array[8] = epsilonH2[k]*epsilonH2[k];
         
         TMatrixD matrix(3,3);
         matrix.SetMatrixArray(array);
         std::cout << "\n\n\nepsilonMu = " << epsilonMu[i] << std::endl;
         std::cout << "epsilonH1 = " << epsilonH1[j] << std::endl;
         std::cout << "epsilonH2 = " << epsilonH2[k] << std::endl;
         std::cout << "\nSystem matrix is:\n";
         for(unsigned int l = 0; l<3; l++)
         {
           std::cout << matrix(l,0) << "\t" << matrix(l,1) << "\t" << matrix(l,2) << std::endl;
         }
         
         TMatrixD invMatrix = matrix.Invert();
         std::cout << "\n";
         std::cout << "Inverted matrix is:\n";
         for(unsigned int l = 0; l<3; l++)
         {
           std::cout << invMatrix(l,0) << " " << invMatrix(l,1) << " " << invMatrix(l,2) << std::endl;
         }
         
         int N0BarrelAll=hMassFail2Barrel->Integral(1,25);
         if(N0BarrelAll<minN0BarrelAll) minN0BarrelAll = N0BarrelAll;
         if(N0BarrelAll>maxN0BarrelAll) maxN0BarrelAll = N0BarrelAll;
         int N1BarrelAll=hMassFail1Barrel->Integral(1,25);
         if(N1BarrelAll<minN1BarrelAll) minN1BarrelAll = N1BarrelAll;
         if(N1BarrelAll>maxN1BarrelAll) maxN1BarrelAll = N1BarrelAll;
         int N2BarrelAll=hMassMVABarrel->Integral(1,25);
         if(N2BarrelAll<minN2BarrelAll) minN2BarrelAll = N2BarrelAll;
         if(N2BarrelAll>maxN2BarrelAll) maxN2BarrelAll = N2BarrelAll;
         double nMuMuBarrelAll = N0BarrelAll*matrix[0][0]+N1BarrelAll*matrix[0][1]+N2BarrelAll*matrix[0][2];
         if(nMuMuBarrelAll<minnMuMuBarrelAll) minnMuMuBarrelAll = nMuMuBarrelAll;
         if(nMuMuBarrelAll>maxnMuMuBarrelAll) maxnMuMuBarrelAll = nMuMuBarrelAll;
         double nMuHBarrelAll = N0BarrelAll*matrix[1][0]+N1BarrelAll*matrix[1][1]+N2BarrelAll*matrix[1][2];
         if(nMuHBarrelAll<minnMuHBarrelAll) minnMuHBarrelAll = nMuHBarrelAll;
         if(nMuHBarrelAll>maxnMuHBarrelAll) maxnMuHBarrelAll = nMuHBarrelAll;
         double nHHBarrelAll = N0BarrelAll*matrix[2][0]+N1BarrelAll*matrix[2][1]+N2BarrelAll*matrix[2][2];
         if(nHHBarrelAll<minnHHBarrelAll) minnHHBarrelAll = nHHBarrelAll;
         if(nHHBarrelAll>maxnHHBarrelAll) maxnHHBarrelAll = nHHBarrelAll;
         double nMuMuMVABarrelAll = nMuMuBarrelAll*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelAll<minnMuMuMVABarrelAll) minnMuMuMVABarrelAll = nMuMuMVABarrelAll;
         if(nMuMuMVABarrelAll>maxnMuMuMVABarrelAll) maxnMuMuMVABarrelAll = nMuMuMVABarrelAll;
         double nMuHMVABarrelAll = nMuHBarrelAll*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelAll<minnMuHMVABarrelAll) minnMuHMVABarrelAll = nMuHMVABarrelAll;
         if(nMuHMVABarrelAll>maxnMuHMVABarrelAll) maxnMuHMVABarrelAll = nMuHMVABarrelAll;
         double nHHMVABarrelAll = nHHBarrelAll*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelAll<minnHHMVABarrelAll) minnHHMVABarrelAll = nHHMVABarrelAll;
         if(nHHMVABarrelAll>maxnHHMVABarrelAll) maxnHHMVABarrelAll = nHHMVABarrelAll;
         
         std::cout << "\nN0BarrelAll = " << N0BarrelAll << std::endl;
         std::cout << "N1BarrelAll = " << N1BarrelAll << std::endl;
         std::cout << "N2BarrelAll = " << N2BarrelAll << std::endl;
         
         std::cout << "nMuMuBarrelAll = " << nMuMuBarrelAll << std::endl;
         std::cout << "nMuHBarrelAll = " << nMuHBarrelAll << std::endl;
         std::cout << "nHHBarrelAll = " << nHHBarrelAll << std::endl;
         std::cout << "nMuMuMVABarrelAll = " << nMuMuMVABarrelAll << std::endl;
         std::cout << "nMuHMVABarrelAll = " << nMuHMVABarrelAll << std::endl;
         std::cout << "nHHMVABarrelAll = " << nHHMVABarrelAll << std::endl;
         
         int N0BarrelLSB=hMassFail2Barrel->Integral(1,8);
         if(N0BarrelLSB<minN0BarrelLSB) minN0BarrelLSB = N0BarrelLSB;
         if(N0BarrelLSB>maxN0BarrelLSB) maxN0BarrelLSB = N0BarrelLSB;
         int N1BarrelLSB=hMassFail1Barrel->Integral(1,8);
         if(N1BarrelLSB<minN1BarrelLSB) minN1BarrelLSB = N1BarrelLSB;
         if(N1BarrelLSB>maxN1BarrelLSB) maxN1BarrelLSB = N1BarrelLSB;
         int N2BarrelLSB=hMassMVABarrel->Integral(1,8);
         if(N2BarrelLSB<minN2BarrelLSB) minN2BarrelLSB = N2BarrelLSB;
         if(N2BarrelLSB>maxN2BarrelLSB) maxN2BarrelLSB = N2BarrelLSB;
         double nMuMuBarrelLSB = N0BarrelLSB*matrix[0][0]+N1BarrelLSB*matrix[0][1]+N2BarrelLSB*matrix[0][2];
         if(nMuMuBarrelLSB<minnMuMuBarrelLSB) minnMuMuBarrelLSB = nMuMuBarrelLSB;
         if(nMuMuBarrelLSB>maxnMuMuBarrelLSB) maxnMuMuBarrelLSB = nMuMuBarrelLSB;
         double nMuHBarrelLSB = N0BarrelLSB*matrix[1][0]+N1BarrelLSB*matrix[1][1]+N2BarrelLSB*matrix[1][2];
         if(nMuHBarrelLSB<minnMuHBarrelLSB) minnMuHBarrelLSB = nMuHBarrelLSB;
         if(nMuHBarrelLSB>maxnMuHBarrelLSB) maxnMuHBarrelLSB = nMuHBarrelLSB;
         double nHHBarrelLSB = N0BarrelLSB*matrix[2][0]+N1BarrelLSB*matrix[2][1]+N2BarrelLSB*matrix[2][2];
         if(nHHBarrelLSB<minnHHBarrelLSB) minnHHBarrelLSB = nHHBarrelLSB;
         if(nHHBarrelLSB>maxnHHBarrelLSB) maxnHHBarrelLSB = nHHBarrelLSB;
         double nMuMuMVABarrelLSB = nMuMuBarrelLSB*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelLSB<minnMuMuMVABarrelLSB) minnMuMuMVABarrelLSB = nMuMuMVABarrelLSB;
         if(nMuMuMVABarrelLSB>maxnMuMuMVABarrelLSB) maxnMuMuMVABarrelLSB = nMuMuMVABarrelLSB;
         double nMuHMVABarrelLSB = nMuHBarrelLSB*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelLSB<minnMuHMVABarrelLSB) minnMuHMVABarrelLSB = nMuHMVABarrelLSB;
         if(nMuHMVABarrelLSB>maxnMuHMVABarrelLSB) maxnMuHMVABarrelLSB = nMuHMVABarrelLSB;
         double nHHMVABarrelLSB = nHHBarrelLSB*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelLSB<minnHHMVABarrelLSB) minnHHMVABarrelLSB = nHHMVABarrelLSB;
         if(nHHMVABarrelLSB>maxnHHMVABarrelLSB) maxnHHMVABarrelLSB = nHHMVABarrelLSB;
         
         std::cout << "\nN0BarrelLSB = " << N0BarrelLSB << std::endl;
         std::cout << "N1BarrelLSB = " << N1BarrelLSB << std::endl;
         std::cout << "N2BarrelLSB = " << N2BarrelLSB << std::endl;
         
         std::cout << "nMuMuBarrelLSB = " << nMuMuBarrelLSB << std::endl;
         std::cout << "nMuHBarrelLSB = " << nMuHBarrelLSB << std::endl;
         std::cout << "nHHBarrelLSB = " << nHHBarrelLSB << std::endl;
         std::cout << "nMuMuMVABarrelLSB = " << nMuMuMVABarrelLSB << std::endl;
         std::cout << "nMuHMVABarrelLSB = " << nMuHMVABarrelLSB << std::endl;
         std::cout << "nHHMVABarrelLSB = " << nHHMVABarrelLSB << std::endl;
         
         int N0BarrelSig=hMassFail2Barrel->Integral(9,15);
         if(N0BarrelSig<minN0BarrelSig) minN0BarrelSig = N0BarrelSig;
         if(N0BarrelSig>maxN0BarrelSig) maxN0BarrelSig = N0BarrelSig;
         int N1BarrelSig=hMassFail1Barrel->Integral(9,15);
         if(N1BarrelSig<minN1BarrelSig) minN1BarrelSig = N1BarrelSig;
         if(N1BarrelSig>maxN1BarrelSig) maxN1BarrelSig = N1BarrelSig;
         int N2BarrelSig=hMassMVABarrel->Integral(9,15);
         if(N2BarrelSig<minN2BarrelSig) minN2BarrelSig = N2BarrelSig;
         if(N2BarrelSig>maxN2BarrelSig) maxN2BarrelSig = N2BarrelSig;
         double nMuMuBarrelSig = N0BarrelSig*matrix[0][0]+N1BarrelSig*matrix[0][1]+N2BarrelSig*matrix[0][2];
         if(nMuMuBarrelSig<minnMuMuBarrelSig) minnMuMuBarrelSig = nMuMuBarrelSig;
         if(nMuMuBarrelSig>maxnMuMuBarrelSig) maxnMuMuBarrelSig = nMuMuBarrelSig;
         double nMuHBarrelSig = N0BarrelSig*matrix[1][0]+N1BarrelSig*matrix[1][1]+N2BarrelSig*matrix[1][2];
         if(nMuHBarrelSig<minnMuHBarrelSig) minnMuHBarrelSig = nMuHBarrelSig;
         if(nMuHBarrelSig>maxnMuHBarrelSig) maxnMuHBarrelSig = nMuHBarrelSig;
         double nHHBarrelSig = N0BarrelSig*matrix[2][0]+N1BarrelSig*matrix[2][1]+N2BarrelSig*matrix[2][2];
         if(nHHBarrelSig<minnHHBarrelSig) minnHHBarrelSig = nHHBarrelSig;
         if(nHHBarrelSig>maxnHHBarrelSig) maxnHHBarrelSig = nHHBarrelSig;
         double nMuMuMVABarrelSig = nMuMuBarrelSig*epsilonMu[i]*epsilonMu[i];
         
         if(nMuMuMVABarrelSig<minnMuMuMVABarrelSig) minnMuMuMVABarrelSig = nMuMuMVABarrelSig;
         if(nMuMuMVABarrelSig>maxnMuMuMVABarrelSig) maxnMuMuMVABarrelSig = nMuMuMVABarrelSig;
         double nMuHMVABarrelSig = nMuHBarrelSig*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelSig<minnMuHMVABarrelSig) minnMuHMVABarrelSig = nMuHMVABarrelSig;
         if(nMuHMVABarrelSig>maxnMuHMVABarrelSig) maxnMuHMVABarrelSig = nMuHMVABarrelSig;
         double nHHMVABarrelSig = nHHBarrelSig*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelSig<minnHHMVABarrelSig) minnHHMVABarrelSig = nHHMVABarrelSig;
         if(nHHMVABarrelSig>maxnHHMVABarrelSig) maxnHHMVABarrelSig = nHHMVABarrelSig;
         
         
         std::cout << "\nN0BarrelSig = " << N0BarrelSig << std::endl;
         std::cout << "N1BarrelSig = " << N1BarrelSig << std::endl;
         std::cout << "N2BarrelSig = " << N2BarrelSig << std::endl;
         
         std::cout << "nMuMuBarrelSig = " << nMuMuBarrelSig << std::endl;
         std::cout << "nMuHBarrelSig = " << nMuHBarrelSig << std::endl;
         std::cout << "nHHBarrelSig = " << nHHBarrelSig << std::endl;
         std::cout << "nMuMuMVABarrelSig = " << nMuMuMVABarrelSig << std::endl;
         std::cout << "nMuHMVABarrelSig = " << nMuHMVABarrelSig << std::endl;
         std::cout << "nHHMVABarrelSig = " << nHHMVABarrelSig << std::endl;
         
         int N0BarrelRSB=hMassFail2Barrel->Integral(16,25);
         if(N0BarrelRSB<minN0BarrelRSB) minN0BarrelRSB = N0BarrelRSB;
         if(N0BarrelRSB>maxN0BarrelRSB) maxN0BarrelRSB = N0BarrelRSB;
         int N1BarrelRSB=hMassFail1Barrel->Integral(16,25);
         if(N1BarrelRSB<minN1BarrelRSB) minN1BarrelRSB = N1BarrelRSB;
         if(N1BarrelRSB>maxN1BarrelRSB) maxN1BarrelRSB = N1BarrelRSB;
         int N2BarrelRSB=hMassMVABarrel->Integral(16,25);
         if(N2BarrelRSB<minN2BarrelRSB) minN2BarrelRSB = N2BarrelRSB;
         if(N2BarrelRSB>maxN2BarrelRSB) maxN2BarrelRSB = N2BarrelRSB;
         double nMuMuBarrelRSB = N0BarrelRSB*matrix[0][0]+N1BarrelRSB*matrix[0][1]+N2BarrelRSB*matrix[0][2];
         if(nMuMuBarrelRSB<minnMuMuBarrelRSB) minnMuMuBarrelRSB = nMuMuBarrelRSB;
         if(nMuMuBarrelRSB>maxnMuMuBarrelRSB) maxnMuMuBarrelRSB = nMuMuBarrelRSB;
         double nMuHBarrelRSB = N0BarrelRSB*matrix[1][0]+N1BarrelRSB*matrix[1][1]+N2BarrelRSB*matrix[1][2];
         if(nMuHBarrelRSB<minnMuHBarrelRSB) minnMuHBarrelRSB = nMuHBarrelRSB;
         if(nMuHBarrelRSB>maxnMuHBarrelRSB) maxnMuHBarrelRSB = nMuHBarrelRSB;
         double nHHBarrelRSB = N0BarrelRSB*matrix[2][0]+N1BarrelRSB*matrix[2][1]+N2BarrelRSB*matrix[2][2];
         if(nHHBarrelRSB<minnHHBarrelRSB) minnHHBarrelRSB = nHHBarrelRSB;
         if(nHHBarrelRSB>maxnHHBarrelRSB) maxnHHBarrelRSB = nHHBarrelRSB;
         double nMuMuMVABarrelRSB = nMuMuBarrelRSB*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelRSB<minnMuMuMVABarrelRSB) minnMuMuMVABarrelRSB = nMuMuMVABarrelRSB;
         if(nMuMuMVABarrelRSB>maxnMuMuMVABarrelRSB) maxnMuMuMVABarrelRSB = nMuMuMVABarrelRSB;
         double nMuHMVABarrelRSB = nMuHBarrelRSB*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelRSB<minnMuHMVABarrelRSB) minnMuHMVABarrelRSB = nMuHMVABarrelRSB;
         if(nMuHMVABarrelRSB>maxnMuHMVABarrelRSB) maxnMuHMVABarrelRSB = nMuHMVABarrelRSB;
         double nHHMVABarrelRSB = nHHBarrelRSB*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelRSB<minnHHMVABarrelRSB) minnHHMVABarrelRSB = nHHMVABarrelRSB;
         if(nHHMVABarrelRSB>maxnHHMVABarrelRSB) maxnHHMVABarrelRSB = nHHMVABarrelRSB;
         
         std::cout << "\nN0BarrelRSB = " << N0BarrelRSB << std::endl;
         std::cout << "N1BarrelRSB = " << N1BarrelRSB << std::endl;
         std::cout << "N2BarrelRSB = " << N2BarrelRSB << std::endl;
         
         std::cout << "nMuMuBarrelRSB = " << nMuMuBarrelRSB << std::endl;
         std::cout << "nMuHBarrelRSB = " << nMuHBarrelRSB << std::endl;
         std::cout << "nHHBarrelRSB = " << nHHBarrelRSB << std::endl;
         std::cout << "nMuMuMVABarrelRSB = " << nMuMuMVABarrelRSB << std::endl;
         std::cout << "nMuHMVABarrelRSB = " << nMuHMVABarrelRSB << std::endl;
         std::cout << "nHHMVABarrelRSB = " << nHHMVABarrelRSB << std::endl;
         
         int N0EndcapAll=hMassFail2Endcap->Integral(1,25);
         if(N0EndcapAll<minN0EndcapAll) minN0EndcapAll = N0EndcapAll;
         if(N0EndcapAll>maxN0EndcapAll) maxN0EndcapAll = N0EndcapAll;
         int N1EndcapAll=hMassFail1Endcap->Integral(1,25);
         if(N1EndcapAll<minN1EndcapAll) minN1EndcapAll = N1EndcapAll;
         if(N1EndcapAll>maxN1EndcapAll) maxN1EndcapAll = N1EndcapAll;
         int N2EndcapAll=hMassMVAEndcap->Integral(1,25);
         if(N2EndcapAll<minN2EndcapAll) minN2EndcapAll = N2EndcapAll;
         if(N2EndcapAll>maxN2EndcapAll) maxN2EndcapAll = N2EndcapAll;
         double nMuMuEndcapAll = N0EndcapAll*matrix[0][0]+N1EndcapAll*matrix[0][1]+N2EndcapAll*matrix[0][2];
         if(nMuMuEndcapAll<minnMuMuEndcapAll) minnMuMuEndcapAll = nMuMuEndcapAll;
         if(nMuMuEndcapAll>maxnMuMuEndcapAll) maxnMuMuEndcapAll = nMuMuEndcapAll;
         double nMuHEndcapAll = N0EndcapAll*matrix[1][0]+N1EndcapAll*matrix[1][1]+N2EndcapAll*matrix[1][2];
         if(nMuHEndcapAll<minnMuHEndcapAll) minnMuHEndcapAll = nMuHEndcapAll;
         if(nMuHEndcapAll>maxnMuHEndcapAll) maxnMuHEndcapAll = nMuHEndcapAll;
         double nHHEndcapAll = N0EndcapAll*matrix[2][0]+N1EndcapAll*matrix[2][1]+N2EndcapAll*matrix[2][2];
         if(nHHEndcapAll<minnHHEndcapAll) minnHHEndcapAll = nHHEndcapAll;
         if(nHHEndcapAll>maxnHHEndcapAll) maxnHHEndcapAll = nHHEndcapAll;
         double nMuMuMVAEndcapAll = nMuMuEndcapAll*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapAll<minnMuMuMVAEndcapAll) minnMuMuMVAEndcapAll = nMuMuMVAEndcapAll;
         if(nMuMuMVAEndcapAll>maxnMuMuMVAEndcapAll) maxnMuMuMVAEndcapAll = nMuMuMVAEndcapAll;
         double nMuHMVAEndcapAll = nMuHEndcapAll*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapAll<minnMuHMVAEndcapAll) minnMuHMVAEndcapAll = nMuHMVAEndcapAll;
         if(nMuHMVAEndcapAll>maxnMuHMVAEndcapAll) maxnMuHMVAEndcapAll = nMuHMVAEndcapAll;
         double nHHMVAEndcapAll = nHHEndcapAll*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapAll<minnHHMVAEndcapAll) minnHHMVAEndcapAll = nHHMVAEndcapAll;
         if(nHHMVAEndcapAll>maxnHHMVAEndcapAll) maxnHHMVAEndcapAll = nHHMVAEndcapAll;
         
         std::cout << "\nN0EndcapAll = " << N0EndcapAll << std::endl;
         std::cout << "N1EndcapAll = " << N1EndcapAll << std::endl;
         std::cout << "N2EndcapAll = " << N2EndcapAll << std::endl;
         
         std::cout << "nMuMuEndcapAll = " << nMuMuEndcapAll << std::endl;
         std::cout << "nMuHEndcapAll = " << nMuHEndcapAll << std::endl;
         std::cout << "nHHEndcapAll = " << nHHEndcapAll << std::endl;
         std::cout << "nMuMuMVAEndcapAll = " << nMuMuMVAEndcapAll << std::endl;
         std::cout << "nMuHMVAEndcapAll = " << nMuHMVAEndcapAll << std::endl;
         std::cout << "nHHMVAEndcapAll = " << nHHMVAEndcapAll << std::endl;
         
         int N0EndcapLSB=hMassFail2Endcap->Integral(1,8);
         if(N0EndcapLSB<minN0EndcapLSB) minN0EndcapLSB = N0EndcapLSB;
         if(N0EndcapLSB>maxN0EndcapLSB) maxN0EndcapLSB = N0EndcapLSB;
         int N1EndcapLSB=hMassFail1Endcap->Integral(1,8);
         if(N1EndcapLSB<minN1EndcapLSB) minN1EndcapLSB = N1EndcapLSB;
         if(N1EndcapLSB>maxN1EndcapLSB) maxN1EndcapLSB = N1EndcapLSB;
         int N2EndcapLSB=hMassMVAEndcap->Integral(1,8);
         if(N2EndcapLSB<minN2EndcapLSB) minN2EndcapLSB = N2EndcapLSB;
         if(N2EndcapLSB>maxN2EndcapLSB) maxN2EndcapLSB = N2EndcapLSB;
         double nMuMuEndcapLSB = N0EndcapLSB*matrix[0][0]+N1EndcapLSB*matrix[0][1]+N2EndcapLSB*matrix[0][2];
         if(nMuMuEndcapLSB<minnMuMuEndcapLSB) minnMuMuEndcapLSB = nMuMuEndcapLSB;
         if(nMuMuEndcapLSB>maxnMuMuEndcapLSB) maxnMuMuEndcapLSB = nMuMuEndcapLSB;
         double nMuHEndcapLSB = N0EndcapLSB*matrix[1][0]+N1EndcapLSB*matrix[1][1]+N2EndcapLSB*matrix[1][2];
         if(nMuHEndcapLSB<minnMuHEndcapLSB) minnMuHEndcapLSB = nMuHEndcapLSB;
         if(nMuHEndcapLSB>maxnMuHEndcapLSB) maxnMuHEndcapLSB = nMuHEndcapLSB;
         double nHHEndcapLSB = N0EndcapLSB*matrix[2][0]+N1EndcapLSB*matrix[2][1]+N2EndcapLSB*matrix[2][2];
         if(nHHEndcapLSB<minnHHEndcapLSB) minnHHEndcapLSB = nHHEndcapLSB;
         if(nHHEndcapLSB>maxnHHEndcapLSB) maxnHHEndcapLSB = nHHEndcapLSB;
         double nMuMuMVAEndcapLSB = nMuMuEndcapLSB*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapLSB<minnMuMuMVAEndcapLSB) minnMuMuMVAEndcapLSB = nMuMuMVAEndcapLSB;
         if(nMuMuMVAEndcapLSB>maxnMuMuMVAEndcapLSB) maxnMuMuMVAEndcapLSB = nMuMuMVAEndcapLSB;
         double nMuHMVAEndcapLSB = nMuHEndcapLSB*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapLSB<minnMuHMVAEndcapLSB) minnMuHMVAEndcapLSB = nMuHMVAEndcapLSB;
         if(nMuHMVAEndcapLSB>maxnMuHMVAEndcapLSB) maxnMuHMVAEndcapLSB = nMuHMVAEndcapLSB;
         double nHHMVAEndcapLSB = nHHEndcapLSB*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapLSB<minnHHMVAEndcapLSB) minnHHMVAEndcapLSB = nHHMVAEndcapLSB;
         if(nHHMVAEndcapLSB>maxnHHMVAEndcapLSB) maxnHHMVAEndcapLSB = nHHMVAEndcapLSB;
         
         std::cout << "\nN0EndcapLSB = " << N0EndcapLSB << std::endl;
         std::cout << "N1EndcapLSB = " << N1EndcapLSB << std::endl;
         std::cout << "N2EndcapLSB = " << N2EndcapLSB << std::endl;
         
         std::cout << "nMuMuEndcapLSB = " << nMuMuEndcapLSB << std::endl;
         std::cout << "nMuHEndcapLSB = " << nMuHEndcapLSB << std::endl;
         std::cout << "nHHEndcapLSB = " << nHHEndcapLSB << std::endl;
         std::cout << "nMuMuMVAEndcapLSB = " << nMuMuMVAEndcapLSB << std::endl;
         std::cout << "nMuHMVAEndcapLSB = " << nMuHMVAEndcapLSB << std::endl;
         std::cout << "nHHMVAEndcapLSB = " << nHHMVAEndcapLSB << std::endl;
         
         int N0EndcapSig=hMassFail2Endcap->Integral(9,15);
         if(N0EndcapSig<minN0EndcapSig) minN0EndcapSig = N0EndcapSig;
         if(N0EndcapSig>maxN0EndcapSig) maxN0EndcapSig = N0EndcapSig;
         int N1EndcapSig=hMassFail1Endcap->Integral(9,15);
         if(N1EndcapSig<minN1EndcapSig) minN1EndcapSig = N1EndcapSig;
         if(N1EndcapSig>maxN1EndcapSig) maxN1EndcapSig = N1EndcapSig;
         int N2EndcapSig=hMassMVAEndcap->Integral(9,15);
         if(N2EndcapSig<minN2EndcapSig) minN2EndcapSig = N2EndcapSig;
         if(N2EndcapSig>maxN2EndcapSig) maxN2EndcapSig = N2EndcapSig;
         double nMuMuEndcapSig = N0EndcapSig*matrix[0][0]+N1EndcapSig*matrix[0][1]+N2EndcapSig*matrix[0][2];
         if(nMuMuEndcapSig<minnMuMuEndcapSig) minnMuMuEndcapSig = nMuMuEndcapSig;
         if(nMuMuEndcapSig>maxnMuMuEndcapSig) maxnMuMuEndcapSig = nMuMuEndcapSig;
         double nMuHEndcapSig = N0EndcapSig*matrix[1][0]+N1EndcapSig*matrix[1][1]+N2EndcapSig*matrix[1][2];
         if(nMuHEndcapSig<minnMuHEndcapSig) minnMuHEndcapSig = nMuHEndcapSig;
         if(nMuHEndcapSig>maxnMuHEndcapSig) maxnMuHEndcapSig = nMuHEndcapSig;
         double nHHEndcapSig = N0EndcapSig*matrix[2][0]+N1EndcapSig*matrix[2][1]+N2EndcapSig*matrix[2][2];
         if(nHHEndcapSig<minnHHEndcapSig) minnHHEndcapSig = nHHEndcapSig;
         if(nHHEndcapSig>maxnHHEndcapSig) maxnHHEndcapSig = nHHEndcapSig;
         double nMuMuMVAEndcapSig = nMuMuEndcapSig*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapSig<minnMuMuMVAEndcapSig) minnMuMuMVAEndcapSig = nMuMuMVAEndcapSig;
         if(nMuMuMVAEndcapSig>maxnMuMuMVAEndcapSig) maxnMuMuMVAEndcapSig = nMuMuMVAEndcapSig;
         double nMuHMVAEndcapSig = nMuHEndcapSig*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapSig<minnMuHMVAEndcapSig) minnMuHMVAEndcapSig = nMuHMVAEndcapSig;
         if(nMuHMVAEndcapSig>maxnMuHMVAEndcapSig) maxnMuHMVAEndcapSig = nMuHMVAEndcapSig;
         double nHHMVAEndcapSig = nHHEndcapSig*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapSig<minnHHMVAEndcapSig) minnHHMVAEndcapSig = nHHMVAEndcapSig;
         if(nHHMVAEndcapSig>maxnHHMVAEndcapSig) maxnHHMVAEndcapSig = nHHMVAEndcapSig;
         
         std::cout << "\nN0EndcapSig = " << N0EndcapSig << std::endl;
         std::cout << "N1EndcapSig = " << N1EndcapSig << std::endl;
         std::cout << "N2EndcapSig = " << N2EndcapSig << std::endl;
         
         std::cout << "nMuMuEndcapSig = " << nMuMuEndcapSig << std::endl;
         std::cout << "nMuHEndcapSig = " << nMuHEndcapSig << std::endl;
         std::cout << "nHHEndcapSig = " << nHHEndcapSig << std::endl;
         std::cout << "nMuMuMVAEndcapSig = " << nMuMuMVAEndcapSig << std::endl;
         std::cout << "nMuHMVAEndcapSig = " << nMuHMVAEndcapSig << std::endl;
         std::cout << "nHHMVAEndcapSig = " << nHHMVAEndcapSig << std::endl;
         
         int N0EndcapRSB=hMassFail2Endcap->Integral(16,25);
         if(N0EndcapRSB<minN0EndcapRSB) minN0EndcapRSB = N0EndcapRSB;
         if(N0EndcapRSB>maxN0EndcapRSB) maxN0EndcapRSB = N0EndcapRSB;
         int N1EndcapRSB=hMassFail1Endcap->Integral(16,25);
         if(N1EndcapRSB<minN1EndcapRSB) minN1EndcapRSB = N1EndcapRSB;
         if(N1EndcapRSB>maxN1EndcapRSB) maxN1EndcapRSB = N1EndcapRSB;
         int N2EndcapRSB=hMassMVAEndcap->Integral(16,25);
         if(N2EndcapRSB<minN2EndcapRSB) minN2EndcapRSB = N2EndcapRSB;
         if(N2EndcapRSB>maxN2EndcapRSB) maxN2EndcapRSB = N2EndcapRSB;
         double nMuMuEndcapRSB = N0EndcapRSB*matrix[0][0]+N1EndcapRSB*matrix[0][1]+N2EndcapRSB*matrix[0][2];
         if(nMuMuEndcapRSB<minnMuMuEndcapRSB) minnMuMuEndcapRSB = nMuMuEndcapRSB;
         if(nMuMuEndcapRSB>maxnMuMuEndcapRSB) maxnMuMuEndcapRSB = nMuMuEndcapRSB;
         double nMuHEndcapRSB = N0EndcapRSB*matrix[1][0]+N1EndcapRSB*matrix[1][1]+N2EndcapRSB*matrix[1][2];
         if(nMuHEndcapRSB<minnMuHEndcapRSB) minnMuHEndcapRSB = nMuHEndcapRSB;
         if(nMuHEndcapRSB>maxnMuHEndcapRSB) maxnMuHEndcapRSB = nMuHEndcapRSB;
         double nHHEndcapRSB = N0EndcapRSB*matrix[2][0]+N1EndcapRSB*matrix[2][1]+N2EndcapRSB*matrix[2][2];
         if(nHHEndcapRSB<minnHHEndcapRSB) minnHHEndcapRSB = nHHEndcapRSB;
         if(nHHEndcapRSB>maxnHHEndcapRSB) maxnHHEndcapRSB = nHHEndcapRSB;
         double nMuMuMVAEndcapRSB = nMuMuEndcapRSB*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapRSB<minnMuMuMVAEndcapRSB) minnMuMuMVAEndcapRSB = nMuMuMVAEndcapRSB;
         if(nMuMuMVAEndcapRSB>maxnMuMuMVAEndcapRSB) maxnMuMuMVAEndcapRSB = nMuMuMVAEndcapRSB;
         double nMuHMVAEndcapRSB = nMuHEndcapRSB*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapRSB<minnMuHMVAEndcapRSB) minnMuHMVAEndcapRSB = nMuHMVAEndcapRSB;
         if(nMuHMVAEndcapRSB>maxnMuHMVAEndcapRSB) maxnMuHMVAEndcapRSB = nMuHMVAEndcapRSB;
         double nHHMVAEndcapRSB = nHHEndcapRSB*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapRSB<minnHHMVAEndcapRSB) minnHHMVAEndcapRSB = nHHMVAEndcapRSB;
         if(nHHMVAEndcapRSB>maxnHHMVAEndcapRSB) maxnHHMVAEndcapRSB = nHHMVAEndcapRSB;
         
         std::cout << "\nN0EndcapRSB = " << N0EndcapRSB << std::endl;
         std::cout << "N1EndcapRSB = " << N1EndcapRSB << std::endl;
         std::cout << "N2EndcapRSB = " << N2EndcapRSB << std::endl;
         
         std::cout << "nMuMuEndcapRSB = " << nMuMuEndcapRSB << std::endl;
         std::cout << "nMuHEndcapRSB = " << nMuHEndcapRSB << std::endl;
         std::cout << "nHHEndcapRSB = " << nHHEndcapRSB << std::endl;
         std::cout << "nMuMuMVAEndcapRSB = " << nMuMuMVAEndcapRSB << std::endl;
         std::cout << "nMuHMVAEndcapRSB = " << nMuHMVAEndcapRSB << std::endl;
         std::cout << "nHHMVAEndcapRSB = " << nHHMVAEndcapRSB << std::endl;
         
         int N0BarrelAllCat1=hMassFail2BarrelCat1->Integral(1,25);
         if(N0BarrelAllCat1<minN0BarrelAllCat1) minN0BarrelAllCat1 = N0BarrelAllCat1;
         if(N0BarrelAllCat1>maxN0BarrelAllCat1) maxN0BarrelAllCat1 = N0BarrelAllCat1;
         int N1BarrelAllCat1=hMassFail1BarrelCat1->Integral(1,25);
         if(N1BarrelAllCat1<minN1BarrelAllCat1) minN1BarrelAllCat1 = N1BarrelAllCat1;
         if(N1BarrelAllCat1>maxN1BarrelAllCat1) maxN1BarrelAllCat1 = N1BarrelAllCat1;
         int N2BarrelAllCat1=hMassMVABarrelCat1->Integral(1,25);
         if(N2BarrelAllCat1<minN2BarrelAllCat1) minN2BarrelAllCat1 = N2BarrelAllCat1;
         if(N2BarrelAllCat1>maxN2BarrelAllCat1) maxN2BarrelAllCat1 = N2BarrelAllCat1;
         double nMuMuBarrelAllCat1 = N0BarrelAllCat1*matrix[0][0]+N1BarrelAllCat1*matrix[0][1]+N2BarrelAllCat1*matrix[0][2];
         if(nMuMuBarrelAllCat1<minnMuMuBarrelAllCat1) minnMuMuBarrelAllCat1 = nMuMuBarrelAllCat1;
         if(nMuMuBarrelAllCat1>maxnMuMuBarrelAllCat1) maxnMuMuBarrelAllCat1 = nMuMuBarrelAllCat1;
         double nMuHBarrelAllCat1 = N0BarrelAllCat1*matrix[1][0]+N1BarrelAllCat1*matrix[1][1]+N2BarrelAllCat1*matrix[1][2];
         if(nMuHBarrelAllCat1<minnMuHBarrelAllCat1) minnMuHBarrelAllCat1 = nMuHBarrelAllCat1;
         if(nMuHBarrelAllCat1>maxnMuHBarrelAllCat1) maxnMuHBarrelAllCat1 = nMuHBarrelAllCat1;
         double nHHBarrelAllCat1 = N0BarrelAllCat1*matrix[2][0]+N1BarrelAllCat1*matrix[2][1]+N2BarrelAllCat1*matrix[2][2];
         if(nHHBarrelAllCat1<minnHHBarrelAllCat1) minnHHBarrelAllCat1 = nHHBarrelAllCat1;
         if(nHHBarrelAllCat1>maxnHHBarrelAllCat1) maxnHHBarrelAllCat1 = nHHBarrelAllCat1;
         double nMuMuMVABarrelAllCat1 = nMuMuBarrelAllCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelAllCat1<minnMuMuMVABarrelAllCat1) minnMuMuMVABarrelAllCat1 = nMuMuMVABarrelAllCat1;
         if(nMuMuMVABarrelAllCat1>maxnMuMuMVABarrelAllCat1) maxnMuMuMVABarrelAllCat1 = nMuMuMVABarrelAllCat1;
         double nMuHMVABarrelAllCat1 = nMuHBarrelAllCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelAllCat1<minnMuHMVABarrelAllCat1) minnMuHMVABarrelAllCat1 = nMuHMVABarrelAllCat1;
         if(nMuHMVABarrelAllCat1>maxnMuHMVABarrelAllCat1) maxnMuHMVABarrelAllCat1 = nMuHMVABarrelAllCat1;
         double nHHMVABarrelAllCat1 = nHHBarrelAllCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelAllCat1<minnHHMVABarrelAllCat1) minnHHMVABarrelAllCat1 = nHHMVABarrelAllCat1;
         if(nHHMVABarrelAllCat1>maxnHHMVABarrelAllCat1) maxnHHMVABarrelAllCat1 = nHHMVABarrelAllCat1;
         
         std::cout << "\nN0BarrelAllCat1 = " << N0BarrelAllCat1 << std::endl;
         std::cout << "N1BarrelAllCat1 = " << N1BarrelAllCat1 << std::endl;
         std::cout << "N2BarrelAllCat1 = " << N2BarrelAllCat1 << std::endl;
         
         std::cout << "nMuMuBarrelAllCat1 = " << nMuMuBarrelAllCat1 << std::endl;
         std::cout << "nMuHBarrelAllCat1 = " << nMuHBarrelAllCat1 << std::endl;
         std::cout << "nHHBarrelAllCat1 = " << nHHBarrelAllCat1 << std::endl;
         std::cout << "nMuMuMVABarrelAllCat1 = " << nMuMuMVABarrelAllCat1 << std::endl;
         std::cout << "nMuHMVABarrelAllCat1 = " << nMuHMVABarrelAllCat1 << std::endl;
         std::cout << "nHHMVABarrelAllCat1 = " << nHHMVABarrelAllCat1 << std::endl;
         
         int N0BarrelLSBCat1=hMassFail2BarrelCat1->Integral(1,8);
         if(N0BarrelLSBCat1<minN0BarrelLSBCat1) minN0BarrelLSBCat1 = N0BarrelLSBCat1;
         if(N0BarrelLSBCat1>maxN0BarrelLSBCat1) maxN0BarrelLSBCat1 = N0BarrelLSBCat1;
         int N1BarrelLSBCat1=hMassFail1BarrelCat1->Integral(1,8);
         if(N1BarrelLSBCat1<minN1BarrelLSBCat1) minN1BarrelLSBCat1 = N1BarrelLSBCat1;
         if(N1BarrelLSBCat1>maxN1BarrelLSBCat1) maxN1BarrelLSBCat1 = N1BarrelLSBCat1;
         int N2BarrelLSBCat1=hMassMVABarrelCat1->Integral(1,8);
         if(N2BarrelLSBCat1<minN2BarrelLSBCat1) minN2BarrelLSBCat1 = N2BarrelLSBCat1;
         if(N2BarrelLSBCat1>maxN2BarrelLSBCat1) maxN2BarrelLSBCat1 = N2BarrelLSBCat1;
         double nMuMuBarrelLSBCat1 = N0BarrelLSBCat1*matrix[0][0]+N1BarrelLSBCat1*matrix[0][1]+N2BarrelLSBCat1*matrix[0][2];
         if(nMuMuBarrelLSBCat1<minnMuMuBarrelLSBCat1) minnMuMuBarrelLSBCat1 = nMuMuBarrelLSBCat1;
         if(nMuMuBarrelLSBCat1>maxnMuMuBarrelLSBCat1) maxnMuMuBarrelLSBCat1 = nMuMuBarrelLSBCat1;
         double nMuHBarrelLSBCat1 = N0BarrelLSBCat1*matrix[1][0]+N1BarrelLSBCat1*matrix[1][1]+N2BarrelLSBCat1*matrix[1][2];
         if(nMuHBarrelLSBCat1<minnMuHBarrelLSBCat1) minnMuHBarrelLSBCat1 = nMuHBarrelLSBCat1;
         if(nMuHBarrelLSBCat1>maxnMuHBarrelLSBCat1) maxnMuHBarrelLSBCat1 = nMuHBarrelLSBCat1;
         double nHHBarrelLSBCat1 = N0BarrelLSBCat1*matrix[2][0]+N1BarrelLSBCat1*matrix[2][1]+N2BarrelLSBCat1*matrix[2][2];
         if(nHHBarrelLSBCat1<minnHHBarrelLSBCat1) minnHHBarrelLSBCat1 = nHHBarrelLSBCat1;
         if(nHHBarrelLSBCat1>maxnHHBarrelLSBCat1) maxnHHBarrelLSBCat1 = nHHBarrelLSBCat1;
         double nMuMuMVABarrelLSBCat1 = nMuMuBarrelLSBCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelLSBCat1<minnMuMuMVABarrelLSBCat1) minnMuMuMVABarrelLSBCat1 = nMuMuMVABarrelLSBCat1;
         if(nMuMuMVABarrelLSBCat1>maxnMuMuMVABarrelLSBCat1) maxnMuMuMVABarrelLSBCat1 = nMuMuMVABarrelLSBCat1;
         double nMuHMVABarrelLSBCat1 = nMuHBarrelLSBCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelLSBCat1<minnMuHMVABarrelLSBCat1) minnMuHMVABarrelLSBCat1 = nMuHMVABarrelLSBCat1;
         if(nMuHMVABarrelLSBCat1>maxnMuHMVABarrelLSBCat1) maxnMuHMVABarrelLSBCat1 = nMuHMVABarrelLSBCat1;
         double nHHMVABarrelLSBCat1 = nHHBarrelLSBCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelLSBCat1<minnHHMVABarrelLSBCat1) minnHHMVABarrelLSBCat1 = nHHMVABarrelLSBCat1;
         if(nHHMVABarrelLSBCat1>maxnHHMVABarrelLSBCat1) maxnHHMVABarrelLSBCat1 = nHHMVABarrelLSBCat1;
         
         std::cout << "\nN0BarrelLSBCat1 = " << N0BarrelLSBCat1 << std::endl;
         std::cout << "N1BarrelLSBCat1 = " << N1BarrelLSBCat1 << std::endl;
         std::cout << "N2BarrelLSBCat1 = " << N2BarrelLSBCat1 << std::endl;
         
         std::cout << "nMuMuBarrelLSBCat1 = " << nMuMuBarrelLSBCat1 << std::endl;
         std::cout << "nMuHBarrelLSBCat1 = " << nMuHBarrelLSBCat1 << std::endl;
         std::cout << "nHHBarrelLSBCat1 = " << nHHBarrelLSBCat1 << std::endl;
         std::cout << "nMuMuMVABarrelLSBCat1 = " << nMuMuMVABarrelLSBCat1 << std::endl;
         std::cout << "nMuHMVABarrelLSBCat1 = " << nMuHMVABarrelLSBCat1 << std::endl;
         std::cout << "nHHMVABarrelLSBCat1 = " << nHHMVABarrelLSBCat1 << std::endl;
         
         int N0BarrelSigCat1=hMassFail2BarrelCat1->Integral(9,15);
         if(N0BarrelSigCat1<minN0BarrelSigCat1) minN0BarrelSigCat1 = N0BarrelSigCat1;
         if(N0BarrelSigCat1>maxN0BarrelSigCat1) maxN0BarrelSigCat1 = N0BarrelSigCat1;
         int N1BarrelSigCat1=hMassFail1BarrelCat1->Integral(9,15);
         if(N1BarrelSigCat1<minN1BarrelSigCat1) minN1BarrelSigCat1 = N1BarrelSigCat1;
         if(N1BarrelSigCat1>maxN1BarrelSigCat1) maxN1BarrelSigCat1 = N1BarrelSigCat1;
         int N2BarrelSigCat1=hMassMVABarrelCat1->Integral(9,15);
         if(N2BarrelSigCat1<minN2BarrelSigCat1) minN2BarrelSigCat1 = N2BarrelSigCat1;
         if(N2BarrelSigCat1>maxN2BarrelSigCat1) maxN2BarrelSigCat1 = N2BarrelSigCat1;
         double nMuMuBarrelSigCat1 = N0BarrelSigCat1*matrix[0][0]+N1BarrelSigCat1*matrix[0][1]+N2BarrelSigCat1*matrix[0][2];
         if(nMuMuBarrelSigCat1<minnMuMuBarrelSigCat1) minnMuMuBarrelSigCat1 = nMuMuBarrelSigCat1;
         if(nMuMuBarrelSigCat1>maxnMuMuBarrelSigCat1) maxnMuMuBarrelSigCat1 = nMuMuBarrelSigCat1;
         double nMuHBarrelSigCat1 = N0BarrelSigCat1*matrix[1][0]+N1BarrelSigCat1*matrix[1][1]+N2BarrelSigCat1*matrix[1][2];
         if(nMuHBarrelSigCat1<minnMuHBarrelSigCat1) minnMuHBarrelSigCat1 = nMuHBarrelSigCat1;
         if(nMuHBarrelSigCat1>maxnMuHBarrelSigCat1) maxnMuHBarrelSigCat1 = nMuHBarrelSigCat1;
         double nHHBarrelSigCat1 = N0BarrelSigCat1*matrix[2][0]+N1BarrelSigCat1*matrix[2][1]+N2BarrelSigCat1*matrix[2][2];
         if(nHHBarrelSigCat1<minnHHBarrelSigCat1) minnHHBarrelSigCat1 = nHHBarrelSigCat1;
         if(nHHBarrelSigCat1>maxnHHBarrelSigCat1) maxnHHBarrelSigCat1 = nHHBarrelSigCat1;
         double nMuMuMVABarrelSigCat1 = nMuMuBarrelSigCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelSigCat1<minnMuMuMVABarrelSigCat1) minnMuMuMVABarrelSigCat1 = nMuMuMVABarrelSigCat1;
         if(nMuMuMVABarrelSigCat1>maxnMuMuMVABarrelSigCat1) maxnMuMuMVABarrelSigCat1 = nMuMuMVABarrelSigCat1;
         double nMuHMVABarrelSigCat1 = nMuHBarrelSigCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelSigCat1<minnMuHMVABarrelSigCat1) minnMuHMVABarrelSigCat1 = nMuHMVABarrelSigCat1;
         if(nMuHMVABarrelSigCat1>maxnMuHMVABarrelSigCat1) maxnMuHMVABarrelSigCat1 = nMuHMVABarrelSigCat1;
         double nHHMVABarrelSigCat1 = nHHBarrelSigCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelSigCat1<minnHHMVABarrelSigCat1) minnHHMVABarrelSigCat1 = nHHMVABarrelSigCat1;
         if(nHHMVABarrelSigCat1>maxnHHMVABarrelSigCat1) maxnHHMVABarrelSigCat1 = nHHMVABarrelSigCat1;
         
         std::cout << "\nN0BarrelSigCat1 = " << N0BarrelSigCat1 << std::endl;
         std::cout << "N1BarrelSigCat1 = " << N1BarrelSigCat1 << std::endl;
         std::cout << "N2BarrelSigCat1 = " << N2BarrelSigCat1 << std::endl;
         
         std::cout << "nMuMuBarrelSigCat1 = " << nMuMuBarrelSigCat1 << std::endl;
         std::cout << "nMuHBarrelSigCat1 = " << nMuHBarrelSigCat1 << std::endl;
         std::cout << "nHHBarrelSigCat1 = " << nHHBarrelSigCat1 << std::endl;
         std::cout << "nMuMuMVABarrelSigCat1 = " << nMuMuMVABarrelSigCat1 << std::endl;
         std::cout << "nMuHMVABarrelSigCat1 = " << nMuHMVABarrelSigCat1 << std::endl;
         std::cout << "nHHMVABarrelSigCat1 = " << nHHMVABarrelSigCat1 << std::endl;
         
         int N0BarrelRSBCat1=hMassFail2BarrelCat1->Integral(16,25);
         if(N0BarrelRSBCat1<minN0BarrelRSBCat1) minN0BarrelRSBCat1 = N0BarrelRSBCat1;
         if(N0BarrelRSBCat1>maxN0BarrelRSBCat1) maxN0BarrelRSBCat1 = N0BarrelRSBCat1;
         int N1BarrelRSBCat1=hMassFail1BarrelCat1->Integral(16,25);
         if(N1BarrelRSBCat1<minN1BarrelRSBCat1) minN1BarrelRSBCat1 = N1BarrelRSBCat1;
         if(N1BarrelRSBCat1>maxN1BarrelRSBCat1) maxN1BarrelRSBCat1 = N1BarrelRSBCat1;
         int N2BarrelRSBCat1=hMassMVABarrelCat1->Integral(16,25);
         if(N2BarrelRSBCat1<minN2BarrelRSBCat1) minN2BarrelRSBCat1 = N2BarrelRSBCat1;
         if(N2BarrelRSBCat1>maxN2BarrelRSBCat1) maxN2BarrelRSBCat1 = N2BarrelRSBCat1;
         double nMuMuBarrelRSBCat1 = N0BarrelRSBCat1*matrix[0][0]+N1BarrelRSBCat1*matrix[0][1]+N2BarrelRSBCat1*matrix[0][2];
         if(nMuMuBarrelRSBCat1<minnMuMuBarrelRSBCat1) minnMuMuBarrelRSBCat1 = nMuMuBarrelRSBCat1;
         if(nMuMuBarrelRSBCat1>maxnMuMuBarrelRSBCat1) maxnMuMuBarrelRSBCat1 = nMuMuBarrelRSBCat1;
         double nMuHBarrelRSBCat1 = N0BarrelRSBCat1*matrix[1][0]+N1BarrelRSBCat1*matrix[1][1]+N2BarrelRSBCat1*matrix[1][2];
         if(nMuHBarrelRSBCat1<minnMuHBarrelRSBCat1) minnMuHBarrelRSBCat1 = nMuHBarrelRSBCat1;
         if(nMuHBarrelRSBCat1>maxnMuHBarrelRSBCat1) maxnMuHBarrelRSBCat1 = nMuHBarrelRSBCat1;
         double nHHBarrelRSBCat1 = N0BarrelRSBCat1*matrix[2][0]+N1BarrelRSBCat1*matrix[2][1]+N2BarrelRSBCat1*matrix[2][2];
         if(nHHBarrelRSBCat1<minnHHBarrelRSBCat1) minnHHBarrelRSBCat1 = nHHBarrelRSBCat1;
         if(nHHBarrelRSBCat1>maxnHHBarrelRSBCat1) maxnHHBarrelRSBCat1 = nHHBarrelRSBCat1;
         double nMuMuMVABarrelRSBCat1 = nMuMuBarrelRSBCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelRSBCat1<minnMuMuMVABarrelRSBCat1) minnMuMuMVABarrelRSBCat1 = nMuMuMVABarrelRSBCat1;
         if(nMuMuMVABarrelRSBCat1>maxnMuMuMVABarrelRSBCat1) maxnMuMuMVABarrelRSBCat1 = nMuMuMVABarrelRSBCat1;
         double nMuHMVABarrelRSBCat1 = nMuHBarrelRSBCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelRSBCat1<minnMuHMVABarrelRSBCat1) minnMuHMVABarrelRSBCat1 = nMuHMVABarrelRSBCat1;
         if(nMuHMVABarrelRSBCat1>maxnMuHMVABarrelRSBCat1) maxnMuHMVABarrelRSBCat1 = nMuHMVABarrelRSBCat1;
         double nHHMVABarrelRSBCat1 = nHHBarrelRSBCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelRSBCat1<minnHHMVABarrelRSBCat1) minnHHMVABarrelRSBCat1 = nHHMVABarrelRSBCat1;
         if(nHHMVABarrelRSBCat1>maxnHHMVABarrelRSBCat1) maxnHHMVABarrelRSBCat1 = nHHMVABarrelRSBCat1;
         
         std::cout << "\nN0BarrelRSBCat1 = " << N0BarrelRSBCat1 << std::endl;
         std::cout << "N1BarrelRSBCat1 = " << N1BarrelRSBCat1 << std::endl;
         std::cout << "N2BarrelRSBCat1 = " << N2BarrelRSBCat1 << std::endl;
         
         std::cout << "nMuMuBarrelRSBCat1 = " << nMuMuBarrelRSBCat1 << std::endl;
         std::cout << "nMuHBarrelRSBCat1 = " << nMuHBarrelRSBCat1 << std::endl;
         std::cout << "nHHBarrelRSBCat1 = " << nHHBarrelRSBCat1 << std::endl;
         std::cout << "nMuMuMVABarrelRSBCat1 = " << nMuMuMVABarrelRSBCat1 << std::endl;
         std::cout << "nMuHMVABarrelRSBCat1 = " << nMuHMVABarrelRSBCat1 << std::endl;
         std::cout << "nHHMVABarrelRSBCat1 = " << nHHMVABarrelRSBCat1 << std::endl;
        
         int N0EndcapAllCat1=hMassFail2EndcapCat1->Integral(1,25);
         if(N0EndcapAllCat1<minN0EndcapAllCat1) minN0EndcapAllCat1 = N0EndcapAllCat1;
         if(N0EndcapAllCat1>maxN0EndcapAllCat1) maxN0EndcapAllCat1 = N0EndcapAllCat1;
         int N1EndcapAllCat1=hMassFail1EndcapCat1->Integral(1,25);
         if(N1EndcapAllCat1<minN1EndcapAllCat1) minN1EndcapAllCat1 = N1EndcapAllCat1;
         if(N1EndcapAllCat1>maxN1EndcapAllCat1) maxN1EndcapAllCat1 = N1EndcapAllCat1;
         int N2EndcapAllCat1=hMassMVAEndcapCat1->Integral(1,25);
         if(N2EndcapAllCat1<minN2EndcapAllCat1) minN2EndcapAllCat1 = N2EndcapAllCat1;
         if(N2EndcapAllCat1>maxN2EndcapAllCat1) maxN2EndcapAllCat1 = N2EndcapAllCat1;
         double nMuMuEndcapAllCat1 = N0EndcapAllCat1*matrix[0][0]+N1EndcapAllCat1*matrix[0][1]+N2EndcapAllCat1*matrix[0][2];
         if(nMuMuEndcapAllCat1<minnMuMuEndcapAllCat1) minnMuMuEndcapAllCat1 = nMuMuEndcapAllCat1;
         if(nMuMuEndcapAllCat1>maxnMuMuEndcapAllCat1) maxnMuMuEndcapAllCat1 = nMuMuEndcapAllCat1;
         double nMuHEndcapAllCat1 = N0EndcapAllCat1*matrix[1][0]+N1EndcapAllCat1*matrix[1][1]+N2EndcapAllCat1*matrix[1][2];
         if(nMuHEndcapAllCat1<minnMuHEndcapAllCat1) minnMuHEndcapAllCat1 = nMuHEndcapAllCat1;
         if(nMuHEndcapAllCat1>maxnMuHEndcapAllCat1) maxnMuHEndcapAllCat1 = nMuHEndcapAllCat1;
         double nHHEndcapAllCat1 = N0EndcapAllCat1*matrix[2][0]+N1EndcapAllCat1*matrix[2][1]+N2EndcapAllCat1*matrix[2][2];
         if(nHHEndcapAllCat1<minnHHEndcapAllCat1) minnHHEndcapAllCat1 = nHHEndcapAllCat1;
         if(nHHEndcapAllCat1>maxnHHEndcapAllCat1) maxnHHEndcapAllCat1 = nHHEndcapAllCat1;
         double nMuMuMVAEndcapAllCat1 = nMuMuEndcapAllCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapAllCat1<minnMuMuMVAEndcapAllCat1) minnMuMuMVAEndcapAllCat1 = nMuMuMVAEndcapAllCat1;
         if(nMuMuMVAEndcapAllCat1>maxnMuMuMVAEndcapAllCat1) maxnMuMuMVAEndcapAllCat1 = nMuMuMVAEndcapAllCat1;
         double nMuHMVAEndcapAllCat1 = nMuHEndcapAllCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapAllCat1<minnMuHMVAEndcapAllCat1) minnMuHMVAEndcapAllCat1 = nMuHMVAEndcapAllCat1;
         if(nMuHMVAEndcapAllCat1>maxnMuHMVAEndcapAllCat1) maxnMuHMVAEndcapAllCat1 = nMuHMVAEndcapAllCat1;
         double nHHMVAEndcapAllCat1 = nHHEndcapAllCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapAllCat1<minnHHMVAEndcapAllCat1) minnHHMVAEndcapAllCat1 = nHHMVAEndcapAllCat1;
         if(nHHMVAEndcapAllCat1>maxnHHMVAEndcapAllCat1) maxnHHMVAEndcapAllCat1 = nHHMVAEndcapAllCat1;
         
         std::cout << "\nN0EndcapAllCat1 = " << N0EndcapAllCat1 << std::endl;
         std::cout << "N1EndcapAllCat1 = " << N1EndcapAllCat1 << std::endl;
         std::cout << "N2EndcapAllCat1 = " << N2EndcapAllCat1 << std::endl;
         
         std::cout << "nMuMuEndcapAllCat1 = " << nMuMuEndcapAllCat1 << std::endl;
         std::cout << "nMuHEndcapAllCat1 = " << nMuHEndcapAllCat1 << std::endl;
         std::cout << "nHHEndcapAllCat1 = " << nHHEndcapAllCat1 << std::endl;
         std::cout << "nMuMuMVAEndcapAllCat1 = " << nMuMuMVAEndcapAllCat1 << std::endl;
         std::cout << "nMuHMVAEndcapAllCat1 = " << nMuHMVAEndcapAllCat1 << std::endl;
         std::cout << "nHHMVAEndcapAllCat1 = " << nHHMVAEndcapAllCat1 << std::endl;
         
         int N0EndcapLSBCat1=hMassFail2EndcapCat1->Integral(1,8);
         if(N0EndcapLSBCat1<minN0EndcapLSBCat1) minN0EndcapLSBCat1 = N0EndcapLSBCat1;
         if(N0EndcapLSBCat1>maxN0EndcapLSBCat1) maxN0EndcapLSBCat1 = N0EndcapLSBCat1;
         int N1EndcapLSBCat1=hMassFail1EndcapCat1->Integral(1,8);
         if(N1EndcapLSBCat1<minN1EndcapLSBCat1) minN1EndcapLSBCat1 = N1EndcapLSBCat1;
         if(N1EndcapLSBCat1>maxN1EndcapLSBCat1) maxN1EndcapLSBCat1 = N1EndcapLSBCat1;
         int N2EndcapLSBCat1=hMassMVAEndcapCat1->Integral(1,8);
         if(N2EndcapLSBCat1<minN2EndcapLSBCat1) minN2EndcapLSBCat1 = N2EndcapLSBCat1;
         if(N2EndcapLSBCat1>maxN2EndcapLSBCat1) maxN2EndcapLSBCat1 = N2EndcapLSBCat1;
         double nMuMuEndcapLSBCat1 = N0EndcapLSBCat1*matrix[0][0]+N1EndcapLSBCat1*matrix[0][1]+N2EndcapLSBCat1*matrix[0][2];
         if(nMuMuEndcapLSBCat1<minnMuMuEndcapLSBCat1) minnMuMuEndcapLSBCat1 = nMuMuEndcapLSBCat1;
         if(nMuMuEndcapLSBCat1>maxnMuMuEndcapLSBCat1) maxnMuMuEndcapLSBCat1 = nMuMuEndcapLSBCat1;
         double nMuHEndcapLSBCat1 = N0EndcapLSBCat1*matrix[1][0]+N1EndcapLSBCat1*matrix[1][1]+N2EndcapLSBCat1*matrix[1][2];
         if(nMuHEndcapLSBCat1<minnMuHEndcapLSBCat1) minnMuHEndcapLSBCat1 = nMuHEndcapLSBCat1;
         if(nMuHEndcapLSBCat1>maxnMuHEndcapLSBCat1) maxnMuHEndcapLSBCat1 = nMuHEndcapLSBCat1;
         double nHHEndcapLSBCat1 = N0EndcapLSBCat1*matrix[2][0]+N1EndcapLSBCat1*matrix[2][1]+N2EndcapLSBCat1*matrix[2][2];
         if(nHHEndcapLSBCat1<minnHHEndcapLSBCat1) minnHHEndcapLSBCat1 = nHHEndcapLSBCat1;
         if(nHHEndcapLSBCat1>maxnHHEndcapLSBCat1) maxnHHEndcapLSBCat1 = nHHEndcapLSBCat1;
         double nMuMuMVAEndcapLSBCat1 = nMuMuEndcapLSBCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapLSBCat1<minnMuMuMVAEndcapLSBCat1) minnMuMuMVAEndcapLSBCat1 = nMuMuMVAEndcapLSBCat1;
         if(nMuMuMVAEndcapLSBCat1>maxnMuMuMVAEndcapLSBCat1) maxnMuMuMVAEndcapLSBCat1 = nMuMuMVAEndcapLSBCat1;
         double nMuHMVAEndcapLSBCat1 = nMuHEndcapLSBCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapLSBCat1<minnMuHMVAEndcapLSBCat1) minnMuHMVAEndcapLSBCat1 = nMuHMVAEndcapLSBCat1;
         if(nMuHMVAEndcapLSBCat1>maxnMuHMVAEndcapLSBCat1) maxnMuHMVAEndcapLSBCat1 = nMuHMVAEndcapLSBCat1;
         double nHHMVAEndcapLSBCat1 = nHHEndcapLSBCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapLSBCat1<minnHHMVAEndcapLSBCat1) minnHHMVAEndcapLSBCat1 = nHHMVAEndcapLSBCat1;
         if(nHHMVAEndcapLSBCat1>maxnHHMVAEndcapLSBCat1) maxnHHMVAEndcapLSBCat1 = nHHMVAEndcapLSBCat1;
         
         std::cout << "\nN0EndcapLSBCat1 = " << N0EndcapLSBCat1 << std::endl;
         std::cout << "N1EndcapLSBCat1 = " << N1EndcapLSBCat1 << std::endl;
         std::cout << "N2EndcapLSBCat1 = " << N2EndcapLSBCat1 << std::endl;
         
         std::cout << "nMuMuEndcapLSBCat1 = " << nMuMuEndcapLSBCat1 << std::endl;
         std::cout << "nMuHEndcapLSBCat1 = " << nMuHEndcapLSBCat1 << std::endl;
         std::cout << "nHHEndcapLSBCat1 = " << nHHEndcapLSBCat1 << std::endl;
         std::cout << "nMuMuMVAEndcapLSBCat1 = " << nMuMuMVAEndcapLSBCat1 << std::endl;
         std::cout << "nMuHMVAEndcapLSBCat1 = " << nMuHMVAEndcapLSBCat1 << std::endl;
         std::cout << "nHHMVAEndcapLSBCat1 = " << nHHMVAEndcapLSBCat1 << std::endl;

         int N0EndcapSigCat1=hMassFail2EndcapCat1->Integral(9,15);
         if(N0EndcapSigCat1<minN0EndcapSigCat1) minN0EndcapSigCat1 = N0EndcapSigCat1;
         if(N0EndcapSigCat1>maxN0EndcapSigCat1) maxN0EndcapSigCat1 = N0EndcapSigCat1;
         int N1EndcapSigCat1=hMassFail1EndcapCat1->Integral(9,15);
         if(N1EndcapSigCat1<minN1EndcapSigCat1) minN1EndcapSigCat1 = N1EndcapSigCat1;
         if(N1EndcapSigCat1>maxN1EndcapSigCat1) maxN1EndcapSigCat1 = N1EndcapSigCat1;
         int N2EndcapSigCat1=hMassMVAEndcapCat1->Integral(9,15);
         if(N2EndcapSigCat1<minN2EndcapSigCat1) minN2EndcapSigCat1 = N2EndcapSigCat1;
         if(N2EndcapSigCat1>maxN2EndcapSigCat1) maxN2EndcapSigCat1 = N2EndcapSigCat1;
         double nMuMuEndcapSigCat1 = N0EndcapSigCat1*matrix[0][0]+N1EndcapSigCat1*matrix[0][1]+N2EndcapSigCat1*matrix[0][2];
         if(nMuMuEndcapSigCat1<minnMuMuEndcapSigCat1) minnMuMuEndcapSigCat1 = nMuMuEndcapSigCat1;
         if(nMuMuEndcapSigCat1>maxnMuMuEndcapSigCat1) maxnMuMuEndcapSigCat1 = nMuMuEndcapSigCat1;
         double nMuHEndcapSigCat1 = N0EndcapSigCat1*matrix[1][0]+N1EndcapSigCat1*matrix[1][1]+N2EndcapSigCat1*matrix[1][2];
         if(nMuHEndcapSigCat1<minnMuHEndcapSigCat1) minnMuHEndcapSigCat1 = nMuHEndcapSigCat1;
         if(nMuHEndcapSigCat1>maxnMuHEndcapSigCat1) maxnMuHEndcapSigCat1 = nMuHEndcapSigCat1;
         double nHHEndcapSigCat1 = N0EndcapSigCat1*matrix[2][0]+N1EndcapSigCat1*matrix[2][1]+N2EndcapSigCat1*matrix[2][2];
         if(nHHEndcapSigCat1<minnHHEndcapSigCat1) minnHHEndcapSigCat1 = nHHEndcapSigCat1;
         if(nHHEndcapSigCat1>maxnHHEndcapSigCat1) maxnHHEndcapSigCat1 = nHHEndcapSigCat1;
         double nMuMuMVAEndcapSigCat1 = nMuMuEndcapSigCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapSigCat1<minnMuMuMVAEndcapSigCat1) minnMuMuMVAEndcapSigCat1 = nMuMuMVAEndcapSigCat1;
         if(nMuMuMVAEndcapSigCat1>maxnMuMuMVAEndcapSigCat1) maxnMuMuMVAEndcapSigCat1 = nMuMuMVAEndcapSigCat1;
         double nMuHMVAEndcapSigCat1 = nMuHEndcapSigCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapSigCat1<minnMuHMVAEndcapSigCat1) minnMuHMVAEndcapSigCat1 = nMuHMVAEndcapSigCat1;
         if(nMuHMVAEndcapSigCat1>maxnMuHMVAEndcapSigCat1) maxnMuHMVAEndcapSigCat1 = nMuHMVAEndcapSigCat1;
         double nHHMVAEndcapSigCat1 = nHHEndcapSigCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapSigCat1<minnHHMVAEndcapSigCat1) minnHHMVAEndcapSigCat1 = nHHMVAEndcapSigCat1;
         if(nHHMVAEndcapSigCat1>maxnHHMVAEndcapSigCat1) maxnHHMVAEndcapSigCat1 = nHHMVAEndcapSigCat1;
         
         std::cout << "\nN0EndcapSigCat1 = " << N0EndcapSigCat1 << std::endl;
         std::cout << "N1EndcapSigCat1 = " << N1EndcapSigCat1 << std::endl;
         std::cout << "N2EndcapSigCat1 = " << N2EndcapSigCat1 << std::endl;
         
         std::cout << "nMuMuEndcapSigCat1 = " << nMuMuEndcapSigCat1 << std::endl;
         std::cout << "nMuHEndcapSigCat1 = " << nMuHEndcapSigCat1 << std::endl;
         std::cout << "nHHEndcapSigCat1 = " << nHHEndcapSigCat1 << std::endl;
         std::cout << "nMuMuMVAEndcapSigCat1 = " << nMuMuMVAEndcapSigCat1 << std::endl;
         std::cout << "nMuHMVAEndcapSigCat1 = " << nMuHMVAEndcapSigCat1 << std::endl;
         std::cout << "nHHMVAEndcapSigCat1 = " << nHHMVAEndcapSigCat1 << std::endl;
         
         int N0EndcapRSBCat1=hMassFail2EndcapCat1->Integral(16,25);
         if(N0EndcapRSBCat1<minN0EndcapRSBCat1) minN0EndcapRSBCat1 = N0EndcapRSBCat1;
         if(N0EndcapRSBCat1>maxN0EndcapRSBCat1) maxN0EndcapRSBCat1 = N0EndcapRSBCat1;
         int N1EndcapRSBCat1=hMassFail1EndcapCat1->Integral(16,25);
         if(N1EndcapRSBCat1<minN1EndcapRSBCat1) minN1EndcapRSBCat1 = N1EndcapRSBCat1;
         if(N1EndcapRSBCat1>maxN1EndcapRSBCat1) maxN1EndcapRSBCat1 = N1EndcapRSBCat1;
         int N2EndcapRSBCat1=hMassMVAEndcapCat1->Integral(16,25);
         if(N2EndcapRSBCat1<minN2EndcapRSBCat1) minN2EndcapRSBCat1 = N2EndcapRSBCat1;
         if(N2EndcapRSBCat1>maxN2EndcapRSBCat1) maxN2EndcapRSBCat1 = N2EndcapRSBCat1;
         double nMuMuEndcapRSBCat1 = N0EndcapRSBCat1*matrix[0][0]+N1EndcapRSBCat1*matrix[0][1]+N2EndcapRSBCat1*matrix[0][2];
         if(nMuMuEndcapRSBCat1<minnMuMuEndcapRSBCat1) minnMuMuEndcapRSBCat1 = nMuMuEndcapRSBCat1;
         if(nMuMuEndcapRSBCat1>maxnMuMuEndcapRSBCat1) maxnMuMuEndcapRSBCat1 = nMuMuEndcapRSBCat1;
         double nMuHEndcapRSBCat1 = N0EndcapRSBCat1*matrix[1][0]+N1EndcapRSBCat1*matrix[1][1]+N2EndcapRSBCat1*matrix[1][2];
         if(nMuHEndcapRSBCat1<minnMuHEndcapRSBCat1) minnMuHEndcapRSBCat1 = nMuHEndcapRSBCat1;
         if(nMuHEndcapRSBCat1>maxnMuHEndcapRSBCat1) maxnMuHEndcapRSBCat1 = nMuHEndcapRSBCat1;
         double nHHEndcapRSBCat1 = N0EndcapRSBCat1*matrix[2][0]+N1EndcapRSBCat1*matrix[2][1]+N2EndcapRSBCat1*matrix[2][2];
         if(nHHEndcapRSBCat1<minnHHEndcapRSBCat1) minnHHEndcapRSBCat1 = nHHEndcapRSBCat1;
         if(nHHEndcapRSBCat1>maxnHHEndcapRSBCat1) maxnHHEndcapRSBCat1 = nHHEndcapRSBCat1;
         double nMuMuMVAEndcapRSBCat1 = nMuMuEndcapRSBCat1*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapRSBCat1<minnMuMuMVAEndcapRSBCat1) minnMuMuMVAEndcapRSBCat1 = nMuMuMVAEndcapRSBCat1;
         if(nMuMuMVAEndcapRSBCat1>maxnMuMuMVAEndcapRSBCat1) maxnMuMuMVAEndcapRSBCat1 = nMuMuMVAEndcapRSBCat1;
         double nMuHMVAEndcapRSBCat1 = nMuHEndcapRSBCat1*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapRSBCat1<minnMuHMVAEndcapRSBCat1) minnMuHMVAEndcapRSBCat1 = nMuHMVAEndcapRSBCat1;
         if(nMuHMVAEndcapRSBCat1>maxnMuHMVAEndcapRSBCat1) maxnMuHMVAEndcapRSBCat1 = nMuHMVAEndcapRSBCat1;
         double nHHMVAEndcapRSBCat1 = nHHEndcapRSBCat1*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapRSBCat1<minnHHMVAEndcapRSBCat1) minnHHMVAEndcapRSBCat1 = nHHMVAEndcapRSBCat1;
         if(nHHMVAEndcapRSBCat1>maxnHHMVAEndcapRSBCat1) maxnHHMVAEndcapRSBCat1 = nHHMVAEndcapRSBCat1;
         
         std::cout << "\nN0EndcapRSBCat1 = " << N0EndcapRSBCat1 << std::endl;
         std::cout << "N1EndcapRSBCat1 = " << N1EndcapRSBCat1 << std::endl;
         std::cout << "N2EndcapRSBCat1 = " << N2EndcapRSBCat1 << std::endl;
         
         std::cout << "nMuMuEndcapRSBCat1 = " << nMuMuEndcapRSBCat1 << std::endl;
         std::cout << "nMuHEndcapRSBCat1 = " << nMuHEndcapRSBCat1 << std::endl;
         std::cout << "nHHEndcapRSBCat1 = " << nHHEndcapRSBCat1 << std::endl;
         std::cout << "nMuMuMVAEndcapRSBCat1 = " << nMuMuMVAEndcapRSBCat1 << std::endl;
         std::cout << "nMuHMVAEndcapRSBCat1 = " << nMuHMVAEndcapRSBCat1 << std::endl;
         std::cout << "nHHMVAEndcapRSBCat1 = " << nHHMVAEndcapRSBCat1 << std::endl;

         int N0BarrelAllCat2=hMassFail2BarrelCat2->Integral(1,25);
         if(N0BarrelAllCat2<minN0BarrelAllCat2) minN0BarrelAllCat2 = N0BarrelAllCat2;
         if(N0BarrelAllCat2>maxN0BarrelAllCat2) maxN0BarrelAllCat2 = N0BarrelAllCat2;
         int N1BarrelAllCat2=hMassFail1BarrelCat2->Integral(1,25);
         if(N1BarrelAllCat2<minN1BarrelAllCat2) minN1BarrelAllCat2 = N1BarrelAllCat2;
         if(N1BarrelAllCat2>maxN1BarrelAllCat2) maxN1BarrelAllCat2 = N1BarrelAllCat2;
         int N2BarrelAllCat2=hMassMVABarrelCat2->Integral(1,25);
         if(N2BarrelAllCat2<minN2BarrelAllCat2) minN2BarrelAllCat2 = N2BarrelAllCat2;
         if(N2BarrelAllCat2>maxN2BarrelAllCat2) maxN2BarrelAllCat2 = N2BarrelAllCat2;
         double nMuMuBarrelAllCat2 = N0BarrelAllCat2*matrix[0][0]+N1BarrelAllCat2*matrix[0][1]+N2BarrelAllCat2*matrix[0][2];
         if(nMuMuBarrelAllCat2<minnMuMuBarrelAllCat2) minnMuMuBarrelAllCat2 = nMuMuBarrelAllCat2;
         if(nMuMuBarrelAllCat2>maxnMuMuBarrelAllCat2) maxnMuMuBarrelAllCat2 = nMuMuBarrelAllCat2;
         double nMuHBarrelAllCat2 = N0BarrelAllCat2*matrix[1][0]+N1BarrelAllCat2*matrix[1][1]+N2BarrelAllCat2*matrix[1][2];
         if(nMuHBarrelAllCat2<minnMuHBarrelAllCat2) minnMuHBarrelAllCat2 = nMuHBarrelAllCat2;
         if(nMuHBarrelAllCat2>maxnMuHBarrelAllCat2) maxnMuHBarrelAllCat2 = nMuHBarrelAllCat2;
         double nHHBarrelAllCat2 = N0BarrelAllCat2*matrix[2][0]+N1BarrelAllCat2*matrix[2][1]+N2BarrelAllCat2*matrix[2][2];
         if(nHHBarrelAllCat2<minnHHBarrelAllCat2) minnHHBarrelAllCat2 = nHHBarrelAllCat2;
         if(nHHBarrelAllCat2>maxnHHBarrelAllCat2) maxnHHBarrelAllCat2 = nHHBarrelAllCat2;
         double nMuMuMVABarrelAllCat2 = nMuMuBarrelAllCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelAllCat2<minnMuMuMVABarrelAllCat2) minnMuMuMVABarrelAllCat2 = nMuMuMVABarrelAllCat2;
         if(nMuMuMVABarrelAllCat2>maxnMuMuMVABarrelAllCat2) maxnMuMuMVABarrelAllCat2 = nMuMuMVABarrelAllCat2;
         double nMuHMVABarrelAllCat2 = nMuHBarrelAllCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelAllCat2<minnMuHMVABarrelAllCat2) minnMuHMVABarrelAllCat2 = nMuHMVABarrelAllCat2;
         if(nMuHMVABarrelAllCat2>maxnMuHMVABarrelAllCat2) maxnMuHMVABarrelAllCat2 = nMuHMVABarrelAllCat2;
         double nHHMVABarrelAllCat2 = nHHBarrelAllCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelAllCat2<minnHHMVABarrelAllCat2) minnHHMVABarrelAllCat2 = nHHMVABarrelAllCat2;
         if(nHHMVABarrelAllCat2>maxnHHMVABarrelAllCat2) maxnHHMVABarrelAllCat2 = nHHMVABarrelAllCat2;
         
         std::cout << "\nN0BarrelAllCat2 = " << N0BarrelAllCat2 << std::endl;
         std::cout << "N1BarrelAllCat2 = " << N1BarrelAllCat2 << std::endl;
         std::cout << "N2BarrelAllCat2 = " << N2BarrelAllCat2 << std::endl;
         
         std::cout << "nMuMuBarrelAllCat2 = " << nMuMuBarrelAllCat2 << std::endl;
         std::cout << "nMuHBarrelAllCat2 = " << nMuHBarrelAllCat2 << std::endl;
         std::cout << "nHHBarrelAllCat2 = " << nHHBarrelAllCat2 << std::endl;
         std::cout << "nMuMuMVABarrelAllCat2 = " << nMuMuMVABarrelAllCat2 << std::endl;
         std::cout << "nMuHMVABarrelAllCat2 = " << nMuHMVABarrelAllCat2 << std::endl;
         std::cout << "nHHMVABarrelAllCat2 = " << nHHMVABarrelAllCat2 << std::endl;
         
         int N0BarrelLSBCat2=hMassFail2BarrelCat2->Integral(1,8);
         if(N0BarrelLSBCat2<minN0BarrelLSBCat2) minN0BarrelLSBCat2 = N0BarrelLSBCat2;
         if(N0BarrelLSBCat2>maxN0BarrelLSBCat2) maxN0BarrelLSBCat2 = N0BarrelLSBCat2;
         int N1BarrelLSBCat2=hMassFail1BarrelCat2->Integral(1,8);
         if(N1BarrelLSBCat2<minN1BarrelLSBCat2) minN1BarrelLSBCat2 = N1BarrelLSBCat2;
         if(N1BarrelLSBCat2>maxN1BarrelLSBCat2) maxN1BarrelLSBCat2 = N1BarrelLSBCat2;
         int N2BarrelLSBCat2=hMassMVABarrelCat2->Integral(1,8);
         if(N2BarrelLSBCat2<minN2BarrelLSBCat2) minN2BarrelLSBCat2 = N2BarrelLSBCat2;
         if(N2BarrelLSBCat2>maxN2BarrelLSBCat2) maxN2BarrelLSBCat2 = N2BarrelLSBCat2;
         double nMuMuBarrelLSBCat2 = N0BarrelLSBCat2*matrix[0][0]+N1BarrelLSBCat2*matrix[0][1]+N2BarrelLSBCat2*matrix[0][2];
         if(nMuMuBarrelLSBCat2<minnMuMuBarrelLSBCat2) minnMuMuBarrelLSBCat2 = nMuMuBarrelLSBCat2;
         if(nMuMuBarrelLSBCat2>maxnMuMuBarrelLSBCat2) maxnMuMuBarrelLSBCat2 = nMuMuBarrelLSBCat2;
         double nMuHBarrelLSBCat2 = N0BarrelLSBCat2*matrix[1][0]+N1BarrelLSBCat2*matrix[1][1]+N2BarrelLSBCat2*matrix[1][2];
         if(nMuHBarrelLSBCat2<minnMuHBarrelLSBCat2) minnMuHBarrelLSBCat2 = nMuHBarrelLSBCat2;
         if(nMuHBarrelLSBCat2>maxnMuHBarrelLSBCat2) maxnMuHBarrelLSBCat2 = nMuHBarrelLSBCat2;
         double nHHBarrelLSBCat2 = N0BarrelLSBCat2*matrix[2][0]+N1BarrelLSBCat2*matrix[2][1]+N2BarrelLSBCat2*matrix[2][2];
         if(nHHBarrelLSBCat2<minnHHBarrelLSBCat2) minnHHBarrelLSBCat2 = nHHBarrelLSBCat2;
         if(nHHBarrelLSBCat2>maxnHHBarrelLSBCat2) maxnHHBarrelLSBCat2 = nHHBarrelLSBCat2;
         double nMuMuMVABarrelLSBCat2 = nMuMuBarrelLSBCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelLSBCat2<minnMuMuMVABarrelLSBCat2) minnMuMuMVABarrelLSBCat2 = nMuMuMVABarrelLSBCat2;
         if(nMuMuMVABarrelLSBCat2>maxnMuMuMVABarrelLSBCat2) maxnMuMuMVABarrelLSBCat2 = nMuMuMVABarrelLSBCat2;
         double nMuHMVABarrelLSBCat2 = nMuHBarrelLSBCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelLSBCat2<minnMuHMVABarrelLSBCat2) minnMuHMVABarrelLSBCat2 = nMuHMVABarrelLSBCat2;
         if(nMuHMVABarrelLSBCat2>maxnMuHMVABarrelLSBCat2) maxnMuHMVABarrelLSBCat2 = nMuHMVABarrelLSBCat2;
         double nHHMVABarrelLSBCat2 = nHHBarrelLSBCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelLSBCat2<minnHHMVABarrelLSBCat2) minnHHMVABarrelLSBCat2 = nHHMVABarrelLSBCat2;
         if(nHHMVABarrelLSBCat2>maxnHHMVABarrelLSBCat2) maxnHHMVABarrelLSBCat2 = nHHMVABarrelLSBCat2;
         
         std::cout << "\nN0BarrelLSBCat2 = " << N0BarrelLSBCat2 << std::endl;
         std::cout << "N1BarrelLSBCat2 = " << N1BarrelLSBCat2 << std::endl;
         std::cout << "N2BarrelLSBCat2 = " << N2BarrelLSBCat2 << std::endl;
         
         std::cout << "nMuMuBarrelLSBCat2 = " << nMuMuBarrelLSBCat2 << std::endl;
         std::cout << "nMuHBarrelLSBCat2 = " << nMuHBarrelLSBCat2 << std::endl;
         std::cout << "nHHBarrelLSBCat2 = " << nHHBarrelLSBCat2 << std::endl;
         std::cout << "nMuMuMVABarrelLSBCat2 = " << nMuMuMVABarrelLSBCat2 << std::endl;
         std::cout << "nMuHMVABarrelLSBCat2 = " << nMuHMVABarrelLSBCat2 << std::endl;
         std::cout << "nHHMVABarrelLSBCat2 = " << nHHMVABarrelLSBCat2 << std::endl;
         
         int N0BarrelSigCat2=hMassFail2BarrelCat2->Integral(9,15);
         if(N0BarrelSigCat2<minN0BarrelSigCat2) minN0BarrelSigCat2 = N0BarrelSigCat2;
         if(N0BarrelSigCat2>maxN0BarrelSigCat2) maxN0BarrelSigCat2 = N0BarrelSigCat2;
         int N1BarrelSigCat2=hMassFail1BarrelCat2->Integral(9,15);
         if(N1BarrelSigCat2<minN1BarrelSigCat2) minN1BarrelSigCat2 = N1BarrelSigCat2;
         if(N1BarrelSigCat2>maxN1BarrelSigCat2) maxN1BarrelSigCat2 = N1BarrelSigCat2;
         int N2BarrelSigCat2=hMassMVABarrelCat2->Integral(9,15);
         if(N2BarrelSigCat2<minN2BarrelSigCat2) minN2BarrelSigCat2 = N2BarrelSigCat2;
         if(N2BarrelSigCat2>maxN2BarrelSigCat2) maxN2BarrelSigCat2 = N2BarrelSigCat2;
         double nMuMuBarrelSigCat2 = N0BarrelSigCat2*matrix[0][0]+N1BarrelSigCat2*matrix[0][1]+N2BarrelSigCat2*matrix[0][2];
         if(nMuMuBarrelSigCat2<minnMuMuBarrelSigCat2) minnMuMuBarrelSigCat2 = nMuMuBarrelSigCat2;
         if(nMuMuBarrelSigCat2>maxnMuMuBarrelSigCat2) maxnMuMuBarrelSigCat2 = nMuMuBarrelSigCat2;
         double nMuHBarrelSigCat2 = N0BarrelSigCat2*matrix[1][0]+N1BarrelSigCat2*matrix[1][1]+N2BarrelSigCat2*matrix[1][2];
         if(nMuHBarrelSigCat2<minnMuHBarrelSigCat2) minnMuHBarrelSigCat2 = nMuHBarrelSigCat2;
         if(nMuHBarrelSigCat2>maxnMuHBarrelSigCat2) maxnMuHBarrelSigCat2 = nMuHBarrelSigCat2;
         double nHHBarrelSigCat2 = N0BarrelSigCat2*matrix[2][0]+N1BarrelSigCat2*matrix[2][1]+N2BarrelSigCat2*matrix[2][2];
         if(nHHBarrelSigCat2<minnHHBarrelSigCat2) minnHHBarrelSigCat2 = nHHBarrelSigCat2;
         if(nHHBarrelSigCat2>maxnHHBarrelSigCat2) maxnHHBarrelSigCat2 = nHHBarrelSigCat2;
         double nMuMuMVABarrelSigCat2 = nMuMuBarrelSigCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelSigCat2<minnMuMuMVABarrelSigCat2) minnMuMuMVABarrelSigCat2 = nMuMuMVABarrelSigCat2;
         if(nMuMuMVABarrelSigCat2>maxnMuMuMVABarrelSigCat2) maxnMuMuMVABarrelSigCat2 = nMuMuMVABarrelSigCat2;
         double nMuHMVABarrelSigCat2 = nMuHBarrelSigCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelSigCat2<minnMuHMVABarrelSigCat2) minnMuHMVABarrelSigCat2 = nMuHMVABarrelSigCat2;
         if(nMuHMVABarrelSigCat2>maxnMuHMVABarrelSigCat2) maxnMuHMVABarrelSigCat2 = nMuHMVABarrelSigCat2;
         double nHHMVABarrelSigCat2 = nHHBarrelSigCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelSigCat2<minnHHMVABarrelSigCat2) minnHHMVABarrelSigCat2 = nHHMVABarrelSigCat2;
         if(nHHMVABarrelSigCat2>maxnHHMVABarrelSigCat2) maxnHHMVABarrelSigCat2 = nHHMVABarrelSigCat2;
         
         std::cout << "\nN0BarrelSigCat2 = " << N0BarrelSigCat2 << std::endl;
         std::cout << "N1BarrelSigCat2 = " << N1BarrelSigCat2 << std::endl;
         std::cout << "N2BarrelSigCat2 = " << N2BarrelSigCat2 << std::endl;
         
         std::cout << "nMuMuBarrelSigCat2 = " << nMuMuBarrelSigCat2 << std::endl;
         std::cout << "nMuHBarrelSigCat2 = " << nMuHBarrelSigCat2 << std::endl;
         std::cout << "nHHBarrelSigCat2 = " << nHHBarrelSigCat2 << std::endl;
         std::cout << "nMuMuMVABarrelSigCat2 = " << nMuMuMVABarrelSigCat2 << std::endl;
         std::cout << "nMuHMVABarrelSigCat2 = " << nMuHMVABarrelSigCat2 << std::endl;
         std::cout << "nHHMVABarrelSigCat2 = " << nHHMVABarrelSigCat2 << std::endl;
         
         int N0BarrelRSBCat2=hMassFail2BarrelCat2->Integral(16,25);
         if(N0BarrelRSBCat2<minN0BarrelRSBCat2) minN0BarrelRSBCat2 = N0BarrelRSBCat2;
         if(N0BarrelRSBCat2>maxN0BarrelRSBCat2) maxN0BarrelRSBCat2 = N0BarrelRSBCat2;
         int N1BarrelRSBCat2=hMassFail1BarrelCat2->Integral(16,25);
         if(N1BarrelRSBCat2<minN1BarrelRSBCat2) minN1BarrelRSBCat2 = N1BarrelRSBCat2;
         if(N1BarrelRSBCat2>maxN1BarrelRSBCat2) maxN1BarrelRSBCat2 = N1BarrelRSBCat2;
         int N2BarrelRSBCat2=hMassMVABarrelCat2->Integral(16,25);
         if(N2BarrelRSBCat2<minN2BarrelRSBCat2) minN2BarrelRSBCat2 = N2BarrelRSBCat2;
         if(N2BarrelRSBCat2>maxN2BarrelRSBCat2) maxN2BarrelRSBCat2 = N2BarrelRSBCat2;
         double nMuMuBarrelRSBCat2 = N0BarrelRSBCat2*matrix[0][0]+N1BarrelRSBCat2*matrix[0][1]+N2BarrelRSBCat2*matrix[0][2];
         if(nMuMuBarrelRSBCat2<minnMuMuBarrelRSBCat2) minnMuMuBarrelRSBCat2 = nMuMuBarrelRSBCat2;
         if(nMuMuBarrelRSBCat2>maxnMuMuBarrelRSBCat2) maxnMuMuBarrelRSBCat2 = nMuMuBarrelRSBCat2;
         double nMuHBarrelRSBCat2 = N0BarrelRSBCat2*matrix[1][0]+N1BarrelRSBCat2*matrix[1][1]+N2BarrelRSBCat2*matrix[1][2];
         if(nMuHBarrelRSBCat2<minnMuHBarrelRSBCat2) minnMuHBarrelRSBCat2 = nMuHBarrelRSBCat2;
         if(nMuHBarrelRSBCat2>maxnMuHBarrelRSBCat2) maxnMuHBarrelRSBCat2 = nMuHBarrelRSBCat2;
         double nHHBarrelRSBCat2 = N0BarrelRSBCat2*matrix[2][0]+N1BarrelRSBCat2*matrix[2][1]+N2BarrelRSBCat2*matrix[2][2];
         if(nHHBarrelRSBCat2<minnHHBarrelRSBCat2) minnHHBarrelRSBCat2 = nHHBarrelRSBCat2;
         if(nHHBarrelRSBCat2>maxnHHBarrelRSBCat2) maxnHHBarrelRSBCat2 = nHHBarrelRSBCat2;
         double nMuMuMVABarrelRSBCat2 = nMuMuBarrelRSBCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelRSBCat2<minnMuMuMVABarrelRSBCat2) minnMuMuMVABarrelRSBCat2 = nMuMuMVABarrelRSBCat2;
         if(nMuMuMVABarrelRSBCat2>maxnMuMuMVABarrelRSBCat2) maxnMuMuMVABarrelRSBCat2 = nMuMuMVABarrelRSBCat2;
         double nMuHMVABarrelRSBCat2 = nMuHBarrelRSBCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelRSBCat2<minnMuHMVABarrelRSBCat2) minnMuHMVABarrelRSBCat2 = nMuHMVABarrelRSBCat2;
         if(nMuHMVABarrelRSBCat2>maxnMuHMVABarrelRSBCat2) maxnMuHMVABarrelRSBCat2 = nMuHMVABarrelRSBCat2;
         double nHHMVABarrelRSBCat2 = nHHBarrelRSBCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelRSBCat2<minnHHMVABarrelRSBCat2) minnHHMVABarrelRSBCat2 = nHHMVABarrelRSBCat2;
         if(nHHMVABarrelRSBCat2>maxnHHMVABarrelRSBCat2) maxnHHMVABarrelRSBCat2 = nHHMVABarrelRSBCat2;
         
         std::cout << "\nN0BarrelRSBCat2 = " << N0BarrelRSBCat2 << std::endl;
         std::cout << "N1BarrelRSBCat2 = " << N1BarrelRSBCat2 << std::endl;
         std::cout << "N2BarrelRSBCat2 = " << N2BarrelRSBCat2 << std::endl;
         
         std::cout << "nMuMuBarrelRSBCat2 = " << nMuMuBarrelRSBCat2 << std::endl;
         std::cout << "nMuHBarrelRSBCat2 = " << nMuHBarrelRSBCat2 << std::endl;
         std::cout << "nHHBarrelRSBCat2 = " << nHHBarrelRSBCat2 << std::endl;
         std::cout << "nMuMuMVABarrelRSBCat2 = " << nMuMuMVABarrelRSBCat2 << std::endl;
         std::cout << "nMuHMVABarrelRSBCat2 = " << nMuHMVABarrelRSBCat2 << std::endl;
         std::cout << "nHHMVABarrelRSBCat2 = " << nHHMVABarrelRSBCat2 << std::endl;
         
         int N0EndcapAllCat2=hMassFail2EndcapCat2->Integral(1,25);
         if(N0EndcapAllCat2<minN0EndcapAllCat2) minN0EndcapAllCat2 = N0EndcapAllCat2;
         if(N0EndcapAllCat2>maxN0EndcapAllCat2) maxN0EndcapAllCat2 = N0EndcapAllCat2;
         int N1EndcapAllCat2=hMassFail1EndcapCat2->Integral(1,25);
         if(N1EndcapAllCat2<minN1EndcapAllCat2) minN1EndcapAllCat2 = N1EndcapAllCat2;
         if(N1EndcapAllCat2>maxN1EndcapAllCat2) maxN1EndcapAllCat2 = N1EndcapAllCat2;
         int N2EndcapAllCat2=hMassMVAEndcapCat2->Integral(1,25);
         if(N2EndcapAllCat2<minN2EndcapAllCat2) minN2EndcapAllCat2 = N2EndcapAllCat2;
         if(N2EndcapAllCat2>maxN2EndcapAllCat2) maxN2EndcapAllCat2 = N2EndcapAllCat2;
         double nMuMuEndcapAllCat2 = N0EndcapAllCat2*matrix[0][0]+N1EndcapAllCat2*matrix[0][1]+N2EndcapAllCat2*matrix[0][2];
         if(nMuMuEndcapAllCat2<minnMuMuEndcapAllCat2) minnMuMuEndcapAllCat2 = nMuMuEndcapAllCat2;
         if(nMuMuEndcapAllCat2>maxnMuMuEndcapAllCat2) maxnMuMuEndcapAllCat2 = nMuMuEndcapAllCat2;
         double nMuHEndcapAllCat2 = N0EndcapAllCat2*matrix[1][0]+N1EndcapAllCat2*matrix[1][1]+N2EndcapAllCat2*matrix[1][2];
         if(nMuHEndcapAllCat2<minnMuHEndcapAllCat2) minnMuHEndcapAllCat2 = nMuHEndcapAllCat2;
         if(nMuHEndcapAllCat2>maxnMuHEndcapAllCat2) maxnMuHEndcapAllCat2 = nMuHEndcapAllCat2;
         double nHHEndcapAllCat2 = N0EndcapAllCat2*matrix[2][0]+N1EndcapAllCat2*matrix[2][1]+N2EndcapAllCat2*matrix[2][2];
         if(nHHEndcapAllCat2<minnHHEndcapAllCat2) minnHHEndcapAllCat2 = nHHEndcapAllCat2;
         if(nHHEndcapAllCat2>maxnHHEndcapAllCat2) maxnHHEndcapAllCat2 = nHHEndcapAllCat2;
         double nMuMuMVAEndcapAllCat2 = nMuMuEndcapAllCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapAllCat2<minnMuMuMVAEndcapAllCat2) minnMuMuMVAEndcapAllCat2 = nMuMuMVAEndcapAllCat2;
         if(nMuMuMVAEndcapAllCat2>maxnMuMuMVAEndcapAllCat2) maxnMuMuMVAEndcapAllCat2 = nMuMuMVAEndcapAllCat2;
         double nMuHMVAEndcapAllCat2 = nMuHEndcapAllCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapAllCat2<minnMuHMVAEndcapAllCat2) minnMuHMVAEndcapAllCat2 = nMuHMVAEndcapAllCat2;
         if(nMuHMVAEndcapAllCat2>maxnMuHMVAEndcapAllCat2) maxnMuHMVAEndcapAllCat2 = nMuHMVAEndcapAllCat2;
         double nHHMVAEndcapAllCat2 = nHHEndcapAllCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapAllCat2<minnHHMVAEndcapAllCat2) minnHHMVAEndcapAllCat2 = nHHMVAEndcapAllCat2;
         if(nHHMVAEndcapAllCat2>maxnHHMVAEndcapAllCat2) maxnHHMVAEndcapAllCat2 = nHHMVAEndcapAllCat2;
         
         std::cout << "\nN0EndcapAllCat2 = " << N0EndcapAllCat2 << std::endl;
         std::cout << "N1EndcapAllCat2 = " << N1EndcapAllCat2 << std::endl;
         std::cout << "N2EndcapAllCat2 = " << N2EndcapAllCat2 << std::endl;
         
         std::cout << "nMuMuEndcapAllCat2 = " << nMuMuEndcapAllCat2 << std::endl;
         std::cout << "nMuHEndcapAllCat2 = " << nMuHEndcapAllCat2 << std::endl;
         std::cout << "nHHEndcapAllCat2 = " << nHHEndcapAllCat2 << std::endl;
         std::cout << "nMuMuMVAEndcapAllCat2 = " << nMuMuMVAEndcapAllCat2 << std::endl;
         std::cout << "nMuHMVAEndcapAllCat2 = " << nMuHMVAEndcapAllCat2 << std::endl;
         std::cout << "nHHMVAEndcapAllCat2 = " << nHHMVAEndcapAllCat2 << std::endl;
         
         int N0EndcapLSBCat2=hMassFail2EndcapCat2->Integral(1,8);
         if(N0EndcapLSBCat2<minN0EndcapLSBCat2) minN0EndcapLSBCat2 = N0EndcapLSBCat2;
         if(N0EndcapLSBCat2>maxN0EndcapLSBCat2) maxN0EndcapLSBCat2 = N0EndcapLSBCat2;
         int N1EndcapLSBCat2=hMassFail1EndcapCat2->Integral(1,8);
         if(N1EndcapLSBCat2<minN1EndcapLSBCat2) minN1EndcapLSBCat2 = N1EndcapLSBCat2;
         if(N1EndcapLSBCat2>maxN1EndcapLSBCat2) maxN1EndcapLSBCat2 = N1EndcapLSBCat2;
         int N2EndcapLSBCat2=hMassMVAEndcapCat2->Integral(1,8);
         if(N2EndcapLSBCat2<minN2EndcapLSBCat2) minN2EndcapLSBCat2 = N2EndcapLSBCat2;
         if(N2EndcapLSBCat2>maxN2EndcapLSBCat2) maxN2EndcapLSBCat2 = N2EndcapLSBCat2;
         double nMuMuEndcapLSBCat2 = N0EndcapLSBCat2*matrix[0][0]+N1EndcapLSBCat2*matrix[0][1]+N2EndcapLSBCat2*matrix[0][2];
         if(nMuMuEndcapLSBCat2<minnMuMuEndcapLSBCat2) minnMuMuEndcapLSBCat2 = nMuMuEndcapLSBCat2;
         if(nMuMuEndcapLSBCat2>maxnMuMuEndcapLSBCat2) maxnMuMuEndcapLSBCat2 = nMuMuEndcapLSBCat2;
         double nMuHEndcapLSBCat2 = N0EndcapLSBCat2*matrix[1][0]+N1EndcapLSBCat2*matrix[1][1]+N2EndcapLSBCat2*matrix[1][2];
         if(nMuHEndcapLSBCat2<minnMuHEndcapLSBCat2) minnMuHEndcapLSBCat2 = nMuHEndcapLSBCat2;
         if(nMuHEndcapLSBCat2>maxnMuHEndcapLSBCat2) maxnMuHEndcapLSBCat2 = nMuHEndcapLSBCat2;
         double nHHEndcapLSBCat2 = N0EndcapLSBCat2*matrix[2][0]+N1EndcapLSBCat2*matrix[2][1]+N2EndcapLSBCat2*matrix[2][2];
         if(nHHEndcapLSBCat2<minnHHEndcapLSBCat2) minnHHEndcapLSBCat2 = nHHEndcapLSBCat2;
         if(nHHEndcapLSBCat2>maxnHHEndcapLSBCat2) maxnHHEndcapLSBCat2 = nHHEndcapLSBCat2;
         double nMuMuMVAEndcapLSBCat2 = nMuMuEndcapLSBCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapLSBCat2<minnMuMuMVAEndcapLSBCat2) minnMuMuMVAEndcapLSBCat2 = nMuMuMVAEndcapLSBCat2;
         if(nMuMuMVAEndcapLSBCat2>maxnMuMuMVAEndcapLSBCat2) maxnMuMuMVAEndcapLSBCat2 = nMuMuMVAEndcapLSBCat2;
         double nMuHMVAEndcapLSBCat2 = nMuHEndcapLSBCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapLSBCat2<minnMuHMVAEndcapLSBCat2) minnMuHMVAEndcapLSBCat2 = nMuHMVAEndcapLSBCat2;
         if(nMuHMVAEndcapLSBCat2>maxnMuHMVAEndcapLSBCat2) maxnMuHMVAEndcapLSBCat2 = nMuHMVAEndcapLSBCat2;
         double nHHMVAEndcapLSBCat2 = nHHEndcapLSBCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapLSBCat2<minnHHMVAEndcapLSBCat2) minnHHMVAEndcapLSBCat2 = nHHMVAEndcapLSBCat2;
         if(nHHMVAEndcapLSBCat2>maxnHHMVAEndcapLSBCat2) maxnHHMVAEndcapLSBCat2 = nHHMVAEndcapLSBCat2;
         
         std::cout << "\nN0EndcapLSBCat2 = " << N0EndcapLSBCat2 << std::endl;
         std::cout << "N1EndcapLSBCat2 = " << N1EndcapLSBCat2 << std::endl;
         std::cout << "N2EndcapLSBCat2 = " << N2EndcapLSBCat2 << std::endl;
         
         std::cout << "nMuMuEndcapLSBCat2 = " << nMuMuEndcapLSBCat2 << std::endl;
         std::cout << "nMuHEndcapLSBCat2 = " << nMuHEndcapLSBCat2 << std::endl;
         std::cout << "nHHEndcapLSBCat2 = " << nHHEndcapLSBCat2 << std::endl;
         std::cout << "nMuMuMVAEndcapLSBCat2 = " << nMuMuMVAEndcapLSBCat2 << std::endl;
         std::cout << "nMuHMVAEndcapLSBCat2 = " << nMuHMVAEndcapLSBCat2 << std::endl;
         std::cout << "nHHMVAEndcapLSBCat2 = " << nHHMVAEndcapLSBCat2 << std::endl;
         
         int N0EndcapSigCat2=hMassFail2EndcapCat2->Integral(9,15);
         if(N0EndcapSigCat2<minN0EndcapSigCat2) minN0EndcapSigCat2 = N0EndcapSigCat2;
         if(N0EndcapSigCat2>maxN0EndcapSigCat2) maxN0EndcapSigCat2 = N0EndcapSigCat2;
         int N1EndcapSigCat2=hMassFail1EndcapCat2->Integral(9,15);
         if(N1EndcapSigCat2<minN1EndcapSigCat2) minN1EndcapSigCat2 = N1EndcapSigCat2;
         if(N1EndcapSigCat2>maxN1EndcapSigCat2) maxN1EndcapSigCat2 = N1EndcapSigCat2;
         int N2EndcapSigCat2=hMassMVAEndcapCat2->Integral(9,15);
         if(N2EndcapSigCat2<minN2EndcapSigCat2) minN2EndcapSigCat2 = N2EndcapSigCat2;
         if(N2EndcapSigCat2>maxN2EndcapSigCat2) maxN2EndcapSigCat2 = N2EndcapSigCat2;
         double nMuMuEndcapSigCat2 = N0EndcapSigCat2*matrix[0][0]+N1EndcapSigCat2*matrix[0][1]+N2EndcapSigCat2*matrix[0][2];
         if(nMuMuEndcapSigCat2<minnMuMuEndcapSigCat2) minnMuMuEndcapSigCat2 = nMuMuEndcapSigCat2;
         if(nMuMuEndcapSigCat2>maxnMuMuEndcapSigCat2) maxnMuMuEndcapSigCat2 = nMuMuEndcapSigCat2;
         double nMuHEndcapSigCat2 = N0EndcapSigCat2*matrix[1][0]+N1EndcapSigCat2*matrix[1][1]+N2EndcapSigCat2*matrix[1][2];
         if(nMuHEndcapSigCat2<minnMuHEndcapSigCat2) minnMuHEndcapSigCat2 = nMuHEndcapSigCat2;
         if(nMuHEndcapSigCat2>maxnMuHEndcapSigCat2) maxnMuHEndcapSigCat2 = nMuHEndcapSigCat2;
         double nHHEndcapSigCat2 = N0EndcapSigCat2*matrix[2][0]+N1EndcapSigCat2*matrix[2][1]+N2EndcapSigCat2*matrix[2][2];
         if(nHHEndcapSigCat2<minnHHEndcapSigCat2) minnHHEndcapSigCat2 = nHHEndcapSigCat2;
         if(nHHEndcapSigCat2>maxnHHEndcapSigCat2) maxnHHEndcapSigCat2 = nHHEndcapSigCat2;
         double nMuMuMVAEndcapSigCat2 = nMuMuEndcapSigCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapSigCat2<minnMuMuMVAEndcapSigCat2) minnMuMuMVAEndcapSigCat2 = nMuMuMVAEndcapSigCat2;
         if(nMuMuMVAEndcapSigCat2>maxnMuMuMVAEndcapSigCat2) maxnMuMuMVAEndcapSigCat2 = nMuMuMVAEndcapSigCat2;
         double nMuHMVAEndcapSigCat2 = nMuHEndcapSigCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapSigCat2<minnMuHMVAEndcapSigCat2) minnMuHMVAEndcapSigCat2 = nMuHMVAEndcapSigCat2;
         if(nMuHMVAEndcapSigCat2>maxnMuHMVAEndcapSigCat2) maxnMuHMVAEndcapSigCat2 = nMuHMVAEndcapSigCat2;
         double nHHMVAEndcapSigCat2 = nHHEndcapSigCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapSigCat2<minnHHMVAEndcapSigCat2) minnHHMVAEndcapSigCat2 = nHHMVAEndcapSigCat2;
         if(nHHMVAEndcapSigCat2>maxnHHMVAEndcapSigCat2) maxnHHMVAEndcapSigCat2 = nHHMVAEndcapSigCat2;
         
         std::cout << "\nN0EndcapSigCat2 = " << N0EndcapSigCat2 << std::endl;
         std::cout << "N1EndcapSigCat2 = " << N1EndcapSigCat2 << std::endl;
         std::cout << "N2EndcapSigCat2 = " << N2EndcapSigCat2 << std::endl;
         
         std::cout << "nMuMuEndcapSigCat2 = " << nMuMuEndcapSigCat2 << std::endl;
         std::cout << "nMuHEndcapSigCat2 = " << nMuHEndcapSigCat2 << std::endl;
         std::cout << "nHHEndcapSigCat2 = " << nHHEndcapSigCat2 << std::endl;
         std::cout << "nMuMuMVAEndcapSigCat2 = " << nMuMuMVAEndcapSigCat2 << std::endl;
         std::cout << "nMuHMVAEndcapSigCat2 = " << nMuHMVAEndcapSigCat2 << std::endl;
         std::cout << "nHHMVAEndcapSigCat2 = " << nHHMVAEndcapSigCat2 << std::endl;
         
         int N0EndcapRSBCat2=hMassFail2EndcapCat2->Integral(16,25);
         if(N0EndcapRSBCat2<minN0EndcapRSBCat2) minN0EndcapRSBCat2 = N0EndcapRSBCat2;
         if(N0EndcapRSBCat2>maxN0EndcapRSBCat2) maxN0EndcapRSBCat2 = N0EndcapRSBCat2;
         int N1EndcapRSBCat2=hMassFail1EndcapCat2->Integral(16,25);
         if(N1EndcapRSBCat2<minN1EndcapRSBCat2) minN1EndcapRSBCat2 = N1EndcapRSBCat2;
         if(N1EndcapRSBCat2>maxN1EndcapRSBCat2) maxN1EndcapRSBCat2 = N1EndcapRSBCat2;
         int N2EndcapRSBCat2=hMassMVAEndcapCat2->Integral(16,25);
         if(N2EndcapRSBCat2<minN2EndcapRSBCat2) minN2EndcapRSBCat2 = N2EndcapRSBCat2;
         if(N2EndcapRSBCat2>maxN2EndcapRSBCat2) maxN2EndcapRSBCat2 = N2EndcapRSBCat2;
         double nMuMuEndcapRSBCat2 = N0EndcapRSBCat2*matrix[0][0]+N1EndcapRSBCat2*matrix[0][1]+N2EndcapRSBCat2*matrix[0][2];
         if(nMuMuEndcapRSBCat2<minnMuMuEndcapRSBCat2) minnMuMuEndcapRSBCat2 = nMuMuEndcapRSBCat2;
         if(nMuMuEndcapRSBCat2>maxnMuMuEndcapRSBCat2) maxnMuMuEndcapRSBCat2 = nMuMuEndcapRSBCat2;
         double nMuHEndcapRSBCat2 = N0EndcapRSBCat2*matrix[1][0]+N1EndcapRSBCat2*matrix[1][1]+N2EndcapRSBCat2*matrix[1][2];
         if(nMuHEndcapRSBCat2<minnMuHEndcapRSBCat2) minnMuHEndcapRSBCat2 = nMuHEndcapRSBCat2;
         if(nMuHEndcapRSBCat2>maxnMuHEndcapRSBCat2) maxnMuHEndcapRSBCat2 = nMuHEndcapRSBCat2;
         double nHHEndcapRSBCat2 = N0EndcapRSBCat2*matrix[2][0]+N1EndcapRSBCat2*matrix[2][1]+N2EndcapRSBCat2*matrix[2][2];
         if(nHHEndcapRSBCat2<minnHHEndcapRSBCat2) minnHHEndcapRSBCat2 = nHHEndcapRSBCat2;
         if(nHHEndcapRSBCat2>maxnHHEndcapRSBCat2) maxnHHEndcapRSBCat2 = nHHEndcapRSBCat2;
         double nMuMuMVAEndcapRSBCat2 = nMuMuEndcapRSBCat2*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapRSBCat2<minnMuMuMVAEndcapRSBCat2) minnMuMuMVAEndcapRSBCat2 = nMuMuMVAEndcapRSBCat2;
         if(nMuMuMVAEndcapRSBCat2>maxnMuMuMVAEndcapRSBCat2) maxnMuMuMVAEndcapRSBCat2 = nMuMuMVAEndcapRSBCat2;
         double nMuHMVAEndcapRSBCat2 = nMuHEndcapRSBCat2*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapRSBCat2<minnMuHMVAEndcapRSBCat2) minnMuHMVAEndcapRSBCat2 = nMuHMVAEndcapRSBCat2;
         if(nMuHMVAEndcapRSBCat2>maxnMuHMVAEndcapRSBCat2) maxnMuHMVAEndcapRSBCat2 = nMuHMVAEndcapRSBCat2;
         double nHHMVAEndcapRSBCat2 = nHHEndcapRSBCat2*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapRSBCat2<minnHHMVAEndcapRSBCat2) minnHHMVAEndcapRSBCat2 = nHHMVAEndcapRSBCat2;
         if(nHHMVAEndcapRSBCat2>maxnHHMVAEndcapRSBCat2) maxnHHMVAEndcapRSBCat2 = nHHMVAEndcapRSBCat2;
         
         std::cout << "\nN0EndcapRSBCat2 = " << N0EndcapRSBCat2 << std::endl;
         std::cout << "N1EndcapRSBCat2 = " << N1EndcapRSBCat2 << std::endl;
         std::cout << "N2EndcapRSBCat2 = " << N2EndcapRSBCat2 << std::endl;
         
         std::cout << "nMuMuEndcapRSBCat2 = " << nMuMuEndcapRSBCat2 << std::endl;
         std::cout << "nMuHEndcapRSBCat2 = " << nMuHEndcapRSBCat2 << std::endl;
         std::cout << "nHHEndcapRSBCat2 = " << nHHEndcapRSBCat2 << std::endl;
         std::cout << "nMuMuMVAEndcapRSBCat2 = " << nMuMuMVAEndcapRSBCat2 << std::endl;
         std::cout << "nMuHMVAEndcapRSBCat2 = " << nMuHMVAEndcapRSBCat2 << std::endl;
         std::cout << "nHHMVAEndcapRSBCat2 = " << nHHMVAEndcapRSBCat2 << std::endl;
         
         int N0BarrelAllCat3=hMassFail2BarrelCat3->Integral(1,25);
         if(N0BarrelAllCat3<minN0BarrelAllCat3) minN0BarrelAllCat3 = N0BarrelAllCat3;
         if(N0BarrelAllCat3>maxN0BarrelAllCat3) maxN0BarrelAllCat3 = N0BarrelAllCat3;
         int N1BarrelAllCat3=hMassFail1BarrelCat3->Integral(1,25);
         if(N1BarrelAllCat3<minN1BarrelAllCat3) minN1BarrelAllCat3 = N1BarrelAllCat3;
         if(N1BarrelAllCat3>maxN1BarrelAllCat3) maxN1BarrelAllCat3 = N1BarrelAllCat3;
         int N2BarrelAllCat3=hMassMVABarrelCat3->Integral(1,25);
         if(N2BarrelAllCat3<minN2BarrelAllCat3) minN2BarrelAllCat3 = N2BarrelAllCat3;
         if(N2BarrelAllCat3>maxN2BarrelAllCat3) maxN2BarrelAllCat3 = N2BarrelAllCat3;
         double nMuMuBarrelAllCat3 = N0BarrelAllCat3*matrix[0][0]+N1BarrelAllCat3*matrix[0][1]+N2BarrelAllCat3*matrix[0][2];
         if(nMuMuBarrelAllCat3<minnMuMuBarrelAllCat3) minnMuMuBarrelAllCat3 = nMuMuBarrelAllCat3;
         if(nMuMuBarrelAllCat3>maxnMuMuBarrelAllCat3) maxnMuMuBarrelAllCat3 = nMuMuBarrelAllCat3;
         double nMuHBarrelAllCat3 = N0BarrelAllCat3*matrix[1][0]+N1BarrelAllCat3*matrix[1][1]+N2BarrelAllCat3*matrix[1][2];
         if(nMuHBarrelAllCat3<minnMuHBarrelAllCat3) minnMuHBarrelAllCat3 = nMuHBarrelAllCat3;
         if(nMuHBarrelAllCat3>maxnMuHBarrelAllCat3) maxnMuHBarrelAllCat3 = nMuHBarrelAllCat3;
         double nHHBarrelAllCat3 = N0BarrelAllCat3*matrix[2][0]+N1BarrelAllCat3*matrix[2][1]+N2BarrelAllCat3*matrix[2][2];
         if(nHHBarrelAllCat3<minnHHBarrelAllCat3) minnHHBarrelAllCat3 = nHHBarrelAllCat3;
         if(nHHBarrelAllCat3>maxnHHBarrelAllCat3) maxnHHBarrelAllCat3 = nHHBarrelAllCat3;
         double nMuMuMVABarrelAllCat3 = nMuMuBarrelAllCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelAllCat3<minnMuMuMVABarrelAllCat3) minnMuMuMVABarrelAllCat3 = nMuMuMVABarrelAllCat3;
         if(nMuMuMVABarrelAllCat3>maxnMuMuMVABarrelAllCat3) maxnMuMuMVABarrelAllCat3 = nMuMuMVABarrelAllCat3;
         double nMuHMVABarrelAllCat3 = nMuHBarrelAllCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelAllCat3<minnMuHMVABarrelAllCat3) minnMuHMVABarrelAllCat3 = nMuHMVABarrelAllCat3;
         if(nMuHMVABarrelAllCat3>maxnMuHMVABarrelAllCat3) maxnMuHMVABarrelAllCat3 = nMuHMVABarrelAllCat3;
         double nHHMVABarrelAllCat3 = nHHBarrelAllCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelAllCat3<minnHHMVABarrelAllCat3) minnHHMVABarrelAllCat3 = nHHMVABarrelAllCat3;
         if(nHHMVABarrelAllCat3>maxnHHMVABarrelAllCat3) maxnHHMVABarrelAllCat3 = nHHMVABarrelAllCat3;
         
         std::cout << "\nN0BarrelAllCat3 = " << N0BarrelAllCat3 << std::endl;
         std::cout << "N1BarrelAllCat3 = " << N1BarrelAllCat3 << std::endl;
         std::cout << "N2BarrelAllCat3 = " << N2BarrelAllCat3 << std::endl;
         
         std::cout << "nMuMuBarrelAllCat3 = " << nMuMuBarrelAllCat3 << std::endl;
         std::cout << "nMuHBarrelAllCat3 = " << nMuHBarrelAllCat3 << std::endl;
         std::cout << "nHHBarrelAllCat3 = " << nHHBarrelAllCat3 << std::endl;
         std::cout << "nMuMuMVABarrelAllCat3 = " << nMuMuMVABarrelAllCat3 << std::endl;
         std::cout << "nMuHMVABarrelAllCat3 = " << nMuHMVABarrelAllCat3 << std::endl;
         std::cout << "nHHMVABarrelAllCat3 = " << nHHMVABarrelAllCat3 << std::endl;
         
         int N0BarrelLSBCat3=hMassFail2BarrelCat3->Integral(1,8);
         if(N0BarrelLSBCat3<minN0BarrelLSBCat3) minN0BarrelLSBCat3 = N0BarrelLSBCat3;
         if(N0BarrelLSBCat3>maxN0BarrelLSBCat3) maxN0BarrelLSBCat3 = N0BarrelLSBCat3;
         int N1BarrelLSBCat3=hMassFail1BarrelCat3->Integral(1,8);
         if(N1BarrelLSBCat3<minN1BarrelLSBCat3) minN1BarrelLSBCat3 = N1BarrelLSBCat3;
         if(N1BarrelLSBCat3>maxN1BarrelLSBCat3) maxN1BarrelLSBCat3 = N1BarrelLSBCat3;
         int N2BarrelLSBCat3=hMassMVABarrelCat3->Integral(1,8);
         if(N2BarrelLSBCat3<minN2BarrelLSBCat3) minN2BarrelLSBCat3 = N2BarrelLSBCat3;
         if(N2BarrelLSBCat3>maxN2BarrelLSBCat3) maxN2BarrelLSBCat3 = N2BarrelLSBCat3;
         double nMuMuBarrelLSBCat3 = N0BarrelLSBCat3*matrix[0][0]+N1BarrelLSBCat3*matrix[0][1]+N2BarrelLSBCat3*matrix[0][2];
         if(nMuMuBarrelLSBCat3<minnMuMuBarrelLSBCat3) minnMuMuBarrelLSBCat3 = nMuMuBarrelLSBCat3;
         if(nMuMuBarrelLSBCat3>maxnMuMuBarrelLSBCat3) maxnMuMuBarrelLSBCat3 = nMuMuBarrelLSBCat3;
         double nMuHBarrelLSBCat3 = N0BarrelLSBCat3*matrix[1][0]+N1BarrelLSBCat3*matrix[1][1]+N2BarrelLSBCat3*matrix[1][2];
         if(nMuHBarrelLSBCat3<minnMuHBarrelLSBCat3) minnMuHBarrelLSBCat3 = nMuHBarrelLSBCat3;
         if(nMuHBarrelLSBCat3>maxnMuHBarrelLSBCat3) maxnMuHBarrelLSBCat3 = nMuHBarrelLSBCat3;
         double nHHBarrelLSBCat3 = N0BarrelLSBCat3*matrix[2][0]+N1BarrelLSBCat3*matrix[2][1]+N2BarrelLSBCat3*matrix[2][2];
         if(nHHBarrelLSBCat3<minnHHBarrelLSBCat3) minnHHBarrelLSBCat3 = nHHBarrelLSBCat3;
         if(nHHBarrelLSBCat3>maxnHHBarrelLSBCat3) maxnHHBarrelLSBCat3 = nHHBarrelLSBCat3;
         double nMuMuMVABarrelLSBCat3 = nMuMuBarrelLSBCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelLSBCat3<minnMuMuMVABarrelLSBCat3) minnMuMuMVABarrelLSBCat3 = nMuMuMVABarrelLSBCat3;
         if(nMuMuMVABarrelLSBCat3>maxnMuMuMVABarrelLSBCat3) maxnMuMuMVABarrelLSBCat3 = nMuMuMVABarrelLSBCat3;
         double nMuHMVABarrelLSBCat3 = nMuHBarrelLSBCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelLSBCat3<minnMuHMVABarrelLSBCat3) minnMuHMVABarrelLSBCat3 = nMuHMVABarrelLSBCat3;
         if(nMuHMVABarrelLSBCat3>maxnMuHMVABarrelLSBCat3) maxnMuHMVABarrelLSBCat3 = nMuHMVABarrelLSBCat3;
         double nHHMVABarrelLSBCat3 = nHHBarrelLSBCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelLSBCat3<minnHHMVABarrelLSBCat3) minnHHMVABarrelLSBCat3 = nHHMVABarrelLSBCat3;
         if(nHHMVABarrelLSBCat3>maxnHHMVABarrelLSBCat3) maxnHHMVABarrelLSBCat3 = nHHMVABarrelLSBCat3;
         
         std::cout << "\nN0BarrelLSBCat3 = " << N0BarrelLSBCat3 << std::endl;
         std::cout << "N1BarrelLSBCat3 = " << N1BarrelLSBCat3 << std::endl;
         std::cout << "N2BarrelLSBCat3 = " << N2BarrelLSBCat3 << std::endl;
         
         std::cout << "nMuMuBarrelLSBCat3 = " << nMuMuBarrelLSBCat3 << std::endl;
         std::cout << "nMuHBarrelLSBCat3 = " << nMuHBarrelLSBCat3 << std::endl;
         std::cout << "nHHBarrelLSBCat3 = " << nHHBarrelLSBCat3 << std::endl;
         std::cout << "nMuMuMVABarrelLSBCat3 = " << nMuMuMVABarrelLSBCat3 << std::endl;
         std::cout << "nMuHMVABarrelLSBCat3 = " << nMuHMVABarrelLSBCat3 << std::endl;
         std::cout << "nHHMVABarrelLSBCat3 = " << nHHMVABarrelLSBCat3 << std::endl;
         
         int N0BarrelSigCat3=hMassFail2BarrelCat3->Integral(9,15);
         if(N0BarrelSigCat3<minN0BarrelSigCat3) minN0BarrelSigCat3 = N0BarrelSigCat3;
         if(N0BarrelSigCat3>maxN0BarrelSigCat3) maxN0BarrelSigCat3 = N0BarrelSigCat3;
         int N1BarrelSigCat3=hMassFail1BarrelCat3->Integral(9,15);
         if(N1BarrelSigCat3<minN1BarrelSigCat3) minN1BarrelSigCat3 = N1BarrelSigCat3;
         if(N1BarrelSigCat3>maxN1BarrelSigCat3) maxN1BarrelSigCat3 = N1BarrelSigCat3;
         int N2BarrelSigCat3=hMassMVABarrelCat3->Integral(9,15);
         if(N2BarrelSigCat3<minN2BarrelSigCat3) minN2BarrelSigCat3 = N2BarrelSigCat3;
         if(N2BarrelSigCat3>maxN2BarrelSigCat3) maxN2BarrelSigCat3 = N2BarrelSigCat3;
         double nMuMuBarrelSigCat3 = N0BarrelSigCat3*matrix[0][0]+N1BarrelSigCat3*matrix[0][1]+N2BarrelSigCat3*matrix[0][2];
         if(nMuMuBarrelSigCat3<minnMuMuBarrelSigCat3) minnMuMuBarrelSigCat3 = nMuMuBarrelSigCat3;
         if(nMuMuBarrelSigCat3>maxnMuMuBarrelSigCat3) maxnMuMuBarrelSigCat3 = nMuMuBarrelSigCat3;
         double nMuHBarrelSigCat3 = N0BarrelSigCat3*matrix[1][0]+N1BarrelSigCat3*matrix[1][1]+N2BarrelSigCat3*matrix[1][2];
         if(nMuHBarrelSigCat3<minnMuHBarrelSigCat3) minnMuHBarrelSigCat3 = nMuHBarrelSigCat3;
         if(nMuHBarrelSigCat3>maxnMuHBarrelSigCat3) maxnMuHBarrelSigCat3 = nMuHBarrelSigCat3;
         double nHHBarrelSigCat3 = N0BarrelSigCat3*matrix[2][0]+N1BarrelSigCat3*matrix[2][1]+N2BarrelSigCat3*matrix[2][2];
         if(nHHBarrelSigCat3<minnHHBarrelSigCat3) minnHHBarrelSigCat3 = nHHBarrelSigCat3;
         if(nHHBarrelSigCat3>maxnHHBarrelSigCat3) maxnHHBarrelSigCat3 = nHHBarrelSigCat3;
         double nMuMuMVABarrelSigCat3 = nMuMuBarrelSigCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelSigCat3<minnMuMuMVABarrelSigCat3) minnMuMuMVABarrelSigCat3 = nMuMuMVABarrelSigCat3;
         if(nMuMuMVABarrelSigCat3>maxnMuMuMVABarrelSigCat3) maxnMuMuMVABarrelSigCat3 = nMuMuMVABarrelSigCat3;
         double nMuHMVABarrelSigCat3 = nMuHBarrelSigCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelSigCat3<minnMuHMVABarrelSigCat3) minnMuHMVABarrelSigCat3 = nMuHMVABarrelSigCat3;
         if(nMuHMVABarrelSigCat3>maxnMuHMVABarrelSigCat3) maxnMuHMVABarrelSigCat3 = nMuHMVABarrelSigCat3;
         double nHHMVABarrelSigCat3 = nHHBarrelSigCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelSigCat3<minnHHMVABarrelSigCat3) minnHHMVABarrelSigCat3 = nHHMVABarrelSigCat3;
         if(nHHMVABarrelSigCat3>maxnHHMVABarrelSigCat3) maxnHHMVABarrelSigCat3 = nHHMVABarrelSigCat3;
         
         std::cout << "\nN0BarrelSigCat3 = " << N0BarrelSigCat3 << std::endl;
         std::cout << "N1BarrelSigCat3 = " << N1BarrelSigCat3 << std::endl;
         std::cout << "N2BarrelSigCat3 = " << N2BarrelSigCat3 << std::endl;
         
         std::cout << "nMuMuBarrelSigCat3 = " << nMuMuBarrelSigCat3 << std::endl;
         std::cout << "nMuHBarrelSigCat3 = " << nMuHBarrelSigCat3 << std::endl;
         std::cout << "nHHBarrelSigCat3 = " << nHHBarrelSigCat3 << std::endl;
         std::cout << "nMuMuMVABarrelSigCat3 = " << nMuMuMVABarrelSigCat3 << std::endl;
         std::cout << "nMuHMVABarrelSigCat3 = " << nMuHMVABarrelSigCat3 << std::endl;
         std::cout << "nHHMVABarrelSigCat3 = " << nHHMVABarrelSigCat3 << std::endl;
         
         int N0BarrelRSBCat3=hMassFail2BarrelCat3->Integral(16,25);
         if(N0BarrelRSBCat3<minN0BarrelRSBCat3) minN0BarrelRSBCat3 = N0BarrelRSBCat3;
         if(N0BarrelRSBCat3>maxN0BarrelRSBCat3) maxN0BarrelRSBCat3 = N0BarrelRSBCat3;
         int N1BarrelRSBCat3=hMassFail1BarrelCat3->Integral(16,25);
         if(N1BarrelRSBCat3<minN1BarrelRSBCat3) minN1BarrelRSBCat3 = N1BarrelRSBCat3;
         if(N1BarrelRSBCat3>maxN1BarrelRSBCat3) maxN1BarrelRSBCat3 = N1BarrelRSBCat3;
         int N2BarrelRSBCat3=hMassMVABarrelCat3->Integral(16,25);
         if(N2BarrelRSBCat3<minN2BarrelRSBCat3) minN2BarrelRSBCat3 = N2BarrelRSBCat3;
         if(N2BarrelRSBCat3>maxN2BarrelRSBCat3) maxN2BarrelRSBCat3 = N2BarrelRSBCat3;
         double nMuMuBarrelRSBCat3 = N0BarrelRSBCat3*matrix[0][0]+N1BarrelRSBCat3*matrix[0][1]+N2BarrelRSBCat3*matrix[0][2];
         if(nMuMuBarrelRSBCat3<minnMuMuBarrelRSBCat3) minnMuMuBarrelRSBCat3 = nMuMuBarrelRSBCat3;
         if(nMuMuBarrelRSBCat3>maxnMuMuBarrelRSBCat3) maxnMuMuBarrelRSBCat3 = nMuMuBarrelRSBCat3;
         double nMuHBarrelRSBCat3 = N0BarrelRSBCat3*matrix[1][0]+N1BarrelRSBCat3*matrix[1][1]+N2BarrelRSBCat3*matrix[1][2];
         if(nMuHBarrelRSBCat3<minnMuHBarrelRSBCat3) minnMuHBarrelRSBCat3 = nMuHBarrelRSBCat3;
         if(nMuHBarrelRSBCat3>maxnMuHBarrelRSBCat3) maxnMuHBarrelRSBCat3 = nMuHBarrelRSBCat3;
         double nHHBarrelRSBCat3 = N0BarrelRSBCat3*matrix[2][0]+N1BarrelRSBCat3*matrix[2][1]+N2BarrelRSBCat3*matrix[2][2];
         if(nHHBarrelRSBCat3<minnHHBarrelRSBCat3) minnHHBarrelRSBCat3 = nHHBarrelRSBCat3;
         if(nHHBarrelRSBCat3>maxnHHBarrelRSBCat3) maxnHHBarrelRSBCat3 = nHHBarrelRSBCat3;
         double nMuMuMVABarrelRSBCat3 = nMuMuBarrelRSBCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelRSBCat3<minnMuMuMVABarrelRSBCat3) minnMuMuMVABarrelRSBCat3 = nMuMuMVABarrelRSBCat3;
         if(nMuMuMVABarrelRSBCat3>maxnMuMuMVABarrelRSBCat3) maxnMuMuMVABarrelRSBCat3 = nMuMuMVABarrelRSBCat3;
         double nMuHMVABarrelRSBCat3 = nMuHBarrelRSBCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelRSBCat3<minnMuHMVABarrelRSBCat3) minnMuHMVABarrelRSBCat3 = nMuHMVABarrelRSBCat3;
         if(nMuHMVABarrelRSBCat3>maxnMuHMVABarrelRSBCat3) maxnMuHMVABarrelRSBCat3 = nMuHMVABarrelRSBCat3;
         double nHHMVABarrelRSBCat3 = nHHBarrelRSBCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelRSBCat3<minnHHMVABarrelRSBCat3) minnHHMVABarrelRSBCat3 = nHHMVABarrelRSBCat3;
         if(nHHMVABarrelRSBCat3>maxnHHMVABarrelRSBCat3) maxnHHMVABarrelRSBCat3 = nHHMVABarrelRSBCat3;
         
         std::cout << "\nN0BarrelRSBCat3 = " << N0BarrelRSBCat3 << std::endl;
         std::cout << "N1BarrelRSBCat3 = " << N1BarrelRSBCat3 << std::endl;
         std::cout << "N2BarrelRSBCat3 = " << N2BarrelRSBCat3 << std::endl;
         
         std::cout << "nMuMuBarrelRSBCat3 = " << nMuMuBarrelRSBCat3 << std::endl;
         std::cout << "nMuHBarrelRSBCat3 = " << nMuHBarrelRSBCat3 << std::endl;
         std::cout << "nHHBarrelRSBCat3 = " << nHHBarrelRSBCat3 << std::endl;
         std::cout << "nMuMuMVABarrelRSBCat3 = " << nMuMuMVABarrelRSBCat3 << std::endl;
         std::cout << "nMuHMVABarrelRSBCat3 = " << nMuHMVABarrelRSBCat3 << std::endl;
         std::cout << "nHHMVABarrelRSBCat3 = " << nHHMVABarrelRSBCat3 << std::endl;
         
         int N0EndcapAllCat3=hMassFail2EndcapCat3->Integral(1,25);
         if(N0EndcapAllCat3<minN0EndcapAllCat3) minN0EndcapAllCat3 = N0EndcapAllCat3;
         if(N0EndcapAllCat3>maxN0EndcapAllCat3) maxN0EndcapAllCat3 = N0EndcapAllCat3;
         int N1EndcapAllCat3=hMassFail1EndcapCat3->Integral(1,25);
         if(N1EndcapAllCat3<minN1EndcapAllCat3) minN1EndcapAllCat3 = N1EndcapAllCat3;
         if(N1EndcapAllCat3>maxN1EndcapAllCat3) maxN1EndcapAllCat3 = N1EndcapAllCat3;
         int N2EndcapAllCat3=hMassMVAEndcapCat3->Integral(1,25);
         if(N2EndcapAllCat3<minN2EndcapAllCat3) minN2EndcapAllCat3 = N2EndcapAllCat3;
         if(N2EndcapAllCat3>maxN2EndcapAllCat3) maxN2EndcapAllCat3 = N2EndcapAllCat3;
         double nMuMuEndcapAllCat3 = N0EndcapAllCat3*matrix[0][0]+N1EndcapAllCat3*matrix[0][1]+N2EndcapAllCat3*matrix[0][2];
         if(nMuMuEndcapAllCat3<minnMuMuEndcapAllCat3) minnMuMuEndcapAllCat3 = nMuMuEndcapAllCat3;
         if(nMuMuEndcapAllCat3>maxnMuMuEndcapAllCat3) maxnMuMuEndcapAllCat3 = nMuMuEndcapAllCat3;
         double nMuHEndcapAllCat3 = N0EndcapAllCat3*matrix[1][0]+N1EndcapAllCat3*matrix[1][1]+N2EndcapAllCat3*matrix[1][2];
         if(nMuHEndcapAllCat3<minnMuHEndcapAllCat3) minnMuHEndcapAllCat3 = nMuHEndcapAllCat3;
         if(nMuHEndcapAllCat3>maxnMuHEndcapAllCat3) maxnMuHEndcapAllCat3 = nMuHEndcapAllCat3;
         double nHHEndcapAllCat3 = N0EndcapAllCat3*matrix[2][0]+N1EndcapAllCat3*matrix[2][1]+N2EndcapAllCat3*matrix[2][2];
         if(nHHEndcapAllCat3<minnHHEndcapAllCat3) minnHHEndcapAllCat3 = nHHEndcapAllCat3;
         if(nHHEndcapAllCat3>maxnHHEndcapAllCat3) maxnHHEndcapAllCat3 = nHHEndcapAllCat3;
         double nMuMuMVAEndcapAllCat3 = nMuMuEndcapAllCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapAllCat3<minnMuMuMVAEndcapAllCat3) minnMuMuMVAEndcapAllCat3 = nMuMuMVAEndcapAllCat3;
         if(nMuMuMVAEndcapAllCat3>maxnMuMuMVAEndcapAllCat3) maxnMuMuMVAEndcapAllCat3 = nMuMuMVAEndcapAllCat3;
         double nMuHMVAEndcapAllCat3 = nMuHEndcapAllCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapAllCat3<minnMuHMVAEndcapAllCat3) minnMuHMVAEndcapAllCat3 = nMuHMVAEndcapAllCat3;
         if(nMuHMVAEndcapAllCat3>maxnMuHMVAEndcapAllCat3) maxnMuHMVAEndcapAllCat3 = nMuHMVAEndcapAllCat3;
         double nHHMVAEndcapAllCat3 = nHHEndcapAllCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapAllCat3<minnHHMVAEndcapAllCat3) minnHHMVAEndcapAllCat3 = nHHMVAEndcapAllCat3;
         if(nHHMVAEndcapAllCat3>maxnHHMVAEndcapAllCat3) maxnHHMVAEndcapAllCat3 = nHHMVAEndcapAllCat3;
         
         std::cout << "\nN0EndcapAllCat3 = " << N0EndcapAllCat3 << std::endl;
         std::cout << "N1EndcapAllCat3 = " << N1EndcapAllCat3 << std::endl;
         std::cout << "N2EndcapAllCat3 = " << N2EndcapAllCat3 << std::endl;
         
         std::cout << "nMuMuEndcapAllCat3 = " << nMuMuEndcapAllCat3 << std::endl;
         std::cout << "nMuHEndcapAllCat3 = " << nMuHEndcapAllCat3 << std::endl;
         std::cout << "nHHEndcapAllCat3 = " << nHHEndcapAllCat3 << std::endl;
         std::cout << "nMuMuMVAEndcapAllCat3 = " << nMuMuMVAEndcapAllCat3 << std::endl;
         std::cout << "nMuHMVAEndcapAllCat3 = " << nMuHMVAEndcapAllCat3 << std::endl;
         std::cout << "nHHMVAEndcapAllCat3 = " << nHHMVAEndcapAllCat3 << std::endl;
         
         int N0EndcapLSBCat3=hMassFail2EndcapCat3->Integral(1,8);
         if(N0EndcapLSBCat3<minN0EndcapLSBCat3) minN0EndcapLSBCat3 = N0EndcapLSBCat3;
         if(N0EndcapLSBCat3>maxN0EndcapLSBCat3) maxN0EndcapLSBCat3 = N0EndcapLSBCat3;
         int N1EndcapLSBCat3=hMassFail1EndcapCat3->Integral(1,8);
         if(N1EndcapLSBCat3<minN1EndcapLSBCat3) minN1EndcapLSBCat3 = N1EndcapLSBCat3;
         if(N1EndcapLSBCat3>maxN1EndcapLSBCat3) maxN1EndcapLSBCat3 = N1EndcapLSBCat3;
         int N2EndcapLSBCat3=hMassMVAEndcapCat3->Integral(1,8);
         if(N2EndcapLSBCat3<minN2EndcapLSBCat3) minN2EndcapLSBCat3 = N2EndcapLSBCat3;
         if(N2EndcapLSBCat3>maxN2EndcapLSBCat3) maxN2EndcapLSBCat3 = N2EndcapLSBCat3;
         double nMuMuEndcapLSBCat3 = N0EndcapLSBCat3*matrix[0][0]+N1EndcapLSBCat3*matrix[0][1]+N2EndcapLSBCat3*matrix[0][2];
         if(nMuMuEndcapLSBCat3<minnMuMuEndcapLSBCat3) minnMuMuEndcapLSBCat3 = nMuMuEndcapLSBCat3;
         if(nMuMuEndcapLSBCat3>maxnMuMuEndcapLSBCat3) maxnMuMuEndcapLSBCat3 = nMuMuEndcapLSBCat3;
         double nMuHEndcapLSBCat3 = N0EndcapLSBCat3*matrix[1][0]+N1EndcapLSBCat3*matrix[1][1]+N2EndcapLSBCat3*matrix[1][2];
         if(nMuHEndcapLSBCat3<minnMuHEndcapLSBCat3) minnMuHEndcapLSBCat3 = nMuHEndcapLSBCat3;
         if(nMuHEndcapLSBCat3>maxnMuHEndcapLSBCat3) maxnMuHEndcapLSBCat3 = nMuHEndcapLSBCat3;
         double nHHEndcapLSBCat3 = N0EndcapLSBCat3*matrix[2][0]+N1EndcapLSBCat3*matrix[2][1]+N2EndcapLSBCat3*matrix[2][2];
         if(nHHEndcapLSBCat3<minnHHEndcapLSBCat3) minnHHEndcapLSBCat3 = nHHEndcapLSBCat3;
         if(nHHEndcapLSBCat3>maxnHHEndcapLSBCat3) maxnHHEndcapLSBCat3 = nHHEndcapLSBCat3;
         double nMuMuMVAEndcapLSBCat3 = nMuMuEndcapLSBCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapLSBCat3<minnMuMuMVAEndcapLSBCat3) minnMuMuMVAEndcapLSBCat3 = nMuMuMVAEndcapLSBCat3;
         if(nMuMuMVAEndcapLSBCat3>maxnMuMuMVAEndcapLSBCat3) maxnMuMuMVAEndcapLSBCat3 = nMuMuMVAEndcapLSBCat3;
         double nMuHMVAEndcapLSBCat3 = nMuHEndcapLSBCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapLSBCat3<minnMuHMVAEndcapLSBCat3) minnMuHMVAEndcapLSBCat3 = nMuHMVAEndcapLSBCat3;
         if(nMuHMVAEndcapLSBCat3>maxnMuHMVAEndcapLSBCat3) maxnMuHMVAEndcapLSBCat3 = nMuHMVAEndcapLSBCat3;
         double nHHMVAEndcapLSBCat3 = nHHEndcapLSBCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapLSBCat3<minnHHMVAEndcapLSBCat3) minnHHMVAEndcapLSBCat3 = nHHMVAEndcapLSBCat3;
         if(nHHMVAEndcapLSBCat3>maxnHHMVAEndcapLSBCat3) maxnHHMVAEndcapLSBCat3 = nHHMVAEndcapLSBCat3;
         
         std::cout << "\nN0EndcapLSBCat3 = " << N0EndcapLSBCat3 << std::endl;
         std::cout << "N1EndcapLSBCat3 = " << N1EndcapLSBCat3 << std::endl;
         std::cout << "N2EndcapLSBCat3 = " << N2EndcapLSBCat3 << std::endl;
         
         std::cout << "nMuMuEndcapLSBCat3 = " << nMuMuEndcapLSBCat3 << std::endl;
         std::cout << "nMuHEndcapLSBCat3 = " << nMuHEndcapLSBCat3 << std::endl;
         std::cout << "nHHEndcapLSBCat3 = " << nHHEndcapLSBCat3 << std::endl;
         std::cout << "nMuMuMVAEndcapLSBCat3 = " << nMuMuMVAEndcapLSBCat3 << std::endl;
         std::cout << "nMuHMVAEndcapLSBCat3 = " << nMuHMVAEndcapLSBCat3 << std::endl;
         std::cout << "nHHMVAEndcapLSBCat3 = " << nHHMVAEndcapLSBCat3 << std::endl;
         
         int N0EndcapSigCat3=hMassFail2EndcapCat3->Integral(9,15);
         if(N0EndcapSigCat3<minN0EndcapSigCat3) minN0EndcapSigCat3 = N0EndcapSigCat3;
         if(N0EndcapSigCat3>maxN0EndcapSigCat3) maxN0EndcapSigCat3 = N0EndcapSigCat3;
         int N1EndcapSigCat3=hMassFail1EndcapCat3->Integral(9,15);
         if(N1EndcapSigCat3<minN1EndcapSigCat3) minN1EndcapSigCat3 = N1EndcapSigCat3;
         if(N1EndcapSigCat3>maxN1EndcapSigCat3) maxN1EndcapSigCat3 = N1EndcapSigCat3;
         int N2EndcapSigCat3=hMassMVAEndcapCat3->Integral(9,15);
         if(N2EndcapSigCat3<minN2EndcapSigCat3) minN2EndcapSigCat3 = N2EndcapSigCat3;
         if(N2EndcapSigCat3>maxN2EndcapSigCat3) maxN2EndcapSigCat3 = N2EndcapSigCat3;
         double nMuMuEndcapSigCat3 = N0EndcapSigCat3*matrix[0][0]+N1EndcapSigCat3*matrix[0][1]+N2EndcapSigCat3*matrix[0][2];
         if(nMuMuEndcapSigCat3<minnMuMuEndcapSigCat3) minnMuMuEndcapSigCat3 = nMuMuEndcapSigCat3;
         if(nMuMuEndcapSigCat3>maxnMuMuEndcapSigCat3) maxnMuMuEndcapSigCat3 = nMuMuEndcapSigCat3;
         double nMuHEndcapSigCat3 = N0EndcapSigCat3*matrix[1][0]+N1EndcapSigCat3*matrix[1][1]+N2EndcapSigCat3*matrix[1][2];
         if(nMuHEndcapSigCat3<minnMuHEndcapSigCat3) minnMuHEndcapSigCat3 = nMuHEndcapSigCat3;
         if(nMuHEndcapSigCat3>maxnMuHEndcapSigCat3) maxnMuHEndcapSigCat3 = nMuHEndcapSigCat3;
         double nHHEndcapSigCat3 = N0EndcapSigCat3*matrix[2][0]+N1EndcapSigCat3*matrix[2][1]+N2EndcapSigCat3*matrix[2][2];
         if(nHHEndcapSigCat3<minnHHEndcapSigCat3) minnHHEndcapSigCat3 = nHHEndcapSigCat3;
         if(nHHEndcapSigCat3>maxnHHEndcapSigCat3) maxnHHEndcapSigCat3 = nHHEndcapSigCat3;
         double nMuMuMVAEndcapSigCat3 = nMuMuEndcapSigCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapSigCat3<minnMuMuMVAEndcapSigCat3) minnMuMuMVAEndcapSigCat3 = nMuMuMVAEndcapSigCat3;
         if(nMuMuMVAEndcapSigCat3>maxnMuMuMVAEndcapSigCat3) maxnMuMuMVAEndcapSigCat3 = nMuMuMVAEndcapSigCat3;
         double nMuHMVAEndcapSigCat3 = nMuHEndcapSigCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapSigCat3<minnMuHMVAEndcapSigCat3) minnMuHMVAEndcapSigCat3 = nMuHMVAEndcapSigCat3;
         if(nMuHMVAEndcapSigCat3>maxnMuHMVAEndcapSigCat3) maxnMuHMVAEndcapSigCat3 = nMuHMVAEndcapSigCat3;
         double nHHMVAEndcapSigCat3 = nHHEndcapSigCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapSigCat3<minnHHMVAEndcapSigCat3) minnHHMVAEndcapSigCat3 = nHHMVAEndcapSigCat3;
         if(nHHMVAEndcapSigCat3>maxnHHMVAEndcapSigCat3) maxnHHMVAEndcapSigCat3 = nHHMVAEndcapSigCat3;
         
         std::cout << "\nN0EndcapSigCat3 = " << N0EndcapSigCat3 << std::endl;
         std::cout << "N1EndcapSigCat3 = " << N1EndcapSigCat3 << std::endl;
         std::cout << "N2EndcapSigCat3 = " << N2EndcapSigCat3 << std::endl;
         
         std::cout << "nMuMuEndcapSigCat3 = " << nMuMuEndcapSigCat3 << std::endl;
         std::cout << "nMuHEndcapSigCat3 = " << nMuHEndcapSigCat3 << std::endl;
         std::cout << "nHHEndcapSigCat3 = " << nHHEndcapSigCat3 << std::endl;
         std::cout << "nMuMuMVAEndcapSigCat3 = " << nMuMuMVAEndcapSigCat3 << std::endl;
         std::cout << "nMuHMVAEndcapSigCat3 = " << nMuHMVAEndcapSigCat3 << std::endl;
         std::cout << "nHHMVAEndcapSigCat3 = " << nHHMVAEndcapSigCat3 << std::endl;
         
         int N0EndcapRSBCat3=hMassFail2EndcapCat3->Integral(16,25);
         if(N0EndcapRSBCat3<minN0EndcapRSBCat3) minN0EndcapRSBCat3 = N0EndcapRSBCat3;
         if(N0EndcapRSBCat3>maxN0EndcapRSBCat3) maxN0EndcapRSBCat3 = N0EndcapRSBCat3;
         int N1EndcapRSBCat3=hMassFail1EndcapCat3->Integral(16,25);
         if(N1EndcapRSBCat3<minN1EndcapRSBCat3) minN1EndcapRSBCat3 = N1EndcapRSBCat3;
         if(N1EndcapRSBCat3>maxN1EndcapRSBCat3) maxN1EndcapRSBCat3 = N1EndcapRSBCat3;
         int N2EndcapRSBCat3=hMassMVAEndcapCat3->Integral(16,25);
         if(N2EndcapRSBCat3<minN2EndcapRSBCat3) minN2EndcapRSBCat3 = N2EndcapRSBCat3;
         if(N2EndcapRSBCat3>maxN2EndcapRSBCat3) maxN2EndcapRSBCat3 = N2EndcapRSBCat3;
         double nMuMuEndcapRSBCat3 = N0EndcapRSBCat3*matrix[0][0]+N1EndcapRSBCat3*matrix[0][1]+N2EndcapRSBCat3*matrix[0][2];
         if(nMuMuEndcapRSBCat3<minnMuMuEndcapRSBCat3) minnMuMuEndcapRSBCat3 = nMuMuEndcapRSBCat3;
         if(nMuMuEndcapRSBCat3>maxnMuMuEndcapRSBCat3) maxnMuMuEndcapRSBCat3 = nMuMuEndcapRSBCat3;
         double nMuHEndcapRSBCat3 = N0EndcapRSBCat3*matrix[1][0]+N1EndcapRSBCat3*matrix[1][1]+N2EndcapRSBCat3*matrix[1][2];
         if(nMuHEndcapRSBCat3<minnMuHEndcapRSBCat3) minnMuHEndcapRSBCat3 = nMuHEndcapRSBCat3;
         if(nMuHEndcapRSBCat3>maxnMuHEndcapRSBCat3) maxnMuHEndcapRSBCat3 = nMuHEndcapRSBCat3;
         double nHHEndcapRSBCat3 = N0EndcapRSBCat3*matrix[2][0]+N1EndcapRSBCat3*matrix[2][1]+N2EndcapRSBCat3*matrix[2][2];
         if(nHHEndcapRSBCat3<minnHHEndcapRSBCat3) minnHHEndcapRSBCat3 = nHHEndcapRSBCat3;
         if(nHHEndcapRSBCat3>maxnHHEndcapRSBCat3) maxnHHEndcapRSBCat3 = nHHEndcapRSBCat3;
         double nMuMuMVAEndcapRSBCat3 = nMuMuEndcapRSBCat3*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapRSBCat3<minnMuMuMVAEndcapRSBCat3) minnMuMuMVAEndcapRSBCat3 = nMuMuMVAEndcapRSBCat3;
         if(nMuMuMVAEndcapRSBCat3>maxnMuMuMVAEndcapRSBCat3) maxnMuMuMVAEndcapRSBCat3 = nMuMuMVAEndcapRSBCat3;
         double nMuHMVAEndcapRSBCat3 = nMuHEndcapRSBCat3*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapRSBCat3<minnMuHMVAEndcapRSBCat3) minnMuHMVAEndcapRSBCat3 = nMuHMVAEndcapRSBCat3;
         if(nMuHMVAEndcapRSBCat3>maxnMuHMVAEndcapRSBCat3) maxnMuHMVAEndcapRSBCat3 = nMuHMVAEndcapRSBCat3;
         double nHHMVAEndcapRSBCat3 = nHHEndcapRSBCat3*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapRSBCat3<minnHHMVAEndcapRSBCat3) minnHHMVAEndcapRSBCat3 = nHHMVAEndcapRSBCat3;
         if(nHHMVAEndcapRSBCat3>maxnHHMVAEndcapRSBCat3) maxnHHMVAEndcapRSBCat3 = nHHMVAEndcapRSBCat3;
         
         std::cout << "\nN0EndcapRSBCat3 = " << N0EndcapRSBCat3 << std::endl;
         std::cout << "N1EndcapRSBCat3 = " << N1EndcapRSBCat3 << std::endl;
         std::cout << "N2EndcapRSBCat3 = " << N2EndcapRSBCat3 << std::endl;
         
         std::cout << "nMuMuEndcapRSBCat3 = " << nMuMuEndcapRSBCat3 << std::endl;
         std::cout << "nMuHEndcapRSBCat3 = " << nMuHEndcapRSBCat3 << std::endl;
         std::cout << "nHHEndcapRSBCat3 = " << nHHEndcapRSBCat3 << std::endl;
         std::cout << "nMuMuMVAEndcapRSBCat3 = " << nMuMuMVAEndcapRSBCat3 << std::endl;
         std::cout << "nMuHMVAEndcapRSBCat3 = " << nMuHMVAEndcapRSBCat3 << std::endl;
         std::cout << "nHHMVAEndcapRSBCat3 = " << nHHMVAEndcapRSBCat3 << std::endl;
         
         int N0BarrelAllCat4=hMassFail2BarrelCat4->Integral(1,25);
         if(N0BarrelAllCat4<minN0BarrelAllCat4) minN0BarrelAllCat4 = N0BarrelAllCat4;
         if(N0BarrelAllCat4>maxN0BarrelAllCat4) maxN0BarrelAllCat4 = N0BarrelAllCat4;
         int N1BarrelAllCat4=hMassFail1BarrelCat4->Integral(1,25);
         if(N1BarrelAllCat4<minN1BarrelAllCat4) minN1BarrelAllCat4 = N1BarrelAllCat4;
         if(N1BarrelAllCat4>maxN1BarrelAllCat4) maxN1BarrelAllCat4 = N1BarrelAllCat4;
         int N2BarrelAllCat4=hMassMVABarrelCat4->Integral(1,25);
         if(N2BarrelAllCat4<minN2BarrelAllCat4) minN2BarrelAllCat4 = N2BarrelAllCat4;
         if(N2BarrelAllCat4>maxN2BarrelAllCat4) maxN2BarrelAllCat4 = N2BarrelAllCat4;
         double nMuMuBarrelAllCat4 = N0BarrelAllCat4*matrix[0][0]+N1BarrelAllCat4*matrix[0][1]+N2BarrelAllCat4*matrix[0][2];
         if(nMuMuBarrelAllCat4<minnMuMuBarrelAllCat4) minnMuMuBarrelAllCat4 = nMuMuBarrelAllCat4;
         if(nMuMuBarrelAllCat4>maxnMuMuBarrelAllCat4) maxnMuMuBarrelAllCat4 = nMuMuBarrelAllCat4;
         double nMuHBarrelAllCat4 = N0BarrelAllCat4*matrix[1][0]+N1BarrelAllCat4*matrix[1][1]+N2BarrelAllCat4*matrix[1][2];
         if(nMuHBarrelAllCat4<minnMuHBarrelAllCat4) minnMuHBarrelAllCat4 = nMuHBarrelAllCat4;
         if(nMuHBarrelAllCat4>maxnMuHBarrelAllCat4) maxnMuHBarrelAllCat4 = nMuHBarrelAllCat4;
         double nHHBarrelAllCat4 = N0BarrelAllCat4*matrix[2][0]+N1BarrelAllCat4*matrix[2][1]+N2BarrelAllCat4*matrix[2][2];
         if(nHHBarrelAllCat4<minnHHBarrelAllCat4) minnHHBarrelAllCat4 = nHHBarrelAllCat4;
         if(nHHBarrelAllCat4>maxnHHBarrelAllCat4) maxnHHBarrelAllCat4 = nHHBarrelAllCat4;
         double nMuMuMVABarrelAllCat4 = nMuMuBarrelAllCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelAllCat4<minnMuMuMVABarrelAllCat4) minnMuMuMVABarrelAllCat4 = nMuMuMVABarrelAllCat4;
         if(nMuMuMVABarrelAllCat4>maxnMuMuMVABarrelAllCat4) maxnMuMuMVABarrelAllCat4 = nMuMuMVABarrelAllCat4;
         double nMuHMVABarrelAllCat4 = nMuHBarrelAllCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelAllCat4<minnMuHMVABarrelAllCat4) minnMuHMVABarrelAllCat4 = nMuHMVABarrelAllCat4;
         if(nMuHMVABarrelAllCat4>maxnMuHMVABarrelAllCat4) maxnMuHMVABarrelAllCat4 = nMuHMVABarrelAllCat4;
         double nHHMVABarrelAllCat4 = nHHBarrelAllCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelAllCat4<minnHHMVABarrelAllCat4) minnHHMVABarrelAllCat4 = nHHMVABarrelAllCat4;
         if(nHHMVABarrelAllCat4>maxnHHMVABarrelAllCat4) maxnHHMVABarrelAllCat4 = nHHMVABarrelAllCat4;
         
         std::cout << "\nN0BarrelAllCat4 = " << N0BarrelAllCat4 << std::endl;
         std::cout << "N1BarrelAllCat4 = " << N1BarrelAllCat4 << std::endl;
         std::cout << "N2BarrelAllCat4 = " << N2BarrelAllCat4 << std::endl;
         
         std::cout << "nMuMuBarrelAllCat4 = " << nMuMuBarrelAllCat4 << std::endl;
         std::cout << "nMuHBarrelAllCat4 = " << nMuHBarrelAllCat4 << std::endl;
         std::cout << "nHHBarrelAllCat4 = " << nHHBarrelAllCat4 << std::endl;
         std::cout << "nMuMuMVABarrelAllCat4 = " << nMuMuMVABarrelAllCat4 << std::endl;
         std::cout << "nMuHMVABarrelAllCat4 = " << nMuHMVABarrelAllCat4 << std::endl;
         std::cout << "nHHMVABarrelAllCat4 = " << nHHMVABarrelAllCat4 << std::endl;
         
         int N0BarrelLSBCat4=hMassFail2BarrelCat4->Integral(1,8);
         if(N0BarrelLSBCat4<minN0BarrelLSBCat4) minN0BarrelLSBCat4 = N0BarrelLSBCat4;
         if(N0BarrelLSBCat4>maxN0BarrelLSBCat4) maxN0BarrelLSBCat4 = N0BarrelLSBCat4;
         int N1BarrelLSBCat4=hMassFail1BarrelCat4->Integral(1,8);
         if(N1BarrelLSBCat4<minN1BarrelLSBCat4) minN1BarrelLSBCat4 = N1BarrelLSBCat4;
         if(N1BarrelLSBCat4>maxN1BarrelLSBCat4) maxN1BarrelLSBCat4 = N1BarrelLSBCat4;
         int N2BarrelLSBCat4=hMassMVABarrelCat4->Integral(1,8);
         if(N2BarrelLSBCat4<minN2BarrelLSBCat4) minN2BarrelLSBCat4 = N2BarrelLSBCat4;
         if(N2BarrelLSBCat4>maxN2BarrelLSBCat4) maxN2BarrelLSBCat4 = N2BarrelLSBCat4;
         double nMuMuBarrelLSBCat4 = N0BarrelLSBCat4*matrix[0][0]+N1BarrelLSBCat4*matrix[0][1]+N2BarrelLSBCat4*matrix[0][2];
         if(nMuMuBarrelLSBCat4<minnMuMuBarrelLSBCat4) minnMuMuBarrelLSBCat4 = nMuMuBarrelLSBCat4;
         if(nMuMuBarrelLSBCat4>maxnMuMuBarrelLSBCat4) maxnMuMuBarrelLSBCat4 = nMuMuBarrelLSBCat4;
         double nMuHBarrelLSBCat4 = N0BarrelLSBCat4*matrix[1][0]+N1BarrelLSBCat4*matrix[1][1]+N2BarrelLSBCat4*matrix[1][2];
         if(nMuHBarrelLSBCat4<minnMuHBarrelLSBCat4) minnMuHBarrelLSBCat4 = nMuHBarrelLSBCat4;
         if(nMuHBarrelLSBCat4>maxnMuHBarrelLSBCat4) maxnMuHBarrelLSBCat4 = nMuHBarrelLSBCat4;
         double nHHBarrelLSBCat4 = N0BarrelLSBCat4*matrix[2][0]+N1BarrelLSBCat4*matrix[2][1]+N2BarrelLSBCat4*matrix[2][2];
         if(nHHBarrelLSBCat4<minnHHBarrelLSBCat4) minnHHBarrelLSBCat4 = nHHBarrelLSBCat4;
         if(nHHBarrelLSBCat4>maxnHHBarrelLSBCat4) maxnHHBarrelLSBCat4 = nHHBarrelLSBCat4;
         double nMuMuMVABarrelLSBCat4 = nMuMuBarrelLSBCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelLSBCat4<minnMuMuMVABarrelLSBCat4) minnMuMuMVABarrelLSBCat4 = nMuMuMVABarrelLSBCat4;
         if(nMuMuMVABarrelLSBCat4>maxnMuMuMVABarrelLSBCat4) maxnMuMuMVABarrelLSBCat4 = nMuMuMVABarrelLSBCat4;
         double nMuHMVABarrelLSBCat4 = nMuHBarrelLSBCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelLSBCat4<minnMuHMVABarrelLSBCat4) minnMuHMVABarrelLSBCat4 = nMuHMVABarrelLSBCat4;
         if(nMuHMVABarrelLSBCat4>maxnMuHMVABarrelLSBCat4) maxnMuHMVABarrelLSBCat4 = nMuHMVABarrelLSBCat4;
         double nHHMVABarrelLSBCat4 = nHHBarrelLSBCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelLSBCat4<minnHHMVABarrelLSBCat4) minnHHMVABarrelLSBCat4 = nHHMVABarrelLSBCat4;
         if(nHHMVABarrelLSBCat4>maxnHHMVABarrelLSBCat4) maxnHHMVABarrelLSBCat4 = nHHMVABarrelLSBCat4;
         
         std::cout << "\nN0BarrelLSBCat4 = " << N0BarrelLSBCat4 << std::endl;
         std::cout << "N1BarrelLSBCat4 = " << N1BarrelLSBCat4 << std::endl;
         std::cout << "N2BarrelLSBCat4 = " << N2BarrelLSBCat4 << std::endl;
         
         std::cout << "nMuMuBarrelLSBCat4 = " << nMuMuBarrelLSBCat4 << std::endl;
         std::cout << "nMuHBarrelLSBCat4 = " << nMuHBarrelLSBCat4 << std::endl;
         std::cout << "nHHBarrelLSBCat4 = " << nHHBarrelLSBCat4 << std::endl;
         std::cout << "nMuMuMVABarrelLSBCat4 = " << nMuMuMVABarrelLSBCat4 << std::endl;
         std::cout << "nMuHMVABarrelLSBCat4 = " << nMuHMVABarrelLSBCat4 << std::endl;
         std::cout << "nHHMVABarrelLSBCat4 = " << nHHMVABarrelLSBCat4 << std::endl;
         
         int N0BarrelSigCat4=hMassFail2BarrelCat4->Integral(9,15);
         if(N0BarrelSigCat4<minN0BarrelSigCat4) minN0BarrelSigCat4 = N0BarrelSigCat4;
         if(N0BarrelSigCat4>maxN0BarrelSigCat4) maxN0BarrelSigCat4 = N0BarrelSigCat4;
         int N1BarrelSigCat4=hMassFail1BarrelCat4->Integral(9,15);
         if(N1BarrelSigCat4<minN1BarrelSigCat4) minN1BarrelSigCat4 = N1BarrelSigCat4;
         if(N1BarrelSigCat4>maxN1BarrelSigCat4) maxN1BarrelSigCat4 = N1BarrelSigCat4;
         int N2BarrelSigCat4=hMassMVABarrelCat4->Integral(9,15);
         if(N2BarrelSigCat4<minN2BarrelSigCat4) minN2BarrelSigCat4 = N2BarrelSigCat4;
         if(N2BarrelSigCat4>maxN2BarrelSigCat4) maxN2BarrelSigCat4 = N2BarrelSigCat4;
         double nMuMuBarrelSigCat4 = N0BarrelSigCat4*matrix[0][0]+N1BarrelSigCat4*matrix[0][1]+N2BarrelSigCat4*matrix[0][2];
         if(nMuMuBarrelSigCat4<minnMuMuBarrelSigCat4) minnMuMuBarrelSigCat4 = nMuMuBarrelSigCat4;
         if(nMuMuBarrelSigCat4>maxnMuMuBarrelSigCat4) maxnMuMuBarrelSigCat4 = nMuMuBarrelSigCat4;
         double nMuHBarrelSigCat4 = N0BarrelSigCat4*matrix[1][0]+N1BarrelSigCat4*matrix[1][1]+N2BarrelSigCat4*matrix[1][2];
         if(nMuHBarrelSigCat4<minnMuHBarrelSigCat4) minnMuHBarrelSigCat4 = nMuHBarrelSigCat4;
         if(nMuHBarrelSigCat4>maxnMuHBarrelSigCat4) maxnMuHBarrelSigCat4 = nMuHBarrelSigCat4;
         double nHHBarrelSigCat4 = N0BarrelSigCat4*matrix[2][0]+N1BarrelSigCat4*matrix[2][1]+N2BarrelSigCat4*matrix[2][2];
         if(nHHBarrelSigCat4<minnHHBarrelSigCat4) minnHHBarrelSigCat4 = nHHBarrelSigCat4;
         if(nHHBarrelSigCat4>maxnHHBarrelSigCat4) maxnHHBarrelSigCat4 = nHHBarrelSigCat4;
         double nMuMuMVABarrelSigCat4 = nMuMuBarrelSigCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelSigCat4<minnMuMuMVABarrelSigCat4) minnMuMuMVABarrelSigCat4 = nMuMuMVABarrelSigCat4;
         if(nMuMuMVABarrelSigCat4>maxnMuMuMVABarrelSigCat4) maxnMuMuMVABarrelSigCat4 = nMuMuMVABarrelSigCat4;
         double nMuHMVABarrelSigCat4 = nMuHBarrelSigCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelSigCat4<minnMuHMVABarrelSigCat4) minnMuHMVABarrelSigCat4 = nMuHMVABarrelSigCat4;
         if(nMuHMVABarrelSigCat4>maxnMuHMVABarrelSigCat4) maxnMuHMVABarrelSigCat4 = nMuHMVABarrelSigCat4;
         double nHHMVABarrelSigCat4 = nHHBarrelSigCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelSigCat4<minnHHMVABarrelSigCat4) minnHHMVABarrelSigCat4 = nHHMVABarrelSigCat4;
         if(nHHMVABarrelSigCat4>maxnHHMVABarrelSigCat4) maxnHHMVABarrelSigCat4 = nHHMVABarrelSigCat4;
         
         std::cout << "\nN0BarrelSigCat4 = " << N0BarrelSigCat4 << std::endl;
         std::cout << "N1BarrelSigCat4 = " << N1BarrelSigCat4 << std::endl;
         std::cout << "N2BarrelSigCat4 = " << N2BarrelSigCat4 << std::endl;
         
         std::cout << "nMuMuBarrelSigCat4 = " << nMuMuBarrelSigCat4 << std::endl;
         std::cout << "nMuHBarrelSigCat4 = " << nMuHBarrelSigCat4 << std::endl;
         std::cout << "nHHBarrelSigCat4 = " << nHHBarrelSigCat4 << std::endl;
         std::cout << "nMuMuMVABarrelSigCat4 = " << nMuMuMVABarrelSigCat4 << std::endl;
         std::cout << "nMuHMVABarrelSigCat4 = " << nMuHMVABarrelSigCat4 << std::endl;
         std::cout << "nHHMVABarrelSigCat4 = " << nHHMVABarrelSigCat4 << std::endl;
         
         int N0BarrelRSBCat4=hMassFail2BarrelCat4->Integral(16,25);
         if(N0BarrelRSBCat4<minN0BarrelRSBCat4) minN0BarrelRSBCat4 = N0BarrelRSBCat4;
         if(N0BarrelRSBCat4>maxN0BarrelRSBCat4) maxN0BarrelRSBCat4 = N0BarrelRSBCat4;
         int N1BarrelRSBCat4=hMassFail1BarrelCat4->Integral(16,25);
         if(N1BarrelRSBCat4<minN1BarrelRSBCat4) minN1BarrelRSBCat4 = N1BarrelRSBCat4;
         if(N1BarrelRSBCat4>maxN1BarrelRSBCat4) maxN1BarrelRSBCat4 = N1BarrelRSBCat4;
         int N2BarrelRSBCat4=hMassMVABarrelCat4->Integral(16,25);
         if(N2BarrelRSBCat4<minN2BarrelRSBCat4) minN2BarrelRSBCat4 = N2BarrelRSBCat4;
         if(N2BarrelRSBCat4>maxN2BarrelRSBCat4) maxN2BarrelRSBCat4 = N2BarrelRSBCat4;
         double nMuMuBarrelRSBCat4 = N0BarrelRSBCat4*matrix[0][0]+N1BarrelRSBCat4*matrix[0][1]+N2BarrelRSBCat4*matrix[0][2];
         if(nMuMuBarrelRSBCat4<minnMuMuBarrelRSBCat4) minnMuMuBarrelRSBCat4 = nMuMuBarrelRSBCat4;
         if(nMuMuBarrelRSBCat4>maxnMuMuBarrelRSBCat4) maxnMuMuBarrelRSBCat4 = nMuMuBarrelRSBCat4;
         double nMuHBarrelRSBCat4 = N0BarrelRSBCat4*matrix[1][0]+N1BarrelRSBCat4*matrix[1][1]+N2BarrelRSBCat4*matrix[1][2];
         if(nMuHBarrelRSBCat4<minnMuHBarrelRSBCat4) minnMuHBarrelRSBCat4 = nMuHBarrelRSBCat4;
         if(nMuHBarrelRSBCat4>maxnMuHBarrelRSBCat4) maxnMuHBarrelRSBCat4 = nMuHBarrelRSBCat4;
         double nHHBarrelRSBCat4 = N0BarrelRSBCat4*matrix[2][0]+N1BarrelRSBCat4*matrix[2][1]+N2BarrelRSBCat4*matrix[2][2];
         if(nHHBarrelRSBCat4<minnHHBarrelRSBCat4) minnHHBarrelRSBCat4 = nHHBarrelRSBCat4;
         if(nHHBarrelRSBCat4>maxnHHBarrelRSBCat4) maxnHHBarrelRSBCat4 = nHHBarrelRSBCat4;
         double nMuMuMVABarrelRSBCat4 = nMuMuBarrelRSBCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVABarrelRSBCat4<minnMuMuMVABarrelRSBCat4) minnMuMuMVABarrelRSBCat4 = nMuMuMVABarrelRSBCat4;
         if(nMuMuMVABarrelRSBCat4>maxnMuMuMVABarrelRSBCat4) maxnMuMuMVABarrelRSBCat4 = nMuMuMVABarrelRSBCat4;
         double nMuHMVABarrelRSBCat4 = nMuHBarrelRSBCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVABarrelRSBCat4<minnMuHMVABarrelRSBCat4) minnMuHMVABarrelRSBCat4 = nMuHMVABarrelRSBCat4;
         if(nMuHMVABarrelRSBCat4>maxnMuHMVABarrelRSBCat4) maxnMuHMVABarrelRSBCat4 = nMuHMVABarrelRSBCat4;
         double nHHMVABarrelRSBCat4 = nHHBarrelRSBCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVABarrelRSBCat4<minnHHMVABarrelRSBCat4) minnHHMVABarrelRSBCat4 = nHHMVABarrelRSBCat4;
         if(nHHMVABarrelRSBCat4>maxnHHMVABarrelRSBCat4) maxnHHMVABarrelRSBCat4 = nHHMVABarrelRSBCat4;
         
         std::cout << "\nN0BarrelRSBCat4 = " << N0BarrelRSBCat4 << std::endl;
         std::cout << "N1BarrelRSBCat4 = " << N1BarrelRSBCat4 << std::endl;
         std::cout << "N2BarrelRSBCat4 = " << N2BarrelRSBCat4 << std::endl;
         
         std::cout << "nMuMuBarrelRSBCat4 = " << nMuMuBarrelRSBCat4 << std::endl;
         std::cout << "nMuHBarrelRSBCat4 = " << nMuHBarrelRSBCat4 << std::endl;
         std::cout << "nHHBarrelRSBCat4 = " << nHHBarrelRSBCat4 << std::endl;
         std::cout << "nMuMuMVABarrelRSBCat4 = " << nMuMuMVABarrelRSBCat4 << std::endl;
         std::cout << "nMuHMVABarrelRSBCat4 = " << nMuHMVABarrelRSBCat4 << std::endl;
         std::cout << "nHHMVABarrelRSBCat4 = " << nHHMVABarrelRSBCat4 << std::endl;
         
         int N0EndcapAllCat4=hMassFail2EndcapCat4->Integral(1,25);
         if(N0EndcapAllCat4<minN0EndcapAllCat4) minN0EndcapAllCat4 = N0EndcapAllCat4;
         if(N0EndcapAllCat4>maxN0EndcapAllCat4) maxN0EndcapAllCat4 = N0EndcapAllCat4;
         int N1EndcapAllCat4=hMassFail1EndcapCat4->Integral(1,25);
         if(N1EndcapAllCat4<minN1EndcapAllCat4) minN1EndcapAllCat4 = N1EndcapAllCat4;
         if(N1EndcapAllCat4>maxN1EndcapAllCat4) maxN1EndcapAllCat4 = N1EndcapAllCat4;
         int N2EndcapAllCat4=hMassMVAEndcapCat4->Integral(1,25);
         if(N2EndcapAllCat4<minN2EndcapAllCat4) minN2EndcapAllCat4 = N2EndcapAllCat4;
         if(N2EndcapAllCat4>maxN2EndcapAllCat4) maxN2EndcapAllCat4 = N2EndcapAllCat4;
         double nMuMuEndcapAllCat4 = N0EndcapAllCat4*matrix[0][0]+N1EndcapAllCat4*matrix[0][1]+N2EndcapAllCat4*matrix[0][2];
         if(nMuMuEndcapAllCat4<minnMuMuEndcapAllCat4) minnMuMuEndcapAllCat4 = nMuMuEndcapAllCat4;
         if(nMuMuEndcapAllCat4>maxnMuMuEndcapAllCat4) maxnMuMuEndcapAllCat4 = nMuMuEndcapAllCat4;
         double nMuHEndcapAllCat4 = N0EndcapAllCat4*matrix[1][0]+N1EndcapAllCat4*matrix[1][1]+N2EndcapAllCat4*matrix[1][2];
         if(nMuHEndcapAllCat4<minnMuHEndcapAllCat4) minnMuHEndcapAllCat4 = nMuHEndcapAllCat4;
         if(nMuHEndcapAllCat4>maxnMuHEndcapAllCat4) maxnMuHEndcapAllCat4 = nMuHEndcapAllCat4;
         double nHHEndcapAllCat4 = N0EndcapAllCat4*matrix[2][0]+N1EndcapAllCat4*matrix[2][1]+N2EndcapAllCat4*matrix[2][2];
         if(nHHEndcapAllCat4<minnHHEndcapAllCat4) minnHHEndcapAllCat4 = nHHEndcapAllCat4;
         if(nHHEndcapAllCat4>maxnHHEndcapAllCat4) maxnHHEndcapAllCat4 = nHHEndcapAllCat4;
         double nMuMuMVAEndcapAllCat4 = nMuMuEndcapAllCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapAllCat4<minnMuMuMVAEndcapAllCat4) minnMuMuMVAEndcapAllCat4 = nMuMuMVAEndcapAllCat4;
         if(nMuMuMVAEndcapAllCat4>maxnMuMuMVAEndcapAllCat4) maxnMuMuMVAEndcapAllCat4 = nMuMuMVAEndcapAllCat4;
         double nMuHMVAEndcapAllCat4 = nMuHEndcapAllCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapAllCat4<minnMuHMVAEndcapAllCat4) minnMuHMVAEndcapAllCat4 = nMuHMVAEndcapAllCat4;
         if(nMuHMVAEndcapAllCat4>maxnMuHMVAEndcapAllCat4) maxnMuHMVAEndcapAllCat4 = nMuHMVAEndcapAllCat4;
         double nHHMVAEndcapAllCat4 = nHHEndcapAllCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapAllCat4<minnHHMVAEndcapAllCat4) minnHHMVAEndcapAllCat4 = nHHMVAEndcapAllCat4;
         if(nHHMVAEndcapAllCat4>maxnHHMVAEndcapAllCat4) maxnHHMVAEndcapAllCat4 = nHHMVAEndcapAllCat4;
         
         std::cout << "\nN0EndcapAllCat4 = " << N0EndcapAllCat4 << std::endl;
         std::cout << "N1EndcapAllCat4 = " << N1EndcapAllCat4 << std::endl;
         std::cout << "N2EndcapAllCat4 = " << N2EndcapAllCat4 << std::endl;
         
         std::cout << "nMuMuEndcapAllCat4 = " << nMuMuEndcapAllCat4 << std::endl;
         std::cout << "nMuHEndcapAllCat4 = " << nMuHEndcapAllCat4 << std::endl;
         std::cout << "nHHEndcapAllCat4 = " << nHHEndcapAllCat4 << std::endl;
         std::cout << "nMuMuMVAEndcapAllCat4 = " << nMuMuMVAEndcapAllCat4 << std::endl;
         std::cout << "nMuHMVAEndcapAllCat4 = " << nMuHMVAEndcapAllCat4 << std::endl;
         std::cout << "nHHMVAEndcapAllCat4 = " << nHHMVAEndcapAllCat4 << std::endl;
         
         int N0EndcapLSBCat4=hMassFail2EndcapCat4->Integral(1,8);
         if(N0EndcapLSBCat4<minN0EndcapLSBCat4) minN0EndcapLSBCat4 = N0EndcapLSBCat4;
         if(N0EndcapLSBCat4>maxN0EndcapLSBCat4) maxN0EndcapLSBCat4 = N0EndcapLSBCat4;
         int N1EndcapLSBCat4=hMassFail1EndcapCat4->Integral(1,8);
         if(N1EndcapLSBCat4<minN1EndcapLSBCat4) minN1EndcapLSBCat4 = N1EndcapLSBCat4;
         if(N1EndcapLSBCat4>maxN1EndcapLSBCat4) maxN1EndcapLSBCat4 = N1EndcapLSBCat4;
         int N2EndcapLSBCat4=hMassMVAEndcapCat4->Integral(1,8);
         if(N2EndcapLSBCat4<minN2EndcapLSBCat4) minN2EndcapLSBCat4 = N2EndcapLSBCat4;
         if(N2EndcapLSBCat4>maxN2EndcapLSBCat4) maxN2EndcapLSBCat4 = N2EndcapLSBCat4;
         double nMuMuEndcapLSBCat4 = N0EndcapLSBCat4*matrix[0][0]+N1EndcapLSBCat4*matrix[0][1]+N2EndcapLSBCat4*matrix[0][2];
         if(nMuMuEndcapLSBCat4<minnMuMuEndcapLSBCat4) minnMuMuEndcapLSBCat4 = nMuMuEndcapLSBCat4;
         if(nMuMuEndcapLSBCat4>maxnMuMuEndcapLSBCat4) maxnMuMuEndcapLSBCat4 = nMuMuEndcapLSBCat4;
         double nMuHEndcapLSBCat4 = N0EndcapLSBCat4*matrix[1][0]+N1EndcapLSBCat4*matrix[1][1]+N2EndcapLSBCat4*matrix[1][2];
         if(nMuHEndcapLSBCat4<minnMuHEndcapLSBCat4) minnMuHEndcapLSBCat4 = nMuHEndcapLSBCat4;
         if(nMuHEndcapLSBCat4>maxnMuHEndcapLSBCat4) maxnMuHEndcapLSBCat4 = nMuHEndcapLSBCat4;
         double nHHEndcapLSBCat4 = N0EndcapLSBCat4*matrix[2][0]+N1EndcapLSBCat4*matrix[2][1]+N2EndcapLSBCat4*matrix[2][2];
         if(nHHEndcapLSBCat4<minnHHEndcapLSBCat4) minnHHEndcapLSBCat4 = nHHEndcapLSBCat4;
         if(nHHEndcapLSBCat4>maxnHHEndcapLSBCat4) maxnHHEndcapLSBCat4 = nHHEndcapLSBCat4;
         double nMuMuMVAEndcapLSBCat4 = nMuMuEndcapLSBCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapLSBCat4<minnMuMuMVAEndcapLSBCat4) minnMuMuMVAEndcapLSBCat4 = nMuMuMVAEndcapLSBCat4;
         if(nMuMuMVAEndcapLSBCat4>maxnMuMuMVAEndcapLSBCat4) maxnMuMuMVAEndcapLSBCat4 = nMuMuMVAEndcapLSBCat4;
         double nMuHMVAEndcapLSBCat4 = nMuHEndcapLSBCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapLSBCat4<minnMuHMVAEndcapLSBCat4) minnMuHMVAEndcapLSBCat4 = nMuHMVAEndcapLSBCat4;
         if(nMuHMVAEndcapLSBCat4>maxnMuHMVAEndcapLSBCat4) maxnMuHMVAEndcapLSBCat4 = nMuHMVAEndcapLSBCat4;
         double nHHMVAEndcapLSBCat4 = nHHEndcapLSBCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapLSBCat4<minnHHMVAEndcapLSBCat4) minnHHMVAEndcapLSBCat4 = nHHMVAEndcapLSBCat4;
         if(nHHMVAEndcapLSBCat4>maxnHHMVAEndcapLSBCat4) maxnHHMVAEndcapLSBCat4 = nHHMVAEndcapLSBCat4;
         
         std::cout << "\nN0EndcapLSBCat4 = " << N0EndcapLSBCat4 << std::endl;
         std::cout << "N1EndcapLSBCat4 = " << N1EndcapLSBCat4 << std::endl;
         std::cout << "N2EndcapLSBCat4 = " << N2EndcapLSBCat4 << std::endl;
         
         std::cout << "nMuMuEndcapLSBCat4 = " << nMuMuEndcapLSBCat4 << std::endl;
         std::cout << "nMuHEndcapLSBCat4 = " << nMuHEndcapLSBCat4 << std::endl;
         std::cout << "nHHEndcapLSBCat4 = " << nHHEndcapLSBCat4 << std::endl;
         std::cout << "nMuMuMVAEndcapLSBCat4 = " << nMuMuMVAEndcapLSBCat4 << std::endl;
         std::cout << "nMuHMVAEndcapLSBCat4 = " << nMuHMVAEndcapLSBCat4 << std::endl;
         std::cout << "nHHMVAEndcapLSBCat4 = " << nHHMVAEndcapLSBCat4 << std::endl;
         
         int N0EndcapSigCat4=hMassFail2EndcapCat4->Integral(9,15);
         if(N0EndcapSigCat4<minN0EndcapSigCat4) minN0EndcapSigCat4 = N0EndcapSigCat4;
         if(N0EndcapSigCat4>maxN0EndcapSigCat4) maxN0EndcapSigCat4 = N0EndcapSigCat4;
         int N1EndcapSigCat4=hMassFail1EndcapCat4->Integral(9,15);
         if(N1EndcapSigCat4<minN1EndcapSigCat4) minN1EndcapSigCat4 = N1EndcapSigCat4;
         if(N1EndcapSigCat4>maxN1EndcapSigCat4) maxN1EndcapSigCat4 = N1EndcapSigCat4;
         int N2EndcapSigCat4=hMassMVAEndcapCat4->Integral(9,15);
         if(N2EndcapSigCat4<minN2EndcapSigCat4) minN2EndcapSigCat4 = N2EndcapSigCat4;
         if(N2EndcapSigCat4>maxN2EndcapSigCat4) maxN2EndcapSigCat4 = N2EndcapSigCat4;
         double nMuMuEndcapSigCat4 = N0EndcapSigCat4*matrix[0][0]+N1EndcapSigCat4*matrix[0][1]+N2EndcapSigCat4*matrix[0][2];
         if(nMuMuEndcapSigCat4<minnMuMuEndcapSigCat4) minnMuMuEndcapSigCat4 = nMuMuEndcapSigCat4;
         if(nMuMuEndcapSigCat4>maxnMuMuEndcapSigCat4) maxnMuMuEndcapSigCat4 = nMuMuEndcapSigCat4;
         double nMuHEndcapSigCat4 = N0EndcapSigCat4*matrix[1][0]+N1EndcapSigCat4*matrix[1][1]+N2EndcapSigCat4*matrix[1][2];
         if(nMuHEndcapSigCat4<minnMuHEndcapSigCat4) minnMuHEndcapSigCat4 = nMuHEndcapSigCat4;
         if(nMuHEndcapSigCat4>maxnMuHEndcapSigCat4) maxnMuHEndcapSigCat4 = nMuHEndcapSigCat4;
         double nHHEndcapSigCat4 = N0EndcapSigCat4*matrix[2][0]+N1EndcapSigCat4*matrix[2][1]+N2EndcapSigCat4*matrix[2][2];
         if(nHHEndcapSigCat4<minnHHEndcapSigCat4) minnHHEndcapSigCat4 = nHHEndcapSigCat4;
         if(nHHEndcapSigCat4>maxnHHEndcapSigCat4) maxnHHEndcapSigCat4 = nHHEndcapSigCat4;
         double nMuMuMVAEndcapSigCat4 = nMuMuEndcapSigCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapSigCat4<minnMuMuMVAEndcapSigCat4) minnMuMuMVAEndcapSigCat4 = nMuMuMVAEndcapSigCat4;
         if(nMuMuMVAEndcapSigCat4>maxnMuMuMVAEndcapSigCat4) maxnMuMuMVAEndcapSigCat4 = nMuMuMVAEndcapSigCat4;
         double nMuHMVAEndcapSigCat4 = nMuHEndcapSigCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapSigCat4<minnMuHMVAEndcapSigCat4) minnMuHMVAEndcapSigCat4 = nMuHMVAEndcapSigCat4;
         if(nMuHMVAEndcapSigCat4>maxnMuHMVAEndcapSigCat4) maxnMuHMVAEndcapSigCat4 = nMuHMVAEndcapSigCat4;
         double nHHMVAEndcapSigCat4 = nHHEndcapSigCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapSigCat4<minnHHMVAEndcapSigCat4) minnHHMVAEndcapSigCat4 = nHHMVAEndcapSigCat4;
         if(nHHMVAEndcapSigCat4>maxnHHMVAEndcapSigCat4) maxnHHMVAEndcapSigCat4 = nHHMVAEndcapSigCat4;
         
         std::cout << "\nN0EndcapSigCat4 = " << N0EndcapSigCat4 << std::endl;
         std::cout << "N1EndcapSigCat4 = " << N1EndcapSigCat4 << std::endl;
         std::cout << "N2EndcapSigCat4 = " << N2EndcapSigCat4 << std::endl;
         
         std::cout << "nMuMuEndcapSigCat4 = " << nMuMuEndcapSigCat4 << std::endl;
         std::cout << "nMuHEndcapSigCat4 = " << nMuHEndcapSigCat4 << std::endl;
         std::cout << "nHHEndcapSigCat4 = " << nHHEndcapSigCat4 << std::endl;
         std::cout << "nMuMuMVAEndcapSigCat4 = " << nMuMuMVAEndcapSigCat4 << std::endl;
         std::cout << "nMuHMVAEndcapSigCat4 = " << nMuHMVAEndcapSigCat4 << std::endl;
         std::cout << "nHHMVAEndcapSigCat4 = " << nHHMVAEndcapSigCat4 << std::endl;
         
         int N0EndcapRSBCat4=hMassFail2EndcapCat4->Integral(16,25);
         if(N0EndcapRSBCat4<minN0EndcapRSBCat4) minN0EndcapRSBCat4 = N0EndcapRSBCat4;
         if(N0EndcapRSBCat4>maxN0EndcapRSBCat4) maxN0EndcapRSBCat4 = N0EndcapRSBCat4;
         int N1EndcapRSBCat4=hMassFail1EndcapCat4->Integral(16,25);
         if(N1EndcapRSBCat4<minN1EndcapRSBCat4) minN1EndcapRSBCat4 = N1EndcapRSBCat4;
         if(N1EndcapRSBCat4>maxN1EndcapRSBCat4) maxN1EndcapRSBCat4 = N1EndcapRSBCat4;
         int N2EndcapRSBCat4=hMassMVAEndcapCat4->Integral(16,25);
         if(N2EndcapRSBCat4<minN2EndcapRSBCat4) minN2EndcapRSBCat4 = N2EndcapRSBCat4;
         if(N2EndcapRSBCat4>maxN2EndcapRSBCat4) maxN2EndcapRSBCat4 = N2EndcapRSBCat4;
         double nMuMuEndcapRSBCat4 = N0EndcapRSBCat4*matrix[0][0]+N1EndcapRSBCat4*matrix[0][1]+N2EndcapRSBCat4*matrix[0][2];
         if(nMuMuEndcapRSBCat4<minnMuMuEndcapRSBCat4) minnMuMuEndcapRSBCat4 = nMuMuEndcapRSBCat4;
         if(nMuMuEndcapRSBCat4>maxnMuMuEndcapRSBCat4) maxnMuMuEndcapRSBCat4 = nMuMuEndcapRSBCat4;
         double nMuHEndcapRSBCat4 = N0EndcapRSBCat4*matrix[1][0]+N1EndcapRSBCat4*matrix[1][1]+N2EndcapRSBCat4*matrix[1][2];
         if(nMuHEndcapRSBCat4<minnMuHEndcapRSBCat4) minnMuHEndcapRSBCat4 = nMuHEndcapRSBCat4;
         if(nMuHEndcapRSBCat4>maxnMuHEndcapRSBCat4) maxnMuHEndcapRSBCat4 = nMuHEndcapRSBCat4;
         double nHHEndcapRSBCat4 = N0EndcapRSBCat4*matrix[2][0]+N1EndcapRSBCat4*matrix[2][1]+N2EndcapRSBCat4*matrix[2][2];
         if(nHHEndcapRSBCat4<minnHHEndcapRSBCat4) minnHHEndcapRSBCat4 = nHHEndcapRSBCat4;
         if(nHHEndcapRSBCat4>maxnHHEndcapRSBCat4) maxnHHEndcapRSBCat4 = nHHEndcapRSBCat4;
         double nMuMuMVAEndcapRSBCat4 = nMuMuEndcapRSBCat4*epsilonMu[i]*epsilonMu[i];
         if(nMuMuMVAEndcapRSBCat4<minnMuMuMVAEndcapRSBCat4) minnMuMuMVAEndcapRSBCat4 = nMuMuMVAEndcapRSBCat4;
         if(nMuMuMVAEndcapRSBCat4>maxnMuMuMVAEndcapRSBCat4) maxnMuMuMVAEndcapRSBCat4 = nMuMuMVAEndcapRSBCat4;
         double nMuHMVAEndcapRSBCat4 = nMuHEndcapRSBCat4*epsilonMu[i]*epsilonH1[j];
         if(nMuHMVAEndcapRSBCat4<minnMuHMVAEndcapRSBCat4) minnMuHMVAEndcapRSBCat4 = nMuHMVAEndcapRSBCat4;
         if(nMuHMVAEndcapRSBCat4>maxnMuHMVAEndcapRSBCat4) maxnMuHMVAEndcapRSBCat4 = nMuHMVAEndcapRSBCat4;
         double nHHMVAEndcapRSBCat4 = nHHEndcapRSBCat4*epsilonH2[k]*epsilonH2[k];
         if(nHHMVAEndcapRSBCat4<minnHHMVAEndcapRSBCat4) minnHHMVAEndcapRSBCat4 = nHHMVAEndcapRSBCat4;
         if(nHHMVAEndcapRSBCat4>maxnHHMVAEndcapRSBCat4) maxnHHMVAEndcapRSBCat4 = nHHMVAEndcapRSBCat4;
         
         std::cout << "\nN0EndcapRSBCat4 = " << N0EndcapRSBCat4 << std::endl;
         std::cout << "N1EndcapRSBCat4 = " << N1EndcapRSBCat4 << std::endl;
         std::cout << "N2EndcapRSBCat4 = " << N2EndcapRSBCat4 << std::endl;
         
         std::cout << "nMuMuEndcapRSBCat4 = " << nMuMuEndcapRSBCat4 << std::endl;
         std::cout << "nMuHEndcapRSBCat4 = " << nMuHEndcapRSBCat4 << std::endl;
         std::cout << "nHHEndcapRSBCat4 = " << nHHEndcapRSBCat4 << std::endl;
         std::cout << "nMuMuMVAEndcapRSBCat4 = " << nMuMuMVAEndcapRSBCat4 << std::endl;
         std::cout << "nMuHMVAEndcapRSBCat4 = " << nMuHMVAEndcapRSBCat4 << std::endl;
         std::cout << "nHHMVAEndcapRSBCat4 = " << nHHMVAEndcapRSBCat4 << std::endl;
       }
     }
   }
   
  std::cout << "\n";
  std::cout << "N0BarrelAll = " << minN0BarrelAll << " ... " << maxN0BarrelAll << std::endl;
  std::cout << "N1BarrelAll = " << minN1BarrelAll << " ... " << maxN1BarrelAll  << std::endl;
  std::cout << "N2BarrelAll = " << minN2BarrelAll << " ... " << maxN2BarrelAll  << std::endl;
  
  std::cout << "nMuMuBarrelAll = " << minnMuMuBarrelAll << " ... " << maxnMuMuBarrelAll << std::endl;
  std::cout << "nMuHBarrelAll = " << minnMuHBarrelAll << " ... " << maxnMuHBarrelAll << std::endl;
  std::cout << "nHHBarrelAll = " << minnHHBarrelAll << " ... " << maxnHHBarrelAll << std::endl;
  std::cout << "nMuMuMVABarrelAll = " << minnMuMuMVABarrelAll << " ... " << maxnMuMuMVABarrelAll << std::endl;
  std::cout << "nMuHMVABarrelAll = " << minnMuHMVABarrelAll << " ... " << maxnMuHMVABarrelAll << std::endl;
  std::cout << "nHHMVABarrelAll = " << minnHHMVABarrelAll << " ... " << maxnHHMVABarrelAll << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelLSB = " << minN0BarrelLSB << " ... " << maxN0BarrelLSB << std::endl;
  std::cout << "N1BarrelLSB = " << minN1BarrelLSB << " ... " << maxN1BarrelLSB  << std::endl;
  std::cout << "N2BarrelLSB = " << minN2BarrelLSB << " ... " << maxN2BarrelLSB  << std::endl;
  
  std::cout << "nMuMuBarrelLSB = " << minnMuMuBarrelLSB << " ... " << maxnMuMuBarrelLSB << std::endl;
  std::cout << "nMuHBarrelLSB = " << minnMuHBarrelLSB << " ... " << maxnMuHBarrelLSB << std::endl;
  std::cout << "nHHBarrelLSB = " << minnHHBarrelLSB << " ... " << maxnHHBarrelLSB << std::endl;
  std::cout << "nMuMuMVABarrelLSB = " << minnMuMuMVABarrelLSB << " ... " << maxnMuMuMVABarrelLSB << std::endl;
  std::cout << "nMuHMVABarrelLSB = " << minnMuHMVABarrelLSB << " ... " << maxnMuHMVABarrelLSB << std::endl;
  std::cout << "nHHMVABarrelLSB = " << minnHHMVABarrelLSB << " ... " << maxnHHMVABarrelLSB << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelSig = " << minN0BarrelSig << " ... " << maxN0BarrelSig << std::endl;
  std::cout << "N1BarrelSig = " << minN1BarrelSig << " ... " << maxN1BarrelSig  << std::endl;
  std::cout << "N2BarrelSig = " << minN2BarrelSig << " ... " << maxN2BarrelSig  << std::endl;
  
  std::cout << "nMuMuBarrelSig = " << minnMuMuBarrelSig << " ... " << maxnMuMuBarrelSig << std::endl;
  std::cout << "nMuHBarrelSig = " << minnMuHBarrelSig << " ... " << maxnMuHBarrelSig << std::endl;
  std::cout << "nHHBarrelSig = " << minnHHBarrelSig << " ... " << maxnHHBarrelSig << std::endl;
  std::cout << "nMuMuMVABarrelSig = " << minnMuMuMVABarrelSig << " ... " << maxnMuMuMVABarrelSig << std::endl;
  std::cout << "nMuHMVABarrelSig = " << minnMuHMVABarrelSig << " ... " << maxnMuHMVABarrelSig << std::endl;
  std::cout << "nHHMVABarrelSig = " << minnHHMVABarrelSig << " ... " << maxnHHMVABarrelSig << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelRSB = " << minN0BarrelRSB << " ... " << maxN0BarrelRSB << std::endl;
  std::cout << "N1BarrelRSB = " << minN1BarrelRSB << " ... " << maxN1BarrelRSB  << std::endl;
  std::cout << "N2BarrelRSB = " << minN2BarrelRSB << " ... " << maxN2BarrelRSB  << std::endl;
  
  std::cout << "nMuMuBarrelRSB = " << minnMuMuBarrelRSB << " ... " << maxnMuMuBarrelRSB << std::endl;
  std::cout << "nMuHBarrelRSB = " << minnMuHBarrelRSB << " ... " << maxnMuHBarrelRSB << std::endl;
  std::cout << "nHHBarrelRSB = " << minnHHBarrelRSB << " ... " << maxnHHBarrelRSB << std::endl;
  std::cout << "nMuMuMVABarrelRSB = " << minnMuMuMVABarrelRSB << " ... " << maxnMuMuMVABarrelRSB << std::endl;
  std::cout << "nMuHMVABarrelRSB = " << minnMuHMVABarrelRSB << " ... " << maxnMuHMVABarrelRSB << std::endl;
  std::cout << "nHHMVABarrelRSB = " << minnHHMVABarrelRSB << " ... " << maxnHHMVABarrelRSB << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapAll = " << minN0EndcapAll << " ... " << maxN0EndcapAll << std::endl;
  std::cout << "N1EndcapAll = " << minN1EndcapAll << " ... " << maxN1EndcapAll  << std::endl;
  std::cout << "N2EndcapAll = " << minN2EndcapAll << " ... " << maxN2EndcapAll  << std::endl;
  
  std::cout << "nMuMuEndcapAll = " << minnMuMuEndcapAll << " ... " << maxnMuMuEndcapAll << std::endl;
  std::cout << "nMuHEndcapAll = " << minnMuHEndcapAll << " ... " << maxnMuHEndcapAll << std::endl;
  std::cout << "nHHEndcapAll = " << minnHHEndcapAll << " ... " << maxnHHEndcapAll << std::endl;
  std::cout << "nMuMuMVAEndcapAll = " << minnMuMuMVAEndcapAll << " ... " << maxnMuMuMVAEndcapAll << std::endl;
  std::cout << "nMuHMVAEndcapAll = " << minnMuHMVAEndcapAll << " ... " << maxnMuHMVAEndcapAll << std::endl;
  std::cout << "nHHMVAEndcapAll = " << minnHHMVAEndcapAll << " ... " << maxnHHMVAEndcapAll << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapLSB = " << minN0EndcapLSB << " ... " << maxN0EndcapLSB << std::endl;
  std::cout << "N1EndcapLSB = " << minN1EndcapLSB << " ... " << maxN1EndcapLSB  << std::endl;
  std::cout << "N2EndcapLSB = " << minN2EndcapLSB << " ... " << maxN2EndcapLSB  << std::endl;
  
  std::cout << "nMuMuEndcapLSB = " << minnMuMuEndcapLSB << " ... " << maxnMuMuEndcapLSB << std::endl;
  std::cout << "nMuHEndcapLSB = " << minnMuHEndcapLSB << " ... " << maxnMuHEndcapLSB << std::endl;
  std::cout << "nHHEndcapLSB = " << minnHHEndcapLSB << " ... " << maxnHHEndcapLSB << std::endl;
  std::cout << "nMuMuMVAEndcapLSB = " << minnMuMuMVAEndcapLSB << " ... " << maxnMuMuMVAEndcapLSB << std::endl;
  std::cout << "nMuHMVAEndcapLSB = " << minnMuHMVAEndcapLSB << " ... " << maxnMuHMVAEndcapLSB << std::endl;
  std::cout << "nHHMVAEndcapLSB = " << minnHHMVAEndcapLSB << " ... " << maxnHHMVAEndcapLSB << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapSig = " << minN0EndcapSig << " ... " << maxN0EndcapSig << std::endl;
  std::cout << "N1EndcapSig = " << minN1EndcapSig << " ... " << maxN1EndcapSig  << std::endl;
  std::cout << "N2EndcapSig = " << minN2EndcapSig << " ... " << maxN2EndcapSig  << std::endl;
  
  std::cout << "nMuMuEndcapSig = " << minnMuMuEndcapSig << " ... " << maxnMuMuEndcapSig << std::endl;
  std::cout << "nMuHEndcapSig = " << minnMuHEndcapSig << " ... " << maxnMuHEndcapSig << std::endl;
  std::cout << "nHHEndcapSig = " << minnHHEndcapSig << " ... " << maxnHHEndcapSig << std::endl;
  std::cout << "nMuMuMVAEndcapSig = " << minnMuMuMVAEndcapSig << " ... " << maxnMuMuMVAEndcapSig << std::endl;
  std::cout << "nMuHMVAEndcapSig = " << minnMuHMVAEndcapSig << " ... " << maxnMuHMVAEndcapSig << std::endl;
  std::cout << "nHHMVAEndcapSig = " << minnHHMVAEndcapSig << " ... " << maxnHHMVAEndcapSig << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapRSB = " << minN0EndcapRSB << " ... " << maxN0EndcapRSB << std::endl;
  std::cout << "N1EndcapRSB = " << minN1EndcapRSB << " ... " << maxN1EndcapRSB  << std::endl;
  std::cout << "N2EndcapRSB = " << minN2EndcapRSB << " ... " << maxN2EndcapRSB  << std::endl;
  
  std::cout << "nMuMuEndcapRSB = " << minnMuMuEndcapRSB << " ... " << maxnMuMuEndcapRSB << std::endl;
  std::cout << "nMuHEndcapRSB = " << minnMuHEndcapRSB << " ... " << maxnMuHEndcapRSB << std::endl;
  std::cout << "nHHEndcapRSB = " << minnHHEndcapRSB << " ... " << maxnHHEndcapRSB << std::endl;
  std::cout << "nMuMuMVAEndcapRSB = " << minnMuMuMVAEndcapRSB << " ... " << maxnMuMuMVAEndcapRSB << std::endl;
  std::cout << "nMuHMVAEndcapRSB = " << minnMuHMVAEndcapRSB << " ... " << maxnMuHMVAEndcapRSB << std::endl;
  std::cout << "nHHMVAEndcapRSB = " << minnHHMVAEndcapRSB << " ... " << maxnHHMVAEndcapRSB << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelAllCat1 = " << minN0BarrelAllCat1 << " ... " << maxN0BarrelAllCat1 << std::endl;
  std::cout << "N1BarrelAllCat1 = " << minN1BarrelAllCat1 << " ... " << maxN1BarrelAllCat1  << std::endl;
  std::cout << "N2BarrelAllCat1 = " << minN2BarrelAllCat1 << " ... " << maxN2BarrelAllCat1  << std::endl;
  
  std::cout << "nMuMuBarrelAllCat1 = " << minnMuMuBarrelAllCat1 << " ... " << maxnMuMuBarrelAllCat1 << std::endl;
  std::cout << "nMuHBarrelAllCat1 = " << minnMuHBarrelAllCat1 << " ... " << maxnMuHBarrelAllCat1 << std::endl;
  std::cout << "nHHBarrelAllCat1 = " << minnHHBarrelAllCat1 << " ... " << maxnHHBarrelAllCat1 << std::endl;
  std::cout << "nMuMuMVABarrelAllCat1 = " << minnMuMuMVABarrelAllCat1 << " ... " << maxnMuMuMVABarrelAllCat1 << std::endl;
  std::cout << "nMuHMVABarrelAllCat1 = " << minnMuHMVABarrelAllCat1 << " ... " << maxnMuHMVABarrelAllCat1 << std::endl;
  std::cout << "nHHMVABarrelAllCat1 = " << minnHHMVABarrelAllCat1 << " ... " << maxnHHMVABarrelAllCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelLSBCat1 = " << minN0BarrelLSBCat1 << " ... " << maxN0BarrelLSBCat1 << std::endl;
  std::cout << "N1BarrelLSBCat1 = " << minN1BarrelLSBCat1 << " ... " << maxN1BarrelLSBCat1  << std::endl;
  std::cout << "N2BarrelLSBCat1 = " << minN2BarrelLSBCat1 << " ... " << maxN2BarrelLSBCat1  << std::endl;
  
  std::cout << "nMuMuBarrelLSBCat1 = " << minnMuMuBarrelLSBCat1 << " ... " << maxnMuMuBarrelLSBCat1 << std::endl;
  std::cout << "nMuHBarrelLSBCat1 = " << minnMuHBarrelLSBCat1 << " ... " << maxnMuHBarrelLSBCat1 << std::endl;
  std::cout << "nHHBarrelLSBCat1 = " << minnHHBarrelLSBCat1 << " ... " << maxnHHBarrelLSBCat1 << std::endl;
  std::cout << "nMuMuMVABarrelLSBCat1 = " << minnMuMuMVABarrelLSBCat1 << " ... " << maxnMuMuMVABarrelLSBCat1 << std::endl;
  std::cout << "nMuHMVABarrelLSBCat1 = " << minnMuHMVABarrelLSBCat1 << " ... " << maxnMuHMVABarrelLSBCat1 << std::endl;
  std::cout << "nHHMVABarrelLSBCat1 = " << minnHHMVABarrelLSBCat1 << " ... " << maxnHHMVABarrelLSBCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelSigCat1 = " << minN0BarrelSigCat1 << " ... " << maxN0BarrelSigCat1 << std::endl;
  std::cout << "N1BarrelSigCat1 = " << minN1BarrelSigCat1 << " ... " << maxN1BarrelSigCat1  << std::endl;
  std::cout << "N2BarrelSigCat1 = " << minN2BarrelSigCat1 << " ... " << maxN2BarrelSigCat1  << std::endl;
  
  std::cout << "nMuMuBarrelSigCat1 = " << minnMuMuBarrelSigCat1 << " ... " << maxnMuMuBarrelSigCat1 << std::endl;
  std::cout << "nMuHBarrelSigCat1 = " << minnMuHBarrelSigCat1 << " ... " << maxnMuHBarrelSigCat1 << std::endl;
  std::cout << "nHHBarrelSigCat1 = " << minnHHBarrelSigCat1 << " ... " << maxnHHBarrelSigCat1 << std::endl;
  std::cout << "nMuMuMVABarrelSigCat1 = " << minnMuMuMVABarrelSigCat1 << " ... " << maxnMuMuMVABarrelSigCat1 << std::endl;
  std::cout << "nMuHMVABarrelSigCat1 = " << minnMuHMVABarrelSigCat1 << " ... " << maxnMuHMVABarrelSigCat1 << std::endl;
  std::cout << "nHHMVABarrelSigCat1 = " << minnHHMVABarrelSigCat1 << " ... " << maxnHHMVABarrelSigCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelRSBCat1 = " << minN0BarrelRSBCat1 << " ... " << maxN0BarrelRSBCat1 << std::endl;
  std::cout << "N1BarrelRSBCat1 = " << minN1BarrelRSBCat1 << " ... " << maxN1BarrelRSBCat1  << std::endl;
  std::cout << "N2BarrelRSBCat1 = " << minN2BarrelRSBCat1 << " ... " << maxN2BarrelRSBCat1  << std::endl;
  
  std::cout << "nMuMuBarrelRSBCat1 = " << minnMuMuBarrelRSBCat1 << " ... " << maxnMuMuBarrelRSBCat1 << std::endl;
  std::cout << "nMuHBarrelRSBCat1 = " << minnMuHBarrelRSBCat1 << " ... " << maxnMuHBarrelRSBCat1 << std::endl;
  std::cout << "nHHBarrelRSBCat1 = " << minnHHBarrelRSBCat1 << " ... " << maxnHHBarrelRSBCat1 << std::endl;
  std::cout << "nMuMuMVABarrelRSBCat1 = " << minnMuMuMVABarrelRSBCat1 << " ... " << maxnMuMuMVABarrelRSBCat1 << std::endl;
  std::cout << "nMuHMVABarrelRSBCat1 = " << minnMuHMVABarrelRSBCat1 << " ... " << maxnMuHMVABarrelRSBCat1 << std::endl;
  std::cout << "nHHMVABarrelRSBCat1 = " << minnHHMVABarrelRSBCat1 << " ... " << maxnHHMVABarrelRSBCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapAllCat1 = " << minN0EndcapAllCat1 << " ... " << maxN0EndcapAllCat1 << std::endl;
  std::cout << "N1EndcapAllCat1 = " << minN1EndcapAllCat1 << " ... " << maxN1EndcapAllCat1  << std::endl;
  std::cout << "N2EndcapAllCat1 = " << minN2EndcapAllCat1 << " ... " << maxN2EndcapAllCat1  << std::endl;
  
  std::cout << "nMuMuEndcapAllCat1 = " << minnMuMuEndcapAllCat1 << " ... " << maxnMuMuEndcapAllCat1 << std::endl;
  std::cout << "nMuHEndcapAllCat1 = " << minnMuHEndcapAllCat1 << " ... " << maxnMuHEndcapAllCat1 << std::endl;
  std::cout << "nHHEndcapAllCat1 = " << minnHHEndcapAllCat1 << " ... " << maxnHHEndcapAllCat1 << std::endl;
  std::cout << "nMuMuMVAEndcapAllCat1 = " << minnMuMuMVAEndcapAllCat1 << " ... " << maxnMuMuMVAEndcapAllCat1 << std::endl;
  std::cout << "nMuHMVAEndcapAllCat1 = " << minnMuHMVAEndcapAllCat1 << " ... " << maxnMuHMVAEndcapAllCat1 << std::endl;
  std::cout << "nHHMVAEndcapAllCat1 = " << minnHHMVAEndcapAllCat1 << " ... " << maxnHHMVAEndcapAllCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapLSBCat1 = " << minN0EndcapLSBCat1 << " ... " << maxN0EndcapLSBCat1 << std::endl;
  std::cout << "N1EndcapLSBCat1 = " << minN1EndcapLSBCat1 << " ... " << maxN1EndcapLSBCat1  << std::endl;
  std::cout << "N2EndcapLSBCat1 = " << minN2EndcapLSBCat1 << " ... " << maxN2EndcapLSBCat1  << std::endl;
  
  std::cout << "nMuMuEndcapLSBCat1 = " << minnMuMuEndcapLSBCat1 << " ... " << maxnMuMuEndcapLSBCat1 << std::endl;
  std::cout << "nMuHEndcapLSBCat1 = " << minnMuHEndcapLSBCat1 << " ... " << maxnMuHEndcapLSBCat1 << std::endl;
  std::cout << "nHHEndcapLSBCat1 = " << minnHHEndcapLSBCat1 << " ... " << maxnHHEndcapLSBCat1 << std::endl;
  std::cout << "nMuMuMVAEndcapLSBCat1 = " << minnMuMuMVAEndcapLSBCat1 << " ... " << maxnMuMuMVAEndcapLSBCat1 << std::endl;
  std::cout << "nMuHMVAEndcapLSBCat1 = " << minnMuHMVAEndcapLSBCat1 << " ... " << maxnMuHMVAEndcapLSBCat1 << std::endl;
  std::cout << "nHHMVAEndcapLSBCat1 = " << minnHHMVAEndcapLSBCat1 << " ... " << maxnHHMVAEndcapLSBCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapSigCat1 = " << minN0EndcapSigCat1 << " ... " << maxN0EndcapSigCat1 << std::endl;
  std::cout << "N1EndcapSigCat1 = " << minN1EndcapSigCat1 << " ... " << maxN1EndcapSigCat1  << std::endl;
  std::cout << "N2EndcapSigCat1 = " << minN2EndcapSigCat1 << " ... " << maxN2EndcapSigCat1  << std::endl;
  
  std::cout << "nMuMuEndcapSigCat1 = " << minnMuMuEndcapSigCat1 << " ... " << maxnMuMuEndcapSigCat1 << std::endl;
  std::cout << "nMuHEndcapSigCat1 = " << minnMuHEndcapSigCat1 << " ... " << maxnMuHEndcapSigCat1 << std::endl;
  std::cout << "nHHEndcapSigCat1 = " << minnHHEndcapSigCat1 << " ... " << maxnHHEndcapSigCat1 << std::endl;
  std::cout << "nMuMuMVAEndcapSigCat1 = " << minnMuMuMVAEndcapSigCat1 << " ... " << maxnMuMuMVAEndcapSigCat1 << std::endl;
  std::cout << "nMuHMVAEndcapSigCat1 = " << minnMuHMVAEndcapSigCat1 << " ... " << maxnMuHMVAEndcapSigCat1 << std::endl;
  std::cout << "nHHMVAEndcapSigCat1 = " << minnHHMVAEndcapSigCat1 << " ... " << maxnHHMVAEndcapSigCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapRSBCat1 = " << minN0EndcapRSBCat1 << " ... " << maxN0EndcapRSBCat1 << std::endl;
  std::cout << "N1EndcapRSBCat1 = " << minN1EndcapRSBCat1 << " ... " << maxN1EndcapRSBCat1  << std::endl;
  std::cout << "N2EndcapRSBCat1 = " << minN2EndcapRSBCat1 << " ... " << maxN2EndcapRSBCat1  << std::endl;
  
  std::cout << "nMuMuEndcapRSBCat1 = " << minnMuMuEndcapRSBCat1 << " ... " << maxnMuMuEndcapRSBCat1 << std::endl;
  std::cout << "nMuHEndcapRSBCat1 = " << minnMuHEndcapRSBCat1 << " ... " << maxnMuHEndcapRSBCat1 << std::endl;
  std::cout << "nHHEndcapRSBCat1 = " << minnHHEndcapRSBCat1 << " ... " << maxnHHEndcapRSBCat1 << std::endl;
  std::cout << "nMuMuMVAEndcapRSBCat1 = " << minnMuMuMVAEndcapRSBCat1 << " ... " << maxnMuMuMVAEndcapRSBCat1 << std::endl;
  std::cout << "nMuHMVAEndcapRSBCat1 = " << minnMuHMVAEndcapRSBCat1 << " ... " << maxnMuHMVAEndcapRSBCat1 << std::endl;
  std::cout << "nHHMVAEndcapRSBCat1 = " << minnHHMVAEndcapRSBCat1 << " ... " << maxnHHMVAEndcapRSBCat1 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelAllCat2 = " << minN0BarrelAllCat2 << " ... " << maxN0BarrelAllCat2 << std::endl;
  std::cout << "N1BarrelAllCat2 = " << minN1BarrelAllCat2 << " ... " << maxN1BarrelAllCat2  << std::endl;
  std::cout << "N2BarrelAllCat2 = " << minN2BarrelAllCat2 << " ... " << maxN2BarrelAllCat2  << std::endl;
  
  std::cout << "nMuMuBarrelAllCat2 = " << minnMuMuBarrelAllCat2 << " ... " << maxnMuMuBarrelAllCat2 << std::endl;
  std::cout << "nMuHBarrelAllCat2 = " << minnMuHBarrelAllCat2 << " ... " << maxnMuHBarrelAllCat2 << std::endl;
  std::cout << "nHHBarrelAllCat2 = " << minnHHBarrelAllCat2 << " ... " << maxnHHBarrelAllCat2 << std::endl;
  std::cout << "nMuMuMVABarrelAllCat2 = " << minnMuMuMVABarrelAllCat2 << " ... " << maxnMuMuMVABarrelAllCat2 << std::endl;
  std::cout << "nMuHMVABarrelAllCat2 = " << minnMuHMVABarrelAllCat2 << " ... " << maxnMuHMVABarrelAllCat2 << std::endl;
  std::cout << "nHHMVABarrelAllCat2 = " << minnHHMVABarrelAllCat2 << " ... " << maxnHHMVABarrelAllCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelLSBCat2 = " << minN0BarrelLSBCat2 << " ... " << maxN0BarrelLSBCat2 << std::endl;
  std::cout << "N1BarrelLSBCat2 = " << minN1BarrelLSBCat2 << " ... " << maxN1BarrelLSBCat2  << std::endl;
  std::cout << "N2BarrelLSBCat2 = " << minN2BarrelLSBCat2 << " ... " << maxN2BarrelLSBCat2  << std::endl;
  
  std::cout << "nMuMuBarrelLSBCat2 = " << minnMuMuBarrelLSBCat2 << " ... " << maxnMuMuBarrelLSBCat2 << std::endl;
  std::cout << "nMuHBarrelLSBCat2 = " << minnMuHBarrelLSBCat2 << " ... " << maxnMuHBarrelLSBCat2 << std::endl;
  std::cout << "nHHBarrelLSBCat2 = " << minnHHBarrelLSBCat2 << " ... " << maxnHHBarrelLSBCat2 << std::endl;
  std::cout << "nMuMuMVABarrelLSBCat2 = " << minnMuMuMVABarrelLSBCat2 << " ... " << maxnMuMuMVABarrelLSBCat2 << std::endl;
  std::cout << "nMuHMVABarrelLSBCat2 = " << minnMuHMVABarrelLSBCat2 << " ... " << maxnMuHMVABarrelLSBCat2 << std::endl;
  std::cout << "nHHMVABarrelLSBCat2 = " << minnHHMVABarrelLSBCat2 << " ... " << maxnHHMVABarrelLSBCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelSigCat2 = " << minN0BarrelSigCat2 << " ... " << maxN0BarrelSigCat2 << std::endl;
  std::cout << "N1BarrelSigCat2 = " << minN1BarrelSigCat2 << " ... " << maxN1BarrelSigCat2  << std::endl;
  std::cout << "N2BarrelSigCat2 = " << minN2BarrelSigCat2 << " ... " << maxN2BarrelSigCat2  << std::endl;
  
  std::cout << "nMuMuBarrelSigCat2 = " << minnMuMuBarrelSigCat2 << " ... " << maxnMuMuBarrelSigCat2 << std::endl;
  std::cout << "nMuHBarrelSigCat2 = " << minnMuHBarrelSigCat2 << " ... " << maxnMuHBarrelSigCat2 << std::endl;
  std::cout << "nHHBarrelSigCat2 = " << minnHHBarrelSigCat2 << " ... " << maxnHHBarrelSigCat2 << std::endl;
  std::cout << "nMuMuMVABarrelSigCat2 = " << minnMuMuMVABarrelSigCat2 << " ... " << maxnMuMuMVABarrelSigCat2 << std::endl;
  std::cout << "nMuHMVABarrelSigCat2 = " << minnMuHMVABarrelSigCat2 << " ... " << maxnMuHMVABarrelSigCat2 << std::endl;
  std::cout << "nHHMVABarrelSigCat2 = " << minnHHMVABarrelSigCat2 << " ... " << maxnHHMVABarrelSigCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelRSBCat2 = " << minN0BarrelRSBCat2 << " ... " << maxN0BarrelRSBCat2 << std::endl;
  std::cout << "N1BarrelRSBCat2 = " << minN1BarrelRSBCat2 << " ... " << maxN1BarrelRSBCat2  << std::endl;
  std::cout << "N2BarrelRSBCat2 = " << minN2BarrelRSBCat2 << " ... " << maxN2BarrelRSBCat2  << std::endl;
  
  std::cout << "nMuMuBarrelRSBCat2 = " << minnMuMuBarrelRSBCat2 << " ... " << maxnMuMuBarrelRSBCat2 << std::endl;
  std::cout << "nMuHBarrelRSBCat2 = " << minnMuHBarrelRSBCat2 << " ... " << maxnMuHBarrelRSBCat2 << std::endl;
  std::cout << "nHHBarrelRSBCat2 = " << minnHHBarrelRSBCat2 << " ... " << maxnHHBarrelRSBCat2 << std::endl;
  std::cout << "nMuMuMVABarrelRSBCat2 = " << minnMuMuMVABarrelRSBCat2 << " ... " << maxnMuMuMVABarrelRSBCat2 << std::endl;
  std::cout << "nMuHMVABarrelRSBCat2 = " << minnMuHMVABarrelRSBCat2 << " ... " << maxnMuHMVABarrelRSBCat2 << std::endl;
  std::cout << "nHHMVABarrelRSBCat2 = " << minnHHMVABarrelRSBCat2 << " ... " << maxnHHMVABarrelRSBCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapAllCat2 = " << minN0EndcapAllCat2 << " ... " << maxN0EndcapAllCat2 << std::endl;
  std::cout << "N1EndcapAllCat2 = " << minN1EndcapAllCat2 << " ... " << maxN1EndcapAllCat2  << std::endl;
  std::cout << "N2EndcapAllCat2 = " << minN2EndcapAllCat2 << " ... " << maxN2EndcapAllCat2  << std::endl;
  
  std::cout << "nMuMuEndcapAllCat2 = " << minnMuMuEndcapAllCat2 << " ... " << maxnMuMuEndcapAllCat2 << std::endl;
  std::cout << "nMuHEndcapAllCat2 = " << minnMuHEndcapAllCat2 << " ... " << maxnMuHEndcapAllCat2 << std::endl;
  std::cout << "nHHEndcapAllCat2 = " << minnHHEndcapAllCat2 << " ... " << maxnHHEndcapAllCat2 << std::endl;
  std::cout << "nMuMuMVAEndcapAllCat2 = " << minnMuMuMVAEndcapAllCat2 << " ... " << maxnMuMuMVAEndcapAllCat2 << std::endl;
  std::cout << "nMuHMVAEndcapAllCat2 = " << minnMuHMVAEndcapAllCat2 << " ... " << maxnMuHMVAEndcapAllCat2 << std::endl;
  std::cout << "nHHMVAEndcapAllCat2 = " << minnHHMVAEndcapAllCat2 << " ... " << maxnHHMVAEndcapAllCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapLSBCat2 = " << minN0EndcapLSBCat2 << " ... " << maxN0EndcapLSBCat2 << std::endl;
  std::cout << "N1EndcapLSBCat2 = " << minN1EndcapLSBCat2 << " ... " << maxN1EndcapLSBCat2  << std::endl;
  std::cout << "N2EndcapLSBCat2 = " << minN2EndcapLSBCat2 << " ... " << maxN2EndcapLSBCat2  << std::endl;
  
  std::cout << "nMuMuEndcapLSBCat2 = " << minnMuMuEndcapLSBCat2 << " ... " << maxnMuMuEndcapLSBCat2 << std::endl;
  std::cout << "nMuHEndcapLSBCat2 = " << minnMuHEndcapLSBCat2 << " ... " << maxnMuHEndcapLSBCat2 << std::endl;
  std::cout << "nHHEndcapLSBCat2 = " << minnHHEndcapLSBCat2 << " ... " << maxnHHEndcapLSBCat2 << std::endl;
  std::cout << "nMuMuMVAEndcapLSBCat2 = " << minnMuMuMVAEndcapLSBCat2 << " ... " << maxnMuMuMVAEndcapLSBCat2 << std::endl;
  std::cout << "nMuHMVAEndcapLSBCat2 = " << minnMuHMVAEndcapLSBCat2 << " ... " << maxnMuHMVAEndcapLSBCat2 << std::endl;
  std::cout << "nHHMVAEndcapLSBCat2 = " << minnHHMVAEndcapLSBCat2 << " ... " << maxnHHMVAEndcapLSBCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapSigCat2 = " << minN0EndcapSigCat2 << " ... " << maxN0EndcapSigCat2 << std::endl;
  std::cout << "N1EndcapSigCat2 = " << minN1EndcapSigCat2 << " ... " << maxN1EndcapSigCat2  << std::endl;
  std::cout << "N2EndcapSigCat2 = " << minN2EndcapSigCat2 << " ... " << maxN2EndcapSigCat2  << std::endl;
  
  std::cout << "nMuMuEndcapSigCat2 = " << minnMuMuEndcapSigCat2 << " ... " << maxnMuMuEndcapSigCat2 << std::endl;
  std::cout << "nMuHEndcapSigCat2 = " << minnMuHEndcapSigCat2 << " ... " << maxnMuHEndcapSigCat2 << std::endl;
  std::cout << "nHHEndcapSigCat2 = " << minnHHEndcapSigCat2 << " ... " << maxnHHEndcapSigCat2 << std::endl;
  std::cout << "nMuMuMVAEndcapSigCat2 = " << minnMuMuMVAEndcapSigCat2 << " ... " << maxnMuMuMVAEndcapSigCat2 << std::endl;
  std::cout << "nMuHMVAEndcapSigCat2 = " << minnMuHMVAEndcapSigCat2 << " ... " << maxnMuHMVAEndcapSigCat2 << std::endl;
  std::cout << "nHHMVAEndcapSigCat2 = " << minnHHMVAEndcapSigCat2 << " ... " << maxnHHMVAEndcapSigCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapRSBCat2 = " << minN0EndcapRSBCat2 << " ... " << maxN0EndcapRSBCat2 << std::endl;
  std::cout << "N1EndcapRSBCat2 = " << minN1EndcapRSBCat2 << " ... " << maxN1EndcapRSBCat2  << std::endl;
  std::cout << "N2EndcapRSBCat2 = " << minN2EndcapRSBCat2 << " ... " << maxN2EndcapRSBCat2  << std::endl;
  
  std::cout << "nMuMuEndcapRSBCat2 = " << minnMuMuEndcapRSBCat2 << " ... " << maxnMuMuEndcapRSBCat2 << std::endl;
  std::cout << "nMuHEndcapRSBCat2 = " << minnMuHEndcapRSBCat2 << " ... " << maxnMuHEndcapRSBCat2 << std::endl;
  std::cout << "nHHEndcapRSBCat2 = " << minnHHEndcapRSBCat2 << " ... " << maxnHHEndcapRSBCat2 << std::endl;
  std::cout << "nMuMuMVAEndcapRSBCat2 = " << minnMuMuMVAEndcapRSBCat2 << " ... " << maxnMuMuMVAEndcapRSBCat2 << std::endl;
  std::cout << "nMuHMVAEndcapRSBCat2 = " << minnMuHMVAEndcapRSBCat2 << " ... " << maxnMuHMVAEndcapRSBCat2 << std::endl;
  std::cout << "nHHMVAEndcapRSBCat2 = " << minnHHMVAEndcapRSBCat2 << " ... " << maxnHHMVAEndcapRSBCat2 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelAllCat3 = " << minN0BarrelAllCat3 << " ... " << maxN0BarrelAllCat3 << std::endl;
  std::cout << "N1BarrelAllCat3 = " << minN1BarrelAllCat3 << " ... " << maxN1BarrelAllCat3  << std::endl;
  std::cout << "N2BarrelAllCat3 = " << minN2BarrelAllCat3 << " ... " << maxN2BarrelAllCat3  << std::endl;
  
  std::cout << "nMuMuBarrelAllCat3 = " << minnMuMuBarrelAllCat3 << " ... " << maxnMuMuBarrelAllCat3 << std::endl;
  std::cout << "nMuHBarrelAllCat3 = " << minnMuHBarrelAllCat3 << " ... " << maxnMuHBarrelAllCat3 << std::endl;
  std::cout << "nHHBarrelAllCat3 = " << minnHHBarrelAllCat3 << " ... " << maxnHHBarrelAllCat3 << std::endl;
  std::cout << "nMuMuMVABarrelAllCat3 = " << minnMuMuMVABarrelAllCat3 << " ... " << maxnMuMuMVABarrelAllCat3 << std::endl;
  std::cout << "nMuHMVABarrelAllCat3 = " << minnMuHMVABarrelAllCat3 << " ... " << maxnMuHMVABarrelAllCat3 << std::endl;
  std::cout << "nHHMVABarrelAllCat3 = " << minnHHMVABarrelAllCat3 << " ... " << maxnHHMVABarrelAllCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelLSBCat3 = " << minN0BarrelLSBCat3 << " ... " << maxN0BarrelLSBCat3 << std::endl;
  std::cout << "N1BarrelLSBCat3 = " << minN1BarrelLSBCat3 << " ... " << maxN1BarrelLSBCat3  << std::endl;
  std::cout << "N2BarrelLSBCat3 = " << minN2BarrelLSBCat3 << " ... " << maxN2BarrelLSBCat3  << std::endl;
  
  std::cout << "nMuMuBarrelLSBCat3 = " << minnMuMuBarrelLSBCat3 << " ... " << maxnMuMuBarrelLSBCat3 << std::endl;
  std::cout << "nMuHBarrelLSBCat3 = " << minnMuHBarrelLSBCat3 << " ... " << maxnMuHBarrelLSBCat3 << std::endl;
  std::cout << "nHHBarrelLSBCat3 = " << minnHHBarrelLSBCat3 << " ... " << maxnHHBarrelLSBCat3 << std::endl;
  std::cout << "nMuMuMVABarrelLSBCat3 = " << minnMuMuMVABarrelLSBCat3 << " ... " << maxnMuMuMVABarrelLSBCat3 << std::endl;
  std::cout << "nMuHMVABarrelLSBCat3 = " << minnMuHMVABarrelLSBCat3 << " ... " << maxnMuHMVABarrelLSBCat3 << std::endl;
  std::cout << "nHHMVABarrelLSBCat3 = " << minnHHMVABarrelLSBCat3 << " ... " << maxnHHMVABarrelLSBCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelSigCat3 = " << minN0BarrelSigCat3 << " ... " << maxN0BarrelSigCat3 << std::endl;
  std::cout << "N1BarrelSigCat3 = " << minN1BarrelSigCat3 << " ... " << maxN1BarrelSigCat3  << std::endl;
  std::cout << "N2BarrelSigCat3 = " << minN2BarrelSigCat3 << " ... " << maxN2BarrelSigCat3  << std::endl;
  
  std::cout << "nMuMuBarrelSigCat3 = " << minnMuMuBarrelSigCat3 << " ... " << maxnMuMuBarrelSigCat3 << std::endl;
  std::cout << "nMuHBarrelSigCat3 = " << minnMuHBarrelSigCat3 << " ... " << maxnMuHBarrelSigCat3 << std::endl;
  std::cout << "nHHBarrelSigCat3 = " << minnHHBarrelSigCat3 << " ... " << maxnHHBarrelSigCat3 << std::endl;
  std::cout << "nMuMuMVABarrelSigCat3 = " << minnMuMuMVABarrelSigCat3 << " ... " << maxnMuMuMVABarrelSigCat3 << std::endl;
  std::cout << "nMuHMVABarrelSigCat3 = " << minnMuHMVABarrelSigCat3 << " ... " << maxnMuHMVABarrelSigCat3 << std::endl;
  std::cout << "nHHMVABarrelSigCat3 = " << minnHHMVABarrelSigCat3 << " ... " << maxnHHMVABarrelSigCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelRSBCat3 = " << minN0BarrelRSBCat3 << " ... " << maxN0BarrelRSBCat3 << std::endl;
  std::cout << "N1BarrelRSBCat3 = " << minN1BarrelRSBCat3 << " ... " << maxN1BarrelRSBCat3  << std::endl;
  std::cout << "N2BarrelRSBCat3 = " << minN2BarrelRSBCat3 << " ... " << maxN2BarrelRSBCat3  << std::endl;
  
  std::cout << "nMuMuBarrelRSBCat3 = " << minnMuMuBarrelRSBCat3 << " ... " << maxnMuMuBarrelRSBCat3 << std::endl;
  std::cout << "nMuHBarrelRSBCat3 = " << minnMuHBarrelRSBCat3 << " ... " << maxnMuHBarrelRSBCat3 << std::endl;
  std::cout << "nHHBarrelRSBCat3 = " << minnHHBarrelRSBCat3 << " ... " << maxnHHBarrelRSBCat3 << std::endl;
  std::cout << "nMuMuMVABarrelRSBCat3 = " << minnMuMuMVABarrelRSBCat3 << " ... " << maxnMuMuMVABarrelRSBCat3 << std::endl;
  std::cout << "nMuHMVABarrelRSBCat3 = " << minnMuHMVABarrelRSBCat3 << " ... " << maxnMuHMVABarrelRSBCat3 << std::endl;
  std::cout << "nHHMVABarrelRSBCat3 = " << minnHHMVABarrelRSBCat3 << " ... " << maxnHHMVABarrelRSBCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapAllCat3 = " << minN0EndcapAllCat3 << " ... " << maxN0EndcapAllCat3 << std::endl;
  std::cout << "N1EndcapAllCat3 = " << minN1EndcapAllCat3 << " ... " << maxN1EndcapAllCat3  << std::endl;
  std::cout << "N2EndcapAllCat3 = " << minN2EndcapAllCat3 << " ... " << maxN2EndcapAllCat3  << std::endl;
  
  std::cout << "nMuMuEndcapAllCat3 = " << minnMuMuEndcapAllCat3 << " ... " << maxnMuMuEndcapAllCat3 << std::endl;
  std::cout << "nMuHEndcapAllCat3 = " << minnMuHEndcapAllCat3 << " ... " << maxnMuHEndcapAllCat3 << std::endl;
  std::cout << "nHHEndcapAllCat3 = " << minnHHEndcapAllCat3 << " ... " << maxnHHEndcapAllCat3 << std::endl;
  std::cout << "nMuMuMVAEndcapAllCat3 = " << minnMuMuMVAEndcapAllCat3 << " ... " << maxnMuMuMVAEndcapAllCat3 << std::endl;
  std::cout << "nMuHMVAEndcapAllCat3 = " << minnMuHMVAEndcapAllCat3 << " ... " << maxnMuHMVAEndcapAllCat3 << std::endl;
  std::cout << "nHHMVAEndcapAllCat3 = " << minnHHMVAEndcapAllCat3 << " ... " << maxnHHMVAEndcapAllCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapLSBCat3 = " << minN0EndcapLSBCat3 << " ... " << maxN0EndcapLSBCat3 << std::endl;
  std::cout << "N1EndcapLSBCat3 = " << minN1EndcapLSBCat3 << " ... " << maxN1EndcapLSBCat3  << std::endl;
  std::cout << "N2EndcapLSBCat3 = " << minN2EndcapLSBCat3 << " ... " << maxN2EndcapLSBCat3  << std::endl;
  
  std::cout << "nMuMuEndcapLSBCat3 = " << minnMuMuEndcapLSBCat3 << " ... " << maxnMuMuEndcapLSBCat3 << std::endl;
  std::cout << "nMuHEndcapLSBCat3 = " << minnMuHEndcapLSBCat3 << " ... " << maxnMuHEndcapLSBCat3 << std::endl;
  std::cout << "nHHEndcapLSBCat3 = " << minnHHEndcapLSBCat3 << " ... " << maxnHHEndcapLSBCat3 << std::endl;
  std::cout << "nMuMuMVAEndcapLSBCat3 = " << minnMuMuMVAEndcapLSBCat3 << " ... " << maxnMuMuMVAEndcapLSBCat3 << std::endl;
  std::cout << "nMuHMVAEndcapLSBCat3 = " << minnMuHMVAEndcapLSBCat3 << " ... " << maxnMuHMVAEndcapLSBCat3 << std::endl;
  std::cout << "nHHMVAEndcapLSBCat3 = " << minnHHMVAEndcapLSBCat3 << " ... " << maxnHHMVAEndcapLSBCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapSigCat3 = " << minN0EndcapSigCat3 << " ... " << maxN0EndcapSigCat3 << std::endl;
  std::cout << "N1EndcapSigCat3 = " << minN1EndcapSigCat3 << " ... " << maxN1EndcapSigCat3  << std::endl;
  std::cout << "N2EndcapSigCat3 = " << minN2EndcapSigCat3 << " ... " << maxN2EndcapSigCat3  << std::endl;
  
  std::cout << "nMuMuEndcapSigCat3 = " << minnMuMuEndcapSigCat3 << " ... " << maxnMuMuEndcapSigCat3 << std::endl;
  std::cout << "nMuHEndcapSigCat3 = " << minnMuHEndcapSigCat3 << " ... " << maxnMuHEndcapSigCat3 << std::endl;
  std::cout << "nHHEndcapSigCat3 = " << minnHHEndcapSigCat3 << " ... " << maxnHHEndcapSigCat3 << std::endl;
  std::cout << "nMuMuMVAEndcapSigCat3 = " << minnMuMuMVAEndcapSigCat3 << " ... " << maxnMuMuMVAEndcapSigCat3 << std::endl;
  std::cout << "nMuHMVAEndcapSigCat3 = " << minnMuHMVAEndcapSigCat3 << " ... " << maxnMuHMVAEndcapSigCat3 << std::endl;
  std::cout << "nHHMVAEndcapSigCat3 = " << minnHHMVAEndcapSigCat3 << " ... " << maxnHHMVAEndcapSigCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapRSBCat3 = " << minN0EndcapRSBCat3 << " ... " << maxN0EndcapRSBCat3 << std::endl;
  std::cout << "N1EndcapRSBCat3 = " << minN1EndcapRSBCat3 << " ... " << maxN1EndcapRSBCat3  << std::endl;
  std::cout << "N2EndcapRSBCat3 = " << minN2EndcapRSBCat3 << " ... " << maxN2EndcapRSBCat3  << std::endl;
  
  std::cout << "nMuMuEndcapRSBCat3 = " << minnMuMuEndcapRSBCat3 << " ... " << maxnMuMuEndcapRSBCat3 << std::endl;
  std::cout << "nMuHEndcapRSBCat3 = " << minnMuHEndcapRSBCat3 << " ... " << maxnMuHEndcapRSBCat3 << std::endl;
  std::cout << "nHHEndcapRSBCat3 = " << minnHHEndcapRSBCat3 << " ... " << maxnHHEndcapRSBCat3 << std::endl;
  std::cout << "nMuMuMVAEndcapRSBCat3 = " << minnMuMuMVAEndcapRSBCat3 << " ... " << maxnMuMuMVAEndcapRSBCat3 << std::endl;
  std::cout << "nMuHMVAEndcapRSBCat3 = " << minnMuHMVAEndcapRSBCat3 << " ... " << maxnMuHMVAEndcapRSBCat3 << std::endl;
  std::cout << "nHHMVAEndcapRSBCat3 = " << minnHHMVAEndcapRSBCat3 << " ... " << maxnHHMVAEndcapRSBCat3 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelAllCat4 = " << minN0BarrelAllCat4 << " ... " << maxN0BarrelAllCat4 << std::endl;
  std::cout << "N1BarrelAllCat4 = " << minN1BarrelAllCat4 << " ... " << maxN1BarrelAllCat4  << std::endl;
  std::cout << "N2BarrelAllCat4 = " << minN2BarrelAllCat4 << " ... " << maxN2BarrelAllCat4  << std::endl;
  
  std::cout << "nMuMuBarrelAllCat4 = " << minnMuMuBarrelAllCat4 << " ... " << maxnMuMuBarrelAllCat4 << std::endl;
  std::cout << "nMuHBarrelAllCat4 = " << minnMuHBarrelAllCat4 << " ... " << maxnMuHBarrelAllCat4 << std::endl;
  std::cout << "nHHBarrelAllCat4 = " << minnHHBarrelAllCat4 << " ... " << maxnHHBarrelAllCat4 << std::endl;
  std::cout << "nMuMuMVABarrelAllCat4 = " << minnMuMuMVABarrelAllCat4 << " ... " << maxnMuMuMVABarrelAllCat4 << std::endl;
  std::cout << "nMuHMVABarrelAllCat4 = " << minnMuHMVABarrelAllCat4 << " ... " << maxnMuHMVABarrelAllCat4 << std::endl;
  std::cout << "nHHMVABarrelAllCat4 = " << minnHHMVABarrelAllCat4 << " ... " << maxnHHMVABarrelAllCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelLSBCat4 = " << minN0BarrelLSBCat4 << " ... " << maxN0BarrelLSBCat4 << std::endl;
  std::cout << "N1BarrelLSBCat4 = " << minN1BarrelLSBCat4 << " ... " << maxN1BarrelLSBCat4  << std::endl;
  std::cout << "N2BarrelLSBCat4 = " << minN2BarrelLSBCat4 << " ... " << maxN2BarrelLSBCat4  << std::endl;
  
  std::cout << "nMuMuBarrelLSBCat4 = " << minnMuMuBarrelLSBCat4 << " ... " << maxnMuMuBarrelLSBCat4 << std::endl;
  std::cout << "nMuHBarrelLSBCat4 = " << minnMuHBarrelLSBCat4 << " ... " << maxnMuHBarrelLSBCat4 << std::endl;
  std::cout << "nHHBarrelLSBCat4 = " << minnHHBarrelLSBCat4 << " ... " << maxnHHBarrelLSBCat4 << std::endl;
  std::cout << "nMuMuMVABarrelLSBCat4 = " << minnMuMuMVABarrelLSBCat4 << " ... " << maxnMuMuMVABarrelLSBCat4 << std::endl;
  std::cout << "nMuHMVABarrelLSBCat4 = " << minnMuHMVABarrelLSBCat4 << " ... " << maxnMuHMVABarrelLSBCat4 << std::endl;
  std::cout << "nHHMVABarrelLSBCat4 = " << minnHHMVABarrelLSBCat4 << " ... " << maxnHHMVABarrelLSBCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelSigCat4 = " << minN0BarrelSigCat4 << " ... " << maxN0BarrelSigCat4 << std::endl;
  std::cout << "N1BarrelSigCat4 = " << minN1BarrelSigCat4 << " ... " << maxN1BarrelSigCat4  << std::endl;
  std::cout << "N2BarrelSigCat4 = " << minN2BarrelSigCat4 << " ... " << maxN2BarrelSigCat4  << std::endl;
  
  std::cout << "nMuMuBarrelSigCat4 = " << minnMuMuBarrelSigCat4 << " ... " << maxnMuMuBarrelSigCat4 << std::endl;
  std::cout << "nMuHBarrelSigCat4 = " << minnMuHBarrelSigCat4 << " ... " << maxnMuHBarrelSigCat4 << std::endl;
  std::cout << "nHHBarrelSigCat4 = " << minnHHBarrelSigCat4 << " ... " << maxnHHBarrelSigCat4 << std::endl;
  std::cout << "nMuMuMVABarrelSigCat4 = " << minnMuMuMVABarrelSigCat4 << " ... " << maxnMuMuMVABarrelSigCat4 << std::endl;
  std::cout << "nMuHMVABarrelSigCat4 = " << minnMuHMVABarrelSigCat4 << " ... " << maxnMuHMVABarrelSigCat4 << std::endl;
  std::cout << "nHHMVABarrelSigCat4 = " << minnHHMVABarrelSigCat4 << " ... " << maxnHHMVABarrelSigCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0BarrelRSBCat4 = " << minN0BarrelRSBCat4 << " ... " << maxN0BarrelRSBCat4 << std::endl;
  std::cout << "N1BarrelRSBCat4 = " << minN1BarrelRSBCat4 << " ... " << maxN1BarrelRSBCat4  << std::endl;
  std::cout << "N2BarrelRSBCat4 = " << minN2BarrelRSBCat4 << " ... " << maxN2BarrelRSBCat4  << std::endl;
  
  std::cout << "nMuMuBarrelRSBCat4 = " << minnMuMuBarrelRSBCat4 << " ... " << maxnMuMuBarrelRSBCat4 << std::endl;
  std::cout << "nMuHBarrelRSBCat4 = " << minnMuHBarrelRSBCat4 << " ... " << maxnMuHBarrelRSBCat4 << std::endl;
  std::cout << "nHHBarrelRSBCat4 = " << minnHHBarrelRSBCat4 << " ... " << maxnHHBarrelRSBCat4 << std::endl;
  std::cout << "nMuMuMVABarrelRSBCat4 = " << minnMuMuMVABarrelRSBCat4 << " ... " << maxnMuMuMVABarrelRSBCat4 << std::endl;
  std::cout << "nMuHMVABarrelRSBCat4 = " << minnMuHMVABarrelRSBCat4 << " ... " << maxnMuHMVABarrelRSBCat4 << std::endl;
  std::cout << "nHHMVABarrelRSBCat4 = " << minnHHMVABarrelRSBCat4 << " ... " << maxnHHMVABarrelRSBCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapAllCat4 = " << minN0EndcapAllCat4 << " ... " << maxN0EndcapAllCat4 << std::endl;
  std::cout << "N1EndcapAllCat4 = " << minN1EndcapAllCat4 << " ... " << maxN1EndcapAllCat4  << std::endl;
  std::cout << "N2EndcapAllCat4 = " << minN2EndcapAllCat4 << " ... " << maxN2EndcapAllCat4  << std::endl;
  
  std::cout << "nMuMuEndcapAllCat4 = " << minnMuMuEndcapAllCat4 << " ... " << maxnMuMuEndcapAllCat4 << std::endl;
  std::cout << "nMuHEndcapAllCat4 = " << minnMuHEndcapAllCat4 << " ... " << maxnMuHEndcapAllCat4 << std::endl;
  std::cout << "nHHEndcapAllCat4 = " << minnHHEndcapAllCat4 << " ... " << maxnHHEndcapAllCat4 << std::endl;
  std::cout << "nMuMuMVAEndcapAllCat4 = " << minnMuMuMVAEndcapAllCat4 << " ... " << maxnMuMuMVAEndcapAllCat4 << std::endl;
  std::cout << "nMuHMVAEndcapAllCat4 = " << minnMuHMVAEndcapAllCat4 << " ... " << maxnMuHMVAEndcapAllCat4 << std::endl;
  std::cout << "nHHMVAEndcapAllCat4 = " << minnHHMVAEndcapAllCat4 << " ... " << maxnHHMVAEndcapAllCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapLSBCat4 = " << minN0EndcapLSBCat4 << " ... " << maxN0EndcapLSBCat4 << std::endl;
  std::cout << "N1EndcapLSBCat4 = " << minN1EndcapLSBCat4 << " ... " << maxN1EndcapLSBCat4  << std::endl;
  std::cout << "N2EndcapLSBCat4 = " << minN2EndcapLSBCat4 << " ... " << maxN2EndcapLSBCat4  << std::endl;
  
  std::cout << "nMuMuEndcapLSBCat4 = " << minnMuMuEndcapLSBCat4 << " ... " << maxnMuMuEndcapLSBCat4 << std::endl;
  std::cout << "nMuHEndcapLSBCat4 = " << minnMuHEndcapLSBCat4 << " ... " << maxnMuHEndcapLSBCat4 << std::endl;
  std::cout << "nHHEndcapLSBCat4 = " << minnHHEndcapLSBCat4 << " ... " << maxnHHEndcapLSBCat4 << std::endl;
  std::cout << "nMuMuMVAEndcapLSBCat4 = " << minnMuMuMVAEndcapLSBCat4 << " ... " << maxnMuMuMVAEndcapLSBCat4 << std::endl;
  std::cout << "nMuHMVAEndcapLSBCat4 = " << minnMuHMVAEndcapLSBCat4 << " ... " << maxnMuHMVAEndcapLSBCat4 << std::endl;
  std::cout << "nHHMVAEndcapLSBCat4 = " << minnHHMVAEndcapLSBCat4 << " ... " << maxnHHMVAEndcapLSBCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapSigCat4 = " << minN0EndcapSigCat4 << " ... " << maxN0EndcapSigCat4 << std::endl;
  std::cout << "N1EndcapSigCat4 = " << minN1EndcapSigCat4 << " ... " << maxN1EndcapSigCat4  << std::endl;
  std::cout << "N2EndcapSigCat4 = " << minN2EndcapSigCat4 << " ... " << maxN2EndcapSigCat4  << std::endl;
  
  std::cout << "nMuMuEndcapSigCat4 = " << minnMuMuEndcapSigCat4 << " ... " << maxnMuMuEndcapSigCat4 << std::endl;
  std::cout << "nMuHEndcapSigCat4 = " << minnMuHEndcapSigCat4 << " ... " << maxnMuHEndcapSigCat4 << std::endl;
  std::cout << "nHHEndcapSigCat4 = " << minnHHEndcapSigCat4 << " ... " << maxnHHEndcapSigCat4 << std::endl;
  std::cout << "nMuMuMVAEndcapSigCat4 = " << minnMuMuMVAEndcapSigCat4 << " ... " << maxnMuMuMVAEndcapSigCat4 << std::endl;
  std::cout << "nMuHMVAEndcapSigCat4 = " << minnMuHMVAEndcapSigCat4 << " ... " << maxnMuHMVAEndcapSigCat4 << std::endl;
  std::cout << "nHHMVAEndcapSigCat4 = " << minnHHMVAEndcapSigCat4 << " ... " << maxnHHMVAEndcapSigCat4 << std::endl;
  
  std::cout << "\n";
  std::cout << "N0EndcapRSBCat4 = " << minN0EndcapRSBCat4 << " ... " << maxN0EndcapRSBCat4 << std::endl;
  std::cout << "N1EndcapRSBCat4 = " << minN1EndcapRSBCat4 << " ... " << maxN1EndcapRSBCat4  << std::endl;
  std::cout << "N2EndcapRSBCat4 = " << minN2EndcapRSBCat4 << " ... " << maxN2EndcapRSBCat4  << std::endl;
  
  std::cout << "nMuMuEndcapRSBCat4 = " << minnMuMuEndcapRSBCat4 << " ... " << maxnMuMuEndcapRSBCat4 << std::endl;
  std::cout << "nMuHEndcapRSBCat4 = " << minnMuHEndcapRSBCat4 << " ... " << maxnMuHEndcapRSBCat4 << std::endl;
  std::cout << "nHHEndcapRSBCat4 = " << minnHHEndcapRSBCat4 << " ... " << maxnHHEndcapRSBCat4 << std::endl;
  std::cout << "nMuMuMVAEndcapRSBCat4 = " << minnMuMuMVAEndcapRSBCat4 << " ... " << maxnMuMuMVAEndcapRSBCat4 << std::endl;
  std::cout << "nMuHMVAEndcapRSBCat4 = " << minnMuHMVAEndcapRSBCat4 << " ... " << maxnMuHMVAEndcapRSBCat4 << std::endl;
  std::cout << "nHHMVAEndcapRSBCat4 = " << minnHHMVAEndcapRSBCat4 << " ... " << maxnHHMVAEndcapRSBCat4 << std::endl;
  
  outfile->cd();
  
  CreateAndWriteCanvas("cMassBarrel",0,21,1,false,false,hMassBarrel);
  CreateAndWriteCanvas("cMassEndcap",0,21,1,false,false,hMassEndcap);
  CreateAndWriteCanvas("cMassMVABarrel",0,21,1,false,false,hMassMVABarrel);
  CreateAndWriteCanvas("cMassMVAEndcap",0,21,1,false,false,hMassMVAEndcap);
  CreateAndWriteCanvas("cMassFail1Barrel",0,21,1,false,false,hMassFail1Barrel);
  CreateAndWriteCanvas("cMassFail1Endcap",0,21,1,false,false,hMassFail1Endcap);
  CreateAndWriteCanvas("cMassFail2Barrel",0,21,1,false,false,hMassFail2Barrel);
  CreateAndWriteCanvas("cMassFail2Endcap",0,21,1,false,false,hMassFail2Endcap);
  
  CreateAndWriteCanvas("cMassBarrelCat1",0,21,1,false,false,hMassBarrelCat1);
  CreateAndWriteCanvas("cMassEndcapCat1",0,21,1,false,false,hMassEndcapCat1);
  CreateAndWriteCanvas("cMassMVABarrelCat1",0,21,1,false,false,hMassMVABarrelCat1);
  CreateAndWriteCanvas("cMassMVAEndcapCat1",0,21,1,false,false,hMassMVAEndcapCat1);
  CreateAndWriteCanvas("cMassFail1BarrelCat1",0,21,1,false,false,hMassFail1BarrelCat1);
  CreateAndWriteCanvas("cMassFail1EndcapCat1",0,21,1,false,false,hMassFail1EndcapCat1);
  CreateAndWriteCanvas("cMassFail2BarrelCat1",0,21,1,false,false,hMassFail2BarrelCat1);
  CreateAndWriteCanvas("cMassFail2EndcapCat1",0,21,1,false,false,hMassFail2EndcapCat1);
  
  CreateAndWriteCanvas("cMassBarrelCat2",0,21,1,false,false,hMassBarrelCat2);
  CreateAndWriteCanvas("cMassEndcapCat2",0,21,1,false,false,hMassEndcapCat2);
  CreateAndWriteCanvas("cMassMVABarrelCat2",0,21,1,false,false,hMassMVABarrelCat2);
  CreateAndWriteCanvas("cMassMVAEndcapCat2",0,21,1,false,false,hMassMVAEndcapCat2);
  CreateAndWriteCanvas("cMassFail1BarrelCat2",0,21,1,false,false,hMassFail1BarrelCat2);
  CreateAndWriteCanvas("cMassFail1EndcapCat2",0,21,1,false,false,hMassFail1EndcapCat2);
  CreateAndWriteCanvas("cMassFail2BarrelCat2",0,21,1,false,false,hMassFail2BarrelCat2);
  CreateAndWriteCanvas("cMassFail2EndcapCat2",0,21,1,false,false,hMassFail2EndcapCat2);
  
  CreateAndWriteCanvas("cMassBarrelCat3",0,21,1,false,false,hMassBarrelCat3);
  CreateAndWriteCanvas("cMassEndcapCat3",0,21,1,false,false,hMassEndcapCat3);
  CreateAndWriteCanvas("cMassMVABarrelCat3",0,21,1,false,false,hMassMVABarrelCat3);
  CreateAndWriteCanvas("cMassMVAEndcapCat3",0,21,1,false,false,hMassMVAEndcapCat3);
  CreateAndWriteCanvas("cMassFail1BarrelCat3",0,21,1,false,false,hMassFail1BarrelCat3);
  CreateAndWriteCanvas("cMassFail1EndcapCat3",0,21,1,false,false,hMassFail1EndcapCat3);
  CreateAndWriteCanvas("cMassFail2BarrelCat3",0,21,1,false,false,hMassFail2BarrelCat3);
  CreateAndWriteCanvas("cMassFail2EndcapCat3",0,21,1,false,false,hMassFail2EndcapCat3);
  
  CreateAndWriteCanvas("cMassBarrelCat4",0,21,1,false,false,hMassBarrelCat4);
  CreateAndWriteCanvas("cMassEndcapCat4",0,21,1,false,false,hMassEndcapCat4);
  CreateAndWriteCanvas("cMassMVABarrelCat4",0,21,1,false,false,hMassMVABarrelCat4);
  CreateAndWriteCanvas("cMassMVAEndcapCat4",0,21,1,false,false,hMassMVAEndcapCat4);
  CreateAndWriteCanvas("cMassFail1BarrelCat4",0,21,1,false,false,hMassFail1BarrelCat4);
  CreateAndWriteCanvas("cMassFail1EndcapCat4",0,21,1,false,false,hMassFail1EndcapCat4);
  CreateAndWriteCanvas("cMassFail2BarrelCat4",0,21,1,false,false,hMassFail2BarrelCat4);
  CreateAndWriteCanvas("cMassFail2EndcapCat4",0,21,1,false,false,hMassFail2EndcapCat4);
  
  outfile->Write();
  outfile->Close();
}
