# Snappy Extension for PHP

[![Linux](https://github.com/kjdev/php-ext-snappy/workflows/Linux/badge.svg?branch=master)](https://github.com/kjdev/php-ext-snappy/actions?query=workflow%3ALinux+branch%3Amaster)
[![Windows](https://github.com/kjdev/php-ext-snappy/workflows/Windows/badge.svg?branch=master)](https://github.com/kjdev/php-ext-snappy/actions?query=workflow%3AWindows+branch%3Amaster)

This extension allows Snappy.

Documentation for Snappy can be found at
[» http://google.github.io/snappy/](http://google.github.io/snappy/).

## Build from sources

```shell
git clone --recursive --depth=1 https://github.com/kjdev/php-ext-snappy.git
cd php-ext-snappy
phpize
./configure
make
make install
```

To use the system library

```shell
./configure --with-snappy-includedir=/usr
```

## Distribution binary packages

### Fedora / CentOS / RHEL

RPM packages of this extension are available in [» Remi's RPM repository](https://rpms.remirepo.net/) and are named **php-snappy**.

## Configuration

php.ini:

```ini
extension=snappy.so
```

### Function : `snappy_compress()`

string snappy_compress( string $data )

#### parameters

data:

    The data to compress.

#### return values

The compressed string or FALSE if an error occurred.

### Function : `snappy_uncompress()`

string snappy_uncompress( string $data )

#### parameters

name:

    The data compressed by snappy_compress(). 

#### return values

The original uncompressed data or FALSE on error.

## Example

```php
$compressed = snappy_compress('Compress me');

$uncompressed = snappy_uncompress($compressed);

echo $uncompressed;
```

## Troubleshooting

### Ubuntu / OSX

Snappy requires C++ and therefore might require for you to install the g++ or build-essential package. 
    
If you get an error about "this file requires compiler and library support" or [compilation errors on OSX](https://github.com/kjdev/php-ext-snappy/issues/19), you need to enforce the compilation with `-std=c++11` flag:

```shell
export CXXFLAGS=-std=c++11
phpize
./configure
make
```
