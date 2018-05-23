#include "spdebugger.h"

#include "spprojectbuilderservice.h"

#include <QDebug>
#include <QRegularExpression>

void SPDebuggerDelegate::debuggerDidGetProcessOutput(SPDebugger *debugger, QString processOutput) {

    qDebug() << "Unused SPDebuggerDelegate debuggerDidGetProcessOutput call " << debugger << " ; " << processOutput;

}

void SPDebuggerDelegate::debuggerDidGetProcessStackNodes(SPDebugger *debugger, shared_ptr<SPList<SPStackNode> > stackNodesList) {

    qDebug() << "Unused SPDebuggerDelegate debuggerDidGetProcessStackNodes call " << debugger << " ; " << stackNodesList.get();

}

void SPDebuggerDelegate::debuggerDidGetProcessVariableNodes(SPDebugger *debugger, shared_ptr<SPList<SPVariableNode> > variableNodesList) {

    qDebug() << "Unused SPDebuggerDelegate debuggerDidGetProcessVariableNodes call " << debugger << " ; " << variableNodesList.get();

}

void SPDebuggerDelegate::debbugerDidGetProcessVariableChildNodes(SPDebugger *debugger, shared_ptr<SPVariableNode> variableNode, shared_ptr<SPList<SPVariableNode> > variableNodesList) {

    qDebug() << "Unused SPDebuggerDelegate debbugerDidGetProcessVariableChildNodes call " << debugger << " ; " << variableNode.get() <<  " ; " << variableNodesList.get();

}

SPDebugger::SPDebugger(QObject *parent) : QObject(parent) {

    process = nullptr;

}

void SPDebugger::sendCommand(shared_ptr<string> command) {

    if (process == nullptr) {

        return;

    }

    if (command.get() == nullptr) {

        return;

    }

    process->write(command->c_str());
    process->write("\n");

}

void SPDebugger::printVariables() {

    if (process == nullptr) {

        return;

    }

    printVariableNode = shared_ptr<SPVariableNode>();

    state = kSPDebuggerVariablesPrintState;

    process->write("fr var --show-types --ptr-depth=0\n");
}

void SPDebugger::stepIn() {

    process->write("thread step-in\n");

    state = kSPDebuggerStackNoneState;
}

void SPDebugger::stepOut() {

    process->write("thread step-out\n");

    state = kSPDebuggerStackNoneState;
}

void SPDebugger::stepOver() {

    process->write("thread step-over\n");

    state = kSPDebuggerStackNoneState;
}

void SPDebugger::continueProcess() {

    process->write("thread continue\n");

    state = kSPDebuggerStackNoneState;
}

void SPDebugger::printStack() {

    state = kSPDebuggerStackPrintState;

    if (process == nullptr) {

        return;

    }

    process->write("thread backtrace\n");

}

void SPDebugger::start() {

    if (SPProjectBuilderService::resolveProjectExecutableIfNeeded(project) == false) {

        return;

    }

    process = new QProcess();
    process->setWorkingDirectory(QString(project->projectProcessWorkingDirectoryPath->c_str()));
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

    for (auto item : project->pathToFilesMap) {

        QString filePath = QString(item.first.c_str());

        QString fileName = filePath.split("/").last();

        auto file = item.second;

        for (auto breakpointItem : file->linesToBreakpointsMap) {

            QString breakpointLine = QString::number(breakpointItem.first);

            QString breakpointCommand = "breakpoint set --file " + fileName + " --line  " + breakpointLine + "\n";

            process->write(breakpointCommand.toUtf8());
        }

    }

    process->write("run\n");

}

void SPDebugger::kill() {

    if (process == nullptr) {

        return;

    }

    process->kill();
    process->deleteLater();

    process = nullptr;
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

    auto outputString = make_shared<string>(output.constData());

    switch (state) {

        case kSPDebuggerStackPrintState:

            handleStackPrintOutput(outputString);

            break;

        case kSPDebuggerVariablesPrintState:

            handleVariablesPrintOutput(outputString);

            break;

        case kSPDebuggerVariablePrintState:

            this->handleVariablePrintOutput(outputString);

            break;

        default:

            break;
    }
}

void SPDebugger::handleStackPrintOutput(shared_ptr<string> output) {

    qDebug() << "handleStackPrintOutput: " << QString(output->c_str());

    auto outputString = QString(output->c_str());

    auto regexp = QRegularExpression("([a-zA-Z]*\.cpp):([0-9]*)\n");

    auto matchIterator = regexp.globalMatch(outputString);

    auto stackNodesList = make_shared<SPList<SPStackNode> >();

    while (matchIterator.hasNext()) {

        auto match = matchIterator.next();

        auto filePath = make_shared<string>(match.captured(1).toUtf8());
        auto line = atoi(match.captured(2).toUtf8());

        auto stackNode = make_shared<SPStackNode>(filePath, line);
        stackNodesList->add(stackNode);
    }

    delegate->debuggerDidGetProcessStackNodes(this, stackNodesList);
}

void SPDebugger::handleVariablePrintOutput(shared_ptr<string> output) {

    qDebug() << "handleVariablePrintOutput: " << QString(output->c_str());

    this->handleVariablesPrintOutput(output);
}

void SPDebugger::revertVariableNodesAfterError() {

    variableNodesList = previousVariableNodesList;

    delegate->debuggerDidGetProcessVariableNodes(this, variableNodesList);

}

void SPDebugger::handleVariablesPrintOutput(shared_ptr<string> output) {

    qDebug() << "handleVariablesPrintOutput: " << QString(output->c_str());

    auto outputString = QString(output->c_str());

    if (outputString.contains("error: "))
    {
        revertVariableNodesAfterError();
        return;
    }

    auto regexp = QRegularExpression("\(.*\) (.*) = (.*)");

    auto matchIterator = regexp.globalMatch(outputString);

    auto variableNodesList = make_shared<SPList<SPVariableNode> >();

    if (state == kSPDebuggerVariablePrintState)
    {
        if (printVariableNode->parent.get() != nullptr)
        {
            variableNodesList->add(printVariableNode->parent);
        }
        else
        {
            auto classIdentifier = make_shared<string>("root node class identifier");
            auto name = make_shared<string>("/");
            auto value = make_shared<string>("(go to root)");
            auto postfix = make_shared<string>("[print root]");

            auto variableNode = make_shared<SPVariableNode>(classIdentifier, name, value, postfix);
            variableNodesList->add(variableNode);
        }
    }

    while (matchIterator.hasNext()) {

        auto match = matchIterator.next();

        auto classIdentifier = make_shared<string>(match.captured(1).toUtf8());
        auto name = make_shared<string>(match.captured(2).toUtf8());
        auto value = make_shared<string>(match.captured(3).toUtf8());
        auto postfix = make_shared<string>("->");

        if (classIdentifier->find("vector")  != string::npos)
        {
            postfix = make_shared<string>("");
        }
        else if (classIdentifier->find("map")  != string::npos)
        {
            postfix = make_shared<string>("");
        }
        else if (classIdentifier->find("shared_ptr") != string::npos)
        {
            postfix = make_shared<string>("._M_ptr->");
        }
        else
        {
            postfix = make_shared<string>(".");
        }


        auto variableNode = make_shared<SPVariableNode>(classIdentifier, name, value, postfix);
        variableNode->parent = printVariableNode;
        variableNodesList->add(variableNode);
    }

    this->variableNodesList = variableNodesList;

    delegate->debuggerDidGetProcessVariableNodes(this, variableNodesList);
}

void SPDebugger::printVariable(shared_ptr<SPVariableNode> variableNode) {

    if (string(variableNode->name->c_str()) == "/") {

        this->printVariables();
        return;

    }

    previousVariableNodesList = variableNodesList;

    state = kSPDebuggerVariablePrintState;

    printVariableNode = variableNode;

    QString printVariableCommand = "fr var --show-types " + QString(variableNode->fullPath()->c_str()) + " --ptr-depth=1\n";

    process->write(printVariableCommand.toUtf8());

}

void SPDebugger::stateChanged(QProcess::ProcessState newState) {

}
