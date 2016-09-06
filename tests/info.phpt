--TEST--
Test phpinfo() displays snappy info
--SKIPIF--
--FILE--
<?php
if (!extension_loaded('snappy')) {
    dl('snappy.' . PHP_SHLIB_SUFFIX);
}

phpinfo();
--EXPECTF--
%a
snappy

Snappy support => enabled
Extension Version => %d.%d.%d
Snappy Version => %s
%a
