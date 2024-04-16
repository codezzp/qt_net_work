#include "icharform.h"
#include "fchartformmanager.h"
#include "dialog/createchartformdialog.h"

ICharForm::ICharForm()
{
    fcfm = new FChartFormManager();
}

ICharForm::~ICharForm()
{
    delete fcfm;
    fcfm = nullptr;
}

bool ICharForm::StartDraw()
{
    if(!_isStart)
    {
        fcfm->StartDraw();
        _isStart = true;
        return true;
    }
    return false;
}

bool ICharForm::StopDraw()
{
    if(_isStart)
    {
        fcfm->StopDraw();
        _isStart = false;
        return true;
    }
    return false;
}

bool ICharForm::DrawData(char *datagram)
{
    fcfm->dealdata(datagram);
    return true;
}

bool ICharForm::CreateChartForm(const FChartFormParam &cfp)
{
    CreatechartFormDialog::Getinstances()->ChangeChartFormParam(cfp);
    CreatechartFormDialog::Getinstances()->exec();
    return true;
}

bool ICharForm::UnInit()
{
    fcfm->UnInit();
    return true;
}
