#include "spdiagnosticsservice.h"

#include <QtDebug>
#include <QRegularExpression>

void SPDiagnosticsServiceDelegate::diagnosticsServiceDidGetProcessOutput(SPDiagnosticsService *diagnosticsService, QString processOutput) {

    qDebug() << "SPDiagnosticsServiceDelegate diagnosticsServiceDidGetProcessOutput call";

}

void SPDiagnosticsServiceDelegate::diagnosticsServiceDidFinishWithIssuesList(SPDiagnosticsService *diagnosticsService, shared_ptr<SPDiagnosticIssuesList> diagnosticIssuesList) {

    qDebug() << "SPDiagnosticsServiceDelegate diagnosticsServiceDidFinishWithIssuesList call";

}

SPDiagnosticsService::SPDiagnosticsService() {

    projectBuilderService = make_unique<SPProjectBuilderService>();
    projectBuilderService->delegate = this;
}

void SPDiagnosticsService::projectServiceDidGetProcessOutput(SPProjectBuilderService *projectService, QString processOutput) {

    buildOutput += processOutput;

    delegate->diagnosticsServiceDidGetProcessOutput(this, processOutput);

}

void SPDiagnosticsService::projectServiceDidFinishPerformance(SPProjectBuilderService *projectService) {

    qDebug() << "SPDiagnosticsService::projectServiceDidFinishPerformance";

    auto diagnosticIssuesList = make_shared<SPDiagnosticIssuesList>();

    {
        auto regexp = QRegularExpression(".*.cpp:[1-9]*:[1-9]*: warning: .*");

        auto matchIterator = regexp.globalMatch(buildOutput);

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            qDebug() << match;

            auto diagnosticIssueMessage = make_shared<string>(match.captured().toUtf8());

            auto diagnosticIssue = make_shared<SPDiagnosticIssue>(diagnosticIssueMessage, SPDiagnosticIssueTypeUnusedParameter);

            diagnosticIssuesList->add(diagnosticIssue);

        }
    }

    delegate->diagnosticsServiceDidFinishWithIssuesList(this, diagnosticIssuesList);

}

void SPDiagnosticsService::setProject(shared_ptr<SPProject> project) {

    project = project;
    projectBuilderService->project = project;

}

void SPDiagnosticsService::performDiagnostics() {

    buildOutput = "";

    projectBuilderService->cleanAndBuild();

    //TODO: grab compiler output, regexp it and return issues

}
