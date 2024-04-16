#ifndef FMODCHART_H
#define FMODCHART_H

#include <QObject>
#include <QLineSeries>

//折线编号
typedef int Seriesno;

//比特位编号
typedef int Bitno;

//背景颜色
enum BackgroundTheme
{
    BULE_THEME,
    LIGHT_THEME,
    DARK_THEME
};

//需要分离的比特数
enum Typebyte
{
    ONE_BYTE =1,
    TWO_BYTE =2,
    FORE_BYTE =4,
    EIGHT_BYTE =8
};

//每条折线对应的比特位
typedef struct SeriesBit
{
    QLineSeries *lineseries;
    Bitno no_bit;
}SeriesBit;

// Chart参数
typedef struct FModChart_
{
    // x/y的网格线
    bool GridLineVisible_X      = false;
    bool GridLineVisible_Y      = false;

    // 是否平滑抗锯齿
    bool IsSmooth               = false;

    // 使用OpenGL绘制
    bool UseOpenGL              = true;

    // 曲线绘制时最大点数
    int MaxPointNumber          = 100;

}FModChartParam;

// 跳变设置参数
typedef struct HopParam_
{
        // 是检测当前数据还是除了规定数据以外的数据
        bool isscancurrentdata = true;
        // 是检测指定数据还是范围数据
        bool isappointdata = false;

        //指定数据组
        QVector<double> appointdatagroup;

        //指定数据范围
        double maxval =999;
        double minval =0;
}HopParam_;

// 单个ChartForm需要的参数
typedef struct FChartFormParam_
{
    // 左上顶点坐标
    int x = 700;
    int y = 70;

    // 宽、高
    int w = 980;
    int h = 760;

    QString title;

    //x轴是否以时间轴形式显示
    bool IsAxisTimeForm_X       = true;

    //是否是无符号显示
    bool Isunsigned             = true;

    //数据的帧长
    int _datalong               = 1;

    //需要分离的比特数
    Typebyte byte               = ONE_BYTE;
    // X坐标范围：倍数
    // x轴对数据间隔的倍数范围，
    // 平均2次数据的时间间隔 * Range = x轴范围
    int XRangeMultiple =30;

    // chart显示参数
    FModChartParam mcp;
    //跳变设置参数
    HopParam_ hp;

    //显示背景
    BackgroundTheme p_theme = LIGHT_THEME;

}FChartFormParam;

#endif // FMODCHART_H
