#include "AppConfig.h"

#include <iostream>

AppConfig loadConfig() {
    std::cout << "[loadConfig]" << std::endl;

    return AppConfig{.device = "esp32", .port = 9003};
}

// Локальна static створюється при першому зверненні, тому її не зачіпає порядок ініціалізації
// глобальних об'єктів між файлами (як kEarlySensor у globals). Плюс конфіг вантажиться лише якщо потрібен.
const AppConfig& appConfig() {
    static const AppConfig config = loadConfig();
    return config;
}
