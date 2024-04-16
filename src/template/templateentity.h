#ifndef TEMPLATEENTITY_H
#define TEMPLATEENTITY_H

#include <QList>

struct locations{
    int beginLocation;
    int endLocation;
};

struct check{
    int check;
    int beginLocation;
    int endLocation;
    int location;
};

struct dataCount{
    int check;
    int location;
    int byteCount;
};

class templateEntity
{
public:
    templateEntity();
    templateEntity(QString define,QList<struct locations> initLocation,QList<struct locations> templateLocation,
    int check,struct check sumCheck,struct check CRCCheck,struct dataCount dataCount);
    void setInitLocation( QList<struct locations> initLocation);
    QList<struct locations> getInitLocation();
    void setTemplateLocation( QList<struct locations> templateLocation);
    QList<struct locations> getTemplateLocation();
    void setCheck(int check);
    int getCheck();
    void setSumCheck(struct check sumCheck);
    struct check getSumCheck();
    void setCRCCheck(struct check CRCCheck);
    struct check getCRCCheck();
    void setDataCount(struct dataCount dataCount);
    struct dataCount getDataCount();
    void setDefine(QString define);
    QString getDefine();

private:
    QString define="";
    QList<struct locations> initLocation;
    QList<struct locations> templateLocation;
    int check=-1;
    struct check sumCheck;
    struct check CRCCheck;
    struct dataCount dataCount;

};

#endif // TEMPLATEENTITY_H
