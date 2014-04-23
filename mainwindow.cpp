#include <QtOpenGL>
#include <QtGui>
#include <QSysInfo>

#include <string.h>
#include <math.h>

#include "mainwindow.h"
#include "glwidget.h"
#include "fratype.h"
#include "maingl.h"
#include "basictool.h"

MainWindow::MainWindow()
{

    // set file paths
    pngpath = "png/";
    imagepath = "ImageFiles/";
    analysispath = "AnalysisFiles/";
    capturedpath = "CapturedPictureFiles/";

    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QMessageBox welcome_box(this);
    welcome_box.setWindowTitle("2D Branching Form Morphometrics");
    welcome_box.setWindowIcon(QIcon(pngpath+"welcome.ico"));
    welcome_box.setText("Welcome to 2D Branching Form Morphometrics");
    welcome_box.setIconPixmap(QPixmap(pngpath+"welcomebranchometer.png"));
    welcome_box.setParent(this);
    welcome_box.exec();


    glWidget = new GLWidget;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setStyleSheet("background-color:black");
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->verticalScrollBar()->setValue(20);
    glWidgetArea->horizontalScrollBar()->setValue(100);
    glWidgetArea->adjustSize();
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(500, 500);

    glWidget->pngpath = pngpath;
    glWidget->analysispath = analysispath;
    glWidget->ppi = sqrt(pow(QApplication::desktop()->width(),2.0)+pow(QApplication::desktop()->height(),2.0))/13.0;
    glWidget->ppmm = glWidget->ppi/25.4;

    curdir =imagepath;
    anacurdir = analysispath;

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);
    centralWidget->setLayout(centralLayout);


    setWindowTitle(tr("2D Branching Form Morphometrics"));
    setUnifiedTitleAndToolBarOnMac(true);
    setActions(false);
    //setScreenDiagonal();
}

void MainWindow::setScreenDiagonal()
{
    bool ok;
    double val = QInputDialog::getDouble(this, tr("Screen Diagonal Request"),tr("Please Input Your Screen Diagonal Length (inches) : "), 13.0, 5.0, 40.0, 2, &ok);
    if(ok)
        glWidget->screen_diagonal = (float)val;
    glWidget->ppi = sqrt(pow(QApplication::desktop()->width(),2.0)+pow(QApplication::desktop()->height(),2.0))/glWidget->screen_diagonal;
    glWidget->ppmm = glWidget->ppi/25.4;
} // setScreenDiagonal

void MainWindow::print(QPrinter *printer)
{
    int w = glWidget->grabFrameBuffer().width();
    int h = glWidget->grabFrameBuffer().height();

     #ifndef QT_NO_PRINTDIALOG
     QPixmap pixmap = glWidget->renderPixmap(w,h);


     QPrintDialog *dlg = new QPrintDialog(printer, this);
     if (dlg->exec() != QDialog::Accepted)
         return;

     QPainter painter(printer);
     QRect rect = painter.viewport();
     QSize size = glWidget->grabFrameBuffer().size();
     size.scale(rect.size(), Qt::KeepAspectRatio);
     painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
     painter.setWindow(glWidget->grabFrameBuffer().rect());
     painter.drawPixmap(0, 0, pixmap);

     statusBar()->showMessage(tr("Ready"), 2000);
 #endif

}

void MainWindow::imagePrint()
{

    // display print dialog and if accepted print
    QPrinter       printer( QPrinter::HighResolution );
    QPrintDialog   dialog( &printer, this );
    if ( dialog.exec() == QDialog::Accepted ) print(&printer);

}
void MainWindow::printpreview(QPrinter* printer)
{
    int w = glWidget->grabFrameBuffer().width();
    int h = glWidget->grabFrameBuffer().height();

    QPixmap pixmap = glWidget->renderPixmap(w,h);

    QGraphicsScene * scene = new QGraphicsScene();
    QPainter painter(printer);
    painter.drawPixmap(0, 0, pixmap);
    scene->render(&painter, QRect(0, 0, w, h));
}

void MainWindow::imagePrintPreview()
{
 #ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowFlags ( Qt::Window );
    connect(&preview,SIGNAL(paintRequested(QPrinter*)), SLOT(printpreview(QPrinter*)));
    preview.exec();
#endif
}

void MainWindow::save()
{
    QString picturepath = capturedpath;
    QString pictureext = ".jpg";

    QString picturefullpath = picturepath + newsavedfilename + pictureext;

    QString fileName = QFileDialog::getSaveFileName(
    this,
    "Save Picture Dialog",
    picturefullpath
     );

     if (fileName.isEmpty())
         return;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Picture Saving"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     glWidget->grabFrameBuffer().save(fileName);
     statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);

}

void MainWindow::updateStatReport(QString filename_input)
{
    figanalysis->clear();
    int ind = filename_input.lastIndexOf("/");
    glWidget->onlyfilename=filename_input.right(filename_input.length()-ind-1);
    figanalysis->addItem("Image Name: "+glWidget->onlyfilename);

    figanalysis->addItem("Scale Unit: "+glWidget->scaleunit);

    char FtoC[50];
    sprintf(FtoC,"%.2f",glWidget->image_x/glWidget->ppmm);
    figanalysis->addItem("Image Width: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->image_y/glWidget->ppmm);
    figanalysis->addItem("Image Height: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->figure_length_x/glWidget->ppmm);
    figanalysis->addItem("Object Width: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->figure_length_y/glWidget->ppmm);
    figanalysis->addItem("Object Height: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->PictureAreaMM);
    figanalysis->addItem("Object Area: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->ContourSize/glWidget->ppmm);
    figanalysis->addItem("Contour length: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->fractalDim);
    figanalysis->addItem("Fractal Dimension: "+(QString)FtoC);

    sprintf(FtoC,"%d",glWidget->junction_amount);
    figanalysis->addItem("The amount of junctions: "+(QString)FtoC);

    sprintf(FtoC,"%d",glWidget->terminal_amount);
    figanalysis->addItem("The amount of terminals: "+(QString)FtoC);

    QVector<QString> header;
    header.append("Amount of Junction Discs : ");
    header.append("Minimum Junction Diameter : ");
    header.append("Maximum Junction Diameter : ");
    header.append("Average Junction Diameter : ");
    header.append("STD of Junction Diameter : ");

    header.append("Amount of Branch Discs : ");
    header.append("Minimum Branch Diameter : ");
    header.append("Maximum Branch Diameter : ");
    header.append("Average Branch Diameter : ");
    header.append("STD of Branch Diameter : ");

    header.append("Amount of Terminal Discs : ");
    header.append("Minimum Terminal Diameter : ");
    header.append("Maximum Terminal Diameter : ");
    header.append("Average Terminal Diameter : ");
    header.append("STD of Terminal Diameter : ");

    header.append("Amount of branches : ");
    header.append("Minimum branch length : ");
    header.append("Maximum branch length : ");
    header.append("Average branch length : ");
    header.append("STD of branch length : ");

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

    header.append("Amount of Branch Spacing : ");
    header.append("Minimum Branch Spacing Distance : ");
    header.append("Maximum Branch Spacing Distance : ");
    header.append("Average Branch Spacing Distance : ");
    header.append("STD of Branch Spacing Distance : ");

    header.append("Amount of branch angles : ");
    header.append("Minimum branch angle : ");
    header.append("Maximum branch angle : ");
    header.append("Average branch angle : ");
    header.append("STD of branch angle : ");

    header.append("Amount of ultimate branches : ");
    header.append("Minimum ultimate branch length : ");
    header.append("Maximum ultimate branch length : ");
    header.append("Average ultimate branch length : ");
    header.append("STD of ultimate branch length : ");

    header.append("After Tree Construction");

    header.append("Amount of branches : ");
    header.append("Minimum branch length : ");
    header.append("Maximum branch length : ");
    header.append("Average branch length : ");
    header.append("STD of branch length : ");

    header.append("Amount of Branch Order : ");

    header.append("Amount of branch angles : ");
    header.append("Minimum branch angle : ");
    header.append("Maximum branch angle : ");
    header.append("Average branch angle : ");
    header.append("STD of branch angle : ");

    header.append("Amount of ultimate branches : ");
    header.append("Minimum ultimate branch length : ");
    header.append("Maximum ultimate branch length : ");
    header.append("Average ultimate branch length : ");
    header.append("STD of ultimate branch length : ");

    for(int i=0;i<glWidget->m_n_da_discs;i++)
        glWidget->prt_da_discs[i].meas=2*(glWidget->m_da_discs[i].meas/glWidget->ppmm);

    for(int i=0;i<glWidget->m_n_db_discs;i++)
        glWidget->prt_db_discs[i].meas=2*(glWidget->m_db_discs[i].meas/glWidget->ppmm);

    for(int i=0;i<glWidget->m_n_dc_discs;i++)
        glWidget->prt_dc_discs[i].meas=2*(glWidget->m_dc_discs[i].meas/glWidget->ppmm);

    for(int i=0;i<glWidget->m_n_br_spacing;i++)
        glWidget->prt_br_spacing[i].meas=2*(glWidget->m_br_spacing[i].meas/glWidget->ppmm);

    if(glWidget->bool_correctrb)
    {
        for(int i=0;i<glWidget->WRRB_count_BeforeTreeCon;i++)
        {
            glWidget->prt_rb[i].meas=glWidget->WithoutRedundantRB_BeforeTreeCon[i].meas/glWidget->ppmm;
        }
    }
    else
    {
        for(int i=0;i<glWidget->m_n_rb;i++)
        {
            glWidget->prt_rb[i].meas=glWidget->m_rb[i].meas/glWidget->ppmm;
        }
    } // else

    for(int i=0;i<glWidget->BOrder1_Count;i++)
    {
        glWidget->prt_urb[i].meas = glWidget->Branch_Order[0][i].meas;
    }

    int a_count=0;
    Real a_min = 361.0;
    Real a_max = -1.0;
    Real a_sum = 0.0;
    Real a_avg = 0.0;
    Real a_std_dev = 0.0;
    Real a_variance = 0.0;
    Real a_sqrt_sum = 0.0;


    for(int ai=0;ai<500;ai++) // MAXJUNC = 500
    {
        if(glWidget->AngleResult[ai][0]>0)
        {
            a_count+=glWidget->AngleResult[ai][1];
            for(int aj=2;aj<25;aj++) // ANGLEATT = 25
            {
                if(glWidget->AngleResult[ai][aj]!=0)
                {
                    a_sum+=glWidget->AngleResult[ai][aj];
                    a_sqrt_sum+=glWidget->AngleResult[ai][aj]*glWidget->AngleResult[ai][aj];
                    if(glWidget->AngleResult[ai][aj]>a_max) a_max = glWidget->AngleResult[ai][aj];
                    if(glWidget->AngleResult[ai][aj]<a_min) a_min = glWidget->AngleResult[ai][aj];
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
    for(int i=0;i<glWidget->WRRB_count;i++)
    {
        if(glWidget->newRB[i].val > 0)
        {
            brTemp[i].meas = glWidget->newRB[i].val/glWidget->ppmm;
          realbrcount++;
        } // if
    }

    for( int i=0;i<glWidget->max_Branch_Order;i++)
    {
        Medge BrOrTemp[100];
        for(int j=0;j<glWidget->BrOr_count[i];j++)
        {
            BrOrTemp[j].meas = glWidget->branch_Order_array[i][j].val/glWidget->ppmm;
        } //j

    } // for i : each branch order

    int BrAngleAmt_Count = 0;
    Medge BrAngles[5000];
    int ca = 0;

        for(int i=0;i<glWidget->m_n_da_discs;i++)
        {
            if(glWidget->JuncWithSortedAngle[i].size()>0)
            {
                if(glWidget->JuncWithSortedAngle[i].at(1)>0)
                     BrAngleAmt_Count += glWidget->JuncWithSortedAngle[i].at(1);
                else BrAngleAmt_Count++;
                if(glWidget->JuncWithSortedAngle[i].size()>2)
                {
                    for(int j=2;j<glWidget->JuncWithSortedAngle[i].size();j++)
                    {
                        BrAngles[ca].meas = glWidget->JuncWithSortedAngle[i].at(j);
                        ca++;
                    } // for j
                } // // if
            }// if
        } // for i

    int NLBrAngleAmt_Count = 0;
    Medge NLBrAngles[5000];
    int nlca = 0;

        for(int i=0;i<glWidget->NLeaveCount;i++)
        {
            if(glWidget->VSortedAngleJuncNotLeaves[i].size()>0)
            {
                if(glWidget->VSortedAngleJuncNotLeaves[i].at(1)>0)
                     NLBrAngleAmt_Count += glWidget->VSortedAngleJuncNotLeaves[i].at(1);
                else NLBrAngleAmt_Count++;

                for(int j=2;j<glWidget->VSortedAngleJuncNotLeaves[i].size();j++)
                {
                    NLBrAngles[nlca].meas = glWidget->VSortedAngleJuncNotLeaves[i].at(j);
                    nlca++;
                } // for j
            }// if
        } // for i

    QVector<float> summary_data;
    summary_data.append(glWidget->m_n_da_discs);
    summary_data.append(glWidget->findMin(glWidget->prt_da_discs,glWidget->m_n_da_discs));
    summary_data.append(glWidget->findMax(glWidget->prt_da_discs,glWidget->m_n_da_discs));
    summary_data.append(glWidget->findAvg(glWidget->prt_da_discs,glWidget->m_n_da_discs));
    summary_data.append(glWidget->findS_dev(glWidget->prt_da_discs,glWidget->m_n_da_discs));

    summary_data.append(glWidget->m_n_db_discs);
    summary_data.append(glWidget->findMin(glWidget->prt_db_discs,glWidget->m_n_db_discs));
    summary_data.append(glWidget->findMax(glWidget->prt_db_discs,glWidget->m_n_db_discs));
    summary_data.append(glWidget->findAvg(glWidget->prt_db_discs,glWidget->m_n_db_discs));
    summary_data.append(glWidget->findS_dev(glWidget->prt_db_discs,glWidget->m_n_db_discs));

    summary_data.append(glWidget->m_n_dc_discs);
    summary_data.append(glWidget->findMin(glWidget->prt_dc_discs,glWidget->m_n_dc_discs));
    summary_data.append(glWidget->findMax(glWidget->prt_dc_discs,glWidget->m_n_dc_discs));
    summary_data.append(glWidget->findAvg(glWidget->prt_dc_discs,glWidget->m_n_dc_discs));
    summary_data.append(glWidget->findS_dev(glWidget->prt_dc_discs,glWidget->m_n_dc_discs));

    if(glWidget->bool_correctrb)
    {
        summary_data.append(glWidget->WRRB_count_BeforeTreeCon);
        summary_data.append(glWidget->findMin(glWidget->prt_rb,glWidget->WRRB_count_BeforeTreeCon));
        summary_data.append(glWidget->findMax(glWidget->prt_rb,glWidget->WRRB_count_BeforeTreeCon));
        summary_data.append(glWidget->findAvg(glWidget->prt_rb,glWidget->WRRB_count_BeforeTreeCon));
        summary_data.append(glWidget->findS_dev(glWidget->prt_rb,glWidget->WRRB_count_BeforeTreeCon));
    }
    else
    {
        summary_data.append(glWidget->m_n_rb);
        summary_data.append(glWidget->findMin(glWidget->prt_rb,glWidget->m_n_rb));
        summary_data.append(glWidget->findMax(glWidget->prt_rb,glWidget->m_n_rb));
        summary_data.append(glWidget->findAvg(glWidget->prt_rb,glWidget->m_n_rb));
        summary_data.append(glWidget->findS_dev(glWidget->prt_rb,glWidget->m_n_rb));
    }
    summary_data.append(glWidget->m_n_geotropy);
    summary_data.append(glWidget->findMin(glWidget->m_geotropy,glWidget->m_n_geotropy));
    summary_data.append(glWidget->findMax(glWidget->m_geotropy,glWidget->m_n_geotropy));
    summary_data.append(glWidget->findAvg(glWidget->m_geotropy,glWidget->m_n_geotropy));
    summary_data.append(glWidget->findS_dev(glWidget->m_geotropy,glWidget->m_n_geotropy));

    summary_data.append(glWidget->m_term_n_geotropy);
    summary_data.append(glWidget->findMin(glWidget->m_term_geotropy,glWidget->m_term_n_geotropy));
    summary_data.append(glWidget->findMax(glWidget->m_term_geotropy,glWidget->m_term_n_geotropy));
    summary_data.append(glWidget->findAvg(glWidget->m_term_geotropy,glWidget->m_term_n_geotropy));
    summary_data.append(glWidget->findS_dev(glWidget->m_term_geotropy,glWidget->m_term_n_geotropy));

    summary_data.append(glWidget->m_n_br_spacing);
    summary_data.append(glWidget->findMin(glWidget->prt_br_spacing,glWidget->m_n_br_spacing));
    summary_data.append(glWidget->findMax(glWidget->prt_br_spacing,glWidget->m_n_br_spacing));
    summary_data.append(glWidget->findAvg(glWidget->prt_br_spacing,glWidget->m_n_br_spacing));
    summary_data.append(glWidget->findS_dev(glWidget->prt_br_spacing,glWidget->m_n_br_spacing));

    summary_data.append(a_count);
    summary_data.append(a_min);
    summary_data.append(a_max);
    summary_data.append(a_avg);
    summary_data.append(a_std_dev);

    summary_data.append(glWidget->BOrder1_Count);
    summary_data.append(glWidget->findMin(glWidget->prt_urb,glWidget->BOrder1_Count));
    summary_data.append(glWidget->findMax(glWidget->prt_urb,glWidget->BOrder1_Count));
    summary_data.append(glWidget->findAvg(glWidget->prt_urb,glWidget->BOrder1_Count));
    summary_data.append(glWidget->findS_dev(glWidget->prt_urb,glWidget->BOrder1_Count));

    summary_data.append(0);

    summary_data.append(realbrcount);
    summary_data.append(glWidget->findMin(brTemp,realbrcount));
    summary_data.append(glWidget->findMax(brTemp,realbrcount));
    summary_data.append(glWidget->findAvg(brTemp,realbrcount));
    summary_data.append(glWidget->findS_dev(brTemp,realbrcount));

    summary_data.append(glWidget->max_Branch_Order);

    summary_data.append(BrAngleAmt_Count);
    summary_data.append(glWidget->findMin(BrAngles,BrAngleAmt_Count));
    summary_data.append(glWidget->findMax(BrAngles,BrAngleAmt_Count));
    summary_data.append(glWidget->findAvg(BrAngles,BrAngleAmt_Count));
    summary_data.append(glWidget->findS_dev(BrAngles,BrAngleAmt_Count));

    summary_data.append(NLBrAngleAmt_Count);
    summary_data.append(glWidget->findMin(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(glWidget->findMax(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(glWidget->findAvg(NLBrAngles,NLBrAngleAmt_Count));
    summary_data.append(glWidget->findS_dev(NLBrAngles,NLBrAngleAmt_Count));

    for(int i=0;i<summary_data.size();i++)
    {
        sprintf(FtoC,"%.2f",summary_data.at(i));
        figanalysis->addItem(header.at(i)+" "+(QString)FtoC);
    }

} // updateStatReport

void MainWindow::updateReport(QString filename_input)
{

    figreport->clear();
    int ind = filename_input.lastIndexOf("/");
    glWidget->onlyfilename=filename_input.right(filename_input.length()-ind-1);
    figreport->addItem("Image Name: "+glWidget->onlyfilename);

    figreport->addItem("Scale Unit: "+glWidget->scaleunit);

    char FtoC[50];
    sprintf(FtoC,"%.2f",glWidget->image_x/glWidget->ppmm);
    figreport->addItem("Image Width: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->image_y/glWidget->ppmm);
    figreport->addItem("Image Height: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->figure_length_x/glWidget->ppmm);
    figreport->addItem("Object Width: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->figure_length_y/glWidget->ppmm);
    figreport->addItem("Object Height: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->PictureAreaMM);
    figreport->addItem("Object Area: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->ContourSize/glWidget->ppmm);
    figreport->addItem("Contour length: "+(QString)FtoC);

    sprintf(FtoC,"%.2f",glWidget->fractalDim);
    figreport->addItem("Fractal Dimension: "+(QString)FtoC);

    sprintf(FtoC,"%d",glWidget->junction_amount);
    figreport->addItem("The amount of junctions: "+(QString)FtoC);

    sprintf(FtoC,"%d",glWidget->terminal_amount);
    figreport->addItem("The amount of terminals: "+(QString)FtoC);

    figreport->addItem("The number of discs type A : "+QString::number(glWidget->m_n_da_discs));

    for(int i=0;i<glWidget->m_n_da_discs;i++)
    {
        char FtoCtemp[50];
        sprintf(FtoCtemp,"%.2f",2*glWidget->m_da_discs[i].meas/glWidget->ppmm);
        figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_da_discs[i].p_i_ct)
                           +","+QString::number(glWidget->m_da_discs[i].p_j_ct)+") : "
                           +QString(FtoCtemp));
    }

    figreport->addItem("The number of discs type B : "+QString::number(glWidget->m_n_db_discs));

    for(int i=0;i<glWidget->m_n_db_discs;i++)
    {
        char FtoCtemp[50];
        sprintf(FtoCtemp,"%.2f",2*glWidget->m_db_discs[i].meas/glWidget->ppmm);
        figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_db_discs[i].p_i_ct)
                           +","+QString::number(glWidget->m_db_discs[i].p_j_ct)+") : "
                           +QString(FtoCtemp));
    }
     // disc c
    figreport->addItem("The number of discs type C : "+QString::number(glWidget->m_n_dc_discs));

    for(int i=0;i<glWidget->m_n_dc_discs;i++)
    {
        char FtoCtemp[50];
        sprintf(FtoCtemp,"%.2f",2*glWidget->m_dc_discs[i].meas/glWidget->ppmm);
        figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_dc_discs[i].p_i_ct)
                           +","+QString::number(glWidget->m_dc_discs[i].p_j_ct)+") : "
                           +QString(FtoCtemp));
    }

    // rb

    if(glWidget->bool_correctrb)
    {
        figreport->addItem("The number of branches : "+QString::number(glWidget->WRRB_count_BeforeTreeCon));
        for(int i=0;i<glWidget->WRRB_count_BeforeTreeCon;i++)
        {
            char FtoCtemp[50];
            sprintf(FtoCtemp,"%.2f",glWidget->WithoutRedundantRB_BeforeTreeCon[i].meas/glWidget->ppmm);
            figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->WithoutRedundantRB_BeforeTreeCon[i].p_i_ct)
                               +","+QString::number(glWidget->WithoutRedundantRB_BeforeTreeCon[i].p_j_ct)+") & ("
                               +QString::number(glWidget->WithoutRedundantRB_BeforeTreeCon[i].i_ct)+","
                               +QString::number(glWidget->WithoutRedundantRB_BeforeTreeCon[i].j_ct)+") : "
                               +QString(FtoCtemp));
        } // for
    }
    else // else bool_correctrb = false
    {
        figreport->addItem("The number of branches : "+QString::number(glWidget->m_n_rb));
        for(int i=0;i<glWidget->m_n_rb;i++)
        {
            char FtoCtemp[50];
            sprintf(FtoCtemp,"%.2f",glWidget->m_rb[i].meas/glWidget->ppmm);
            figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_rb[i].p_i_ct)
                               +","+QString::number(glWidget->m_rb[i].p_j_ct)+") & ("
                               +QString::number(glWidget->m_rb[i].i_ct)+","
                               +QString::number(glWidget->m_rb[i].j_ct)+") : "
                               +QString(FtoCtemp));
        } // for

    }

    // Ultimate branch
    figreport->addItem("The number of ultimate branches : "+QString::number(glWidget->BOrder1_Count));

     for(int i=0;i<glWidget->BOrder1_Count;i++)
     {
         char FtoCtemp[50];
         sprintf(FtoCtemp,"%.2f",glWidget->Branch_Order[0][i].meas);
         figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->Branch_Order[0][i].p_i_ct)
                            +","+QString::number(glWidget->Branch_Order[0][i].p_j_ct)+") & ("
                            +QString::number(glWidget->Branch_Order[0][i].i_ct)+","
                            +QString::number(glWidget->Branch_Order[0][i].j_ct)+") : "
                            +QString(FtoCtemp));
     }


    // geo
   figreport->addItem("The number of geo : "+QString::number(glWidget->m_n_geotropy));

    for(int i=0;i<glWidget->m_n_geotropy;i++)
    {
        char FtoCtemp[50];
        sprintf(FtoCtemp,"%.2f",glWidget->m_geotropy[i].meas);
        figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_geotropy[i].p_i_ct)
                           +","+QString::number(glWidget->m_geotropy[i].p_j_ct)+") & ("
                           +QString::number(glWidget->m_geotropy[i].i_ct)+","
                           +QString::number(glWidget->m_geotropy[i].j_ct)+") : "
                           +QString(FtoCtemp));
    }

    // term
    figreport->addItem("The number of term : "+QString::number(glWidget->m_term_n_geotropy));

    for(int i=0;i<glWidget->m_term_n_geotropy;i++)
    {
        char FtoCtemp[50];
        sprintf(FtoCtemp,"%.2f",glWidget->m_term_geotropy[i].meas);

        figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_term_geotropy[i].p_i_ct)
                           +","+QString::number(glWidget->m_term_geotropy[i].p_j_ct)+") & ("
                           +QString::number(glWidget->m_term_geotropy[i].i_ct)+","
                           +QString::number(glWidget->m_term_geotropy[i].j_ct)+") : "
                           +QString(FtoCtemp));
    }

    // Branch Angle
    figreport->addItem("Detail of Branch Angle");
    for(int i=0;i<500;i++) // MAXJUNC = 500
    {
        if(glWidget->AngleResult[i][0]>0)
        {
            QString jitem="";
            for(int j=0;j<25;j++)
            {
                if(glWidget->AngleResult[i][j]!=0)
                {
                    char FtoCtemp[50];
                    sprintf(FtoCtemp,"%.2f",glWidget->AngleResult[i][j]);
                    jitem = jitem+QString(FtoCtemp)+" ";
                }
             //   printf("angleResult from updateReport %f",glWidget->AngleResult[i][j]);
            } // for j
            figreport->addItem("  "+QString::number((i+1))+")- "+jitem);
        }
    } // for i

    // br_spacing
   figreport->addItem("The number of brach spacing : "+QString::number(glWidget->m_n_br_spacing));
   for(int i=0;i<glWidget->m_n_br_spacing;i++)
   {
       char FtoCtemp[50];
       sprintf(FtoCtemp,"%.2f",glWidget->m_br_spacing[i].meas/glWidget->ppmm);
       figreport->addItem("  "+QString::number((i+1))+"-("+QString::number(glWidget->m_br_spacing[i].p_i_ct)
                          +","+QString::number(glWidget->m_br_spacing[i].p_j_ct)+") : "
                          +QString(FtoCtemp));
   }

   // After tree construction
   figreport->addItem("\n---After Tree Construction---\n");

   int jacount = 0;
   figreport->addItem("Branch angles for each junction");
    for (int i = 0; i < glWidget->m_n_da_discs; i++)
    {
        if(glWidget->JuncWithSortedAngle[i].size()>0)
        {
          QString t = "";
          char FtoCtemp[50];
          for( int j=0;j<glWidget->JuncWithSortedAngle[i].size();j++)
          {
            sprintf(FtoCtemp,"%.2f ",glWidget->JuncWithSortedAngle[i].at(j));
            t = t + QString(FtoCtemp);
          } // for j

          figreport->addItem(QString::number((jacount+1))+")- "+t);
          jacount++;
        } // if
    } // for i

    int jacount2 = 0;
    figreport->addItem("Non Ultimate Branch angles for each junction");
    for (int i = 0; i < glWidget->NLeaveCount; i++)
    {
        if(glWidget->VSortedAngleJuncNotLeaves[i].size()>0)
        {
          QString t = "";
          char FtoCtemp[50];
          for( int j=0;j<glWidget->VSortedAngleJuncNotLeaves[i].size();j++)
          {
              sprintf(FtoCtemp,"%.2f ",glWidget->VSortedAngleJuncNotLeaves[i].at(j));
              t = t + QString(FtoCtemp);
          } // for j
          figreport->addItem(QString::number((jacount2+1))+")- "+t);
          jacount2++;
        } // if
    } // for i

    int rrbc=0;
    for(int i=0;i<glWidget->WRRB_count;i++)
        if(glWidget->newRB[i].val > 0) rrbc++;

    figreport->addItem("Amount of all branches : "+QString::number(rrbc));

    int rrbcf = 0;
    for(int i=0;i<glWidget->WRRB_count;i++)
    {
        char FtoCtemp[50];
        if(glWidget->newRB[i].val > 0)
        {
            sprintf(FtoCtemp,"%.2f",glWidget->newRB[i].val/glWidget->ppmm);
            figreport->addItem(QString::number((rrbcf+1))+") "
                               + QString::number(glWidget->newRB[i].p1) + " "
                               + QString::number(glWidget->newRB[i].x1) + " "
                               + QString::number(glWidget->newRB[i].y1) + " "
                               + QString::number(glWidget->newRB[i].x2) + " "
                               + QString::number(glWidget->newRB[i].y2) + " "
                               + QString::number(glWidget->newRB[i].RbN) + " "
                               + QString::number(glWidget->newRB[i].BrOrNo) + " "
                               + QString(FtoCtemp));
            rrbcf++;
        }
    } // for i show all branches

    figreport->addItem("Amount of branch order : "+QString::number(glWidget->max_Branch_Order));

    for( int i=0;i<glWidget->max_Branch_Order;i++)
    {
        figreport->addItem("Branch Order No."+QString::number((i+1)));
        figreport->addItem(" Amount of Branches in Branch Order No."+
                           QString::number((i+1))+" : "+QString::number(glWidget->BrOr_count[i]));
        char FtoCtemp[50];
        for(int j=0;j<glWidget->BrOr_count[i];j++)
        {
            sprintf(FtoCtemp,"%.2f",glWidget->branch_Order_array[i][j].val/glWidget->ppmm);
            figreport->addItem(" "+QString::number((j+1))+") "
                               + QString::number(glWidget->branch_Order_array[i][j].p1) + " "
                               + QString::number(glWidget->branch_Order_array[i][j].x1) + " "
                               + QString::number(glWidget->branch_Order_array[i][j].y1) + " "
                               + QString::number(glWidget->branch_Order_array[i][j].x2) + " "
                               + QString::number(glWidget->branch_Order_array[i][j].y2) + " "
                               + QString::number(glWidget->branch_Order_array[i][j].RbN) + " "
                               + QString(FtoCtemp));
        } // for j show branches in each branch order
    } // for i : each branch order

} // updateReport

void MainWindow::open()
{
    filenames= QFileDialog::getOpenFileName(this,
               tr("Open File"),
               curdir,
               tr("Bitmap (*.pbm);;All files (*.*)"));

    QString path(filenames);
    if (!path.isNull())
    {
      curdir = QFileInfo(filenames).path();
      QFileInfo pathInfo( path );
      Qfilenameonly = pathInfo.completeBaseName(); // without extension
      // Cfilenameonly = Qfilenameonly.toUtf8().constData(); //invalid pointer

      QString date = QDate::currentDate().toString("-dd-MMM-yyyy");
      newsavedfilename = Qfilenameonly+date;
      anasavedfilename = newsavedfilename;

      //fullname = filenames.toUtf8().constData(); //invalid pointer
      fullname = strdup(filenames.toUtf8().constData());

      glWidget->filename_dump_prt=filenames;
      glWidget->readfile(fullname);
      glWidget->firstfilename =  fullname;
      updateReport(filenames);
      updateStatReport(filenames);
      setActions(true);

     T_pictureAct->setChecked(true);
     //gridAct->setChecked(false);
     T_skeletonAct->setChecked(false);
     T_junctionAct->setChecked(false);
     T_terminalAct->setChecked(false);
     T_contourAct->setChecked(false);
     T_disc_a_Act->setChecked(false);
     T_disc_b_Act->setChecked(false);
     T_disc_c_Act->setChecked(false);
     T_rb_Act->setChecked(false);
     T_geo_Act->setChecked(false);
     T_term_Act->setChecked(false);
     T_br_Act->setChecked(false);
     T_FS_Act->setChecked(false);
     T_Tree_Act->setChecked(false);
     writeResAct->setChecked(false);
     ItAct->setChecked(false);
     TestAct->setChecked(false);
     AutoStart->setChecked(false);
     Man_AutoAct->setChecked(false);
    } // if path is not null
} // open()

void MainWindow::undo() {}

void MainWindow::redo() {}

void MainWindow::setColor()
{
    QString textColorName="";
    QColor color = QColorDialog::getColor(Qt::white, this, "Set Color",  QColorDialog::DontUseNativeDialog);
    if(color.isValid())
    {
        textColorName = color.name();
        glWidget->colorname_selected = textColorName;

        //const char *hh = textColorName.toUtf8().constData();
	//invalid pointer

        glWidget->first_time = true;
        glWidget->updateGL();
     }
}

void MainWindow::writeResToDB() {}

void MainWindow::writeResToFile()
{
    QString anapath = analysispath;
    QString anaext = ".res";
    QString anafullpath = anapath+anasavedfilename+anaext;

    QString fileName = QFileDialog::getSaveFileName(
    this,
    "Save File Dialog",
    anafullpath);

     if (fileName.isEmpty())
         return;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Analytical data Saving"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }
     //const char* filename_saved = fileName.toUtf8().constData();

     glWidget->writeAnaToFile(fileName.toUtf8().constData());
     statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
     updateReport(filenames);
     updateStatReport(filenames);
}

void MainWindow::showReport(const QString &reporttxt)
{
    updateReport(filenames);
}

void MainWindow::showAnalysis(const QString &analysistxt)
{
    updateStatReport(filenames);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About 2D Morphometric Software"),
            tr("The <b>software</b> analyzes 2D image of branching growth form"));
}

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(pngpath+"open.ico"), tr("&Open Image..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open image file (.pbm)"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(pngpath+"saveimg.ico"), tr("&Save Image..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current image"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(pngpath+"print.ico"), tr("&Print Image..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current image"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(imagePrint()));

    printPreviewAct = new QAction(QIcon(pngpath+"print_preview.ico"), tr("&Print Preview"), this);
    printPreviewAct->setShortcuts(QKeySequence::Print);
    printPreviewAct->setStatusTip(tr("Preview the current image before printing"));
    connect(printPreviewAct, SIGNAL(triggered()), this, SLOT(imagePrintPreview()));

    zoominAct = new QAction(QIcon(pngpath+"zoomin.png"), tr("&Zoom In"), this);
    zoominAct->setStatusTip(tr("Zoom In"));
    connect(zoominAct, SIGNAL(triggered()), glWidget, SLOT(zoomin()));

    zoomoutAct = new QAction(QIcon(pngpath+"zoomout.png"), tr("&Zoom Out"), this);
    zoomoutAct->setStatusTip(tr("Zoom Out"));
    connect(zoomoutAct, SIGNAL(triggered()), glWidget, SLOT(zoomout()));

    undoAct = new QAction(QIcon(pngpath+"undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(pngpath+"redo.png"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last editing action"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    colorAct = new QAction(QIcon(pngpath+"pickcolor.ico"), tr("&Set Image Color"), this);
    colorAct->setStatusTip(tr("Set color"));
    connect(colorAct, SIGNAL(triggered()), this, SLOT(setColor()));

    gridAct = new QAction(QIcon(pngpath+"showgrid.png"), tr("&Show Grid"), this);
    gridAct->setStatusTip(tr("Show/Hide Grid"));
    gridAct->setCheckable(true);
    gridAct->setChecked(false);
    connect(gridAct, SIGNAL(triggered()), glWidget, SLOT(showGrid()));

    exportAct = new QAction(QIcon(pngpath+"showExport.png"), tr("&Export"), this);
    exportAct->setStatusTip(tr("Export"));
    connect(exportAct, SIGNAL(triggered()), glWidget, SLOT(showExport()));

    exportSkelAct = new QAction(QIcon(pngpath+"showexportSkel.png"), tr("&Skeleton"), this);
    exportSkelAct->setStatusTip(tr("Export Skeleton"));
    connect(exportSkelAct, SIGNAL(triggered()), glWidget, SLOT(showexportSkel()));

    exportContourAct = new QAction(QIcon(pngpath+"showexportContour.png"), tr("&Contour"), this);
    exportContourAct->setStatusTip(tr("Export Contour"));
    connect(exportContourAct, SIGNAL(triggered()), glWidget, SLOT(showexportContour()));

    scaleAct = new QAction(QIcon(pngpath+"showscale.ico"), tr("&Unit Settings"), this);
    scaleAct->setStatusTip(tr("Set Unit"));
    connect(scaleAct, SIGNAL(triggered()), glWidget, SLOT(showScale()));

    scalenmAct = new QAction(QIcon(pngpath+"showscalemm.png"), tr("&Nanometer"), this);
    scalenmAct->setStatusTip(tr("Nanometer"));
    scalenmAct->setCheckable(true);
    scalenmAct->setChecked(false);
//    scalenmAct->setVisible(false);
    connect(scalenmAct, SIGNAL(triggered()), glWidget, SLOT(showScalenm()));

    scaleumAct = new QAction(QIcon(pngpath+"showscalemm.png"), tr("&Micrometer"), this);
    scaleumAct->setStatusTip(tr("Micrometer"));
    scaleumAct->setCheckable(true);
    scaleumAct->setChecked(false);
//    scaleumAct->setVisible(false);
    connect(scaleumAct, SIGNAL(triggered()), glWidget, SLOT(showScaleum()));

    scalemmAct = new QAction(QIcon(pngpath+"showscalemm.png"), tr("&Millimeter"), this);
    scalemmAct->setStatusTip(tr("Millimeter"));
    scalemmAct->setCheckable(true);
    scalemmAct->setChecked(true);
    connect(scalemmAct, SIGNAL(triggered()), glWidget, SLOT(showScalemm()));

    scalecmAct = new QAction(QIcon(pngpath+"showscalecm.png"), tr("&Centimeter"), this);
    scalecmAct->setStatusTip(tr("Centimeter"));
    scalecmAct->setCheckable(true);
    scalecmAct->setChecked(false);
    connect(scalecmAct, SIGNAL(triggered()), glWidget, SLOT(showScalecm()));

    scaleinchAct = new QAction(QIcon(pngpath+"showscaleinch.png"), tr("&Inch"), this);
    scaleinchAct->setStatusTip(tr("Inch"));
    scaleinchAct->setCheckable(true);
    scaleinchAct->setChecked(false);
    connect(scaleinchAct, SIGNAL(triggered()), glWidget, SLOT(showScaleinch()));

    figurelengthAct = new QAction(QIcon(pngpath+"showobjectlength.png"), tr("&Show/Hide Object Length"), this);
    figurelengthAct->setStatusTip(tr("Object Length"));
    figurelengthAct->setCheckable(true);
    figurelengthAct->setChecked(false);
    connect(figurelengthAct, SIGNAL(triggered()), glWidget, SLOT(showfigureLength()));

    pointupAct= new QAction(QIcon(pngpath+"pointup.png"), tr("&Increase Point Size"), this);
    pointupAct->setStatusTip(tr("Increase Point Size"));
    connect(pointupAct, SIGNAL(triggered()), glWidget, SLOT(setPointUp()));
    connect(pointupAct, SIGNAL(triggered()), this, SLOT(setPointUp()));
    pointupAct->setVisible(true);

    pointdownAct= new QAction(QIcon(pngpath+"pointdown.png"), tr("&Decrease Point Size"), this);
    pointdownAct->setStatusTip(tr("Decrease Point Size"));
    connect(pointdownAct, SIGNAL(triggered()), glWidget, SLOT(setPointDown()));
    connect(pointdownAct, SIGNAL(triggered()), this, SLOT(setPointDown()));
    pointdownAct->setVisible(true);

    screenDiagonalAct= new QAction(QIcon(pngpath+"setScreenDiagonal.png"), tr("&Set Screen Diagonal"), this);
    screenDiagonalAct->setStatusTip(tr("Set Screen Diagonal"));
    connect(screenDiagonalAct, SIGNAL(triggered()), this, SLOT(setScreenDiagonal()));

    setObjectScale = new QAction(QIcon(pngpath+"setobjectscale.png"), tr("&Set Object Scale"), this);
    setObjectScale->setStatusTip(tr("Set Object Scale"));
    connect(setObjectScale, SIGNAL(triggered()), glWidget, SLOT(setObjScale()));

    cleanSettingAct = new QAction(QIcon(pngpath+"cleansetting.ico"), tr("&Clean Settings"), this);
    cleanSettingAct->setStatusTip(tr("Set threshold for deletion"));
    connect(cleanSettingAct, SIGNAL(triggered()), glWidget, SLOT(cleanSetting()));

    XYPosSettingAct = new QAction(QIcon(pngpath+"xypossetting.ico"), tr("&Display Settings"), this);
    XYPosSettingAct->setStatusTip(tr("Set Position of The Begining of the image"));
    connect(XYPosSettingAct, SIGNAL(triggered()), glWidget, SLOT(XYPosSetting()));

    setCurrentDir = new QAction(QIcon(pngpath+"setcurrentdir.ico"), tr("&File and Directory Name Settings"), this);
    setCurrentDir->setStatusTip(tr("Set Current Image and Analyzed Output Directory"));
    connect(setCurrentDir, SIGNAL(triggered()), this, SLOT(setCurrentDirectory()));

    showObjectScale = new QAction(QIcon(pngpath+"setobjectscale.png"), tr("&Show Object Scale"), this);
    showObjectScale->setStatusTip(tr("Show Object Scale"));
    connect(showObjectScale, SIGNAL(triggered()), glWidget, SLOT(showObjectScale()));

    showJunctionLabelAct = new QAction(QIcon(pngpath+"showjunctionlabel.png"), tr("&Show/Hide Junction Label"), this);
    showJunctionLabelAct->setStatusTip(tr("Show Junction Label"));
    showJunctionLabelAct->setCheckable(true);
    showJunctionLabelAct->setChecked(false);
    connect(showJunctionLabelAct, SIGNAL(triggered()), glWidget, SLOT(showJunctionLabel()));

    showJunctionCrossAct = new QAction(QIcon(pngpath+"showjunctioncross.png"), tr("&Show/Hide Junction Cross"), this);
    showJunctionCrossAct->setStatusTip(tr("Show Junction Cross"));
    showJunctionCrossAct->setCheckable(true);
    showJunctionCrossAct->setChecked(false);
    connect(showJunctionCrossAct, SIGNAL(triggered()), glWidget, SLOT(showJunctionCross()));

    fileToolbarAct = new QAction(QIcon(pngpath+"showfiletoolbar.png"), tr("&File Toolbar"), this);
    fileToolbarAct->setStatusTip(tr("File Toolbar"));
    fileToolbarAct->setCheckable(true);
    fileToolbarAct->setChecked(true);
    connect(fileToolbarAct, SIGNAL(triggered()), this, SLOT(showFileToolbar()));

    editToolbarAct = new QAction(QIcon(pngpath+"showfiletoolbar.png"), tr("&Edit Toolbar"), this);
    editToolbarAct->setStatusTip(tr("Edit Toolbar"));
    editToolbarAct->setCheckable(true);
    editToolbarAct->setChecked(true);
    connect(editToolbarAct, SIGNAL(triggered()), this, SLOT(showEditToolbar()));

    displayToolbarAct = new QAction(QIcon(pngpath+"showfiletoolbar.png"), tr("&Display Toolbar"), this);
    displayToolbarAct->setStatusTip(tr("Display Toolbar"));
    displayToolbarAct->setCheckable(true);
    displayToolbarAct->setChecked(true);
    connect(displayToolbarAct, SIGNAL(triggered()), this, SLOT(showDisplayToolbar()));

    processToolbarAct = new QAction(QIcon(pngpath+"showfiletoolbar.png"), tr("&Process Toolbar"), this);
    processToolbarAct->setStatusTip(tr("Process Toolbar"));
    processToolbarAct->setCheckable(true);
    processToolbarAct->setChecked(true);
    connect(processToolbarAct, SIGNAL(triggered()), this, SLOT(showProcessToolbar()));


    quitAct = new QAction(QIcon(pngpath+"quit.ico"),tr("&Quit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About the project"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // display menu
    T_pictureAct = new QAction(QIcon(pngpath+"pict.png"), tr("&Picture"), this);
    T_pictureAct->setStatusTip(tr("Show/Hide picture"));
    T_pictureAct->setCheckable(true);
    T_pictureAct->setChecked(true);
    connect(T_pictureAct, SIGNAL(triggered()), glWidget, SLOT(showPicture()));

    T_skeletonAct = new QAction(QIcon(pngpath+"skel.png"), tr("&Skeleton"), this);
    T_skeletonAct->setStatusTip(tr("Show/Hide skeleton"));
    T_skeletonAct->setCheckable(true);
    T_skeletonAct->setChecked(false);
    connect(T_skeletonAct, SIGNAL(triggered()), glWidget, SLOT(showSkeleton()));

    T_junctionAct = new QAction(QIcon(pngpath+"junc.png"), tr("&Junction"), this);
    T_junctionAct->setStatusTip(tr("Show/Hide junction"));
    T_junctionAct->setCheckable(true);
    T_junctionAct->setChecked(false);
    connect(T_junctionAct, SIGNAL(triggered()), glWidget, SLOT(showJunction()));

    T_terminalAct = new QAction(QIcon(pngpath+"term.png"), tr("&Terminal"), this);
    T_terminalAct->setStatusTip(tr("Show/Hide terminal"));
    T_terminalAct->setCheckable(true);
    T_terminalAct->setChecked(false);
    connect(T_terminalAct, SIGNAL(triggered()), glWidget, SLOT(showTerminal()));

    T_contourAct = new QAction(QIcon(pngpath+"cont.png"), tr("&Contour"), this);
    T_contourAct->setStatusTip(tr("Show/Hide contour"));
    T_contourAct->setCheckable(true);
    T_contourAct->setChecked(false);
    connect(T_contourAct, SIGNAL(triggered()), glWidget, SLOT(showContour()));

    T_disc_a_Act = new QAction(QIcon(pngpath+"da.png"), tr("&Disc A"), this);
    T_disc_a_Act->setStatusTip(tr("Show/Hide disc A"));
    T_disc_a_Act->setCheckable(true);
    T_disc_a_Act->setChecked(false);
    connect(T_disc_a_Act, SIGNAL(triggered()), glWidget, SLOT(showDiscA()));

    T_disc_b_Act = new QAction(QIcon(pngpath+"db.png"), tr("&Disc B"), this);
    T_disc_b_Act->setStatusTip(tr("Show/Hide disc B"));
    T_disc_b_Act->setCheckable(true);
    T_disc_b_Act->setChecked(false);
    connect(T_disc_b_Act, SIGNAL(triggered()), glWidget, SLOT(showDiscB()));

    T_disc_c_Act = new QAction(QIcon(pngpath+"dc.png"), tr("&Disc C"), this);
    T_disc_c_Act->setStatusTip(tr("Show/Hide disc C"));
    T_disc_c_Act->setCheckable(true);
    T_disc_c_Act->setChecked(false);
    connect(T_disc_c_Act, SIGNAL(triggered()), glWidget, SLOT(showDiscC()));

    T_rb_Act = new QAction(QIcon(pngpath+"bl.png"), tr("&Branch Length"), this);
    T_rb_Act->setStatusTip(tr("Show/Hide Branch Length"));
    T_rb_Act->setCheckable(true);
    T_rb_Act->setChecked(false);
    connect(T_rb_Act, SIGNAL(triggered()), glWidget, SLOT(showRb()));

    T_geo_Act = new QAction(QIcon(pngpath+"geo.png"), tr("&Geo Angle"), this);
    T_geo_Act->setStatusTip(tr("Show/Hide Geo"));
    T_geo_Act->setCheckable(true);
    T_geo_Act->setChecked(false);
    connect(T_geo_Act, SIGNAL(triggered()), glWidget, SLOT(showGeo()));

    T_term_Act = new QAction(QIcon(pngpath+"term.png"), tr("&Term Angle"), this);
    T_term_Act->setStatusTip(tr("Show/Hide Term"));
    T_term_Act->setCheckable(true);
    T_term_Act->setChecked(false);
    connect(T_term_Act, SIGNAL(triggered()), glWidget, SLOT(showTerm()));

    T_br_Act = new QAction(QIcon(pngpath+"br.png"), tr("&Branch Spacing"), this);
    T_br_Act->setStatusTip(tr("Show/Hide Branch Spacing"));
    T_br_Act->setCheckable(true);
    T_br_Act->setChecked(false);
    connect(T_br_Act, SIGNAL(triggered()), glWidget, SLOT(showBr()));

    T_FS_Act = new QAction(QIcon(pngpath+"st.png"), tr("&Skeleton Track"), this);
    T_FS_Act->setStatusTip(tr("Show/Hide Skeleton Track"));
    T_FS_Act->setCheckable(true);
    T_FS_Act->setChecked(false);
    connect(T_FS_Act, SIGNAL(triggered()), glWidget, SLOT(showFS()));

    T_Tree_Act = new QAction(QIcon(pngpath+"tree.png"), tr("&Tree"), this);
    T_Tree_Act->setStatusTip(tr("Show/Hide Tree"));
    T_Tree_Act->setCheckable(true);
    T_Tree_Act->setChecked(false);
    connect(T_Tree_Act, SIGNAL(triggered()), glWidget, SLOT(showTree()));

    writeResAct = new QAction(QIcon(pngpath+"writetofile.ico"), tr("&Save Result To File..."), this);
    writeResAct->setStatusTip(tr("Write result to a file"));
    connect(writeResAct, SIGNAL(triggered()), this, SLOT(writeResToFile()));

    writeToDB = new QAction(QIcon(pngpath+"writetodb.png"), tr("&Save Result To DB..."), this);
    writeToDB->setStatusTip(tr("Write result to DB"));
    connect(writeToDB, SIGNAL(triggered()), this, SLOT(writeResToDB()));
    writeToDB->setVisible(false);

    ItAct = new QAction(QIcon(pngpath+"editmode.png"), tr("&Edit Mode"), this);
    ItAct->setStatusTip(tr("Edit Mode"));
    ItAct->setCheckable(true);
    ItAct->setChecked(false);
    connect(ItAct, SIGNAL(triggered()), glWidget, SLOT(InteractionAct()));

    TestAct = new QAction(QIcon(pngpath+"manual.ico"), tr("&Manual Process"), this);
    TestAct->setStatusTip(tr("Manual Process"));
    TestAct->setCheckable(true);
    TestAct->setChecked(false);
    connect(TestAct, SIGNAL(triggered()), glWidget, SLOT(forTest_slot()));
    connect(TestAct, SIGNAL(triggered()), this, SLOT(ResetManual()));

    Man_AutoAct = new QAction(QIcon(pngpath+"detectjunction.png"), tr("&Detect/Select Junctions"), this);
    Man_AutoAct->setStatusTip(tr("Detect/Select Junctions"));
    Man_AutoAct->setCheckable(true);
    Man_AutoAct->setChecked(false);
    connect(Man_AutoAct, SIGNAL(triggered()), glWidget, SLOT(Man_Start()));

    AutoStart = new QAction(QIcon(pngpath+"autostart.ico"), tr("&Automatic process"), this);
    AutoStart->setStatusTip(tr("Start processing automatically"));
    AutoStart->setCheckable(true);
    AutoStart->setChecked(false);
    connect(AutoStart, SIGNAL(triggered()), glWidget, SLOT(autostart()));
    connect(AutoStart, SIGNAL(triggered()), this, SLOT(ResetStart()));

    TreeConstruction = new QAction(QIcon(pngpath+"treeconstruction.ico"), tr("&Tree Construction"), this);
    TreeConstruction->setStatusTip(tr("Tree Construction"));
    TreeConstruction->setCheckable(true);
    TreeConstruction->setChecked(false);
    connect(TreeConstruction, SIGNAL(triggered()), glWidget, SLOT(TreeStart()));
    connect(TreeConstruction, SIGNAL(triggered()), this, SLOT(ResetTreeStart()));

    CleanUltimateBrAct = new QAction(QIcon(pngpath+"cleanultimatebr.ico"), tr("&Clean Ultimate Branches"), this);
    CleanUltimateBrAct->setStatusTip(tr("Clean Ultimate Branch"));
    connect(CleanUltimateBrAct, SIGNAL(triggered()), glWidget, SLOT(CleanUltimateBranchAction()));
    connect(CleanUltimateBrAct, SIGNAL(triggered()), this, SLOT(CleanUltimateBranchActionUpdate()));

    CleanTerminalAct = new QAction(QIcon(pngpath+"cleanterminal.png"), tr("&Clean Terminals"), this);
    CleanTerminalAct->setStatusTip(tr("Clean Terminal"));
    connect(CleanTerminalAct, SIGNAL(triggered()), glWidget, SLOT(CleanTerminalAction()));
    connect(CleanTerminalAct, SIGNAL(triggered()), this, SLOT(CleanTerminalActionUpdate()));

    RepairBranchAct = new QAction(QIcon(pngpath+"repairbranch.ico"), tr("&Repair Branches"), this);
    RepairBranchAct->setStatusTip(tr("Repair/Delete Redundant Branches"));
    connect(RepairBranchAct, SIGNAL(triggered()), glWidget, SLOT(RepairBranchAction()));
    connect(RepairBranchAct, SIGNAL(triggered()), this, SLOT(RepairBranchActionUpdate()));

    SetRootAction = new QAction(QIcon(pngpath+"selectroot.png"), tr("&Set Root"), this);
    SetRootAction->setStatusTip(tr("Set Root"));
    connect(SetRootAction, SIGNAL(triggered()), glWidget, SLOT(setNewRoot()));
}

void MainWindow::setActions(bool b)
{
    saveAct->setEnabled(b);

    exportAct->setEnabled(b);
//    exportGroup->setEnabled(b);
    exportSkelAct->setEnabled(b);
    exportContourAct->setEnabled(b);

    printAct->setEnabled(b);
    printPreviewAct->setEnabled(b);
    undoAct->setEnabled(b);
    redoAct->setEnabled(b);
    zoominAct->setEnabled(b);
    zoomoutAct->setEnabled(b);
    colorAct->setEnabled(b);

    gridAct->setEnabled(b);
    scaleAct->setEnabled(b);
    scaleGroup->setEnabled(b);
    scalenmAct->setEnabled(b);
    scaleumAct->setEnabled(b);
    scalemmAct->setEnabled(b);
    scalecmAct->setEnabled(b);
    scaleinchAct->setEnabled(b);
    figurelengthAct->setEnabled(b);
    showJunctionLabelAct->setEnabled(b);
    showJunctionCrossAct->setEnabled(b);
    pointupAct->setEnabled(b);
    pointdownAct->setEnabled(b);
    //setObjectScale->setEnabled(b);

    T_pictureAct->setEnabled(b);
    T_skeletonAct->setEnabled(b);
    T_junctionAct->setEnabled(b);
    T_terminalAct->setEnabled(b);
    T_contourAct->setEnabled(b);
    T_disc_a_Act->setEnabled(b);
    T_disc_b_Act->setEnabled(b);
    T_disc_c_Act->setEnabled(b);
    T_rb_Act->setEnabled(b);
    T_geo_Act->setEnabled(b);
    T_term_Act->setEnabled(b);
    T_br_Act->setEnabled(b);
    T_FS_Act->setEnabled(b);
    T_Tree_Act->setEnabled(b);

    writeResAct->setEnabled(b);
    writeToDB->setEnabled(b);
    ItAct->setEnabled(b);

    TestAct->setEnabled(b);
    AutoStart->setEnabled(b);
    Man_AutoAct->setEnabled(b);
    TreeConstruction->setEnabled(b);
    CleanUltimateBrAct->setEnabled(b);
    CleanTerminalAct->setEnabled(b);
    RepairBranchAct->setEnabled(b);
    SetRootAction->setEnabled(b);

} // setActions

void MainWindow::setPointDown()
{
    if(glWidget->pointsize > glWidget->MinPointSize)
        pointdownAct->setEnabled(true);
    else
        pointdownAct->setEnabled(false);

    if(glWidget->pointsize < glWidget->MaxPointSize)
        pointupAct->setEnabled(true);
    else
        pointupAct->setEnabled(false);
} // setPointDown

void MainWindow::setPointUp()
{
    if(glWidget->pointsize < glWidget->MaxPointSize)
        pointupAct->setEnabled(true);
    else
        pointupAct->setEnabled(false);

    if(glWidget->pointsize > glWidget->MinPointSize)
        pointdownAct->setEnabled(true);
    else
        pointdownAct->setEnabled(false);
} // setPointUp

void MainWindow::ResetManual()
{
    if(glWidget->process_finish)
    {
        TestAct->setChecked(false);
        updateReport(filenames);
        updateStatReport(filenames);
    }
    else
    {
        TestAct->setChecked(true);
    }
} // ResetManual

void MainWindow::ResetStart()
{
    if(glWidget->process_finish)
    {
        AutoStart->setChecked(false);
        updateReport(filenames);
        updateStatReport(filenames);
    }
    else
    {
        AutoStart->setChecked(true);
    }
} // ResetStart

void MainWindow::ResetTreeStart()
{
    if(glWidget->Tree_process_finish)
    {
        TreeConstruction->setChecked(false);
        updateReport(filenames);
        updateStatReport(filenames);
    }
    else
    {
        TreeConstruction->setChecked(true);
    }
} // ResetTreeStart

void MainWindow::CleanUltimateBranchActionUpdate()
{
    updateReport(filenames);
    updateStatReport(filenames);
}

void MainWindow::CleanTerminalActionUpdate()
{
    updateReport(filenames);
    updateStatReport(filenames);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(writeResAct);
    fileMenu->addAction(writeToDB);
    fileMenu->addAction(saveAct);

//    exportMenu = fileMenu->addMenu(QIcon(pngpath+"showExport.png"),tr("&Export"));
//    exportGroup = new QActionGroup(exportAct);
//    exportGroup->addAction(exportSkelAct);
//    exportGroup->addAction(exportContourAct);
//    exportMenu->addAction(exportSkelAct);
//    exportMenu->addAction(exportContourAct);

    fileMenu->addAction(printAct);
    fileMenu->addAction(printPreviewAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    scaleMenu = editMenu->addMenu(QIcon(pngpath+"showscale.ico"),tr("&Unit Settings"));
    scaleGroup = new QActionGroup(scaleAct);
    scaleGroup->addAction(scalenmAct);
    scaleGroup->addAction(scaleumAct);
    scaleGroup->addAction(scalemmAct);
    scaleGroup->addAction(scalecmAct);
    scaleGroup->addAction(scaleinchAct);
    scalemmAct->setChecked(true);

    scaleMenu->addAction(scalemmAct);
    scaleMenu->addAction(scalecmAct);
    scaleMenu->addAction(scaleinchAct);

    editMenu->addAction(setObjectScale);
    editMenu->addAction(setCurrentDir);
    editMenu->addAction(cleanSettingAct);
    editMenu->addAction(XYPosSettingAct);

    displayMenu = menuBar()->addMenu(tr("&Display"));
    displayMenu->addAction(T_pictureAct);
    displayMenu->addAction(T_skeletonAct);
    displayMenu->addAction(T_junctionAct);
    displayMenu->addAction(T_terminalAct);
    displayMenu->addAction(T_contourAct);
    displayMenu->addAction(T_disc_a_Act);
    displayMenu->addAction(T_disc_b_Act);
    displayMenu->addAction(T_disc_c_Act);
    displayMenu->addAction(T_rb_Act);
    displayMenu->addAction(T_FS_Act);
    displayMenu->addAction(T_br_Act);
    displayMenu->addAction(T_Tree_Act);

    analyzeMenu = menuBar()->addMenu(tr("&Process"));
    analyzeMenu->addAction(ItAct);
    analyzeMenu->addAction(TestAct);
    analyzeMenu->addAction(AutoStart);
    analyzeMenu->addAction(Man_AutoAct);
    analyzeMenu->addAction(TreeConstruction);
    analyzeMenu->addAction(CleanUltimateBrAct);
    analyzeMenu->addAction(CleanTerminalAct);
    analyzeMenu->addAction(RepairBranchAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    toolbarMenu = viewMenu->addMenu(QIcon(pngpath+"showtoolbar.ico"),tr("&Show Toolbar"));
    toolbarMenu->addAction(fileToolbarAct);
    toolbarMenu->addAction(editToolbarAct);
    toolbarMenu->addAction(displayToolbarAct);
    toolbarMenu->addAction(processToolbarAct);

    viewMenu->addAction(gridAct);
    viewMenu->addAction(showJunctionLabelAct);
    viewMenu->addAction(showJunctionCrossAct);
    viewMenu->addAction(figurelengthAct);
    viewMenu->addAction(showObjectScale);


    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(writeResAct);
    fileToolBar->addAction(writeToDB);
    fileToolBar->addAction(printAct);
    fileToolBar->addAction(printPreviewAct);
    fileToolBar->addSeparator();

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(gridAct);
//    editToolBar->addAction(scalemmAct);
//    editToolBar->addAction(scalecmAct);
//    editToolBar->addAction(scaleinchAct);
    editToolBar->addAction(setObjectScale);
//    editToolBar->addAction(pointupAct);
//    editToolBar->addAction(pointdownAct);
    editToolBar->addAction(figurelengthAct);
    editToolBar->addAction(showJunctionLabelAct);
    editToolBar->addAction(showJunctionCrossAct);

    editToolBar->addSeparator();

    displayToolBar = addToolBar(tr("Display"));

    displayToolBar->addAction(T_pictureAct);
    displayToolBar->addAction(T_skeletonAct);
    displayToolBar->addAction(T_junctionAct);
    displayToolBar->addAction(T_terminalAct);
    displayToolBar->addAction(T_contourAct);
    displayToolBar->addAction(T_disc_a_Act);
    displayToolBar->addAction(T_disc_b_Act);
    displayToolBar->addAction(T_disc_c_Act);
    displayToolBar->addAction(T_rb_Act);
    displayToolBar->addAction(T_br_Act);
    displayToolBar->addAction(T_FS_Act);
    displayToolBar->addAction(T_Tree_Act);
    displayToolBar->addSeparator();

    processToolBar = addToolBar(tr("Process"));
    processToolBar->addAction(Man_AutoAct);
    processToolBar->addAction(AutoStart);
    processToolBar->addAction(ItAct);
    processToolBar->addAction(TestAct);
    processToolBar->addAction(TreeConstruction);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Show Raw Data"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setVisible(false);
    dock->setStyleSheet("width:100px;color: red; background-color: white; selection-yellow: red;selection-background-color: green;background-image: url("+pngpath+"background1.jpg)");

    figreport = new QListWidget(dock);
    figreport->addItems(QStringList()
            << "No Image");
    dock->setWidget(figreport);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
    connect(figreport, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(showReport(const QString &)));


    dock = new QDockWidget(tr("Show Statistical Data"), this);
    dock->setVisible(false);
    dock->setStyleSheet("width:100px;color: blue; background-color: white; selection-color: red;selection-background-color: green;background-image: url("+pngpath+"background1.jpg)");
    figanalysis = new QListWidget(dock);
    figanalysis->addItems(QStringList()
            << "No Image");
    dock->setWidget(figanalysis);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    connect(figanalysis, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(showAnalysis(const QString &)));

 } // createDockWindows

void MainWindow::showFileToolbar()
{
    if(fileToolbarAct->isChecked())
       fileToolBar->setVisible(true);
    else
       fileToolBar->setVisible(false);
}

void MainWindow::showEditToolbar()
{
    if(editToolbarAct->isChecked())
       editToolBar->setVisible(true);
    else
       editToolBar->setVisible(false);
}

void MainWindow::showDisplayToolbar()
{
    if(displayToolbarAct->isChecked())
       displayToolBar->setVisible(true);
    else
       displayToolBar->setVisible(false);
}

void MainWindow::showProcessToolbar()
{
    if(processToolbarAct->isChecked())
       processToolBar->setVisible(true);
    else
       processToolBar->setVisible(false);
}

void MainWindow::setCurrentDirectory()
{
    txtInputDlg = new QDialog();
    txtInputDlg->setFixedSize(500, 120);

    QLabel* dLblLabel = new QLabel(txtInputDlg);
    dLblLabel->setText("Image Files Directory");
    dLblLabel->setGeometry(20, 5, 180, 20);

    ImgEdtInputStr = new QLineEdit(txtInputDlg);
    ImgEdtInputStr->setGeometry(200, 5, 200, 20);
    ImgEdtInputStr->setText(imagepath);

    QPushButton* imgbrowse = new QPushButton(txtInputDlg);
    imgbrowse->setText("Browse");
    imgbrowse->setGeometry(405, 5, 50, 20);

    QLabel* uLblLabel = new QLabel(txtInputDlg);
    uLblLabel->setText("Analyzed Output Directory");
    uLblLabel->setGeometry(20, 30, 200, 20);

    AnaEdtInputStr = new QLineEdit(txtInputDlg);
    AnaEdtInputStr->setGeometry(200, 30, 200, 20);
    AnaEdtInputStr->setText(analysispath);

    QLabel* csvLbl = new QLabel(txtInputDlg);
    csvLbl->setText("CSV File Name");
    csvLbl->setGeometry(20, 55, 200, 20);

    csvfilename = new QLineEdit(txtInputDlg);
    csvfilename->setGeometry(200, 55, 200, 20);
    csvfilename->setText(glWidget->CSVFilename);

    QPushButton* anabrowse = new QPushButton(txtInputDlg);
    anabrowse->setText("Browse");
    anabrowse->setGeometry(405, 30, 50, 20);

    QPushButton* okbutton = new QPushButton(txtInputDlg);
    okbutton->setText("OK");
    okbutton->setGeometry(120, 90, 80, 20);

    QPushButton* cancelbutton = new QPushButton(txtInputDlg);
    cancelbutton->setText("Cancel");
    cancelbutton->setGeometry(210, 90, 150, 20);

    txtInputDlg->show();

    connect( imgbrowse, SIGNAL(clicked()), this, SLOT(DimgbrowseClicked()) );
    connect( anabrowse, SIGNAL(clicked()), this, SLOT(DanabrowseClicked()) );
    connect( okbutton, SIGNAL(clicked()), this, SLOT(DokClicked()) );
    connect( cancelbutton, SIGNAL(clicked()), this, SLOT(DcancelClicked()) );
}

void MainWindow::DimgbrowseClicked()
{
    QString imgdirname = QFileDialog::getExistingDirectory(this, tr("Choose Image Working Directory"),imagepath, QFileDialog::ShowDirsOnly);
    if (!imgdirname.isNull())
    {
        curdir = imgdirname+"/";
      ImgEdtInputStr->setText(curdir);
    }
    txtInputDlg->activateWindow();
}

void MainWindow::DanabrowseClicked()
{
    QString imgdirname = QFileDialog::getExistingDirectory(this, tr("Choose Analyzed Output Working Directory"),analysispath, QFileDialog::ShowDirsOnly);
    if (!imgdirname.isNull())
    {
        anacurdir = imgdirname+"/";
      AnaEdtInputStr->setText(anacurdir);
    }
    txtInputDlg->activateWindow();
}

void MainWindow::DokClicked()
{
    imagepath = curdir;
    analysispath = anacurdir;
    glWidget->CSVFilename = csvfilename->text();
    txtInputDlg->close();
}

void MainWindow::DcancelClicked()
{
    txtInputDlg->close();
}

void MainWindow::RepairBranchActionUpdate()
{
    updateReport(filenames);
    updateStatReport(filenames);
}



