#ifndef FCHARTFORM_H
#define FCHARTFORM_H

#include <QWidget>
#include <QtCharts>
#include <QDateTime>
#include <QMouseEvent>
#include "fmodchart.h"
#include "dialog/settingdialog.h"
#include "dialog/querydialog.h"
#include "dialog/addseriesdialog.h"
//using namespace QtCharts;


namespace Ui {
class FChartForm;
}


// 绘制曲线的主界面
class FChartForm : public QWidget, public QRunnable
{
    Q_OBJECT

public:
    explicit FChartForm(FChartFormParam cfp,QWidget *parent = nullptr);
    ~FChartForm();

    // mcp是chart参数，range是x轴对数据间隔的倍数范围，默认30倍
    void Start(const FModChartParam &mcp, int range =30);

    void OnTime(QDateTime curdt, double val,Seriesno no);

    // 设置窗体title
    void SetTitle(const QString &title);

    //设置折线
    void InitSeries(Bitno no);

    //获取设置参数
    const FChartFormParam &GetParam();

    //获取每条折线对应的比特位
    QVector<Bitno> GetseriesBitnos();

protected:
    void run() override;

private:
    void InitPopupMenu();
    void ReleasePopupMenu();
    //记录跳变
    void RecordHop(QDateTime curdt, double val,Seriesno no);
    //创建跳变
    void CreateHop(QDateTime curdt, double val,Seriesno no);
    //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event) override;
    //鼠标滚轮事件
    void wheelEvent(QWheelEvent *event) override;
private slots:
    // 暂停
    void Slot_PopupMenuPause();
    // 缩小
    void Slot_PopupMenuNarrow();
    // 放大
    void Slot_PopupMenuEnlarge();

    // 左移/右移
    void Slot_PopupMenuLeft();
    void Slot_PopupMenuRight();

    //跳变设置的窗口
    void Slot_SettingJump();

    //跳变查询的窗口
    void Slot_QueryJump();

    //添加折线的窗口
    void Slot_Addseries();

    //隐藏跳变
    void Slot_HideJump();

    //隐藏折线图
    void Slot_Hidechart();

    //设置为蓝色主题
    void Slot_ChangeBluechartTheme();
    //设置为亮色主题
    void Slot_ChangeLightchartTheme();
    //设置为暗色主题
    void Slot_ChangeDarkchartTheme();

    //响应跳变设置，改变跳变设置的参数
    void Slot_Hopdata(bool isscancurrentdata,bool isappointdata,QVector<double> appointdata);
    void Slot_Hopdata(bool isscancurrentdata,bool isappointdata,double maxval,double minval);

    //响应跳变查询
    void Slot_Querytext(QString querytype,QString queryvalue);
    //鼠标点击事件(出现菜单)
    void mousePressEvent(QMouseEvent *mouse) override;

    void Slot_Draw(QDateTime min, QDateTime max, qint64 curdt, double val,Seriesno no=0);

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

signals:
    void Signal_OnDraw(QDateTime min, QDateTime max, qint64 curdt, double val,Seriesno no=0);
    void Signal_Keyleft();
    void Signal_Keyright();
    void Signal_Keydown();
    void Signal_Keyup();
private:
    Ui::FChartForm *ui;
    // 右键菜单
    QMenu *pChartPopupMenu = nullptr;
    // 菜单项
    QList<QAction *> ActionList;

    // 绘图
    QChart chart;

    //折线表
    QList<SeriesBit*> LineSeriesList;

    // 绘图需要的时间和值
    QDateTime curDateTime;
    double curvalue;

    QDateTimeAxis ax_time;
    QValueAxis ax_value;
    QValueAxis ay;

    qint64 PreTime=0;

    double y_min = -1;
    double y_max = 1;

    // 最开始的计时
    qint64 time1 =0;
    //qint64 time2 =0;

    // 暂停绘制
    bool Paused = false;

    // x轴对数据间隔的倍数范围，
    // 平均2次数据的时间间隔 * Range = x轴范围
    int Range = 1;

    //帧长
    int _datalong=0;

    //有无字符
    bool _isunsigned =false;

    //字节数
    Typebyte _byte =ONE_BYTE;
    uint Count =0;

    //显示窗口的参数
    FChartFormParam _cfp;

    //次数计数
    int count =0;

    //x轴间隔数
    int tickcount =10;
};



#endif // FCHARTFORM_H
