--TEST--
Test snappy_uncompress() function : error conditions
--SKIPIF--
--FILE--
<?php

echo "*** Testing snappy_uncompress() : error conditions ***\n";

// Zero arguments
echo "\n-- Testing snappy_uncompress() function with Zero arguments --\n";
var_dump( snappy_uncompress() );

//Test snappy_uncompress with one more than the expected number of arguments
echo "\n-- Testing snappy_uncompress() function with more than expected no. of arguments --\n";
$data = 'string_val';
$extra_arg = 10;
var_dump( snappy_uncompress($data, $extra_arg) );


echo "\n-- Testing with incorrect arguments --\n";
var_dump(snappy_uncompress(123));

class Tester
{
    function Hello()
    {
        echo "Hello\n";
    }
}

$testclass = new Tester();
var_dump(snappy_uncompress($testclass));
?>
===DONE===
--EXPECTF--
*** Testing snappy_uncompress() : error conditions ***

-- Testing snappy_uncompress() function with Zero arguments --

Warning: snappy_uncompress() expects exactly 1 parameter, 0 given in %s on line %d
bool(false)

-- Testing snappy_uncompress() function with more than expected no. of arguments --

Warning: snappy_uncompress() expects exactly 1 parameter, 2 given in %s on line %d
bool(false)

-- Testing with incorrect arguments --

Warning: snappy_uncompress : expects parameter to be string. in %s on line %d
bool(false)

Warning: snappy_uncompress : expects parameter to be string. in %s on line %d
bool(false)
===DONE===
