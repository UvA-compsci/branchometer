#include "basctype.h"
#include "basictool.h"
#include <math.h>
#include <stdio.h>


BSTool::BSTool() { }
BSTool::~BSTool() { }

Real BSTool::BSzzz_cosalp2(Wc *n,Wc *o,Wc *l)
{
  Real		cosln = 0.0;		/* cos angle between l, n	*/
  Real		a;			/* D(l, n)			*/
  Real 		b;			/* D(l, o)			*/
  Real		c;			/* D(n, o)			*/

  a = BShypot2( l, n );
  b = BShypot2( l, o );
  c = BShypot2( n, o );

  if ( (b == 0.0) || (c == 0.0) ){
    //printf("basc: error in BSzzz_cosalp2, b, c wrong");
  }

  if ( a == 0.0 ) cosln = 1.0;
  else if ( a == (b + c) ) cosln = -1.0;
  else if ( (a != 0.0) && (b != 0.0) && (c != 0.0) ){
    cosln = (a * a - b * b - c * c ) /
    (-2.0 * b * c);
  }
  else{
    //printf("basc: error in BSzzz_cosalp2");
  }


  if ( (cosln  >= -1.0 - TOL) && (cosln <= 1.0 + TOL) ){
    return( cosln );
  }
  else{
    return( 0.0 );
  }
} /* BSzzz_cosalp2 */

Real BSTool::BShypot2(Wc *p1,Wc *p2)
{
  Real nside, a, b;

  a = p2->w_x - p1->w_x;
  b = p2->w_y - p1->w_y;

  nside = BSsqroot( a * a + b * b );
  return( nside );
} /* BShypot2 */

Real BSTool::BSsqroot(Real  z)
{
  double  w;

  if (z < 0.0) z = 0.0;

  w = sqrt((double) z);
  return((Real)w);
} /* BSsqroot */

Real BSTool::BSangle(Real cosalp, Real sinalp)
{
   Real		rad;

  if ( cosalp >  1.0 ) cosalp =  1.0;
  if ( cosalp < -1.0 ) cosalp = -1.0;
  rad = (Real)acos( (double)cosalp );
  if (rad < 0.0) rad = -rad;
  /*if (sinalp < 0.0) rad += 0.5 * PI;*/
  return( rad );
} /* BSangle */

