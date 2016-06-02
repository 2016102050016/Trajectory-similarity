#include "searchwin.h"
#include "ui_mainwindow.h"
SearchWin::SearchWin(Ui::MainWindow *ui,DataBase *db)
{
    this->ui = ui;
    this->db = db;
    input = new Sequence();
    time = false;
    distinct = true;
    numOfSeqs = 3;
    tracs = new QStringList();
    rowcount = 0;
    partRowcount = 0;
    db = new DataBase(1);
    ui->searchMap->initJS();
    ui->searchMap->reload();
    initTable(ui->searchTable_common);
    initTable(ui->searchTable_common_part);
    initTable(ui->searchTable_common_point);
    initTable(ui->searchTable_time);
    initTable(ui->searchTable_time_part);
    initTable(ui->searchTable_time_point);
    initSeqPartTable(ui->searchTable_common_part);
    initSeqPartTable(ui->searchTable_time_part);
    initPointTable(ui->searchTable_common_point);
    initPointTable(ui->searchTable_time_point);

    ui->searchStackedWidget->setCurrentIndex(0);
    ui->searchStackedWidget_time->setCurrentIndex(0);
    initSig();
    //refreshTable();
}

void SearchWin::setTracs(QStringList *tracs)
{
    this->tracs = tracs;
}

void SearchWin::setDB(DataBase *db)
{
    this->db = db;
}


SearchWin::~SearchWin()
{
    delete input;
}

void SearchWin::initTable(QTableWidget *table)
{
        //table->horizontalHeader()->setStretchLastSection(true);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->hide();
        table->setContextMenuPolicy(Qt::ActionsContextMenu);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setColumnCount(4);
        table->setRowCount(ROW_NUM);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SearchWin::initSeqPartTable(QTableWidget *table)
{
    table->setColumnCount(4);
    table->setRowCount(ROW_NUM);
    table->clearContents();
    QStringList header;
    header << tr("轨迹段所在轨迹")
           << tr("起点-终点")
           << tr("对应起点-终点")
           << tr("相似度");
    table->setHorizontalHeaderLabels(header);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void SearchWin::initPointTable(QTableWidget *table)
{    
    table->setColumnCount(4);
    table->setRowCount(ROW_NUM);
    table->clearContents();
    QStringList header;
    header << "轨迹点所在轨迹"
           << "点在轨迹中位置"
           << "点在输入轨迹中位置"
           << "DIS";

    table->setHorizontalHeaderLabels(header);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void SearchWin::showPartofSeq()
{
    //qDebug() << "show Part Of Sequence\n";
}

void SearchWin::drawPoints()
{

    QString sid = "";
    int index2 = 0;
    int index1 = 0;
    double dis = 0;
    int num = 0;
    for (int j = 0; j < rowcount; j ++)
    {


        if (!time)
        {
            sid =  ui->searchTable_common_point->item(j,0)->text();
            index2 =  ui->searchTable_common_point->item(j,1)->text().toInt();
            index1 =  ui->searchTable_common_point->item(j,4)->text().toInt();
            dis = ui->searchTable_common_point->item(j,3)->text().toDouble();
            if (dis !=0 )
                num = ui->searchTable_common_point->item(j,2)->text().toInt();
        }
        else
        {
            sid =  ui->searchTable_time_point->item(j,0)->text();
            index2 = ui->searchTable_time_point->item(j,1)->text().toInt();
            index1 =  ui->searchTable_time_point->item(j,4)->text().toInt();
            dis = ui->searchTable_time_point->item(j,3)->text().toDouble();
            if (dis !=0 )
                num = ui->searchTable_time_point->item(j,2)->text().toInt();
        }

        if (dis > 1)
        {
            break;
        }
        if (dis == 0){
            ui->searchMap->drawPoint(&((id_seq_map[sid]).pts[index2]),"",seq_index[sid],true);
        }
        else
        {
            ui->searchMap->drawPoint(&((id_seq_map[sid]).pts[index2]),"",seq_index[sid],num);
            ui->searchMap->drawPoint(&(input->pts[index1]),"",seq_index[sid],num);
        }
       // ui->searchMap->drawPoint(&((seqs[seq_index[sid]])[index1]),"",seq_index[sid],j);

    }
    seq_index.clear();
}
/*
void SearchWin::search()
{
    fillTable(*input);//填充三个表格
    if (!time)
    {
        sortPointTable(ui->searchTable_common_point);
    }
    else
    {
        sortPointTable(ui->searchTable_time_point);
    }

    ui->searchMap->initJS();
    ui->searchMap->showPoints(true);
    ui->searchMap->showTimes(true);
    ui->searchMap->setCentralPoint(getCenterPoint(seqs), 5);
    ui->searchMap->setFilter(time);
    ui->searchMap->setNumOfSeq(4);
    ui->searchMap->drawSequences(seqs, coincide);
    //drawPoints();
    ui->searchMap->reload();

    qDebug() << "Search Over";

}
*/
void SearchWin::searchSeq()
{
    double dfDis;
    double maxDis = 0;
    int c = 0;
    int t = 0;


    for (int i = 0;i < tracs->length();i++)
    {

        Sequence sf;
        sf = id_seq_map[tracs->at(i)];
        QTableWidgetItem *tItem = new QTableWidgetItem();

        dfDis = computeDiscreteFrechet(input,&sf);
        if (dfDis == 0)
        {
            coincide << sf.getID();
        }
        tItem->setData(Qt::DisplayRole,dfDis);

        if (sf.hasTime() && input->hasTime())
        {
            ui->searchTable_time->setItem(t,2,tItem);
            t++;
        }
        else if (!sf.hasTime() && !input->hasTime())
        {
            ui->searchTable_common->setItem(c,2,tItem);
            c++;
        }
        if (dfDis >= maxDis)
        {
            maxDis = dfDis;
        }
    }


    if ( c != 0)
    {
        time = false;
        ui->searchTable_common->sortItems(2,Qt::AscendingOrder);
        for (int i = 0; i < c; i++)
        {
            QTableWidgetItem *tItem = new QTableWidgetItem();
            double dis = ui->searchTable_common->item(i, 2)->text().toDouble();
            double percent = (maxDis - dis)/maxDis * 100;

            if (percent < 0)
            {
                percent = 0;
            }
            tItem->setData(Qt::DisplayRole,
                           QString::number(percent) + "%");
            ui->searchTable_common->setItem(i,3,tItem);
        }
    }
    else if (t != 0)
    {
        time = true;
        ui->searchTable_time->sortItems(2,Qt::AscendingOrder);
        for (int i = 0; i < t; i++)
        {
            QTableWidgetItem *tItem = new QTableWidgetItem();
            double dis = ui->searchTable_time->item(i, 2)->text().toDouble();
            double percent = (maxDis - dis)/maxDis * 100;
            if (percent < 0)
            {
                percent = 0;
            }
            tItem->setData(Qt::DisplayRole,
                           QString::number(percent) + "%");
            ui->searchTable_time->setItem(i,3,tItem);
        }
    }
}

void SearchWin::sortPointTable()
{
    ui->searchTable_common_point->sortItems(3, Qt::AscendingOrder);
    ui->searchTable_time_point->sortItems(3, Qt::AscendingOrder);
    /*
    int length = rowcount;
    int j = 0;
    for (int i =0; i < length; i++)
    {
        if (table->item(i,3)->text().toDouble() == 0)
        {
            table->setItem(i,2, new QTableWidgetItem(""));
            continue;
        }

        table->setItem(i,2, new QTableWidgetItem(QString::number(j)));
        j ++;
        seq_index.insert(table->item(i,0)->text(), table->item(i,2)->text().toInt());
    }
    */

}

void SearchWin::sortPartTable()
{
    ui->searchTable_common_part->sortItems(3, Qt::AscendingOrder);
    ui->searchTable_time_part->sortItems(3, Qt::AscendingOrder);
}

void SearchWin::sortSeqTable()
{
    ui->searchTable_common->sortItems(2, Qt::AscendingOrder);
    ui->searchTable_time->sortItems(2, Qt::AscendingOrder);
}

void SearchWin::initSig()
{
    connect(ui->searchStartBtn, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->calSeqPartBtn, SIGNAL(clicked()), this, SLOT(rankPartOfSeq()));
    connect(ui->calSeqBtn, SIGNAL(clicked()), this, SLOT(rankSeqClicked()));
    connect(ui->calSeqPointBtn, SIGNAL(clicked()), this, SLOT(rankSeqPointClicked()));
}

void SearchWin::fillPointTable(QTableWidget *table, QVector<PointCompare> pointsV, Sequence *se)
{
   // qDebug() << "rowcount = "
     //        << QString::number(rowcount);
    if (distinct)
    {
        if (coincide.contains(se->getID()))
        {
            return;
        }
    }
    int start = rowcount;
    int end = start + pointsV.length();
    if (end > rowcount)
    {
        table->setRowCount(end + 10);
    }
    else if (rowcount == 0)
    {
        table->setRowCount(ROW_NUM);
    }
    for (int x = start; x < end; x++)
    {
        table->setItem(x,0, new QTableWidgetItem(se->getID()));
        table->setItem(x,1, new QTableWidgetItem(
                                                QString::number(pointsV[x - start].index2)));

        table->setItem(x,3, new QTableWidgetItem(
                                                QString::number(pointsV[x - start].distance)));
        table->setItem(x,2, new QTableWidgetItem(
                                                QString::number(pointsV[x - start].index1)));
    }
    rowcount = end;
}

void SearchWin::fillPartTable(QTableWidget *table, QVector<QVector<int> > partInfo,Sequence *se)
{
    if (distinct)
    {
        if (coincide.contains(se->getID()))
        {
            return;
        }
    }

    /*
     *    header << tr("轨迹段所在轨迹")
           << tr("起点-终点")
           << tr("对应起点-终点")
           << tr("相似度");
     */
    QVector<int>pv=partInfo[0];
    QVector<int>qv=partInfo[1];
    QString str;
    initP_Q(input, se);
    initMemSpace(input, se);
    for(int i=0; i<pv.size() - 1; i=i+2){
        int begin1=pv[i];
        int end1=pv[i+1];
        int begin2=qv[i];
        int end2=qv[i+1];

        table->setItem(partRowcount, 0, new QTableWidgetItem(se->getID()));
        str  = QString::number(begin2) + "-"+QString::number(end2);
        table->setItem(partRowcount, 1, new QTableWidgetItem(str));

        str  = QString::number(begin1) + "-"+QString::number(end1);
        table->setItem(partRowcount, 2, new QTableWidgetItem(str));

        double res = computeDFD_new(begin1, end1, begin2, end2);
        str  = QString::number(res);
        table->setItem(partRowcount, 3, new QTableWidgetItem(str));
        partRowcount ++;
    }

}

void SearchWin::fillTable(Sequence inSeq)
{
    string tb = "importtest";
    QVector<PointCompare> pVec;
    double dfDis;
    double maxDis = 0;
    int c = 0;
    int t = 0;

    seqs.append(inSeq);
    id_seq_map.insert(inSeq.getID(), inSeq);

    for (int i = 0;i < tracs->length();i++)
    {
        Sequence sf;
        QTableWidgetItem *tItem = new QTableWidgetItem();
        db->getSequenceByID(tb,&sf,QString((*tracs)[i]).toStdString());
        seqs.append(sf);
        id_seq_map.insert(sf.getID(), sf);

        dfDis = computeDiscreteFrechet(&inSeq,&sf);
        if (dfDis == 0)
        {
            coincide << sf.getID();
        }

        tItem->setData(Qt::DisplayRole,dfDis);
        pVec = getNearestPoint(&inSeq, &sf);

        if (sf.hasTime() && inSeq.hasTime())
        {
            ui->searchTable_time->setItem(t,2,tItem);
            fillPointTable(ui->searchTable_time_point, pVec, &sf);
            t++;
        }
        else if (!sf.hasTime() && !inSeq.hasTime())
        {
            ui->searchTable_common->setItem(c,2,tItem);
            fillPointTable(ui->searchTable_common_point, pVec, &sf);
            c++;
        }
        if (dfDis >= maxDis)
        {
            maxDis = dfDis;
        }
    }


    if ( c != 0)
    {
        time = false;
        ui->searchTable_common->sortItems(2,Qt::AscendingOrder);
        for (int i = 0; i < c; i++)
        {
            Sequence sf;
            if (i < numOfSeqs) {
                QString id = ui->searchTable_common->item(i, 0)->text();
                db->getSequenceByID(tb,&sf,id.toStdString());             
            }

            QTableWidgetItem *tItem = new QTableWidgetItem();
            double dis = ui->searchTable_common->item(i, 2)->text().toDouble();
            double percent = (maxDis - dis)/maxDis * 100;

            if (percent < 0)
            {
                percent = 0;
            }
            tItem->setData(Qt::DisplayRole,
                           QString::number(percent) + "%");
            ui->searchTable_common->setItem(i,3,tItem);
        }
    }
    else if (t != 0)
    {
        time = true;
        ui->searchTable_time->sortItems(2,Qt::AscendingOrder);
        for (int i = 0; i < t; i++)
        {
            Sequence sf;
            if (i < numOfSeqs)
            {
                QString id = ui->searchTable_time->item(i, 0)->text();
                db->getSequenceByID(tb,&sf,id.toStdString());
            }
            QTableWidgetItem *tItem = new QTableWidgetItem();
            double dis = ui->searchTable_time->item(i, 2)->text().toDouble();
            double percent = (maxDis - dis)/maxDis * 100;
            if (percent < 0)
            {
                percent = 0;
            }
            tItem->setData(Qt::DisplayRole,
                           QString::number(percent) + "%");
            ui->searchTable_time->setItem(i,3,tItem);
        }
    }
}

void SearchWin::calSecPart()
{
    for (int i = 0;i < tracs->length();i++)
    {
        QVector<QVector<int> >qc;
        Sequence sf;
        sf = id_seq_map[tracs->at(i)];
        qc = getSimplify(input,&sf);

        if (sf.hasTime() && input->hasTime())
        {
            fillPartTable(ui->searchTable_time_part,
                      qc, &sf);
        }
        else if (!sf.hasTime() && !input->hasTime())
        {
            fillPartTable(ui->searchTable_common_part,
                      qc, &sf);
        }
    }
}

void SearchWin::drawSeq()
{
    ui->searchMap->initJS();
    ui->searchMap->showPoints(true);
    ui->searchMap->showTimes(true);
    ui->searchMap->setCentralPoint(getCenterPoint(seqs), 5);
    ui->searchMap->setFilter(time);
    ui->searchMap->setNumOfSeq(4);
    ui->searchMap->drawSequences(seqs, coincide);
    //drawPoints();
    ui->searchMap->reload();
}

void SearchWin::searchPoint()
{
    QVector<PointCompare> pVec;
    for (int i = 0;i < tracs->length();i++)
    {
        Sequence sf;
        sf = id_seq_map[tracs->at(i)];
        if (sf.hasTime() && input->hasTime())
        {
            pVec = getNearestPoint(input, &sf);
            fillPointTable(ui->searchTable_time_point, pVec, &sf);
        }
        else if (!sf.hasTime() && !input->hasTime())
        {
            pVec = getNearestPoint(input, &sf);
            fillPointTable(ui->searchTable_common_point, pVec, &sf);
        }
    }
}

void SearchWin::loadIntoMem()
{
    string tb = "importtest";
    seqs.append(*input);
    id_seq_map.insert(input->getID(), *input);

    for (int i = 0;i < tracs->length();i++)
    {
        //将轨迹存入内存，便于后期使用
        Sequence sf;
        db->getSequenceByID(tb,&sf,QString((*tracs)[i]).toStdString());
        seqs.append(sf);
        id_seq_map.insert(sf.getID(), sf);
    }
}

void SearchWin::refreshTable()
{
    initSeqPartTable(ui->searchTable_common_part);
    initSeqPartTable(ui->searchTable_time_part);
    initPointTable(ui->searchTable_common_point);
    initPointTable(ui->searchTable_time_point);
    ui->searchTable_common->clearContents();
    ui->searchTable_time->clearContents();
    QStringList header;
    header << tr("ID")
           << tr("PointNumber")
           << tr("Frechet Distance")
           << tr("Similarity");
    ui->searchTable_common->setHorizontalHeaderLabels(header);
    ui->searchTable_time->setHorizontalHeaderLabels(header);
    if (tracs->length() == 0)
    {
        ui->searchTable_common->clearContents();
        ui->searchTable_time->clearContents();
    }
    else {
        int pos_c = 0;
        int pos_t = 0;
        if (tracs->length() > ROW_NUM)
        {
            ui->searchTable_common->setRowCount(tracs->length() + 10);
            ui->searchTable_time->setRowCount(tracs->length() + 10);
        }
        else
        {
            ui->searchTable_common->setRowCount(ROW_NUM);
            ui->searchTable_time->setRowCount(ROW_NUM);
        }
        for(int i = 0; i < tracs->length();i++)
        {
            QString temp = (*tracs)[i];
            if (db->hasTime(temp.toStdString(),"importtest"))
            {
                ui->searchTable_time->setItem(pos_t,0, new QTableWidgetItem(temp));
                ui->searchTable_time->setItem(pos_t,1,
                                   new QTableWidgetItem(QString::number(db->getPointNumByID(temp.toStdString()))));
                pos_t ++;
            }
            else
            {
                ui->searchTable_common->setItem(pos_c,0, new QTableWidgetItem(temp));
                ui->searchTable_common->setItem(pos_c,1,
                                   new QTableWidgetItem(QString::number(db->getPointNumByID(temp.toStdString()))));
                pos_c ++;
            }
        }
    }

}

void SearchWin::setNumOfSeqs(int num)
{
    numOfSeqs = num;
}

void SearchWin::init()
{
    coincide.clear();
    seqs.clear();
    id_seq_map.clear();
    ui->searchtabWidget->setCurrentIndex(0);
    ui->searchStackedWidget->setCurrentIndex(0);
    ui->searchStackedWidget_time->setCurrentIndex(0);
}


void SearchWin::openFile()
{

    if (input->getNum() != 0)
    {
        delete input;
        input = new Sequence();
    }
    QString file_name = QFileDialog::getOpenFileName(NULL,
            tr("Open File"),
            "",
            "CSV Files(*.csv)",
            0);
    if (!file_name.isNull())
        {
            qDebug()<<file_name;
        }
        else{
            return;
    }
    string fileName = file_name.toLocal8Bit().data();
    ifstream fin(fileName.c_str());
    Csv csv(fin);
    getSquFromFile(&csv,input);
    ui->searchPathEdit->setText(file_name);
    init();
    refreshTable();
}

void SearchWin::rankPartOfSeq()
{
    if (input == NULL)
    {
        return;
    }
    if (input->getNum() == 0)
    {
        return;
    }
    partRowcount = 0;

    this->ui->searchMap->initJS();
    this->ui->searchMap->setDefaultCentralPt();
    this->ui->searchMap->reload();

    if (seqs.length() == 0)
    {
        loadIntoMem();

    }
    if (coincide.length() == 0)
    {
        searchSeq();
        sortSeqTable();
    }
    calSecPart();
    sortPartTable();
    if (time)
    {
        ui->searchtabWidget->setCurrentIndex(1);
    }
    else
    {
        ui->searchtabWidget->setCurrentIndex(0);
    }
    ui->searchStackedWidget->setCurrentIndex(2);
    ui->searchStackedWidget_time->setCurrentIndex(2);

}

void SearchWin::rankSeqClicked()
{
    if (input == NULL)
    {
        return;
    }
    if (input->getNum() == 0)
    {
        return;
    }
    this->ui->searchMap->initJS();
    this->ui->searchMap->setDefaultCentralPt();
    this->ui->searchMap->reload();

    if (seqs.length() == 0)
    {
        loadIntoMem();
    }

    if (coincide.length() == 0)
    {
        searchSeq();
        sortSeqTable();
    }
    drawSeq();
    if (time)
    {
        ui->searchtabWidget->setCurrentIndex(1);
    }
    else
    {
        ui->searchtabWidget->setCurrentIndex(0);
    }
    ui->searchStackedWidget->setCurrentIndex(0);
    ui->searchStackedWidget_time->setCurrentIndex(0);

    showPartofSeq();

}

void SearchWin::rankSeqPointClicked()
{

    if (input == NULL)
    {
        return;
    }
    if (input->getNum() == 0)
    {
        return;
    }

    rowcount = 0;
    this->ui->searchMap->initJS();
    this->ui->searchMap->setDefaultCentralPt();
    this->ui->searchMap->reload();
    if (seqs.length() == 0)
    {
        loadIntoMem();
    }

    if (coincide.length() == 0)
    {
        searchSeq();
        sortSeqTable();
    }
    searchPoint();
    if (time)
    {
        ui->searchtabWidget->setCurrentIndex(1);
    }
    else
    {
        ui->searchtabWidget->setCurrentIndex(0);
    }
    ui->searchStackedWidget->setCurrentIndex(1);
    sortPointTable();
    ui->searchStackedWidget_time->setCurrentIndex(1);
    showPartofSeq();

}


