#pragma once

#include "Tracer.h"

// Глобальний об'єкт: створюється до main, але порядок відносно глобальних в інших .cpp не визначений.
extern Tracer g_main_board;

// Локальна static: створюється при першому виклику функції.
const Tracer& spareBoard();
