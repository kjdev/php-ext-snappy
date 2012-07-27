--TEST--
Test snappy_compress() function : error conditions
--SKIPIF--
--FILE--
<?php 
include(dirname(__FILE__) . '/data.inc');

echo "*** Testing snappy_compress() : error conditions ***\n";

// Zero arguments
echo "\n-- Testing snappy_compress() function with Zero arguments --\n";
var_dump(snappy_compress());

//Test snappy_compress with one more than the expected number of arguments
echo "\n-- Testing snappy_compress() function with more than expected no. of arguments --\n";
$data = 'string_val';
$extra_arg = 10;
var_dump(snappy_compress($data, $extra_arg));

class Tester {
    function Hello() {
        echo "Hello\n";
    }
}

echo "\n-- Testing with incorrect parameters --\n";
$testclass = new Tester();
var_dump(snappy_compress($testclass));
?>
===Done===
--EXPECTF--
*** Testing snappy_compress() : error conditions ***

-- Testing snappy_compress() function with Zero arguments --

Warning: snappy_compress() expects exactly 1 parameter, 0 given in %s on line %d
bool(false)

-- Testing snappy_compress() function with more than expected no. of arguments --

Warning: snappy_compress() expects exactly 1 parameter, 2 given in %s on line %d
bool(false)

-- Testing with incorrect parameters --

Warning: snappy_compress : expects parameter to be string. in %s on line %d
bool(false)
===Done===
