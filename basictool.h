#ifndef BASICTOOL_H
#define BASICTOOL_H

#include "basctype.h"

class BSTool
{
  public:
    BSTool();
    ~BSTool();

    // BSzzz_cosalp2 needs BShypot2
    Real BSzzz_cosalp2(Wc *n,Wc *o,Wc *l);
    // BShypot2 needs BSsqroot
    Real BShypot2(Wc *p1,Wc *p2);
    // BSsqroot needs nothing
    Real BSsqroot(Real z);
    // BSangle needs nothing
    Real BSangle(Real cosalp, Real sinalp);
    // Pixel to mm calculation
};


#endif // BASICTOOL_H
