# System Monitor CLI
Helper application for GUI of System Monitor, written in C++ and using Windows API, currently just prints value based on argument.
In future it may serialize info to files, not just standard output.

Currently only first argument is processed, to get several values you need to run program few times.



## Currently available arguments with with associated returned values:

`-c` / `--cpu` - returns CPU load in percentage, as value between 0 and 100.

`-r` / `--ram` - returns RAM usage in percentage, as value between 0 and 100.

`-n` / `--net-adapter` - first returned line is maximum speed of current network adapter in KB, second returned line is 
name of current network adapter.

`-w` / `--win` - returns currently used version of Windows.

