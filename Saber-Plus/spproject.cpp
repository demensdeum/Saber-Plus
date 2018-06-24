#include "spproject.h"

#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>

SPProject::SPProject(shared_ptr<string> name, shared_ptr<string> projectWorkingDirectoryPath) {

    this->name = name;
    this->projectProcessWorkingDirectoryPath = projectWorkingDirectoryPath;

}

void SPProject::removeAllBreakpoints() {

    for (auto filePath : pathToFilesMap) {

        filePath.second->removeAllBreakpoints();

    }
}

shared_ptr<SPFile> SPProject::fileAtPath(shared_ptr<string> filePath) {

    auto filePathPointer = filePath.get();

    if (filePathPointer == nullptr) {

        return shared_ptr<SPFile>();

    }

    auto filePathString = *filePathPointer;

    auto isExists = pathToFilesMap.find(filePathString) != pathToFilesMap.end();

    if (isExists) {

        return this->pathToFilesMap[filePathString];

    }

    return shared_ptr<SPFile>();

}

shared_ptr<SPFile> SPProject::makeFileWithPath(shared_ptr<string> filePath) {

    auto filePathPointer = filePath.get();

    if (filePathPointer == nullptr) {

        return shared_ptr<SPFile>();

    }

    auto filePathString = *filePathPointer;

    auto file = make_shared<SPFile>();

    file->path = filePath;

    pathToFilesMap[filePathString] = file;

    return file;
}

void SPProject::serialize(shared_ptr<string> path) {

    QJsonObject dictionary;

    dictionary["name"] = QString(this->name->c_str());

    QJsonDocument document(dictionary);

    QByteArray bytes = document.toJson();

    QFile projectFile(QString(path->c_str()));
    projectFile.open(QIODevice::WriteOnly | QIODevice::Text);
    projectFile.write(bytes);
    projectFile.close();

}

void SPProject::deserialize(shared_ptr<string> path) {

    QFile projectFile(path->c_str());
    projectFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray bytes = projectFile.readAll();

    projectFile.close();

    auto dictionary = QJsonDocument::fromJson(bytes).object();

    auto jsonName = dictionary["name"].toString();

    name = make_shared<string>(jsonName.toUtf8());
    projectDirectoryPath = make_shared<string>(QFileInfo(QString(path->c_str())).absolutePath().toUtf8());

}
