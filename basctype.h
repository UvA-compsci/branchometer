#ifndef BASCTYPE_H
#define BASCTYPE_H


/*
 * BASIC TYPEDEFS AND MACRO SUBSTITUTIONS
 *
 * COPYRIGHT 1988 - UNIVERSITY OF AMSTERDAM
 *
 * Author: Jaap Kaandorp
 *	   University of Amsterdam
 *         Amsterdam
 *
 */


#include	<stdio.h>

/*****************************************************************/
/* constants */
/*****************************************************************/

/***********************************/
/* start addition hydra April 1994 */
/***********************************/
/* NULL deleted                    */
/***********************************/
/* start addition hydra April 1994 */
/***********************************/
#define	PI		3.141
#define	TOL		0.001
#define	STRLENGTH	100
/*long strings October 1998 */
#define LSTRLENGTH      (2 * STRLENGTH)
#define	FALSE		0
#define	TRUE		1
#define	NWC		4 /* number of wc-coordinates on 1 line */
#define	NWC3		2 /* number of wc3-coordinates on 1 line */
#define NIC		16 /* number of integers (until) 4ch on 1 line */
#define	NRC		8 /* number of reals on 1 line */
#define	NLRC		6 /* number of large reals on 1 line */

/*****************************************************************/
/* constants determining meta file types                         */
/*****************************************************************/
#define	D2_POLYLINE	0 /* *.frc file filled 2D coord; polylines */
#define	D2_FILLAREA	1 /* *.frc file filled 2D coord; fillareas */
#define	D2_DET_FRAC	3 /* *.frc file filled 2D coord; det_frac  */
#define	D3_SURFS	4 /* *.frc file filled 3D coord;           */
#define	TRIANGLE_MODEL	5 /* *.frc file with triangluar network    */
#define	D2_POLYLINEMAT	6 /* *.frc file filled 2D:coord; matrix    */
#define	D2_DLA 		7 /* *.frc file filled dots;     matrix    */
#define	D2_CORAL	8 /* *.frc file drawn in layered style     */
#define	TRNGL_MODEL_R	9 /* *.frc file with triangluar network    */
                          /* reduced version                       */
#define	TRNGL_MODEL92 	10 /* *.frc file with triangluar network   */
               /* version sept-1992                    */
#define	DLA_3D		11 /* *.frc file with the discrete version */
                           /* of the triangluar network            */

#define	DLA_3D_PROJ	12 /* *.frc file with the projection of    */
                           /* the discrete version                 */
                           /* of the triangluar network            */
#define	FLOW_3D		13 /* *.frc file with flow velocites       */
               /* FLOW2 project                        */
#define	P_SUB_2D_DLA	14 /* sub-lattice produced with PARIX appl */
#define	P_2D_DLA	15 /* lattice produced with PARIX appl     */
#define	FLOW_3D_RED	16 /* reduced *.frc file with flow vel.    */
               /* FLOW2 project                        */
#define	P_SUB_3D_DLA	17 /* sub-lattice3 produced with PARIX appl*/
#define	DLA_2D_1024	18 /* lattice2 produced with FRN       appl*/

/*****************************************************************/
/* constants set operations                                      */
/*****************************************************************/
#define			MAX_LSET_NR	2000
#define			MAX_SSET_NR	300
#define			EMPTY		-1



/*****************************************************************/
/* graphics system dependent constants */
/*****************************************************************/
#define	WHITE		1
#define	BACKGROUND	0
#define	DOT		1
#define	SQUARE		2
#define	ASTERISK	3
#define	SOLID		1
#define	DOTTED		3

/* fillarea styles */
#define		GSLSOLID		0
#define		GSLHATCH5		5
#define		GSLHATCH6		6

/* different styles for 3D drawings */
#define		FILLED			0
#define		WIREFRAME		1
#define		PLOTTER_WIREFRAME	2
#define		DEBUG_STYLE		3 /* sort is skipped */
#define		CSHADING		4 /* constant shading */

/* different styles to distinguish dynamical visualization of 3D drawings */
#define		NORMAL_VIS		0 /* no dynamical visualization */
#define		NORMAL_VIS_FAST		1 /* no dynamical visualization */
#define		ROTATING_YAXIS		2 /* rotating about y-axis      */
#define		GROWING			3 /* growing			*/
#define		NORMAL_VIS_OVER		5 /* no dynamical visualization */
                      /* no clear before drawing    */

/*****************************************************************/
/* allocation macros */
/*****************************************************************/
#define	CALLOCJ( R, N, TYPE )\
    if ((R = (TYPE *)calloc( N, sizeof(TYPE))) == NULL){\
          printf( "error in CALLOCJ" );\
    }

/* Change 2-7-00 */
/*#define	FREEJ( R )\*/
    /*if (R != NULL){\*/
      /*free(R);\*/
          /*R = NULL;\*/
    /*}\*/
    /*else{\*/
          /*printf( "error in FREEJ" );\*/
    /*}*/

#define	FREEJ( R )\
      free(R);\
          R = NULL;\

/*****************************************************************/
/* assignment macros */
/*****************************************************************/
#define	ASSW( PTRW1, PTRW2 )\
    {\
    PTRW1->w_x = PTRW2->w_x;\
    PTRW1->w_y = PTRW2->w_y;\
    }

#define	ASSW3( PTRW1, PTRW2 )\
    {\
    PTRW1->w_x = PTRW2->w_x;\
    PTRW1->w_y = PTRW2->w_y;\
    PTRW1->w_z = PTRW2->w_z;\
    }

#define	ASSW2W3_Y( PTRW1, PTRW2 )\
    {\
    PTRW1->w_x = PTRW2->w_x;\
    PTRW1->w_y = PTRW2->w_z;\
    }

#define	ASSV3( V1, V2 )\
    {\
    V1[0] = V2[0];\
    V1[1] = V2[1];\
    V1[2] = V2[2];\
    }

/*****************************************************************/
/* logic operator macros */
/*****************************************************************/

#define	EQUAL( R1, R2 )\
    (( R1 > R2 - TOL) &&\
    ( R1 < R2 + TOL))

#define	EQUAL_WC( WC1, WC2 )\
    ((WC1->w_x > WC2->w_x - TOL) && (WC1->w_x < WC2->w_x + TOL) &&\
    (WC1->w_y > WC2->w_y - TOL) && (WC1->w_y < WC2->w_y + TOL))

#define	EQUAL_WC3( WC1, WC2 )\
    ((WC1->w_x > WC2->w_x - TOL) && (WC1->w_x < WC2->w_x + TOL) &&\
    (WC1->w_y > WC2->w_y - TOL) && (WC1->w_y < WC2->w_y + TOL) &&\
    (WC1->w_z > WC2->w_z - TOL) && (WC1->w_z < WC2->w_z + TOL))

#define	EQUAL_V3( V1, V2 )\
    ((V1[0] > V2[0] - TOL) && (V1[0] < V2[0] + TOL) &&\
    (V1[1] > V2[1] - TOL) && (V1[1] < V2[1] + TOL) &&\
    (V1[2] > V2[2] - TOL) && (V1[2] < V2[2] + TOL))

/*****************************************************************/
/* GTmssge macros */
/*****************************************************************/
#define	MSSR( R )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "R = %f", R);\
    printf( dummy );\
    }

#define	MSSI( I )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "I = %d", I);\
    printf( dummy );\
    }

#define	MSSO( O )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "O = %o", O);\
    printf( dummy );\
    }

#define	MSSH( H )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "H = %x", H);\
    printf( dummy );\
    }

#define	MSSS( S )\
    {\
    char	dummyxxxxxxxx[STRLENGTH];\
    sprintf( dummyxxxxxxxx, "S = %s", S);\
    printf( dummyxxxxxxxx );\
    }

#define	MSSB( B )\
    {\
    char	dummy[STRLENGTH];\
    if (B == TRUE)\
      sprintf( dummy, "B == TRUE");\
    else\
      sprintf( dummy, "B == FALSE");\
    printf( dummy );\
    }


#define MSSWC( WC )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "WC->w_x = %f", WC->w_x);\
    printf( dummy );\
    sprintf( dummy, "WC->w_y = %f", WC->w_y);\
    printf( dummy );\
    }

#define MSSWC3( WC )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "WC->w_x = %f", WC->w_x);\
    printf( dummy );\
    sprintf( dummy, "WC->w_y = %f", WC->w_y);\
    printf( dummy );\
    sprintf( dummy, "WC->w_z = %f", WC->w_z);\
    printf( dummy );\
    }

#define MSSV3( V )\
    {\
    char	dummy[STRLENGTH];\
    sprintf( dummy, "V[0] = %f", V[0]);\
    printf( dummy );\
    sprintf( dummy, "V[1] = %f", V[1]);\
    printf( dummy );\
    sprintf( dummy, "V[2] = %f", V[2]);\
    printf( dummy );\
    }

#define MSSV4( V )\
    {\
    char	dummy[STRLENGTH];\
    int	ix;\
    for (ix = 0; ix < 4; ix++){\
      sprintf( dummy, "V[ix] = %f", V[ix]);\
      printf( dummy );\
    }\
    }

#define MSSMAT44( MAT )\
    {\
    char	dummy[STRLENGTH];\
    int	ix, jx;\
    printf("----------------------");\
    for (ix = 0; ix < 4; ix++){\
      for (jx = 0; jx < 4; jx++){\
        sprintf( dummy, "MAT[ix][jx] = %f", MAT[ix][jx] );\
        printf( dummy );\
          }\
    }\
    }

#define MSSSSET( SSET )\
    {\
    char	dummy[STRLENGTH];\
    int	i;\
    printf("----------------------");\
        sprintf( dummy, "nr of elements of SSET = %d", SSET[0] );\
    printf( dummy );\
    for (i = 1; i <= SSET[0]; i++){\
      sprintf( dummy, "SSET[i] = %d", SSET[i] );\
      printf( dummy );\
        }\
    }


/*****************************************************************/
/* file macros */
/*****************************************************************/
#define	SKIPL( FPTR )\
        {\
        char	dummy[100];\
        fscanf( FPTR, "%s", dummy);\
        }

#define	WR_I_TO_F(FPTR,S)\
        {\
        fprintf( FPTR, "S\n");\
        fprintf( FPTR, "%d\n", S);\
        }

#define	WR_R_TO_F( FPTR, S)\
        {\
        fprintf( FPTR, "S\n");\
        fprintf( FPTR, "%f\n", S);\
        }

/*****************************************************************/
/* function macros */
/*****************************************************************/
#define	SQR(R)\
    (R) * (R)

/*****************************************************************/
/* basic data types */
/*****************************************************************/
typedef	unsigned		Int;

typedef	float			Real;

typedef	unsigned		Bool;

typedef	struct{
    Real	w_x;
    Real	w_y;
    } Wc;

typedef	struct{
    Real	w_x;
    Real	w_y;
    Real	w_z;
    } Wc3;

typedef	struct{
    Int	colour;		/* colour surface		*/
    Wc3	*pts;		/* points surface		*/
    Wc3	*norm;		/* normal vector surface	*/
        /* the next field is allocated in the case the mean nor */
    /* mal vectors of each vertex are necessary             */
    Wc3	*xnorm;
    } Surface;


typedef	Surface	*Surfaceptr;


/* used in BSpolygons, (August 1998) version of BSpolysurf*/
typedef struct{
        Int     colour;         /* colour surface               */
        Int     npts;           /* number vertices polygon      */
        Wc3     *pts;           /* points polygon               */
        Wc3     *norm;          /* normal vector polygon        */
        /* the next field is allocated in the case the mean nor */
        /* mal vectors of each vertex are necessary             */
        Wc3     *xnorm;
        } Polysurface;

/* used in BSpolygons, (August 1998) version of BSpolysurf*/
typedef Polysurface *Polysurfaceptr;

/* datatype String is changed into Stringx for GL interface */
typedef	char			*String;

typedef	FILE			File;

typedef	int	(*Fun)();	/* function pointer */

typedef	Wc	*Wcptr;

typedef	Wc3	*Wc3ptr;

typedef short           *shortptr;

typedef shortptr        *shortptrptr;

typedef shortptrptr     *shortptrptrptr;


typedef	int	*intptr;

typedef intptr  *intptrptr;

typedef	Bool	*Boolptr;

typedef Int	*Intptr;

typedef Intptr	*Intptrptr;

typedef Real	*Realptr;

typedef Realptr	*Realptrptr;

/* datatype String is changed into Stringx for GL interface */
typedef String	*Stringptr;



/*****************************************************************/
/* data structures for lists           */
/*****************************************************************/
/* data structures used by cop_listC() */
typedef	struct	LISTEL {
    char	*ch;
    struct	LISTEL	*next;
    } Listel;

typedef	Listel	*Listelptr;


/* data structures used by cop_listW() */
typedef	struct	LISTWC {
    Wc	wc;
    struct	LISTWC	*next;
    } Listwc;



/* data structures for set operations, note that they are int */
typedef		int	Sset[MAX_SSET_NR]; /* small set */
typedef		int	Lset[MAX_LSET_NR]; /* large set */



/*****************************************************************/
/* values returned by functions in basctools.c                   */
/*****************************************************************/

/*
Real 		BSsqroot();
Real	 	BShypot2();
Real	 	BShypot3();
Real	 	BScurvature();
Real		BSrandRe();
Real	 	BSangle();
Real 		BSzzz_cosalp2();
Real 		BSzzz_cosalp3();
Real 		BSdet_sign();
Bool 		BSodd_int();
Stringptr 	BScop_listC();
Wc 		*BScop_listW();
Real		BSabsx();
*/

/*****************************************************************/
/* values returned by functions in bascmatrix.c                   */
/*****************************************************************/

/*
Real		BSlength_v3();
Real		BSlength_v4();
*/

#endif // BASCTYPE_H
