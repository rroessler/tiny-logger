#include "tiny-logger.h"
using namespace tiny;

/// Logger Options
constexpr const char* PROMPT = " * tiny ({sev}) | ";
Logger::Options opts{PROMPT};

/// Loggable Derivation
class TestLoggable : public Loggable {
   public:
    std::string toString() const { return "\x1b[35m[This is a my internal value]\x1b[0m"; }
};

int main() {
    // ensure initialisation
    Logger::initialise(opts);

    // and do a simple log instance
    Logger::log(Logger::INFO, "Hello, World!");

    // can print all generic types as per std::cout allows
    Logger::log(Logger::WARNING, "@, @, @", 42, "'WOW!'", false);

    // can also print singular values without needing to format
    Logger::logValue(123.456);

    // derived loggable instances can also be logged simply
    TestLoggable test;
    Logger::log(Logger::FATAL, "Test value: @", test);

    return 0;
}