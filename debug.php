<?php

require_once('Game.class.inc');
require_once('Player.class.inc');
require_once('Executive.class.inc');



session_start();

print('<pre>');

$area = Game::get()->getArea('davids_room');
print_r($area);


$object = Game::get()->getObject('computer');
print_r($object);