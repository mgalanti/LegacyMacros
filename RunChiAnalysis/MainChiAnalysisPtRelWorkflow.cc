#include <iostream>



#include "../ChiAnalysis/ChiAnalysisPtRelWorkflow.C"



int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    
    ChiAnalysisPtRelWorkflow* a = new ChiAnalysisPtRelWorkflow(argv[1]);
    a->RunPtRelWorkflow();
    return 0;
  }
  else
  {
    std::cout << "This program should be invoked with exactly one command line argument!\n";
    return 1;
  }
}
