#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fratype.h"


QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QActionGroup;

class QLabel;
class QScrollArea;
class QSlider;

class GLWidget;
class QProgressBar;

class QDialog;
class QLineEdit;

QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

        QWidget *centralWidget;
        QScrollArea *glWidgetArea;
        GLWidget *glWidget;
        GLWidget *glRead;

        MainWindow();
        void updateReport(QString filename_input);
        void updateStatReport(QString filename_input);

        const char* fullname;
        const char* Cfilenameonly;
        QString Qfilenameonly;
        QString filenames;
        QString newsavedfilename;
        QString anasavedfilename;

        QString curdir;
        QString anacurdir;
        QString pngpath;
        QString imagepath;
        QString analysispath;
        QString capturedpath;

        QDialog* txtInputDlg;
        QLineEdit* ImgEdtInputStr;
        QLineEdit* AnaEdtInputStr;
        QLineEdit* csvfilename;


        void setActions(bool b);

private slots:
    void save();
    void print(QPrinter *printer);
    void printpreview(QPrinter* printer);
    void imagePrintPreview();
    void imagePrint();
    void undo();
    void redo();
    void setColor();
    void about();
    void showReport(const QString &reporttxt);
    void showAnalysis(const QString &analysistxt);
    void open();
    void writeResToFile();
    void writeResToDB();
    void ResetStart();
    void ResetManual();
    void ResetTreeStart();
    void CleanUltimateBranchActionUpdate();
    void CleanTerminalActionUpdate();
    void RepairBranchActionUpdate();

    void showFileToolbar();
    void showEditToolbar();
    void showDisplayToolbar();
    void showProcessToolbar();

    void setPointDown();
    void setPointUp();
    void setScreenDiagonal();
    void setCurrentDirectory();
    void DimgbrowseClicked();
    void DanabrowseClicked();
    void DokClicked();
    void DcancelClicked();


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    QTextEdit *textEdit;
    QListWidget *figreport;
    QListWidget *figanalysis;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *displayMenu;
    QMenu *analyzeMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *scaleMenu;
    QMenu *exportMenu;
    QMenu *toolbarMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *displayToolBar;
    QToolBar *processToolBar;

    QActionGroup *scaleGroup;
    QActionGroup *exportGroup;

    QAction *saveAct;
    QAction *exportAct;
    QAction *exportSkelAct;
    QAction *exportContourAct;

    QAction *printAct;
    QAction *printPreviewAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *zoominAct;
    QAction *zoomoutAct;
    QAction *colorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *openAct;
    QAction *gridAct;
    QAction *scaleAct;

    QAction *scalenmAct;
    QAction *scaleumAct;
    QAction *scalemmAct;
    QAction *scalecmAct;
    QAction *scaleinchAct;

    QAction *figurelengthAct;
    QAction *pointupAct;
    QAction *pointdownAct;
    QAction *screenDiagonalAct; // Set diagonal of computer monitor
    QAction *setObjectScale;  // Set scale of real object
    QAction *cleanSettingAct; // set mininum ultimate branch length and dc radius for deletion
    QAction *XYPosSettingAct; // set begining of XY Position for displaying images
    QAction *showObjectScale;
    QAction *showJunctionLabelAct; // Toggle menu to show/hide junction label
    QAction *showJunctionCrossAct; // Toggle menu to show/hide junction cross
    QAction *setCurrentDir;  // Set Current Directory for Images and analyzed output files

    QAction *fileToolbarAct;
    QAction *editToolbarAct;
    QAction *displayToolbarAct;
    QAction *processToolbarAct;

    QAction *T_pictureAct;
    QAction *T_skeletonAct;
    QAction *T_junctionAct;
    QAction *T_terminalAct;
    QAction *T_contourAct;
    QAction *T_disc_a_Act;
    QAction *T_disc_b_Act;
    QAction *T_disc_c_Act;
    QAction *T_rb_Act;
    QAction *T_geo_Act;
    QAction *T_term_Act;
    QAction *T_br_Act;
    QAction *T_FS_Act;
    QAction *T_Tree_Act;


    QAction *writeResAct;
    QAction *writeToDB;
    QAction *ItAct;

    QAction *TestAct;
    QAction *AutoStart;
    QAction *Man_AutoAct;
    QAction *TreeConstruction;
    QAction *CleanUltimateBrAct;
    QAction *CleanTerminalAct;
    QAction *RepairBranchAct;
    QAction *SetRootAction;
};

#endif
