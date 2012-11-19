<?php

if (!file_exists('data/tempA.rrd')) {
  $options = array(
     '--step', '30',
     'DS:tempa:GAUGE:60:-40:100',
     'RRA:AVERAGE:0.5:2:1440',
     'RRA:AVERAGE:0.5:180:168'
  );

  rrd_create('data/tempA.rrd', $options, count($options));
}

if (!file_exists('data/tempB.rrd')) {
  $options = array(
     '--step', '30',
     'DS:tempb:GAUGE:60:-40:100',
     'RRA:AVERAGE:0.5:2:1440',
     'RRA:AVERAGE:0.5:180:168'
  );

  rrd_create('data/tempB.rrd', $options, count($options));
}

if (!file_exists('data/weight.rrd')) {
  $options = array(
     '--step', '30',
     'DS:weight:GAUGE:60:0:1024',
     'RRA:AVERAGE:0.5:2:1440',
     'RRA:AVERAGE:0.5:180:168'
  );

  rrd_create('data/weight.rrd', $options, count($options));
}
if (array_key_exists('tempA', $_GET)) {
  file_put_contents('data/tempA', $_GET['tempA']);
  rrd_update('data/tempA.rrd', 'N:' . $_GET['tempA']);
}

if (array_key_exists('tempB', $_GET)) {
  file_put_contents('data/tempB', $_GET['tempB']);
  rrd_update('data/tempB.rrd', 'N:' . $_GET['tempB']);
}

if (array_key_exists('weight', $_GET)) {
  file_put_contents('data/weight', $_GET['weight']);
  rrd_update('data/weight.rrd', 'N:' . $_GET['weight']);
}

?>Y
