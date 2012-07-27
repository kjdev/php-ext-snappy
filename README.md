# php-ext-snappy #

This PHP extension is Snappy.

## Build ##

    % phpize
    % ./configure
    % make
    % make install

## Configration ##

snappy.ini:

    extension=snappy.so

## Function : snappy_compress ##

string snappy_compress( string $data )

### parameters ###

data:

    The data to compress.

### return values ###

The compressed string or FALSE if an error occurred.

## Function : snappy_uncompress ##

string snappy_uncompress( string $data )

### parameters ###

name:

    The data compressed by snappy_gzcompress(). 

### return values ###

The original uncompressed data or FALSE on error.

## Example ##

    $compressed = snappy_compress('Compress me');

    $uncompressed = snappy_uncompress($compressed);

    echo $uncompressed;
