#ifndef CMD_HANDLERS_H
#define CMD_HANDLERS_H

#include <StreamCommandParser.h>
#include "pid_globals.h"
#include "hardware_helper.h"
#include "pid_tuner_helper.h"

namespace cmd_handlers {

    void help(StreamCommandParser& commandParser);
    void servo(StreamCommandParser& commandParser);
    void dist(StreamCommandParser& commandParser);
    void v(StreamCommandParser& commandParser);
    void tune(StreamCommandParser& commandParser);
    void stop(StreamCommandParser& commandParser);
    void start(StreamCommandParser& commandParser);
    void set(StreamCommandParser& commandParser);
    void gpid(StreamCommandParser& commandParser);
    void spid(StreamCommandParser& commandParser);

}

#endif