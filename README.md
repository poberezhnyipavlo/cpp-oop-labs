# cpp-oop-labs

A series of small, self-contained C++20 labs on objects, ownership and resource management.

> **Note:** This is a personal learning repository — a hands-on refresher of C++ OOP.
> The code is written for practice and experimentation (some experiments contain UB on purpose),
> not for reuse as a library.

I'm a backend developer with 6+ years of PHP/Laravel, moving into C++ (embedded / networking). In PHP an object
variable is a handle and the garbage collector cleans up; in C++ the variable *is* the object, copying copies the
object, and destruction happens at a precisely known point. These labs work through what follows from that:
who owns a resource, when to copy and when to move, when something is destroyed, and what happens when a
reference outlives its object.

Each lab is driven by experiments rather than theory: predict the output, run it, then break the code on purpose
and read what the compiler or AddressSanitizer says.

## Labs

| Lab | Topic | What it covers |
|---|---|---|
| [oop-01](oop-01-lifetime) | Object lifetime | stack, heap, member, local `static`; destruction order; `new`/`delete` pitfalls (leak, double free, use-after-free) |
| [oop-02](oop-02-pointers-references) | `*`, `&`, `&&` | pointer vs reference, binding rules for `T&` / `const T&` / `T&&`, `const` with pointers, what each parameter style copies |
| [oop-03](oop-03-constructors) | Constructors | init list vs assignment, member init order (`-Wreorder`), delegation, `explicit`, `{}` vs `()` |
| [oop-04](oop-04-static) | `static` | static members and methods, function-local statics, internal linkage, static initialization order fiasco |
| [oop-05](oop-05-encapsulation) | Encapsulation and `const` | `const` methods, `mutable`, `friend`, keeping invariants in one place |
| [oop-06](oop-06-copy) | Copying | copy ctor vs assignment, shallow copy → double free, self-assignment, copy-and-swap, `= delete` |
| [oop-07](oop-07-move) | Moving | move ctor/assignment, `std::move` is a cast, `std::exchange`, `noexcept` and `std::vector` growth, RVO/NRVO |
| [oop-08](oop-08-raii) | RAII, rule of 0/3/5 | wrappers over `FILE*` and sockets, exceptions in constructors, why destructors don't throw |
| [oop-09](oop-09-smart-pointers) | Smart pointers | `unique_ptr` with custom deleters, `shared_ptr` control block, `weak_ptr`, reference cycles, ownership in signatures |
| [oop-10](oop-10-inheritance) | Inheritance | construction order, base constructor calls, slicing, `public`/`protected`/`private` inheritance, inheriting constructors |
| [oop-11](oop-11-polymorphism) | Polymorphism | `virtual`, `override`, `final`, interfaces, virtual destructor, `dynamic_cast`, vptr cost, diamond, NVI |
| [oop-12](oop-12-overloading) | Overloading | overload resolution and its traps, overload vs override vs hiding, operators, `<=>`, `= delete` overloads, user-defined literals |
| [oop-13](oop-13-dangling) | Dangling references and views | temporaries and lifetime extension, `string_view` from a temporary, iterator invalidation, reading ASan reports |
| [oop-14](oop-14-lambdas) | Lambdas | closures as classes, capture modes, `mutable`, `std::function`, `std::jthread` argument copying, `[this]` in event subscriptions |
| [oop-15](oop-15-templates) | Templates vs virtual functions | instantiation, why templates live in headers, concepts, `std::variant` + `std::visit`, a fixed-size `RingBuffer<T, N>` |
| [oop-16](oop-16-lmdb-raii) | RAII over a C API | LMDB env/transaction/cursor wrappers, typed errors, move-only handles, `unique_ptr` with custom deleters, atomic transfers, GoogleTest tests |

## Layout

Every lab is an independent CMake project (open the folder on its own in CLion or build it from the terminal):

```
oop-NN-topic/
├── CMakeLists.txt       # C++20, -Wall -Wextra -Werror, option SANITIZE
├── CMakePresets.json    # presets: debug, asan
├── *.h / *.cpp          # classes for the lab
├── main.cpp             # experiments: ./oop-NN <experiment>
└── homework.cpp         # tasks written from a text description, no template code
```

Some classes travel between labs and grow with them (for example `Tracer`, which logs construction, copies, moves
and destruction, so you can see exactly when an object is copied or destroyed).

## Build and run

```bash
cd oop-14-lambdas
cmake --preset debug && cmake --build --preset debug
./build/debug/oop-14                    # no argument: prints the list of experiments
./build/debug/oop-14 capture-modes      # run one experiment

cmake --preset asan && cmake --build --preset asan
./build/asan/oop-14 dangling-capture    # same experiment under AddressSanitizer + UBSan
./build/asan/oop-14-homework
```

Some experiments contain undefined behaviour **on purpose** (dangling references, use-after-free) to show how it
looks with and without sanitizers — for example `dangling-capture` in oop-14 or the dangling experiments in oop-13.

## Tooling

- C++20, CMake presets, `-Wall -Wextra -Werror`
- AddressSanitizer + UndefinedBehaviorSanitizer (`asan` preset)
- `clang-format` (Google style, 4 spaces, 100 columns) and `clang-tidy` (`readability-identifier-naming`,
  `misc-include-cleaner`) from the repository root
- Checked with GCC and Clang on Linux and Apple Clang on macOS

```bash
clang-format --dry-run --Werror oop-*/*.h oop-*/*.cpp
clang-tidy -p oop-15-templates/build/debug oop-15-templates/*.cpp
```

### macOS notes

- LeakSanitizer is not supported by Apple Clang on Apple Silicon; leaks are checked with
  `leaks --atExit -- ./build/debug/oop-NN <experiment>` (debug build) or with the `Tracer::alive()` counter.
- GCC and Clang catch different things (`-Wdangling-reference` vs `-Wdangling-gsl`, non-virtual destructor
  deletion), and undefined behaviour often prints something different on macOS (an empty string instead of
  garbage). The point is not the exact output but finding the bug with the sanitizer.
