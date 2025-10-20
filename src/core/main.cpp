#include <catch_amalgamated.hpp>
#include <lightwave/core.hpp>
#include <lightwave/logger.hpp>
#include <lightwave/parallel.hpp>
#include <lightwave/registry.hpp>

#include "../cmake/git_version.h"

#include "parser.hpp"

#include <fstream>

#ifdef LW_OS_WINDOWS
#include <Windows.h>
#include <cstdlib>
#else
#include <unistd.h>
#endif

using namespace lightwave;

std::string get_hostname() {
    char buffer[1024];
    gethostname(buffer, sizeof(buffer));
    return buffer;
}

void print_exception(const std::exception &e, int level = 0) {
    logger(EError, "%s%s", std::string(2 * level, ' '), e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &nestedException) {
        print_exception(nestedException, level + 1);
    } catch (...) {
    }
}

int runUnitTests(int argc, const char *argv[]) {
    return Catch::Session().run(argc, argv);
}

int main(int argc, const char *argv[]) {
    logger(EInfo, "welcome to lightwave, git hash %s", kGitHash);
    logger(EInfo,
           "running on %s with %d threads",
           get_hostname(),
           get_number_of_threads());
    logger(EInfo, "running with arguments");
    for (int i = 0; i < argc; i++)
        logger(EInfo, "  '%s'", argv[i]);

#ifdef LW_DEBUG
    logger(EWarn,
           "lightwave was compiled in Debug mode, expect rendering to "
           "be much slower");
#endif
#ifdef LW_CC_MSC
    logger(EWarn,
           "lightwave was compiled using MSVC, expect rendering to be "
           "slower (we recommend using clang instead)");
#endif

#ifdef LW_OS_WINDOWS
    // The error dialog might interfere with the run_tests.py script on Windows,
    // so disable it.
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    // Set console code page to UTF-8 so console known how to interpret string
    // data
    SetConsoleOutputCP(CP_UTF8);

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif

    try {
        if (argc <= 1) {
            logger(EInfo, "running unit tests since no scene path was given");
            return runUnitTests(argc, argv);
        }

        // MARK: Parse arguments
        std::vector<std::filesystem::path> sceneFiles;
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg.starts_with("-D")) {
                // define variable
                int j = 2;
                while (arg[j] != '=' && arg[j] != ' ')
                    if (++j >= int(arg.size()))
                        lightwave_throw(
                            "expected '=' or ' ' after argument '%s'", arg);

                auto variable = arg.substr(2, j - 2);
                auto value    = arg.substr(j + 1);
                logger(
                    EDebug, "setting variable '%s' to '%s'", variable, value);
                sceneVariables.emplace(variable, value);
            } else {
                // scene file
                sceneFiles.push_back(arg);
            }
        }

        for (const auto &scenePath : sceneFiles) {
            logger.linebreak();
            SceneParser parser{ scenePath };
            for (auto &object : parser.objects()) {
                if (auto executable =
                        dynamic_cast<Executable *>(object.get())) {
                    logger.linebreak();
                    logger(EInfo, "running %s", executable);
                    executable->execute();
                }
            }
        }
    } catch (const std::exception &e) {
        print_exception(e);
        return 1;
    }

    return 0;
}
