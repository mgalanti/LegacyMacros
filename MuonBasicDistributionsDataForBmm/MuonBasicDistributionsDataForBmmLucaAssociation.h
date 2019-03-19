#ifndef MuonBasicDistributionsDataForBmmLucaAssociation_h
#define MuonBasicDistributionsDataForBmmLucaAssociation_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"
#include "../utilities/EfficiencyFactorsGetter.C"



bool absLessThan(double x, double y) { return (fabs(x)<fabs(y)); }



class MuonBasicDistributionsDataForBmmLucaAssociation : public TreeAnalyzerMC , public EfficiencyFactorsGetter
{
  public:
    MuonBasicDistributionsDataForBmmLucaAssociation(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonBasicDistributionsDataForBmmLucaAssociation", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
    std::string GetBinningVariableString(const std::string sBinning);
    std::vector<double> GetBins(const std::string sBinning);
    std::vector<double> ParseBins(const std::string sBins);
    std::vector<double> GetBinningVariablesValues(const unsigned int iMuon);
    std::vector<double> GetGenBinningVariablesValues(const unsigned int iGenParticle);
    template <typename T> bool from_string(T& t, const std::string& str, std::ios_base& (*f)(std::ios_base&))
    {
      std::istringstream iss(str);
      return !(iss >> f >> t).fail();
    };
    template <typename T, typename U> void Fill(std::vector<T*>* vHistos, const U value, const double binningValue, std::vector<double> vLowBins)
    {
      unsigned int i = 0;
      for(/* */; i < vLowBins.size(); i++)
      {
        if(binningValue < vLowBins[i])
        {
          break;
        }
      }
      vHistos->at(i)->Fill(value);
    }
    template <typename T, typename U> void Fill(std::vector<T*>* vHistos, const U valueX, const U valueY, const double binningValue, std::vector<double> vLowBins)
    {
      unsigned int i = 0;
      for(/* */; i < vLowBins.size(); i++)
      {
        if(binningValue < vLowBins[i])
        {
          break;
        }
      }
      vHistos->at(i)->Fill(valueX, valueY);
    }
    inline void FindAbsMinMaxAvgMedianStddev(std::vector<double>* vect, double& min, double& max, double& avg, double& median, double& stddev)
    {
      avg = 0;
      stddev = 0;
      int size = vect->size();
      for(int i = 0 ; i < size ; i++)
      {
        avg+=fabs(vect->at(i));
      }
      if(size != 0)
      {
        std::sort(vect->begin(), vect->end(), absLessThan);
        min = fabs(*(vect->begin()));
        max = fabs(*(vect->end()-1));
        avg/=size;
        if(size%2 == 0)
        {
          median = (fabs(vect->at(size/2-1)) + fabs(vect->at(size/2))) / 2;
        }
        else 
        {
          median = fabs(vect->at(size/2));
        }
        for(int i = 0 ; i < size ; i++)
        {
          stddev+=(fabs(vect->at(i))-avg)*(fabs(vect->at(i))-avg);
        }
        stddev/=size;
        stddev=sqrt(stddev);
      }
      else
      {
        avg = 9999999;
        stddev = 9999999;
      }
    }
    inline void FindMinMaxAvgMedianStddev(std::vector<double>* vect, double& min, double& max, double& avg, double& median, double& stddev)
    {
      avg = 0;
      stddev = 0;
      int size = vect->size();
      for(int i = 0 ; i < size ; i++)
      {
        avg+=vect->at(i);
      }
      if(size != 0)
      {
        std::sort(vect->begin(), vect->end());
        min = *(vect->begin());
        max = *(vect->end()-1);
        avg/=size;
        if(size%2 == 0)
        {
          median = (vect->at(size/2-1) + vect->at(size/2)) / 2;
        }
        else 
        {
          median = vect->at(size/2);
        }
        for(int i = 0 ; i < size ; i++)
        {
          stddev+=(vect->at(i)-avg)*(vect->at(i)-avg);
        }
        stddev/=size;
        stddev=sqrt(stddev);
      }
      else
      {
        avg = 9999999;
        stddev = 9999999;
      }
    }
};

#endif // MuonBasicDistributionsDataForBmmLucaAssociation_h
