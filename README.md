*tiny-logger*
=============
A compact and simple C++ logger library.

Quick Start
-----------
The tiny-logger can be quickly implemented within a project by simply downloading the base header file `tiny-logger.h`. From here, the logger can be used as desired. For more information regarding the available API, see below.

Usage
-----
To begin using the tiny-logger, the initialisation method must be called.

```cpp

/// All options can be override as desired.
tiny::Logger::Options opts;
opts.prompt = "tiny";           // Optional prompt string.
opts.formatChar = '@';          // Optional format character (default is "@").

/// To set a prompt with severity details, add the "{sev}" substring to the prompt.
opts.prompt = "tiny-w-severity : {sev}"; // "{sev}" is replaced with the current severity.

/// This initialisation step is optional, but can be used to
/// change any options as required.
tiny::Logger::initialise(opts);

```

There are two available logging methods. These include severity logs and value logs. They can be implemented as such.

```cpp

/// Severity Log Examples.
tiny::Logger::log(tiny::Logger::WARNING, "Uh oh!");
tiny::Logger::log(tiny::Logger::INFO, "Value: @", 42);

/// Other severities include: FATAL, ERROR and TRACE.

/// Value Log Examples.
tiny::Logger::logValue("Hello", "World!"); // will concatenate with " " between values
tiny::Logger::logValue(30, true, '#', &var); // various generic types work

```

Advanced Usage
--------------
To streamline debug printing, the `Loggable` abstract class has also been created to aid in quickly logging class instances. By inheriting from the `Loggable` base class, and implementing the `toString` method, these class instances can now be logged with ease.

```cpp

class MyClass : public tiny::Loggable {
    public:
     std::string toString() const { return "Stringified Version!"; }
};

MyClass instance;
tiny::Logger::log(tiny::Logger::TRACE, "MyClass Value: @", instance);

```

In terms of development, it is useful to wrap the base logging method with macros to simplify choosing the appropriate severity. As such the following macros are available from tiny-logger to do just this.

```cpp

// Severity Wrappers
#define TL_FATAL(FMT, ...) ::tiny::Logger::log(::tiny::Logger::FATAL, FMT, ##__VA_ARGS__)
#define TL_ERROR(FMT, ...) ::tiny::Logger::log(::tiny::Logger::ERROR, FMT, ##__VA_ARGS__)
#define TL_WARNING(FMT, ...) ::tiny::Logger::log(::tiny::Logger::WARNING, FMT, ##__VA_ARGS__)
#define TL_INFO(FMT, ...) ::tiny::Logger::log(::tiny::Logger::INFO, FMT, ##__VA_ARGS__)
#define TL_TRACE(FMT, ...) ::tiny::Logger::log(::tiny::Logger::TRACE, FMT, ##__VA_ARGS__)

// Value Wrapper
#define TL_VALUE(...) ::tiny::Logger::logValue(__VA_ARGS__)

```

License
-------
[MIT](https://opensource.org/licenses/MIT)