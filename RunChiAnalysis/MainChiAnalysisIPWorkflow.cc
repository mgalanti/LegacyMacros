#include <iostream>



#include "../ChiAnalysis/ChiAnalysisIPWorkflow.C"



int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    
    ChiAnalysisIPWorkflow* a = new ChiAnalysisIPWorkflow(argv[1]);
    a->RunIPWorkflow();
    return 0;
  }
  else
  {
    std::cout << "This program should be invoked with exactly one command line argument!\n";
    return 1;
  }
}
