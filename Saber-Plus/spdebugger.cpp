#include "spdebugger.h"

#include "spprojectservice.h"

#include <QDebug>

void SPDebuggerDelegate::debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput) {

    qDebug() << "Unused SPDebuggerDelegate debuggerDidGetProcessOutput call "<< debugger << " ; " << processOutput;

}

SPDebugger::SPDebugger(QObject *parent) : QObject(parent) {

}

void SPDebugger::start() {

    if (SPProjectService::resolveProjectExecutableIfNeeded(project) == false) {

        return;

    }

    process = new QProcess();
    process->setWorkingDirectory(QString(project->projectWorkingDirectoryPath->c_str()));
    process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(process, &QProcess::readyReadStandardOutput, this, &SPDebugger::readyReadStandardOutput);
    QObject::connect(process, &QProcess::stateChanged, this, &SPDebugger::stateChanged);

    auto runFilePath = QString(project->projectExecutablePath->c_str());

    process->start("lldb " + runFilePath);

}

void SPDebugger::run() {

    if (process == nullptr) {

        return;

    }

    process->write("run\n");

}

void SPDebugger::kill() {

    if (process == nullptr) {

        return;

    }

    process->kill();

}

void SPDebugger::toogleBreakpointForFilePathAtLine(QString filePath, int line) {

    if (filePath.startsWith("/")) {

        qDebug() << "SPDebugger::toogleBreakpointForFilePathAtLine incorrect path: " << filePath << "; only relative path supported.";

        return;

    }

    auto filePathString = make_shared<string>(filePath.toUtf8());

    auto file = project->fileAtPath(filePathString);

    if (file.get() == nullptr) {

        file = project->makeFileWithPath(filePathString);

    }

    if (file.get() == nullptr) {

        return;

    }

    file->toggleBreakpointAtLine(line);

}

void SPDebugger::readyReadStandardOutput() {

    auto output = process->readAllStandardOutput();

    delegate->debuggerDidGetProcessOutput(this, output);

}

void SPDebugger::stateChanged(QProcess::ProcessState newState) {

}
