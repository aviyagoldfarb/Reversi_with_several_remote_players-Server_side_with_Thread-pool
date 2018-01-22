//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX7_SERVER_TASK_H
#define EX7_SERVER_TASK_H

class Task {
public:
    Task(void * (*func)(void *arg), void* arg) : func(func), arg(arg) {}

    void execute() {
        func(arg);
    }

    virtual ~Task() {}

private:
    void * (*func)(void *arg);
    void *arg;
};

#endif //EX7_SERVER_TASK_H
