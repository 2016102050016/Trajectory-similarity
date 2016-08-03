/**
  *   赛题：第五届“中国软件杯”大学生软件设计大赛——针对以经纬度或经纬度带时间定义的不同轨迹
  *   软件名称：基于“弗雷歇距离”的轨迹相似度分析软件
  *   开发团队：Kryptonite
  *   开发时间：2016年4至6月
  *   文件功能概述：核心算法实现文件，包含轨迹(带及不带时间)相似度计算、轨迹(带及不带时间)相似轨迹段查询、
  *             轨迹(带及不带时间)相似轨迹点查询的实现函数
  */

#ifndef CORE_H
#define CORE_H

#define EARTH_RADIUS 6378.137
#define PI 3.14
#include <QString>
#include <QFileDialog>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include "sequence.h"
#include "csv.h"

struct PointCompare{
  int index1;
  int index2;
  double distance;
};

struct SecCompare{
    int beginIndex1;//起点1
    int endIndex1;//终点1
    int beginIndex2;
    int endIndex2;
    double simliarity;//轨迹段相似度
};

void initP_Q(Sequence *m, Sequence *n);
QVector<SecCompare> getBestSce(QVector<SecCompare> secCompareV_a);
void initMemSpace(Sequence *p, Sequence *q);
void getSquFromFile(Csv *csv, Sequence *se);
double computeDiscreteFrechet(Sequence *sa,Sequence *sb);
double computeDFD(int i,int j, Sequence *p_a, Sequence * q_a);
double euclideanDistance(Point* a, Point* b);
double  getSecSim(int i1,int j1,int i2,int j2);
bool compare(SecCompare s1,SecCompare s2);
QVector<SecCompare> findSimilarSection(Sequence *se_a, Sequence *se_b);//调用以计算轨迹段
void calculateSec(int gap, int h,QVector<SecCompare>&q1);
void mergeChange(int gap,QVector<SecCompare>&q1,QVector<SecCompare>&q2,double limit);
Sequence* longestCommonSeq(Sequence &p, Sequence &q, double thres);
Point getCenterPoint(Sequence *se_a, int num);
Point getCenterPoint(QVector<Sequence> seqV);
Time loadToStruct(QString time);
bool timeCompare(Point*p1,Point*p2);
double calTimeDistance(Point *a,Point *b);
double calCoef();
QVector<SecCompare> findBest(Sequence*p,Sequence*q);
QVector<QVector<int> > getSimplify(Sequence*p,Sequence*q);
QVector<PointCompare> getNearestPoint(Sequence *se_a, Sequence *se_b);
bool compareDis(PointCompare p1,PointCompare p2);
void out2DArray(double **arr, int x, int y);
double computeDFD_new(int startx, int endx, int starty, int endy);
double rad(double d);
double getDistance(double lng1, double lat1, double lng2, double lat2);
int getZoom(QVector<Sequence> seqV);
int getZoom(Sequence seq_a);
#endif // CORE_H
