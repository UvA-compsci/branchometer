#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QStack>

#include "fratype.h"
#include "basictool.h"

// maximum size of the environment of a site
#define	MAX_ENV		36


#define NSUBDIV_NRS	57
/* note: slope interval within [0, NSUBDIV_NRS-1]                    */
#define	START_SLOPE	12
#define	END_SLOPE	35

#define COLOR_NO  20  // 14 is now being used.

#define XALIGNMENT -50
#define YALIGNMENT -50
#define XLENGTH 1600
#define YLENGTH 800

#define MAXNOSKEL 5000  // maxinum number of branches between joints
#define MAXPOINT 2000   // maxinum pixels of each branch

#define MAXJUNC 500  // maximum juncion in an image

#define ANGLEATT 25 // angle detail attritube in AngleRec array

#define MYPI 3.14159265
// the bin_list used in fractal dimension calculation
typedef struct{
        int     npts;
        } Box;

// For keeping fixing RB
typedef struct{
        int leafno;
        int juncno;
        } Ij_Leaf;

typedef struct{
        int pno;
        int nno[10];
        } Ij_Info;

typedef struct{
        int F_no;
        int S_no;
        Real val;
        } Ij_Value;

typedef struct{
        int y1; // y of first coord
        int x1; // x of first coord
        int p1; // first point
        int y2; // y of second coord
        int x2; // x of first coord
        int p2; // second point
        int RbN; // rb no
        Real val; // length
        int BrOrNo; // Branch order no
        } Ij_PC;

// end of keeping fixing RB

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    int im_tmp[MAX_ISIZE][MAX_ISIZE];

    int ScreenWidth;
    int ScreenHeight;

    int	max_skel_length;
    Wc	geotropy_axis;
    int image_x;
    int image_y;
    float figure_length_x;
    float figure_length_y;

    const char* firstfilename;
    const char* filenameana;

    QString onlyfilename;

    bool process_finish;

 // variables to display in mainwindow.cpp
    Medge		m_da_discs[MAX_RES];
    int             m_n_da_discs;
    Medge		m_db_discs[MAX_RES];
    int             m_n_db_discs;
    Medge		m_dc_discs[MAX_RES];
    int             m_n_dc_discs;
    Medge		m_rb[MAX_RES];
    int             m_n_rb;
    Medge		m_geotropy[MAX_RES];
    int             m_n_geotropy;
    Medge		m_term_geotropy[MAX_RES];
    int             m_term_n_geotropy;
    Medge		m_logeps_box[MAX_RES];
    int             m_n_logeps_box;
    Medge		m_logneps_box[MAX_RES];
    int             m_n_logneps_box;
    Real            m_box_dimension;
    Medge           m_angle[MAX_RES];
    int             m_n_angle;
    Medge           m_br_spacing[MAX_RES];
    int             m_n_br_spacing;
    Medge           m_langle[MAX_RES];
    int             m_n_langle;


    Medge		prt_da_discs[MAX_RES];
    Medge       prt_db_discs[MAX_RES];
    Medge       prt_dc_discs[MAX_RES];
    Medge       prt_br_spacing[MAX_RES];
    Medge       prt_rb[MAX_RES];
    Medge       prt_urb[MAX_RES];

    // end of variables

    // Branch order detail
    Medge  Branch_Order[10][200]; // [order no.][detail in each order]
    // end of branch order detail

    QString pngpath;
    QString analysispath;
    QString filename_dump_prt;

    bool setScaleFlag;
    bool setFirstClick;
    int firstposx,firstposy, secondposx, secondposy;
    float scaledistancepixel;
    float realscaleinpixel;
    float realscaleinunit;
    QString scaleunit;
    QDialog* pInputDlg;
    QLineEdit* dEdtInputStr;
    QLineEdit* uEdtInputStr;
    QComboBox* UComboBox;

    int junction_amount;
    int terminal_amount;
    int discA_amount;
    int discB_amount;
    int discC_amount;

    int figure_topx;
    int figure_topy;

    QString colorname[COLOR_NO];
    QString colorname_selected,colorname_detected;
    bool first_time;

    QRgb coloratxy;
    int rr;
    int gg;
    int bb;


    int posx;
    int posy;

    int wcx;
    int wcy;

    double pointsize;  // keep the point size
    double MaxPointSize; // keep the max point size (in case users want to increase point size)
    double MinPointSize; // keep the min point size

    float screen_diagonal; // ask for input from users to get their screen diagonal
    float ppi; // ppi of screen ( not 72 as the result of function in qt (dpix)
    float ppmm; // 1 mm = ppmm pixels
    float xratio;
    float yratio;  // ratio between ortho and screen resolution

    int x_pos_start; // position of first x to draw component to put img in the middle of screen
    int y_pos_start; // position of first y to draw component to put img in the middle of screen

    int param; // the parameter to define the size of the group of pixel
    int mark_label; // mark group of pixel no. --for segmentation
    int img_temp[MAX_ISIZE][MAX_ISIZE]; // for segmentation
    // image for termimal which contains only 1 cell for each tip.
    int im_term1[MAX_ISIZE][MAX_ISIZE];
    // for keeping original junction before doing idet_discs
    int im_junc_temp[MAX_ISIZE][MAX_ISIZE];
    int im_junc_reduced[MAX_ISIZE][MAX_ISIZE];

    const char* qstrToch;
    const char* idobj;
    const char* xypos_ch;
    const char* pic_xypos_ch;
    const char* pic_idvalue;
    const char* pic_idvalue1;

    int count_disc_area;
    int disc_area_x[MAXJUNC];
    int disc_area_y[MAXJUNC];

    bool set_show;
    int disc_label;

    int ExactJuncLabel; // keep junc lable which is detected


    int count_maxskel; // No. of Max Skel
    Ij_pos FS[MAXNOSKEL][MAXPOINT]; // coordinates of FS
    int FLength[MAXNOSKEL][6]; // detail of each skel(x1,y1,x2,y2,length,markedlabel)

    // detail of each skel(x1,y1,x2,y2,length,markedlabel) after cutting out
    // the 1 or 2 pixels length in FLength (for recording in .res)
    int FLengthTrim[MAXNOSKEL][6];
    int count_FLengthTrim;

    int AngleRec[MAXJUNC][ANGLEATT];  // keep branch angle [juncno,a,b0,b1,..,bn]
    // sorting coordinates from array AngleRec in the clockwise style
    void sortingCoord();
    int AngleA; // keep junc label in order to keep into AngleRec
    Real AngleResult[MAXJUNC][ANGLEATT]; // keep angle calculation [juncno,no.of angle,cal1,cal2...]
    int ExactJuncAmnt; // exact no. of junction
    // picture area
    int PictureArea; // pixel
    Real PictureAreaMM; // millimeter

    float ContourSize;
    Real ContourSizeMM;

    // keep junction position in an array
    Ij_pos JuncPos[MAXJUNC];

    int	subdiv_nrs[NSUBDIV_NRS];
    //int	*subdiv_nrs;

    BSTool *bstool;

    void init_im(int im[MAX_ISIZE][MAX_ISIZE], int *imax, int *jmax);
    void read_frain(int im[MAX_ISIZE][MAX_ISIZE], int *imax, int *jmax,const char* fname_);
    void readfile(const char* fname_);
    void erase_im(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);
    void rm_spec_m_im(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);
// Thining
    // thin_b needs substr and t1a
    void thin_b(int object[MAX_ISIZE][MAX_ISIZE], int image[MAX_ISIZE][MAX_ISIZE], int nn, int mm);
    void subtr(int a[MAX_ISIZE][MAX_ISIZE], int b[MAX_ISIZE][MAX_ISIZE],int n,int m);
    int t1a(int image[MAX_ISIZE][MAX_ISIZE],int i,int j, int a[8],int *b,int nn,int mm);

// Junctions
    // find_junctions needs init_an_im, collect1_neighbours, collect2_neighbours
    void find_junctions(int im[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE], int imax, int jmax);
    void init_an_im(int	im[MAX_ISIZE][MAX_ISIZE]);
    // collect1_neighbours needs add_site_env
    void collect1_neighbours(int im[MAX_ISIZE][MAX_ISIZE], int i, int j,Ij_pos	env[MAX_ENV],int *n_env);
    void add_site_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env);
    // collect2_neighbours needs collect1_neighbours,test_in_env,add_site_env,rm_site_env,print_env
    void collect2_neighbours(int im[MAX_ISIZE][MAX_ISIZE],int i_org, int j_org,Ij_pos env1[MAX_ENV],int *n_env1,Ij_pos env2[MAX_ENV],int	*n_env2);
    bool test_in_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env);
    void rm_site_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env);
    void print_env(Ij_pos env[MAX_ENV],int *n_env);

// Terminals
    // find_terminals needs init_an_im, collect1_neighbours, collect2_neighbours
    void find_terminals( int im[MAX_ISIZE][MAX_ISIZE], int term[MAX_ISIZE][MAX_ISIZE], int imax, int jmax);

// Contour
    // contour_object needs nothings
    void contour_object(int im[MAX_ISIZE][MAX_ISIZE],int contour[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);

// Disc
    // det_discs2 needs mich_circle, test_disc_too_large
    void det_discs2(int object[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);

    // det_discs needs mich_circle, test_disc_too_large
    void det_discs(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE], int imax, int jmax);
    // mich_circle needs half_circle_points
    void mich_circle(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,int imx, int imy, int radius,Ij_pos circle[MAX_CIRC],int *n_circle, int m_type);
    // half_circle_points needs add_site_circle
    void half_circle_points(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,int imx, int imy, int ix, int iy,Ij_pos circle[MAX_CIRC],int *n_circle, int m_type,bool *error);
    void add_site_circle(int i, int j,Ij_pos circle[MAX_CIRC],int *n_circle);
    bool test_disc_too_large(int  im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,Ij_pos circle[MAX_CIRC],int n_circle);
    bool test_disc_too_large_br(int  im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,Ij_pos circle[MAX_CIRC],int n_circle);

    // revisit_max_disc needs mich_circle and show_circle
    void revisit_max_disc(int skeleton[MAX_ISIZE][MAX_ISIZE], int contour[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, Medge result[MAX_RES], int	*n_result, char label[STRLENGTH]);
    // show_circle needs nothings
    void show_circle(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, Ij_pos circle[MAX_CIRC],int n_circle, int m_type, Int colour,bool erase);

    // clean_image needs show_cross, mich_circle, show_circle
    void clean_image(int object[MAX_ISIZE][MAX_ISIZE],int imax, int jmax, bool exterior);
    void show_cross(int i, int j,int colour,Real move);

    // det_box_dim needs add_result,regres_coef and show_boxes
    void det_box_dim(int contour[MAX_ISIZE][MAX_ISIZE], int imax, int jmax);
    // add_result needs nothing
    void add_result(Real result,Medge res[MAX_RES],int *n_res,int p_i_ct,int p_j_ct,int i_ct,int j_ct,int i1_ct, int j1_ct);
    // show_boxes needs nothing
    void show_boxes(int nbox,Box *box_list,Real size_box,int nsubdiv);
    // regres_coef
    Real regres_coef(int n_val,Real	*x_values,Real *y_values,Real *coefficient);
    // det_max_discs needs mich_circle, test_disc_too_large
    void det_max_discs(int object[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax, int jmax);
    // skel_max_discs needs collect1_neighbours, test_maximum2
    void skel_max_discs(int junc[MAX_ISIZE][MAX_ISIZE],int skel[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);
    // test_maximum2 needs mich_circle,test_maximum_disc
    bool test_maximum2(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int i,int j);
    // test_maximum_disc needs bresenham
    bool test_maximum_disc(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, int centre_i, int centre_j, Ij_pos  circle[MAX_CIRC],int n_circle,int	radius);
    // bresenham needs nothing
    void bresenham(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int ix0,int iy0,int ix1,int iy1,int	centre_value,bool *maximum);

    // idet_discs needs show_cross, merge_imaimb, mich_circle, test_disc_too_large,show_circle
    void idet_discs(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int skel[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);
    void merge_imaimb(int ima[MAX_ISIZE][MAX_ISIZE], int imb[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);

    // ifollow_track needs show_cross,find_center_mark, mich_circle,test_disc_too_large,--
    // --show_circle,test_start_pts, follow_skel,display_skel_part,display_disc,correct2_start_pts,--
    // --add_result,follow_track,sort_angles
    void ifollow_track(int object[MAX_ISIZE][MAX_ISIZE], int skeleton[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE], int scratch[MAX_ISIZE][MAX_ISIZE],int	imax, int jmax);
    //find_center_mark needs add_site_circle, add_neighbours
    void find_center_mark(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax, int i_pos,int j_pos,int *i_ct,int *j_ct,bool *error);
    // add_neightbours needs add_site_circle
    void add_neighbours(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos	pos,int	*n_border,Ij_pos	border[MAX_CIRC]);

    //test_start_pts needs test_neighbours
    void test_start_pts(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos circle[MAX_CIRC],int n_circle,Ij_pos start_pts[MAX_CIRC],int *n_start_pts);
    // test_neighbours needs add_site_circle
    void test_neighbours(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int i,int j,Ij_pos  start_pts[MAX_CIRC],int *n_start_pts);

    //follow_skel needs add_site_circle,add_neigh_skel,find_db_disc,show_db_disc,show_cross,--
    //--mich_circle,show_circle,test_disc_too_large,add_result,cp_ima_to_imb,add_site_env,--
    //--follow_and_rm_skel,test_disc_part_skel,det_geotropy
    void follow_skel(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int	skeleton[MAX_ISIZE][MAX_ISIZE], int scratch[MAX_ISIZE][MAX_ISIZE],int	imax, int jmax,int	i_start,int j_start,Ij_pos	skel_part[MAX_CIRC],int	*n_skel_part,Ij_pos	disc[MAX_CIRC],int	*n_disc,int i_ct,int j_ct,bool *dc_added);

    //add_neigh_skel needs add_site_circle
    void add_neigh_skel(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  pos,int   *n_border,Ij_pos  border[MAX_CIRC],int     *n_disc,Ij_pos  disc[MAX_CIRC]);
    //find_db_disc needs mich_circle,test_db_disc
    void find_db_disc(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int	imax,int jmax,Ij_pos border[MAX_CIRC],int	n_border,int	*i_ct_db,int *j_ct_db,int *max_r,bool *found);
    //test_db_disc needs nothing
    bool test_db_disc(int im[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  circle[MAX_CIRC],int n_circle,bool *found);
    //show_db_disc needs mich_circle,show_circle,test_disc_too_large,add_result,store_angle
    void show_db_disc(int im[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int  i_ct, int j_ct, int ai_ct, int aj_ct);
    //store_angle needs add_result
    void store_angle(int ai_ct, int aj_ct, int b0i_ct, int b0j_ct);
    //cp_ima_to_imb needs nothings
    void cp_ima_to_imb(int ima[MAX_ISIZE][MAX_ISIZE],int imb[MAX_ISIZE][MAX_ISIZE],int imax,int jmax);

    //follow_and_rm_skel needs add_neigh_skel
    void follow_and_rm_skel(int	scratch[MAX_ISIZE][MAX_ISIZE],int	imax,int jmax,int	i_start,int j_start,Ij_pos	skel_part[MAX_CIRC],int	*n_skel_part);
    //test_disc_part_skel needs nothing
    bool test_disc_part_skel(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  circle[MAX_CIRC],int n_circle);

    //display_skel_part needs nothing
    void display_skel_part(Ij_pos skel_part[MAX_CIRC],int	n_skel_part,Int	colour);
    //display_disc needs find_center_mark, show_cross
    void display_disc(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  disc[MAX_CIRC],int     n_disc,Int     colour,int	*i_ct,int *j_ct,bool	*error);
    //correct2_start_pts needs nothing
    void correct2_start_pts(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  start_pts[MAX_CIRC],int     *n_start_pts);

    //follow_track needs det_rb,det_geotropy,add_site_circle,mich_circle,test_disc_too_large,--
    //--show_circle,add_result,test_start_pts,correct2_start_pts,follow_skel,display_skel_part,--
    //--find_center_disc,rm_site_circle
    void follow_track(int object[MAX_ISIZE][MAX_ISIZE],int skeleton[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int scratch[MAX_ISIZE][MAX_ISIZE],int	imax,int jmax,int i_ct,int j_ct,int i_start,int j_start);
    // det_rb needs add_result
    void det_rb(int	prev_i_ct, int prev_j_ct,int i_ct,int j_ct);
    //det_geotropy needs add_result
    void det_geotropy(int	prev_i_ct, int prev_j_ct,int i_ct,int j_ct,Medge result[MAX_RES],int	*n_result);
    // find_center_disc needs find_center_mark, show_cross
    void find_center_disc(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  disc[MAX_CIRC],int n_disc,Int colour,int *i_ct, int *j_ct,bool	*error);

    //sort_angles needs det_angle
    void sort_angles(Medge result[MAX_RES],int  *n_result);
    // det_angle needs nothings
    void det_angle(Real	*angle,int ai_ct,int aj_ct,int b0i_ct,int b0j_ct,int b1i_ct,int b1j_ct);

    //geotropy
    //change_geotropy_axis needs show_geotropy_axis
    void change_geotropy_axis();
    void show_geotropy_axis();

    // change_max_skel_length needs show_max_skel_length
    void change_max_skel_length();
    // show_max_skel_length needs nothings
    void show_max_skel_length();


    void rm_site_circle(int i, int j,Ij_pos circle[MAX_CIRC],int *n_circle);

   // write analytical result to file
    void init_res();
    void dump_res(const char* savedname);
    void dump_prt(const char* savedname);
    void dump_prt_temp(const char* savedname);
    void dump_csv(const char* savedname);

    float findMin( Medge ds[MAX_RES] ,int n);
    float findMax(Medge ds[MAX_RES], int n);
    float findAvg(Medge ds[MAX_RES], int n);
    float findS_dev(Medge ds[MAX_RES], int n);


    void writeAnaToFile(const char* savedname);

    void check_neibor(int inputimg[MAX_ISIZE][MAX_ISIZE],int h, int w, int label);
    int getValue(int inputimg[MAX_ISIZE][MAX_ISIZE],int h, int w);
    int do_segment(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w);
    int count_object(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w);

    void ReducedTerm(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x,int icount);
    void ReducedJunc(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x);

    int compareColor(QString inputcolor);
    const char* identifyObject(int index);

    void drawString (const char *s);
    void displayMousePosition();
    void pictdisplayMousePosition();
    void drawAline(int x1, int y1, int x2, int y2);
    void drawPositionInImg(int i,int j);
    void writeTextAtPos(int x,int y, QString str );
    int getImage_value(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w);

    void m_show_object(int inputimg[MAX_ISIZE][MAX_ISIZE],int xstart, int ystart, int h,int w,int icolor);
    void m_show_object_disc(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w,int icolor,int jcolor,int ccolor, bool showText);

    void showComponent(const char* type,int discamnt,Medge res[MAX_RES],int xstart,int ystart,int icolor);

    void forTest();

    void var_cp();
    void clearVar();
    void clearComp_var();
    void cleanImage(int object[MAX_ISIZE][MAX_ISIZE],int imax, int jmax);
    void cleanSkeleton(int object[MAX_ISIZE][MAX_ISIZE],int imax, int jmax);

    void cal_dc(int imax, int jmax);
    void cal_br_spacing(int imax, int jmax);

    void showSkel();
    void KeepSkel(int ct, int c,Ij_pos skel_part[MAX_CIRC],int n_skel_part);
    void clearFS();
    void ReportFS();
    bool isSameFS(int x1,int y1,int xn, int yn);
    void KeepJunction(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w);
    void KeepJuncInArray(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x,int xstart,int ystart);

    int Contour_Size(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax);
    int Picture_Area(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax);

    void TrackSkel(int junc[MAX_ISIZE][MAX_ISIZE],int imax, int jmax,int i_start,int j_start,Ij_pos skel_part[MAX_CIRC],int *n_skel_part);
    void add_neigh_skel1(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  pos,int   *n_border,Ij_pos  border[MAX_CIRC]);

    void clearAngleRec();
    void KeepAngle(int junclabel,int ai_ct,int aj_ct, int bi_ct,int bj_ct);
    void ReportAngle();
    // measure each angle
    Real measure_angle(int ai_ct,int aj_ct,int b0i_ct,int b0j_ct,int b1i_ct,int b1j_ct);
    void Cal_Angle(); // keep result into the array AngleResult

    void drawBox(int x1,int y1, int x2, int y2); // draw line around picture
    void displayGrid();
    void displayFigurelength();
    void ObjScaleDialog(float noofpixels);

    // Branch order
    void cal_Branch_Order1();
    int BOrder1_Count;
    void showBranchOrder(int xstart, int ystart);
    // end of Branch order
    //fractal dimension using box counting
    Real fractalDim;
    // end of fractal dimension

    // fix RB in order to measure branch order, angle
    bool Tree_process_finish;
    Ij_Value Treerootpos;
    Medge WithoutRedundantRB[MAX_RES]; // update RB without redundant route
    Medge WithoutRedundantRB_BeforeTreeCon[MAX_RES]; // copy of WithoutRedundantRB when choosing repair branch in menu
    Medge OrderedRB[MAX_RES]; // order RB from WithoutRedundantRB
    int WRRB_count;
    int WRRB_count_BeforeTreeCon; // copy of WRRB_count when choosing repair branch in menu
    Medge repairedRB[MAX_RES]; // update RB, but still have redundant route
    Ij_PC newRB[MAX_RES]; // new format of New RB (y1,x1,p1,y2,x2,p2,RbN,val,BrOrNo)
    Ij_Leaf Leaf[MAX_RES]; // keeping leaf point in the tree
    Ij_Info AllPoint[MAX_RES]; // keeping coordinates of each point in the tree
    // in orderRB()
    QStack<int> RBStack;
    int Ord_RB[MAX_RES]; // Keep RB no in order
    Medge Ord_Coord_RB[MAX_RES]; // Keep correct direction of RBs
    bool RB_Check[MAX_RES];
    int rcolor[20][3]; // ramdom color MAXCOLOR = 20
    int max_Branch_Order;
    // Keep junction no. with amount of branch, and Brach No (s) and use this one to sort angle
    // format : JuncNo,AmountOfRbs,RbNo#1,RbNo#2..
    QVector<int> JuncWithSortedRB[MAX_RES];
    // Keep junction no. with angle of each pair of braches located in the junction no. (Closewise direction)
    // format : Juncno,AmountOfAngle,Angle#1,Angle#2..
    QVector<float> JuncWithSortedAngle[MAX_RES];

    QVector<int> SortedAngleJuncNotLeaves;
    int NLeaveCount; // Count record (leaves) in SortedAngleJuncNotLeaves

    // keep junction connected with leaves (filter from JuncWithSortedAngle)
    QVector<float> VSortedAngleJuncNotLeaves[MAX_RES];

    Ij_PC branch_Order_array[100][200];
    int BrOr_count[100]; // counting amt of rb(s) for each branch order
    int new_n_rb; // The number of links after linkrepair()

    Ij_Value rootSearching(Medge whatgroup[MAX_RES],int nrb);
    Ij_Value NearestJunction(int x,int y,int excljuncno); // Ij_value means {y,x,juncno:junction or 1000+dcno.:leaf}
    void correctRB();
    void CoordToNode();
    void NodeToCoord();
    void orderRB();
    void sortRB(int y_i_ct, int x_j_ct,int n);
    void showTreeGL(int xstart,int ystart);
    void ComputeBranchOrder();
    void cutBranch(Ij_PC newrb[MAX_RES],int n,int loopno);
    void LTRBRSorting(); // travel each branches in each junction from Left to Right
    QVector<int> LeftToRightBRSorting(QVector<int> vinput);
    void random_Color();
    void sortAngleInJunc(); // sorting angles in each junction
    QVector<int> JunctionConnectedToLeaf(Ij_PC newrb[MAX_RES]);
    void ListJunctionNotConnectedToLeft();
    void BranchEachBr();
    void RepairLinks();
    QVector<int> getRBsWithDcs(int dcy,int dcx,Medge noredundantrb[MAX_RES],int rbc);


    void saveSkelToFile();
    void saveContourToFile();

    // cleaning ultimate branch
    void updateBranchOrder0(int pos);
    void CleanUltimateBranch(float MinPixLengthToDel);
    void updateDC(int dcy,int dcx);
    int MinUltiLengthInPix;
    float MinUltiLength;
    void updateDCArray();
    void updateDCinReport();
    void updateBranchLength(int dcy,int dcx);
    void updateBranchRB(int pos);
    void updateBranchMRB(int pos); // for updateReport
    void updateBrSpacing(int dcy,int dcx);
    void updateBrSpacingArray(int pos);
    void updateBrSpacinginReport();
    void update_br_spacing_meas(int dcy,int dcx);
    bool test_disc_too_large_br_update(int  im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,Ij_pos circle[MAX_CIRC],int n_circle);

    Ij_Value NearestDC(int y,int x);
    void updateBranchOrder0FromDC(int dcy,int dcx);

    int MinTerminalRadiusInPix;
    float MinTerminalDiameter;
    void CleanTerminal(float MinTermimalRadiusToDel);
    Ij_Value myroot;
    QString CSVFilename;

    bool bool_correctrb;
    void clearWRRB();
    void copy_clearWRRB();

    float CalCCCLength(int con[MAX_ISIZE][MAX_ISIZE], int imax, int jmax);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void showPicture();
    void showSkeleton();
    void showJunction();
    void showTerminal();
    void showContour();
    void showDiscA();
    void showDiscB();
    void showDiscC();
    void showRb();
    void showGeo();
    void showTerm();
    void showBr();
    void showFS();
    void showTree();
    void setColor();
    void setEdit();
    void deleteTerminal();
    void showDetail();
    void InteractionAct();
    void showTest();
    void showStart();
    void forTest_slot();
    void autostart();
    void TreeStart();
    void showBranchOrderSlot();
    void CleanUltimateBranchAction();
    void CleanTerminalAction();
    void RepairBranchAction();
    void selectroot();

    void zoomin();
    void zoomout();
    void showGrid();
    void showScale();
    void showExport();
    void showexportSkel();
    void showexportContour();

    void showScalenm();
    void showScaleum();
    void showScalemm();
    void showScalecm();
    void showScaleinch();
    void showfigureLength();
    void setPointUp();
    void setPointDown();
    void Man_Start();
    void setObjScale();
    void showObjectScale();
    void showJunctionLabel();
    void showJunctionCross();

    void okClicked();
    void cancelClicked();
    void CleancancelClicked();
    void CleanokClicked();

    void cleanSetting();
    void setNewRoot();
    void XYPosSetting();
    void XYPosokClicked();
    void XYPoscancelClicked();

    private :
    bool bool_show_picture;
    bool bool_show_skeleton;
    bool bool_show_junction;
    bool bool_show_terminal;
    bool bool_show_contour;
    bool bool_show_discA;
    bool bool_show_discB;
    bool bool_show_discC;
    bool bool_show_rb;
    bool bool_show_geo;
    bool bool_show_term;
    bool bool_show_br;
    bool bool_show_FS;
    bool bool_show_Tree;

    bool bool_show_edit;
    bool bool_show_test;
    bool bool_show_start;
    bool bool_show_manual;

    bool bool_show_grid;
    bool bool_show_Scalenm;
    bool bool_show_Scaleum;
    bool bool_show_Scalemm;
    bool bool_show_Scalecm;
    bool bool_show_Scaleinch;
    bool bool_show_figurelength;
    bool bool_show_junctionlabel;
    bool bool_show_junctioncross;

    QAction *pictureAct;
    QAction *skeletonAct;
    QAction *junctionAct;
    QAction *terminalAct;
    QAction *contourAct;
    QAction *discAct;
    QAction *SelectRootAct;

 };

#endif
