--TEST--
Test snappy_uncompress() function : basic functionality
--SKIPIF--
--FILE--
<?php
include(dirname(__FILE__) . '/data.inc');

echo "*** Testing snappy_uncompress() : basic functionality ***\n";

// Initialise all required variables
$compressed = snappy_compress($data);

echo "\n-- Basic decompress --\n";
var_dump(strcmp($data, snappy_uncompress($compressed)));
?>
===DONE===
--EXPECT--
*** Testing snappy_uncompress() : basic functionality ***

-- Basic decompress --
int(0)
===DONE===
