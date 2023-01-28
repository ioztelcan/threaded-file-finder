# Threaded File Finder
A file find app that uses C++ threads to search concurrently.


## Dependencies
- The project was developed with C++20 standards and the build system configures it to build for that. This is the minimum required standard.
- It was tested with clang++ 14.0.6.
- The builds system checks for clang-tidy and uses it if it finds it.

## How to Build
There is a build script provided to help build the application. Please run it in the top project directory. 
There are two build types available `Debug` and `Release`.
The script will create a build directory and place the generated binaries inside, under `"build/build_type"/src`.
  
`./build.sh <Debug | Release>` : Builds the threaded file finder app.

`./build/Release/src/file_finder directory substr1 substr2 ...`: Runs the app to search for substrs in directory.

## Usage
Once the app starts running, it will immediately start searching for the filenames with the substrings, in the search directory.
While the app is running, `dump` command will dump the contents of the shared container to the console and `exit` command
will trigger the clean-up process and exit the application.

## Assumptions
- The input is passed in the order of directory first, then substrings and they are delimited by spaces as shown in the
  example `file-finder tmp aaa bbb ccc`.
- The application runs until the exit command is given, it does not exit by itself.
- The maximum number of substrings that can be searched is limited to the amount of concurrent threads that can be run
  by the host device.
- The periodic dump of the contents of the shared container is set to 200ms.
- Substrings will consist of valid characters that can't be interpreted by the shell in other ways.

