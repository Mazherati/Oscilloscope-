#include "mythread.h"
#include <QtCore>

MyThread::MyThread()
{

}
void MyThread::run()
{
    QMutex mutex;
    mutex.lock();


}
