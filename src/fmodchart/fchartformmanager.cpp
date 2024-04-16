#include "fmodchart/fchartformmanager.h"

namespace
{
    FChartFormManager *m_instance=nullptr;
}

FChartFormManager::FChartFormManager()
{
    CreatechartFormDialog::Getinstances();
    connect(CreatechartFormDialog::Getinstances(),&CreatechartFormDialog::Signal_CreatechartForm,this,&FChartFormManager::Slot_CreateChartForm);
}

FChartFormManager::~FChartFormManager()
{
    //UnInit();
}

FChartFormManager *FChartFormManager::GetInstance()
{
    if(m_instance == nullptr)
    {
        m_instance = new FChartFormManager();
    }

    return m_instance;
}

void FChartFormManager::ReleaseInstance()
{
    if(m_instance != nullptr)
    {
       delete m_instance;
    }
    m_instance = nullptr;
}

void FChartFormManager::Init(const QVector<FChartFormParam> &arr)
{
    FChartForm *pcf;
    foreach (const FChartFormParam &cfp, arr)
    {
        pcf = new FChartForm(cfp);
        pcf->setWindowFlags(Qt::Window);
        pcf->setGeometry(cfp.x, cfp.y, cfp.w, cfp.h);
        pcf->SetTitle(cfp.title);
        pcf->Start(cfp.mcp, cfp.XRangeMultiple);
        pcf->show();
        ChartFormArray.append(pcf);
    }

    ChartFormPool.setMaxThreadCount(arr.size());
}

void FChartFormManager::UnInit()
{
    ChartFormPool.clear();

    Clear();
}

void FChartFormManager::Clear()
{
    ArrayMutex.lock();
    Stop = true;

    foreach(FChartForm *pcf, ChartFormArray)
    {
        delete pcf;
    }
    //qDebug("22222222222222222");
    ChartFormArray.clear();
    //qDebug("111111111111111");
    ArrayMutex.unlock();
}

double FChartFormManager::splitdata_x16(char *datagram, Bitno no, Typebyte type, bool isunsigned, int datalong)
{
    double res = 0.0;
    char* ptr =datagram;
    QVector<long> datalist;
    if(no <= datalong)
    {
        switch(type)
        {
        case ONE_BYTE:
            ptr +=no;
            if(isunsigned)
            {
                quint8 udatatemp = *((quint8*)ptr);
                res = udatatemp;
            }
            else
            {
                qint8 datatemp = *((qint8*)ptr);
                res = datatemp;
            }
            break;
        case TWO_BYTE:
            ptr +=(2*no);
            if(isunsigned)
            {
                quint16 udatatemp = *((quint16*)ptr);
                res = udatatemp;
            }
            else
            {
                qint16 datatemp = *((qint16*)ptr);
                res = datatemp;
            }
            break;
        case FORE_BYTE:
            ptr +=(4*no);
            if(isunsigned)
            {
                quint64 udatatemp = *((quint64*)ptr);
                res = udatatemp;
            }
            else
            {
                qint64 datatemp = *((qint64*)ptr);
                res = datatemp;
            }
            break;
        case EIGHT_BYTE:
            //不可用
            ptr +=(4*no);
            if(isunsigned)
            {
                quint64 udatatemp = *((quint64*)ptr);
                res = udatatemp;
            }
            else
            {
                qint64 datatemp = *((qint64*)ptr);
                res = datatemp;
            }
            break;
        }
    }
    return res;
}

void FChartFormManager::Slot_CreateChartForm(const FChartFormParam &cfp, Bitno no)
{
    qDebug() << "[FChartFormManager]Slot_CreateChartForm running";
    FChartForm* fcf = new FChartForm(cfp);
    fcf->setGeometry(cfp.x, cfp.y, cfp.w, cfp.h);
    fcf->SetTitle(cfp.title);
    fcf->InitSeries(no);
    //fcf->Start(cfp.mcp, cfp.XRangeMultiple);
    fcf->show();
    ChartFormArray.append(fcf);
    ChartFormPool.setMaxThreadCount(ChartFormPool.maxThreadCount()+1);
}

void FChartFormManager::OnTime(const QVector<double> &arr)
{
    ArrayMutex.lock();
    if (! Stop)
    {
        int c =0;
        for(int j =0;j<ChartFormArray.size();j++)
        {
            FChartForm* fcf = ChartFormArray.at(j);
            QVector<Bitno> bitnos =fcf->GetseriesBitnos();
            for(int i=0;i<bitnos.size();i++)
            {
                fcf ->OnTime(QDateTime::currentDateTime(),arr.at(c),i);
                c++;
                if(c>arr.size())
                {
                    qDebug() << "[FChartFormManager]OnTime: Dataarr error!";
                    ArrayMutex.unlock();
                    return;
                }
            }
        }
//        if (arr.size() == ChartFormArray.size())
//        {
//            FChartForm *pcf;
//            for(int i=0; i<ChartFormArray.size(); i++)
//            {
//                pcf = ChartFormArray.at(i);
//                pcf->SetChartData(QDateTime::currentDateTime(), arr[i]);
//                ChartFormPool.start(pcf);
//            }
//        }
    }
    ArrayMutex.unlock();
}

void FChartFormManager::dealdata(char*datagram)
{
    QVector<double> dealeddata;
    //提取每个窗口每条折线的数据
    foreach(FChartForm *ff,ChartFormArray)
    {    //获取每个窗口每条折线相应的比特位
        QVector<Bitno> bitnos;
        FChartFormParam param =ff->GetParam();
        bitnos = ff->GetseriesBitnos();
        foreach(Bitno no,bitnos)
        {
            //分离出value
            double value=splitdata_x16(datagram,no,param.byte,param.Isunsigned,param._datalong);
            dealeddata.append(value);
        }
    }
    //调用OnTime函数
    OnTime(dealeddata);
}

const QVector<FChartForm *> &FChartFormManager::GetFormArray()
{
    return ChartFormArray;
}

void FChartFormManager::StartDraw()
{
    Stop =false;
}

void FChartFormManager::StopDraw()
{
    Stop=true;
}

