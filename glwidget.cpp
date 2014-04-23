#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <stdio.h>
#include <string.h>


#ifdef Q_WS_MAC
    // for mac
   #include <GLUT/glut.h>  // you have to put the lib in .pro file
#endif

#ifdef Q_WS_WIN
    // for windows
   #include "glut.h"  // on windows, you have to add libs in .pro file
#endif
#ifdef Q_WS_X11       // on X11 / linux
    #include "GL/glut.h"
#endif


#include "glwidget.h"
#include "fravar.h"
#include "fratype.h"
#include "basictool.h"

#include <QList>

#define BG_COLOR 0
#define PICT_COLOR 1
#define SKEL_COLOR 2
#define JUNC_COLOR 3
#define TERM_COLOR 4
#define CONT_COLOR 5
#define DISCA_COLOR 6
#define DISCB_COLOR 7
#define DISCC_COLOR 8
#define RB_COLOR 9
#define GEO_COLOR 10
#define TERMA_COLOR 11
#define BR_COLOR 12
#define FS_COLOR 13
#define CROSS_COLOR 14

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
      setMouseTracking(true);
      setFocusPolicy ( Qt::StrongFocus ); // you need this to use keyevent, otherwise it is not working

      ScreenWidth = QApplication::desktop()->width();
      ScreenHeight = QApplication::desktop()->height();

      printf("Screen width = %d, screen height = %d \n",ScreenWidth,ScreenHeight);

      bstool = new BSTool();

      xratio = (float)XLENGTH/QApplication::desktop()->width();
      yratio = (float)XLENGTH/QApplication::desktop()->width();
      screen_diagonal = 13.0;

      clearVar();
      colorname[PICT_COLOR]="#eeee11";
      colorname[SKEL_COLOR]="#11ff11";
      colorname[JUNC_COLOR]="#7b82ff";
      colorname[TERM_COLOR]="#ff7be9";
      colorname[CONT_COLOR]="#18ffc0";
      colorname[DISCA_COLOR]="#df113a";
      colorname[DISCB_COLOR]="#848484";
      colorname[DISCC_COLOR]="#11ce11";
      colorname[RB_COLOR]="#ff11e0";
      colorname[GEO_COLOR]="#22ffe0";
      colorname[TERMA_COLOR]="#fe642e";
      colorname[BR_COLOR]="#ffffff";
      colorname[FS_COLOR]="#4000ff";
      colorname[CROSS_COLOR]="#1C1FD4"; // blue

      param = 4; // define the size of the group of pixel

subdiv_nrs[0] = 8; subdiv_nrs[1] = 9; subdiv_nrs[2] = 10; subdiv_nrs[3] = 11; subdiv_nrs[4] = 12;
subdiv_nrs[5] = 13; subdiv_nrs[6] = 14; subdiv_nrs[7] = 15 ; subdiv_nrs[8] = 16; subdiv_nrs[9] =18 ;
subdiv_nrs[10] = 20; subdiv_nrs[11] =22 ; subdiv_nrs[12] =24 ; subdiv_nrs[13] = 26; subdiv_nrs[14] =28 ;
subdiv_nrs[15] = 30; subdiv_nrs[16] = 32; subdiv_nrs[17] =36 ; subdiv_nrs[18] =40 ; subdiv_nrs[19] =44 ;
subdiv_nrs[20] = 48; subdiv_nrs[21] = 52; subdiv_nrs[22] = 56; subdiv_nrs[23] = 60; subdiv_nrs[24] = 64;
subdiv_nrs[25] = 72; subdiv_nrs[26] = 80; subdiv_nrs[27] = 88; subdiv_nrs[28] = 96; subdiv_nrs[29] = 104;
subdiv_nrs[30] = 112; subdiv_nrs[31] = 120 ; subdiv_nrs[32] =128; subdiv_nrs[33] =144 ; subdiv_nrs[34] =160 ;
subdiv_nrs[35] = 176; subdiv_nrs[36] = 192; subdiv_nrs[37] = 208; subdiv_nrs[38] = 224; subdiv_nrs[39] =240 ;
subdiv_nrs[40] = 256; subdiv_nrs[41] = 288; subdiv_nrs[42] = 320; subdiv_nrs[43] = 352; subdiv_nrs[44] = 384;
subdiv_nrs[45] = 416; subdiv_nrs[46] = 448 ; subdiv_nrs[47] = 480; subdiv_nrs[48] = 512; subdiv_nrs[49] =576 ;
subdiv_nrs[50] = 640; subdiv_nrs[51] = 704; subdiv_nrs[52] = 768; subdiv_nrs[53] = 832; subdiv_nrs[54] = 896;
subdiv_nrs[55] = 960 ; subdiv_nrs[56] = 1024;


        x_pos_start = 50;
        y_pos_start = 50;

        CSVFilename="DataCSV.csv";
        firstfilename = "";
        readfile(firstfilename);
}

GLWidget::~GLWidget() { }

void GLWidget::initializeGL()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,ScreenWidth,ScreenHeight,0,-1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} // initializeGL

void GLWidget::clearVar()
{
    bool_show_picture=true;
    bool_show_skeleton=false;
    bool_show_junction=false;
    bool_show_terminal=false;
    bool_show_contour=false;
    bool_show_discA=false;
    bool_show_discB=false;
    bool_show_discC=false;
    bool_show_rb=false;
    bool_show_geo=false;
    bool_show_term=false;
    bool_show_br=false;
    bool_show_FS=false;
    bool_show_Tree=false;

    bool_show_edit=false;
    bool_show_test=false;
    bool_show_start=false;
    bool_show_manual=false;

    bool_show_Scalemm = true;
    bool_show_Scalecm = false;
    bool_show_Scaleinch = false;
    bool_show_Scalenm = true;
    bool_show_Scaleum = true;

    bool_correctrb = false;

    image_x=0;
    image_y=0;
    figure_length_x=0.0;
    figure_length_y=0.0;

    junction_amount=0;
    terminal_amount=0;
    discA_amount=0;
    discB_amount=0;

    idobj="";
    qstrToch="";
    xypos_ch="";
    pic_xypos_ch="";
    pic_idvalue="";
    pic_idvalue1="";
    posx = 0;
    posy = 0;
    wcx = 0;
    wcy = 0;
    colorname_selected = "";
    colorname_detected = "";
    first_time = false;

    max_skel_length = MAX_SKEL_LENGTH;
    scaleunit = "mm";

    count_disc_area=0;
    for(int d=0;d<MAXJUNC;d++)
    {
        disc_area_x[d] = 0;
        disc_area_y[d] = 0;
    }

    ExactJuncLabel=0;

    set_show = true;
    disc_label = 0;
    clearFS();
    AngleA = 0;
    ExactJuncAmnt=0;
    clearAngleRec();
    count_maxskel = 0;
    count_FLengthTrim = 0;

    MaxPointSize=4.0;
    MinPointSize=1.0;
    pointsize = 1.0;  // Point Size of Image on the screen

    setScaleFlag=false;
    setFirstClick=false;

    // root
    myroot.F_no = 0;
    myroot.S_no = 0;
    myroot.val = 0;

    for(int i=0;i<MAX_RES;i++)
    {
        repairedRB[i].p_i_ct=0;
        repairedRB[i].p_j_ct=0;
        repairedRB[i].i_ct=0;
        repairedRB[i].j_ct=0;
        repairedRB[i].i1_ct=0;
        repairedRB[i].j1_ct=0;
        repairedRB[i].meas=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        WithoutRedundantRB[i].p_i_ct=0;
        WithoutRedundantRB[i].p_j_ct=0;
        WithoutRedundantRB[i].i_ct=0;
        WithoutRedundantRB[i].j_ct=0;
        WithoutRedundantRB[i].i1_ct=0;
        WithoutRedundantRB[i].j1_ct=0;
        WithoutRedundantRB[i].meas=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        OrderedRB[i].p_i_ct=0;
        OrderedRB[i].p_j_ct=0;
        OrderedRB[i].i_ct=0;
        OrderedRB[i].j_ct=0;
        OrderedRB[i].i1_ct=0;
        OrderedRB[i].j1_ct=0;
        OrderedRB[i].meas=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        Ord_RB[i]=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        Ord_Coord_RB[i].p_i_ct=0;
        Ord_Coord_RB[i].p_j_ct=0;
        Ord_Coord_RB[i].i_ct=0;
        Ord_Coord_RB[i].j_ct=0;
        Ord_Coord_RB[i].i1_ct=0;
        Ord_Coord_RB[i].j1_ct=0;
        Ord_Coord_RB[i].meas=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        RB_Check[i]=false;
    }

    for(int i=0;i<100;i++)
    {
        BrOr_count[i]=0;
    }

    for(int i=0;i<MAX_RES;i++)
    {
        newRB[i].x1=0;
        newRB[i].y1=0;
        newRB[i].p1=0;
        newRB[i].x2=0;
        newRB[i].y2=0;
        newRB[i].p2=0;
        newRB[i].RbN=0;
        newRB[i].BrOrNo=0;
        newRB[i].val=0;
    }

    SortedAngleJuncNotLeaves.clear();
    for(int i=0;i<MAX_RES;i++)
    {
        JuncWithSortedRB[i].clear();
        JuncWithSortedAngle[i].clear();
        VSortedAngleJuncNotLeaves[i].clear();
    }

    for(int i=0;i<100;i++)
    {
        for(int j=0;j<200;j++)
        {
            branch_Order_array[i][j].x1=0;
            branch_Order_array[i][j].y1=0;
            branch_Order_array[i][j].p1=0;
            branch_Order_array[i][j].x2=0;
            branch_Order_array[i][j].y2=0;
            branch_Order_array[i][j].p2=0;
            branch_Order_array[i][j].RbN=0;
            branch_Order_array[i][j].BrOrNo=0;
            branch_Order_array[i][j].val=0;
        }
    }

    BOrder1_Count=0;
    MinUltiLengthInPix = 1;
    MinTerminalRadiusInPix = 2;
    MinUltiLength = 0;
    MinTerminalDiameter = 0;

} //clearVar

void GLWidget::clearAngleRec()
{
    for(int i=0;i<MAXJUNC;i++)
        for(int j=0;j<ANGLEATT;j++)
         {
            AngleRec[i][j] = 0;
            AngleResult[i][j] = 0.0;
        }
}

const char* GLWidget::identifyObject(int index)
{
    const char* getobject="Nothing";
    switch (index)
    {
    case PICT_COLOR : getobject = "Picture"; break;
    case SKEL_COLOR : getobject = "Skeleton"; break;
    case JUNC_COLOR : getobject = "Junction"; break;
    case TERM_COLOR : getobject = "Terminal"; break;
    case CONT_COLOR : getobject = "Contour"; break;
    case DISCA_COLOR : getobject = "Disc A"; break;
    case DISCB_COLOR : getobject = "Disc B"; break;
    case DISCC_COLOR : getobject = "Disc C"; break;
    case RB_COLOR : getobject = "Rb"; break;
    case GEO_COLOR : getobject = "Geo"; break;
    case TERMA_COLOR : getobject = "Term"; break;
    case BR_COLOR : getobject = "Br"; break;
    case FS_COLOR : getobject = "fs"; break;
    case BG_COLOR : getobject = "Background"; break;
    }
    return getobject;
} // identifyObject

int GLWidget::compareColor(QString inputcolor)
{
    int value = 0;

    for( int i=0;i<COLOR_NO;i++)
    {
        if(inputcolor==colorname[i])
        {
            value = i;
            exit;
        }
    }
    return value;
} // compareColor

int GLWidget::getImage_value(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w)
{
    return inputimg[h][w];
}

void GLWidget::displayMousePosition()
{
    glColor3f (1.0F, 1.0F, 1.0F);
    glRasterPos2i (-5, -5);
    drawString(xypos_ch);
    drawString(idobj);
}

void GLWidget::pictdisplayMousePosition()
{
    glColor3f (1.0F, 1.0F, 1.0F);
    glRasterPos2i (-2, -10);
    drawString(pic_xypos_ch);
    drawString(pic_idvalue);
    drawString(pic_idvalue1);
}

void GLWidget::writeTextAtPos(int x,int y, QString str )
{
    qglColor("#F50808");
    glRasterPos2i (x+x_pos_start, y+y_pos_start);
    //const char* char_str = str.toUtf8().constData();
    //drawString(char_str);
    drawString(str.toUtf8().constData());
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(bool_show_grid) displayGrid();

    if(bool_show_figurelength) displayFigurelength();
    glPointSize(pointsize);
    glBegin(GL_POINTS);

        // picture
        if(bool_show_picture) m_show_object(im_in,x_pos_start,y_pos_start,im_in_imax,im_in_jmax,PICT_COLOR);
        // skeleton
        if(bool_show_skeleton) m_show_object(im_skel,x_pos_start,y_pos_start,im_in_imax,im_in_jmax,SKEL_COLOR);
        // junction
        if(bool_show_junction) m_show_object(im_junc_reduced,x_pos_start,y_pos_start,im_in_imax,im_in_jmax,JUNC_COLOR);
        // terminal
        if(bool_show_terminal) m_show_object(im_term,x_pos_start,y_pos_start,im_in_imax,im_in_jmax,TERM_COLOR);
        // contour
        if(bool_show_contour) m_show_object(im_contour,x_pos_start,y_pos_start,im_in_imax,im_in_jmax,CONT_COLOR);

        for (int x=0;x<count_disc_area;x++)
        {
            if(bool_show_junctioncross) show_cross(disc_area_x[x],disc_area_y[x],CROSS_COLOR,NORMAL_SIZE);
            if(bool_show_junctionlabel) writeTextAtPos(disc_area_x[x],disc_area_y[x],QString::number(x+1));
        }

        if(bool_show_discA) showComponent("disca",n_da_discs,da_discs,x_pos_start,y_pos_start,DISCA_COLOR);
        if(bool_show_discB) showComponent("discb",n_db_discs,db_discs,x_pos_start,y_pos_start,DISCB_COLOR);
        if(bool_show_discC) showComponent("discc",n_dc_discs,dc_discs,x_pos_start,y_pos_start,DISCC_COLOR);
        if(bool_show_rb)
        {
            if(bool_correctrb)
                showComponent("rb",WRRB_count_BeforeTreeCon,WithoutRedundantRB_BeforeTreeCon,x_pos_start,y_pos_start,RB_COLOR);
            else
                showComponent("rb",n_rb,rb,x_pos_start,y_pos_start,RB_COLOR);
            showBranchOrder(x_pos_start,y_pos_start); // show ultimate branch in another color
        }
        if(bool_show_geo) showComponent("geo",n_geotropy,geotropy,x_pos_start,y_pos_start,GEO_COLOR);
        if(bool_show_term) showComponent("term",term_n_geotropy,term_geotropy,x_pos_start,y_pos_start,TERMA_COLOR);
        if(bool_show_br) showComponent("br",n_br_spacing,br_spacing,x_pos_start,y_pos_start,BR_COLOR);
        if(bool_show_FS) showComponent("fs",n_rb,rb,x_pos_start,y_pos_start,FS_COLOR);
        if(bool_show_Tree) showTreeGL(x_pos_start,y_pos_start);
   glEnd();

        if(setScaleFlag)
        {
            if(secondposx==0)
            {
                secondposx=firstposx;
                secondposy=firstposy;
            }
            drawAline(firstposx,firstposy,secondposx,secondposy); // draw a line to set object scale measurement
        }
} // paintGL

void GLWidget::KeepSkel(int ct,int c,Ij_pos skel_part[MAX_CIRC],int n_skel_part)
{
    FLength[c][0] = skel_part[0].i_x;
    FLength[c][1] = skel_part[0].j_y;
    FLength[c][2] = skel_part[n_skel_part-1].i_x;
    FLength[c][3] = skel_part[n_skel_part-1].j_y;
    FLength[c][4] = n_skel_part;
    FLength[c][5] = ct;

    for( int i= 0; i< n_skel_part; i++)
    {
      FS[c][i] = skel_part[i];
    }
} // KeepSkel

void GLWidget::clearFS()
{
    for(int i=0;i<MAXNOSKEL;i++)
    {
        for(int k=0;k<6;k++)
        {
          FLength[i][k]=0;
          FLengthTrim[i][k]=0;
        }
        for(int j=0;j<MAXPOINT;j++)
        {
            FS[i][j].i_x=0;
            FS[i][j].j_y=0;
        }
    }
} // clearFS

void GLWidget::showSkel()
{
    glColor3f (0.0F, 0.0F, 0.0F);
    glBegin(GL_POINTS);
    for(int i=0;i<count_maxskel;i++)
        for(int j=0;j<MAXPOINT;j++)
        {
          if(FS[i][j].j_y != 0 && FS[i][j].i_x != 0)
            glVertex2d(FS[i][j].j_y,FS[i][j].i_x);
        }
    glEnd();
} // showSkel

void GLWidget:: showComponent(const char* type,int discamnt,Medge res[MAX_RES],int xstart,int ystart,int icolor)
{

    for(int i=0;i<discamnt;i++)
    {
         qglColor(colorname[icolor]);
         int x1 = res[i].p_i_ct+ystart;
         int y1 = res[i].p_j_ct+xstart;
         int x2 = res[i].i_ct+ystart;
         int y2 = res[i].j_ct+xstart;
         int meas = (int)res[i].meas;

         if(strcmp(type,"rb")==0 ) // draw lines between junctions
         {
             glLineWidth(pointsize);
             glBegin(GL_LINES);
              glVertex2i(y1,x1);
              glVertex2i(y2,x2);
             glEnd();

         } // if rb
         else if(strcmp(type,"fs")==0)
         {
             glPointSize(pointsize);
             glBegin(GL_POINTS);
             for(int i=0;i<count_maxskel;i++)
                 for(int j=0;j<MAXPOINT;j++)
                 {
                   if(FS[i][j].j_y != 0 && FS[i][j].i_x != 0)
                     glVertex2d(FS[i][j].j_y+xstart,FS[i][j].i_x+ystart);
                 }
             glEnd();
         }
         else if(strcmp(type,"geo")==0 || strcmp(type,"term")==0 )  // draw lines between junctions
         {
                 glLineWidth(pointsize);
                 glBegin(GL_LINES);
                  glVertex2i(y1,x1);
                  glVertex2i(int((y2+y1)/2),int((x2+x1)/2));
                 glEnd();
                // draw dotted line
                 glPushAttrib(GL_ENABLE_BIT);
                 glLineStipple(1, 0xAAAA);
                 glEnable(GL_LINE_STIPPLE);
                 glBegin(GL_LINES);
                  glVertex2i(y1,x1);
                  glVertex2i(y1,x1-70);
                 glEnd();
                 glPopAttrib();
                 // end dotted line

         } // if geo
         else if(strcmp(type,"br")==0)
         {
             glLineWidth(pointsize);
             glColor3f(1.0f,1.0f,1.0f);
             glBegin(GL_LINE_STRIP);
             for (float a=0;a<2*MYPI;a+=MYPI/180)
             {
               glVertex2f(y1 + cos(a) * meas,x1 + sin(a) * meas);
             } // drawing a circle for br
             glEnd();
         } // if br
         else
         {
             glLineWidth(pointsize);
             glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
             glBegin(GL_TRIANGLE_FAN);
             for (float a=0;a<2*MYPI;a+=MYPI/180)
             {
               glVertex2f(y1 + cos(a) * meas,x1 + sin(a) * meas);
             } // drawing a circle disc a, disc b, disc c
             glEnd();
             //writeTextAtPos(y1,x1,QString::number(i+1));
         }
    } // for
} // showComponent

void GLWidget::drawBox(int x1,int y1, int x2, int y2)
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1,1,1);
        glLineWidth(1.0);
        glBegin(GL_LINE_LOOP);
         glVertex2i(x1,y1);
         glVertex2i(x2,y1);
         glVertex2i(x2, y2);
         glVertex2i(x1, y2);
        glEnd();
}

void GLWidget::displayGrid()
{
    int xgridstart = 20,ygridstart = 10;
    int xstep = rint(ppi*xratio/25.4);
    int ystep = rint(ppi*yratio/25.4);

    int x_txt_start = 0;
    int y_txt_start = 0;
    int jump = 5;

    if(bool_show_Scalemm)
    {
        xstep = rint(ppi*xratio/25.4);
        ystep = rint(ppi*yratio/25.4);
    }
    if(bool_show_Scalecm)
    {
        xstep = rint(ppi*xratio/2.54);
        ystep = rint(ppi*yratio/2.54);
    }
    if(bool_show_Scaleinch)
    {
        xstep = rint(ppi*xratio);
        ystep = rint(ppi*yratio);
    }

    drawBox(xgridstart,ygridstart,ScreenWidth-50,ScreenHeight-50);
    for(int x=xgridstart; x<=ScreenWidth-50; x+=xstep,x_txt_start+=1)
    {
        if(bool_show_Scalemm)
        {
            if(x_txt_start%(jump*1)==0)
            {
                glColor3f (1.0F, 1.0F, 1.0F);
                glRasterPos2i (x, 0);
                QString txt_number = QString::number(x_txt_start);

                //const char* char_str = txt_number.toUtf8().constData();
                //drawString(char_str);
		drawString(txt_number.toUtf8().constData());

                glBegin(GL_LINES);
                   glVertex2i(x,ygridstart);
                   glVertex2i(x,ScreenHeight-50);
                glEnd();

                glRasterPos2i (x, ScreenHeight-30);
                QString btxt_number = QString::number(x_txt_start);

                //const char* bchar_str = btxt_number.toUtf8().constData();
                //drawString(bchar_str);
		drawString(btxt_number.toUtf8().constData());
            }
        }
        else
        {
            if(x_txt_start%jump==0)
            {
                glColor3f (1.0F, 1.0F, 1.0F);
                glRasterPos2i (x, 0);
                QString txt_number = QString::number(x_txt_start);
                //const char* char_str = txt_number.toUtf8().constData();
                //drawString(char_str);
		drawString(txt_number.toUtf8().constData());

                glRasterPos2i (x, ScreenHeight-30);
                QString btxt_number = QString::number(x_txt_start);
                //const char* bchar_str = btxt_number.toUtf8().constData();
                //drawString(bchar_str);
		drawString(btxt_number.toUtf8().constData());
            }
            glBegin(GL_LINES);
               glVertex2i(x,ygridstart);
               glVertex2i(x,ScreenHeight-50);
            glEnd();
        }
    } // for x

    // y axis
    for(int y=ygridstart; y<=ScreenHeight-50; y+=ystep,y_txt_start+=1)
    {
        if(bool_show_Scalemm)
        {
            if(y!=ygridstart && y_txt_start%(jump*1)==0)
            {
                glColor3f (1.0F, 1.0F, 1.0F);
                glRasterPos2i (0, y);
                QString txt_number = QString::number(y_txt_start);
                //const char* char_str = txt_number.toUtf8().constData();
                //drawString(char_str);
		drawString(txt_number.toUtf8().constData());

                glRasterPos2i (ScreenWidth-40, y);
                QString btxt_number = QString::number(y_txt_start);
                //const char* bchar_str = btxt_number.toUtf8().constData();
                //drawString(bchar_str);
		drawString(btxt_number.toUtf8().constData());

                glBegin(GL_LINES);
                   glVertex2i(xgridstart,y);
                   glVertex2i(ScreenWidth-50,y);
                glEnd();
            }
        }
        else
        {
            if(y!=ygridstart && y_txt_start%jump==0)
            {
                glColor3f (1.0F, 1.0F, 1.0F);
                glRasterPos2i (0, y);
                QString txt_number = QString::number(y_txt_start);
                //const char* char_str = txt_number.toUtf8().constData();
                //drawString(char_str);
		drawString(txt_number.toUtf8().constData());

                glRasterPos2i (ScreenWidth-40, y);
                QString btxt_number = QString::number(y_txt_start);
                //const char* bchar_str = btxt_number.toUtf8().constData();
                //drawString(bchar_str);
		drawString(btxt_number.toUtf8().constData());
            }
            glBegin(GL_LINES);
               glVertex2i(xgridstart,y);
               glVertex2i(ScreenWidth-50,y);
            glEnd();
        }
    } // for y
} // displayGrid

void GLWidget::displayFigurelength()
{
    int fig_height = figure_length_y;
    int fig_width = figure_length_x;
    int linedistance = 50;
    int top = figure_topy+y_pos_start;
    int bottom = top+fig_height;
    int left = figure_topx+x_pos_start;
    int right = left+fig_width;
    int w_txt_middle = (left+right)/2;
    int h_txt_middle = (top+bottom)/2;
    int h_txtdistance = 20;
    int w_txtdistance = 30;


    float hpixelconvert = fig_height/ppmm;
    float wpixelconvert = fig_width/ppmm;

    QString unit = " "+scaleunit;
    QString txt_pix = " pixels ( ";
    QString txt_end = " )";

    //  y axis
    glColor3f(1,0,0);
    glLineWidth(5.0);
    glBegin(GL_LINES);
       glVertex2i(right+linedistance,top);
       glVertex2i(right+linedistance,bottom);
    glEnd();

    glRasterPos2i (right+linedistance+h_txtdistance,h_txt_middle);
    QString htxt_number = QString::number(fig_height);
    QString htxt_convert = QString::number(hpixelconvert);
    QString halltxt = htxt_number+txt_pix+htxt_convert+unit+txt_end;
    
    //const char* hchar_str = halltxt.toUtf8().constData();
    //drawString(hchar_str);
    drawString(halltxt.toUtf8().constData());

    // x axis
    glColor3f(1,0,0);
    glLineWidth(5.0);
    glBegin(GL_LINES);
       glVertex2i(left,bottom+linedistance);
       glVertex2i(right,bottom+linedistance);
    glEnd();

    glRasterPos2i (w_txt_middle,bottom+linedistance+w_txtdistance);
    QString wtxt_number = QString::number(fig_width);
    QString wtxt_convert = QString::number(wpixelconvert);
    QString walltxt = wtxt_number+txt_pix+wtxt_convert+unit+txt_end;
    //const char* wchar_str = walltxt.toUtf8().constData();
    //drawString(wchar_str);
    drawString(walltxt.toUtf8().constData());

}

void GLWidget::showScale() { }

void GLWidget::showExport() { }

void GLWidget::showexportSkel()
{
    saveSkelToFile();
}

void GLWidget::showexportContour()
{
    saveContourToFile();
}

void GLWidget::saveContourToFile()
{
    QString anafullpath = analysispath+"Contour_"+onlyfilename;
    QString fileName = QFileDialog::getSaveFileName(
    this,
    "Save File Dialog",
    anafullpath);

     if (fileName.isEmpty())
         return;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Contour Saving"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

    QImage myimage(im_in_jmax,im_in_imax,QImage::Format_Mono);

    for(int i=0;i<im_in_imax;i++)
        for(int j=0;j<im_in_jmax;j++)
            myimage.setPixel(j,i,!im_contour[i][j]);
    QImageWriter writer(fileName, "pbm");
    writer.write(myimage);
}

void GLWidget::saveSkelToFile()
{

    QString anafullpath = analysispath+"Skeleton_"+onlyfilename;
    QString fileName = QFileDialog::getSaveFileName(
    this,
    "Save File Dialog",
    anafullpath);

     if (fileName.isEmpty())
         return;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Skeleton Saving"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

    QImage myimage(im_in_jmax,im_in_imax,QImage::Format_MonoLSB);

    for(int i=0;i<im_in_imax;i++)
        for(int j=0;j<im_in_jmax;j++)
            myimage.setPixel(j,i,!im_skel[i][j]);
    QImageWriter writer(fileName, "pbm");
    writer.write(myimage);
}

void GLWidget::showScalenm()
{
    bool_show_Scalenm=true;
    bool_show_Scaleum=false;
    bool_show_Scalemm=false;
    bool_show_Scalecm=false;
    bool_show_Scaleinch=false;
    updateGL();
}

void GLWidget::showScaleum()
{
    bool_show_Scalenm=false;
    bool_show_Scaleum=true;
    bool_show_Scalemm=false;
    bool_show_Scalecm=false;
    bool_show_Scaleinch=false;
    updateGL();
}

void GLWidget::showScalemm()
{
    bool_show_Scalenm=false;
    bool_show_Scaleum=false;
    bool_show_Scalemm=true;
    bool_show_Scalecm=false;
    bool_show_Scaleinch=false;
    updateGL();
}

void GLWidget::showScalecm()
{
    bool_show_Scalenm=false;
    bool_show_Scaleum=false;
    bool_show_Scalemm=false;
    bool_show_Scalecm=true;
    bool_show_Scaleinch=false;
    updateGL();
}

void GLWidget::showScaleinch()
{
    bool_show_Scalenm=false;
    bool_show_Scaleum=false;
    bool_show_Scalemm=false;
    bool_show_Scalecm=false;
    bool_show_Scaleinch=true;
    updateGL();
}

void GLWidget::showfigureLength()
{
    if(bool_show_figurelength) bool_show_figurelength=false;
    else  bool_show_figurelength=true;
    updateGL();
}

void GLWidget:: forTest()
{
    ifollow_track( im_in, im_skel, im_junc, scratch, im_in_imax, im_in_jmax );
    QString analysisdefaultfilename = analysispath+"result.res";

    //const char* analysisfilename_saved = analysisdefaultfilename.toUtf8().constData();
    //dump_res(analysisfilename_saved);
    dump_res(analysisdefaultfilename.toUtf8().constData());
}

void GLWidget:: autostart()
{
    int loop = 0;
    process_finish = false;
    QProgressDialog progressDialog(0);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, count_disc_area);
    progressDialog.setWindowTitle(tr("Calculating.."));

    while (loop < count_disc_area)
    {
        progressDialog.setValue(loop);
        progressDialog.setLabelText(tr("Processing %1 of %2...")
                                    .arg(loop).arg(count_disc_area));
        qApp->processEvents();
        if (progressDialog.wasCanceled())
            break;

        ifollow_track( im_in, im_skel, im_junc, scratch, im_in_imax, im_in_jmax );
        loop++;
    } // while
    cal_Branch_Order1(); // calculate length of branch order 1
    var_cp();
    ReportFS();
    ReportAngle();
    Cal_Angle();

    process_finish = true;

} // autostart

void GLWidget:: forTest_slot()
{
    process_finish = false;
    ifollow_track( im_in, im_skel, im_junc, scratch, im_in_imax, im_in_jmax );
    cal_Branch_Order1(); // calculate length of branch order 1
    var_cp();

    //QString resdefaultfilename = analysispath+"result.res";
    //const char* resfilename_saved = resdefaultfilename.toUtf8().constData();

    //QString stadefaultfilename = analysispath+"result.sta";
    //const char* stafilename_saved = stadefaultfilename.toUtf8().constData();
    // these variables are not used.
    // the char *:s are invalid, so they cannot be used.


    process_finish = true;

} // forTest_slot


void GLWidget:: m_show_object_disc(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w,int icolor,int jcolor, int ccolor, bool showText)
{
      for (int i = 0; i < im_in_imax; i++)
       {
         for (int j = 0; j < im_in_jmax; j++)
         {
             if(showText)
             {
                 if (im_junc[i][j] == CENTER_MARK) drawPositionInImg(i,j);
             }
             else
             {
                  glPointSize(pointsize);
                  glBegin(GL_POINTS);
                  if (im_junc[i][j] == 1)
                  {
                     qglColor(colorname[icolor]);
                     glVertex2d(j,i);
                  }
                  else if (im_junc[i][j] == DISC_MARK)
                  {
                     qglColor(colorname[jcolor]);
                     glVertex2d(j,i);
                  }
                  else if (im_junc[i][j] == CENTER_MARK)
                  {
                     qglColor(colorname[ccolor]);
                     glVertex2d(j,i);
                  }
                  glEnd( );
             } // if showText
         } // for j
      } // for i
} // m_show_object_disc

void GLWidget:: drawPositionInImg(int y,int x)
{
    QString possx = QString::number(x);
    QString possy = QString::number(y);
    QString posf ="(";
    QString posc =",";
    QString posl =")";
    QString possxy = posf.append(possx.append(posc.append(possy.append(posl))));

    glColor3f (1.0F, 0.0F, 0.0F);
    glRasterPos2i(x,y);
    drawString(possxy.toUtf8().constData());
} // drawPositionInImg

void GLWidget:: drawAline(int x1, int y1, int x2, int y2)
{
    glColor3f(1,0,0);
    glLineWidth(1.0);
    glBegin(GL_LINES);
     glVertex2i(x1,y1);
     glVertex2i(x2,y2);
    glEnd();
} // drawAline

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, height,0.0);
} // resizeGL


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        setScaleFlag = false;
        setFirstClick = false;
        updateGL();
        wcx = 0;
        wcy = 0;
        firstposx=0;firstposy=0;secondposx=0;secondposy=0;
        setCursor(Qt::ArrowCursor);
    }

} // keyPressEvent

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{} // keyReleaseEvent

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
    } // if rightbutton

    if(event->button() == Qt::LeftButton)
    {

       int x = event->pos().x();
       posx = x;

       int y = event->pos().y();
       posy = y;

       GLint viewport[4]; //var to hold the viewport info
       GLdouble modelview[16]; //var to hold the modelview info
       GLdouble projection[16]; //var to hold the projection matrix info
       GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
       GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

       glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
       glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
       glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

       winX = (float)x;
       winY = (float)viewport[3] - (float)y;
       winZ = 0;
       gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

       wcx = (int) worldX;
       wcy = (int) worldY;
       QRgb coloratxy_ = this->grabFrameBuffer().pixel(posx,posy);
       int rr_ = qRed(coloratxy_);
       int gg_ = qGreen(coloratxy_);
       int bb_ = qBlue(coloratxy_);
       QString zero = "0";
       QString t = "#";
       QString sr = QString::number(rr_, 16);
       if(sr.length()==1) sr = zero + sr;
       QString sg = QString::number(gg_, 16);
       if(sg.length()==1) sg = zero + sg;
       QString sb = QString::number(bb_, 16);
       if(sb.length()==1) sb = zero + sb;

       colorname_detected = t.append(sr.append(sg.append(sb)));

       updateGL();
       QMenu menu;
       QAction *colorAct;
       QAction *editAct;
       QAction *detailAct;
       QAction *ObjScaleAct;
       QAction *BranchOrderAct;
       QAction *DelTerminalAct;

       if(setScaleFlag)
       {
           if(setFirstClick)
           {
               secondposx = wcx;
               secondposy = wcy;
               // show the line
               updateGL(); // draw a measured line
               // calculate the number of pixels between the 2 points
               scaledistancepixel = sqrt(pow((secondposx-firstposx),2)+pow((secondposy-firstposy),2));
               setFirstClick = false;
               setCursor(Qt::ArrowCursor);

               ObjScaleDialog(scaledistancepixel);
           }
           else
           {
               setFirstClick = true;
               firstposx = wcx;
               firstposy = wcy;
           } // else setFirstClick
       } // if setScaleFlag
       else
       {
           if(bool_show_edit)
           {
               editAct = new QAction(QIcon(pngpath+"addjunction.png"), tr("&Add/Repair Junction"), this);
               editAct->setStatusTip(tr("Add or repair the junction"));
               connect(editAct, SIGNAL(triggered()), this, SLOT(setEdit()));
               menu.addAction(editAct);

               DelTerminalAct = new QAction(QIcon(pngpath+"delterminal.png"), tr("&Delete Terminal"), this);
               DelTerminalAct->setStatusTip(tr("Delete Terminal"));
               connect(DelTerminalAct, SIGNAL(triggered()), this, SLOT(deleteTerminal()));
               menu.addAction(DelTerminalAct);

               SelectRootAct = new QAction(QIcon(pngpath+"selectroot.png"), tr("&Select Root"), this);
               SelectRootAct->setStatusTip(tr("Select Root"));
               connect(SelectRootAct, SIGNAL(triggered()), this, SLOT(selectroot()));
               menu.addAction(SelectRootAct);

           }
           else
           {
               colorAct = new QAction(QIcon(pngpath+"pickcolor.ico"), tr("&Set Color"), this);
               colorAct->setStatusTip(tr("Select Color"));
               connect(colorAct, SIGNAL(triggered()), this, SLOT(setColor()));
               menu.addAction(colorAct);

               detailAct = new QAction(QIcon(pngpath+"viewdetail.png"), tr("&Show Detail"), this);
               detailAct->setStatusTip(tr("Show Detail"));
               connect(detailAct, SIGNAL(triggered()), this, SLOT(showDetail()));

               // set object scal submenu
               ObjScaleAct = new QAction(QIcon(pngpath+"setobjectscale.ico"), tr("&Set Object Scale"), this);
               ObjScaleAct->setStatusTip(tr("Set Object Scale"));
               connect(ObjScaleAct, SIGNAL(triggered()), this, SLOT(setObjScale()));
               menu.addAction(ObjScaleAct);

               BranchOrderAct = new QAction(QIcon(pngpath+"viewdetail.png"), tr("&Show Branch Order"), this);
               BranchOrderAct->setStatusTip(tr("Show Branch Order"));
               connect(BranchOrderAct, SIGNAL(triggered()), this, SLOT(showBranchOrderSlot()));

           } // else bool_show_edit
       }// else setscaleflag
       menu.exec(mapToGlobal(event->pos()));
    } // LeftButton

    QGLWidget::mouseReleaseEvent(event);  //Dont forget to pass on the event to parent

} // mousePressEvent

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateGL();
}

void GLWidget::selectroot()
{
    int mousex = wcx-x_pos_start;
    int mousey = wcy-y_pos_start;

    myroot = NearestDC(mousey,mousex);
}

Ij_Value GLWidget::NearestDC(int y,int x)
{
    Real dist_min=1000.0;
    Real r0;
    Ij_Value nearestDCpos;
    nearestDCpos.F_no = dc_discs[0].p_i_ct; // y
    nearestDCpos.S_no = dc_discs[0].p_j_ct; // x
    nearestDCpos.val = 0;

    for(int i=0;i<n_dc_discs;i++)
    {
            r0 = sqrt(pow(y-dc_discs[i].p_i_ct,2)+pow(x-dc_discs[i].p_j_ct,2));
           // find min
            if(r0<dist_min)
            {
                dist_min = r0;
                nearestDCpos.F_no = dc_discs[i].p_i_ct; // y
                nearestDCpos.S_no = dc_discs[i].p_j_ct; // x
                nearestDCpos.val = i; // >=1000 is Leaf
            } // find min
    } // for
    return nearestDCpos;
} //NearestDC

void GLWidget::updateBranchOrder0FromDC(int dcy,int dcx)
{
   int pos=0;
   for(int i=0;i<BOrder1_Count;i++)
   {
       if(Branch_Order[0][i].p_i_ct==dcy && Branch_Order[0][i].p_j_ct==dcx)
       {
           pos = i;
           break;
       }
   } // for
   updateBranchOrder0(pos);

} //updateBranchOrder0FromDC

void GLWidget::deleteTerminal()
{
   // delete dc picked by mouse
    int mousex,mousey;
    Ij_Value nearestdc;
    mousex = wcx-x_pos_start;
    mousey = wcy-y_pos_start;

    nearestdc = NearestDC(mousey,mousex);
    updateDC(nearestdc.F_no,nearestdc.S_no);
    updateBranchOrder0FromDC(nearestdc.F_no,nearestdc.S_no);
    updateBranchLength(nearestdc.F_no,nearestdc.S_no);
    update_br_spacing_meas(nearestdc.F_no,nearestdc.S_no);
    updateBrSpacing(nearestdc.F_no,nearestdc.S_no);
    updateGL();

} // deleteTerminal

void GLWidget::setEdit()
{
    if(count_disc_area<MAXJUNC)
    {
        disc_area_x[count_disc_area] = wcx-x_pos_start;
        disc_area_y[count_disc_area] = wcy-y_pos_start;
        updateGL();
        count_disc_area++;
    }
} // setEdit

void GLWidget::setColor()
{
   QString textColorName;

   QMessageBox setcolor_box;
   setcolor_box.setText("You are selecting "+(QString)identifyObject(compareColor(colorname_detected))+". Do you want to change the color ?");
   setcolor_box.setIconPixmap(QPixmap(pngpath+"changecolor.png"));
   setcolor_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   int ret = setcolor_box.exec();
   if(ret == QMessageBox::Ok)
   {
   QColor color = QColorDialog::getColor(Qt::green, 0, "Select Color",  QColorDialog::DontUseNativeDialog);
   if(color.isValid())
   {
       textColorName = color.name();
       colorname_selected= textColorName;
       int match_index = compareColor(colorname_detected);
       colorname[match_index] = colorname_selected;
       updateGL();
    }
   } // if ret
} // setColor

void GLWidget::showStart()
{
    if(bool_show_start) bool_show_start=false;
    else  bool_show_start=true;
}

void GLWidget::Man_Start()
{
    count_disc_area=0;
    for(int d=0;d<MAXJUNC;d++)
    {
        disc_area_x[d] = 0;
        disc_area_y[d] = 0;
    }
    if(bool_show_manual)
    {
        bool_show_manual=false;
    }
    else
    {
        bool_show_manual=true;
        KeepJuncInArray(im_junc_reduced,im_in_imax, im_in_jmax,x_pos_start,y_pos_start);
    }
    updateGL();
} // Man_Start



void GLWidget::setPointUp()
{
   if(pointsize < MaxPointSize)
     pointsize = pointsize + 0.5;
   else pointsize = MaxPointSize;
   updateGL();
}

void GLWidget::setPointDown()
{
    if(pointsize > MinPointSize)
      pointsize = pointsize - 0.5;
    else pointsize = MinPointSize;
    updateGL();
}

void GLWidget::showGrid()
{
    if(bool_show_grid) bool_show_grid=false;
    else  bool_show_grid=true;
    updateGL();
}


void GLWidget::showTest()
{
    if(bool_show_test) bool_show_test=false;
    else  bool_show_test=true;
}

void GLWidget::showPicture()
{
    if(bool_show_picture) bool_show_picture=false;
    else  bool_show_picture=true;
    updateGL();
}

void GLWidget::showSkeleton()
{
    if(bool_show_skeleton) bool_show_skeleton=false;
    else bool_show_skeleton=true;
    updateGL();
}

void GLWidget::showJunction()
{
    if(bool_show_junction) bool_show_junction=false;
    else bool_show_junction=true;
    updateGL();
}

void GLWidget::showTerminal()
{
    if(bool_show_terminal) bool_show_terminal=false;
    else bool_show_terminal=true;
    updateGL();
}

void GLWidget::showContour()
{
    if(bool_show_contour) bool_show_contour=false;
    else bool_show_contour=true;
    updateGL();
}

void GLWidget::showDiscA()
{
    if(bool_show_discA) bool_show_discA=false;
    else bool_show_discA=true;
    updateGL();
}

void GLWidget::showDiscB()
{
    if(bool_show_discB) bool_show_discB=false;
    else bool_show_discB=true;
    updateGL();
}

void GLWidget::showDiscC()
{
    if(bool_show_discC) bool_show_discC=false;
    else bool_show_discC=true;
    updateGL();
}

void GLWidget::showRb()
{
    if(bool_show_rb) bool_show_rb=false;
    else bool_show_rb=true;
    updateGL();
}

void GLWidget::showGeo()
{
    if(bool_show_geo) bool_show_geo=false;
    else bool_show_geo=true;
    updateGL();
}

void GLWidget::showTerm()
{
    if(bool_show_term) bool_show_term=false;
    else bool_show_term=true;
    updateGL();
}

void GLWidget::showBr()
{
    if(bool_show_br) bool_show_br=false;
    else bool_show_br=true;
    updateGL();
}

void GLWidget::showFS()
{
    if(bool_show_FS) bool_show_FS=false;
    else bool_show_FS=true;
    updateGL();
}

void GLWidget::showTree()
{
    if(bool_show_Tree) bool_show_Tree=false;
    else bool_show_Tree=true;
    updateGL();
}

void GLWidget::InteractionAct()
{
    if(bool_show_edit) bool_show_edit=false;
    else bool_show_edit=true;
    updateGL();
}

void GLWidget::rm_spec_m_im(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  /* removes special marks */
  int   i, j;

  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
        if ( im[i][j] >= SPECIAL_MARK ) im[i][j] -= SPECIAL_MARK;
    } /* for j */
  } /* for i */

} /* earse_im */

void GLWidget::init_im(int im[MAX_ISIZE][MAX_ISIZE], int *imax, int *jmax)
{

   int	i, j;

   for (i = 0; i < MAX_ISIZE; i++){
    for (j = 0; j < MAX_ISIZE; j++){
    im[i][j] = 0;
    } //j
  } // i

  *imax = 0;
  *jmax = 0;

  geotropy_axis.w_x = 0.0;
  geotropy_axis.w_y = 0.1;

} // end init_im

void GLWidget::init_an_im(int im[MAX_ISIZE][MAX_ISIZE])
{
    // initializes an image im
    int	i, j;

    for (i = 0; i < MAX_ISIZE; i++){
        for (j = 0; j < MAX_ISIZE; j++){
            im[i][j] = 0;
        } // for j
    } // for i
} // init_an_im

void GLWidget::erase_im(int	im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  // erasres alll 1 marks in an image im
  int	i, j;

  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
    if ( im[i][j] == 1 ) im[i][j] = 0;
    } // for j
  } // for i

} // earse_im

void GLWidget::read_frain(int im[MAX_ISIZE][MAX_ISIZE], int *imax, int *jmax,const char* fname_)
{
     File *fptr;
     char dummy[STRLENGTH];
     char majic[100];
     char pbmcomment[200];
     int  i, j, k, move_up = 60;
//     int  border_i = 5, border_j = 125;
     int  border_i = 2, border_j = 2; // mine
     int  data;
     int  max_y = 0, min_y = MAX_ISIZE;

     int max_x = 0, min_x = MAX_ISIZE;


     if ( (fptr = fopen(fname_, "r")) == NULL ){
         printf("fra: error in reading file, be sure it is pbm file\n");
     }

    else{

        init_im( im, imax, jmax );

        fscanf ( fptr, "%s", majic);
        if ((majic[0]!='P')||(majic[1]!='1'))
        {
            QMessageBox::critical(0, tr("Loading File Error"), tr("Not a pbm ascii input file"));
            exit(1);
        } // if majic

        fscanf ( fptr, "%s", pbmcomment);
        if (pbmcomment[0]=='#')
        {
           fscanf ( fptr, " %[^\n]", pbmcomment);
           fscanf( fptr, "%d", jmax);
           fscanf( fptr, "%d", imax);
        }
        else
        {
           rewind(fptr);
           fscanf ( fptr, "%s", majic);
           fscanf( fptr, "%d", jmax);
           fscanf( fptr, "%d", imax);
        }

        image_x = *jmax;
        image_y = *imax;

         if ((*imax > MAX_ISIZE) || (*jmax > MAX_ISIZE)) {
            printf("fra: fatal error, pbm ascii input file too large \n");
             //exit(1);
         } // if max_isize


         for (i = 0; i < *imax; i++)
         {
            for (j = 0; j < *jmax; j++)
            {
                fscanf ( fptr, "%1d",&data);
                im[i+border_i][j + move_up + border_j] = (data==1)?1:0;
                if ( im[i][j + move_up] == 1 )
                {
                    if (j + move_up >  max_y) max_y = j + move_up;
                    if (j + move_up <  min_y) min_y = j + move_up;

                    if(i>max_x) max_x = i;
                    if(i<min_x) min_x = i;
                } // if
            } // j
        } // i

        figure_topx = min_y;
        figure_topy = min_x;
        length_object = (float)(max_y - min_y);
        figure_length_x = length_object;
        MSSR( length_object );
        float width_object = (float)(max_x - min_x);
        figure_length_y = width_object;

// mine

        *jmax += move_up;
        *jmax += (border_i + border_j);
        *imax += (border_i + border_j);

         if ((*imax > MAX_ISIZE) || (*jmax > MAX_ISIZE)) {
             printf("fra: fatal error 2, pbm ascii input file too large \n");
            //exit(1);
        } // if max_isize

         fclose(fptr);
    } // fptr != NULL

} // read_frain

void GLWidget::subtr(int a[MAX_ISIZE][MAX_ISIZE], int b[MAX_ISIZE][MAX_ISIZE],int n,int m)
{
    int i,j;

    for (i=0; i<n; i++)
        for (j=0; j<m; j++)
            b[i][j] -= a[i][j];
}


int GLWidget::t1a (int image[MAX_ISIZE][MAX_ISIZE],int i,int j, int a[8],int *b,int nn,int mm)
{
    // Return the number of 01 patterns in the sequence of pixels
    // p2 p3 p4 p5 p6 p7 p8 p9.

    int n,m;

    for (n=0; n<8; n++) a[n] = 0;
    if (i-1 >= 0) {
        a[0] = image[i-1][j];
        if (j+1 < mm) a[1] = image[i-1][j+1];
        if (j-1 >= 0) a[7] = image[i-1][j-1];
    }
    if (i+1 < nn) {
        a[4] = image[i+1][j];
        if (j+1 < mm) a[3] = image[i+1][j+1];
        if (j-1 >= 0) a[5] = image[i+1][j-1];
    }
    if (j+1 < mm) a[2] = image[i][j+1];
    if (j-1 >= 0) a[6] = image[i][j-1];

    m= 0;	*b = 0;
    for (n=0; n<7; n++) {
        if ((a[n]==0) && (a[n+1]==1)) m++;
        *b = *b + a[n];
    }
    if ((a[7] == 0) && (a[0] == 1)) m++;
    *b = *b + a[7];
    return m;

}

void GLWidget::thin_b (int object[MAX_ISIZE][MAX_ISIZE], int image[MAX_ISIZE][MAX_ISIZE], int nn, int mm)
{
    // Thinning algorithm: CACM 1984 march (Zhang and Suen)
    // object is the input image, image is the skeleton

    int i,j,n,m,k, cont, br,ar,p1,p2;
    int a[8];

    // copy object to image
    for (i=0; i<nn; i++) {
        for (j=0; j<mm; j++) {
            image[i][j] = object[i][j];
        }
    }

    cont = 1;
    while (cont) {
        fprintf (stderr,"+\n");
        cont = 0;

        //	Sub-iteration 1:
        for (i=0; i<nn; i++)
            for (j=0; j<mm; j++) {		// Scan the entire image
                if (image[i][j] == 0) {
                    im_tmp[i][j] = 0;
                    continue;
                } // if

                ar = t1a (image, i, j, a, &br,nn,mm);	// Function A
                p1 = a[0]*a[2]*a[4];
                p2 = a[2]*a[4]*a[6];
                if ( (ar == 1) && ((br>=2) && (br<=6)) &&
                    (p1 == 0) && (p2 == 0) )  {
                    im_tmp[i][j] = 1;
                    cont = 1;
                } // if
                else im_tmp[i][j] = 0;

            } // j
        subtr (im_tmp, image,nn,mm);


        // Sub iteration 2:
        for (i=0; i<nn; i++)
            for (j=0; j<mm; j++) {		// Scan the entire image
                if (image[i][j] == 0) {
                    im_tmp[i][j] = 0;
                    continue;
                }
                ar = t1a (image, i, j, a, &br,nn,mm);	// Function A
                p1 = a[0]*a[2]*a[6];
                p2 = a[0]*a[4]*a[6];
                if ( (ar == 1) && ((br>=2) && (br<=6)) &&
                    (p1 == 0) && (p2 == 0) )  {
                    im_tmp[i][j] = 1;
                    cont = 1;
                }
                else im_tmp[i][j] = 0;
            }
        subtr (im_tmp, image,nn,mm);

    } // while
 } // thin_b

void GLWidget::find_junctions(int im[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE], int imax, int jmax)
{
    // detecting the junctions in the skeleton, image im
    // contains the skeleton generated with thin_b
    // The image im contains the skeleton, the image
    // is the result with the junctions

    int   	i,j,k,cnt, tmp;
    Ij_pos	env1[MAX_ENV], env2[MAX_ENV];
    int	 	n_env1, n_env2;
    char	reqstring[STRLENGTH];

     // image junc is initialized
    init_an_im( junc );

    cnt = 0;
    for (i = 1; i < imax - 1; i++){
        for (j = 1; j < jmax - 1; j++){
            if ( im[i][j] == 1 ){
                // site i,j part of the skeleton
                n_env1 = 0;
                collect1_neighbours( im, i, j, env1, &n_env1 );
                collect2_neighbours( im, i, j, env1, &n_env1, env2, &n_env2 );

                if (n_env2 >= 3){
                    // site i,j junction in the skeleton
                    junc[i][j] = 1;
                    cnt++;
                }
            } // part of the skeleton
        } // for j
        if ( ((i  % 100) == 0) ){
            tmp = i / 10;
            sprintf( reqstring, "determining junctions %d perc done", tmp );
            printf( reqstring );
         }
    } // for i
    junction_amount = count_object(junc,imax,jmax);

} // find_junctions


// all method for segmentations (counting junctions)
void GLWidget::check_neibor(int inputimg[MAX_ISIZE][MAX_ISIZE],int h, int w, int label)
{
    int i,j;

    for(i=h-param;i<=h+param;i++)
    {
        for(j=w-param;j<=w+param;j++)
        {
            if (inputimg[i][j]==1 || inputimg[i][j]==label)
                inputimg[i][j]=label;
        }
    }
}

int GLWidget::getValue(int inputimg[MAX_ISIZE][MAX_ISIZE],int h, int w)
{
    int value=1;
    for(int c=2;c<=mark_label;c++)
        if (inputimg[h][w]==c) value = c;
    return value;
}

int GLWidget::do_segment(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w)
{
    int cnt = 0;
    int data;
    mark_label = 2;

    for(int i=param;i<h-param;i++)
    {
        for(int j=param;j<w-param;j++)
        {
             data = getValue(inputimg,i,j);
             if (inputimg[i][j]==1)
                     check_neibor(inputimg,i, j, mark_label++);
             else if(inputimg[i][j]== data)
                     check_neibor(inputimg,i, j, data);
        }
    }
    cnt = --mark_label-1;
    return cnt;
}

int GLWidget::count_object(int inputimg[MAX_ISIZE][MAX_ISIZE],int h,int w)
{
    cp_ima_to_imb(inputimg,img_temp,h,w);
    return do_segment(img_temp,h,w);
}

void GLWidget::ReducedJunc(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x)
{
    bool mark = false;
    int data;
    int icount = junction_amount;
    int markornot[icount+2];

    cp_ima_to_imb(im_junc_temp,inputimg,y,x);

    for(int t=0;t<2;t++)
        markornot[t] = -1;

    for(int t=2;t<icount+2;t++)
        markornot[t] = 1;

    for(int t=0;t<icount+2;t++)
        printf("%d ",markornot[t]);

    int max_ = -20;
    for(int i=0;i<y;i++)
    {
        for(int j=0;j<x;j++)
        {
            if(inputimg[i][j] > max_) max_ = inputimg[i][j];
        }
    }
    printf(" \n max value in im_junc_reduced : %d \n",max_);

    for(int i=0;i<y;i++)
    {
        for(int j=0;j<x;j++)
        {
             data = inputimg[i][j];
             if(markornot[data]==1)
             {
                 markornot[data] = 0;
             }
             else inputimg[i][j]=0;
        } // for j
    } // for i

    // replace data more than 1 with 1 in order to display in paintgl()
    // and also keep the junc coordinates to an array from bottom to top
    for(int i=y-1;i>=0;i--)
    {
        for(int j=0;j<x;j++)
        {
             if (inputimg[i][j] > 1)
             {
                 inputimg[i][j]=1;
             }
        } // for j
    } // for i
} // void ReducedJunc

void GLWidget::KeepJuncInArray(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x, int xstart, int ystart)
{
    count_disc_area = 0;
    for(int i=y-1;i>=0;i--)
    {
        for(int j=0;j<x;j++)
        {
             if (inputimg[i][j] ==1)
             {
                 disc_area_x[count_disc_area] = j;
                 disc_area_y[count_disc_area] = i;
                 count_disc_area++;
             } // if
        } // for j
    } // for i

} // keepJuncInArray

// Reduce cell down to 1 cell for each tip
void GLWidget::ReducedTerm(int inputimg[MAX_ISIZE][MAX_ISIZE],int y,int x,int icount)
{
    bool mark = false;
    int data;
    int markornot[icount+2];

    cp_ima_to_imb(img_temp,inputimg,y,x);

    for(int t=0;t<2;t++)
        markornot[t] = -1;

    for(int t=2;t<icount+2;t++)
        markornot[t] = 1;

    int max_ = -20;
    for(int i=0;i<y;i++)
    {
        for(int j=0;j<x;j++)
        {
            if(inputimg[i][j] > max_) max_ = inputimg[i][j];
        }
    }

    for(int i=0;i<y;i++)
    {
        for(int j=0;j<x;j++)
        {
             data = im_term1[i][j];
             if(markornot[data]==1)
             {
                markornot[data] = 0;
             }
             else im_term1[i][j]=0;
        } // for j
    } // for i
} // end

void GLWidget::add_site_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env)
{
    int	in_env;

    // site i, j is added to array env, i,j can only be present one time
    for( in_env = 0; in_env < *n_env; in_env++){
        if ( (env[in_env].i_x == i) && (env[in_env].j_y == j) ) return;
    }

    if (*n_env >= MAX_ENV)
         printf("fra: error in add_site_env");

    env[*n_env].i_x = i;
    env[*n_env].j_y = j;
    (*n_env)++;
} // add_site_env

void GLWidget::collect1_neighbours(int im[MAX_ISIZE][MAX_ISIZE], int i, int j,Ij_pos	env[MAX_ENV],int	*n_env)
{
    // first order neighbours are collected
    // note n_env is initialized externally

     if ( (i < 0) || (i >= MAX_ISIZE) || (j < 0) || (j >= MAX_ISIZE) ){
         printf("fra: error in collect1_neighbours");
        return;
    }

    if ( im[i-1][j+1] == 1 ) add_site_env( i-1, j+1, env, n_env );
        if ( im[i  ][j+1] == 1 ) add_site_env( i  , j+1, env, n_env );
            if ( im[i+1][j+1] == 1 ) add_site_env( i+1, j+1, env, n_env );
                if ( im[i+1][j  ] == 1 ) add_site_env( i+1, j  , env, n_env );
                    if ( im[i+1][j-1] == 1 ) add_site_env( i+1, j-1, env, n_env );
                        if ( im[i  ][j-1] == 1 ) add_site_env( i  , j-1, env, n_env );
                            if ( im[i-1][j-1] == 1 ) add_site_env( i-1, j-1, env, n_env );
                                if ( im[i-1][j  ] == 1 ) add_site_env( i-1, j  , env, n_env );

} // collect1_neighbours

void GLWidget::rm_site_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env)
{
    int		in_env, n_tmp_env;
    Ij_pos	tmp_env[MAX_ENV];

    n_tmp_env = 0;
    for( in_env = 0; in_env < *n_env; in_env++){
        if ( !((env[in_env].i_x == i) && (env[in_env].j_y == j)) ){
            tmp_env[n_tmp_env].i_x = env[in_env].i_x;
            tmp_env[n_tmp_env].j_y = env[in_env].j_y;
            n_tmp_env++;
        }
    }

    *n_env = n_tmp_env;
    for( in_env = 0; in_env < *n_env; in_env++){
        env[in_env].i_x = tmp_env[in_env].i_x;
        env[in_env].j_y = tmp_env[in_env].j_y;
    }
} // rm_site_env


void GLWidget::print_env(Ij_pos  env[MAX_ENV],int     *n_env)
{
    int	in_env;
    char  dummyx[STRLENGTH];

    MSSI( (*n_env) );
    for( in_env = 0; in_env < *n_env; in_env++){
        sprintf( dummyx, "(%d, %d)", env[in_env].i_x, env[in_env].j_y);
         printf( dummyx );

    }

} // print_env

bool GLWidget::test_in_env(int i, int j,Ij_pos env[MAX_ENV],int *n_env)
{
    int	in_env;
    for( in_env = 0; in_env < *n_env; in_env++){
        if ( (env[in_env].i_x == i) && (env[in_env].j_y == j) ) return( TRUE );
    }
    return( FALSE );
} // test_in_env


void GLWidget::collect2_neighbours(int im[MAX_ISIZE][MAX_ISIZE],int i_org, int j_org,Ij_pos env1[MAX_ENV],int *n_env1,Ij_pos env2[MAX_ENV],int *n_env2)
{
    // second order neighbours are collected

    int		in_env1, in_env2;
    Ij_pos	tmp_env[MAX_ENV];
    int		n_tmp_env;

    n_tmp_env = 0;
    for (in_env1 = 0; in_env1 < *n_env1; in_env1++){
        collect1_neighbours( im, env1[in_env1].i_x, env1[in_env1].j_y,
                            tmp_env, &n_tmp_env );
    }

    // neighbours of second order are determined
    *n_env2 = 0;
    for (in_env2 = 0; in_env2 < n_tmp_env; in_env2++){
        if ( !test_in_env(tmp_env[in_env2].i_x, tmp_env[in_env2].j_y,
                          env1, n_env1) &&
            !((tmp_env[in_env2].i_x == i_org) && (tmp_env[in_env2].j_y == j_org)) ){
            add_site_env( tmp_env[in_env2].i_x, tmp_env[in_env2].j_y, env2, n_env2 );
        }
    } // for in_env2

    // now the second order patches are reduced to single sites
    // by removing their first order neighbours
    in_env2 = 0;
    while (in_env2 < *n_env2){
        n_tmp_env = 0;
        collect1_neighbours( im, env2[in_env2].i_x, env2[in_env2].j_y,
                            tmp_env, &n_tmp_env );
        for (in_env1 = 0; in_env1 < n_tmp_env; in_env1++){
            rm_site_env( tmp_env[in_env1].i_x, tmp_env[in_env1].j_y,
                        env2, n_env2 );
        }
        in_env2++;
    }  //while in_env2


    // if n_env2 == 3 then i_org, j_org is a junction
    if ( (*n_env2 > 3) && FALSE ){
         printf("fra: error in collect2_neighbours ");
        MSSI( i_org );
        MSSI( j_org );
        print_env( env1, n_env1 );
        print_env( env2, n_env2 );
    }
} // collect2_neighbours

void GLWidget::find_terminals( int im[MAX_ISIZE][MAX_ISIZE], int term[MAX_ISIZE][MAX_ISIZE], int imax, int jmax)
{
    // Modification 18 Oct 02
    // detecting the terminal point in the skeleton, image im contains the skeleton generated with thin_b
    // The image im contains the skeleton, the image is the result with the junctions

    int           i,j,k,cnt, tmp;
    Ij_pos        env1[MAX_ENV], env2[MAX_ENV];
    int           n_env1, n_env2;
    char          reqstring[STRLENGTH];

    // image term is initialized
    init_an_im( term );

    cnt = 0;
    for (i = 1; i < imax - 1; i++){
        for (j = 1; j < jmax - 1; j++){
            if ( im[i][j] == 1 ){
                // site i,j part of the skeleton
                n_env1 = 0;
                collect1_neighbours( im, i, j, env1, &n_env1 );
                collect2_neighbours( im, i, j, env1, &n_env1, env2, &n_env2 );

                if (n_env2 == 1){
                    term[i][j] = 1;
                    cnt++;
                }
            } // part of the skeleton
        } // for j
        if ( ((i  % 100) == 0) ){
            tmp = i / 10;
            sprintf( reqstring, "determining terms %d perc done", tmp );
         }
    } // for i
    terminal_amount = count_object(term,imax,jmax);
 } // find_terminals

void GLWidget::contour_object(int im[MAX_ISIZE][MAX_ISIZE],int contour[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
    // determines the contour image
    int   i, j;

     // initializng the contour
    for (i = 0; i < imax; i++){
        for (j = 0; j < jmax; j++){
            contour[i][j] = 0;
        } // for j
    } // for i

    for (i = 0; i < imax; i++){
        for (j = 0; j < jmax; j++){
            if ( im[i][j] == 1 ){
                if ( (im[i-1][j+1] == 0) ||
                    (im[i  ][j+1] == 0) ||
                    (im[i+1][j+1] == 0) ||
                    (im[i+1][j  ] == 0) ||
                    (im[i+1][j-1] == 0) ||
                    (im[i  ][j-1] == 0) ||
                    (im[i-1][j-1] == 0) ||
                    (im[i-1][j  ] == 0) ){
                    // contour site
                    contour[i][j] = 1;
                } // contour site
                if ( (im[i-1][j+1] == 0) &&
                    (im[i  ][j+1] == 0) &&
                    (im[i+1][j+1] == 0) &&
                    (im[i+1][j  ] == 0) &&
                    (im[i+1][j-1] == 0) &&
                    (im[i  ][j-1] == 0) &&
                    (im[i-1][j-1] == 0) &&
                    (im[i-1][j  ] == 0) ){
                    // an isolated site
                    contour[i][j] = 0;
                }
            } // site part of object
        } // for j
    } // for i
} // contour_object

void GLWidget::det_discs(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE], int imax, int jmax)
{
    // determines the maximum discs in the objects using the
    // junctions

    int   	i, j, radius;
    // measure disc  used in det_discs
    Ij_pos  circle_buf[MAX_CIRC];
    int   n_circle_buf, nr_discs;
    bool too_large;

    // GTmssge("det_discs ent");
    printf("det_discs ent");

    nr_discs = 0;
    for (i = 1; i < imax - 1; i++)
    {
        for (j = 1; j < jmax - 1; j++)
        {
            if ( junc[i][j] == 1 )
            {
                radius = 1;
                too_large = FALSE;
                nr_discs++;
                while ( (radius < MAX_RADIUS) && !too_large )
                {
                    n_circle_buf = 0;
                    mich_circle( junc, imax, jmax, i, j, radius,circle_buf, &n_circle_buf, DISC_MARK );
                    too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
                    radius++;
                } // while
                if (radius == MAX_RADIUS)
                {
                     printf("fra: warning MAX_RADIUS in det_discs");
                }
            } // junction
        } // for j
    } // for i
    discA_amount = nr_discs;
    MSSI( nr_discs );

} // det_discs


bool GLWidget::test_disc_too_large(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos circle[MAX_CIRC],int n_circle)
{
    // A test for identifying if a site on the circle is not situated on the object
    int	in_circle;
    bool result = FALSE;

    for( in_circle = 0; in_circle < n_circle; in_circle++)
    {
        if ( (im[circle[in_circle].i_x][circle[in_circle].j_y] == 0) )
        {
            result = TRUE;
            break;
        }
    } // for in_circle
    return( result );
} // test_disc

// compare with skeleton
// updated 31-aug-12
bool GLWidget::test_disc_too_large_br(int  im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,Ij_pos circle[MAX_CIRC],int n_circle)
{

    // only check the nearest terminal
    int	in_circle;
    bool result = FALSE;

    for( in_circle = 0; in_circle < n_circle; in_circle++)
    {
        if ( (im_term[circle[in_circle].i_x][circle[in_circle].j_y] == 1) )
        {
            result = TRUE;
            break;
        }
    } // for in_circle
    return( result );

} // test_disc_too_large_br

void GLWidget::mich_circle(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,int imx, int imy, int radius,Ij_pos circle[MAX_CIRC],int *n_circle, int m_type)
{
    // after Foley and Van Dam, 1984
    // draws a half circle

    int   ix, iy, d, i;
    bool	error = FALSE;

    // n_circle_buf = 0;
    ix = 0;
    iy = (int)radius;
    d  = 3 - 2 * (int)radius;
    while ( (ix < iy) && (!error) )
    {
        half_circle_points( im, imax, jmax, imx, imy, ix, iy, circle, n_circle, m_type, &error );
        if (d < 0)
        {
            // select S
            d = d + 4 * ix + 6;
        }
        else
        {
            // select T
            d = d + 4 * (ix - iy) + 10;
            iy--;
        }
        ix++;
    } // while
    if (ix == iy)
    {
        half_circle_points( im, imax, jmax, imx, imy, ix, iy, circle, n_circle,
                           m_type, &error );
    }

} // mich_circle

void GLWidget::half_circle_points(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,int imx, int imy, int ix, int iy,Ij_pos circle[MAX_CIRC],int *n_circle, int m_type,bool	*error)
{
    // after Foley and Van Dam, 1984
    // "draws" only the upper half of the circle
    // imx, imy is middle point
     if ((imx +ix >= MAX_ISIZE) || (imx +iy >= MAX_ISIZE) ||
        (imx -ix >= MAX_ISIZE) || (imx -iy >= MAX_ISIZE) ||
        (imy +ix >= MAX_ISIZE) || (imy +iy >= MAX_ISIZE) ||
        (imy -ix >= MAX_ISIZE) || (imy -iy >= MAX_ISIZE) )
        {
            *error = TRUE;
            return;
        } // if


    if (m_type != EMPTY_MARK){
        im[imx +ix   ][imy +iy] = m_type;
        im[imx +iy   ][imy +ix] = m_type;
        im[imx +ix   ][imy -iy] = m_type;
        im[imx +iy   ][imy -ix] = m_type;

        im[imx -ix   ][imy +iy] = m_type;
        im[imx -iy   ][imy +ix] = m_type;
        im[imx -ix   ][imy -iy] = m_type;
        im[imx -iy   ][imy -ix] = m_type;
    }

    add_site_circle( imx +ix, imy +iy, circle, n_circle );
    add_site_circle( imx +iy, imy +ix, circle, n_circle );
    add_site_circle( imx +ix, imy -iy, circle, n_circle );
    add_site_circle( imx +iy, imy -ix, circle, n_circle );

    add_site_circle( imx -ix, imy +iy, circle, n_circle );
    add_site_circle( imx -iy, imy +ix, circle, n_circle );
    add_site_circle( imx -ix, imy -iy, circle, n_circle );
    add_site_circle( imx -iy, imy -ix, circle, n_circle );
} // half_circle_points

void GLWidget::add_site_circle(int i, int j,Ij_pos circle[MAX_CIRC],int *n_circle)
{
    int   in_circle;

    if (*n_circle >= MAX_CIRC)
        printf("fra: error in add_site_circle");

    circle[*n_circle].i_x = i;
    circle[*n_circle].j_y = j;
    (*n_circle)++;
} // add_site_circle

void GLWidget::rm_site_circle(int i, int j,Ij_pos circle[MAX_CIRC],int *n_circle)
{
    int           in_circle, n_tmp_circle;
    Ij_pos        tmp_circle[MAX_CIRC];

    n_tmp_circle = 0;
    for( in_circle = 0; in_circle< *n_circle; in_circle++){
        if ( !((circle[in_circle].i_x == i) && (circle[in_circle].j_y == j)) ){
            tmp_circle[n_tmp_circle].i_x = circle[in_circle].i_x;
            tmp_circle[n_tmp_circle].j_y = circle[in_circle].j_y;
            n_tmp_circle++;
        }
    }

    *n_circle= n_tmp_circle;
    for( in_circle = 0; in_circle < *n_circle; in_circle++){
        circle[in_circle].i_x = tmp_circle[in_circle].i_x;
        circle[in_circle].j_y = tmp_circle[in_circle].j_y;
    }
} // rm_site_circle



void GLWidget::show_geotropy_axis()
{
  Wc	wc[2], wctmp;

  wc[1].w_x = wc[0].w_x = 0.2;
  wc[1].w_y = wc[0].w_y = 0.8;
  wc[1].w_x += geotropy_axis.w_x;
  wc[1].w_y += geotropy_axis.w_y;
  wctmp.w_x = wc[1].w_x;
  wctmp.w_y = wc[1].w_y;

} // show_geotropy_axis



void GLWidget::revisit_max_disc(int skeleton[MAX_ISIZE][MAX_ISIZE], int	contour[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, Medge result[MAX_RES], int *n_result, char label[STRLENGTH])
{
  // The discs or other results in the array result are revisited
  // for each result it is considered interactively if it is alright or should be skipped

  Wc            wcs[3], wc[2], wctmp;
  Real  	size  = (Real)MAX_ISIZE;
  Ij_pos        circle_buf[MAX_CIRC];
  int           n_circle_buf, radius;
  int 		max_r, in_result, i_ct, j_ct;
  Int		choice;
  Medge		new_result[MAX_RES];
  int		new_n_result;
  char      reqstring[STRLENGTH];
  Wc		wcx;

  sprintf( reqstring, "revisit %s measurements?", label );
  choice = 1;
  if (choice == 2){
    //GTmssge("revisit_max_disc left");
    return;
  }

  new_n_result = 0;
  for (in_result = 0; in_result < *n_result; in_result++){
    if ( strcmp(label, "rb") == 0 ) {
      // the measurement is an edge
      wc[0].w_x = (Real)result[in_result].p_i_ct / size;
      wc[0].w_y = (Real)result[in_result].p_j_ct / size;
      wc[1].w_x = (Real)result[in_result].i_ct / size;
      wc[1].w_y = (Real)result[in_result].j_ct / size;
      choice =1;
      if (choice == 1){
      } // choice == 1
    } // rb
    else if ( (strcmp(label, "geotropy") == 0) ||
    (strcmp(label, "term_geotropy") == 0) ) {
      // the measurement is a vector
      wc[0].w_x = (Real)result[in_result].p_i_ct / size;
      wc[0].w_y = (Real)result[in_result].p_j_ct / size;
      wc[1].w_x = (Real)result[in_result].i_ct / size;
      wc[1].w_y = (Real)result[in_result].j_ct / size;
      //GTset_col_pf( WHITE );
      //GTpolyline2(2, wc);

      wctmp.w_x = wc[1].w_x;
      wctmp.w_y = wc[1].w_y;

      choice = 1;
      if (choice == 1){
      } // choice == 1
    } // geotropy
    else if ( strcmp(label, "angle") == 0 ) {
      // the measurement is an angle b0, a, b1
      wcs[0].w_x = (Real)result[in_result].i_ct / size;
      wcs[0].w_y = (Real)result[in_result].j_ct / size;
      wcs[1].w_x = (Real)result[in_result].p_i_ct / size;
      wcs[1].w_y = (Real)result[in_result].p_j_ct / size;
      wcs[2].w_x = (Real)result[in_result].i1_ct / size;
      wcs[2].w_y = (Real)result[in_result].j1_ct / size;
      choice = 1;
      if (choice == 1){
        // edge is displayed again and copied
        //GTset_col_pf( SKEL_COLOUR );
        //GTpolyline2( 3, wcs );
      } // choice == 1
    } // angle
    else{
      // the measurement is the radius of a max disc

      // The center of  the array of results is taken
      i_ct = result[in_result].p_i_ct;
      j_ct = result[in_result].p_j_ct;
      max_r = result[in_result].meas;

      radius = 1;
      while ( (radius <= max_r) && (radius < MAX_RADIUS) ){
      //while ( (radius < max_r) && (radius < MAX_RADIUS) ){
        n_circle_buf = 0;
        // the sites on the outer circle are not marked
        mich_circle( contour, imax, jmax, i_ct, j_ct, radius,circle_buf, &n_circle_buf, EMPTY_MARK );
        //show_circle( contour, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,WHITE, FALSE );
        if ( (radius == max_r) || ((strcmp(label, "da") != 0)  &&
        (strcmp(label, "br_spacing") != 0)) ){
          show_circle( contour, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,WHITE, FALSE );
        }
// Nov 98
        show_circle( contour, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,WHITE, FALSE );
        radius++;
      } //while
      choice = 1; // my code
      if ( (choice == 1) && FALSE ){
        // disc is displayed again
        radius = 1;
        while ( (radius < max_r) && (radius < MAX_RADIUS) ){
          n_circle_buf = 0;
          // the sites on the outer circle are not marked
          mich_circle( contour, imax, jmax, i_ct, j_ct, radius,circle_buf, &n_circle_buf, EMPTY_MARK );
          show_circle( contour, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,SKEL_COLOUR, FALSE );
          radius++;
        } // while
      } // choice == 1

      if (radius == MAX_RADIUS){
        //GTmssge("fra: warning MAX_RADIUS in revisit_max_disc");
      }
    } // !rb and !geotropy

    // when the measurement is accepted it is copied
    choice = 1;
    if (choice == 1){
      new_result[new_n_result].meas = result[in_result].meas;
      new_result[new_n_result].p_i_ct = result[in_result].p_i_ct;
      new_result[new_n_result].p_j_ct = result[in_result].p_j_ct;
      new_result[new_n_result].i_ct = result[in_result].i_ct;
      new_result[new_n_result].j_ct = result[in_result].j_ct;
      new_result[new_n_result].i1_ct = result[in_result].i1_ct;
      new_result[new_n_result].j1_ct = result[in_result].j1_ct;
      new_n_result++;
    } // choice == 1
  } // for in_result

  // the result array is changed a copy of the array of accepted
  // measurements is made
  *n_result =  new_n_result;
  for (in_result = 0; in_result < *n_result; in_result++){
    result[in_result].meas = new_result[in_result].meas;
    result[in_result].p_i_ct = new_result[in_result].p_i_ct;
    result[in_result].p_j_ct = new_result[in_result].p_j_ct;
    result[in_result].i_ct = new_result[in_result].i_ct;
    result[in_result].j_ct = new_result[in_result].j_ct;
    result[in_result].i1_ct = new_result[in_result].i1_ct;
    result[in_result].j1_ct = new_result[in_result].j1_ct;
  } // for in_result

} // revisit_max_disc



void GLWidget::show_circle(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, Ij_pos circle[MAX_CIRC],int n_circle, int m_type,Int colour,bool erase)
{
  // Procedure for displaying the test circle neighbouring sites in the state 1 are erased in order to
  // avoid many other test points within the disc erasing should be avoided in the outer circle

  int	in_circle;
  Real  size  = (Real)MAX_ISIZE;
  Wc		  wc;

  for( in_circle = 0; in_circle < n_circle; in_circle++){
    if ( (m_type == DISC_MARK) || (m_type == DISC_MARK + SPECIAL_MARK) ||
    ((m_type == 0) && (colour == BACKGROUND)) ){
      wc.w_y = (Real)circle[in_circle].j_y / size;
      wc.w_x = (Real)circle[in_circle].i_x / size;
    }
    else if ( (m_type == DISC_MARK) || (m_type == DISC_MARK + SPECIAL_MARK) ||
    ((m_type == OBJ_COLOUR) && (colour == OBJ_COLOUR)) ){
      wc.w_y = (float)circle[in_circle].j_y / size;
      wc.w_x = (float)circle[in_circle].i_x / size;
    }

    if ( (erase) && (m_type == BACKGROUND) ){
      printf("erasing exterior\n");
    //erase neighbours
    // May 2002 or fill holes
      if ( im[circle[in_circle].i_x -1][circle[in_circle].j_y   ] == 1 )
        im[circle[in_circle].i_x -1][circle[in_circle].j_y   ] = 0;
      if ( im[circle[in_circle].i_x +1][circle[in_circle].j_y   ] == 1 )
        im[circle[in_circle].i_x +1][circle[in_circle].j_y   ] = 0;
      if ( im[circle[in_circle].i_x   ][circle[in_circle].j_y +1] == 1 )
        im[circle[in_circle].i_x   ][circle[in_circle].j_y +1] = 0;
      if ( im[circle[in_circle].i_x   ][circle[in_circle].j_y -1] == 1 )
        im[circle[in_circle].i_x   ][circle[in_circle].j_y -1] = 0;
    } // erase
    else if ((erase) && (m_type == OBJ_COLOUR)){
      printf("erasing interior\n");
      if ( im[circle[in_circle].i_x -1][circle[in_circle].j_y   ] == OBJ_COLOUR)
        im[circle[in_circle].i_x -1][circle[in_circle].j_y   ] = 1;
      if ( im[circle[in_circle].i_x +1][circle[in_circle].j_y   ] == OBJ_COLOUR)
        im[circle[in_circle].i_x +1][circle[in_circle].j_y   ] = 1;
      if ( im[circle[in_circle].i_x   ][circle[in_circle].j_y +1] == OBJ_COLOUR)
        im[circle[in_circle].i_x   ][circle[in_circle].j_y +1] = 1;
      if ( im[circle[in_circle].i_x   ][circle[in_circle].j_y -1] == OBJ_COLOUR)
        im[circle[in_circle].i_x   ][circle[in_circle].j_y -1] = 1;
    } // fill holes
  } // for in_circle
} // show_circle

void GLWidget::init_res()
{
  length_object = 0;
  mult_factor = 1.0;
  n_da_discs = 0;
  n_db_discs = 0;
  n_dc_discs = 0;
  n_rb = 0;
  n_geotropy = 0;
  term_n_geotropy = 0;
  n_logeps_box = 0;
  n_logneps_box = 0;
  n_angle = 0;
  // addition 3-9-97 measuring branch spacing
  n_br_spacing = 0;

  //GTmssge("init_res left");
} // init_res

void GLWidget::clean_image(int object[MAX_ISIZE][MAX_ISIZE],int imax, int jmax, bool exterior)
{
  // removes ``dust'' from the image by drawing empty circles
  // May 2002: cleans exterior if exterior == TRUE, otherwise it fills holes in the object

  int           i, j, radius, i_pos, j_pos, size_radius;
  // measure disc  used in det_discs
  Ij_pos          circle_buf[MAX_CIRC];
  int             n_circle_buf, nr_discs, m_type;
  Bool            too_large;
  Real  	  size  = (Real)MAX_ISIZE;
  Wc		  wc, wc2[2], wc3[2];
  Int		  choice;
  Bool		  erase;

  //GTmssge("clean image");

  if (exterior)
      printf("clean exterior of the image?");
  else printf("fill holes of the image?");
  choice = 2;
  if (choice == 1) return;

    if (exterior) m_type = BACKGROUND;
    else m_type = OBJ_COLOUR;
    i_pos = (int)(size * wc.w_x);
    j_pos = (int)(size * wc.w_y);
    if ( (i_pos < 0) || (j_pos < 0) ||
    (i_pos > MAX_ISIZE) || (j_pos > MAX_ISIZE) ){
      //GTmssge("fra: error in ifollow_track");
      return;
    }
    show_cross( i_pos, j_pos, CROSS_COLOR, NORMAL_SIZE );

    switch(choice){
      case 1: // stop
      case 2: // small disc
        size_radius = 10;
        break;
      case 3: // normal disc
        size_radius = 20;
        break;
      case 4: // huge disc
        size_radius = 50;
        break;
    } // switch

    radius = 1;
    while ( (radius < size_radius) && (choice != 1) ){
      n_circle_buf = 0;
      mich_circle( object, imax, jmax, i_pos, j_pos, radius,circle_buf, &n_circle_buf, m_type );
      erase = TRUE;
      if (exterior){
        show_circle( object, imax, jmax, circle_buf, n_circle_buf, m_type,
        BACKGROUND, erase );
      }
      else{
        show_circle( object, imax, jmax, circle_buf, n_circle_buf, m_type,
        OBJ_COLOUR, erase );
      }
      radius++;
    } // while
} // clean_image

void GLWidget::show_cross(int i, int j,int colour,Real move)
{
  qglColor(colorname[colour]);
  glLineWidth(2.0);
  glBegin(GL_LINES);
   glVertex2i(i-5+x_pos_start,j-5+y_pos_start);
   glVertex2i(i+5+x_pos_start,j+5+y_pos_start);
  glEnd();

  glBegin(GL_LINES);
   glVertex2i(i+5+x_pos_start,j-5+y_pos_start);
   glVertex2i(i-5+x_pos_start,j+5+y_pos_start);
  glEnd();

} // show_cross

void GLWidget::change_geotropy_axis()
{
  Int	choice;

  show_geotropy_axis();
  choice = 2;
  switch(choice){
    case 1: // ino changes
      break;
    case 2:
      geotropy_axis.w_x =  0.0;
      geotropy_axis.w_y =  0.1;
      break;
    case 3:
      geotropy_axis.w_x =  0.0;
      geotropy_axis.w_y = -0.1;
      break;
    case 4:
      geotropy_axis.w_x =  0.1;
      geotropy_axis.w_y =  0.0;
      break;
    case 5:
      geotropy_axis.w_x = -0.1;
      geotropy_axis.w_y =  0.0;
      break;
   }

   show_geotropy_axis();
} // change_geotropy_axis

void GLWidget::show_max_skel_length()
{
  Real  size  = (Real)MAX_ISIZE;
  char  xwstype[STRLENGTH];
  Bool  gl_ws = FALSE;
  int	i;
  Wc	wc;

  if ( strcmp(xwstype, "GL") == 0 ) gl_ws = TRUE;
  for (i = 0; i < max_skel_length; i++){
    if ( gl_ws ){
      //GTpixel2( i, MAX_ISIZE - MAX_SKEL_LENGTH );
    }
    else{
      wc.w_y = (Real)(MAX_ISIZE - MAX_SKEL_LENGTH) / size;
      wc.w_x = (Real)i / size;
    }
  } // for i
} // show_max_skel_length

void GLWidget::change_max_skel_length()
{
  Int	choice;
  Real	value;

  show_max_skel_length();
  choice = 2;
  if (choice == 2){
    value = 200; // my input
    max_skel_length = (int)value;
    MSSI( max_skel_length );
    show_max_skel_length();
  }
} // max_skel_length

void GLWidget::det_box_dim(int contour[MAX_ISIZE][MAX_ISIZE], int imax, int jmax)
{
  // Estimating the box dimension
  // In the first estimation the MAX_ISIZE * MAX_ISIZE image is
  // subdivided NSUBDIV * NSUBDIV boxes. In each following estimation
  // the edges of each box are subdivided again into two until the
  // NREST boxes are obtained
  // The bin list starts numbering at minx,miny
  // The end of the first row is mini, maxj

  int	i, j, inrest, nbox, inbox;
  int	nsubdiv, box_index, nepsilon;
  Box	*box_list;
  Real  size  = (Real)MAX_ISIZE, size_box, epsilon, estimation;
  Real	tmp;
  //double  log10();
  char	  reqstring[STRLENGTH];
  Real	x_slope[NSUBDIV_NRS];
  Real	y_slope[NSUBDIV_NRS];
  int	nslope;

 // printf("\n---- dim box ------\n");
  nslope = 0;
  for (inrest = 0; inrest < NSUBDIV_NRS; inrest++)
  {

    nsubdiv = subdiv_nrs[inrest];
    nbox = nsubdiv * nsubdiv;
    //allocation of the box_list
    CALLOCJ( box_list, nbox, Box );

    // initializing the counters of the box list
    for (inbox = 0; inbox < nbox; inbox++){
      (box_list + inbox)->npts = 0;
    } // for inbox

    // dimensions of a box are deteremined
    epsilon = size_box = size / (Real)nsubdiv;

    // counting the number of elements in each box
    for (i = 0; i < MAX_ISIZE; i++){
      for (j = 0; j < MAX_ISIZE; j++){
        if ( contour[i][j] == 1 )
        {
          box_index = (int)((Real)j / size_box) * nsubdiv + (int)((Real)i / size_box);
          if ( box_index >= nbox )
          {
            box_index = nbox - 1;
          }
          (box_list + box_index)->npts++;
        } // site part of contour
      } // for j
    } // for i

    // count the nr of boxes in which a site of the contour is present
    nepsilon = 0;
    for (inbox = 0; inbox < nbox; inbox++){
      if ( (box_list + inbox)->npts != 0 ) nepsilon++;
    } // for inbox

    tmp = (Real)log10( (double) epsilon );
    if ( (inrest >= START_SLOPE) && (inrest <= END_SLOPE) )x_slope[nslope]=tmp;
    add_result( tmp, logeps_box, &n_logeps_box, 0, 0, 0, 0, 0, 0  );
    tmp = (Real)log10( (double) nepsilon );
    if ( (inrest >= START_SLOPE) && (inrest <= END_SLOPE) ){
      y_slope[nslope] = tmp;
      nslope++;
    }
    add_result( tmp, logneps_box, &n_logneps_box, 0, 0, 0, 0, 0, 0 );
    tmp = (Real)inrest / (Real)NSUBDIV_NRS * 100.0;
    sprintf( reqstring, "determining D %f perc done", tmp );
    // visualizes the state of the boxes
    if ((inrest % 8) == 0)
        show_boxes( nbox, box_list, size_box, nsubdiv );

    // freeing the box_list
    FREEJ( box_list );
  } // in_rest
  fractalDim = -(regres_coef( nslope, x_slope, y_slope, &box_dimension ));

} // det_box_dim

// my version of add_result
void GLWidget::add_result(Real result,Medge res[MAX_RES],int *n_res,int p_i_ct,int p_j_ct,int i_ct,int j_ct,int i1_ct, int j1_ct)
{
  // a measurement is added to an array with results
  // The last six  arguments represent the coordinates
  // of three centers of a maximum disc, with these it is
  // for example possible to recontruct the original ed
  // on which the measurement is based
  // With three centers an angle is represented
  // When i_ct and j_ct equal zero than only one center
  // is relevant, if all four are zero only the measu
  // is relevant

  if (*n_res >= MAX_RES)
  {
      printf("fra: error in add_result : MAX_RES variable is too small");
      return;
  }
  for(int i=0;i<*n_res;i++)
  {
      if((res[i].p_i_ct==p_i_ct) && (res[i].p_j_ct==p_j_ct)
          && (res[i].i_ct==i_ct) && (res[i].j_ct==j_ct))
      {
          return;
      }
      if((res[i].p_i_ct==i_ct) && (res[i].p_j_ct==j_ct)
          && (res[i].i_ct==p_i_ct) && (res[i].j_ct==p_j_ct))
      {
          return;
      }
      if((abs(res[i].p_i_ct-p_i_ct)<=2) && (abs(res[i].p_j_ct-p_j_ct)<=2))
      {
          return;
      }

  }
  if((i_ct==p_i_ct) && (j_ct==p_j_ct))
  {
      return;
  }

  res[*n_res].meas = result;
  res[*n_res].p_i_ct = p_i_ct;
  res[*n_res].p_j_ct = p_j_ct;
  res[*n_res].i_ct = i_ct;
  res[*n_res].j_ct = j_ct;
  res[*n_res].i1_ct = i1_ct;
  res[*n_res].j1_ct = j1_ct;
  (*n_res)++;
}  //add_result

void GLWidget::show_boxes(int nbox,Box *box_list,Real size_box,int nsubdiv)
{
  /* visualizes the state of the boxes */

  int 	i, j, box_index, inbox, isize_box = (int)size_box;
  Real  size  = (Real)MAX_ISIZE;
  Wc	wc;
  char  xwstype[STRLENGTH];
  Bool  gl_ws = FALSE;

  MSSI( nbox );
  MSSR( size_box );
  MSSI( nsubdiv );

  if (isize_box == 0){
    return;
  }

  if ( strcmp(xwstype, "GL") == 0 ) gl_ws = TRUE;

  for (i = 0; i < MAX_ISIZE; i++){
    for (j = 0; j < MAX_ISIZE; j++){
      box_index =
      (int)((Real)j / size_box) * nsubdiv + (int)((Real)i / size_box);
      if ( box_index >= nbox ){
         box_index = nbox - 1;
      }

      if ( (box_list + box_index)->npts != 0 ){
        if ( ((i % isize_box) == 0) || ((j % isize_box) == 0) ){
          if ( gl_ws ){
            //GTpixel2( i, j );
          }
          else {
            wc.w_y = (Real)j / size;
            wc.w_x = (Real)i / size;
          }
        }
      } /* box not empty */
    } /* for j */
  } /* for i */
} /* show_boxes */

Real GLWidget::regres_coef(int	n_val,Real	*x_values,Real *y_values,Real *coefficient)
{
  /* This procedure is derived from the procedure test_regres_coef         */
  /* determination of the regression coefficient b (after Inroductory Math.*/
  /* Statistics-  E. Kreyszig, 1970 ). (section 17.7)                      */
  /* case s_dev for x-values and y-values is zero                          */
  Int	i, n;
  Real	min_x, min_y;
  Real	mean_x, mean_y;
  Real	sum_x_star, sum_sqr_x_star;
  Real	sum_x_star_y_star;
  Real	sum_y_star, sum_sqr_y_star;
  Real	s_sqr_x, s_sqr_y, s_xy;
  Real	b, c, q, t0, tmp;

  /* searching minimum x value */
  min_x = 999.9;
  for (i = 0; i < n_val; i++){
    if ( x_values[i] < min_x ) min_x = x_values[i];
  } /* for i */
  MSSR( min_x );

  /* searching minimum y value */
  min_y = 999.9;
  for (i = 0; i < n_val; i++){
    if ( y_values[i] < min_y ) min_y = y_values[i];
  } /* for i */
  MSSR( min_y );

  /* x-values are coded */
  for (i = 0; i < n_val; i++){
    x_values[i] -= min_x;
  } /* for i */

  /* y-values are coded */
  for (i = 0; i < n_val; i++){
    y_values[i] -= min_y;
  } /* for i */

  /* calculation sum_x_star */
  sum_x_star = 0.0;
  for (i = 0; i < n_val; i++){
    sum_x_star += x_values[i];
  } /* for i */
  MSSR( sum_x_star );

  /* calculation sum_y_star */
  sum_y_star = 0.0;
  for (i = 0; i < n_val; i++){
    sum_y_star += y_values[i];
  } /* for i */
  MSSR( sum_y_star );

  /* calculation sum_sqr_x_star */
  sum_sqr_x_star = 0.0;
  for (i = 0; i < n_val; i++){
    sum_sqr_x_star += SQR( x_values[i] );
  } /* for i */
  MSSR( sum_sqr_x_star );

  /* calculation sum_sqr_y_star */
  sum_sqr_y_star = 0.0;
  for (i = 0; i < n_val; i++){
    sum_sqr_y_star += SQR( y_values[i] );
  } /* for i */
  MSSR( sum_sqr_y_star );

  /* calculation sum_x_star_y_star */
  sum_x_star_y_star = 0.0;
  for (i = 0; i < n_val; i++){
    sum_x_star_y_star += y_values[i] * x_values[i];
  } /* for i */
  MSSR( sum_x_star_y_star );

  /* calulation mean_x */
  mean_x = sum_x_star / (Real)n_val + min_x;
  MSSR( mean_x );

  /* calulation mean_y */
  mean_y = sum_y_star / (Real)n_val + min_y;
  MSSR( mean_y );

  /* calculation s_sqr_x */
  s_sqr_x = (1.0 / ((Real)n_val - 1.0)) *
  (sum_sqr_x_star - (1.0 / (Real)n_val) * SQR(sum_x_star));
  MSSR( s_sqr_x );

  /* calculation s_sqr_y */
  s_sqr_y = (1.0 / ((Real)n_val - 1.0)) *
  (sum_sqr_y_star - (1.0 / (Real)n_val) * SQR(sum_y_star));
  MSSR( s_sqr_y );

  /* calculation s_sqr_xy */
  s_xy = (1.0 / ((Real)n_val - 1.0)) *
  (sum_x_star_y_star  - (1.0 / (Real)n_val) * (sum_x_star * sum_y_star));
  MSSR( s_xy );

  /* calculation b */
  b = s_xy / s_sqr_x;
  MSSR( b );

  *coefficient = b;
  return b;
} /* regres_coef */


void GLWidget::dump_res(const char* savedname)
{
  // dumps all results on the file fra.res

  File          *fptr;
  char  	name[STRLENGTH];
  int		i;
  float     figY,FigX,PictA,PictCon;

  const char* currentdate = "24-July-12";
  QString date = QDate::currentDate().toString("dd-MMM-yyyy");
  //currentdate = date.toUtf8().constData();
  if ( (fptr = fopen(savedname, "w")) == NULL){
      printf("fra: error can't create file .res");
  }
  else{ // writing all results on fra.res
    fprintf( fptr, "Filename : %s\n", savedname );
    fprintf( fptr, "Version : 2012 \n");
    fprintf( fptr, "Date of Record : %s \n",date.toUtf8().constData());
    fprintf( fptr, "Image Filename : %s \n", onlyfilename.toUtf8().constData());
    fprintf( fptr, "The height of Image : %.2f pixels (%.2f mm) \n",figure_length_y,figure_length_y/ppmm);
    fprintf( fptr, "The width of Image: %.2f pixels (%.2f mm) \n",figure_length_x,figure_length_x/ppmm);
    fprintf( fptr, "Image Area: %.2f mm\n",PictureAreaMM );
    fprintf( fptr, "Image Contour Length: %.2f mm\n",ContourSize/ppmm );
    fprintf( fptr, "Fractal Dimension : %.2f \n", fractalDim );

    fprintf( fptr, "\nThe number of da_discs: %d\n",n_da_discs);
    for (i = 0; i < n_da_discs; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", 2*(da_discs[i].meas/ppmm));
      fprintf( fptr, "%d ", da_discs[i].p_i_ct );
      fprintf( fptr, "%d ", da_discs[i].p_j_ct );
      fprintf( fptr, "%d ", da_discs[i].i_ct );
      fprintf( fptr, "%d ", da_discs[i].j_ct );
      fprintf( fptr, "%d ", da_discs[i].i1_ct );
      fprintf( fptr, "%d\n", da_discs[i].j1_ct );
    }

    fprintf( fptr, "\nThe number of db_discs: %d\n",n_db_discs);
    for (i = 0; i < n_db_discs; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", 2*(db_discs[i].meas/ppmm));
      fprintf( fptr, "%d ", db_discs[i].p_i_ct );
      fprintf( fptr, "%d ", db_discs[i].p_j_ct );
      fprintf( fptr, "%d ", db_discs[i].i_ct );
      fprintf( fptr, "%d ", db_discs[i].j_ct );
      fprintf( fptr, "%d ", db_discs[i].i1_ct );
      fprintf( fptr, "%d\n", db_discs[i].j1_ct );
    }

    fprintf( fptr, "\nThe number of dc_discs: %d\n",n_dc_discs);
    for (i = 0; i < n_dc_discs; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", 2*(dc_discs[i].meas/ppmm));
      fprintf( fptr, "%d ", dc_discs[i].p_i_ct );
      fprintf( fptr, "%d ", dc_discs[i].p_j_ct );
      fprintf( fptr, "%d ", dc_discs[i].i_ct );
      fprintf( fptr, "%d ", dc_discs[i].j_ct );
      fprintf( fptr, "%d ", dc_discs[i].i1_ct );
      fprintf( fptr, "%d\n", dc_discs[i].j1_ct );
    }

    if(bool_correctrb) // repair branch was selected
    {
        fprintf( fptr, "\nThe number of branches(euclidean distance): %d\n",WRRB_count_BeforeTreeCon);
        for (i = 0; i < WRRB_count_BeforeTreeCon; i++)
        {
          fprintf( fptr, "%d) ", (i+1) );
          fprintf( fptr, "%.2f ", WithoutRedundantRB_BeforeTreeCon[i].meas/ppmm);
          fprintf( fptr, "%d ", WithoutRedundantRB_BeforeTreeCon[i].p_i_ct );
          fprintf( fptr, "%d ", WithoutRedundantRB_BeforeTreeCon[i].p_j_ct );
          fprintf( fptr, "%d ", WithoutRedundantRB_BeforeTreeCon[i].i_ct );
          fprintf( fptr, "%d ", WithoutRedundantRB_BeforeTreeCon[i].j_ct );
          fprintf( fptr, "%d ", WithoutRedundantRB_BeforeTreeCon[i].i1_ct );
          fprintf( fptr, "%d\n", WithoutRedundantRB_BeforeTreeCon[i].j1_ct );
        }
    }
    else // use rb
    {
        fprintf( fptr, "\nThe number of branches(euclidean distance): %d\n",n_rb);
        for (i = 0; i < n_rb; i++)
        {
          fprintf( fptr, "%d) ", (i+1) );
          fprintf( fptr, "%.2f ", rb[i].meas/ppmm);
          fprintf( fptr, "%d ", rb[i].p_i_ct );
          fprintf( fptr, "%d ", rb[i].p_j_ct );
          fprintf( fptr, "%d ", rb[i].i_ct );
          fprintf( fptr, "%d ", rb[i].j_ct );
          fprintf( fptr, "%d ", rb[i].i1_ct );
          fprintf( fptr, "%d\n", rb[i].j1_ct );
        } // for
    } // else bool_correctrb

    // Ultimate branch
    fprintf( fptr, "\nThe number of ultimate branches(euclidean distance): %d\n",BOrder1_Count);
    for (i = 0; i < BOrder1_Count; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", Branch_Order[0][i].meas);
      fprintf( fptr, "%d ", Branch_Order[0][i].p_i_ct );
      fprintf( fptr, "%d ", Branch_Order[0][i].p_j_ct );
      fprintf( fptr, "%d ", Branch_Order[0][i].i_ct );
      fprintf( fptr, "%d ", Branch_Order[0][i].j_ct );
      fprintf( fptr, "%d ", Branch_Order[0][i].i1_ct );
      fprintf( fptr, "%d\n", Branch_Order[0][i].j1_ct );
    }

    int cft=0;
    fprintf( fptr, "\nThe number of branches (pixels): %d\n",count_FLengthTrim);
    for (i = 0; i < count_maxskel; i++)
    {
      if(FLengthTrim[i][0]!=0)
      {
          cft++;
          fprintf( fptr, "%d) ", cft );
          fprintf( fptr, "%d ", FLengthTrim[i][5] );
          fprintf( fptr, "%.2f ", FLengthTrim[i][4]/ppmm );
          fprintf( fptr, "%d ", FLengthTrim[i][0] );
          fprintf( fptr, "%d ", FLengthTrim[i][1] );
          fprintf( fptr, "%d ", FLengthTrim[i][2] );
          fprintf( fptr, "%d\n", FLengthTrim[i][3] );
      } // if
    } // for


    fprintf( fptr, "\nThe number of disc A angles: %d\n",term_n_geotropy);
    for (i = 0; i < term_n_geotropy; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ",term_geotropy[i].meas);
      fprintf( fptr, "%d ", term_geotropy[i].p_i_ct );
      fprintf( fptr, "%d ", term_geotropy[i].p_j_ct );
      fprintf( fptr, "%d ", term_geotropy[i].i_ct );
      fprintf( fptr, "%d ", term_geotropy[i].j_ct );
      fprintf( fptr, "%d ", term_geotropy[i].i1_ct );
      fprintf( fptr, "%d\n", term_geotropy[i].j1_ct );
    }

    fprintf( fptr, "\nThe number of disc B angles: %d\n",n_geotropy);
    for (i = 0; i < n_geotropy; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", geotropy[i].meas );
      fprintf( fptr, "%d ", geotropy[i].p_i_ct );
      fprintf( fptr, "%d ", geotropy[i].p_j_ct );
      fprintf( fptr, "%d ", geotropy[i].i_ct );
      fprintf( fptr, "%d ", geotropy[i].j_ct );
      fprintf( fptr, "%d ", geotropy[i].i1_ct );
      fprintf( fptr, "%d\n", geotropy[i].j1_ct );
    }
    // branch angle
    fprintf( fptr, "\n Detail of Branch Angle \n");
    for(i=0;i<MAXJUNC;i++)
    {
        if(AngleResult[i][0]>0)
        {
            fprintf( fptr, "%d) ", (i+1) );
            for(int j=0;j<ANGLEATT;j++)
            {
                if(AngleResult[i][j]!=0)
                {
                    fprintf( fptr, "%.2f ", AngleResult[i][j]);
                }
            } // for j
            fprintf( fptr, "\n");
        }
    } // for i

    fprintf( fptr, "\nThe number of branching space: %d\n",n_br_spacing);
    for (i = 0;i < n_br_spacing; i++)
    {
      fprintf( fptr, "%d) ", (i+1) );
      fprintf( fptr, "%.2f ", 2*(br_spacing[i].meas/ppmm));
      fprintf( fptr, "%d ",br_spacing[i].p_i_ct );
      fprintf( fptr, "%d ",br_spacing[i].p_j_ct );
      fprintf( fptr, "%d ",br_spacing[i].i_ct );
      fprintf( fptr, "%d ",br_spacing[i].j_ct );
      fprintf( fptr, "%d ",br_spacing[i].i1_ct );
      fprintf( fptr, "%d\n",br_spacing[i].j1_ct );
    }


    // add these detail after doing tree things..
    // After tree construction
    fprintf( fptr, "-------------------------------\n");
    fprintf( fptr, "----After Tree Construction----\n");
    fprintf( fptr, "-------------------------------\n");

    //printf("\n dump_resssssss\n");
    int jacount = 0;
    fprintf( fptr, "\n------------------------------------\n");
    fprintf( fptr, "Branch angles for each junction \n");
        for (i = 0; i < n_da_discs; i++)
        {
            if(JuncWithSortedAngle[i].size()>0)
            {
              fprintf( fptr, "%d) ", (jacount+1) );
              for( int j=0;j<JuncWithSortedAngle[i].size();j++)
              {
                fprintf( fptr, "%.2f ", JuncWithSortedAngle[i].at(j));
              } // for j
              jacount++;
              fprintf( fptr, "\n");
            } // if
        } // for i

    int jacount2 = 0;
    fprintf( fptr, "\nNon Ultimate Branch angles for each junction \n");
        for (i = 0; i < NLeaveCount; i++)
        {
            if(VSortedAngleJuncNotLeaves[i].size()>0)
            {
              fprintf( fptr, "%d) ", (jacount2+1) );
              for( int j=0;j<VSortedAngleJuncNotLeaves[i].size();j++)
              {
                fprintf( fptr, "%.2f ", VSortedAngleJuncNotLeaves[i].at(j));
              } // for j
              jacount2++;
              fprintf( fptr, "\n");
            } // if
        } // for i

    fprintf( fptr, "\n------------------------------------\n");
    int rrbc=0;
    for(i=0;i<WRRB_count;i++)
        if(newRB[i].val > 0) rrbc ++;



    fprintf( fptr, "Amount of all branches : %d \n",rrbc);
    int rrbcf = 0;
    for(i=0;i<WRRB_count;i++)
    {
        if(newRB[i].val > 0)
        {
            fprintf( fptr,"%d) %d %d %d %d %d %d %d %.2f\n",(rrbcf+1),
            newRB[i].p1,
            newRB[i].x1,
            newRB[i].y1,
            newRB[i].x2,
            newRB[i].y2,
            newRB[i].RbN,
            newRB[i].BrOrNo,
            newRB[i].val/ppmm);
            rrbcf++;
        }
    } // for i show all branches
    fprintf( fptr,"\n");

    fprintf( fptr, "The amount of branch order : %d \n",max_Branch_Order);
    for( i=0;i<max_Branch_Order;i++)
    {
        fprintf( fptr, "Branch Order No. %d \n",(i+1));
        fprintf( fptr, "Amount of Branches in Branch Order No.%d : %d \n",(i+1),BrOr_count[i]);
        for(int j=0;j<BrOr_count[i];j++)
        {
            fprintf( fptr,"%d) %d %d %d %d %d %d %.2f\n",(j+1),
            branch_Order_array[i][j].p1,
            branch_Order_array[i][j].x1,
            branch_Order_array[i][j].y1,
            branch_Order_array[i][j].x2,
            branch_Order_array[i][j].y2,
            branch_Order_array[i][j].RbN,
            branch_Order_array[i][j].val/ppmm);
        } // for j show branches in each branch order
        fprintf( fptr,"\n");
    } // for i : each branch order

    // end of adding things after tree things.

    fprintf( fptr, "end_analysis\n");

    fclose( fptr );
  } // fptr != NULL
} // dump_res


void GLWidget::det_max_discs(int object[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax, int jmax)
{
  /* determines the maximum discs in the object in each site */
  /* the value of the radius is stored in corresponding site */
  /* in junc                                                 */

  int i, j, radius, tmp;
  /* measure disc  used in det_max_discs           */
  Ij_pos circle_buf[MAX_CIRC];
  int n_circle_buf;
  bool too_large;
  char reqstring[STRLENGTH];

  //GTmssge("det_max_discs ent");

  //GTbusytextpp("determining all max discs");

  /* junc is initialzied */
  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
        junc[i][j] = 0;
    } /* for j */
  } /* for i */
  //GTmssge("A");

  for (i = 1; i < imax - 1; i++){
    for (j = 1; j < jmax - 1; j++){
      if ( object[i][j] == 1 ){
        radius = 1;
        too_large = FALSE;
        while ( (radius < MAX_RADIUS) && !too_large ){
          n_circle_buf = 0;
          mich_circle( junc, imax, jmax, i, j, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
          too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
          radius++;
        } /* while */
        if (radius > 100 ){
          //GTmssge("fra: warning radius > 100 in det_max_discs");
        }
        junc[i][j] = radius;
      } /* site */
    } /* for j */
    if ( ((i  % 100) == 0) ){
      tmp = i / 10;
      sprintf( reqstring, "determining all discs %d perc done", tmp );
      //GTmssge( reqstring );
      //GTupdate();
    }
  } /* for i */

  //GTmssge("det_max_discs left");
} /* det_max_discs */

void GLWidget::skel_max_discs(int junc[MAX_ISIZE][MAX_ISIZE],int skel[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  /* July 1999 exp: determines the morphologocal skelton     */
  /* using the maximum discs in the object in each site      */
  /* produced in det_max_discs                               */

  int             i, j, radius, tmp;
  /* measure disc  used in det_max_discs           */
  Ij_pos          circle_buf[MAX_CIRC];
  int             n_circle_buf;
  bool            too_large;
  char            reqstring[STRLENGTH];
  Ij_pos          env1[MAX_ENV];
  int             n_env1;

  /* junc is initialzied */
  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
        skel[i][j] = 0;
    } /* for j */
  } /* for i */

  for (i = 2; i < imax - 2; i++){
    for (j = 2; j < jmax - 2; j++){
      if ( junc[i][j] > 0 ){
        /* in object */

        n_env1 = 0;
        collect1_neighbours( junc, i, j, env1, &n_env1 );


        /*if ( test_maximum( junc, i, j, env1, &n_env1 ) ){*/
        if ( test_maximum2( junc, imax, jmax, i, j ) ){
          skel[i][j] = SKEL_COLOUR;
        }
      } /* site */
    } /* for j */
    if ( ((i  % 10) == 0) ){
      tmp = (int)((float)i / (float)imax * 100.0);
      sprintf( reqstring, "skel max discs %d perc done", tmp );
    }
  } /* for i */

} /* skel_max_discs */

bool GLWidget::test_maximum2(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int i,int j)
{
  /* July 1999: tests if i, j is een local maximum */
  /* Gives about reasonable (but ``hairy'') looking */
  /* results */
  int	radius;
  Ij_pos circle_buf[MAX_CIRC];
  int n_circle_buf;
  bool maximum = FALSE;

  radius = 1;
  maximum = FALSE;
  while ( (radius < 3) ){
    n_circle_buf = 0;
    mich_circle( im, imax, jmax,i, j, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
    maximum = test_maximum_disc( im, imax, jmax, i, j, circle_buf, n_circle_buf, radius );
    radius++;
  } /* while */

  return( maximum );
} /* test_maximum2 */

bool GLWidget::test_maximum_disc(int im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax, int centre_i, int centre_j, Ij_pos  circle[MAX_CIRC],int n_circle,int	radius)
{
  /* A test for identifying if the centre of a circle is a local max */
  int   in_circle, iin_circle, max_in_circle;
  bool  maximum;
  bool  max_found = FALSE;
  float	d, max_d;
  bool	first;

  maximum = FALSE;
  for( in_circle = 0; in_circle < n_circle; in_circle++){
    max_d = 0.0;
    max_found = FALSE;
    for( iin_circle = 0; iin_circle < n_circle; iin_circle++){
      if (in_circle != iin_circle){
        d = sqrt(
        (float) pow( (circle[in_circle].i_x - circle[iin_circle].i_x),2) +
        (float) pow( (circle[in_circle].j_y - circle[iin_circle].j_y),2) );
        if( d > max_d ){
          max_found = TRUE;
          max_in_circle = iin_circle;
          max_d = d;
        }
      }
    } /* for iin_circle */
    if ( (max_found) && (!maximum) ){
      if ((int)(max_d) >= (2 * radius)){
        maximum = TRUE;
        if (im[circle[in_circle].i_x][circle[in_circle].j_y] >=
        im[centre_i][centre_j]){
          maximum = FALSE;
        }
        else{
          bresenham( im, imax, jmax, centre_i, centre_j, circle[in_circle].i_x, circle[in_circle].j_y, im[centre_i][centre_j], &maximum );
        }

        if (maximum){
          if (im[circle[max_in_circle].i_x][circle[max_in_circle].j_y] >=
          im[centre_i][centre_j]){
            maximum = FALSE;
          }
          else{
            bresenham( im, imax, jmax, centre_i, centre_j,
            circle[max_in_circle].i_x, circle[max_in_circle].j_y,im[centre_i][centre_j], &maximum );
          }
        }
      }
    } /* max_found */
  } /* for in_circle */

  return( maximum );
} /* test_max_disc */

void GLWidget::bresenham(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int ix0,int iy0,int ix1,int iy1,int	centre_value,bool *maximum)
{
  /* bresenham algorithm, see Foley and Van Dam, 1984 */
  /* if the boolean assign is TRUE an assignment is done */
  /* in the matrix, otherwise an estimation is done from the local */
  /* concentrations in the considered points */
  int 	dx, dy, d, incr0, incr1, sign, y, x, maxy, miny;
  int 	tmpx, tmpy;
  int   prev_value = 0;

  if ( ix1 < ix0 ){
    /* swapping */
    tmpx = ix0;
    tmpy = iy0;
    ix0  = ix1;
    iy0  = iy1;
    ix1  = tmpx;
    iy1  = tmpy;
  }

  sign = (iy1 < iy0) ? -1 : 1;
  maxy = (iy1 < iy0) ? iy0 : iy1;
  miny = (iy1 < iy0) ? iy1 : iy0;

  dx = (ix1 - ix0);
  dy = (iy0 < iy1) ? (iy1 - iy0) : (iy0 - iy1);

  d = (dy <= dx ) ? 2 * dy - dx: 2 * dx - dy;
  incr0 =  (dy <= dx) ? 2 * dy: 2 * dx;
  incr1 =  (dy <= dx) ? 2 * (dy - dx): 2 * (dx - dy);
  x = ix0;
  if (dy <= dx){
    y = iy0;
    /* first point */
    if ( (im[x][y] > centre_value)  ){
      *maximum = FALSE;
      prev_value = im[x][y];
    }

    while ( x < ix1 ){
      x++;
      if ( d < 0 ){
        d += incr0;
      }
      else{
        y += sign;
        d += incr1;
      }
      if ( (im[x][y] > centre_value) ){
        *maximum = FALSE;
        prev_value = im[x][y];
      }


    } /* while */
  } /* dy <= dx */
  else if (dy > dx){
    if (sign > 0) y = (iy1 < iy0) ? iy1 : iy0;
    else y = (iy1 < iy0) ? iy0 : iy1;
    /* first point */
    if ( (im[x][y] > centre_value) ){
      *maximum = FALSE;
      prev_value = im[x][y];
    }



    while ( ((y < maxy) && (sign > 0)) || ((y > miny) && (sign < 0)) ){
      y += sign;
      if ( d < 0 ){
        d += incr0;
      }
      else{
        x++;
        d += incr1;
      }
      if ( (im[x][y] > centre_value) ){
        *maximum = FALSE;
        prev_value = im[x][y];
      }


    } /* while */
  } /* dy > dx */

} /* bresenham */

void GLWidget::idet_discs(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int skel[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
//determines the maximum discs in the objects using the junctions interactive version

  int i, j, radius;
  // measure disc  used in det_discs
  Ij_pos circle_buf[MAX_CIRC];
  int n_circle_buf, nr_discs, m_type;
  bool too_large;
  Real size  = (Real)MAX_ISIZE;
  Wc wc, wc2[2], wc3[2];
  int choice;
  bool erase;

  nr_discs = 0;
  for (i = 1; i < imax - 1; i++){
    for (j = 1; j < jmax - 1; j++){
      if ( junc[i][j] == 1 ){
        // displaying center point
        show_cross( j, i, CROSS_COLOR, NORMAL_SIZE );

        //choice = GTreq_chcs( 3, choices );
        //scanf("Input choice %d", choice);
        choice = 3;
        switch(choice){
        case 1: // stop
          // the junc image is merged with the skeleton imgae
          // without removing centermarks and discmarks
          merge_imaimb( junc, skel, imax, jmax );
          // all 1 marks are removed from junc
          //erase_im( junc, imax, jmax );
          return;
        case 2: // discard junction, junction and disc is erased
          show_cross( j, i, CROSS_COLOR, NORMAL_SIZE );
          m_type = 0;
          break;
        case 3: // use junction
          m_type = DISC_MARK;
          break;
        } // switch

        radius = 1;
        too_large = FALSE;
        nr_discs++;
        while ( (radius < MAX_RADIUS) && !too_large ){
          n_circle_buf = 0;
          mich_circle( junc, imax, jmax, i, j, radius,circle_buf, &n_circle_buf, m_type );
          // circle is immediately displayed
          too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
          // note neighbours in the state 1 are erased
          // when the circle is not the outer circle
          erase = too_large ? FALSE : TRUE;
          show_circle( junc, imax, jmax, circle_buf, n_circle_buf, m_type,DISC_COLOUR, erase );
          radius++;
        } // while
        // marking or erasing the center of the disc
        junc[i][j] = (m_type == DISC_MARK) ? CENTER_MARK : 0;
        if (radius == 1)
            printf("fra: warning radius 1 in det_discs");
        if (radius == MAX_RADIUS){
            printf("fra: warning radius MAX_RADIUS in det_discs");
        }
      } //junction
    } // for j
  } // for i
  MSSI( nr_discs );

  // the junc image is merged with the skeleton image without removing centermarks and discmarks
  merge_imaimb( junc, skel, imax, jmax );
} // idet_discs


void GLWidget::merge_imaimb(int ima[MAX_ISIZE][MAX_ISIZE], int imb[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  /* the marks of imb are copied into ima without overwriting the */
  /* non-zero marks of ima                                        */

  int   i, j;

  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
        if ( ima[i][j] == 0 ) ima[i][j] = imb[i][j];
    } /* for j */
  } /* for i */
} /* merge_imaimb */


void GLWidget::find_center_disc(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  disc[MAX_CIRC],int n_disc,Int colour,int *i_ct, int *j_ct,bool	*error)
{
  int   in_disc;

  if (n_disc == 0){
    /* empty disc in find_center_disc */
    *error = TRUE;
  }

  for( in_disc= 0; in_disc< n_disc; in_disc++){
    find_center_mark( im, imax, jmax, disc[in_disc].i_x, disc[in_disc].j_y,
    i_ct, j_ct, error );
    if (!(*error)){
      return;
    }
    else{
      show_cross( disc[in_disc].i_x, disc[in_disc].j_y, colour,  LARGE_SIZE );
    }
  } /* for in_disc */
} /* find_center_disc */

// my version of add_neighbours
void GLWidget::add_neighbours(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos	pos,int	*n_border,Ij_pos	border[MAX_CIRC])
{
  if ( (junc[pos.i_x - 1][pos.j_y + 1] == DISC_MARK) ||
  (junc[pos.i_x - 1][pos.j_y + 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x - 1, pos.j_y + 1, border, n_border );
  }
  if ( (junc[pos.i_x    ][pos.j_y + 1] == DISC_MARK) ||
  (junc[pos.i_x    ][pos.j_y + 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x    , pos.j_y + 1, border, n_border );
  }
  if ( (junc[pos.i_x + 1][pos.j_y + 1] == DISC_MARK) ||
  (junc[pos.i_x + 1][pos.j_y + 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x + 1, pos.j_y + 1, border, n_border );
  }
  if ( (junc[pos.i_x + 1][pos.j_y    ] == DISC_MARK) ||
  (junc[pos.i_x + 1][pos.j_y    ] == CENTER_MARK) ){
    add_site_circle( pos.i_x + 1, pos.j_y    , border, n_border );
  }
  if ( (junc[pos.i_x + 1][pos.j_y - 1] == DISC_MARK) ||
  (junc[pos.i_x + 1][pos.j_y - 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x + 1, pos.j_y - 1, border, n_border );
  }
  if ( (junc[pos.i_x    ][pos.j_y - 1] == DISC_MARK) ||
  (junc[pos.i_x    ][pos.j_y - 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x    , pos.j_y - 1, border, n_border );
  }
  if ( (junc[pos.i_x - 1][pos.j_y - 1] == DISC_MARK) ||
  (junc[pos.i_x - 1][pos.j_y - 1] == CENTER_MARK) ){
    add_site_circle( pos.i_x - 1, pos.j_y - 1, border, n_border );
  }
  if ( (junc[pos.i_x - 1][pos.j_y    ] == DISC_MARK) ||
  (junc[pos.i_x - 1][pos.j_y    ] == CENTER_MARK) ){
    add_site_circle( pos.i_x - 1, pos.j_y    , border, n_border );
  }


  // site is marked
  junc[pos.i_x][pos.j_y] += SPECIAL_MARK;
} // add_neigh_bours

void GLWidget::find_center_mark(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax, int i_pos,int j_pos,int *i_ct,int *j_ct,bool *error)
{
 Ij_pos  patch[MAX_CIRC];
 Ij_pos  border[MAX_CIRC];
 int     in_patch, n_patch;
 int     in_border, n_border;
 bool	 ready;

 *i_ct = 0;
 *j_ct = 0;
 *error = FALSE;
 n_patch = 0;
 add_site_circle( i_pos, j_pos, patch, &n_patch );
 ready = FALSE;
 while( !ready){
   n_border = 0;
   for( in_patch = 0; in_patch< n_patch; in_patch++){
    if ( (junc[patch[in_patch].i_x][patch[in_patch].j_y] == CENTER_MARK) ){
      /* center disc found */
      *i_ct = patch[in_patch].i_x;
      *j_ct = patch[in_patch].j_y;
      return;
    }
    else if ( (junc[patch[in_patch].i_x][patch[in_patch].j_y] == DISC_MARK) ){
      add_neighbours( junc, imax, jmax, patch[in_patch], &n_border, border );
    }
   } /* for in_patch */

   if (n_border == 0){
     /* no new sites added ready */
     ready = TRUE;
     *error = TRUE;
   }
   else{
     /* border is copied to patch */
     for( in_border = 0; in_border < n_border ; in_border++){
        patch[in_border].i_x =  border[in_border].i_x;
        patch[in_border].j_y =  border[in_border].j_y;
     }
     n_patch = n_border;
   }
 } /* while */

} /* find_center_mark */

void GLWidget::display_skel_part(Ij_pos	skel_part[MAX_CIRC],int	n_skel_part,Int	colour)
{
  int		  in_skel_part;
  Real            size  = (Real)MAX_ISIZE;
  Wc              wc;

  for( in_skel_part= 0; in_skel_part< n_skel_part; in_skel_part++){
    wc.w_x = (Real) skel_part[in_skel_part].i_x / size;
    wc.w_y = (Real) skel_part[in_skel_part].j_y / size;
  }
} /* display_skel_part */

void GLWidget::display_disc(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  disc[MAX_CIRC],int n_disc,Int colour,int	*i_ct,int *j_ct,bool *error)
{
  int	in_disc;

  for( in_disc= 0; in_disc< n_disc; in_disc++){
    find_center_mark( im, imax, jmax, disc[in_disc].i_x, disc[in_disc].j_y, i_ct, j_ct, error );
    if (!(*error)){
      return;
    }
  } /* for in_disc */
} /* display_disc */

void GLWidget::test_neighbours(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int i,int j,Ij_pos  start_pts[MAX_CIRC],int *n_start_pts)
{
  /* test the neighbours of i,j whether they are skeleton elemnts */
  /* the neighbours of neighbours are tested recursively          */

  if ( im[i - 1][j + 1] == 1 ){
    add_site_circle( i - 1, j + 1, start_pts, n_start_pts);
  }
  if ( im[i    ][j + 1] == 1 ){
    add_site_circle( i    , j + 1, start_pts, n_start_pts);
  }
  if ( im[i + 1][j + 1] == 1 ){
    add_site_circle( i + 1, j + 1, start_pts, n_start_pts);
  }
  if ( im[i + 1][j    ] == 1 ){
    add_site_circle( i + 1, j    , start_pts, n_start_pts);
  }
  if ( im[i + 1][j - 1] == 1 ){
    add_site_circle( i + 1, j - 1, start_pts, n_start_pts);
  }
  if ( im[i    ][j - 1] == 1 ){
    add_site_circle( i    , j - 1, start_pts, n_start_pts);
  }
  if ( im[i - 1][j - 1] == 1 ){
    add_site_circle( i - 1, j - 1, start_pts, n_start_pts);
  }
  if ( im[i - 1][j    ] == 1 ){
    add_site_circle( i - 1, j    , start_pts, n_start_pts);
  }
} /* test_neighbours */


void GLWidget::test_start_pts(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos circle[MAX_CIRC],int n_circle,Ij_pos start_pts[MAX_CIRC],int *n_start_pts)
{
  /* A test for identifying if a neighbour site of the circle is */
  /* part of the skeleton and a starting point of the track      */
  int   in_circle;

  for( in_circle = 0; in_circle < n_circle; in_circle++){
    test_neighbours( im, imax, jmax, circle[in_circle].i_x, circle[in_circle].j_y, start_pts, n_start_pts );
  } /* for in_circle */
} /* test_start_pts */

void GLWidget::correct2_start_pts(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  start_pts[MAX_CIRC],int     *n_start_pts)
{
  /* start pts are corrected pts corresponding to sites in im */
  /* that are already marked are removed                      */
  int           in_start_pts, in_tmp_pts, n_tmp_pts;
  Ij_pos        tmp_pts[MAX_CIRC];
  bool		present;

  n_tmp_pts = 0;
  for( in_start_pts= 0; in_start_pts< *n_start_pts; in_start_pts++){
    /* element of start_pts is only copied to tmp_pts when not */
    /* present yet */
    present = FALSE;
    for( in_tmp_pts= 0; in_tmp_pts< n_tmp_pts; in_tmp_pts++){
      if ( (tmp_pts[in_tmp_pts].i_x ==  start_pts[in_start_pts].i_x) &&
      (tmp_pts[in_tmp_pts].j_y ==  start_pts[in_start_pts].j_y) ){
         present = TRUE;
      }
    }
    if (!present){
      tmp_pts[n_tmp_pts].i_x = start_pts[in_start_pts].i_x;
      tmp_pts[n_tmp_pts].j_y = start_pts[in_start_pts].j_y;
      n_tmp_pts++;
    }
  } /* for in_start_pts */

  /* tmp_pts is copied to start_pts */
  *n_start_pts =  n_tmp_pts;
  for( in_start_pts= 0; in_start_pts< *n_start_pts; in_start_pts++){
    start_pts[in_start_pts].i_x = tmp_pts[in_start_pts].i_x;
    start_pts[in_start_pts].j_y = tmp_pts[in_start_pts].j_y;
  } /* for in_start_pts */
} /* correct_start_pts */

void GLWidget::sort_angles(Medge   result[MAX_RES],int  *n_result)
{
  /* The array with da discs and the corresponding db0, db1 discs */
  /* that is organized in the pairs a, b0 and a, b1               */
  /* is reorganized in triplets a, b0, b1                         */

  int 	in_result, iin_result, new_n_result;
  int	ai_ct, aj_ct;
  Real	angle, mark = 999.0;
  Medge new_result[MAX_RES];

  //GTmssge("sort_angles entered");

  new_n_result = 0;
  for (in_result = 0; in_result < *n_result; in_result++){
    if (result[in_result].meas < 0.5 * mark){
      /* field not marked */
      ai_ct = result[in_result].p_i_ct;
      aj_ct = result[in_result].p_j_ct;
      /* searching b1 */
      for (iin_result = in_result + 1; iin_result < *n_result; iin_result++){
        if ( (result[iin_result].p_i_ct == ai_ct) &&
        (result[iin_result].p_j_ct == aj_ct) ){
          /* the triplet can be organized */
           new_result[new_n_result].meas = result[in_result].meas;
           new_result[new_n_result].p_i_ct = result[in_result].p_i_ct;
           new_result[new_n_result].p_j_ct = result[in_result].p_j_ct;
           new_result[new_n_result].i_ct = result[in_result].i_ct;
           new_result[new_n_result].j_ct = result[in_result].j_ct;
           new_result[new_n_result].i1_ct = result[iin_result].i_ct;
           new_result[new_n_result].j1_ct = result[iin_result].j_ct;
           new_n_result++;

           result[iin_result].meas = mark;
           break;
        } /* triplet */
      } /* for iin_result */
    } /* not marked */
  } /* in_result */

  /* the result array is changed a copy of the array of organized */
  /* triplets is made */
  *n_result =  new_n_result;
  for (in_result = 0; in_result < *n_result; in_result++){
    det_angle( &angle,
    new_result[in_result].p_i_ct, new_result[in_result].p_j_ct,
    new_result[in_result].i_ct, new_result[in_result].j_ct,
    new_result[in_result].i1_ct, new_result[in_result].j1_ct );

    result[in_result].meas = angle;
    result[in_result].p_i_ct = new_result[in_result].p_i_ct;
    result[in_result].p_j_ct = new_result[in_result].p_j_ct;
    result[in_result].i_ct = new_result[in_result].i_ct;
    result[in_result].j_ct = new_result[in_result].j_ct;
    result[in_result].i1_ct = new_result[in_result].i1_ct;
    result[in_result].j1_ct = new_result[in_result].j1_ct;
  } /* for in_result */

} /* sort_angles */

void GLWidget::det_angle(Real	*angle,int     ai_ct,int aj_ct,int b0i_ct,int b0j_ct,int b1i_ct,int b1j_ct)
{
  /* the branching angle is determined */
  Real  size  = (Real)MAX_ISIZE, cosalpha, sinalpha;
  Wc    a, b0, b1;


  a.w_x = (Real)ai_ct / size;
  a.w_y = (Real)aj_ct / size;
  b0.w_x = (Real)b0i_ct / size;
  b0.w_y = (Real)b0j_ct / size;
  b1.w_x = (Real)b1i_ct / size;
  b1.w_y = (Real)b1j_ct / size;

// check the functions later!!!!!!
  cosalpha =  bstool->BSzzz_cosalp2( &b0, &a, &b1 );
  sinalpha = sqrt( 1.0 - pow( cosalpha,2 ));
  *angle = bstool->BSangle( cosalpha, sinalpha );
} /* det_angle */

void GLWidget::det_geotropy(int	prev_i_ct, int prev_j_ct,int i_ct,int j_ct,Medge result[MAX_RES],int	*n_result)
{
 // the angle of geotropy (angle between vertical and the vector prev_ct, ct is determined

  Real m;
  Real alpha;
  if((prev_j_ct-j_ct)!= 0)
  {
      m = (Real)(prev_i_ct-i_ct)/(prev_j_ct-j_ct);
      alpha = 90 - abs(atan(m)*180/MYPI);
      add_result( alpha, result, n_result, prev_i_ct, prev_j_ct, i_ct, j_ct, ExactJuncLabel, 0 );
  }
} // det_geotropy

void GLWidget::det_rb(int	prev_i_ct, int prev_j_ct,int i_ct,int j_ct)
{

  // my version euclidean distance calculated in pixel
    Real d;
    d = sqrt(pow(i_ct-prev_i_ct,2.0)+pow(j_ct-prev_j_ct,2.0));
    add_result( d, rb, &n_rb,  prev_i_ct, prev_j_ct, i_ct, j_ct, ExactJuncLabel, 0 );

} // det_rb

void GLWidget::follow_track(int object[MAX_ISIZE][MAX_ISIZE],int skeleton[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int scratch[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int i_ct,int j_ct,int i_start,int j_start)
{
  /* A track is followed in order to determine successive discs */
  /* non-interactve version                                     */
  /* The procedure is initialized using ifollow_track           */
  /* where interactively the starting disc and the direction    */
  /* of processing of the track is determined                   */

  Wc     wc;
  Real   size  = (Real)MAX_ISIZE;
  Ij_pos circle_buf[MAX_CIRC];
  Ij_pos start_pts[MAX_CIRC];
  Ij_pos skel_part[MAX_CIRC];
  Ij_pos disc[MAX_CIRC];
  Ij_pos ct_pts[MAX_CIRC]; // array of center points
  int    n_circle_buf, n_start_pts, in_start_pts, radius;
  int	 n_skel_part, n_disc, n_ct_pts, cnt;
  int	 prev_i_ct, prev_j_ct;
  bool	 too_large, error = FALSE;
  Int	 choice, choice2;
  bool	 erase, dc_added = FALSE;


  /* first rb is dtermined */
  det_rb( i_start, j_start, i_ct, j_ct );
  /* geotropy angle is dtermined */
  det_geotropy( i_start, j_start, i_ct, j_ct, geotropy, &n_geotropy );


  n_ct_pts = 0; // number of center points
  add_site_circle( i_ct, j_ct, ct_pts, &n_ct_pts );
  cnt = 0;
  while( n_ct_pts > 0){
    /* The first center of  the array of center points */
    /* is taken                                        */
    prev_i_ct = i_ct = ct_pts[0].i_x;
    prev_j_ct = j_ct = ct_pts[0].j_y;

    radius = 1;
    too_large = FALSE;
    while ( (radius < MAX_RADIUS) && !too_large ){
      n_circle_buf = 0;
      /* the current disc is marked to indiacate that it is       */
      /* visited, the first center of  the array of center points */
      /* is taken                                                 */
      mich_circle( junc, imax, jmax, i_ct, j_ct, radius,circle_buf, &n_circle_buf, DISC_MARK + SPECIAL_MARK );
      too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
      /* note neighbours in tthe state 1 are erased */
      /* when the circle is not the outer circle    */
      erase = too_large ? FALSE : TRUE;
      show_circle( junc, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,SKEL_COLOUR, erase );
      if (too_large){
        /* outer circle reached, radius of current da disc is stored */
        add_result( (Real)radius, da_discs, &n_da_discs, i_ct, j_ct, 0,0,0,0 );
      MSSI( n_da_discs );

        /* outer circle reached, now starting points can be determined */
        n_start_pts = 0;
        /*test_start_pts( junc, imax, jmax, circle_buf, n_circle_buf, */
        test_start_pts( skeleton, imax, jmax, circle_buf, n_circle_buf,
        start_pts, &n_start_pts  );
MSSI( n_start_pts );
        /* sites that are present more than once in     */
        /* start_pts are removed to avoid double        */
        /* tracks                                       */
        correct2_start_pts( junc, imax, jmax, start_pts, &n_start_pts );

        in_start_pts = 0;
        while (in_start_pts <  n_start_pts ){
          n_skel_part = 0;
          n_disc = 0;
          /*follow_skel( object, junc,imax, jmax,start_pts[in_start_pts].i_x, */
          /*start_pts[in_start_pts].j_y, skel_part, &n_skel_part,*/
          /*disc, &n_disc, i_ct, j_ct, &dc_added  );*/
          /* note that prev_i_ct and prev_j_ct are the coord of the previous */
          /* da disc */
          follow_skel( object, junc, skeleton, scratch,imax, jmax, start_pts[in_start_pts].i_x,
          start_pts[in_start_pts].j_y, skel_part, &n_skel_part,disc, &n_disc, prev_i_ct, prev_j_ct, &dc_added  );
          /* the skeleton part and the new detected disc are shown */
          display_skel_part( skel_part, n_skel_part, WHITE );
          /* center of the next disc is determined and displayed */
          find_center_disc( junc, imax, jmax, disc, n_disc, CONT_COLOR, &i_ct,&j_ct, &error );
          if (!error){
            MSSI( i_ct );
            MSSI( j_ct );
            add_site_circle( i_ct, j_ct, ct_pts, &n_ct_pts );
            cnt++;
            /* rb is determined */
            det_rb( prev_i_ct, prev_j_ct, i_ct, j_ct );
            /* geotropy angle is determined */
            det_geotropy( prev_i_ct, prev_j_ct, i_ct, j_ct,
            geotropy, &n_geotropy );
          } /* !error */
          in_start_pts++;
        } /* while in_start_pts */
        /* all special marks remain in junc */
        /* to avoid double tracks in        */
        /* follow_skel                             */
      } /* too_large */
      radius++;
    } /* while */

    if (radius == MAX_RADIUS){
      //GTmssge("fra: warning MAX_RADIUS in follow_track");
    }

    /* the firsts center in queue is removed from the array of center   */
    /* points                                                   */
    rm_site_circle(  ct_pts[0].i_x,  ct_pts[0].j_y, ct_pts, &n_ct_pts);
  } /* while n_ct_pts > 0 */

  /* all special marks are removed from junc */
  /* rm_spec_m_im( junc, imax, jmax );*/
  MSSI( cnt );
} /* follow_track */


void GLWidget::ifollow_track(int object[MAX_ISIZE][MAX_ISIZE], int skeleton[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE], int scratch[MAX_ISIZE][MAX_ISIZE],int	imax, int jmax)
{
  // A track is followed in order to determine successive discs intersactive version
  Wc  wc;
  Real  size  = (Real)MAX_ISIZE;
  int	i_pos, j_pos, i_ct, j_ct;
  Ij_pos circle_buf[MAX_CIRC];
  Ij_pos start_pts[MAX_CIRC];
  Ij_pos skel_part[MAX_CIRC];
  Ij_pos disc[MAX_CIRC];
  int n_circle_buf, n_start_pts, in_start_pts, radius;
  int n_skel_part, n_disc;
  bool too_large, error = FALSE;
  Int choice, choice2;
  bool erase, start_da_added, dc_added;
  int i_start, j_start;


  count_maxskel = 0;
  for(int ii=0;ii<count_disc_area;ii++)
  {
    i_pos = disc_area_y[ii];
    j_pos = disc_area_x[ii];

    if ((i_pos < 0) || (j_pos < 0) || (i_pos > MAX_ISIZE) || (j_pos > MAX_ISIZE))
    {
      printf("fra: error in ifollow_track");
      return;
    }
    start_da_added = FALSE;
    dc_added = FALSE;
    find_center_mark( junc, imax, jmax, i_pos, j_pos, &i_ct, &j_ct, &error );
    i_start = i_ct;
    j_start = j_ct;

    if (error)
    {
      printf("error no center detected, please try again");
    }
    else
    {
      AngleA = ii; // keep junc lable for later use in show_db_discs
      ExactJuncLabel = ii+1;

          for(int k=0;k<count_disc_area;k++)
          {
              if(abs(i_start-disc_area_y[k])<2 && abs(j_start-disc_area_x[k])<2)
              {
                  ExactJuncLabel = k+1;
                  AngleA = k;
              }
          } // for k
      radius = 1;
      too_large = FALSE;
      while ( (radius < MAX_RADIUS) && !too_large )
      {
        n_circle_buf = 0;
        // the starting disc is marked to indiacate that it is visited
        mich_circle( junc, imax, jmax, i_ct, j_ct, radius, circle_buf, &n_circle_buf, DISC_MARK + SPECIAL_MARK );
        //mich_circle( im_contour, imax, jmax, i_ct, j_ct, radius, circle_buf, &n_circle_buf, DISC_MARK + SPECIAL_MARK );
        too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
        // note neighbours in the state 1 are erased when the circle is not the outer circle
        if (too_large)
        {
          // outer circle reached, now starting points can be determined
          n_start_pts = 0;
          test_start_pts( skeleton, imax, jmax, circle_buf, n_circle_buf, start_pts, &n_start_pts  );
          MSSI( n_start_pts );

          in_start_pts = 0;
          printf("\n n_start_pts:%d \n",n_start_pts);
          while (in_start_pts <  n_start_pts )
          {
            n_skel_part = 0;
            n_disc = 0;

            follow_skel( object,junc,skeleton, scratch, imax,jmax,start_pts[in_start_pts].i_x,
            start_pts[in_start_pts].j_y, skel_part, &n_skel_part,disc, &n_disc, i_start, j_start, &dc_added  );

            KeepSkel((AngleA+1),count_maxskel,skel_part, n_skel_part);

            correct2_start_pts( skeleton, imax, jmax, start_pts, &n_start_pts );
            choice2 = 2; // my choice
            if (choice2 == 1)
            {
              // loop is interrupted
              in_start_pts = n_start_pts;
            }
            else if ( (choice2 == 2) && error )
            {
              //printf("warning: no starting center point  ");
            }
            else if ( (choice2 == 2) && !error )
            {
              // current da_disc is added to the results
              if (!start_da_added)
              {
                add_result( (Real)radius, da_discs, &n_da_discs, i_start, j_start, ExactJuncLabel, 0, 0, 0 );
                start_da_added = TRUE;
              }


            }
            else if (choice2 == 3)
            {
              // part of skeleton is devisualized again
              display_skel_part( skel_part, n_skel_part, SKEL_COLOUR );
              // last db disc is removed again
              n_db_discs--;
              // last triple for angle calculation is removed again
              n_angle--;
              // last dc disc and geotropy measurement is removed again
              if (dc_added) n_dc_discs--;
              if (dc_added) n_geotropy--;
              if (dc_added) term_n_geotropy--;
              if (in_start_pts + 1 == n_start_pts)
              {
                printf("warning: no start points left");
              }
            } // choice2
            count_maxskel++;
            in_start_pts++;
          } // while in_start_pts
          // all special marks are removed from junc afterwards to avoid double tracks in follow_skel try
           rm_spec_m_im( junc, imax, jmax );

           if (n_start_pts == 0)
           {
              printf("no start points detected, please try other disc");
           }
        } // too_large
        radius++;
      } // while
      if (radius == MAX_RADIUS)
      {
        printf("fra: warning MAX_RADIUS in ifollow_track");
      }
    } // else (no error)
  } // while testxpos

  sort_angles( angle, &n_angle );

} // ifollow_track


void GLWidget::follow_skel(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int	skeleton[MAX_ISIZE][MAX_ISIZE], int scratch[MAX_ISIZE][MAX_ISIZE],int	imax, int jmax,int	i_start,int j_start,Ij_pos	skel_part[MAX_CIRC],int	*n_skel_part,Ij_pos	disc[MAX_CIRC],int	*n_disc,int i_ct,int j_ct,bool *dc_added)
{
  // the skeleton is followed until the end interactve version

 Ij_pos     patch[MAX_CIRC];
 Ij_pos     border[MAX_CIRC];
 int        in_patch, n_patch;
 int        in_border, n_border;
 bool       ready, found;
 Real       size  = (Real)MAX_ISIZE;
 Wc    		 wc;
 int		 i_ct_db, j_ct_db, max_r, last_i, last_j;
 // measure disc  used in det_discs
 Ij_pos      circle_buf[MAX_CIRC];
 int         n_circle_buf, radius;
 bool        too_large;
 int		 cci, ccj;
 Ij_pos  	 old_env[MAX_ENV];
 Ij_pos  	 new_env[MAX_ENV];
 int     	 n_old_env, n_new_env, in_env;
 Ij_pos  	 tmp_skel_part[MAX_CIRC];
 int     	 n_tmp_skel_part;
 Int		 choice;

 // initialization variables for the db test
 i_ct_db = j_ct_db = max_r = 0;
 found = FALSE;

 n_patch = 0;
 if ( (junc[i_start][j_start] == DISC_MARK) )
 {
   // the start point is part of a disc, but is situated also on
   // the skeleton, the site is set to one otherwise the algorithm does not work
   junc[i_start][j_start] = 1;
 }
 add_site_circle( i_start, j_start, patch, &n_patch );
 add_site_circle( i_start, j_start, skel_part, n_skel_part );
 last_i = i_start;
 last_j = j_start;
 ready = FALSE;

 while( !ready)
 {
   n_border = 0;
   for( in_patch = 0; in_patch< n_patch; in_patch++)
   {
    if ( (junc[patch[in_patch].i_x][patch[in_patch].j_y] == 1) )
    {
      add_neigh_skel( junc, imax, jmax, patch[in_patch], &n_border, border,
      n_disc, disc );
    }
   } // for in_patch

   if ( (n_border == 0) || (*n_skel_part >= max_skel_length) )
   {
     ready = TRUE;      // no new sites added ready
   }
   else
   {
     // border is copied to patch border is collected in skel_part
     for( in_border = 0; in_border < n_border ; in_border++)
     {
        last_i = patch[in_border].i_x =  border[in_border].i_x;
        last_j = patch[in_border].j_y =  border[in_border].j_y;
        if ( *n_skel_part < max_skel_length )
        {
          add_site_circle(  border[in_border].i_x, border[in_border].j_y,
          skel_part, n_skel_part );
        }
     } // for in_border
     n_patch = n_border;

     // a test is carried out to determine the db disc
     find_db_disc( object, junc, imax, jmax, border, n_border,
     &i_ct_db, &j_ct_db, &max_r, &found );

   } // !ready
 } // while

 // showing the final db disc
 if (max_r == 0) printf("fra: warning in follow_skel max_r == 0");
 else
 {
   MSSI( max_r );
   // show and store the final resulting radius of the db disc determine branching angle
   show_db_disc( object, junc, imax, jmax, i_ct_db, j_ct_db, i_ct, j_ct );
   det_rb( last_i, last_j, i_ct, j_ct );
   det_geotropy( i_ct, j_ct, i_ct_db, j_ct_db, term_geotropy, &term_n_geotropy );
 } // max_r != 0

} // follow_skel

void GLWidget::add_neigh_skel(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  pos,int   *n_border,Ij_pos  border[MAX_CIRC],int     *n_disc,Ij_pos  disc[MAX_CIRC])
{
Wc    wc;
Real  size  = (Real)MAX_ISIZE;

  if ( junc[pos.i_x - 1][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x    ][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x    , pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y    ] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y    , border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x    ][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x    , pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x - 1][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x - 1][pos.j_y    ] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y    , border, n_border );
  }

  /* detecting a new disc */
  if ( junc[pos.i_x - 1][pos.j_y + 1] == DISC_MARK ){
    add_site_circle( pos.i_x - 1, pos.j_y + 1, disc, n_disc );
  }
  if ( junc[pos.i_x    ][pos.j_y + 1] == DISC_MARK ){
    add_site_circle( pos.i_x    , pos.j_y + 1, disc, n_disc );
  }
  if ( junc[pos.i_x + 1][pos.j_y + 1] == DISC_MARK ){
    add_site_circle( pos.i_x + 1, pos.j_y + 1, disc, n_disc );
  }
  if ( junc[pos.i_x + 1][pos.j_y    ] == DISC_MARK ){
    add_site_circle( pos.i_x + 1, pos.j_y    , disc, n_disc );
  }
  if ( junc[pos.i_x + 1][pos.j_y - 1] == DISC_MARK ){
    add_site_circle( pos.i_x + 1, pos.j_y - 1, disc, n_disc );
  }
  if ( junc[pos.i_x    ][pos.j_y - 1] == DISC_MARK ){
    add_site_circle( pos.i_x    , pos.j_y - 1, disc, n_disc );
  }
  if ( junc[pos.i_x - 1][pos.j_y - 1] == DISC_MARK ){
    add_site_circle( pos.i_x - 1, pos.j_y - 1, disc, n_disc );
  }
  if ( junc[pos.i_x - 1][pos.j_y    ] == DISC_MARK ){
    add_site_circle( pos.i_x - 1, pos.j_y    , disc, n_disc );
  }

  junc[pos.i_x][pos.j_y] += SPECIAL_MARK;
} /* add_neigh_skel */

void GLWidget::add_neigh_skel1(int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  pos,int   *n_border,Ij_pos  border[MAX_CIRC])
{
Wc    wc;
Real  size  = (Real)MAX_ISIZE;

  if ( junc[pos.i_x - 1][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x    ][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x    , pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y + 1] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y + 1, border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y    ] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y    , border, n_border );
  }
  if ( junc[pos.i_x + 1][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x + 1, pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x    ][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x    , pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x - 1][pos.j_y - 1] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y - 1, border, n_border );
  }
  if ( junc[pos.i_x - 1][pos.j_y    ] == 1 ){
    add_site_circle( pos.i_x - 1, pos.j_y    , border, n_border );
  }
} /* add_neigh_skel1 */



void GLWidget::find_db_disc(int object[MAX_ISIZE][MAX_ISIZE], int junc[MAX_ISIZE][MAX_ISIZE],int	imax,int jmax,Ij_pos border[MAX_CIRC],int	n_border,int	*i_ct_db,int *j_ct_db,int *max_r,bool *found)
{
  /* the skeleton is followed and all the time a max disc is     */
  /* constructed. The construction stops as soon other DISC_MARK */
  /* are found, the construction then proceeds to the next       */
  /* skeleton site. The procedure stops as soon the max disc     */
  /* fitting between the DISC_MARKS and boundaries of the        */
  /* object                                                      */
  int           radius, in_border;
  bool          too_large;
  Ij_pos        circle_buf[MAX_CIRC];
  int		n_circle_buf;

  if ( *found ) return;

  for( in_border = 0; in_border < n_border ; in_border++){
    radius = 1;
    too_large = FALSE;
    while ( (radius < MAX_RADIUS) && !too_large ){
      n_circle_buf = 0;
      /* the sites on the outer circle are not marked */
      mich_circle( junc, imax, jmax,
      border[in_border].i_x, border[in_border].j_y, radius,
      circle_buf, &n_circle_buf, EMPTY_MARK );
      /* the circle is tested whether it contains sites outside */
      /* the object or sites on a marked da disc                */
      too_large =
      test_db_disc( object, junc, imax, jmax, circle_buf, n_circle_buf,
      found );
      if ( (too_large) || (*found) ){
        *i_ct_db = border[in_border].i_x;
        *j_ct_db = border[in_border].j_y;
        *max_r   = radius;
        if ( (*found) && (!too_large) ){
          too_large = TRUE;
        }
      }
      radius++;
    } /* while */
    if (radius == MAX_RADIUS){
        printf("fra: warning MAX_RADIUS in find_db_disc");
    }
    /* interrupt loop when disc exceeds boundaries */
    if (*found) break;
  } /* for in_border */
} /* find_db_disc */

void GLWidget::show_db_disc(int im[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,int  i_ct, int j_ct, int ai_ct, int aj_ct)
{
  /* shows the db disc */
  int           radius;
  bool          too_large;
  Ij_pos        circle_buf[MAX_CIRC];
  int           n_circle_buf;

  radius = 1;
  too_large = FALSE;
  while ( (radius < MAX_RADIUS) && !too_large ){
    n_circle_buf = 0;
    /* the sites on the outer circle are not marked */
    mich_circle( junc, imax, jmax,
    i_ct, j_ct, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
    /* the circle is tested whether it contains sites outside */
    /* the object or sites on a marked da disc                */
    show_circle( junc, imax, jmax, circle_buf, n_circle_buf, DISC_MARK,
    WHITE, FALSE );
    too_large =
    test_disc_too_large( im, imax, jmax, circle_buf, n_circle_buf );
    if (too_large)
    {
      /* store the final resulting radius of the db disc */
        add_result( (Real)radius, db_discs, &n_db_discs, i_ct, j_ct, ai_ct, aj_ct, ExactJuncLabel, 0 );
      det_geotropy( i_ct, j_ct, ai_ct, aj_ct, geotropy, &n_geotropy );
      store_angle( ai_ct, aj_ct, i_ct, j_ct );
      KeepAngle(AngleA,ai_ct,aj_ct,i_ct,j_ct);
    }
    radius++;
  } /* while */
  if (radius == MAX_RADIUS){
    //GTmssge("fra: warning MAX_RADIUS in show_db_disc");
  }
} /* show_db_disc */

bool GLWidget::test_db_disc(int im[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  circle[MAX_CIRC],int n_circle,bool *found)
{
  /* A test for identifying if a site on the circle is not */
  /* situated on the object                                */
  /* or situated on a marked disc                          */
  int   in_circle;
  bool	result2 = FALSE;

  for( in_circle = 0; in_circle < n_circle; in_circle++){
    /* note: the previous da disc is marked */
    if ( (junc[circle[in_circle].i_x][circle[in_circle].j_y] ==
    DISC_MARK + SPECIAL_MARK) ){
      result2 = TRUE;
    }
    if ( !(im[circle[in_circle].i_x][circle[in_circle].j_y] == 1) ){
      *found = TRUE;
    }
  } /* for in_circle */
  return( result2 );
} /* test_db_disc */


void GLWidget::store_angle(int ai_ct, int aj_ct, int b0i_ct, int b0j_ct)
{
  Real  size  = (Real)MAX_ISIZE;
   /* store the result */
  add_result( 0.0, angle, &n_angle,  ai_ct, aj_ct, b0i_ct, b0j_ct, 0, 0 );

} /* store_angle */

void GLWidget::cp_ima_to_imb(int ima[MAX_ISIZE][MAX_ISIZE],int imb[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  /* 3-9-97                             */
  /* the marks of ima are copied to imb */

  int   i, j;

  for (i = 0; i < imax; i++){
    for (j = 0; j < jmax; j++){
        imb[i][j] = ima[i][j];
    } /* for j */
  } /* for i */

} /* cp_ima_to_imb */

void GLWidget::follow_and_rm_skel(int scratch[MAX_ISIZE][MAX_ISIZE],int	imax,int jmax,int	i_start,int j_start,Ij_pos	skel_part[MAX_CIRC],int	*n_skel_part)
{
  /* the skeleton is followed and removed stepwise until the end */
  /*interactve version                      */

 Ij_pos          border[MAX_CIRC];
 int             in_skel_part;
 int             in_border, n_border;
 bool            ready;
 Wc    		 wc;
 /* measure disc  used in det_discs           */
 Ij_pos          dummy[MAX_CIRC];
 int		 n_dummy;
 int		 n_step = 2, in_step;


 for (in_step = 0; in_step < n_step; in_step++){
 n_border = 0;
 for( in_skel_part= 0; in_skel_part < *n_skel_part; in_skel_part++){
  if (
  (scratch[skel_part[in_skel_part].i_x][skel_part[in_skel_part].j_y] == 1) ){
    add_neigh_skel( scratch, imax, jmax, skel_part[in_skel_part],
    &n_border, border, &n_dummy, dummy );
  }
 } /* for in_skel_part*/

 if ( (n_border == 0) || (*n_skel_part >= max_skel_length) ){
   /* no new sites added ready */
   ready = TRUE;
 }
 else{
   /* patch is removed */
   for( in_skel_part= 0; in_skel_part< *n_skel_part; in_skel_part++){
     scratch[skel_part[in_skel_part].i_x][skel_part[in_skel_part].j_y] = 0;
   }

   /* border is copied to skel_part*/
   /* border is collected in skel_part */
   for( in_border = 0; in_border < n_border ; in_border++){
      skel_part[in_border].i_x =  border[in_border].i_x;
      skel_part[in_border].j_y =  border[in_border].j_y;
   } /* for in_border */
   *n_skel_part= n_border;

 } /* !ready */
 } /* for in_step */
} /* follow_and_rm_skel */

bool GLWidget::test_disc_part_skel(int im[MAX_ISIZE][MAX_ISIZE],int imax,int jmax,Ij_pos  circle[MAX_CIRC],int n_circle)
{
  /* addition 3-9-97 measuring branch spacing */
  /* A test for identifying if a site on the circle is */
  /* part of the skeleton */
  int   in_circle;
  bool  result = FALSE;
  int	i_ct, j_ct;
  Wc	wc;

  for( in_circle = 0; in_circle < n_circle; in_circle++){
    if ( (im[circle[in_circle].i_x][circle[in_circle].j_y] == 1) ){
      i_ct = circle[in_circle].i_x;
      j_ct = circle[in_circle].j_y;
      result = TRUE;
    }
  } /* for in_circle */

if (TRUE) {
}
  return( result );
} /* test_disc */


void GLWidget::det_discs2(int object[MAX_ISIZE][MAX_ISIZE],int junc[MAX_ISIZE][MAX_ISIZE],int imax,int jmax)
{
  /* determines the maximum discs in the objects using the */
  /* CENTER_MARKS produced in idet_discs                   */

  int           i, j, radius;
  /* measure disc  used in det_discs           */
  Ij_pos          circle_buf[MAX_CIRC];
  int             n_circle_buf, nr_discs;
  bool            too_large;

  int i_cm = 0;
  int j_cm = 0;
  int cm_count = 0;

  int cmi[1000];
  int cmj[1000];
  printf("\n Now you are in det_discs2");

  nr_discs = 0;
  for (i = 1; i < imax - 1; i++){
    for (j = 1; j < jmax - 1; j++){
      if ( junc[i][j] == CENTER_MARK ){
          cmi[cm_count]= i;
          cmj[cm_count]= j;
          cm_count++;
          i_cm = i;
          j_cm = j;
        radius = 1;
        too_large = FALSE;
        nr_discs++;
        while ( (radius < MAX_RADIUS) && !too_large ){
          n_circle_buf = 0;
          mich_circle( junc, imax, jmax, i, j, radius,circle_buf, &n_circle_buf, DISC_MARK );
          too_large = test_disc_too_large( object, imax, jmax, circle_buf, n_circle_buf );
          radius++;
        } // while
        if (radius == MAX_RADIUS){
           printf("Warning : MAX_RADIUS in det_discs2");
        }
      } // junction
    } // for j
  } // for i
} // det_discs2


void GLWidget::writeAnaToFile(const char* savedname)
{
    dump_res(savedname);
    QString qname1 = QString(savedname);
    QString fullqname = qname1.replace(".res",".sta");

    //const char* savedname_prt = fullqname.toUtf8().constData();
    //dump_prt(savedname_prt);
    dump_prt(fullqname.toUtf8().constData());
 
    QString t_name = analysispath+CSVFilename;
    //const char* savedcsvname_prt = t_name.toUtf8().constData();
    //dump_csv(savedcsvname_prt);
    dump_csv(t_name.toUtf8().constData());
}


void GLWidget::drawString (const char *s)
{
    unsigned int i;
    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

void GLWidget::readfile(const char* fname_)
{
    if(strcmp(fname_,"") != 0)
    {
     filenameana = fname_;
     clearVar();
     init_res();
     clearComp_var();
     read_frain( im_in, &im_in_imax, &im_in_jmax,fname_ );
     PictureArea = Picture_Area(im_in, im_in_imax, im_in_jmax);
     PictureAreaMM = PictureArea/ppmm;
     thin_b (im_in, im_skel, im_in_imax, im_in_jmax );
     find_junctions( im_skel, im_junc, im_in_imax, im_in_jmax );
     cp_ima_to_imb(img_temp,im_junc_temp,im_in_imax, im_in_jmax);
     ReducedJunc(im_junc_reduced, im_in_imax, im_in_jmax );
     find_terminals( im_skel, im_term, im_in_imax, im_in_jmax );
     contour_object( im_in, im_contour, im_in_imax, im_in_jmax );
     det_box_dim( im_contour, im_in_imax, im_in_jmax );
     ContourSize = CalCCCLength(im_contour, im_in_imax, im_in_jmax); // use chain code
     cal_dc(im_in_imax,im_in_jmax);
     cal_br_spacing(im_in_imax,im_in_jmax);
     idet_discs( im_in, im_junc, im_skel, im_in_imax, im_in_jmax ); // the function marks the area of discs on the skeleton line
     det_discs2( im_in, im_junc, im_in_imax, im_in_jmax );

     updateGL();
    } // if
}

void GLWidget::m_show_object(int inputimg[MAX_ISIZE][MAX_ISIZE],int xstart,int ystart,int h,int w,int icolor)
{
         qglColor(colorname[icolor]); // can be substituted by glColor3ub(255, 255, 0);-- yellow
         for(int i=0;i<h;i++)
         {
            for(int j=0;j<w;j++)
            {
                if(inputimg[i][j]==1) glVertex2d(j+xstart,i+ystart);
            } // j
         } // i
} // m_show_object

void GLWidget::var_cp()
{
    m_n_da_discs = n_da_discs;
    m_n_db_discs = n_db_discs;
    m_n_dc_discs = n_dc_discs;
    m_n_rb = n_rb;
    m_n_geotropy = n_geotropy;
    m_term_n_geotropy = term_n_geotropy;
    m_n_logeps_box = n_logeps_box;
    m_n_logneps_box = n_logneps_box;
    m_box_dimension = box_dimension;
    m_n_angle = n_angle;
    m_n_br_spacing = n_br_spacing;

    for(int i=0;i<m_n_da_discs;i++)
    {
        m_da_discs[i].meas = da_discs[i].meas;
        m_da_discs[i].p_i_ct = da_discs[i].p_i_ct;
        m_da_discs[i].p_j_ct = da_discs[i].p_j_ct;
        m_da_discs[i].i_ct = da_discs[i].i_ct;
        m_da_discs[i].j_ct = da_discs[i].j_ct;
        m_da_discs[i].i1_ct = da_discs[i].i1_ct;
        m_da_discs[i].j1_ct = da_discs[i].j1_ct;
    }

    for(int i=0;i<m_n_db_discs;i++)
    {
        m_db_discs[i].meas = db_discs[i].meas;
        m_db_discs[i].p_i_ct = db_discs[i].p_i_ct;
        m_db_discs[i].p_j_ct = db_discs[i].p_j_ct;
        m_db_discs[i].i_ct = db_discs[i].i_ct;
        m_db_discs[i].j_ct = db_discs[i].j_ct;
        m_db_discs[i].i1_ct = db_discs[i].i1_ct;
        m_db_discs[i].j1_ct = db_discs[i].j1_ct;
    }

    for(int i=0;i<m_n_dc_discs;i++)
    {
        m_dc_discs[i].meas = dc_discs[i].meas;
        m_dc_discs[i].p_i_ct = dc_discs[i].p_i_ct;
        m_dc_discs[i].p_j_ct = dc_discs[i].p_j_ct;
        m_dc_discs[i].i_ct = dc_discs[i].i_ct;
        m_dc_discs[i].j_ct = dc_discs[i].j_ct;
        m_da_discs[i].i1_ct = dc_discs[i].i1_ct;
        m_dc_discs[i].j1_ct = dc_discs[i].j1_ct;
    }

    for(int i=0;i<m_n_rb;i++)
    {
        m_rb[i].meas = rb[i].meas;
        m_rb[i].p_i_ct = rb[i].p_i_ct;
        m_rb[i].p_j_ct = rb[i].p_j_ct;
        m_rb[i].i_ct = rb[i].i_ct;
        m_rb[i].j_ct = rb[i].j_ct;
        m_rb[i].i1_ct = rb[i].i1_ct;
        m_rb[i].j1_ct = rb[i].j1_ct;
    }

    for(int i=0;i<m_n_geotropy;i++)
    {
        m_geotropy[i].meas = geotropy[i].meas;
        m_geotropy[i].p_i_ct = geotropy[i].p_i_ct;
        m_geotropy[i].p_j_ct = geotropy[i].p_j_ct;
        m_geotropy[i].i_ct = geotropy[i].i_ct;
        m_geotropy[i].j_ct = geotropy[i].j_ct;
        m_geotropy[i].i1_ct = geotropy[i].i1_ct;
        m_geotropy[i].j1_ct = geotropy[i].j1_ct;
    }
    for(int i=0;i<m_term_n_geotropy;i++)
    {
        m_term_geotropy[i].meas = term_geotropy[i].meas;
        m_term_geotropy[i].p_i_ct = term_geotropy[i].p_i_ct;
        m_term_geotropy[i].p_j_ct = term_geotropy[i].p_j_ct;
        m_term_geotropy[i].i_ct = term_geotropy[i].i_ct;
        m_term_geotropy[i].j_ct = term_geotropy[i].j_ct;
        m_term_geotropy[i].i1_ct = term_geotropy[i].i1_ct;
        m_term_geotropy[i].j1_ct = term_geotropy[i].j1_ct;
    }

    for(int i=0;i<m_n_logeps_box;i++)
    {
        m_logeps_box[i].meas = logeps_box[i].meas;
        m_logeps_box[i].p_i_ct = logeps_box[i].p_i_ct;
        m_logeps_box[i].p_j_ct = logeps_box[i].p_j_ct;
        m_logeps_box[i].i_ct = logeps_box[i].i_ct;
        m_logeps_box[i].j_ct = logeps_box[i].j_ct;
        m_logeps_box[i].i1_ct = logeps_box[i].i1_ct;
        m_logeps_box[i].j1_ct = logeps_box[i].j1_ct;
    }

    for(int i=0;i<m_n_logneps_box;i++)
    {
        m_logneps_box[i].meas = logneps_box[i].meas;
        m_logneps_box[i].p_i_ct = logneps_box[i].p_i_ct;
        m_logneps_box[i].p_j_ct = logneps_box[i].p_j_ct;
        m_logneps_box[i].i_ct = logneps_box[i].i_ct;
        m_logneps_box[i].j_ct = logneps_box[i].j_ct;
        m_logneps_box[i].i1_ct = logneps_box[i].i1_ct;
        m_logneps_box[i].j1_ct = logneps_box[i].j1_ct;
    }
    for(int i=0;i<m_n_angle;i++)
    {
        m_angle[i].meas = angle[i].meas;
        m_angle[i].p_i_ct = angle[i].p_i_ct;
        m_angle[i].p_j_ct = angle[i].p_j_ct;
        m_angle[i].i_ct = angle[i].i_ct;
        m_angle[i].j_ct = angle[i].j_ct;
        m_angle[i].i1_ct = angle[i].i1_ct;
        m_angle[i].j1_ct = angle[i].j1_ct;
    }
    for(int i=0;i<m_n_br_spacing;i++)
    {
        m_br_spacing[i].meas = br_spacing[i].meas;
        m_br_spacing[i].p_i_ct = br_spacing[i].p_i_ct;
        m_br_spacing[i].p_j_ct = br_spacing[i].p_j_ct;
        m_br_spacing[i].i_ct = br_spacing[i].i_ct;
        m_br_spacing[i].j_ct = br_spacing[i].j_ct;
        m_br_spacing[i].i1_ct = br_spacing[i].i1_ct;
        m_br_spacing[i].j1_ct = br_spacing[i].j1_ct;
    }
}

void GLWidget::cal_dc(int imax, int jmax)
{
    int           i,j,radius;
    Bool          too_large;
    Ij_pos        circle_buf[MAX_CIRC];
    int           n_circle_buf;

    int c = terminal_amount;
    ReducedTerm(im_term1,imax,jmax,c);

    for (i = 1; i < imax - 1; i++)
    {
      for (j = 1; j < jmax - 1; j++)
      {
        if ( im_term1[i][j] != 0 && im_term1[i][j] != 1)
        {
            radius = 1;
            too_large = FALSE;
            while ( (radius < MAX_RADIUS) && !too_large )
            {
              n_circle_buf = 0;
              mich_circle( im_contour, imax, jmax,
              i, j, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
              too_large =
              test_disc_too_large( im_in, imax, jmax, circle_buf, n_circle_buf );
              if (too_large)
              {
                add_result( (Real)radius, dc_discs, &n_dc_discs, i, j, 0, 0, 0, 0 );
              } // if too_large
              radius++;
            } // while
            if (radius == MAX_RADIUS){
              printf("fra: warning MAX_RADIUS ");
            } // if radius
        } // if term==1
      } // j
    } // i
} // cal_dc
// the latest before new one
void GLWidget::cal_br_spacing(int imax, int jmax)
{
    int           i,j,radius;
    Bool          too_large;
    Ij_pos        circle_buf[MAX_CIRC];
    int           n_circle_buf;

    for (i = 1; i < imax - 1; i++)
    {
      for (j = 1; j < jmax - 1; j++)
      {
        if ( im_term1[i][j] != 0 && im_term1[i][j] != 1 )
        {
            radius = 2;
            too_large = FALSE;
            while ( (radius < MAX_RADIUS) && !too_large )
            {
              n_circle_buf = 0;
              mich_circle( im_contour, imax, jmax,i, j, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
              too_large = test_disc_too_large_br( im_skel, imax, jmax, circle_buf, n_circle_buf );
              if (too_large)
              {
                add_result( (Real)radius, br_spacing, &n_br_spacing, i, j, 0, 0, 0, 0 );
              } // if too_large
              radius++;
            } // while
            if (radius == MAX_RADIUS){
              printf("fra: warning MAX_RADIUS ");
            } // if radius
        } // if im_term1
      } // j
    } // i
} // br_spacing


void GLWidget::dump_prt(const char* savedname)
{
  // dumps all results on the file fra.prt
  QVector<QString> header;
  File          *fptr;

  if ((fptr = fopen(savedname, "w")) == NULL) printf("Error can't create file .sta");
  else
  { // writing all results on csv file for all files and on sta for the current saved file
      header.append("Filename");
      header.append("Scale Unit");
      header.append("The height of object");
      header.append("The width of object");
      header.append("Area");
      header.append("Contour");
      header.append("Fractal Dimension");

      header.append("da_discs_n");
      header.append("da_discs_min");
      header.append("da_discs_max");
      header.append("da_discs_avg");
      header.append("da_discs_std_dev");

      header.append("db_discs_n");
      header.append("db_discs_min");
      header.append("db_discs_max");
      header.append("db_discs_avg");
      header.append("db_discs_std_dev");

      header.append("dc_discs_n");
      header.append("dc_discs_min");
      header.append("dc_discs_max");
      header.append("dc_discs_avg");
      header.append("dc_discs_std_dev");

      header.append("branch_length_n");
      header.append("branch_length_min");
      header.append("branch_length_max");
      header.append("branch_length_avg");
      header.append("branch_length_std_dev");

      header.append("geotropy_n");
      header.append("geotropy_min");
      header.append("geotropy_max");
      header.append("geotropy_avg");
      header.append("geotropy_std_dev");

      header.append("term_geotropy_n");
      header.append("term_geotropy_min");
      header.append("term_geotropy_max");
      header.append("term_geotropy_avg");
      header.append("term_geotropy_std_dev");

      header.append("br_spacing_n");
      header.append("br_spacing_min");
      header.append("br_spacing_max");
      header.append("br_spacing_avg");
      header.append("br_spacing_std_dev");

      header.append("branch_angle_n");
      header.append("branch_angle_min");
      header.append("branch_angle_max");
      header.append("branch_angle_avg");
      header.append("branch_angle_std_dev");

      header.append("Ultimate_branch_n");
      header.append("Ultimate_branch_min");
      header.append("Ultimate_branch_max");
      header.append("Ultimate_branch_avg");
      header.append("Ultimate_branch_std_dev");

      header.append("After Tree Configuration");

      header.append("Tree_branch_length_n");
      header.append("Tree_branch_length_min");
      header.append("Tree_branch_length_max");
      header.append("Tree_branch_length_avg");
      header.append("Tree_branch_length_std_dev");

      header.append("Tree_branch_order_n");

      header.append("Tree_branch_angle_n");
      header.append("Tree_branch_angle_min");
      header.append("Tree_branch_angle_max");
      header.append("Tree_branch_angle_avg");
      header.append("Tree_branch_angle_std_dev");

      header.append("Tree_Non_Ultimate_branch_angle_n");
      header.append("Tree_Non_Ultimate_branch_angle_min");
      header.append("Tree_Non_Ultimate_branch_angle_max");
      header.append("Tree_Non_Ultimate_branch_angle_avg");
      header.append("Tree_Non_Ultimate_branch_angle_std_dev");
     for(int i=0;i<header.size();i++)
     {
       //const char* header_tofile = header.at(i).toUtf8().constData();
       //fprintf(fptr,"%s\t",header_tofile);
       fprintf(fptr,"%s\t",header.at(i).toUtf8().constData());
     }
     fprintf(fptr, "\n");

    for(int i=0;i<n_da_discs;i++)
        prt_da_discs[i].meas=2*(da_discs[i].meas/ppmm);

    for(int i=0;i<n_db_discs;i++)
        prt_db_discs[i].meas=2*(db_discs[i].meas/ppmm);

    for(int i=0;i<n_dc_discs;i++)
        prt_dc_discs[i].meas=2*(dc_discs[i].meas/ppmm);

    for(int i=0;i<n_br_spacing;i++)
        prt_br_spacing[i].meas=2*(br_spacing[i].meas/ppmm);

    if(bool_correctrb)
    {
        for(int i=0;i<WRRB_count_BeforeTreeCon;i++)
            prt_rb[i].meas=WithoutRedundantRB_BeforeTreeCon[i].meas/ppmm;
    }
    else
    {
        for(int i=0;i<n_rb;i++)
            prt_rb[i].meas=rb[i].meas/ppmm;
    } // else

    for(int i=0;i<BOrder1_Count;i++)
        prt_urb[i].meas = Branch_Order[0][i].meas;

    int a_count=0;
    Real a_min = 361.0;
    Real a_max = -1.0;
    Real a_sum = 0.0;
    Real a_avg = 0.0;
    Real a_std_dev = 0.0;
    Real a_variance = 0.0;
    Real a_sqrt_sum = 0.0;


    for(int ai=0;ai<MAXJUNC;ai++)
    {
        if(AngleResult[ai][0]>0)
        {
            a_count+=AngleResult[ai][1];
            for(int aj=2;aj<ANGLEATT;aj++)
            {
                if(AngleResult[ai][aj]!=0)
                {
                    a_sum+=AngleResult[ai][aj];
                    a_sqrt_sum+=AngleResult[ai][aj]*AngleResult[ai][aj];
                    if(AngleResult[ai][aj]>a_max) a_max = AngleResult[ai][aj];
                    if(AngleResult[ai][aj]<a_min) a_min = AngleResult[ai][aj];
                }
            } // for aj
        }
    } // for ai
    a_avg = a_sum/a_count;
    // standard deviation
    if (a_count > 1)
    {
      a_variance = 1.0 / ((Real)(a_count - 1)) * (a_sqrt_sum - (Real)a_count * (a_avg*a_avg));
    }
    else if (a_count == 1)
    {
      a_variance = 0.0;
    }
    a_std_dev = sqrt(a_variance);


    Medge brTemp[1000];
    int realbrcount = 0;
    for(int i=0;i<WRRB_count;i++)
    {
        if(newRB[i].val > 0)
        {
            brTemp[i].meas = newRB[i].val/ppmm;
          realbrcount++;
        } // if
    }

    for( int i=0;i<max_Branch_Order;i++)
    {
        Medge BrOrTemp[100];
        for(int j=0;j<BrOr_count[i];j++)
        {
            BrOrTemp[j].meas = branch_Order_array[i][j].val/ppmm;
        } //j

    } // for i : each branch order

    int BrAngleAmt_Count = 0;
    Medge BrAngles[5000];
    int ca = 0;

        for(int i=0;i<n_da_discs;i++)
        {
            if(JuncWithSortedAngle[i].size()>0)
            {
                if(JuncWithSortedAngle[i].at(1)>0)
                     BrAngleAmt_Count += JuncWithSortedAngle[i].at(1);
                else BrAngleAmt_Count++;
                if(JuncWithSortedAngle[i].size()>2)
                {
                    for(int j=2;j<JuncWithSortedAngle[i].size();j++)
                    {
                        BrAngles[ca].meas = JuncWithSortedAngle[i].at(j);
                        ca++;
                    } // for j
                } // // if
            }// if
        } // for i

    int NLBrAngleAmt_Count = 0;
    Medge NLBrAngles[5000];
    int nlca = 0;

        for(int i=0;i<NLeaveCount;i++)
        {
            if(VSortedAngleJuncNotLeaves[i].size()>0)
            {
                if(VSortedAngleJuncNotLeaves[i].at(1)>0)
                     NLBrAngleAmt_Count += VSortedAngleJuncNotLeaves[i].at(1);
                else NLBrAngleAmt_Count++;

                for(int j=2;j<VSortedAngleJuncNotLeaves[i].size();j++)
                {
                    NLBrAngles[nlca].meas = VSortedAngleJuncNotLeaves[i].at(j);
                    nlca++;
                } // for j
            }// if
        } // for i


    // write data as the format of csv into a file

    QVector<float> summary_data;
    summary_data.append(figure_length_y/ppmm);
    summary_data.append(figure_length_x/ppmm);
    summary_data.append(PictureAreaMM);
    summary_data.append(ContourSize/ppmm);
    summary_data.append(fractalDim);

    summary_data.append(n_da_discs);
    summary_data.append(findMin(prt_da_discs,n_da_discs));
    summary_data.append(findMax(prt_da_discs,n_da_discs));
    summary_data.append(findAvg(prt_da_discs,n_da_discs));
    summary_data.append(findS_dev(prt_da_discs,n_da_discs));

    summary_data.append(n_db_discs);
    summary_data.append(findMin(prt_db_discs,n_db_discs));
    summary_data.append(findMax(prt_db_discs,n_db_discs));
    summary_data.append(findAvg(prt_db_discs,n_db_discs));
    summary_data.append(findS_dev(prt_db_discs,n_db_discs));

    summary_data.append(n_dc_discs);
    summary_data.append(findMin(prt_dc_discs,n_dc_discs));
    summary_data.append(findMax(prt_dc_discs,n_dc_discs));
    summary_data.append(findAvg(prt_dc_discs,n_dc_discs));
    summary_data.append(findS_dev(prt_dc_discs,n_dc_discs));

    if(bool_correctrb)
    {
        summary_data.append(WRRB_count_BeforeTreeCon);
        summary_data.append(findMin(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findMax(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findAvg(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findS_dev(prt_rb,WRRB_count_BeforeTreeCon));
    }
    else
    {
        summary_data.append(n_rb);
        summary_data.append(findMin(prt_rb,n_rb));
        summary_data.append(findMax(prt_rb,n_rb));
        summary_data.append(findAvg(prt_rb,n_rb));
        summary_data.append(findS_dev(prt_rb,n_rb));
    }


    summary_data.append(n_geotropy);
    summary_data.append(findMin(geotropy,n_geotropy));
    summary_data.append(findMax(geotropy,n_geotropy));
    summary_data.append(findAvg(geotropy,n_geotropy));
    summary_data.append(findS_dev(geotropy,n_geotropy));

    summary_data.append(term_n_geotropy);
    summary_data.append(findMin(term_geotropy,term_n_geotropy));
    summary_data.append(findMax(term_geotropy,term_n_geotropy));
    summary_data.append(findAvg(term_geotropy,term_n_geotropy));
    summary_data.append(findS_dev(term_geotropy,term_n_geotropy));

    summary_data.append(n_br_spacing);
    summary_data.append(findMin(prt_br_spacing,n_br_spacing));
    summary_data.append(findMax(prt_br_spacing,n_br_spacing));
    summary_data.append(findAvg(prt_br_spacing,n_br_spacing));
    summary_data.append(findS_dev(prt_br_spacing,n_br_spacing));

    summary_data.append(a_count);
    summary_data.append(a_min);
    summary_data.append(a_max);
    summary_data.append(a_avg);
    summary_data.append(a_std_dev);

    summary_data.append(BOrder1_Count);
    summary_data.append(findMin(prt_urb,BOrder1_Count));
    summary_data.append(findMax(prt_urb,BOrder1_Count));
    summary_data.append(findAvg(prt_urb,BOrder1_Count));
    summary_data.append(findS_dev(prt_urb,BOrder1_Count));

    summary_data.append(0);

    summary_data.append(realbrcount);
    summary_data.append(findMin(brTemp,realbrcount));
    summary_data.append(findMax(brTemp,realbrcount));
    summary_data.append(findAvg(brTemp,realbrcount));
    summary_data.append(findS_dev(brTemp,realbrcount));

    summary_data.append(max_Branch_Order);

    summary_data.append(BrAngleAmt_Count);
    summary_data.append(findMin(BrAngles,BrAngleAmt_Count));
    summary_data.append(findMax(BrAngles,BrAngleAmt_Count));
    summary_data.append(findAvg(BrAngles,BrAngleAmt_Count));
    summary_data.append(findS_dev(BrAngles,BrAngleAmt_Count));

    summary_data.append(NLBrAngleAmt_Count);
    summary_data.append(findMin(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findMax(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findAvg(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findS_dev(NLBrAngles,NLBrAngleAmt_Count));

    QString ftemp_input = filename_dump_prt;
    int ind = ftemp_input.lastIndexOf("/");
    QString ftemp_onlyfilename = ftemp_input.right(ftemp_input.length()-ind-1);

    fprintf( fptr, "%s\t", ftemp_onlyfilename.toUtf8().constData() );
    fprintf( fptr, "%s\t",scaleunit.toUtf8().constData()); // scale Unit

    // write summary data
    for(int i=0;i<summary_data.size();i++)
    {
        fprintf(fptr,"%.2f\t",summary_data.at(i));
    }
    fprintf(fptr, "\n");
    fclose( fptr );
  } // else fptr != NULL

} // dump_prt

void GLWidget::dump_csv(const char* savedname)
{
  // dumps all results to .csv
  QVector<QString> header;
  File          *fptr;
  Real fileSize;


  if ((fptr = fopen(savedname, "a+")) == NULL){
        printf("Error can't create file");
  }
  else
  { // writing all results on csv file for all files and on sta for the current saved file
      header.append("Filename");
      header.append("Scale Unit");
      header.append("The height of object");
      header.append("The width of object");
      header.append("Area");
      header.append("Contour");
      header.append("Fractal Dimension");

      header.append("da_discs_n");
      header.append("da_discs_min");
      header.append("da_discs_max");
      header.append("da_discs_avg");
      header.append("da_discs_std_dev");

      header.append("db_discs_n");
      header.append("db_discs_min");
      header.append("db_discs_max");
      header.append("db_discs_avg");
      header.append("db_discs_std_dev");

      header.append("dc_discs_n");
      header.append("dc_discs_min");
      header.append("dc_discs_max");
      header.append("dc_discs_avg");
      header.append("dc_discs_std_dev");

      header.append("branch_length_n");
      header.append("branch_length_min");
      header.append("branch_length_max");
      header.append("branch_length_avg");
      header.append("branch_length_std_dev");

      header.append("geotropy_n");
      header.append("geotropy_min");
      header.append("geotropy_max");
      header.append("geotropy_avg");
      header.append("geotropy_std_dev");

      header.append("term_geotropy_n");
      header.append("term_geotropy_min");
      header.append("term_geotropy_max");
      header.append("term_geotropy_avg");
      header.append("term_geotropy_std_dev");

      header.append("br_spacing_n");
      header.append("br_spacing_min");
      header.append("br_spacing_max");
      header.append("br_spacing_avg");
      header.append("br_spacing_std_dev");

      header.append("branch_angle_n");
      header.append("branch_angle_min");
      header.append("branch_angle_max");
      header.append("branch_angle_avg");
      header.append("branch_angle_std_dev");

      header.append("Ultimate_branch_n");
      header.append("Ultimate_branch_min");
      header.append("Ultimate_branch_max");
      header.append("Ultimate_branch_avg");
      header.append("Ultimate_branch_std_dev");

      header.append("After Tree Configuration");

      header.append("Tree_branch_length_n");
      header.append("Tree_branch_length_min");
      header.append("Tree_branch_length_max");
      header.append("Tree_branch_length_avg");
      header.append("Tree_branch_length_std_dev");

      header.append("Tree_branch_order_n");

      header.append("Tree_branch_angle_n");
      header.append("Tree_branch_angle_min");
      header.append("Tree_branch_angle_max");
      header.append("Tree_branch_angle_avg");
      header.append("Tree_branch_angle_std_dev");

      header.append("Tree_Non_Ultimate_branch_angle_n");
      header.append("Tree_Non_Ultimate_branch_angle_min");
      header.append("Tree_Non_Ultimate_branch_angle_max");
      header.append("Tree_Non_Ultimate_branch_angle_avg");
      header.append("Tree_Non_Ultimate_branch_angle_std_dev");

     fseek(fptr, 0, SEEK_END);
     fileSize = ftell(fptr);
     if(abs(fileSize-0)<0.0001)
     {
         for(int i=0;i<header.size();i++)
         {
	   //const char* header_tofile = header.at(i).toUtf8().constData();
           //fprintf(fptr,"%s\t",header_tofile);
	   fprintf(fptr,"%s\t",header.at(i).toUtf8().constData());
         }
         fprintf(fptr, "\n");

     }
    for(int i=0;i<n_da_discs;i++)
    {
        prt_da_discs[i].meas=2*(da_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_db_discs;i++)
    {
          prt_db_discs[i].meas=2*(db_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_dc_discs;i++)
    {
        prt_dc_discs[i].meas=2*(dc_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_br_spacing;i++)
    {
        prt_br_spacing[i].meas=2*(br_spacing[i].meas/ppmm);
    }

    if(bool_correctrb)
    {
        for(int i=0;i<WRRB_count_BeforeTreeCon;i++)
        {
            prt_rb[i].meas=WithoutRedundantRB_BeforeTreeCon[i].meas/ppmm;
        } // for
    }
    else
    {
        for(int i=0;i<n_rb;i++)
        {
            prt_rb[i].meas=rb[i].meas/ppmm;
        }   // for
    } // else

    for(int i=0;i<BOrder1_Count;i++)
    {
        prt_urb[i].meas = Branch_Order[0][i].meas;
    }

    int a_count=0;
    Real a_min = 361.0;
    Real a_max = -1.0;
    Real a_sum = 0.0;
    Real a_avg = 0.0;
    Real a_std_dev = 0.0;
    Real a_variance = 0.0;
    Real a_sqrt_sum = 0.0;


    for(int ai=0;ai<MAXJUNC;ai++)
    {
        if(AngleResult[ai][0]>0)
        {
            a_count+=AngleResult[ai][1];
            for(int aj=2;aj<ANGLEATT;aj++)
            {
                if(AngleResult[ai][aj]!=0)
                {
                    a_sum+=AngleResult[ai][aj];
                    a_sqrt_sum+=AngleResult[ai][aj]*AngleResult[ai][aj];
                    if(AngleResult[ai][aj]>a_max) a_max = AngleResult[ai][aj];
                    if(AngleResult[ai][aj]<a_min) a_min = AngleResult[ai][aj];
                }
            } // for aj
        }
    } // for ai
    a_avg = a_sum/a_count;
    // standard deviation
    if (a_count > 1)
    {
      a_variance = 1.0 / ((Real)(a_count - 1)) * (a_sqrt_sum - (Real)a_count * (a_avg*a_avg));
    }
    else if (a_count == 1)
    {
      a_variance = 0.0;
    }
    a_std_dev = sqrt(a_variance);


    Medge brTemp[1000];
    int realbrcount = 0;
    for(int i=0;i<WRRB_count;i++)
    {
        if(newRB[i].val > 0)
        {
            brTemp[i].meas = newRB[i].val/ppmm;
          realbrcount++;
        } // if
    }

    for( int i=0;i<max_Branch_Order;i++)
    {
        Medge BrOrTemp[100];
        for(int j=0;j<BrOr_count[i];j++)
        {
            BrOrTemp[j].meas = branch_Order_array[i][j].val/ppmm;
        } //j

    } // for i : each branch order


    int BrAngleAmt_Count = 0;
    Medge BrAngles[5000];
    int ca = 0;

        for(int i=0;i<n_da_discs;i++)
        {
            if(JuncWithSortedAngle[i].size()>0)
            {
                if(JuncWithSortedAngle[i].at(1)>0)
                     BrAngleAmt_Count += JuncWithSortedAngle[i].at(1);
                else BrAngleAmt_Count++;
                if(JuncWithSortedAngle[i].size()>2)
                {
                    for(int j=2;j<JuncWithSortedAngle[i].size();j++)
                    {
                        BrAngles[ca].meas = JuncWithSortedAngle[i].at(j);
                        ca++;
                    } // for j
                } // // if
            }// if
        } // for i

    int NLBrAngleAmt_Count = 0;
    Medge NLBrAngles[5000];
    int nlca = 0;

        for(int i=0;i<NLeaveCount;i++)
        {
            if(VSortedAngleJuncNotLeaves[i].size()>0)
            {
                if(VSortedAngleJuncNotLeaves[i].at(1)>0)
                     NLBrAngleAmt_Count += VSortedAngleJuncNotLeaves[i].at(1);
                else NLBrAngleAmt_Count++;

                for(int j=2;j<VSortedAngleJuncNotLeaves[i].size();j++)
                {
                    NLBrAngles[nlca].meas = VSortedAngleJuncNotLeaves[i].at(j);
                    nlca++;
                } // for j
            }// if
        } // for i


    // write data as the format of csv into a file

    QVector<float> summary_data;
    summary_data.append(figure_length_y/ppmm);
    summary_data.append(figure_length_x/ppmm);
    summary_data.append(PictureAreaMM);
    summary_data.append(ContourSize/ppmm);
    summary_data.append(fractalDim);

    summary_data.append(n_da_discs);
    summary_data.append(findMin(prt_da_discs,n_da_discs));
    summary_data.append(findMax(prt_da_discs,n_da_discs));
    summary_data.append(findAvg(prt_da_discs,n_da_discs));
    summary_data.append(findS_dev(prt_da_discs,n_da_discs));

    summary_data.append(n_db_discs);
    summary_data.append(findMin(prt_db_discs,n_db_discs));
    summary_data.append(findMax(prt_db_discs,n_db_discs));
    summary_data.append(findAvg(prt_db_discs,n_db_discs));
    summary_data.append(findS_dev(prt_db_discs,n_db_discs));

    summary_data.append(n_dc_discs);
    summary_data.append(findMin(prt_dc_discs,n_dc_discs));
    summary_data.append(findMax(prt_dc_discs,n_dc_discs));
    summary_data.append(findAvg(prt_dc_discs,n_dc_discs));
    summary_data.append(findS_dev(prt_dc_discs,n_dc_discs));

    if(bool_correctrb)
    {
        summary_data.append(WRRB_count_BeforeTreeCon);
        summary_data.append(findMin(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findMax(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findAvg(prt_rb,WRRB_count_BeforeTreeCon));
        summary_data.append(findS_dev(prt_rb,WRRB_count_BeforeTreeCon));
    }
    else
    {
        summary_data.append(n_rb);
        summary_data.append(findMin(prt_rb,n_rb));
        summary_data.append(findMax(prt_rb,n_rb));
        summary_data.append(findAvg(prt_rb,n_rb));
        summary_data.append(findS_dev(prt_rb,n_rb));
    }
    summary_data.append(n_geotropy);
    summary_data.append(findMin(geotropy,n_geotropy));
    summary_data.append(findMax(geotropy,n_geotropy));
    summary_data.append(findAvg(geotropy,n_geotropy));
    summary_data.append(findS_dev(geotropy,n_geotropy));

    summary_data.append(term_n_geotropy);
    summary_data.append(findMin(term_geotropy,term_n_geotropy));
    summary_data.append(findMax(term_geotropy,term_n_geotropy));
    summary_data.append(findAvg(term_geotropy,term_n_geotropy));
    summary_data.append(findS_dev(term_geotropy,term_n_geotropy));

    summary_data.append(n_br_spacing);
    summary_data.append(findMin(prt_br_spacing,n_br_spacing));
    summary_data.append(findMax(prt_br_spacing,n_br_spacing));
    summary_data.append(findAvg(prt_br_spacing,n_br_spacing));
    summary_data.append(findS_dev(prt_br_spacing,n_br_spacing));

    summary_data.append(a_count);
    summary_data.append(a_min);
    summary_data.append(a_max);
    summary_data.append(a_avg);
    summary_data.append(a_std_dev);

    summary_data.append(BOrder1_Count);
    summary_data.append(findMin(prt_urb,BOrder1_Count));
    summary_data.append(findMax(prt_urb,BOrder1_Count));
    summary_data.append(findAvg(prt_urb,BOrder1_Count));
    summary_data.append(findS_dev(prt_urb,BOrder1_Count));

    summary_data.append(0);

    summary_data.append(realbrcount);
    summary_data.append(findMin(brTemp,realbrcount));
    summary_data.append(findMax(brTemp,realbrcount));
    summary_data.append(findAvg(brTemp,realbrcount));
    summary_data.append(findS_dev(brTemp,realbrcount));

    summary_data.append(max_Branch_Order);

    summary_data.append(BrAngleAmt_Count);
    summary_data.append(findMin(BrAngles,BrAngleAmt_Count));
    summary_data.append(findMax(BrAngles,BrAngleAmt_Count));
    summary_data.append(findAvg(BrAngles,BrAngleAmt_Count));
    summary_data.append(findS_dev(BrAngles,BrAngleAmt_Count));

    summary_data.append(NLBrAngleAmt_Count);
    summary_data.append(findMin(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findMax(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findAvg(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findS_dev(NLBrAngles,NLBrAngleAmt_Count));

    QString ftemp_input = filename_dump_prt;
    int ind = ftemp_input.lastIndexOf("/");
    QString ftemp_onlyfilename = ftemp_input.right(ftemp_input.length()-ind-1);

    fprintf( fptr, "%s\t", ftemp_onlyfilename.toUtf8().constData() );
    fprintf( fptr, "%s\t",scaleunit.toUtf8().constData()); // scale Unit

    // write summary data
    for(int i=0;i<summary_data.size();i++)
    {
        fprintf(fptr,"%.2f\t",summary_data.at(i));
    }
    fprintf(fptr, "\n");

    fclose( fptr );
  } // else fptr != NULL

} // dump_csv


void GLWidget::dump_prt_temp(const char* savedname)
{
  // dumps all results on the file fra.prt
  QVector<QString> header;
  File          *fptr;
  Real fileSize;

  header.append("Filename");
  header.append("Scale Unit");
  header.append("The height of object");
  header.append("The width of object");
  header.append("Area");
  header.append("Contour");
  header.append("Fractal Dimension");

  header.append("da_discs_n");
  header.append("da_discs_min");
  header.append("da_discs_max");
  header.append("da_discs_avg");
  header.append("da_discs_std_dev");

  header.append("db_discs_n");
  header.append("db_discs_min");
  header.append("db_discs_max");
  header.append("db_discs_avg");
  header.append("db_discs_std_dev");

  header.append("dc_discs_n");
  header.append("dc_discs_min");
  header.append("dc_discs_max");
  header.append("dc_discs_avg");
  header.append("dc_discs_std_dev");

  header.append("branch_length_n");
  header.append("branch_length_min");
  header.append("branch_length_max");
  header.append("branch_length_avg");
  header.append("branch_length_std_dev");

  header.append("geotropy_n");
  header.append("geotropy_min");
  header.append("geotropy_max");
  header.append("geotropy_avg");
  header.append("geotropy_std_dev");

  header.append("term_geotropy_n");
  header.append("term_geotropy_min");
  header.append("term_geotropy_max");
  header.append("term_geotropy_avg");
  header.append("term_geotropy_std_dev");

  header.append("br_spacing_n");
  header.append("br_spacing_min");
  header.append("br_spacing_max");
  header.append("br_spacing_avg");
  header.append("br_spacing_std_dev");

  header.append("branch_angle_n");
  header.append("branch_angle_min");
  header.append("branch_angle_max");
  header.append("branch_angle_avg");
  header.append("branch_angle_std_dev");

  header.append("Ultimate_branch_n");
  header.append("Ultimate_branch_min");
  header.append("Ultimate_branch_max");
  header.append("Ultimate_branch_avg");
  header.append("Ultimate_branch_std_dev");

  header.append("After Tree Configuration");

  header.append("Tree_branch_length_n");
  header.append("Tree_branch_length_min");
  header.append("Tree_branch_length_max");
  header.append("Tree_branch_length_avg");
  header.append("Tree_branch_length_std_dev");

  header.append("Tree_branch_order_n");

  header.append("Tree_branch_angle_n");
  header.append("Tree_branch_angle_min");
  header.append("Tree_branch_angle_max");
  header.append("Tree_branch_angle_avg");
  header.append("Tree_branch_angle_std_dev");

  header.append("Tree_Non_Ultimate_branch_angle_n");
  header.append("Tree_Non_Ultimate_branch_angle_min");
  header.append("Tree_Non_Ultimate_branch_angle_max");
  header.append("Tree_Non_Ultimate_branch_angle_avg");
  header.append("Tree_Non_Ultimate_branch_angle_std_dev");


  if ( (fptr = fopen(savedname, "w")) == NULL){
      printf("fra: error can't create file fra.prt");
  }
  else
  { // writing all results on fra.prt

    for(int i=0;i<n_da_discs;i++)
    {
        prt_da_discs[i].meas=2*(da_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_db_discs;i++)
    {
          prt_db_discs[i].meas=2*(db_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_dc_discs;i++)
    {
        prt_dc_discs[i].meas=2*(dc_discs[i].meas/ppmm);
    }

    for(int i=0;i<n_br_spacing;i++)
    {
        prt_br_spacing[i].meas=2*(br_spacing[i].meas/ppmm);
    }

    for(int i=0;i<n_rb;i++)
    {
        prt_rb[i].meas=rb[i].meas/ppmm;
    }

    for(int i=0;i<BOrder1_Count;i++)
    {
        prt_urb[i].meas = Branch_Order[0][i].meas;
    }

    int a_count=0;
    Real a_min = 361.0;
    Real a_max = -1.0;
    Real a_sum = 0.0;
    Real a_avg = 0.0;
    Real a_std_dev = 0.0;
    Real a_variance = 0.0;
    Real a_sqrt_sum = 0.0;


    for(int ai=0;ai<MAXJUNC;ai++)
    {
        if(AngleResult[ai][0]>0)
        {
            a_count+=AngleResult[ai][1];
            for(int aj=2;aj<ANGLEATT;aj++)
            {
                if(AngleResult[ai][aj]!=0)
                {
                    a_sum+=AngleResult[ai][aj];
                    a_sqrt_sum+=AngleResult[ai][aj]*AngleResult[ai][aj];
                    if(AngleResult[ai][aj]>a_max) a_max = AngleResult[ai][aj];
                    if(AngleResult[ai][aj]<a_min) a_min = AngleResult[ai][aj];
                }
            } // for aj
        }
    } // for ai
    a_avg = a_sum/a_count;
    // standard deviation
    if (a_count > 1)
    {
      a_variance = 1.0 / ((Real)(a_count - 1)) * (a_sqrt_sum - (Real)a_count * (a_avg*a_avg));
    }
    else if (a_count == 1)
    {
      a_variance = 0.0;
    }
    a_std_dev = sqrt(a_variance);

    Medge brTemp[1000];
    int realbrcount = 0;
    for(int i=0;i<WRRB_count;i++)
    {
        if(newRB[i].val > 0)
        {
            brTemp[i].meas = newRB[i].val/ppmm;
          realbrcount++;
        } // if
    }
    for( int i=0;i<max_Branch_Order;i++)
    {
        Medge BrOrTemp[100];
        for(int j=0;j<BrOr_count[i];j++)
        {
            BrOrTemp[j].meas = branch_Order_array[i][j].val/ppmm;
        } //j

    } // for i : each branch order


    int BrAngleAmt_Count = 0;
    Medge BrAngles[5000];
    int ca = 0;

        for(int i=0;i<n_da_discs;i++)
        {
            if(JuncWithSortedAngle[i].size()>0)
            {
                if(JuncWithSortedAngle[i].at(1)>0)
                     BrAngleAmt_Count += JuncWithSortedAngle[i].at(1);
                else BrAngleAmt_Count++;
                if(JuncWithSortedAngle[i].size()>2)
                {
                    for(int j=2;j<JuncWithSortedAngle[i].size();j++)
                    {
                        BrAngles[ca].meas = JuncWithSortedAngle[i].at(j);
                        ca++;
                    } // for j
                } // // if
            }// if
        } // for i


    int NLBrAngleAmt_Count = 0;
    Medge NLBrAngles[5000];
    int nlca = 0;

        for(int i=0;i<NLeaveCount;i++)
        {
            if(VSortedAngleJuncNotLeaves[i].size()>0)
            {
                if(VSortedAngleJuncNotLeaves[i].at(1)>0)
                     NLBrAngleAmt_Count += VSortedAngleJuncNotLeaves[i].at(1);
                else NLBrAngleAmt_Count++;

                for(int j=2;j<VSortedAngleJuncNotLeaves[i].size();j++)
                {
                    NLBrAngles[nlca].meas = VSortedAngleJuncNotLeaves[i].at(j);
                    nlca++;
                } // for j
            }// if
        } // for i


    // write data as the format of csv into a file

    QVector<float> summary_data;
    summary_data.append(figure_length_y/ppmm);
    summary_data.append(figure_length_x/ppmm);
    summary_data.append(PictureAreaMM);
    summary_data.append(ContourSize/ppmm);
    summary_data.append(fractalDim);

    summary_data.append(n_da_discs);
    summary_data.append(findMin(prt_da_discs,n_da_discs));
    summary_data.append(findMax(prt_da_discs,n_da_discs));
    summary_data.append(findAvg(prt_da_discs,n_da_discs));
    summary_data.append(findS_dev(prt_da_discs,n_da_discs));

    summary_data.append(n_db_discs);
    summary_data.append(findMin(prt_db_discs,n_db_discs));
    summary_data.append(findMax(prt_db_discs,n_db_discs));
    summary_data.append(findAvg(prt_db_discs,n_db_discs));
    summary_data.append(findS_dev(prt_db_discs,n_db_discs));

    summary_data.append(n_dc_discs);
    summary_data.append(findMin(prt_dc_discs,n_dc_discs));
    summary_data.append(findMax(prt_dc_discs,n_dc_discs));
    summary_data.append(findAvg(prt_dc_discs,n_dc_discs));
    summary_data.append(findS_dev(prt_dc_discs,n_dc_discs));

    summary_data.append(n_rb);
    summary_data.append(findMin(prt_rb,n_rb));
    summary_data.append(findMax(prt_rb,n_rb));
    summary_data.append(findAvg(prt_rb,n_rb));
    summary_data.append(findS_dev(prt_rb,n_rb));

    summary_data.append(n_geotropy);
    summary_data.append(findMin(geotropy,n_geotropy));
    summary_data.append(findMax(geotropy,n_geotropy));
    summary_data.append(findAvg(geotropy,n_geotropy));
    summary_data.append(findS_dev(geotropy,n_geotropy));

    summary_data.append(term_n_geotropy);
    summary_data.append(findMin(term_geotropy,term_n_geotropy));
    summary_data.append(findMax(term_geotropy,term_n_geotropy));
    summary_data.append(findAvg(term_geotropy,term_n_geotropy));
    summary_data.append(findS_dev(term_geotropy,term_n_geotropy));

    summary_data.append(n_br_spacing);
    summary_data.append(findMin(prt_br_spacing,n_br_spacing));
    summary_data.append(findMax(prt_br_spacing,n_br_spacing));
    summary_data.append(findAvg(prt_br_spacing,n_br_spacing));
    summary_data.append(findS_dev(prt_br_spacing,n_br_spacing));

    summary_data.append(a_count);
    summary_data.append(a_min);
    summary_data.append(a_max);
    summary_data.append(a_avg);
    summary_data.append(a_std_dev);

    summary_data.append(BOrder1_Count);
    summary_data.append(findMin(prt_urb,BOrder1_Count));
    summary_data.append(findMax(prt_urb,BOrder1_Count));
    summary_data.append(findAvg(prt_urb,BOrder1_Count));
    summary_data.append(findS_dev(prt_urb,BOrder1_Count));

    summary_data.append(0);

    summary_data.append(realbrcount);
    summary_data.append(findMin(brTemp,realbrcount));
    summary_data.append(findMax(brTemp,realbrcount));
    summary_data.append(findAvg(brTemp,realbrcount));
    summary_data.append(findS_dev(brTemp,realbrcount));

    summary_data.append(max_Branch_Order);

    summary_data.append(BrAngleAmt_Count);
    summary_data.append(findMin(BrAngles,BrAngleAmt_Count));
    summary_data.append(findMax(BrAngles,BrAngleAmt_Count));
    summary_data.append(findAvg(BrAngles,BrAngleAmt_Count));
    summary_data.append(findS_dev(BrAngles,BrAngleAmt_Count));

    summary_data.append(NLBrAngleAmt_Count);
    summary_data.append(findMin(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findMax(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findAvg(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(findS_dev(NLBrAngles,NLBrAngleAmt_Count));

    // write header
     for(int i=0;i<header.size();i++)
     {
       // const char* header_tofile = header.at(i).toUtf8().constData();
       // fprintf(fptr,"%s\t",header_tofile);
       fprintf(fptr,"%s\t",header.at(i).toUtf8().constData());
     }
     fprintf(fptr, "\n");

    fprintf( fptr, "%s\t", savedname );
    fprintf( fptr, "%s\t",scaleunit.toUtf8().constData()); // scale Unit

    for(int i=0;i<summary_data.size();i++)
    {
        fprintf(fptr,"%f\t",summary_data.at(i));
    }
    fprintf(fptr, "\n");
    fclose( fptr );
  } // fptr != NULL
} // dump_prt


float GLWidget::findMin(Medge ds[MAX_RES], int n)
{
    float result = ds[0].meas;
    for(int i=0;i<n;i++)
    {
        if(ds[i].meas < result)
            result = ds[i].meas;
    }
  return result;
}

float GLWidget::findMax(Medge ds[MAX_RES], int n)
{
    float result = ds[0].meas;
    for(int i=0;i<n;i++)
    {
        if(ds[i].meas > result)
            result = ds[i].meas;
    }
  return result;
}

float GLWidget::findAvg(Medge ds[MAX_RES], int n)
{
    float result = 0, sum = 0;
    for(int i=0;i<n;i++)
    {
        sum += ds[i].meas;
    }
    result = sum/n;
  return result;
}

float GLWidget::findS_dev(Medge ds[MAX_RES], int n)
{
    float sqr_total = 0, result = 0;
    for (int i = 0; i < n; i++){
        sqr_total += (ds[i].meas*ds[i].meas);
    } /* for i */
    if (n > 1){
      result = 1.0 / ((Real)(n - 1)) * (sqr_total - (Real)n * (findAvg(ds,n)*findAvg(ds,n)));
    }
    else if (n == 1){
      result = 0.0;
    }
     result = bstool->BSsqroot(result);
  return result;
}

void GLWidget::clearComp_var()
{
    m_n_da_discs = 0;
    m_n_db_discs = 0;
    m_n_dc_discs = 0;
    m_n_rb = 0;
    m_n_geotropy = 0;
    m_term_n_geotropy = 0;
    m_n_logeps_box = 0;
    m_n_logneps_box = 0;
    m_box_dimension = 0;
    m_n_angle = 0;
    m_n_br_spacing = 0;
}

void GLWidget::cleanImage(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax)
{
    int count=0;
    for (int i = 1; i < imax - 1; i++)
    {
      for (int j = 1; j < jmax - 1; j++)
      {
          count=0;
          if(im[i][j]==1)
          {
           if ( im[i-1][j+1] == 0 ) count++;
           if ( im[i  ][j+1] == 0 ) count++;
           if ( im[i+1][j+1] == 0 ) count++;
           if ( im[i+1][j  ] == 0 ) count++;
           if ( im[i+1][j-1] == 0 ) count++;
           if ( im[i  ][j-1] == 0 ) count++;
           if ( im[i-1][j-1] == 0 ) count++;
           if ( im[i-1][j  ] == 0 ) count++;
          } // if object==1
          if(count>7) im[i][j]=0;
      } // j
    } // i
} // cleanImage

void GLWidget::cleanSkeleton(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax)
{
    int count=0;
    for (int i = 1; i < imax - 1; i++)
    {
      for (int j = 1; j < jmax - 1; j++)
      {
          count=0;
          if(im[i][j]==1)
          {
           if ( im[i-1][j+1] == 0 ) count++;
           if ( im[i  ][j+1] == 0 ) count++;
           if ( im[i+1][j+1] == 0 ) count++;
           if ( im[i+1][j  ] == 0 ) count++;
           if ( im[i+1][j-1] == 0 ) count++;
           if ( im[i  ][j-1] == 0 ) count++;
           if ( im[i-1][j-1] == 0 ) count++;
           if ( im[i-1][j  ] == 0 ) count++;
          } // if object==1
          if(count==8) im[i][j]=0;
      } // j
    } // i
} // cleanSkeleton

int GLWidget::Contour_Size(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax)
{
    int contoursize = 0;
    for(int i=0;i<imax;i++)
        for(int j=0;j<jmax;j++)
            if(im[i][j]==1) contoursize++;
    return contoursize;
}

int GLWidget::Picture_Area(int im[MAX_ISIZE][MAX_ISIZE],int imax, int jmax)
{
    int picturearea = 0;
    for(int i=0;i<imax;i++)
        for(int j=0;j<jmax;j++)
            if(im[i][j]==1) picturearea++;
    return picturearea;
}

void GLWidget::zoomin(){}

void GLWidget::zoomout(){}

void GLWidget::TrackSkel(int junc[MAX_ISIZE][MAX_ISIZE],int imax, int jmax,int i_start,int j_start,Ij_pos skel_part[MAX_CIRC],int *n_skel_part)
{
 Ij_pos patch[MAX_CIRC];
 Ij_pos border[MAX_CIRC];
 int    in_patch, n_patch;
 int    in_border, n_border;
 bool   ready, found;
 int	max_r, last_i, last_j;

 max_r = 0;
 found = FALSE;

 n_patch = 0;
 if ( (junc[i_start][j_start] == DISC_MARK) ) junc[i_start][j_start] = 1;
 add_site_circle( i_start, j_start, patch, &n_patch );
 add_site_circle( i_start, j_start, skel_part, n_skel_part );
 last_i = i_start;
 last_j = j_start;
 ready = FALSE;

 while( !ready)
 {
   n_border = 0;
   for( in_patch = 0; in_patch< n_patch; in_patch++)
   {
    if ( (junc[patch[in_patch].i_x][patch[in_patch].j_y] == 1) )
      add_neigh_skel1( junc, imax, jmax, patch[in_patch], &n_border, border);
   } // for in_patch

   if ( (n_border == 0) || (*n_skel_part >= max_skel_length) )
     ready = TRUE;      // no new sites added ready
   else
   {
     for( in_border = 0; in_border < n_border ; in_border++)
     {
        last_i = patch[in_border].i_x =  border[in_border].i_x;
        last_j = patch[in_border].j_y =  border[in_border].j_y;
        if ( *n_skel_part < max_skel_length )
        {
          add_site_circle(  border[in_border].i_x, border[in_border].j_y,
          skel_part, n_skel_part );
        }
     } // for in_border
     n_patch = n_border;
   } // !ready
 } // while

} // TrackSkel


void GLWidget::ReportFS()
{

    int count[count_maxskel];
    for(int i=0;i<count_maxskel;i++)
        count[i]=1;

    for(int i=0;i<count_maxskel;i++)
    {
        if(FLength[i][4]>2)
        {
            if(count[i]!=0)
            {
                for(int j=i+1;j<count_maxskel;j++)
                {
                  if(((abs(FLength[i][0]-FLength[j][0])<3) && (abs(FLength[i][1]-FLength[j][1])<3)
                    && (abs(FLength[i][2]-FLength[j][2])<3) && (abs(FLength[i][3]-FLength[j][3])<3))
                    || ((abs(FLength[i][0]-FLength[j][2])<3) && (abs(FLength[i][1]-FLength[j][3])<3)
                    && (abs(FLength[i][2]-FLength[j][0])<3) && (abs(FLength[i][3]-FLength[j][1])<3)))
                    {
                      count[j]=0;
                    } // if

                } // for j
            } // if count != 0
        } // if length > 2
        else
        {
            count[i]=0;
        } // else
    } // for i
    int c = 0;
    for(int i=0;i<count_maxskel;i++)
    {
        if(count[i]!=0)
        {
            c++;
            FLengthTrim[i][0] = FLength[i][0];
            FLengthTrim[i][1] = FLength[i][1];
            FLengthTrim[i][2] = FLength[i][2];
            FLengthTrim[i][3] = FLength[i][3];
            FLengthTrim[i][4] = FLength[i][4];
            FLengthTrim[i][5] = FLength[i][5];
        } // if count <> 0
    } // for i
    count_FLengthTrim = c;
} // ReportFS

bool GLWidget::isSameFS(int x1,int y1,int xn, int yn)
{
    bool result = false;
    for(int i=0;i<count_maxskel;i++)
    {
        if(FLength[i][4]>2)
        {
            if(((x1==FLength[i][0]) && (y1==FLength[i][1])
              && (xn==FLength[i][2]) && (yn==FLength[i][3])) ||
              ((x1==FLength[i][2]) && (y1==FLength[i][3])
              && (xn==FLength[i][0]) && (yn==FLength[i][1])))
              {
                return result = true;
              }
        }
    }
   return result;
} // isSameFS

void GLWidget::KeepAngle(int junclabel,int ai_ct,int aj_ct, int bi_ct,int bj_ct)
{
    int reallabel = junclabel+1;

    for(int i=0;i<junclabel;i++)
    {
        if((abs(AngleRec[i][1]-ai_ct)<2)
           && (abs(AngleRec[i][2]-aj_ct)<2))
        {
            junclabel = i;
            break;
        }
    }
      if((abs(AngleRec[junclabel][1]-ai_ct)<2)
         && (abs(AngleRec[junclabel][2]-aj_ct)<2)) //  update the existing record in the array
      {
        for(int i=3;i<ANGLEATT;i+=2)
        {
            if((abs(AngleRec[junclabel][i]-bi_ct)<2) && (abs(AngleRec[junclabel][i+1]-bj_ct)<2))
            {
                break;
            }
            else
            if(AngleRec[junclabel][i]==0)
            {
                AngleRec[junclabel][i]=bi_ct;
                AngleRec[junclabel][i+1]=bj_ct;
                break;
            }
        }
    }
    else // insert new record into the array
    {
        AngleRec[junclabel][0]= reallabel;
        AngleRec[junclabel][1]= ai_ct;
        AngleRec[junclabel][2]= aj_ct;
        AngleRec[junclabel][3]= bi_ct;
        AngleRec[junclabel][4]= bj_ct;
    }
} // KeepAngle

void GLWidget::ReportAngle()
{
    int temp[count_disc_area][ANGLEATT];
    // update AngleRec by taking out the undetected junctions (no disc A on the junc)

    for(int i=0;i<count_disc_area;i++)
        for(int j=0;j<ANGLEATT;j++)
            temp[i][j] = 0;

    int a=0;
    int cx=0;
    while (a<count_disc_area)
    {
        if(AngleRec[a][0]!=0)
        {
            for(int j=0;j<ANGLEATT;j++)
            {

                temp[cx][j]=AngleRec[a][j];
            } // for  j
            cx++;
        }
        a++;
    } // while

    // clear data in AngleRec
    for(int i=0;i<count_disc_area;i++)
        for(int j=0;j<ANGLEATT;j++)
            AngleRec[i][j] = 0;

    // Copy data in temp(which deleted the unwanted records) into AngleRec
    for(int i=0;i<count_disc_area;i++)
        for(int j=0;j<ANGLEATT;j++)
            AngleRec[i][j] = temp[i][j];

    // order the coordinates by clockwise
    sortingCoord();
} // ReportAngle


Real GLWidget::measure_angle(int ai_ct,int aj_ct,int b0i_ct,int b0j_ct,int b1i_ct,int b1j_ct)
{

  //using vector concept cos x = a.b/|a||b| which can measure angle that more than 90

  Real result=0;
  int a1i=0,b1j=0,a2i=0,b2j=0;
  Real arccos = 0.0;
  a1i = b0j_ct-aj_ct;  // x1
  b1j = b0i_ct-ai_ct;  // y1
  a2i = b1j_ct-aj_ct;  // x2
  b2j = b1i_ct-ai_ct;  // y2

  arccos = (a1i*a2i+b1j*b2j)/(sqrt(pow(a1i,2.0)+pow(b1j,2.0))*sqrt(pow(a2i,2.0)+pow(b2j,2.0)));
  result = (acos(arccos))*180/MYPI;
  return result;
} // measure_angle

void GLWidget::Cal_Angle()
{
    int angle_amt = 0;
    int lastj = 0;
    for(int i=0;i<MAXJUNC && (AngleRec[i][0]!=0);i++)
    {
            angle_amt = 0;
            AngleResult[i][0] = AngleRec[i][0];
            for(int j=3;j<ANGLEATT && (AngleRec[i][j+2]!=0) ;j+=2)
            {
                 AngleResult[i][angle_amt+2] = measure_angle(AngleRec[i][1],AngleRec[i][2],
                                   AngleRec[i][j],AngleRec[i][j+1],
                                   AngleRec[i][j+2],AngleRec[i][j+3]);
                 angle_amt++;
                 lastj = j;
            } // j
            AngleResult[i][angle_amt+2] = measure_angle(AngleRec[i][1],AngleRec[i][2],
                              AngleRec[i][lastj+2],AngleRec[i][lastj+3],
                              AngleRec[i][3],AngleRec[i][4]);

            AngleResult[i][1] =  (angle_amt+1);
            if(AngleResult[i][1]==1) // only 1 branch
            {
               AngleResult[i][2] = measure_angle(AngleRec[i][1],AngleRec[i][2],
                                                 AngleRec[i][1],AngleRec[i][2]+10,
                                                 AngleRec[i][3],AngleRec[i][4]);
            }
            if(AngleResult[i][1]==2) // 2 branches, so the other angle 's degree should be 360- x
            {
                AngleResult[i][3] = 360-AngleResult[i][2];
            }
    } // i

    ExactJuncAmnt = 0;
    for(int i=0;i<MAXJUNC;i++)
    {
        if(AngleResult[i][0]>0)
        {
            ExactJuncAmnt++;
        } // if AngleResult[i][0]>0
    } // for i
} // Cal_Angle

void GLWidget::sortingCoord()
{
    int data[ANGLEATT];
    // clear data
    for(int v=0;v<ANGLEATT;v++)
    {
        data[v] = 0;
    }

    for(int w=0;w<count_disc_area;w++)
    {
        // copy each row of data in AngleRec[][x] into data[x]
        for(int v=0;v<ANGLEATT;v++)
        {
            data[v] = AngleRec[w][v];
        }
        // clear data in AngleRec[w][]
        for(int v=0;v<ANGLEATT;v++)
        {
            AngleRec[w][v]=0;
        }
        // start sorting
        int result[ANGLEATT];

        QList<int> q1x,q1y,q2x,q2y,q3x,q3y,q4x,q4y;
        QList<float> slope1,slope2,slope3,slope4;
        int cy = data[1];
        int cx = data[2];

        for(int c=0;c<ANGLEATT;c++)
            result[c] = 0;

        for(int i=3;i<ANGLEATT;i+=2)
        {
            if(data[i]==0) break;
            else
            {
            int iy=data[i];
            int ix=data[i+1];

            if(ix>=cx && iy<=cy) // q1
            {
                float m=0;
                int y = cy-iy;
                int x = ix-cx;
                if(ix==cx) m = 1000.0; // in case x==0, m should be the biggest
                else m = (float)y/x;
                if(q1x.size()==0)
                {
                    q1x.append(ix);
                    q1y.append(iy);
                    slope1.append(m);
                }
                else
                {
                    bool inserted = true;
                    for(int c=0;c<slope1.size();c++)
                    {
                        if(m-slope1[c]<0) // order from low to high
                        {
                            slope1.insert(c,m);
                            q1x.insert(c,ix);
                            q1y.insert(c,iy);
                            inserted = false;
                            break;
                        } // if
                    } // for
                    if(inserted)
                    {
                        slope1.append(m);
                        q1x.append(ix);
                        q1y.append(iy);
                    }
                } // else

            } else if(ix<cx && iy<cy) // q2
            {
                float m=0;
                int y = cy-iy;  // being changed according to quadrant
                int x = cx-ix;  // being changed according to quadrant
                m = (float)y/x;
                if(q2x.size()==0)
                {
                    q2x.append(ix);
                    q2y.append(iy);
                    slope2.append(m);
                }
                else
                {
                    bool inserted = true;
                    for(int c=0;c<slope2.size();c++)
                    {
                        if(m-slope2[c]>0) // order from high to low
                        {
                            slope2.insert(c,m);
                            q2x.insert(c,ix);
                            q2y.insert(c,iy);
                            inserted = false;
                            break;
                        } // if
                    } // for
                    if(inserted)
                    {
                        slope2.append(m);
                        q2x.append(ix);
                        q2y.append(iy);
                    }
                } // else

            } else if(ix<=cx && iy>=cy) // q3
            {
                float m=0;
                int y = iy-cy;  // being changed according to quadrant
                int x = cx-ix;  // being changed according to quadrant
                if(ix==cx) m = 1000.0; // in case x==0, m should be the biggest
                else m = (float)y/x;
                if(q3x.size()==0)
                {
                    q3x.append(ix);
                    q3y.append(iy);
                    slope3.append(m);
                }
                else
                {
                    bool inserted = true;
                    for(int c=0;c<slope3.size();c++)
                    {
                        if(m-slope3[c]<0) // order from high to low
                        {
                            slope3.insert(c,m);
                            q3x.insert(c,ix);
                            q3y.insert(c,iy);
                            inserted = false;
                            break;
                        } // if
                    } // for
                    if(inserted)
                    {
                        slope3.append(m);
                        q3x.append(ix);
                        q3y.append(iy);
                    }
                } // else
            }
            else // q4
            {
                float m=0;
                int y = iy-cy;  // being changed according to quadrant
                int x = ix-cx;  // being changed according to quadrant
                m = (float)y/x;
                if(q4x.size()==0)
                {
                    q4x.append(ix);
                    q4y.append(iy);
                    slope4.append(m);
                }
                else
                {
                    bool inserted = true;
                    for(int c=0;c<slope4.size();c++)
                    {
                        if(m-slope4[c]>0) // order from high to low
                        {
                            slope4.insert(c,m);
                            q4x.insert(c,ix);
                            q4y.insert(c,iy);
                            inserted = false;
                            break;
                        } // if
                    } // for
                    if(inserted)
                    {
                        slope4.append(m);
                        q4x.append(ix);
                        q4y.append(iy);
                    }
                } // else
            }
          } // else break
        } // for i

        int i=0;
        int index = 0;
        for(i=0;i<3;i++) result[i] = data[i];
        index = i;
        if(q1x.size()>0)
        for(i=0;i<q1x.size();i++)
        {
            result[index] = q1y[i];
            index++;
            result[index] = q1x[i];
            index++;
        }
        if(q2x.size()>0)
        for(i=0;i<q2x.size();i++)
        {
            result[index] = q2y[i];
            index++;
            result[index] = q2x[i];
            index++;
        }
        if(q3x.size()>0)
        for(i=0;i<q3x.size();i++)
        {
            result[index] = q3y[i];
            index++;
            result[index] = q3x[i];
            index++;
        }
        if(q4x.size()>0)
        for(i=0;i<q4x.size();i++)
        {
            result[index] = q4y[i];
            index++;
            result[index] = q4x[i];
            index++;
        }

        // copy data in result[] to AngleRec[w][]
        for(int c=0;c<ANGLEATT;c++)
            AngleRec[w][c]= result[c];

        // end sorting
    } // for w
} //sortingCoord

void GLWidget::showDetail()
{

} // showDetail

void GLWidget::setObjScale()
{
    setScaleFlag = true;
    setCursor(Qt::PointingHandCursor);
    setFirstClick = false;

} // setObjScale

void GLWidget::okClicked()
{
    wcx = 0;
    wcy = 0;
    xratio = 1.0;
    yratio = 1.0;
    scaledistancepixel = dEdtInputStr->text().toFloat();
    realscaleinunit = uEdtInputStr->text().toFloat();
    scaleunit = UComboBox->currentText();
    ppmm=(scaledistancepixel/realscaleinunit);
    pInputDlg->hide();
    pInputDlg->close();
    firstposx=0;firstposy=0;secondposx=0;secondposy=0;
    updateGL();
    setScaleFlag = false;
}

void GLWidget::cancelClicked()
{
    pInputDlg->hide();
    pInputDlg->close();
    firstposx=0;firstposy=0;secondposx=0;secondposy=0;
    updateGL();
    setScaleFlag = false;

}

void GLWidget::ObjScaleDialog(float noofpixels)
{
    pInputDlg = new QDialog();
    pInputDlg->setFixedSize(500, 120);

    QLabel* dLblLabel = new QLabel(pInputDlg);
    dLblLabel->setText("The number of pixels/unit");
    dLblLabel->setGeometry(20, 5, 180, 20);

    dEdtInputStr = new QLineEdit(pInputDlg);
    dEdtInputStr->setGeometry(300, 5, 305, 20);
    dEdtInputStr->setText(QString::number(noofpixels));
    dEdtInputStr->resize(100, dEdtInputStr->height());

    QLabel* uLblLabel = new QLabel(pInputDlg);
    uLblLabel->setText("The number of units");
    uLblLabel->setGeometry(20, 30, 200, 20);

    uEdtInputStr = new QLineEdit(pInputDlg);
    uEdtInputStr->setGeometry(300, 30, 310, 20);
    uEdtInputStr->setText("1.0");
    uEdtInputStr->resize(50, uEdtInputStr->height());

    QLabel* unLblLabel = new QLabel(pInputDlg);
    unLblLabel->setText("Unit");
    unLblLabel->setGeometry(20, 60, 200, 20);

    UComboBox = new QComboBox(pInputDlg);
    UComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    UComboBox->addItem("nm");
    UComboBox->addItem("um");
    UComboBox->addItem("mm");
    UComboBox->addItem("cm");
    UComboBox->addItem("inch");
    UComboBox->adjustSize();
    UComboBox->update();
    UComboBox->setGeometry(300, 60, 450, 20);

    QPushButton* okbutton = new QPushButton(pInputDlg);
    okbutton->setText("OK");
    okbutton->setGeometry(120, 90, 80, 20);

    QPushButton* cancelbutton = new QPushButton(pInputDlg);
    cancelbutton->setText("Cancel");
    cancelbutton->setGeometry(210, 90, 150, 20);

    pInputDlg->show();
    dEdtInputStr->setFocus();

    connect( okbutton, SIGNAL(clicked()), this, SLOT(okClicked()) );
    connect( cancelbutton, SIGNAL(clicked()), this, SLOT(cancelClicked()) );

} // ObjScaleDialog

void GLWidget::showObjectScale()
{
      QMessageBox::information(0, "Show Current Object Scale", "The Current Object Scale\n 1 mm = "+QString::number(ppmm/25.4)+" pixels \n 1 cm = "+QString::number(ppmm/2.54)+" pixels \n 1 inch = "+QString::number(ppmm)+"  pixels ");
} // showObjectScale

void GLWidget::showJunctionLabel()
{
    if(bool_show_junctionlabel) bool_show_junctionlabel=false;
    else  bool_show_junctionlabel=true;
    updateGL();
}

void GLWidget::showJunctionCross()
{
    if(bool_show_junctioncross) bool_show_junctioncross=false;
    else  bool_show_junctioncross=true;
    updateGL();
}

void GLWidget::cal_Branch_Order1()
{
 int howClose = 1;
 int B_order = 0;
 int B_Count = 0;
 bool isMatch = false;
 for(int i=0;i<n_dc_discs;i++)
 {
     for(int j=0;j<n_rb;j++)
     {
         isMatch = (
                   ((abs(dc_discs[i].p_i_ct-rb[j].p_i_ct)<=howClose)
                      && (abs(dc_discs[i].p_j_ct-rb[j].p_j_ct)<=howClose))
                    ||
                    ((abs(dc_discs[i].p_i_ct-rb[j].i_ct)<=howClose)
                    &&
                    (abs(dc_discs[i].p_j_ct-rb[j].j_ct)<=howClose))
                   );
         if(isMatch)
         {
             int x2 = rb[j].p_i_ct;
             int y2 = rb[j].p_j_ct;

             if(abs(dc_discs[i].p_i_ct-rb[j].p_i_ct)<=howClose)
             {
                  x2 = rb[j].i_ct;
                  y2 = rb[j].j_ct;
             }

             Branch_Order[B_order][B_Count].meas = rb[j].meas/ppmm; // length
             Branch_Order[B_order][B_Count].p_i_ct = dc_discs[i].p_i_ct; // x in dc
             Branch_Order[B_order][B_Count].p_j_ct = dc_discs[i].p_j_ct; // y in dc
             Branch_Order[B_order][B_Count].i_ct = x2;  // x in junc
             Branch_Order[B_order][B_Count].j_ct = y2;  // y in junc
             B_Count++;
         } // if isMatch
     } // for j
 } // for i
 BOrder1_Count = B_Count;
} // cal_Branch_Order1


void GLWidget::updateDC(int dcy,int dcx)
{
   for(int i=0;i<n_dc_discs;i++)
   {
       if(dc_discs[i].p_i_ct==dcy && dc_discs[i].p_j_ct==dcx)
       {
           m_dc_discs[i].p_i_ct=-1;
           m_dc_discs[i].p_j_ct=-1;
           dc_discs[i].p_i_ct=-1;
           dc_discs[i].p_j_ct=-1;
           updateDCArray();
           updateDCinReport();
       }
    }

} //updateDC

void GLWidget::updateDCinReport()
{
    for(int i=0;i<m_n_dc_discs;i++)
    {
            m_dc_discs[i].p_i_ct=0;
            m_dc_discs[i].p_j_ct=0;
            m_dc_discs[i].i_ct=0;
            m_dc_discs[i].j_ct=0;
            m_dc_discs[i].i1_ct=0;
            m_dc_discs[i].j1_ct=0;
            m_dc_discs[i].meas=0;
    }
    m_n_dc_discs = n_dc_discs;
    for(int i=0;i<m_n_dc_discs;i++)
    {
            m_dc_discs[i].p_i_ct=dc_discs[i].p_i_ct;
            m_dc_discs[i].p_j_ct=dc_discs[i].p_j_ct;
            m_dc_discs[i].i_ct=dc_discs[i].i_ct;
            m_dc_discs[i].j_ct=dc_discs[i].j_ct;
            m_dc_discs[i].i1_ct=dc_discs[i].i1_ct;
            m_dc_discs[i].j1_ct=dc_discs[i].j1_ct;
            m_dc_discs[i].meas=dc_discs[i].meas;
    }
}

void GLWidget::updateDCArray()
{
    Medge temp[MAX_RES];
    int rs = 0;
    for(int i=0;i<n_dc_discs;i++)
    {
        if(dc_discs[i].p_i_ct!=-1 && dc_discs[i].p_j_ct!=-1)
        {
            temp[rs].p_i_ct=dc_discs[i].p_i_ct;
            temp[rs].p_j_ct=dc_discs[i].p_j_ct;
            temp[rs].i_ct=dc_discs[i].i_ct;
            temp[rs].j_ct=dc_discs[i].j_ct;
            temp[rs].i1_ct=dc_discs[i].i1_ct;
            temp[rs].j1_ct=dc_discs[i].j1_ct;
            temp[rs].meas=dc_discs[i].meas;
            rs++;
        }
    }
    for(int i=0;i<n_dc_discs;i++)
    {
            dc_discs[i].p_i_ct=0;
            dc_discs[i].p_j_ct=0;
            dc_discs[i].i_ct=0;
            dc_discs[i].j_ct=0;
            dc_discs[i].i1_ct=0;
            dc_discs[i].j1_ct=0;
            dc_discs[i].meas=0;
    }

    n_dc_discs = rs;
    for(int i=0;i<n_dc_discs;i++)
    {
            dc_discs[i].p_i_ct=temp[i].p_i_ct;
            dc_discs[i].p_j_ct=temp[i].p_j_ct;
            dc_discs[i].i_ct=temp[i].i_ct;
            dc_discs[i].j_ct=temp[i].j_ct;
            dc_discs[i].i1_ct=temp[i].i1_ct;
            dc_discs[i].j1_ct=temp[i].j1_ct;
            dc_discs[i].meas=temp[i].meas;
    }
} // updateDCArray

void GLWidget::updateBranchOrder0(int pos)
{
    Medge temp[500];
    int rs=0;
    for(int i=0;i<BOrder1_Count;i++)
    {
        if(i!=pos)
        {
            temp[rs].p_i_ct=Branch_Order[0][i].p_i_ct;
            temp[rs].p_j_ct=Branch_Order[0][i].p_j_ct;
            temp[rs].i_ct=Branch_Order[0][i].i_ct;
            temp[rs].j_ct=Branch_Order[0][i].j_ct;
            temp[rs].i1_ct=Branch_Order[0][i].i1_ct;
            temp[rs].j1_ct=Branch_Order[0][i].j1_ct;
            temp[rs].meas=Branch_Order[0][i].meas;
            rs++;
        }
    }
    for(int i=0;i<BOrder1_Count;i++)
    {
            Branch_Order[0][i].p_i_ct=0;
            Branch_Order[0][i].p_j_ct=0;
            Branch_Order[0][i].i_ct=0;
            Branch_Order[0][i].j_ct=0;
            Branch_Order[0][i].i1_ct=0;
            Branch_Order[0][i].j1_ct=0;
            Branch_Order[0][i].meas=0;
    }
    BOrder1_Count = rs;
    for(int i=0;i<BOrder1_Count;i++)
    {
            Branch_Order[0][i].p_i_ct=temp[i].p_i_ct;
            Branch_Order[0][i].p_j_ct=temp[i].p_j_ct;
            Branch_Order[0][i].i_ct=temp[i].i_ct;
            Branch_Order[0][i].j_ct=temp[i].j_ct;
            Branch_Order[0][i].i1_ct=temp[i].i1_ct;
            Branch_Order[0][i].j1_ct=temp[i].j1_ct;
            Branch_Order[0][i].meas=temp[i].meas;
    }
}//updateBranchOrder0

void GLWidget::updateBranchLength(int dcy,int dcx)
{
    for(int i=0;i<n_rb;i++)
    {
      if((rb[i].p_i_ct==dcy && rb[i].p_j_ct==dcx) || (rb[i].i_ct==dcy && rb[i].j_ct==dcx))
      {
          printf("rb founded");
          updateBranchRB(i);
          updateBranchMRB(i); // for updateReport
      }
    }
} // updateBranchLength

void GLWidget::updateBranchRB(int pos)
{
    Medge temp[MAX_RES];
    int rs=0;
    for(int i=0;i<n_rb;i++)
    {
        if(i!=pos)
        {
            temp[rs].p_i_ct=rb[i].p_i_ct;
            temp[rs].p_j_ct=rb[i].p_j_ct;
            temp[rs].i_ct=rb[i].i_ct;
            temp[rs].j_ct=rb[i].j_ct;
            temp[rs].i1_ct=rb[i].i1_ct;
            temp[rs].j1_ct=rb[i].j1_ct;
            temp[rs].meas=rb[i].meas;
            rs++;
        }
    }
    for(int i=0;i<n_rb;i++)
    {
            rb[i].p_i_ct=0;
            rb[i].p_j_ct=0;
            rb[i].i_ct=0;
            rb[i].j_ct=0;
            rb[i].i1_ct=0;
            rb[i].j1_ct=0;
            rb[i].meas=0;
    }
    n_rb = rs;
    for(int i=0;i<n_rb;i++)
    {
            rb[i].p_i_ct=temp[i].p_i_ct;
            rb[i].p_j_ct=temp[i].p_j_ct;
            rb[i].i_ct=temp[i].i_ct;
            rb[i].j_ct=temp[i].j_ct;
            rb[i].i1_ct=temp[i].i1_ct;
            rb[i].j1_ct=temp[i].j1_ct;
            rb[i].meas=temp[i].meas;
    }
} //updateBranchRB

void GLWidget::updateBranchMRB(int pos)
{
    for(int i=0;i<m_n_rb;i++)
    {
            m_rb[i].p_i_ct=0;
            m_rb[i].p_j_ct=0;
            m_rb[i].i_ct=0;
            m_rb[i].j_ct=0;
            m_rb[i].i1_ct=0;
            m_rb[i].j1_ct=0;
            m_rb[i].meas=0;
    }
    m_n_rb = n_rb;
    for(int i=0;i<m_n_rb;i++)
    {
            m_rb[i].p_i_ct=rb[i].p_i_ct;
            m_rb[i].p_j_ct=rb[i].p_j_ct;
            m_rb[i].i_ct=rb[i].i_ct;
            m_rb[i].j_ct=rb[i].j_ct;
            m_rb[i].i1_ct=rb[i].i1_ct;
            m_rb[i].j1_ct=rb[i].j1_ct;
            m_rb[i].meas=rb[i].meas;
    }

} // updateBranchMRB

void GLWidget::updateBrSpacing(int dcy,int dcx)
{
    for(int i=0;i<n_br_spacing;i++)
    {
        if(br_spacing[i].p_i_ct==dcy && br_spacing[i].p_j_ct==dcx)
        {
            printf("branch spacing found should be deleted\n");
            updateBrSpacingArray(i);
        }
     }
    updateBrSpacinginReport();

}

bool GLWidget::test_disc_too_large_br_update(int  im[MAX_ISIZE][MAX_ISIZE], int imax, int jmax,Ij_pos circle[MAX_CIRC],int n_circle)
{
    // only check the nearest terminal
    int	in_circle;
    bool result = FALSE;

    for( in_circle = 0; in_circle < n_circle; in_circle++)
    {
        if ( (im_term1[circle[in_circle].i_x][circle[in_circle].j_y] != 1) && (im_term1[circle[in_circle].i_x][circle[in_circle].j_y] != 0) )
        {
            result = TRUE;
            break;
        }
    } // for in_circle
    return( result );

} // test_disc_too_large_br



void GLWidget::update_br_spacing_meas(int dcy, int dcx)
{
    int           i,j,radius;
    Bool          too_large;
    Ij_pos        circle_buf[MAX_CIRC];
    int           n_circle_buf;


    for(int a=0;a<n_br_spacing;a++)
    {
        br_spacing[a].p_i_ct=0;
        br_spacing[a].p_j_ct=0;
        br_spacing[a].i_ct=0;
        br_spacing[a].j_ct=0;
        br_spacing[a].i1_ct=0;
        br_spacing[a].j1_ct=0;
        br_spacing[a].meas=0;
    }
    n_br_spacing = 0;

    im_term1[dcy][dcx]=0;
    for (i = 1; i < im_in_imax - 1; i++)
    {
      for (j = 1; j < im_in_jmax - 1; j++)
      {
        if ( im_term1[i][j] != 0 && im_term1[i][j] != 1 )
        {
            radius = 2;
            too_large = FALSE;
            while ( (radius < MAX_RADIUS) && !too_large )
            {
              n_circle_buf = 0;
              mich_circle( im_contour, im_in_imax, im_in_jmax,i, j, radius, circle_buf, &n_circle_buf, EMPTY_MARK );
              too_large = test_disc_too_large_br_update( im_skel, im_in_imax, im_in_jmax, circle_buf, n_circle_buf );
              if (too_large)
              {
                add_result( (Real)radius, br_spacing, &n_br_spacing, i, j, 0, 0, 0, 0 );
              } // if too_large
              radius++;
            } // while
            if (radius == MAX_RADIUS){
              printf("fra: warning MAX_RADIUS ");
            } // if radius
        } // if im_term1
      } // j
    } // i
} // br_spacing



void GLWidget::updateBrSpacingArray(int pos)
{
    Medge temp[MAX_RES];
    int rs = 0;
    for(int i=0;i<n_br_spacing;i++)
    {
        if(i!=pos)
        {
            temp[rs].p_i_ct=br_spacing[i].p_i_ct;
            temp[rs].p_j_ct=br_spacing[i].p_j_ct;
            temp[rs].i_ct=br_spacing[i].i_ct;
            temp[rs].j_ct=br_spacing[i].j_ct;
            temp[rs].i1_ct=br_spacing[i].i1_ct;
            temp[rs].j1_ct=br_spacing[i].j1_ct;
            temp[rs].meas=br_spacing[i].meas;
            rs++;
        }
    }
    for(int i=0;i<n_br_spacing;i++)
    {
            br_spacing[i].p_i_ct=0;
            br_spacing[i].p_j_ct=0;
            br_spacing[i].i_ct=0;
            br_spacing[i].j_ct=0;
            br_spacing[i].i1_ct=0;
            br_spacing[i].j1_ct=0;
            br_spacing[i].meas=0;
    }

    n_br_spacing = rs;
    for(int i=0;i<n_br_spacing;i++)
    {
            br_spacing[i].p_i_ct=temp[i].p_i_ct;
            br_spacing[i].p_j_ct=temp[i].p_j_ct;
            br_spacing[i].i_ct=temp[i].i_ct;
            br_spacing[i].j_ct=temp[i].j_ct;
            br_spacing[i].i1_ct=temp[i].i1_ct;
            br_spacing[i].j1_ct=temp[i].j1_ct;
            br_spacing[i].meas=temp[i].meas;
    }

}

void GLWidget::updateBrSpacinginReport()
{
    for(int i=0;i<m_n_br_spacing;i++)
    {
            m_br_spacing[i].p_i_ct=0;
            m_br_spacing[i].p_j_ct=0;
            m_br_spacing[i].i_ct=0;
            m_br_spacing[i].j_ct=0;
            m_br_spacing[i].i1_ct=0;
            m_br_spacing[i].j1_ct=0;
            m_br_spacing[i].meas=0;
    }
    m_n_br_spacing = n_br_spacing;
    for(int i=0;i<m_n_br_spacing;i++)
    {
            m_br_spacing[i].p_i_ct=br_spacing[i].p_i_ct;
            m_br_spacing[i].p_j_ct=br_spacing[i].p_j_ct;
            m_br_spacing[i].i_ct=br_spacing[i].i_ct;
            m_br_spacing[i].j_ct=br_spacing[i].j_ct;
            m_br_spacing[i].i1_ct=br_spacing[i].i1_ct;
            m_br_spacing[i].j1_ct=br_spacing[i].j1_ct;
            m_br_spacing[i].meas=br_spacing[i].meas;
    }
}

void GLWidget::CleanUltimateBranch(float MinPixLengthToDel)
{
    for(int i=0;i<BOrder1_Count;i++)
    {
        if(Branch_Order[0][i].meas <= MinPixLengthToDel)
        {
            int pict = Branch_Order[0][i].p_i_ct;
            int pjct = Branch_Order[0][i].p_j_ct;
            updateDC(pict,pjct);
            updateBranchOrder0(i);
            updateBranchLength(pict,pjct);
            update_br_spacing_meas(pict,pjct);
            updateBrSpacing(pict,pjct);
        }
    } // for amount of ultimate branch ( branch order no 1)
} // CleanUltimatebranch

void GLWidget::CleanUltimateBranchAction()
{
   CleanUltimateBranch(MinUltiLength);
   updateGL();
}

void GLWidget::CleanTerminal(float MinTermimalRadiusToDel)
{
    for(int i=0;i<n_dc_discs;i++)
    {
        if(2*(dc_discs[i].meas/ppmm) <= MinTermimalRadiusToDel)
        {
            int pict = dc_discs[i].p_i_ct;
            int pjct = dc_discs[i].p_j_ct;
            updateDC(pict,pjct);
            updateBranchOrder0(i);
            updateBranchLength(pict,pjct);
            update_br_spacing_meas(pict,pjct);
            updateBrSpacing(pict,pjct);
        }
    } // for n_dc_discs
} //CleanTerminal

void GLWidget::CleanTerminalAction()
{
   CleanTerminal(MinTerminalDiameter);
   updateGL();

}

void GLWidget::showBranchOrder(int xstart, int ystart)
{
    glLineWidth(pointsize);
    glColor3f(0,1,1);
    glBegin(GL_LINES);
    for(int i=0;i<BOrder1_Count;i++)
    {
        if(Branch_Order[0][i].p_i_ct!=0)
        {
        int x11 = Branch_Order[0][i].p_i_ct+ystart;
        int y11 = Branch_Order[0][i].p_j_ct+xstart;
        int x21 = Branch_Order[0][i].i_ct+ystart;
        int y21 = Branch_Order[0][i].j_ct+xstart;
        glVertex2i(y11,x11);
        glVertex2i(y21,x21);
        } // if
    } // for
    glEnd();
} // showBranchOrder

void GLWidget::cleanSetting()
{
    pInputDlg = new QDialog();
    pInputDlg->setFixedSize(500, 120);

    QLabel* dLblLabel = new QLabel(pInputDlg);
    dLblLabel->setText("Minimum Ultimate Branch Length");
    dLblLabel->setGeometry(20, 5, 220, 20);

    dEdtInputStr = new QLineEdit(pInputDlg);
    dEdtInputStr->setGeometry(300, 5, 305, 20);
    dEdtInputStr->setText(QString::number(MinUltiLength));
    dEdtInputStr->resize(100, dEdtInputStr->height());

    QLabel* uLblLabel = new QLabel(pInputDlg);
    uLblLabel->setText("Minimum Terminal Diameter");
    uLblLabel->setGeometry(20, 30, 200, 20);

    uEdtInputStr = new QLineEdit(pInputDlg);
    uEdtInputStr->setGeometry(300, 30, 310, 20);
    uEdtInputStr->setText(QString::number(MinTerminalDiameter));
    uEdtInputStr->resize(50, uEdtInputStr->height());

    QPushButton* okbutton = new QPushButton(pInputDlg);
    okbutton->setText("OK");
    okbutton->setGeometry(120, 90, 80, 20);

    QPushButton* cancelbutton = new QPushButton(pInputDlg);
    cancelbutton->setText("Cancel");
    cancelbutton->setGeometry(210, 90, 150, 20);

    pInputDlg->show();
    dEdtInputStr->setFocus();

    connect( okbutton, SIGNAL(clicked()), this, SLOT(CleanokClicked()) );
    connect( cancelbutton, SIGNAL(clicked()), this, SLOT(CleancancelClicked()) );

}

void GLWidget::CleanokClicked()
{

    MinUltiLength = dEdtInputStr->text().toFloat();
    MinTerminalDiameter = ceil(uEdtInputStr->text().toFloat());
    pInputDlg->close();

}

void GLWidget::CleancancelClicked()
{
    pInputDlg->close();
}

void GLWidget::TreeStart()
{
    Tree_process_finish = false;
    // clear QVector
    SortedAngleJuncNotLeaves.clear();
    for(int i=0;i<MAX_RES;i++)
    {
        JuncWithSortedRB[i].clear();
        JuncWithSortedAngle[i].clear();
        VSortedAngleJuncNotLeaves[i].clear();
    }
    // end clearing QVector

    correctRB();
    orderRB();
    int cnewrb = 0;
    for(int i=0;i<WRRB_count;i++)
    {
        if(newRB[i].val==0) cnewrb++;
    }

    if(cnewrb==0)
    {
        LTRBRSorting();

        int count0=0;
        for(int i=0;i<WRRB_count;i++)
        {
            if(Ord_RB[i]==0) count0++;
        }
        if(count0<=1) ComputeBranchOrder();

        sortAngleInJunc();
        ListJunctionNotConnectedToLeft();
        BranchEachBr(); // put each branch order detail into an array
    } // if cnewrb==0

    updateGL();
    Tree_process_finish = true;
} //TreeStart

Ij_Value GLWidget::rootSearching(Medge whatgroup[MAX_RES],int nrb)
{
    Ij_Value rootpos;
    rootpos.F_no = whatgroup[0].p_i_ct; // y
    rootpos.S_no = whatgroup[0].p_j_ct; // x
    rootpos.val = 0;

    for(int i=0;i<nrb;i++)
    {
        if(whatgroup[i].i1_ct==1)
        {
           // find min
            if(whatgroup[i].p_i_ct>rootpos.F_no)
            {
                rootpos.F_no = whatgroup[i].p_i_ct; // y
                rootpos.S_no = whatgroup[i].p_j_ct; // x
                rootpos.val = i; // RB No.
            } // find min
        } // if rb
    } // for
    return rootpos;
} // rootSearching

Ij_Value GLWidget::NearestJunction(int y,int x,int excljuncno)
{
    Real dist_min=1000.0;
    Real r0;
    Ij_Value nearestJuncpos;
    nearestJuncpos.F_no = da_discs[0].p_i_ct; // y
    nearestJuncpos.S_no = da_discs[0].p_j_ct; // x
    nearestJuncpos.val = 0;

    for(int i=0;i<n_da_discs;i++)
    {
            if(i!=excljuncno)
            {
            r0 = sqrt(pow(y-da_discs[i].p_i_ct,2)+pow(x-da_discs[i].p_j_ct,2));
           // find min
            if(r0<dist_min)
            {
                dist_min = r0;
                nearestJuncpos.F_no = da_discs[i].p_i_ct; // y
                nearestJuncpos.S_no = da_discs[i].p_j_ct; // x
                nearestJuncpos.val = i; // i = Junction
            } // find min
            }
    } // for

    for(int i=0;i<n_dc_discs;i++)
    {
            r0 = sqrt(pow(y-dc_discs[i].p_i_ct,2)+pow(x-dc_discs[i].p_j_ct,2));
           // find min
            if(r0<dist_min)
            {
                dist_min = r0;
                nearestJuncpos.F_no = dc_discs[i].p_i_ct; // y
                nearestJuncpos.S_no = dc_discs[i].p_j_ct; // x
                nearestJuncpos.val = 1000+i; // >=1000 is Leaf
            } // find min
    } // for
    return nearestJuncpos;
} //NearestJunction


void GLWidget::correctRB()
{
  Ij_Value nearestPoint;
  Ij_Value nearestPoint_2;
  new_n_rb = n_rb;
  for(int i=0;i<new_n_rb;i++)
  {
      nearestPoint = NearestJunction(rb[i].p_i_ct,rb[i].p_j_ct,-1);
      nearestPoint_2 = NearestJunction(rb[i].i_ct,rb[i].j_ct,-1);
      repairedRB[i].meas = sqrt(pow(nearestPoint.F_no-nearestPoint_2.F_no,2)+pow(nearestPoint.S_no-nearestPoint_2.S_no,2));
      if((nearestPoint.F_no==nearestPoint_2.F_no) && (nearestPoint.S_no==nearestPoint_2.S_no))
      {
        int t = nearestPoint.val;
        float d1 = sqrt(pow(rb[i].p_i_ct-da_discs[t].p_i_ct,2)+pow(rb[i].p_j_ct-da_discs[t].p_j_ct,2));
        float d2 = sqrt(pow(rb[i].i_ct-da_discs[t].p_i_ct,2)+pow(rb[i].j_ct-da_discs[t].p_j_ct,2));
        if(d1<d2)
        {
            nearestPoint = NearestJunction(rb[i].p_i_ct,rb[i].p_j_ct,-1);
            nearestPoint_2 = NearestJunction(rb[i].i_ct,rb[i].j_ct,t);
        } else // d1>d2
        {
            nearestPoint = NearestJunction(rb[i].i_ct,rb[i].j_ct,-1);
            nearestPoint_2 = NearestJunction(rb[i].p_i_ct,rb[i].p_j_ct,t);
        }
        repairedRB[i].meas = sqrt(pow(nearestPoint.F_no-nearestPoint_2.F_no,2)+pow(nearestPoint.S_no-nearestPoint_2.S_no,2));
      }

      repairedRB[i].p_i_ct = nearestPoint.F_no; // y
      repairedRB[i].p_j_ct = nearestPoint.S_no; // x
      repairedRB[i].i_ct = nearestPoint_2.F_no; // y
      repairedRB[i].j_ct = nearestPoint_2.S_no; // x
      repairedRB[i].i1_ct = rb[i].i1_ct;

  } // for n_rb

  bool check[new_n_rb];
  for(int i=0;i<new_n_rb;i++)
      check[i] = false;

  for(int i=0;i<new_n_rb;i++)
  {
      if(repairedRB[i].meas==0) check[i]=true;
      if(!check[i])
      {
          for(int j=(i+1);j<new_n_rb;j++)
          {
              if(
                  ((repairedRB[i].meas==repairedRB[j].meas)
                  && (repairedRB[i].p_i_ct==repairedRB[j].i_ct)
                  && (repairedRB[i].p_j_ct==repairedRB[j].j_ct)
                  && (repairedRB[i].i_ct==repairedRB[j].p_i_ct)
                  && (repairedRB[i].j_ct==repairedRB[j].p_j_ct))
                  ||
                  ((repairedRB[i].meas==repairedRB[j].meas)
                  && (repairedRB[i].p_i_ct==repairedRB[j].p_i_ct)
                  && (repairedRB[i].p_j_ct==repairedRB[j].p_j_ct))
                  )
              {
                  check[j] = true;
              } // if meas is equal
          } // for j
      }  // if check
  } // for i


  Medge NoredundantRB[MAX_RES];
  int rbcount = 0;
  for(int i=0;i<new_n_rb;i++)
  {
      if(!check[i])
      {
        NoredundantRB[rbcount].meas = repairedRB[i].meas;
        NoredundantRB[rbcount].p_i_ct = repairedRB[i].p_i_ct; // y
        NoredundantRB[rbcount].p_j_ct = repairedRB[i].p_j_ct; // x
        NoredundantRB[rbcount].i_ct = repairedRB[i].i_ct;
        NoredundantRB[rbcount].j_ct = repairedRB[i].j_ct;
        NoredundantRB[rbcount].i1_ct = repairedRB[i].i1_ct;
        rbcount++;
      }
  } // for i

  QVector<int> rbsdc;
  for(int i=0;i<n_dc_discs;i++)
  {
      rbsdc = getRBsWithDcs(dc_discs[i].p_i_ct,dc_discs[i].p_j_ct,NoredundantRB,rbcount);
      if(rbsdc.size()>1)
      {
          for(int j=1;j<rbsdc.size();j++)
          {
              check[rbsdc.at(j)]= true;
          }
      } // if
  }
  // dc checking end


  WRRB_count = rbcount;
  // copy local to global variables.
  for(int i=0;i<WRRB_count;i++)
  {
      WithoutRedundantRB[i].meas = NoredundantRB[i].meas;
      WithoutRedundantRB[i].p_i_ct = NoredundantRB[i].p_i_ct; // y
      WithoutRedundantRB[i].p_j_ct = NoredundantRB[i].p_j_ct; // x
      WithoutRedundantRB[i].i_ct = NoredundantRB[i].i_ct;
      WithoutRedundantRB[i].j_ct = NoredundantRB[i].j_ct;
      WithoutRedundantRB[i].i1_ct = NoredundantRB[i].i1_ct;
  }
} // correctRB


void GLWidget::orderRB()
{
    int co = 0;
    bool fin=true;
    while(fin && co<=1)
    {
        RBStack.clear();
        for(int i=0;i<WRRB_count;i++) RB_Check[i] = false;

        // finding root
        if(myroot.F_no==0 && myroot.S_no==0) // don't set root yet, so root comes from automatic root detection
        {
            Treerootpos = rootSearching(WithoutRedundantRB,WRRB_count);
        } else  // startpos comes from visual root selection
        {
            for(int i=0;i<WRRB_count;i++)
            {
                    if(WithoutRedundantRB[i].p_i_ct==myroot.F_no && WithoutRedundantRB[i].p_j_ct==myroot.S_no)
                    {
                        myroot.val = i; // RB No.
                    } // find min
            } //
            Treerootpos = myroot;
        }
        // end of finding root


        // OrderedRB;
        sortRB(Treerootpos.F_no,Treerootpos.S_no,0); // recursive method
        CoordToNode();
        RepairLinks();
        co++;

    } // while

} // orderRB


void GLWidget::sortRB(int y_i_ct, int x_j_ct,int n)
{
    bool leaf = false;
    QVector<int> RBTemp;
    if(n<WRRB_count)
    {
        if(n>0) // if it is a root , don't do this (n==0 is the start which means root)
        {
            for(int i=0;i<n_dc_discs;i++)
            {
                if(dc_discs[i].p_i_ct==y_i_ct && dc_discs[i].p_j_ct==x_j_ct)
                {
                    leaf = true;
                    break;
                }
            } // for n_dc_disc
        } // n>0
        if(leaf)
        {
            if(RBStack.size()!=0)
            {
                int rbno = RBStack.pop();
                RB_Check[rbno] = true;
                Ord_RB[n] = rbno;
                n++;
                sortRB(Ord_Coord_RB[rbno].i_ct,Ord_Coord_RB[rbno].j_ct,n);
            }
        } else // not leaf
        {
            for(int i=0;i<WRRB_count;i++) // checking loop
            {
              if(!RB_Check[i]) // if the rb hasn't yet visited
              {
                  bool firstpair = (y_i_ct==WithoutRedundantRB[i].p_i_ct)
                          && (x_j_ct==WithoutRedundantRB[i].p_j_ct);
                  bool secondpair = (y_i_ct==WithoutRedundantRB[i].i_ct)
                          && (x_j_ct==WithoutRedundantRB[i].j_ct);
                  if(firstpair || secondpair)
                  {
                      if(firstpair)//firstpair is matched(remain its polar(keep the direction))
                      {
                          Ord_Coord_RB[i].meas = WithoutRedundantRB[i].meas;
                          Ord_Coord_RB[i].p_i_ct = WithoutRedundantRB[i].p_i_ct;
                          Ord_Coord_RB[i].p_j_ct = WithoutRedundantRB[i].p_j_ct;
                          Ord_Coord_RB[i].i_ct = WithoutRedundantRB[i].i_ct;
                          Ord_Coord_RB[i].j_ct = WithoutRedundantRB[i].j_ct;
                          Ord_Coord_RB[i].i1_ct = WithoutRedundantRB[i].i1_ct;
                      } else // secondpair is matched(swap its polar (change direction))
                      {
                          Ord_Coord_RB[i].meas = WithoutRedundantRB[i].meas;
                          Ord_Coord_RB[i].p_i_ct = WithoutRedundantRB[i].i_ct;
                          Ord_Coord_RB[i].p_j_ct = WithoutRedundantRB[i].j_ct;
                          Ord_Coord_RB[i].i_ct = WithoutRedundantRB[i].p_i_ct;
                          Ord_Coord_RB[i].j_ct = WithoutRedundantRB[i].p_j_ct;
                          Ord_Coord_RB[i].i1_ct = WithoutRedundantRB[i].i1_ct;
                      }
                      RBTemp.append(i); // add RB_no into a vector
                  } // if firstpair || secondpair
              } // if RBCheck = false
             } // for i

            if (RBTemp.size()>1)
            {
                // sorting the end point
                // we use the top one, then put the rest into a stack for later use.
                QVector<int> Ysorting;
                for(int r=0;r<RBTemp.size();r++)
                {
                    Ysorting.append(Ord_Coord_RB[RBTemp.at(r)].j_ct);
                }

                qStableSort(Ysorting.begin(),Ysorting.end());

                for(int r=0;r<RBTemp.size();r++)
                {
                    int tempx;
                    for(int s=0;s<RBTemp.size();s++)
                    {
                        if(Ord_Coord_RB[RBTemp.at(s)].j_ct==Ysorting.at(r))
                        {
                            tempx = RBTemp.at(r);
                            RBTemp[r] = RBTemp.at(s);
                            RBTemp[s] = tempx;
                            break;
                        } // if
                    } // for s
                }// for r

                //putting into a stack
                for(int v1=RBTemp.size()-1;v1>=1;v1--)
                {
                    RBStack.push(RBTemp.at(v1));
                } // for v1
            } // if RBTemp.size() > 1

            if(RBTemp.size()!=0)
            {
                int label = RBTemp.at(0);
                RB_Check[label] = true;
                Ord_RB[n] = label;
                n++;
                sortRB(Ord_Coord_RB[label].i_ct,Ord_Coord_RB[label].j_ct,n);
            } // if RBTemp.size()!=0
        } // else not leaf
    } // if n<WRRB_Count
} //sortRB

void GLWidget::CoordToNode()
{
//use WithoutRedundantRB,da_discs(Intermediate Node or junction),and dc_discs(leaf)
  for(int i=0;i<WRRB_count;i++)
  {
      newRB[i].val = Ord_Coord_RB[i].meas;
      newRB[i].RbN = i;
      for(int j=0;j<n_da_discs;j++)
      {
          if((Ord_Coord_RB[i].p_i_ct==da_discs[j].p_i_ct)
              &&(Ord_Coord_RB[i].p_j_ct==da_discs[j].p_j_ct))
          {
              newRB[i].y1 = da_discs[j].p_i_ct;
              newRB[i].x1 = da_discs[j].p_j_ct;
              newRB[i].p1 = j;
          }
          if((Ord_Coord_RB[i].i_ct==da_discs[j].p_i_ct)
              &&(Ord_Coord_RB[i].j_ct==da_discs[j].p_j_ct))
          {
              newRB[i].y2 = da_discs[j].p_i_ct;
              newRB[i].x2 = da_discs[j].p_j_ct;
              newRB[i].p2 = j;
          }
      } // matching junction no.

      for(int j=0;j<n_dc_discs;j++)
      {
          if((Ord_Coord_RB[i].p_i_ct==dc_discs[j].p_i_ct)
              &&(Ord_Coord_RB[i].p_j_ct==dc_discs[j].p_j_ct))
          {
              newRB[i].y1 = dc_discs[j].p_i_ct;
              newRB[i].x1 = dc_discs[j].p_j_ct;
              newRB[i].p1 = 1000+(j);
          }
          if((Ord_Coord_RB[i].i_ct==dc_discs[j].p_i_ct)
              &&(Ord_Coord_RB[i].j_ct==dc_discs[j].p_j_ct))
          {
              newRB[i].y2 = dc_discs[j].p_i_ct;
              newRB[i].x2 = dc_discs[j].p_j_ct;
              newRB[i].p2 = 1000+(j);
          }
      } // matching leaf no.
  } // for RB

} // CoordToNode

void GLWidget::NodeToCoord() {  }

void GLWidget::showTreeGL(int xstart,int ystart)
{

    for(int i=0;i<WRRB_count;i++)
    {
        glLineWidth(newRB[i].BrOrNo);
        if(newRB[i].p2<1000) // draw branch not connected with leaf
        {
            glColor3f(1,0,0);
            glBegin(GL_LINES);
                 glVertex2i(newRB[i].x1+xstart,newRB[i].y1+ystart);
                 glVertex2i(newRB[i].x2+xstart,newRB[i].y2+ystart);
            glEnd();
        }
        else // draw branch connected with leaf
        {
            glColor3f(0,1,0);
            glBegin(GL_LINES);
                 glVertex2i(newRB[i].x1+xstart,newRB[i].y1+ystart);
                 glVertex2i(newRB[i].x2+xstart,newRB[i].y2+ystart);
            glEnd();
        } //
    } // rb
    for(int i=0;i<WRRB_count;i++) // use newRB
    {
        if(newRB[i].RbN==(int)Treerootpos.val)
        {
            glColor3f(1,1,1); // white color
            glBegin(GL_TRIANGLE_FAN);
            for (float a=0;a<2*MYPI;a+=MYPI/180)
            {
              glVertex2f(newRB[i].x1+xstart + cos(a) * 3,
                         newRB[i].y1+ystart + sin(a) * 3);
            } // drawing root
            glEnd();
        }
        else  // not root
        {
            if(newRB[i].p1<1000) glColor3f(0,1,1);
            else glColor3f(1,0,1);
            glBegin(GL_TRIANGLE_FAN);
            for (float a=0;a<2*MYPI;a+=MYPI/180)
            {
              glVertex2f(newRB[i].x1+xstart + cos(a) * 3,
                         newRB[i].y1+ystart + sin(a) * 3);
            } // drawing junction
            glEnd();

            if(newRB[i].p2<1000) glColor3f(0,1,1);
            else glColor3f(1,0,1);
            glBegin(GL_TRIANGLE_FAN);
            for (float a=0;a<2*MYPI;a+=MYPI/180)
            {
              glVertex2f(newRB[i].x2+xstart + cos(a) * 3,
                         newRB[i].y2+ystart + sin(a) * 3);
            } // drawing Leaf
            glEnd();
        }
    } // for Node

} // showTreeGL

void GLWidget::ComputeBranchOrder()
{
  cutBranch(newRB,WRRB_count,1); //each recursion (each order) produces branches which are kept in finalBranchOrder
} // ComputeBranchOrder

void GLWidget::cutBranch(Ij_PC newrb[MAX_RES],int n,int loopno)
{
  int noOfLeafToEnd = 0;
  for(int i=0;i<n;i++)
  {
        if(newrb[i].p2>=1000)
        {
           noOfLeafToEnd++;
        } // if
  }

  if(noOfLeafToEnd>1)
  {
      int lbcount=0,nlbcount=0;
      Ij_PC Leafbranch[n], NotLeafbranch[n];
      for(int i=0;i<n;i++)
      {
          if(newrb[i].p2>=loopno*1000) // || newrb[i].BrOrNo==loopno)
          {
              Leafbranch[lbcount].y1 = newrb[i].y1;
              Leafbranch[lbcount].x1 = newrb[i].x1;
              Leafbranch[lbcount].p1 = newrb[i].p1;
              Leafbranch[lbcount].y2 = newrb[i].y2;
              Leafbranch[lbcount].x2 = newrb[i].x2;
              Leafbranch[lbcount].p2 = newrb[i].p2;
              Leafbranch[lbcount].RbN = newrb[i].RbN;
              Leafbranch[lbcount].val = newrb[i].val;
              Leafbranch[lbcount].BrOrNo = loopno;
              newRB[newrb[i].RbN].BrOrNo = loopno;
              lbcount++;
          }
          else
          {
             NotLeafbranch[nlbcount].y1 = newrb[i].y1;
             NotLeafbranch[nlbcount].x1 = newrb[i].x1;
             NotLeafbranch[nlbcount].p1 = newrb[i].p1;
             NotLeafbranch[nlbcount].y2 = newrb[i].y2;
             NotLeafbranch[nlbcount].x2 = newrb[i].x2;
             NotLeafbranch[nlbcount].p2 = newrb[i].p2;
             NotLeafbranch[nlbcount].RbN = newrb[i].RbN;
             NotLeafbranch[nlbcount].val = newrb[i].val;
             NotLeafbranch[nlbcount].BrOrNo = 0;
             newRB[newrb[i].RbN].BrOrNo = 0;
             nlbcount++;
          } // else if
      } // for i

     // check if any junctions is in the same branch by checking if the junction is more than 2,
     // then delete that route from

      //change leave node (+ loopno*1000)
       int RCount[nlbcount];
       for(int i=0;i<nlbcount; i++)
       {
          RCount[i] = 0;
          for(int j=0;j<nlbcount;j++)
          {
              if (NotLeafbranch[i].p2==NotLeafbranch[j].p2)
                  RCount[i]++;
          } // for j
       } // for i

       for(int i=0;i<nlbcount;i++)
       {
           int icount = 0;
           if(RCount[i]==1)
           {
               for(int j=0;j<nlbcount;j++)
               {
                   if(NotLeafbranch[i].p2==NotLeafbranch[j].p1)
                       icount++;
               }
               if(icount==0) // it means that p2 is the leaf node
               {
                   NotLeafbranch[i].p2 = (loopno+1)*1000+NotLeafbranch[i].p2;
               }
           } // if
       } // for i

    cutBranch(NotLeafbranch, nlbcount,loopno+1);
  } // if n > 1
  else if(noOfLeafToEnd==1)// noOfLeafToEnd == 1, end of the recursive
  {
      for(int i=0;i<n;i++)
      {
          newRB[newrb[i].RbN].BrOrNo = loopno;
      }
      max_Branch_Order = loopno;
  } // end of the recursive
} // cutBranch

void GLWidget::random_Color()
{
    for(int c=1;c<=max_Branch_Order;c++)
    {
        for(int d=0;d<3;d++)
            rcolor[c][d] = ((rand()%c)*2000*(d+1))%255;
    }
} // ramdom_Color

void GLWidget::showBranchOrderSlot()
{
    glColor3f(0,1,0);
    glBegin(GL_LINES);
         glVertex2i(100,100);
         glVertex2i(200,200);
    glEnd();
    updateGL();
}

// sorting branch for left to right for each junction by using slope
void GLWidget:: LTRBRSorting()
{

  for(int i=0;i<n_da_discs;i++)
  {
    QVector<int> Br4JuncIn; // keep RbN(s) for each junction before sorting
    QVector<int> Br4JuncOut; // keep RbN(s) for each junction after sorting
    for(int j=0;j<WRRB_count;j++)  // get from newRB
    {
        if(newRB[j].p1==i)
        {
            Br4JuncIn.append(newRB[j].RbN);
        }
    } // for WRRB_count

    if(Br4JuncIn.size()>0)
    {
        // sorting
        Br4JuncOut = LeftToRightBRSorting(Br4JuncIn);

        JuncWithSortedRB[i].append(i);
        JuncWithSortedRB[i].append(Br4JuncOut.size());
        for(int k=0;k<Br4JuncOut.size();k++)
        {
            JuncWithSortedRB[i].append(Br4JuncOut.at(k));
        }
    } // if Br4JuncIn.size()>0
  } // for n_da_discs
} // LTRBRSorting


QVector<int> GLWidget::LeftToRightBRSorting(QVector<int> vinput)
{
  // input : A list of branches originated in each junction
  // process : compare slope in the junction with quadrant of i_ct,j_ct
  // Output : A list of sorted branched from left to right originated in each junction

  QVector<int> result;
  //QVector< QVector< int > > q;
  QVector<int> q1,q2,q3,q4;
  QVector<int> Fq4,Fq3,Fq2,Fq1; //keeps rb after being sorted
  QVector<float> Sq1,Sq2,Sq3,Sq4; // keeps slope
  QVector<float> Sq4_c,Sq3_c,Sq2_c,Sq1_c; // copy of slope to get track of rb no.

  // classify each branch in vinput into the right quadrant

  int RefX = newRB[vinput.at(0)].x1;
  int RefY = newRB[vinput.at(0)].y1;

  for(int s=0;s<vinput.size();s++)
  {

    int vx = newRB[vinput.at(s)].x2;
    int vy = newRB[vinput.at(s)].y2;

      if(vx<RefX && vy>RefY) // q4
       q4.append(vinput.at(s));
    else if(vx<=RefX && vy<=RefY) // q3
       q3.append(vinput.at(s));
    else if(vx>=RefX && vy<RefY) // q2
       q2.append(vinput.at(s));
    else if(vx>RefX && vy>=RefY) // q1
       q1.append(vinput.at(s));
  } // for : quadrant classification


  if(q4.size()!=0)
  {
      for(int r=0;r<q4.size();r++)
      {

          int x1 = newRB[q4.at(r)].x1;
          int y1 = newRB[q4.at(r)].y1;
          int x2 = newRB[q4.at(r)].x2;
          int y2 = newRB[q4.at(r)].y2;

          if((x2-x1)!=0)
              Sq4.append((float)(y2-y1)/(x2-x1));
          else Sq4.append(0);
      }
  } // q4.size !=0
  if(q3.size()!=0)
  {
      for(int r=0;r<q3.size();r++)
      {
          int x1 = newRB[q3.at(r)].x1;
          int y1 = newRB[q3.at(r)].y1;
          int x2 = newRB[q3.at(r)].x2;
          int y2 = newRB[q3.at(r)].y2;
          if((x2-x1)!=0)
              Sq3.append((float)(y2-y1)/(x2-x1));
          else Sq3.append(0);
      }
  } // q3.size !=0
  if(q2.size()!=0)
  {
      for(int r=0;r<q2.size();r++)
      {
          int x1 = newRB[q2.at(r)].x1;
          int y1 = newRB[q2.at(r)].y1;
          int x2 = newRB[q2.at(r)].x2;
          int y2 = newRB[q2.at(r)].y2;
          if((x2-x1)!=0)
              Sq2.append((float)(y2-y1)/(x2-x1));
          else Sq2.append(10000);
      }
  } // q2.size !=0
  if(q1.size()!=0)
  {
      for(int r=0;r<q1.size();r++)
      {
          int x1 = newRB[q1.at(r)].x1;
          int y1 = newRB[q1.at(r)].y1;
          int x2 = newRB[q1.at(r)].x2;
          int y2 = newRB[q1.at(r)].y2;
          if((x2-x1)!=0)
              Sq1.append((float)(y2-y1)/(x2-x1));
          else Sq1.append(0);
      }
  } // q1.size !=0


  if(Sq4.size()>0)
  {
      for(int a=0;a<Sq4.size();a++)
          Sq4_c.append(Sq4.at(a));
  }
  if(Sq3.size()>0)
  {
      for(int a=0;a<Sq3.size();a++)
          Sq3_c.append(Sq3.at(a));
  }
  if(Sq2.size()>0)
  {
      for(int a=0;a<Sq2.size();a++)
          Sq2_c.append(Sq2.at(a));
  }
  if(Sq1.size()>0)
  {
      for(int a=0;a<Sq1.size();a++)
          Sq1_c.append(Sq1.at(a));
  }

  //sorting branches in each quadrant
  if(Sq4.size()>=1)
  {
      qStableSort(Sq4.begin(),Sq4.end());

      for(int r=0;r<Sq4.size();r++)
      {
          for(int s=0;s<Sq4.size();s++)
          {
              if(Sq4.at(r)==Sq4_c.at(s))
              {
                  Fq4.append(q4.at(s));
                  break;
              } // if
          } // for s
      } // for r

  } // if Sq4.size()>1

  if(Sq3.size()>=1)
  {
      qStableSort(Sq3.begin(),Sq3.end());

      for(int r=0;r<Sq3.size();r++)
      {
          for(int s=0;s<Sq3.size();s++)
          {
              if(Sq3.at(r)==Sq3_c.at(s))
              {
                  Fq3.append(q3.at(s));
                  break;
              } // if
          } // for s
      } // for r

  } // if Sq3.size()>1

  if(Sq2.size()>=1)
  {
      qStableSort(Sq2.begin(),Sq2.end());

      for(int r=0;r<Sq2.size();r++)
      {
          for(int s=0;s<Sq2.size();s++)
          {
              if(Sq2.at(r)==Sq2_c.at(s))
              {
                  Fq2.append(q2.at(s));
                  break;
              } // if
          } // for s
      } // for r

  } // if Sq2.size()>1

  if(Sq1.size()>=1)
  {
      qStableSort(Sq1.begin(),Sq1.end());

      for(int r=0;r<Sq1.size();r++)
      {
          for(int s=0;s<Sq1.size();s++)
          {
              if(Sq1.at(r)==Sq1_c.at(s))
              {
                  Fq1.append(q1.at(s));
                  break;
              } // if
          } // for s
      } // for r

  } // if q1.size()>1

  // put all q1-4 into result
  if(Fq4.size()>0)
  {
      for(int a=0;a<Fq4.size();a++)
          result.append(Fq4.at(a));
  }
  if(Fq3.size()>0)
  {
      for(int a=0;a<Fq3.size();a++)
          result.append(Fq3.at(a));
  }
  if(Fq2.size()>0)
  {
      for(int a=0;a<Fq2.size();a++)
          result.append(Fq2.at(a));
  }
  if(Fq1.size()>0)
  {
      for(int a=0;a<Fq1.size();a++)
          result.append(Fq1.at(a));
  }

  return result;
} //LeftToRightBranchSorting

void GLWidget::sortAngleInJunc()
{
   //input : JuncWithSortedRB :JuncNo,AmountOfRbs,RbNo#1,RbNo#2
   //process : Sorting using vector
   //  : Real measure_angle(int ai_ct,int aj_ct,int b0i_ct,int b0j_ct,int b1i_ct,int b1j_ct)
   //output : JuncWithSortedAngle : Juncno,AmountOfAngle,Angle#1,Angle#2

    for(int i=0;i<n_da_discs;i++)
    {
        int y0 = da_discs[i].p_i_ct;
        int x0 = da_discs[i].p_j_ct;

        if(JuncWithSortedRB[i].size()>0)
        {
            JuncWithSortedAngle[i].append(i); // JuncNo
            if(JuncWithSortedRB[i].at(1)==1) // if the junc. contains only 1 rb
            {
                JuncWithSortedAngle[i].append(JuncWithSortedRB[i].at(1)-1); //AmountOfAngle
                JuncWithSortedAngle[i].append(measure_angle(y0,x0,y0,x0+50,
                                              newRB[JuncWithSortedRB[i].at(2)].y2,
                                              newRB[JuncWithSortedRB[i].at(2)].x2));
            } // if
            else
            {
                JuncWithSortedAngle[i].append(JuncWithSortedRB[i].at(1)-1); //AmountOfAngle
                for(int j=2;j<JuncWithSortedRB[i].size()-1;j++)
                {
                    int y1 = newRB[JuncWithSortedRB[i].at(j)].y2;
                    int x1 = newRB[JuncWithSortedRB[i].at(j)].x2;
                    int y2 = newRB[JuncWithSortedRB[i].at(j+1)].y2;
                    int x2 = newRB[JuncWithSortedRB[i].at(j+1)].x2;
                    JuncWithSortedAngle[i].append(measure_angle(y0,x0,y1,x1,y2,x2));
                }// for j
            } //else
        } // if size > 0
    } // for i

} //sortAngleInJunc()

QVector<int> GLWidget::JunctionConnectedToLeaf(Ij_PC newrb[MAX_RES])
{
   // input : newRB
   // process : RB no. whose p2 >= 1000
   // output : QVector
   QVector<int> result;

   for(int i=0;i<n_da_discs;i++)
   {
       int c=0;
       if(JuncWithSortedRB[i].size()>0)
       {
           for(int j=2;j<JuncWithSortedRB[i].size();j++)
           {
               if(newrb[JuncWithSortedRB[i].at(j)].p2>=1000)
               {
                   c++;
               }
           } // for j
           if(c==JuncWithSortedRB[i].at(1))
           {
               result.append(i);
           } // if
       } // if size >0
   } // for i

   return result;
} // JunctionConnectedToLeaf

void GLWidget::ListJunctionNotConnectedToLeft()
{
    // input : JuncWithSortedAngle
    // process : junc in the vector templist == JuncWithSortedAngle[]
    // output : array of vector VSortedAngleJuncNotLeaves

    QVector<int> xv;
    xv = JunctionConnectedToLeaf(newRB);

    NLeaveCount = n_da_discs - xv.size();

    for(int i=0;i<n_da_discs;i++)
    {
        int lc = 0;
        for(int j=0;j<xv.size();j++)
        {
            if(i==xv.at(j))
            { lc++;break; }
        } // for j
        if(lc==0) SortedAngleJuncNotLeaves.append(i);
    } // for i

    int cnl = 0;
    for(int i=0;i<n_da_discs;i++)
    {
        for(int j=0;j<SortedAngleJuncNotLeaves.size();j++)
        {
            if(i==SortedAngleJuncNotLeaves.at(j))
            {
                for(int k=0;k<JuncWithSortedAngle[i].size();k++)
                {
                 VSortedAngleJuncNotLeaves[cnl].append(JuncWithSortedAngle[i].at(k));
                }
                cnl++;
            } // if
        } // for j
    } // for i
} // ListJunctionNotConnectedToLeft

void GLWidget::BranchEachBr()
{

    for(int i=0;i<100;i++) BrOr_count[i] = 0;

    for(int i=0;i<WRRB_count;i++) // each rb
    {
        for(int j=0;j<max_Branch_Order;j++) // each branch order
        {
            if(newRB[i].BrOrNo==j+1)
            {
                branch_Order_array[j][BrOr_count[j]].x1 = newRB[i].x1;
                branch_Order_array[j][BrOr_count[j]].y1 = newRB[i].y1;
                branch_Order_array[j][BrOr_count[j]].p1 = newRB[i].p1;
                branch_Order_array[j][BrOr_count[j]].x2 = newRB[i].x2;
                branch_Order_array[j][BrOr_count[j]].y2 = newRB[i].y2;
                branch_Order_array[j][BrOr_count[j]].p2 = newRB[i].p2;
                branch_Order_array[j][BrOr_count[j]].RbN = newRB[i].RbN;
                branch_Order_array[j][BrOr_count[j]].val = newRB[i].val;
                branch_Order_array[j][BrOr_count[j]].BrOrNo = newRB[i].BrOrNo;
                BrOr_count[j]++;
            }  // if
        } //for j
    } // for i

} // BranchEachBr

void GLWidget::RepairLinks()
{
  // list links not connected to the tree
  QVector<int> notconlinks;
  for(int i=0;i<WRRB_count;i++)
  {
    if(newRB[i].val == 0)
    {
        notconlinks.append(newRB[i].RbN);
    } // if
  } // for i

  QVector<int> VNotConLinkJunc;
  for(int i=0;i<n_da_discs;i++)
  {
      for(int j=0;j<WRRB_count;j++)
      {
          if(newRB[j].val>0)
          {
              if(i==newRB[j].p1 || i==newRB[j].p2)
              {
                  VNotConLinkJunc.append(i); break;
              } // if
          } // if .val > 0
      } // for j
  } // for i


  int pp1;
  if(notconlinks.size()>0)
  {
    int fy1,fx1,fy2,fx2;
    for(int i=0;i<notconlinks.size();i++)
    {
        int y1 = WithoutRedundantRB[notconlinks.at(i)].p_i_ct;
        int x1 = WithoutRedundantRB[notconlinks.at(i)].p_j_ct;
        int y2 = WithoutRedundantRB[notconlinks.at(i)].i_ct;
        int x2 = WithoutRedundantRB[notconlinks.at(i)].j_ct;
        bool isleaf = false;
        for(int k=0;k<n_dc_discs;k++)
        {
            if(y1==dc_discs[k].p_i_ct && x1==dc_discs[k].p_j_ct)
            {
             isleaf = true;
             y1 = y2;
             x1 = x2;
             y2 = dc_discs[k].p_i_ct;
             x2 = dc_discs[k].p_j_ct;
            } else
            if(y2==dc_discs[k].p_i_ct && x2==dc_discs[k].p_j_ct)
            {
             isleaf = true;
            }
        } // for k

        float dmin = 1000;
        float dd;
        if(isleaf)
        {
            for(int j=0;j<VNotConLinkJunc.size();j++)
            {
               dd = sqrt(pow((y1-da_discs[VNotConLinkJunc.at(j)].p_i_ct),2)+pow((x1-da_discs[VNotConLinkJunc.at(j)].p_j_ct),2));
               if(dd<dmin)
               {
                 dmin = dd;
                 fy1 = y1;
                 fx1 = x1;
                 fy2 = da_discs[VNotConLinkJunc.at(j)].p_i_ct;
                 fx2 = da_discs[VNotConLinkJunc.at(j)].p_j_ct;
                 pp1 = VNotConLinkJunc.at(j);
               } // if
            } // for j
        } else // not leaf, means to have compare both ends
        {
            for(int j=0;j<VNotConLinkJunc.size();j++)
            {
               dd = sqrt(pow((y1-da_discs[VNotConLinkJunc.at(j)].p_i_ct),2)+pow((x1-da_discs[VNotConLinkJunc.at(j)].p_j_ct),2));
               if(dd<dmin)
               {
                 dmin = dd;
                 fy1 = y1;
                 fx1 = x1;
                 fy2 = da_discs[VNotConLinkJunc.at(j)].p_i_ct;
                 fx2 = da_discs[VNotConLinkJunc.at(j)].p_j_ct;
                 pp1 = VNotConLinkJunc.at(j);
               } // if
            } // for j

            for(int j=0;j<VNotConLinkJunc.size();j++)
            {
               dd = sqrt(pow((y2-da_discs[VNotConLinkJunc.at(j)].p_i_ct),2)+pow((x2-da_discs[VNotConLinkJunc.at(j)].p_j_ct),2));
               if(dd<dmin)
               {
                 dmin = dd;
                 fy1 = y2;
                 fx1 = x2;
                 fy2 = da_discs[VNotConLinkJunc.at(j)].p_i_ct;
                 fx2 = da_discs[VNotConLinkJunc.at(j)].p_j_ct;
                 pp1 = VNotConLinkJunc.at(j);
               } // if
            } // for j
        } // not leaf

        repairedRB[new_n_rb].p_i_ct = fy1;
        repairedRB[new_n_rb].p_j_ct = fx1;
        repairedRB[new_n_rb].i_ct = fy2;
        repairedRB[new_n_rb].j_ct = fx2;
        repairedRB[new_n_rb].i1_ct = pp1;
        repairedRB[new_n_rb].meas = dmin;
        new_n_rb++;

    } // for links in notconlinks vector

    //correctRB();

    bool check[new_n_rb];
    for(int i=0;i<new_n_rb;i++)
        check[i] = false;

    for(int i=0;i<new_n_rb;i++)
    {
        if(repairedRB[i].meas==0) check[i]=true;
        if(!check[i])
        {
            for(int j=(i+1);j<new_n_rb;j++)
            {
                if(
                    ((repairedRB[i].meas==repairedRB[j].meas)
                    && (repairedRB[i].p_i_ct==repairedRB[j].i_ct)
                    && (repairedRB[i].p_j_ct==repairedRB[j].j_ct)
                    && (repairedRB[i].i_ct==repairedRB[j].p_i_ct)
                    && (repairedRB[i].j_ct==repairedRB[j].p_j_ct))
                    ||
                    ((repairedRB[i].meas==repairedRB[j].meas)
                    && (repairedRB[i].p_i_ct==repairedRB[j].p_i_ct)
                    && (repairedRB[i].p_j_ct==repairedRB[j].p_j_ct))
                    )
                {
                    check[j] = true;
                } // if meas is equal
            } // for j
        }  // if check
    } // for i

    Medge NoredundantRB[MAX_RES];
    int rbcount = 0;
    for(int i=0;i<new_n_rb;i++)
    {
        if(!check[i])
        {
          NoredundantRB[rbcount].meas = repairedRB[i].meas;
          NoredundantRB[rbcount].p_i_ct = repairedRB[i].p_i_ct; // y
          NoredundantRB[rbcount].p_j_ct = repairedRB[i].p_j_ct; // x
          NoredundantRB[rbcount].i_ct = repairedRB[i].i_ct;
          NoredundantRB[rbcount].j_ct = repairedRB[i].j_ct;
          NoredundantRB[rbcount].i1_ct = repairedRB[i].i1_ct;
          rbcount++;
        }
    } // for i

    QVector<int> rbsdc;
    for(int i=0;i<n_dc_discs;i++)
    {
        rbsdc = getRBsWithDcs(dc_discs[i].p_i_ct,dc_discs[i].p_j_ct,NoredundantRB,rbcount);
        if(rbsdc.size()>1)
        {
            for(int j=1;j<rbsdc.size();j++)
            {
                check[rbsdc.at(j)]= true;
            }
        } // if
    }
    // dc checking end

    WRRB_count = rbcount;
    // copy local to global variables.
    for(int i=0;i<WRRB_count;i++)
    {
        WithoutRedundantRB[i].meas = NoredundantRB[i].meas;
        WithoutRedundantRB[i].p_i_ct = NoredundantRB[i].p_i_ct; // y
        WithoutRedundantRB[i].p_j_ct = NoredundantRB[i].p_j_ct; // x
        WithoutRedundantRB[i].i_ct = NoredundantRB[i].i_ct;
        WithoutRedundantRB[i].j_ct = NoredundantRB[i].j_ct;
        WithoutRedundantRB[i].i1_ct = NoredundantRB[i].i1_ct;
    }
  } // if notconlinks.size()>0

} // RepairLinks

QVector<int> GLWidget::getRBsWithDcs(int dcy,int dcx,Medge noredundantrb[MAX_RES],int rbc)
{
  QVector<int> result;
  for(int i=0;i<rbc;i++)
  {
      if(noredundantrb[i].meas > 0)
      {
        if((noredundantrb[i].p_i_ct==dcy && noredundantrb[i].p_j_ct==dcx)
           ||
           (noredundantrb[i].i_ct==dcy && noredundantrb[i].j_ct==dcx))
        {
            result.append(i);
        } // if compare dcy and dcx
      } // if meas > 0
  } // for i

  return result;
} // getRBsWithDcs

void GLWidget::RepairBranchAction()
{
    bool_correctrb = true;
    clearWRRB();
    correctRB();
    copy_clearWRRB();
    updateGL();
}

void GLWidget::clearWRRB()
{
    for(int i=0;i<MAX_RES;i++)
    {
        WithoutRedundantRB[i].p_i_ct=0;
        WithoutRedundantRB[i].p_j_ct=0;
        WithoutRedundantRB[i].i_ct=0;
        WithoutRedundantRB[i].j_ct=0;
        WithoutRedundantRB[i].i1_ct=0;
        WithoutRedundantRB[i].j1_ct=0;
        WithoutRedundantRB[i].meas=0;
    }
} // clearWRRB

void GLWidget::copy_clearWRRB()
{
  WRRB_count_BeforeTreeCon = WRRB_count;
  for(int i=0;i<WRRB_count_BeforeTreeCon;i++)
  {
      WithoutRedundantRB_BeforeTreeCon[i].p_i_ct=WithoutRedundantRB[i].p_i_ct;
      WithoutRedundantRB_BeforeTreeCon[i].p_j_ct=WithoutRedundantRB[i].p_j_ct;
      WithoutRedundantRB_BeforeTreeCon[i].i_ct=WithoutRedundantRB[i].i_ct;
      WithoutRedundantRB_BeforeTreeCon[i].j_ct=WithoutRedundantRB[i].j_ct;
      WithoutRedundantRB_BeforeTreeCon[i].i1_ct=WithoutRedundantRB[i].i1_ct;
      WithoutRedundantRB_BeforeTreeCon[i].j1_ct=WithoutRedundantRB[i].j1_ct;
      WithoutRedundantRB_BeforeTreeCon[i].meas=WithoutRedundantRB[i].meas;
  }
  WRRB_count = 0;
  clearWRRB();
} // copy_clearWRRB

void GLWidget::setNewRoot()
{

}

void GLWidget::XYPosSetting()
{
    pInputDlg = new QDialog();
    pInputDlg->setFixedSize(500, 120);

    QLabel* dLblLabel = new QLabel(pInputDlg);
    dLblLabel->setText("Starting X Position");
    dLblLabel->setGeometry(20, 5, 220, 20);

    dEdtInputStr = new QLineEdit(pInputDlg);
    dEdtInputStr->setGeometry(300, 5, 305, 20);
    dEdtInputStr->setText(QString::number(x_pos_start));
    dEdtInputStr->resize(50, dEdtInputStr->height());

    QLabel* uLblLabel = new QLabel(pInputDlg);
    uLblLabel->setText("Starting Y Position");
    uLblLabel->setGeometry(20, 30, 200, 20);

    uEdtInputStr = new QLineEdit(pInputDlg);
    uEdtInputStr->setGeometry(300, 30, 310, 20);
    uEdtInputStr->setText(QString::number(y_pos_start));
    uEdtInputStr->resize(50, uEdtInputStr->height());

    QPushButton* okbutton = new QPushButton(pInputDlg);
    okbutton->setText("OK");
    okbutton->setGeometry(120, 90, 80, 20);

    QPushButton* cancelbutton = new QPushButton(pInputDlg);
    cancelbutton->setText("Cancel");
    cancelbutton->setGeometry(210, 90, 150, 20);

    pInputDlg->show();
    dEdtInputStr->setFocus();

    connect( okbutton, SIGNAL(clicked()), this, SLOT(XYPosokClicked()) );
    connect( cancelbutton, SIGNAL(clicked()), this, SLOT(XYPoscancelClicked()) );

}

void GLWidget::XYPosokClicked()
{

    x_pos_start = dEdtInputStr->text().toInt();
    y_pos_start = ceil(uEdtInputStr->text().toInt());
    pInputDlg->close();

}

void GLWidget::XYPoscancelClicked()
{
    pInputDlg->close();
}


float GLWidget::CalCCCLength(int con[MAX_ISIZE][MAX_ISIZE], int imax, int jmax)
{

    float result = 0;
    int alignAmnt=0;
    int diagAmnt=0;

    int src[imax][jmax];

    for(int i=0;i<imax;i++)
        for(int j=0;j<jmax;j++)
            src[i][j]=0;

    for(int i=0;i<imax;i++)
        for(int j=0;j<jmax;j++)
            src[i][j] = con[i][j];

    // replace contour pixels(1) with chain code (2-9)
    for(int i=0;i<imax;i++)
    {
        for(int j=0;j<jmax;j++)
        {
           if(src[i][j]!=0)
           {
               if(src[i-1][j-1]!=0) src[i-1][j-1]=5;
               if(src[i-1][j]!=0) src[i-1][j]=4;
               if(src[i-1][j+1]!=0) src[i-1][j+1]=3;
               if(src[i][j-1]!=0) src[i][j-1]=6;
               if(src[i+1][j-1]!=0) src[i+1][j-1]=7;
               if(src[i+1][j]!=0) src[i+1][j]=8;
               if(src[i+1][j+1]!=0) src[i+1][j+1]=9;
               if(src[i][j+1]!=0) src[i][j+1]=2;
           } // if it is contour pixels
        } // for col
    } // for row
    // count pixel

    for(int i=0;i<imax;i++)
    {
        for(int j=0;j<jmax;j++)
        {
          if(src[i][j]!=0)
              if(src[i][j]==2 || src[i][j]==4 || src[i][j]==6 || src[i][j]==8)
                  alignAmnt++;
              else diagAmnt++;
        }
    }
    result = alignAmnt + 1.414*diagAmnt;

    if(result==0) result=1;
    return result;
} //ContourLength



