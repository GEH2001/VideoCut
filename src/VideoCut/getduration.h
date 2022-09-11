#ifndef GETDURATION_H
#define GETDURATION_H

#include <QString>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavdevice/avdevice.h"
}

class GetDuration
{
public:
    GetDuration();
    int getDura(const QString &strPath);
};

#endif // GETDURATION_H
