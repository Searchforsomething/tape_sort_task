#ifndef TAPE_SORT_TASK_TAPEINTERFACE_H
#define TAPE_SORT_TASK_TAPEINTERFACE_H

#include <cstdint>
#include <string>

class ITape {
public:
    virtual ~ITape() = default;
    virtual int32_t read() = 0;
    virtual void write(int32_t value) = 0;
    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual size_t getPosition() = 0;
    virtual size_t getDataSize() = 0;
    virtual void restart() = 0;
};

#endif //TAPE_SORT_TASK_TAPEINTERFACE_H
