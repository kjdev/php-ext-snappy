--TEST--
Test snappy_compress() function : variation
--SKIPIF--
<?php
include(dirname(__FILE__) . '/endianness.inc');
if (isLittleEndian()) {
  die('skip Little endian');
}
--FILE--
<?php
include(dirname(__FILE__) . '/data.inc');

echo "*** Testing snappy_compress() : variation ***\n";

echo "\n-- Testing multiple compression --\n";
$output = snappy_compress($data);
var_dump(md5($output));

$c = snappy_compress($output);
var_dump(md5($output) === md5($c));
var_dump(md5($output) === md5(snappy_uncompress($c)));

?>
===Done===
--EXPECTF--
*** Testing snappy_compress() : variation ***

-- Testing multiple compression --
string(32) "c66d873df26fbc39f55fde06affbb6c7"
bool(false)
bool(true)
===Done===
