# WebServer AMXX
WebServer AMXX is a module that host a web server on top of a HLDS and provides API to AMXX plugins, allowing them to create responses.
Plugins can respond with text strings, binary strings and files.

## Build process
[AMBuild](https://wiki.alliedmods.net/AMBuild) is a requirement.

Go into the project directory (where configure.py is) and do:
```
mkdir build
cd build
python ../configure.py
ambuild
```

You will be required to have HLSDK and METAMOD environment variables setup. You can also pass them to configure.py as command line arguments. Execute `python configure.py --help` to see available options.