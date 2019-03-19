#ifndef CustomContainers_h
#define CustomContainers_h



class HistogramBnnedVector
{
  public:
    void Fill(double value, );
    inline std::vector<TH1> Get() const {return m_vector;};
    
  private:
    std::vector<TH1> m_vector;
    std::vector<double> m_vBinLowerLimits;
};



#endif // CustomContainers_h
