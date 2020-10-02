# Snappy Extension for PHP

[![Build Status](https://secure.travis-ci.org/kjdev/php-ext-snappy.png?branch=master)](http://travis-ci.org/kjdev/php-ext-snappy)
[![Build status](https://ci.appveyor.com/api/projects/status/20i7jiflddmfhkus/branch/master?svg=true)](https://ci.appveyor.com/project/kjdev/php-ext-snappy/branch/master)

This extension allows Snappy.

Documentation for Snappy can be found at
[» http://google.github.io/snappy/](http://google.github.io/snappy/).

## Build from sources

    % git clone --recursive --depth=1 https://github.com/kjdev/php-ext-snappy.git
    % cd php-ext-snappy
    % phpize
    % ./configure
    % make
    % make install

To use the system library

``` bash
% ./configure --with-snappy-includedir=/usr
```

## Distribution binary packages

### Fedora / CentOS / RHEL

RPM packages of this extension are available in [» Remi's RPM repository](https://rpms.remirepo.net/) and are named **php-snappy**.

## Configuration

php.ini:

    extension=snappy.so

## Function : snappy_compress

string snappy_compress( string $data )

### parameters

data:

    The data to compress.

### return values

The compressed string or FALSE if an error occurred.

## Function : snappy_uncompress

string snappy_uncompress( string $data )

### parameters

name:

    The data compressed by snappy_gzcompress(). 

### return values

The original uncompressed data or FALSE on error.

## Example

    $compressed = snappy_compress('Compress me');

    $uncompressed = snappy_uncompress($compressed);

    echo $uncompressed;

## Ubuntu Tipps

snappy requires C++ and therefore might require for you to install the  g++ or build-essential package. 
    
If you get an error about "this file requires compiler and library support" you might need to enfore the compilation with

    $ export CXXFLAGS=-std=c++11
    $ phpize
    $ ./configure
    $ make
