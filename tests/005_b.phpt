--TEST--
Test snappy_uncompress() function : error conditions
--SKIPIF--
<?php
if (version_compare(PHP_VERSION, '8.0', '<')) die('skip PHP is too new');
--FILE--
<?php

echo "*** Testing snappy_uncompress() : error conditions ***\n";

// Zero arguments
echo "\n-- Testing snappy_uncompress() function with Zero arguments --\n";
try {
  var_dump( snappy_uncompress() );
} catch (Error $e) {
  echo $e, PHP_EOL;
}

//Test snappy_uncompress with one more than the expected number of arguments
echo "\n-- Testing snappy_uncompress() function with more than expected no. of arguments --\n";
$data = 'string_val';
$extra_arg = 10;
try {
  var_dump( snappy_uncompress($data, $extra_arg) );
} catch (Error $e) {
  echo $e, PHP_EOL;
}

echo "\n-- Testing with incorrect arguments --\n";
try {
  var_dump(snappy_uncompress(123));
} catch (Error $e) {
  echo $e, PHP_EOL;
}

class Tester
{
    function Hello()
    {
        echo "Hello\n";
    }
}

$testclass = new Tester();
try {
  var_dump(snappy_uncompress($testclass));
} catch (Error $e) {
  echo $e, PHP_EOL;
}
?>
===DONE===
--EXPECTF--
*** Testing snappy_uncompress() : error conditions ***

-- Testing snappy_uncompress() function with Zero arguments --
ArgumentCountError: snappy_uncompress() expects exactly 1 %s, 0 given in %s:%d
Stack trace:
#0 %s(%d): snappy_uncompress()
#1 {main}

-- Testing snappy_uncompress() function with more than expected no. of arguments --
ArgumentCountError: snappy_uncompress() expects exactly 1 %s, 2 given in %s:%d
Stack trace:
#0 %s(%d): snappy_uncompress(%s)
#1 {main}

-- Testing with incorrect arguments --

Warning: snappy_uncompress : expects parameter to be string. in %s on line %d
bool(false)

Warning: snappy_uncompress : expects parameter to be string. in %s on line %d
bool(false)
===DONE===
