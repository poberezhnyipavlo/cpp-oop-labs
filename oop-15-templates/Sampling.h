#pragma once

#include <concepts>
#include <iostream>
#include <string>

// Шаблон - рецепт для компілятора: тіло мусить бути в заголовку,
// шоб його бачив кожен .cpp, шо викликає.

// Без обмежень: підійде будь-який тип з name() i read().
// Перевіряється дише під час інстанціювання.
template <class SensorT>
void reportStatic(const SensorT& sensor) {
    std::cout << "      [template] " << sensor.name() << ": " << sensor.read() << "\n";
}

// Концепт - названа вимога до типу, перевіряється під час компіляції.
template <class T>
concept Readable = requires(const T& sensor){
    { sensor.name() } -> std::convertible_to<std::string>;
    { sensor.read() } -> std::convertible_to<double>;
};

// Скорочений запис шаблону (С++20): const Readable auto& == template <Readable T> ... const T&.
void reportReadable(const Readable auto& sensor) {
    std::cout << "      [concept] " << sensor.name() << ": " << sensor.read() << "\n";
}

// T виводиться з аргументів; усі три мають дати той самий T.
template <class T>
T clampReading(T value, T low, T high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

// Окрема функція (і окрема static-зміна) для кожного T, з яким її викликали.
template <class T>
void countCalls() {
    static int calls = 0;
    ++calls;
    std::cout << "  " << __PRETTY_FUNCTION__ << "- виклик " << calls << "\n";
}