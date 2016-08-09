/**
  *   赛题：第五届“中国软件杯”大学生软件设计大赛——针对以经纬度或经纬度带时间定义的不同轨迹
  *   软件名称：基于“弗雷歇距离”的轨迹相似度分析软件
  *   开发团队：Kryptonite
  *   开发时间：2016年4至6月
  *   文件功能概述：主页界面，包含数据的导入导出，轨迹预览，详情查看等功能;主要ui都在此处理，分发
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include "header.h"
#include "omp.h"
#include "csv.h"
#include "database.h"
#include "mapwindow.h"
#include "canvas.h"
#include "detailwin.h"
#include "calwindow.h"
#include "searchwin.h"
#include "settingwin.h"

#define ROW_NUM 100

#define ANIMATE_DURATION 400

class MapWindow;
class CalWindow;
class SearchWin;

//class ImportThread : public QThread {
//    Q_OBJECT
//private:
//    DataBase *&database;
//    const std::string &tName;

//public:
//    QStringList fileList;

//    ImportThread(DataBase *&db, const std::string &t, const QStringList &ls)
//            : QThread(), database(db), tName(t), fileList(ls) {}

//    void run() {
//        int n_ok = 0;
//        int total = 0;

//        foreach (const QString eachFile, fileList) {

//            string fileName = eachFile.toLocal8Bit().data();
//      //   ifstream fin(fileName.c_str());
//            emit importHandledSignal(total++);


//            ifstream fin2(fileName.c_str());
//            Csv format(fin2);
//      //      Sequence *t = new Sequence();
//            try {
//       //     getSquFromFile(&format, t);
//               database->insertData(&format, tName);
//            } catch (int i) {
//                emit importFileErrorSignal(i);
//                break;
//            }

//   //         delete t;
//            /*
//            Csv csv(fin);
//            database->insertData(&csv, tName);
//            */
//            emit importedOneFileSignal(n_ok);
//            ++n_ok;
//        }
//        emit importFinishedSignal(n_ok, fileList.size() - n_ok);
//    }
//signals:
//    void importFinishedSignal(int, int);
//    void importedOneFileSignal(int);
//    void importFileErrorSignal(int);
//    void importHandledSignal(int);
//};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setDb(DataBase *database);
    QVector<Sequence> getCurrentSeqs();

    ~MainWindow();

    /*
     * 动画效果
     */
    void startAnimation();
    void closeWindowAnimation();
    void shakeWindow();

    /*
     * starck设置当前页面
     */
    void setStackCurrentPage(quint8 index);


protected:
    /*
     * 窗体拖动
     */
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_closeToolButton_clicked();
    void on_minToolButton_clicked();
    void on_searchToolButton_clicked();
    void on_checkToolButton_2_clicked();
    void on_calToolButton_clicked();
    void on_mapToolButton_clicked();
    void on_opToolButton_clicked();
    void on_exToolButton_clicked();
    void aboutButton_clicked();
    void on_gjToolButton_clicked();
    void openFile();
    void showMap();
    void about();
    void searchInDB();
    void paint();
    void clearDB();
    void openFilesSlot();
    void clickTBSlot(const QModelIndex index);
    void deleteOneAct();
    void searchSlot_R();
    void showInMapSlot_R();
    void detailSlot_R();
    void goBackSlot();
    void showInMap();
    void setSlot();
    void exportSlot();
    void refreshTable();
//    void refreshValue(int n);
    void importFileErrorSlot(int code);
    void importFinished(int ok, int bad);
 //   void transferImportNumber(int n);

private:
    Ui::MainWindow *ui;
    friend class CalWindow;
    friend class SearchWin;
    SettingWin *settingWin;
    /*
     * 记录窗口位置
     */
    QPoint startPos;
    QPoint endPos;

    // Database
    string tName;
    bool search_mode;
    DataBase *db;
    Canvas *can;
    // Thread for import file
//    ImportThread importThread;
    //QProgressDialog *importProgressDialog;
    //CSS
//    QProgressDialog importProgressDialog;
    QFile *cssFile;
    QString *strCSS;
    // calWindow *calWin;
    detailWin *detWin;
    CalWindow *cal;
    SearchWin *searchWin;
    QStringList *tracs;

    QVector<Sequence> selectedSeqS;

    //Table action
    QAction *deleteAct;
    QAction *searchAct;
    QAction *showInMapAct;
    QAction *detailAct;

    Sequence input;

    void initAction();
    void initCSS();
    void initTable();
    void initSig();
    void initCan();
    void initSearchWin();
    void search(Sequence input);
    void initMap();
    void searchMode(bool inSearch);
    void getDetail(Sequence *se_a);
};


#endif // MAINWINDOW_H