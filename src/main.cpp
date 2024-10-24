#include <spdlog/spdlog.h>
#include "app.hpp"

int main() {
    lve::App app{};

    try {
        app.Run();
    } catch (const std::exception &e) {
        SPDLOG_ERROR(e.what());
        return 1;
    }

    return 0;
}