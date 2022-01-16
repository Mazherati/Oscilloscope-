#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
public:
    explicit MyThread();
    void run();
    bool Stop();

signals:
    void nexttemp(double);


public slots:

};

#endif // MYTHREAD_H
