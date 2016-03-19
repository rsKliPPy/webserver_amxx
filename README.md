# WebServer AMXX

Has to be statically linked against [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) (Linux build already builds microhttpd).

Requires [AMTL](https://www.github.com/alliedmodders/amtl) to be downloaded and path to it setup in the Makefile.

## Compile process
#### Linux
Open up `./Makefile` and setup `HLSDK`, `METAMOD` and `AMTL` variables for yourself. Then run:
```
cd libmicrohttpd
make
cd ..
make
```
#### Windows
Make sure that `HLSDK`, `METAMOD` and `AMTL` environment variables are properly set up.

Download [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) and compile it by hand and put the output `libmicrohttpd.lib` in project's root folder (this step will be cut out in the future). 

Navigate to `vs2015/` and open up `webserver_amxx.sln` in Visual Studio 2015 (will possibly work in earlier versions). Hit the compile button and binary will be output to `Release/webserver_amxx.dll` or `Debug/webserver_amxx.dll` depending on your configuration.
