#include "Cli.h"

#include "features/Memory.h"
#include "features/Processor.h"
#include "features/Storage.h"
#include "features/Network.h"
#include "features/System.h"
#include "features/Display.h"
#include "features/Graphics.h"

#include <iostream>
#include <WinUser.h>

int main(int argc, char* argv[])
{
    if (SetProcessDPIAware() == false) {        // Disabling DPI-awareness for newer Windows versions, to get actual screen resolution, not virtual resolution, after scaling
        std::cerr << "Error while calculating display scaling, resolution info may be misleading" << std::endl;
    }


    Cli cli;
    std::string argument = cli.parse(argc, argv);

    if (argument.compare("ram") == 0) {
        Mem mem;
        auto info = mem.getMemoryInfo();
        for (auto& el : info) {
            std::cout << el << std::endl;
        }
    }
    else if (argument.compare("cpu") == 0) {
        Processor cpu;
        std::cout << cpu.getCpuLoad() << std::endl;
    }
    else if (argument.compare("cn") == 0) {
        Processor cpu;
        auto info = cpu.getCpuInfo();
        std::cout << std::get<0>(info) << std::endl;
        std::cout << std::get<1>(info) << std::endl;
        std::cout << std::get<2>(info) << std::endl;
        std::cout << std::get<3>(info) << std::endl;
    }
    else if (argument.compare("net-adapter") == 0) {
        Network net;
        auto info = net.getNetworkSpeed();
        std::cout << std::get<0>(info) << std::endl;
        std::cout << std::get<1>(info) << std::endl;
    }
    else if (argument.compare("win") == 0) {
        System sys;
        std::cout << sys.getSystemVersion() << std::endl;
    }
    else if (argument.compare("uptime") == 0) {
        System sys;
        auto uptime = sys.getUpTime();
        std::cout << std::get<0>(uptime) << std::endl;
        std::cout << std::get<1>(uptime) << std::endl;
        std::cout << std::get<2>(uptime) << std::endl;
        std::cout << std::get<3>(uptime) << std::endl;
    }
    else if (argument.compare("name") == 0) {
        System sys;
        std::cout << sys.getComputerName() << std::endl;
    }
    else if (argument.compare("disk-space") == 0) {
        Storage st;
        std::vector<UINT64> disk_space;

        if (argc > 2)
        {
            char letter = argv[2][1];
            if (letter == '-') letter = argv[2][2];
            std::string path(1,letter);
            path.append(":\\");
            disk_space = st.getDiskSizeInfo(path);

        }
        else disk_space = st.getDiskSizeInfo();

        for (auto& x : disk_space) {
            std::cout << x << std::endl;
        }
    }
    else if (argument.compare("disk-type") == 0) {
        Storage st;
        std::cout << st.getDriveType() << std::endl;
    }
    else if (argument.compare("monitor-res") == 0) {
        Display display;
        auto result = display.get_display_resolution();
        std::cout << std::get<0>(result) << std::endl;
        std::cout << std::get<1>(result) << std::endl;
    }
    else if (argument.compare("monitor-info") == 0) {
        Display display;
        auto result = display.get_display_info();
        for (auto& x : result) {
            std::cout << x << std::endl;
        }
    }
    else if (argument.compare("gpu-info") == 0) {
        Graphics graphics;
        auto result = graphics.get_gpu_name();
        for (auto& x : result) {
            std::cout << x << std::endl;
        }
    }
    else if (argument.compare("gpu-usage") == 0) {
        Graphics graphics;
        auto result = graphics.get_gpu_usage();
        for (auto& x : result) {
            std::cout << x << std::endl;
        }
    }
    else 
    {
        std::cerr << "Wrong argument provided, program will stop" << std::endl;
        exit(-1);
    }
    exit(0);
}
