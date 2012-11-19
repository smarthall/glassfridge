<?php


$draw=0;
$filename = '/tmp/out' . rand(0,999) . '.png';

$options = array(
   '--start', '-6h',
   '--no-legend',
   '--width', '640',
   '--height', '480',
);

if (array_key_exists('tempA', $_GET)) {
  $draw=1;
  array_push($options,
    'DEF:freezer=data/tempA.rrd:tempa:AVERAGE',
    'LINE:freezer#5050E0:Freezer');
}

if (array_key_exists('tempB', $_GET)) {
  $draw=1;
  array_push($options,
    'DEF:fridge=data/tempB.rrd:tempb:AVERAGE',
    'LINE:fridge#5050E0:Fridge');
}

if (array_key_exists('weight', $_GET)) {
  $draw=1;
  array_push($options,
    'DEF:weight=data/weight.rrd:weight:AVERAGE',
    'LINE:weight#000000:Weight');
}

if ($draw == 1) {
  rrd_graph($filename, $options, count($options));
  header('Content-type: image/png');
  readfile($filename);
  unlink($filename);
}
  
?>
