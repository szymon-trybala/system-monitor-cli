# System Monitor CLI
Helper application for GUI of System Monitor, written in C++ and using Windows API, currently just prints values based on argument.
In future it may serialize info to files, not just standard output.

CLI is made with [cxxopts](https://github.com/jarro2783/cxxopts) library.

Currently only first argument is processed, to get several values you need to run program few times.



## Currently available arguments with with associated returned values:

`-h` / `--help` - shows help and lists all possible arguments.

`-c` / `--cpu` - returns CPU load in percentage, as value between 0 and 100.

`-r` / `--ram` - first returned line is RAM usage in percentage, as value between 0 and 100, second is full amount of computer's memory in MB, third is amount of free memory in MB, and the last is amount of used memory in MB

`-n` / `--net-adapter` - first returned line is maximum speed of current network adapter in KB, second returned line is 
name of current network adapter.

`-w` / `--win` - returns currently used version of Windows.

`-u` / `--uptime` - returns amount of time elapsed since computer was turned on. First line is hours, second line is minutes, third line is seconds, and the last line is miliseconds.

`--name` - returns hostname of computer

`-d` / `--disk-space` - returns info about disk space - first line is free space in MB, second line is used space in MB, and the last line is whole space.
Currently it shows info only about system partition, in future it should be possible to provide value with disk letter (like `-d c:` or `-d all`), to show info about different partitions.

`--disk-type` - returns the type of disk on which is the current partition. Possible values are: `External drive`, `Hard drive`, `Network drive`, `CD drive` and `RAM disk`.
Currently it shows info only about system partition, in future it should be possible to provide value with disk letter (like `-d c:` or `-d all`), to show info about different partitions.