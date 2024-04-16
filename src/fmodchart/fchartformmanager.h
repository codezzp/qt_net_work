#ifndef FCHARTFORMMANAGER_H
#define FCHARTFORMMANAGER_H

#include <QObject>
#include "fmodchart.h"
#include "fchartform.h"
#include "dialog/createchartformdialog.h"

// 统管所有ChartForm窗体
class FChartFormManager: public QObject
{
    Q_OBJECT

public:
    FChartFormManager();
    ~FChartFormManager();

    static FChartFormManager *GetInstance();
    static void ReleaseInstance();

    void Init(const QVector<FChartFormParam> &arr);
    void UnInit();
    // 定时做事情,去绘制所有窗口
    void OnTime(const QVector<double> &arr);

    //初步处理数据，把需要的比特位留下,并且调用OnTime
    void dealdata(char*datagram);

    //获取FormArray
    const QVector<FChartForm *> & GetFormArray();

    //开始所有窗口绘制
    void StartDraw();
    //停止所有窗口绘制
    void StopDraw();
private:
    void Clear();
    //分离数据
    double splitdata_x16(char*datagram,Bitno no,Typebyte type,bool isunsigned,int datalong);
    QVector<FChartForm *> ChartFormArray;
    QMutex ArrayMutex;
    QThreadPool ChartFormPool;
    bool Stop = false;
private slots:
    void Slot_CreateChartForm(const FChartFormParam& cfp,Bitno no);
};

#endif // FCHARTFORMMANAGER_H
