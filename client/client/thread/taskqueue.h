#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <QObject>
#include "cards.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

struct Task{
    int bet=0;
    Cards cs;
};

class TaskQueue : public QObject
{
    Q_OBJECT
public:
    TaskQueue(const TaskQueue&)=delete;
    TaskQueue& operator=(const TaskQueue&)=delete;
    TaskQueue(const TaskQueue&&)=delete;
    TaskQueue& operator=(const TaskQueue&&)=delete;
    static TaskQueue* getInstance();

    void add(Task& t);
    Task take();
    //返回队列中任务数量
    int size();
    //清空队列
    void clear();
private:
    explicit TaskQueue(QObject *parent = nullptr);

    static TaskQueue m_taskQ;
    QQueue<Task> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;
signals:

};

#endif // TASKQUEUE_H
