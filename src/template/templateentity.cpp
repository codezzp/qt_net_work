#include "templateentity.h"

templateEntity::templateEntity()
{

}

templateEntity::templateEntity(QString define, QList<struct locations> initLocation, QList<struct locations> templateLocation,
int check,struct check sumCheck,struct check CRCCheck,struct dataCount dataCount)
{
    this->define=define;
    this->initLocation=initLocation;
    this->templateLocation=templateLocation;
    this->check=check;
    this->sumCheck=sumCheck;
    this->CRCCheck=CRCCheck;
    this->dataCount=dataCount;
}

void templateEntity::setInitLocation( QList<struct locations> initLocation)
{
    this->initLocation=initLocation;
}
 QList<struct locations> templateEntity::getInitLocation()
{
    return this->initLocation;
}

void templateEntity::setTemplateLocation( QList<struct locations> templateLocation)
{
    this->templateLocation=templateLocation;
}

 QList<struct locations> templateEntity::getTemplateLocation()
{
    return this->templateLocation;
}

void templateEntity::setCheck(int check)
{
    this->check=check;
}

int templateEntity::getCheck()
{
    return this->check;
}

void templateEntity::setSumCheck(struct check sumCheck)
{
    this->sumCheck=sumCheck;
}

struct check templateEntity::getSumCheck()
{
    return this->sumCheck;
}

void templateEntity::setCRCCheck(struct check CRCCheck)
{
    this->CRCCheck=CRCCheck;
}

struct check templateEntity::getCRCCheck()
{
    return this->CRCCheck;
}

void templateEntity::setDataCount(struct dataCount dataCount)
{
    this->dataCount=dataCount;
}

struct dataCount templateEntity::getDataCount()
{
    return this->dataCount;
}

void templateEntity::setDefine(QString define)
{
    this->define=define;
}

QString templateEntity::getDefine()
{
    return define;
}
