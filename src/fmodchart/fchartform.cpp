#include <QMenu>
#include "fchartform.h"
#include "ui_fchartform.h"

FChartForm::FChartForm(FChartFormParam cfp, QWidget *parent):
    QWidget(parent),
    ui(new Ui::FChartForm),
    _cfp(cfp)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,80);
    ui->splitter->setStretchFactor(1,20);
    //将table表设置为选中整行
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWindowIcon(QIcon(":/view.png"));
    setAutoDelete(false);
    connect(this, SIGNAL(Signal_OnDraw(QDateTime, QDateTime, qint64, double,Seriesno)), this, SLOT(Slot_Draw(QDateTime, QDateTime, qint64, double,Seriesno)));
    InitPopupMenu();
}

FChartForm::~FChartForm()
{
    delete ui;
}

void FChartForm::Start(const FModChartParam &mcp, int range)
{
    Range = range;
    _cfp.mcp = mcp;
    QDateTime cur = QDateTime::currentDateTime();
    count = 0;

    ax_time.setTickCount(10);
    ax_value.setTickCount(tickcount);
    ay.setTickCount(8);

    ax_time.setRange(cur.addSecs(-10), cur.addSecs(10));
    ax_time.setFormat("HH:mm:ss:zzz"); //"mm:ss");

    ax_value.setRange(1,1+tickcount);

    ay.setMin(y_min);
    ay.setMax(y_max);

    if(_cfp.IsAxisTimeForm_X == true)
    {
        chart.addAxis(&ax_time, Qt::AlignBottom);
    }
    else
    {
        chart.addAxis(&ax_value,Qt::AlignBottom);
    }
    chart.addAxis(&ay, Qt::AlignLeft);

    for(int i=0;i<LineSeriesList.size();i++)
    {
        chart.addSeries(LineSeriesList.at(i)->lineseries);

        LineSeriesList.at(i)->lineseries->attachAxis(&ax_time);
        LineSeriesList.at(i)->lineseries->attachAxis(&ax_value);

        LineSeriesList.at(i)->lineseries->attachAxis(&ay);

        LineSeriesList.at(i)->lineseries->setPointsVisible();
    }

    // 隐藏marker
    foreach (QLegendMarker *marker, chart.legend()->markers())
    {
        if (marker->type() == QLegendMarker::LegendMarkerTypeXY)
        {
            marker->setVisible(false);
        }
    }

    // 不显图例，并且chart绘图区域会扩充至满
    chart.legend()->detachFromChart();
    chart.legend()->hide();

    PreTime = cur.toMSecsSinceEpoch();

    ui->graphicsView->setChart(&chart);
}

void FChartForm::OnTime(QDateTime curdt, double val, Seriesno no)
{
        if(no == 0)
            count++;
        if (Paused)
        {
            return;
        }
        else
        {
            QString title = _cfp.title + ":running";
            setWindowTitle(title);
        }

        if (! this->isVisible())
            return;

        qint64 now = curdt.toMSecsSinceEpoch();
        qint64 len = now - PreTime;

        PreTime = now;

        if (time1 == 0)
        {
            time1 = now;
            return;
        }

        if (val < y_min)
        {
            y_min = val -5;
            ay.setMin(y_min);
        }
        else if (val > y_max)
        {
            y_max = val +5;
            ay.setMax(y_max);
        }

        // 计算总平均间隔长度，用于决定x轴显示范围
        int avglen = len;
        if (Count >0)
            avglen = (now - time1)/Count;

//    // 根据平均值的倍数决定x轴的显示范围
//    // 40ms 是为了避免显示在最右边
//    ax.setRange(curdt.addMSecs(-avglen *Range +len), curdt.addMSecs(40 +len));
    if (LineSeriesList.at(no)->lineseries->points().size() > 100)
    {        
        LineSeriesList.at(no)->lineseries->removePoints(0,1);
    }

    Count++;

    //记录跳变
    RecordHop(curdt,val,no);

    // 界面绘制独立出来
    emit (Signal_OnDraw(curdt.addMSecs(-avglen *Range +len), curdt.addMSecs(40 +len), now, val,no));
}

void FChartForm::SetTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void FChartForm::InitSeries(Bitno no)
{
    QLineSeries *lineseries = new QLineSeries();
    SeriesBit *sb = new SeriesBit();
    sb->no_bit = no;
    sb->lineseries = lineseries;
    LineSeriesList.append(sb);
    Start(_cfp.mcp,_cfp.XRangeMultiple);
}

const FChartFormParam &FChartForm::GetParam()
{
    return _cfp;
}

QVector<Bitno> FChartForm::GetseriesBitnos()
{
    QVector<Bitno> bitnolist;
    foreach(const SeriesBit *sb,LineSeriesList)
    {
        bitnolist.append(sb->no_bit);
    }
    return bitnolist;
}

void FChartForm::run()
{
    //qDebug("CharForm obj=%p, SubThread=%u", this, GetCurrentThreadId());
    for(int i=0;i<LineSeriesList.size();i++)
    {
        OnTime(curDateTime, curvalue,i);
    }
}

void FChartForm::InitPopupMenu()
{
    pChartPopupMenu = new QMenu(this);
    QMenu *themeMenu = new QMenu(this);
    QAction *act = NULL;
    QAction *sact = NULL;

    act = new QAction("暂停/继续", pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_PopupMenuPause()));
    pChartPopupMenu->addAction(act);

    act = new QAction("缩小", pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_PopupMenuNarrow()));
    connect(this,&FChartForm::Signal_Keydown,this,&FChartForm::Slot_PopupMenuNarrow);
    pChartPopupMenu->addAction(act);

    act = new QAction("放大", pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_PopupMenuEnlarge()));
    connect(this,&FChartForm::Signal_Keyup,this,&FChartForm::Slot_PopupMenuEnlarge);
    pChartPopupMenu->addAction(act);

    act = new QAction("右移", pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_PopupMenuRight()));
    connect(this,&FChartForm::Signal_Keyright,this,&FChartForm::Slot_PopupMenuRight);
    pChartPopupMenu->addAction(act);

    act = new QAction("左移", pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_PopupMenuLeft()));
    connect(this,&FChartForm::Signal_Keyleft,this,&FChartForm::Slot_PopupMenuLeft);
    pChartPopupMenu->addAction(act);

    act =pChartPopupMenu->addMenu(themeMenu);
    act->setText("背景颜色");
    sact = new QAction("蓝色",themeMenu);
    connect(sact,SIGNAL(triggered(bool)),this, SLOT(Slot_ChangeBluechartTheme()));
    themeMenu->addAction(sact);
    sact = new QAction("亮色",themeMenu);
    connect(sact,SIGNAL(triggered(bool)),this, SLOT(Slot_ChangeLightchartTheme()));
    themeMenu->addAction(sact);
    sact = new QAction("暗色",themeMenu);
    connect(sact,SIGNAL(triggered(bool)),this, SLOT(Slot_ChangeDarkchartTheme()));
    themeMenu->addAction(sact);

    act = new QAction("隐藏/显示跳变",pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_HideJump()));
    pChartPopupMenu->addAction(act);

    act = new QAction("隐藏/显示折线图",pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)), this , SLOT(Slot_Hidechart()));
    pChartPopupMenu->addAction(act);

    act = new QAction("跳变设置",pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)),this, SLOT(Slot_SettingJump()));
    pChartPopupMenu->addAction(act);


    act = new QAction("跳变查询",pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)),this, SLOT(Slot_QueryJump()));
    pChartPopupMenu->addAction(act);

    act = new QAction("增加折线",pChartPopupMenu);
    connect(act, SIGNAL(triggered(bool)),this, SLOT(Slot_Addseries()));
    pChartPopupMenu->addAction(act);

}

void FChartForm::ReleasePopupMenu()
{
    if(pChartPopupMenu != nullptr)
    {
        delete pChartPopupMenu;
        pChartPopupMenu = nullptr;
    }
}

void FChartForm::RecordHop(QDateTime curdt, double val, Seriesno no)
{
    //是否已经记录了跳变
    bool havehopflag =false;

    //记录指定数据组的跳变
    if(_cfp.hp.isappointdata == true)
    {
        QVector<double> datagroup =_cfp.hp.appointdatagroup;
        foreach(double data,datagroup)
        {
                if(val == data)
                {
                    //检测当前指定的数据
                    if(_cfp.hp.isscancurrentdata)
                    {
                        CreateHop(curdt,val,no);
                    }
                    havehopflag = true; //记录了跳变
                    break;
                }

        }
        //检测除了指定数据以外的数据
        if(!_cfp.hp.isscancurrentdata && havehopflag == false)
        {
            CreateHop(curdt,val,no);
        }

    }
    //记录指定数据范围的跳变
    else
    {
        double max = _cfp.hp.maxval;
        double min = _cfp.hp.minval;
        if(_cfp.hp.isscancurrentdata)
        {
            if(val >= min && val <= max)
            {
                CreateHop(curdt,val,no);
            }
        }
        else
        {
            if(val <=min || val >=max)
            {
                CreateHop(curdt,val,no);
            }
        }
    }
}

void FChartForm::Slot_PopupMenuPause()
{
    Paused = !Paused;
    if(Paused)
    {
        QString title = _cfp.title + ":stoped";
        setWindowTitle(title);
    }
    else
    {
        QString title = _cfp.title + ":running";
        setWindowTitle(title);
    }
}

void FChartForm::Slot_PopupMenuNarrow()
{

    if(_cfp.IsAxisTimeForm_X == true)
    {
        const double factor = 1;
        qint64 xmin = ax_time.min().toMSecsSinceEpoch() ;
        qint64 xmax = ax_time.max().toMSecsSinceEpoch() ;
        qint64 dist = xmax - xmin;

        xmin = xmin - dist*factor/2;
        xmax = xmax + dist*factor/2;
        ax_time.setRange(QDateTime::fromMSecsSinceEpoch(xmin), QDateTime::fromMSecsSinceEpoch(xmax));
    }
    else
    {
        if(tickcount >0)
            tickcount-=2;
        ax_time.setTickCount(tickcount);
    }
}

void FChartForm::Slot_PopupMenuEnlarge()
{
    if(_cfp.IsAxisTimeForm_X == true)
    {
        const double factor = 1;
        qint64 xmin = ax_time.min().toMSecsSinceEpoch() ;
        qint64 xmax = ax_time.max().toMSecsSinceEpoch() ;
        qint64 dist = xmax - xmin;

        xmin = xmin + dist*factor/4;
        xmax = xmax - dist*factor/4;
        ax_time.setRange(QDateTime::fromMSecsSinceEpoch(xmin), QDateTime::fromMSecsSinceEpoch(xmax));

    }
    else
    {
        if(tickcount >0)
            tickcount+=2;
        ax_time.setTickCount(tickcount);
    }
}

void FChartForm::Slot_PopupMenuLeft()
{
    if(_cfp.IsAxisTimeForm_X == true)
    {
        const double factor = 1;
        qint64 xmin = ax_time.min().toMSecsSinceEpoch() ;
        qint64 xmax = ax_time.max().toMSecsSinceEpoch() ;
        qint64 dist = xmax - xmin;

        xmin = xmin + dist*factor/10;
        xmax = xmax + dist*factor/10;
        ax_time.setRange(QDateTime::fromMSecsSinceEpoch(xmin), QDateTime::fromMSecsSinceEpoch(xmax));
    }
    else
    {
        if(ax_value.min() >1)
        {
            ax_value.setRange(ax_value.min()-1,ax_value.max()-1);
        }
    }
}

void FChartForm::Slot_PopupMenuRight()
{
    if(_cfp.IsAxisTimeForm_X == true)
    {
        const double factor = 1;
        qint64 xmin = ax_time.min().toMSecsSinceEpoch() ;
        qint64 xmax = ax_time.max().toMSecsSinceEpoch() ;
        qint64 dist = xmax - xmin;

        xmin = xmin - dist*factor/10;
        xmax = xmax - dist*factor/10;
        ax_time.setRange(QDateTime::fromMSecsSinceEpoch(xmin), QDateTime::fromMSecsSinceEpoch(xmax));
    }
    else
    {
        ax_value.setRange(ax_value.min()+1,ax_value.max()+1);
    }

}

void FChartForm::Slot_SettingJump()
{
    qDebug() << "[FChartForm]pChartPopupMenu:Slot_settingdialog open dialog";
    SettingDialog dialog;
    void (FChartForm::*hopdataptr)(bool,bool,QVector<double>) = &FChartForm::Slot_Hopdata;
    void (FChartForm::*hopdataptr_2)(bool,bool,double,double) = &FChartForm::Slot_Hopdata;
    void (SettingDialog::*Signalhopdataptr)(bool,bool,QVector<double>) = &SettingDialog::Signal_Hopdata;
    void (SettingDialog::*Signalhopdataptr_2)(bool,bool,double,double) = &SettingDialog::Signal_Hopdata;
    connect(&dialog,Signalhopdataptr,this,hopdataptr);
    connect(&dialog,Signalhopdataptr_2,this, hopdataptr_2);
    dialog.exec();
}

void FChartForm::Slot_QueryJump()
{
    qDebug() << "[FChartForm]pChartPopupMenu:Slot_querydialog open dialog";
    QueryDialog dialog;
    connect(&dialog,&QueryDialog::Signal_Querytext,this,&FChartForm::Slot_Querytext);
    dialog.exec();
}

void FChartForm::Slot_Addseries()
{
    AddSeriesDialog dialog;
    connect(&dialog,&AddSeriesDialog::Signal_CreateSeries,this , &FChartForm::InitSeries);
    dialog.exec();
}

void FChartForm::Slot_HideJump()
{
    if(!ui->tableWidget->isHidden())
    {
        qDebug() << "[FChartForm]pChartPopupMenu:Slot_HideJump hide jumplist";
        ui->tableWidget->hide();
    }
    else
    {
        qDebug() << "[FChartForm]pChartPopupMenu:Slot_HideJump show jumplist";
        ui->tableWidget->show();
    }
}

void FChartForm::Slot_Hidechart()
{
    if(!ui->graphicsView->isHidden())
    {
        qDebug() << "[FChartForm]pChartPopupMenu:Slot_Hidechart hide graphicsView";
        ui->graphicsView->hide();
    }
    else
    {
        qDebug() << "[FChartForm]pChartPopupMenu:Slot_Hidechart show graphicsView";
        ui->graphicsView->show();
    }
}

void FChartForm::Slot_ChangeBluechartTheme()
{
    _cfp.p_theme = BULE_THEME;
    ui->graphicsView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
}

void FChartForm::Slot_ChangeLightchartTheme()
{
    _cfp.p_theme = LIGHT_THEME;
    ui->graphicsView->chart()->setTheme(QChart::ChartThemeLight);
}

void FChartForm::Slot_ChangeDarkchartTheme()
{
    _cfp.p_theme = DARK_THEME;
    ui->graphicsView->chart()->setTheme(QChart::ChartThemeDark);
}

void FChartForm::Slot_Hopdata(bool isscancurrentdata, bool isappointdata, QVector<double> appointdata)
{
    _cfp.hp.isscancurrentdata = isscancurrentdata;
    _cfp.hp.isappointdata = isappointdata;
    _cfp.hp.appointdatagroup = appointdata;
}

void FChartForm::Slot_Hopdata(bool isscancurrentdata, bool isappointdata, double maxval, double minval)
{
    _cfp.hp.isscancurrentdata = isscancurrentdata;
    _cfp.hp.isappointdata = isappointdata;
    _cfp.hp.maxval = maxval;
    _cfp.hp.minval = minval;
}

void FChartForm::Slot_Querytext(QString querytype, QString queryvalue)
{

    //确定查询类型
    int columno =0;
    if(querytype == "时间")
    {
        columno = 0;
    }
    else if(querytype == "当值")
    {
        columno = 1;
    }
    else if(querytype == "前值")
    {
        columno = 2;
    }
    else if(querytype == "差值")
    {
        columno = 3;
    }
    else if(querytype == "折线")
    {
        columno = 4;
    }

    //在查询类型中寻找
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem* it = ui->tableWidget->item(i,columno);
        if(queryvalue == it->text())
        {
            ui->tableWidget->setCurrentItem(it);
        }
    }
}

void FChartForm::mousePressEvent(QMouseEvent *mouse)
{
    if(mouse->button() == Qt::RightButton)
    {
        pChartPopupMenu->exec(QCursor::pos());
    }
}

void FChartForm::Slot_Draw(QDateTime min, QDateTime max, qint64 curdt, double val, Seriesno no)
{
    if(_cfp.IsAxisTimeForm_X == true)
    {
        ax_time.setRange(min, max);
        LineSeriesList.at(no)->lineseries->append(curdt,val);

    }
    else
    {
            if(count > (tickcount/2))
                ax_value.setRange(ax_value.min(),ax_value.max()+1);
        LineSeriesList.at(no)->lineseries->append(count,val);
    }
}

void FChartForm::CreateHop(QDateTime curdt, double val, Seriesno no)
{
    //时间
    QTableWidgetItem *it_date = new QTableWidgetItem;
    it_date->setText(curdt.toString("HH:mm:ss:zzz"));
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,it_date);
    //当值
    QTableWidgetItem *it_cur = new QTableWidgetItem;
    it_cur->setText(QString::number(val));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,it_cur);
    //前值
    QTableWidgetItem *it_pre = new QTableWidgetItem;
    QLineSeries *series = LineSeriesList.at(no)->lineseries;
    double pre;
    if(series->count() == 0)
    {
        it_pre->setText("无");
    }
    else
    {
        pre =series->at(series->count()-1).y();
        it_pre->setText(QString::number(pre));
    }
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,it_pre);
    //差值
    QTableWidgetItem *it_minus = new QTableWidgetItem;
    it_minus->setText(QString::number(val-pre));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,it_minus);
    //折线
    QTableWidgetItem *it_no = new QTableWidgetItem;
    it_no->setText(QString::number(no));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,4,it_no);

    //设置table滚动条到最底部
    ui->tableWidget->scrollToBottom();
}

void FChartForm::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left)
    {
        emit Signal_Keyleft();
    }
    else if(event->key() == Qt::Key_Right)
    {
        emit Signal_Keyright();
    }
    else if(event->key() == Qt::Key_Up)
    {
        emit Signal_Keyup();
    }
    else if(event->key() == Qt::Key_Down)
    {
        emit Signal_Keydown();
    }
    event->accept();
}

void FChartForm::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y()>0)
    {
        emit Signal_Keyup();
    }
    else
    {
        emit Signal_Keydown();
    }
}


void FChartForm::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    QDateTime cur = QDateTime::currentDateTime();
    qDebug() << "curtime" << cur;
    qint64 xmin = ax_time.min().toMSecsSinceEpoch() ;
    qint64 xmax = ax_time.max().toMSecsSinceEpoch() ;
    qint64 dist = xmax - xmin;

    qDebug() << "xmin:"<< xmin << "xmax:" << xmax << "dist:" <<dist;
    qDebug() << "itmetext:" <<  ui->tableWidget->item(item->row(),0)->text();

    QDate date;
    QDateTime xmintime =QDateTime::fromString(ui->tableWidget->item(item->row(),0)->text(), "HH:mm:ss:zzz");
    xmintime = xmintime.addYears(cur.date().year());
    date.setDate(cur.date().year(),cur.date().month(),cur.date().day());
    xmintime.setDate(date);
    qDebug() << "xmintime:" << xmintime;

    xmin = xmintime.toMSecsSinceEpoch();
    xmax = xmin+dist;

    ax_time.setRange(QDateTime::fromMSecsSinceEpoch(xmin),QDateTime::fromMSecsSinceEpoch(xmax));
    qDebug() <<"xmin:"<<QDateTime::fromMSecsSinceEpoch(xmin) << "xmax:" <<QDateTime::fromMSecsSinceEpoch(xmax);
}

