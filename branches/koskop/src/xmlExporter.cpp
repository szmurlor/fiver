#include "xmlExporter.h"

bool XmlExporter::beginDocument(QString fileName)
{
    file = new QFile(fileName);
    if (!file->open(QFile::WriteOnly | QFile::Text))
    {
        file->close();
        return false;
    }

    xmlWriter = new QXmlStreamWriter(file);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    return true;
}

void XmlExporter::endDocument()
{
    xmlWriter->writeEndDocument();
    delete xmlWriter;
    xmlWriter = NULL;
    file->close();
    delete file;
}

void XmlExporter::beginElement(QString type)
{
    xmlWriter->writeStartElement(type);
}

void XmlExporter::endElement()
{
    xmlWriter->writeEndElement();
}

void XmlExporter::addAttribute(QString name, QString value)
{
    xmlWriter->writeAttribute(name, value);
}
