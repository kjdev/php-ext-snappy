<?php
function isLittleEndian()
{
  $var = 0x00FF;
  return $var === current(unpack('v', pack('S', $var)));
}
