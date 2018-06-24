#include "spprocessmonitorthread.h"
#include <QProcess>
#include <QDebug>

void SPProcessMonitorThread::run()
{
    while(true)
    {
        sleep(5);

        auto process = new QProcess();

        char buffer[1024];
        sprintf(buffer, "pmap -d %lld", pid);

        QString processString = QString(buffer);

        process->start(processString);
        process->waitForFinished();

        QString output = process->readAllStandardOutput();
        auto splitted = output.split("mapped: ");

        if (splitted.length() > 1) {

            qDebug() << "mapped: " << splitted[1];
        }
        else
        {
            break;

            qDebug() << "SPProcessMonitorThread commited sudoku" << endl;

            delete this;
        }
    }
}

