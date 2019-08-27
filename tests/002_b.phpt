--TEST--
Test snappy_compress() function : error conditions
--SKIPIF--
<?php
if (version_compare(PHP_VERSION, '8.0', '<')) die('skip PHP is too new');
--FILE--
<?php
include(dirname(__FILE__) . '/data.inc');

echo "*** Testing snappy_compress() : error conditions ***\n";

// Zero arguments
echo "\n-- Testing snappy_compress() function with Zero arguments --\n";
try {
  var_dump(snappy_compress());
} catch (Error $e) {
  echo $e, PHP_EOL;
}

//Test snappy_compress with one more than the expected number of arguments
echo "\n-- Testing snappy_compress() function with more than expected no. of arguments --\n";
$data = 'string_val';
$extra_arg = 10;
try {
  var_dump(snappy_compress($data, $extra_arg));
} catch (Error $e) {
  echo $e, PHP_EOL;
}

class Tester {
    function Hello() {
        echo "Hello\n";
    }
}

echo "\n-- Testing with incorrect parameters --\n";
$testclass = new Tester();
try {
  var_dump(snappy_compress($testclass));
} catch (Error $e) {
  echo $e, PHP_EOL;
}
?>
===Done===
--EXPECTF--
*** Testing snappy_compress() : error conditions ***

-- Testing snappy_compress() function with Zero arguments --
ArgumentCountError: snappy_compress() expects exactly 1 parameter, 0 given in %s:%d
Stack trace:
#0 %s(%d): snappy_compress()
#1 {main}

-- Testing snappy_compress() function with more than expected no. of arguments --
ArgumentCountError: snappy_compress() expects exactly 1 parameter, 2 given in %s:%d
Stack trace:
#0 %s(%d): snappy_compress(%s)
#1 {main}

-- Testing with incorrect parameters --

Warning: snappy_compress : expects parameter to be string. in %s on line %d
bool(false)
===Done===
