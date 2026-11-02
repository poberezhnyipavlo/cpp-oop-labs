#include "Boards.h"

#include "Tracer.h"

Tracer g_main_board{"main-board"};

const Tracer& spareBoard() {
    static const Tracer kBoard{"spare-board"};
    return kBoard;
}
