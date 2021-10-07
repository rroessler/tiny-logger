#ifndef TINY_LOGGER_H
#define TINY_LOGGER_H

/// C++ STL
#include <array>
#include <iostream>
#include <string>

/// Core Tiny Namespace.
namespace tiny {

    /*****************
     *  CORE LOGGER  *
     *****************/

    /// Base Logger Class. Can be static or instanced.
    class Logger {
       public:
        /// Base Logger Severities.
        typedef enum {
            FATAL = 0,
            ERROR = 1,
            WARNING = 2,
            INFO = 3,
            TRACE = 4,
        } Severity;

        /// Logger Options.
        struct Options {
            std::string prompt = "";
            char formatChar = '@';
        };

        /****************
         *  PROPERTIES  *
         ****************/

        /// Instanced Logger Options.
        Options options = {};

        /******************
         *  CONSTRUCTORS  *
         ******************/

        /**
         * Constructs a new instance of a logger with the current details.
         * @param opts                      Logger options.
         */
        static void initialise(const Options& opts = {"", '@'}) {
            /// assign the base options
            m_options = opts;
        }

        /*****************
         *  LOG METHODS  *
         *****************/

        /**
         * Core Logging method. Takes a base message format, and variable number of arguments
         * to print. Starts by printing the core prompt with severity given and then finishes with the desired message.
         * @param sev                           Log Severity.
         * @param fmt                           Message Format.
         * @param args                          Variable Message Arguments.
         */
        template <typename... Args>
        static void log(const Severity& sev, const std::string& fmt, Args&&... args) {
            // begin the logging output
            std::cout << m_preparePrompt(sev);

            // process all the arguments recursively
            std::string buffer = fmt;
            m_processArguments(buffer, std::forward<Args>(args)...);

            // complete the logged output by flushing
            std::cout << std::endl;
        }

        /**
         * Logs singular values with no prompt. This method helps to print numerous loggables
         * as desired.
         * @param initial                       Forced initial value to print.
         * @param args                          Optional additional values to print.
         */
        template <typename T, typename... Args>
        static void logValue(const T& initial, Args&&... args) {
            // grab the total number of arguments
            const size_t len = sizeof...(Args);

            // prepare a output string
            std::string format = "";
            for (size_t ii = 0; ii < len; ii++) format += "@ ";
            format += "@";  // cap off

            // and process the arguments
            m_processArguments(format, initial, std::forward<Args>(args)...);
            std::cout << std::endl;
        }

       private:
        /// Base Logger Severity Strings.
        static const std::array<std::string, 5> m_severityStrings;

        /// Core options.
        static Options m_options;

        /********************
         *  HELPER METHODS  *
         ********************/

        /**
         * Prepares the prompt with a given severity.
         * @param sev                           Severity to prepare a prompt with.
         */
        static std::string m_preparePrompt(const Severity& sev) {
            constexpr size_t REPLACE_LEN = 5;
            constexpr const char* REPLACE_STR = "{sev}";

            // if the prompt is less than size of 5 then ignore
            if (m_options.prompt.size() < REPLACE_LEN) return m_options.prompt;

            // attempt matching the severity
            const size_t pos = m_options.prompt.find(REPLACE_STR);

            // if there is not matching string, then return the base prompt
            if (pos == std::string::npos) return m_options.prompt;

            // prepare a suitable string to replace with
            std::string temp = m_options.prompt;

            // otherwise replace with the desired severity.
            return temp.replace(pos, REPLACE_LEN, m_severityStrings[sev]);
        }

        /**
         * Attempts finding the next available format character.
         * @param buffer                        Current message buffer.
         */
        static std::string m_findNextFormatCharacter(const std::string& buffer) {
            std::string::size_type pos = buffer.find(m_options.formatChar);
            if (pos != std::string::npos) return buffer.substr(0, pos);
            return buffer;
        }

        /**
         * Base argument processing case.
         * @param buffer                        Final string buffer.
         */
        static void m_processArguments(std::string& buffer) { std::cout << buffer; }

        /**
         * Heavy lifter method to process variadic arguments and buffer. Finds the next format character,
         * replaces this as needed with an argument, otherwise prints the rest of the available buffer.
         * @param buffer                        Current message buffer.
         * @param next                          Next variable argument.
         * @param args                          Other variable arguments.
         */
        template <typename T, typename... Args>
        static void m_processArguments(std::string& buffer, const T& next, Args&&... args) {
            // process the current buffer format
            std::string trimmed = m_findNextFormatCharacter(buffer);

            // pre-emptively print the current trimmed string
            std::cout << trimmed;

            // if trimmed same size as buffer, then complete
            if (trimmed.size() == buffer.size()) return;

            // otherwise print the current argument
            std::cout << next;
            buffer = buffer.substr(trimmed.size() + 1);

            // and continue to next argument
            m_processArguments(buffer, std::forward<Args>(args)...);
        }
    };

    /***************************
     *  STATIC INITIALISATION  *
     ***************************/

    const std::array<std::string, 5> Logger::m_severityStrings = {"\x1b[1;31mFATAL\x1b[0m", "\x1b[31mERROR\x1b[0m", "\x1b[33mWARNING\x1b[0m", "\x1b[34mINFO\x1b[0m", "TRACE"};
    Logger::Options Logger::m_options;

    /*******************
     *  CORE LOGGABLE  *
     *******************/

    /// Helper Loggable Abstract Class. Aids in making
    /// derived classes be loggable.
    class Loggable {
       public:
        /// Make a pure virtual class.
        virtual ~Loggable() = default;

        /// Virtualized stringification method.
        virtual std::string toString() const = 0;

        /**
         * Use the friend keyword to modify the core output stream operator for
         * simple printing of derived classes.
         * @param os                                Base Output Stream.
         * @param self                              Loggable Instance.
         */
        friend std::ostream& operator<<(std::ostream& os, const Loggable& self) {
            os << self.toString();
            return os;
        }
    };

}  // namespace tiny

/*******************
 *  HELPER MACROS  *
 *******************/

#define TL_FATAL(FMT, ...) ::tiny::Logger::log(::tiny::Logger::FATAL, FMT, ##__VA_ARGS__)
#define TL_ERROR(FMT, ...) ::tiny::Logger::log(::tiny::Logger::ERROR, FMT, ##__VA_ARGS__)
#define TL_WARNING(FMT, ...) ::tiny::Logger::log(::tiny::Logger::WARNING, FMT, ##__VA_ARGS__)
#define TL_INFO(FMT, ...) ::tiny::Logger::log(::tiny::Logger::INFO, FMT, ##__VA_ARGS__)
#define TL_TRACE(FMT, ...) ::tiny::Logger::log(::tiny::Logger::TRACE, FMT, ##__VA_ARGS__)
#define TL_VALUE(...) ::tiny::Logger::logValue(__VA_ARGS__)

#endif