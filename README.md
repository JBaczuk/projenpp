# Projen++
![GitHub (pre-)release](https://img.shields.io/github/release/JBaczuk/projenpp/all.svg)

Projen++ generates a working c++ program that sets up autotools such as autoconf and make.

## Features
* CLI (Optional) - uses Boost Program Options
* Boost Unit Test Framework (Optional)
* Autotools

## Installation
Download the latest binary on the [Releases Page](https://github.com/JBaczuk/projenpp/releases)

*Optional - copy to a system binary directory such as `/usr/local/bin`*

## Build from Source
### Dependencies
Requires Boost, curl, autotools, gcc  
`$ sudo apt-get install build-essential autotools-dev automake autoconf pkg-config libboost-all-dev libcurl4-gnutls-dev`

### Build
```
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
```

## Usage
```
$ projenpp --help
Projen++
Generate and configure C++ Programs

Usage: command [options]

Available Commands:
-------------------
Command | Description |
-------------------
init | Initialize a new project interactively

Allowed options:
  --help                      Show this help message
  -v [ --verbose ] [=arg(=1)] Enable verbosity (optionally specify level)
  -t [ --some_flag ]          Use some_flag configuration
  --command arg               Command
```

