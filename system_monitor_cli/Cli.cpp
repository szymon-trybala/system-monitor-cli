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
			("n, net-adapter", "Current working network adapter name")
			("w, win", "Version of installed Windows");

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