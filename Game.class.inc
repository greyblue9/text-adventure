<?php

require_once('Area.class.inc');
require_once('Object.class.inc');
require_once('Exit.class.inc');

class Game
{
	private $areasByName;
	private $objectsByName;
	public $metadata;
	
	public function __construct($pGameFile)
	{
		require_once('Tokenizer.class.inc');
		require_once('Parser.class.inc');

		$tokenizer = new Tokenizer(file_get_contents($pGameFile));

		$parser = new Parser($tokenizer);
		if (!$parser->parse()) exit; // parse and quit on error

		$this->areasByName = array();
		foreach ($parser->areas as $area)
		{
			$this->areasByName[$area->name] = $area;
		}

		$this->objectsByName = array();
		foreach ($parser->objects as $object)
		{
			$this->objectsByName[$object->name] = $object;
		}
		
		$this->metadata = $parser->metadata;
	}
	
	public static function get()
	{
		if (!isset($_SESSION['game']))
		{
			$_SESSION['game'] = new Game('main.game');
		}
		
		return $_SESSION['game'];
	}
	
	public function getArea($pName)
	{
		if (array_key_exists($pName, $this->areasByName))
		{
			return $this->areasByName[$pName];
		}
		return false;
	}
	
	public function getObject($pName)
	{
		if (array_key_exists($pName, $this->objectsByName))
		{
			return $this->objectsByName[$pName];
		}
		return false;
	}
}