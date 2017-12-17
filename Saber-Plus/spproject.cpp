#include "spproject.h"

#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>

SPProject::SPProject(shared_ptr<string> name, shared_ptr<string> projectWorkingDirectoryPath)
{
    this->name = name;
    this->projectWorkingDirectoryPath = projectWorkingDirectoryPath;
}

void SPProject::serialize(shared_ptr<string> path)
{
    QJsonObject dictionary;

    dictionary["name"] = QString(this->name->c_str());

    QJsonDocument document(dictionary);

    QByteArray bytes = document.toJson();

    QFile projectFile(QString(path->c_str()));
    projectFile.open(QIODevice::WriteOnly | QIODevice::Text);
    projectFile.write(bytes);
    projectFile.close();
}

void SPProject::deserialize(shared_ptr<string> path)
{
    QFile projectFile(path->c_str());
    projectFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray bytes = projectFile.readAll();

    projectFile.close();

    auto dictionary = QJsonDocument::fromJson(bytes).object();

    auto jsonName = dictionary["name"].toString();

    name = make_shared<string>(jsonName.toUtf8());
    projectWorkingDirectoryPath = make_shared<string>(QFileInfo(QString(path->c_str())).absolutePath().toUtf8());
}
