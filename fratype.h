#ifndef FRATYPE_H
#define FRATYPE_H

/*
 * GT (Graphics system independent Tools)
 * BS (Basic toolS)
 *
 * COPYRIGHT 1993 - UNIVERSITY OF AMSTERDAM
 *
 * Author: Jaap Kaandorp
 *	   University of Amsterdam
 *         Amsterdam
 *
 */

/*************************************************************/
/* morphological analysis system (fra)                       */
/* using pbm ascii files as input                            */
/* TYPEDEFS AND MACRO SUBSTITIONS USED IN FRA                */
/*************************************************************/


#include	"basctype.h"


/* maximum size (i and j direction) of an image */
/*#define	MAX_ISIZE	1000*/
#define	MAX_ISIZE	1500

/* Maximum size of the outer circle of the measure disc */
#define MAX_CIRC        2000

/* colour coding */
#define	OBJ_COLOUR	15     /* blue */
#define	SKEL_COLOUR	121    /* red  */
#define	DISC_COLOUR	26     /* yellow-white */
#define	MARK_COLOUR	WHITE  /* white */
#define	MARK2_COLOUR	21

/* size cross */
#define	NORMAL_SIZE	0.01
#define	MEDIUM_SIZE	0.02
#define	LARGE_SIZE	0.04

/* special mark to indicate a site within a disc within image */
#define	DISC_MARK	2
#define	CENTER_MARK	3
#define	CONTOUR_MARK	4
/* special mark to indicate to change nothing in the image */
#define	EMPTY_MARK	100
/* special mark used in collecting enviroinments of a site */
#define	SPECIAL_MARK	1000

/* default maximum length of a skeleton part */
/* note: this size strongly depends on the image type, a maximum */
/* skeleton length which is too small will lead to non-detection */
/* of the next maximum discs a maximum which is too large may    */
/* to loop formation                                             */
#define	MAX_SKEL_LENGTH	300



/* maximum radius of a disc used in the max discs tests          */
#define	MAX_RADIUS	200

/* structure which contains the coord (i,j index) of positions in the */
/* matrix  */
typedef struct{
        int     i_x;
        int     j_y;
        } Ij_pos;

/* structure which contains a measurement and can contain the     */
/* coordinates of one, two or three centers of discs in the       */
/* lattice                                                        */
/* with this structure one maximum disc, an edge connecting the   */
/* centers of two maxmum discs, or an angle defined by three      */
/* maximum discs  can be stored.                                       */

typedef struct{
    Real	meas;
    int	p_i_ct;
    int	p_j_ct;
    int	i_ct;
    int	j_ct;
    int	i1_ct;
    int	j1_ct;
    } Medge;



/* definitions used in the arrays with results */
/* note version 1: terminal geotropy added; 4 fields for the cen- */
/* of the discs on which the measurement is based                 */
#define	OLD_VERSION_NR	0            /* The version nr of fra.res */
#define	VERSION_NR	1            /* The version nr of fra.res */
/* addition 3-9-97 measurements of branch spacing included */
#define	VERSION_NR97	2            /* The version nr of fra.res */
#define	MAX_RES		10000	     /* The maximum nr of results */


/* factors */
/* factor image-sponge to photo-sponge */
/* the image is converted to cm        */
/* based on samples haoc49_0.05 and haoc44B_0.05 */
#define         IM_SP_TO_PH_SP  0.0252
/* based on samples haoc49_0.05 and haoc44B_0.05 and simulations p0.5s13 */
/* p0.5s3 p0.5s5 p0.5s9                                                  */
#define         IM_SIM_TO_IM_SP  0.3576
/* maximum size discs (mean + s_dev) HAOC samples in cm */
#define		MAX_DISC_HAOC	0.70
/* maximum size rb (mean + s_dev) HAOC samples in cm */
#define		MAX_RB_HAOC	2.1
/* maximum size discs (mean + s_dev) MIAL (> PODA) samples in cm */
/*#define		MAX_DISC_MIAL	1.30*/
/* August 96 */
#define		MAX_DISC_MIAL	1.50
/* December 97 */
#define         MAX_DISC_BR_SPACING	3.00
/* maximum size rb (mean + s_dev) PODA (> MIAL) samples in cm */
/*#define		MAX_RB_PODA	3.80*/
#define		MAX_RB_PODA	3.50



/******************************************************************/
/* end file fratype.h                                             */
/******************************************************************/

#endif // FARTYPE_H
