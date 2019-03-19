#ifndef DuplicateEventHandler_h
#define DuplicateEventHandler_h

#include <iostream>
#include <vector>
#include <map>

class DuplicateEventHandler 
{
  public:
    DuplicateEventHandler() {
      RunEventMap.clear();
    };
    ~DuplicateEventHandler();

    bool CheckEvent( int RunNo, int EvNo ) {

      /// Find the Run Number in the map
      if ( RunEventMap.find(RunNo) == RunEventMap.end() ) {

        /// If not found, the Run and Event are good
        std::vector<int> eventVec;
        eventVec.clear();
        eventVec.push_back(EvNo);
        RunEventMap.insert( std::pair<int, std::vector<int> >( RunNo, eventVec ) );
        //std::cerr << "Brand New Event " << RunNo << " " << EvNo << std::endl;
        return true;
      }
      else {

        /// If the Run is found, check the events
        std::vector<int> eventVec = RunEventMap.find(RunNo)->second;
        for ( unsigned int j = 0; j < eventVec.size(); j++ ) {
          if ( eventVec.at(j) == EvNo ) {
            //std::cerr << "Skip Duplicate Event " << RunNo << " " << EvNo << std::endl;
            return false;
          }
        }

        /// Here we are ok because of the "return false"
        RunEventMap.find(RunNo)->second.push_back(EvNo);
        //std::cerr << "Brand New Event " << RunNo << " " << EvNo << std::endl;
        return true;
      }

    };

  private:
    std::map<int, std::vector<int> > RunEventMap;
};

#endif // DuplicateEventHandler_h

