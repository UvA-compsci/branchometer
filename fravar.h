#ifndef FRAVAR_H
#define FRAVAR_H

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
/* DEFINITION EXTERNALS / FUNCTIONS                          */
/*************************************************************/


#include "fratype.h"

/* IMAGE VARIABLES                                    */
/*                                                    */
/* a) imax and jmax are the dimensions                */
/* of all images:                                     */
        int 		im_in_imax;
        int 		im_in_jmax;
/* b) The input image as read from the frain.pbm file */
     int 		im_in[MAX_ISIZE][MAX_ISIZE];
/* c) The skeleton image created with thin_b:         */
     int 		im_skel[MAX_ISIZE][MAX_ISIZE];
/* d) The junction image created with find_junctions: */
     int 		im_junc[MAX_ISIZE][MAX_ISIZE];
/* e) The outline image created with contour_object: */
    int             im_contour[MAX_ISIZE][MAX_ISIZE];
/* addition 3-9-97 measuring branch spacing */
/* f) scratch image for displaying branch spacing measurements */
    int             scratch[MAX_ISIZE][MAX_ISIZE];
/* Modification 18 Oct 02 */
/* g) The term image created with term_junctions: */
     int             im_term[MAX_ISIZE][MAX_ISIZE];


/* THE ARRAYS WITH RESULTS                            */
char            title[STRLENGTH];
float            length_object;
    Real            mult_factor;
    Medge		da_discs[MAX_RES];
    int             n_da_discs;
    Medge		db_discs[MAX_RES];
    int             n_db_discs;
    Medge		dc_discs[MAX_RES];
    int             n_dc_discs;
    Medge		rb[MAX_RES];
    int             n_rb;
    Medge		geotropy[MAX_RES];
    int             n_geotropy;
    Medge		term_geotropy[MAX_RES];
    int             term_n_geotropy;
    Medge		logeps_box[MAX_RES];
    int             n_logeps_box;
    Medge		logneps_box[MAX_RES];
    int             n_logneps_box;
    Real            box_dimension;
    Medge           angle[MAX_RES];
    int             n_angle;

/* addition 3-9-97 measuring branch spacing */
   Medge           br_spacing[MAX_RES];
   int             n_br_spacing;
/* addition December 2002 leaf angle */
    Medge           langle[MAX_RES];
    int             n_langle;


/* Modification 18 Oct 02 */
extern	int             m_org_x;
extern	int             m_org_y;

 /******************************************************************/
/* end file fravar.h                                              */
/******************************************************************/

 #endif // FRAVAR_H

