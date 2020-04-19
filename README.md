# System Monitor CLI
Helper application for GUI of System Monitor, written in C++ and using Windows API, currently just prints values based on argument.
In future it may serialize info to files, not just standard output.

CLI is made with [cxxopts](https://github.com/jarro2783/cxxopts) library.

Currently only first argument is processed, to get several values you need to run program few times.

**Currently can be compiled for x64 systems only if you comment lines for getting scale factor in Display.cpp**

## Currently available arguments with with associated returned values:

* `-h` / `--help` - shows help and lists all possible arguments.

* `-c` / `--cpu` - returns CPU load in percentage, as value between 0 and 100.

* `--cn` - returns CPU info in 4 lines. First is processor name, second is its architecture, like `x64`, `x86` or `ARM64`, third is number of cores, and the last is number of threads.

* `-r` / `--ram` - first returned line is RAM usage in percentage, as value between 0 and 100, second is full amount of computer's memory in MB, third is amount of free memory in MB, and the last is amount of used memory in MB

* `-n` / `--net-adapter` - first returned line is maximum speed of current network adapter in KB, second returned line is 
name of current network adapter.

* `-w` / `--win` - returns currently used version, build and release of Windows.

* `-u` / `--uptime` - returns amount of time elapsed since computer was turned on. First line is hours, second line is minutes, third line is seconds, and the last line is miliseconds.

* `--name` - returns hostname of computer

* `-d -[drive-letter]` / `--disk-space --[drive-letter]` - returns info about disk space - first line is free space in MB, second line is used space in MB, and the last line is whole space. Second parameter is optional, if you skip it you will get system partition information.

* `--disk-type` - returns the type of disk on which is the current partition. Possible values are: `External drive`, `Hard drive`, `Network drive`, `CD drive` and `RAM disk`. Currently it shows info only about system partition, in future it should be possible to provide value with disk letter (like `-d c:` or `-d all`), to show info about different partitions.

* `--monitor-res` - Returns monitor's resolution - first line is horizontal resolution, sedond line is vertical resolution.

* `--monitor-info` - Returns monitor informations - first line is amount of monitors, second is name of main monitor, third is refresh rate in Hz, 4th is adjacent color bits for each pixel, 5th is color resolution, in bits per pixel, 6th is main monitor's estimated physical width in mm, 7th is main monitor's estimaged height in mm, 8th is scaling factor (inaccurate on newer versions of W10).

* `-g / --gpu-info` - Returns informations about GPU - first is name if currently used GPU, second is name of driver file, third is GPU's Windows name, and 4th is amount of GPU memory, IT'S DISPLAYED ONLY ON WINDOWS 10, ON OLDER SYSTEMS THERE WILL BE ONLY 3 LINES.

* `--gpu-usage` -  Returns GPU resources usage - ONLY FOR NVIDIA, OTHER GPU BRANDS RETURN ERROR IN CERR - first line is GPU usage.