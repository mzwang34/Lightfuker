/**
 * @file logger.hpp
 * @brief Provides utility functions to log messages to console output.
 */

#pragma once

#include <atomic>
#include <lightwave/core.hpp>
#include <mutex>
#include <sstream>

namespace lightwave {

/// @brief The severity of a log message.
enum LogLevel {
    EDebug = 0,
    EInfo  = 1,
    EWarn  = 2,
    EError = 3,
};

/// @brief The interface used to log messages to console output.
class Logger {
    /// @brief Synchronization to ensure that messages from different threads
    /// are not intermangled.
    std::mutex m_mutex;
    /// @brief A status message to be shown at the bottom of console output
    /// (e.g., render progress in percent).
    std::string m_status;

    /// @brief The log of everything that has been printed so far, without
    /// formatting or status lines.
    std::ostringstream m_history;

public:
    /// @brief Logs a message to console output, which will be constructed from
    /// the given format string and corresponding arguments.
    template <typename... Args>
    void operator()(LogLevel level, const char *fmt, const Args &...args) {
        std::unique_lock lock{ m_mutex };

        auto message = tfm::format(fmt, args...);

        std::cout << "\033[2K\r";
        switch (level) {
        case EDebug:
            std::cout << "\033[90m[debug] \033[0m";
            m_history << "[debug] ";
            break;
        case EInfo:
            std::cout << "\033[32m[info] \033[0m";
            m_history << "[info] ";
            break;
        case EWarn:
            std::cout << "\033[33m[warn] \033[0m";
            m_history << "[warn] ";
            break;
        case EError:
            std::cerr << "\033[31m[error] \033[0m";
            m_history << "[error] ";
            break;
        }

        (level >= EError ? std::cerr : std::cout) << message << std::endl;
        m_history << message << std::endl;

        std::cout << m_status << std::flush;
    }

    void linebreak() {
        std::cout << "\033[2K\r" << std::endl;
        std::cout << m_status << std::flush;

        m_history << std::endl;
    }

    /// @brief Returns everything that has been logged so far, excluding color
    /// formatting and status lines.
    std::string history() const { return m_history.str(); }

    /// @brief Sets the status text for display at the bottom of console output,
    /// constructed from a given format string.
    template <typename... Args>
    void setStatus(const char *fmt, const Args &...args) {
        std::unique_lock lock{ m_mutex };
        m_status = tfm::format(fmt, args...);
        std::cout << "\033[2K\r" << m_status << std::flush;
    }
};

/// @brief The interface used to log messages to console output.
extern Logger logger;

/// @brief A convenience class to keep the user updated about the progress of a
/// long-running task.
class ProgressReporter {
    /// @brief The name of the currently running task.
    std::string m_name;
    /// @brief The number of work units that need to be completed for the task
    /// to finish.
    long m_unitsTotal;
    /// @brief The number of work units that have been completed so far.
    std::atomic<long> m_unitsCompleted;
    /// @brief Measures how much time has elapsed since work began.
    Timer m_timer;
    /// @brief Tracks whether the work has been finished.
    bool m_hasFinished;

    std::string makeProgressBar(float progress, int width = 32) {
        int index          = int(round(progress * width));
        std::string result = "\033[96m";
        for (int i = 0; i < width; i++) {
            if (i == index)
                result += "\033[90m";
            result += i == index && index > 0 ? "╺" : "━";
        }
        return result + "\033[0m";
    }

public:
    ProgressReporter(long unitsTotal = 0)
        : ProgressReporter("render", unitsTotal) {}

    ProgressReporter(const std::string &name, long unitsTotal = 0) {
        m_name           = name;
        m_unitsTotal     = unitsTotal;
        m_unitsCompleted = 0;
        m_hasFinished    = false;

        logger.setStatus("\033[96m[%s]\033[0m starting render job", m_name);
    }

    /// @brief The number of work units that have been completed so far.
    long unitsCompleted() const { return m_unitsCompleted; }
    /// @brief The number of work units that need to be completed for the task
    /// to finish.
    long unitsTotal() const { return m_unitsTotal; }

    /// @brief Increments @c unitsCompleted (and optionally @c unitsTotal ) and
    /// notifies the user about the progress.
    void update(long unitsCompleted, long unitsTotal = 0) {
        m_unitsCompleted += unitsCompleted;
        m_unitsTotal += unitsTotal;

        const auto progress    = m_unitsCompleted / float(m_unitsTotal);
        const auto elapsedTime = m_timer.getElapsedTime();

        logger.setStatus(
            "\033[96m[%s]\033[0m %s \033[96m%3.0f%%\033[0m "
            "(\033[92m%.0fs\033[0m elapsed, \033[93m%.0fs\033[0m eta)",
            m_name,
            makeProgressBar(progress),
            100 * progress,
            elapsedTime,
            elapsedTime * (1 - progress) / progress);
    }

    /// @brief Returns the elapsed seconds since the creation of this
    /// ProgressReporter instance.
    float getElapsedTime() const { return m_timer.getElapsedTime(); }

    /// @brief Marks a number of @c unitsCompleted as completed and notifies the
    /// user about the progress.
    void operator+=(long unitsCompleted) { update(unitsCompleted); }

    /// @brief Marks the task as finished and notifies the user.
    void finish() {
        if (m_hasFinished)
            return;
        logger.setStatus("");
        logger(EInfo,
               "%s done! took %.2f seconds",
               m_name,
               m_timer.getElapsedTime());
        m_hasFinished = true;
    }

    ~ProgressReporter() { finish(); }
};

} // namespace lightwave
