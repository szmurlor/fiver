#ifndef XMLEXPORTER_H
#define XMLEXPORTER_H

#include <QString>
#include <QFile>
#include <qxmlstream.h>



class XmlExporter
{
public:
    bool beginDocument(QString fileName);
    void endDocument();
    void beginElement(QString type);
    void endElement();
    void addAttribute(QString name, QString value);

private:
    QXmlStreamWriter* xmlWriter;
    QFile* file;
};

#endif // XMLEXPORTER_H
