#include "Cli.h";

#include <iostream>

std::string Cli::parse(int argc, char* argv[])
{
	try
	{
		cxxopts::Options options("System Monitor CLI", " - tool to show system info depending on argument. Program can process only one, first argument");
		options.positional_help("[optional args]")
			.show_positional_help();

		options
			.allow_unrecognised_options()
			.add_options()
			("h, help", "Print help")
			("r, ram", "RAM usage in MB")
			("c, cpu", "CPU usage in %")
			("cn", "CPU info - 1st line is its name, 2nd is architecture, 3rd is core count, 4th is thread count")
			("n, net-adapter", "Current active network adapter name and speed")
			("w, win", "Currently used version, build and release version of Windows")
			("u, uptime", "System uptime. First line is hours, second line is minutes, third line is seconds, and the last line is miliseconds. Careful: when 'Fast startup' in Windows settings is on, turning off computer doesn't reset counter!")
			("name", "Hostname of computer")
			("d, disk-space", "Info about disk space - first line is free space in MB, second line is used space in MB, and the last line is whole space. Second parameter is optional, if you skip it you will get system partition information.")
			("disk-type", "Returns type of disk on which is the selected or system partition. Possible values are: `External drive`, `Hard drive`, `Network drive`, `CD drive` and `RAM disk`. To select different partition, provide its letter in second argument, for example 'system_monitor_cli.exe --disk-type -D:\\-'. Without additional argument, system partition will be probed.")
			("p, partitions", "Return letters of all available partitions, in one line, for example 'C:\\D:\\'")
			("monitor-res", "Returns monitor's resolution - first line is horizontal resolution, sedond line is vertical resolution")
			("monitor-info", "Returns monitor informations - first line is amount of monitors, second is name of main monitor, third is refresh rate in Hz, 4th is adjacent color bits for each pixel, 5th is color resolution, in bits per pixel, 6th is main monitor's estimated physical width in mm, 7th is main monitor's estimaged height in mm, 8th is scaling factor (inaccurate on newer versions of W10)")
			("g, gpu-info", "Returns informations about GPU - first is name if currently used GPU, second is name of driver file, third is GPU's Windows name, and 4th is amount of GPU memory, IT'S DISPLAYED ONLY ON WINDOWS 10, ON OLDER SYSTEMS THERE WILL BE ONLY 3 LINES. ")
			("gpu-usage", "Returns GPU resources usage - ONLY FOR NVIDIA, OTHER GPU BRANDS RETURN ERROR IN CERR - first line is GPU usage");

		auto result = options.parse(argc, argv);
		if (result.count("help")) {
			std::cout << options.help({ "", "Group" }) << std::endl;
			exit(0);
		}
		auto arguments = result.arguments();
		if (arguments.empty()) {
			std::cerr << "No valid arguments provided, use --help if you don't know them" << std::endl;
			exit(1);
		}
		else {
			return arguments[0].key();
		}

	}
	catch (const cxxopts::OptionException& e) {
		std::cerr << "Error parsing argumetns: " << e.what() << std::endl;
		exit(1);
	}
}