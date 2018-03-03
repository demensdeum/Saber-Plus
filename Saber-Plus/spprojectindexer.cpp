#include "spprojectindexer.h"

#include <iostream>
#include <qregularexpression.h>
#include "spclass.h"

void SPProjectIndexerDelegate::projectIndexerDidFinishIndexing(SPProjectIndexer *projectIndexer)
{

}

SPProjectIndexer::SPProjectIndexer()
{
    delegate = nullptr;

    textSearchInFilesService = make_unique<SPTextSearchInFilesService>();
    textSearchInFilesService->delegate = this;
}

void SPProjectIndexer::textSearchInFilesServiceDidFinishWithSearchMatchesList(SPTextSearchInFilesService *textSearchInFilesService, shared_ptr<SPList<SPTextSearchInFilesMatch> > textSearchInFilesMatchesList)
{
    classNameToFileMap.clear();

    for (auto i = 0; i < textSearchInFilesMatchesList->count(); i++)
    {
        auto item = textSearchInFilesMatchesList->at(i);

        cout << item->match->c_str() << endl;

        auto match = QString(item->match->c_str());

        if (match.at(match.length() - 1) == ';')
        {
            continue;
        }

        auto regexp = QRegularExpression("class ([a-zA-Z]*)");

        auto matchIterator = regexp.globalMatch(match);

        while (matchIterator.hasNext()) {

            auto match = matchIterator.next();

            auto className = string(match.captured(1).toUtf8());
            auto filePath = make_shared<string>(item->filePath->c_str());

            auto file = make_shared<SPFile>();
            file->path = filePath;

            cout << className.c_str() << endl;
            cout << filePath->c_str() << endl;

            classNameToFileMap[className] = file;
        }
    }

    delegate->projectIndexerDidFinishIndexing(this);
}

shared_ptr<SPFile> SPProjectIndexer::fileFromClassName(shared_ptr<string> className)
{
    auto iterator = classNameToFileMap.find(string(className->c_str()));

    if (iterator != classNameToFileMap.end())
    {
        return classNameToFileMap[*className.get()];
    }

    return shared_ptr<SPFile>();
}

void SPProjectIndexer::index()
{
    auto regexp = "class [a-zA-Z]*";

    auto searchText = make_shared<string>(regexp);

    textSearchInFilesService->project = project;
    textSearchInFilesService->search(searchText);
}
