#ifndef HOLOGRAM_MONGOOSE_SERVER_H
#define HOLOGRAM_MONGOOSE_SERVER_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "mg/mongoose.h"
#include "settings.h"
#include <mutex>

void launch_server(int, char**, settings*, bool*, std::mutex*, std::mutex*, std::mutex*);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
