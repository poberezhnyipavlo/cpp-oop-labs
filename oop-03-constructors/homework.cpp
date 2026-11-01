#include <iostream>

#include "Led.h"

void ledConstructors() {
    std::cout << "ledConstructors" << std::endl;

    Led a{1, "all parameters", 150};
    a.print();

    Led b{2, "two parameters"};
    b.print();

    Led c{3};
    c.print();
}

void blink(const Led& led) {
    std::cout << "blink<" << led.name() << ">" << std::endl;
}

void implicitConversion() {
    std::cout << "implicitConversion" << std::endl;
    Led status{13, "status", 50};
    // Без explicit на Led(int pin) рядок blink(13) компілюється: компілятор мовчки
    // створює тимчасовий Led{13} і передає його в blink замість status.
    // Вивід без explicit:
    //       [ctor] status
    //       [Led 2 parameters] pin13
    //       [Led 1 parameter] pin13
    // blink<pin13>
    //       [dtor] pin13
    //       [dtor] status
    // Блимнув не status, а новий світлодіод на тому ж піні, який одразу зник.
    // З explicit blink(13) — помилка компіляції, тому передаємо сам об'єкт:
    blink(status);
}

int main() {
    // ledConstructors();

    implicitConversion();
    return 0;
}
