cmake_minimum_required(VERSION 3.1)
project(launcher Java)
find_package(Java 1.6 REQUIRED COMPONENTS Development)

include(UseJava)
set(CMAKE_JAVA_JAR_ENTRY_POINT org.polycraftlauncher.EntryPoint)
set(CMAKE_JAVA_COMPILE_FLAGS -target 1.6 -source 1.6 -Xlint:deprecation -Xlint:unchecked)

set(SRC
    org/polycraftlauncher/EntryPoint.java
    org/polycraftlauncher/Launcher.java
    org/polycraftlauncher/LegacyFrame.java
    org/polycraftlauncher/NotFoundException.java
    org/polycraftlauncher/ParamBucket.java
    org/polycraftlauncher/ParseException.java
    org/polycraftlauncher/Utils.java
    org/polycraftlauncher/onesix/OneSixLauncher.java
    net/minecraft/Launcher.java
)
add_jar(NewLaunch ${SRC})
install_jar(NewLaunch "${JARS_DEST_DIR}")
