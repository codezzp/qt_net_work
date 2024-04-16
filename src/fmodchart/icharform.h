#ifndef ICHARFORM_H
#define ICHARFORM_H
#include <QVector>
#include "fmodchart/fmodchart.h"

class FChartFormManager;

class ICharForm
{
public:
    ICharForm();
    ~ICharForm();

    //开始绘制
    bool StartDraw();
    //停止绘制
    bool StopDraw();
    //接收数据并且绘制
    bool DrawData(char *datagram);
    //创建窗口
    bool CreateChartForm(const FChartFormParam &cfp);
    bool UnInit();
private:
    bool _isStart = false;
    FChartFormManager *fcfm;
};

#endif // ICHARFORM_H
