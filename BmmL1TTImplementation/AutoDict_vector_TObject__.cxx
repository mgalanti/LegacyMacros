#include "vector"
class TObject;
#ifdef __CINT__ 
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ class vector<TObject*>+;
#pragma link C++ class vector<TObject*>::*;
#ifdef G__VECTOR_HAS_CLASS_ITERATOR
#pragma link C++ operators vector<TObject*>::iterator;
#pragma link C++ operators vector<TObject*>::const_iterator;
#pragma link C++ operators vector<TObject*>::reverse_iterator;
#endif
#endif
