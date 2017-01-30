--TEST--
Test snappy_compress() function : variation
--SKIPIF--
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
string(32) "aa706ffad8df5ca3f37f4ec061d91ca2"
bool(false)
bool(true)
===Done===
