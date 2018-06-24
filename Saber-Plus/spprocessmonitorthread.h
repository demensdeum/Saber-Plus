#ifndef SPPROCESSMONITORTHREAD_H
#define SPPROCESSMONITORTHREAD_H

#include <QThread>


class SPProcessMonitorThread : public QThread
{
public:
    virtual void run();
    long long pid = -1;
};

#endif // SPPROCESSMONITORTHREAD_H
