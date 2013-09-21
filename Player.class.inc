<?php

require_once('Game.class.inc');

class Player
{
	public $location;
    public $inventoryByName;
	
	public function __construct()
	{
		$this->location = Game::get()->metadata['start'];
        $this->inventoryByName = array();
	}
	
	public static function get()
	{
		if (!isset($_SESSION['player']))
		{
			$_SESSION['player'] = new Player();
		}
		
		return $_SESSION['player'];
	}
    
    public function addToInventory($pObjName)
    {
        $this->inventoryByName[$pObjName] = true;
    }
    
    public function removeFromInventory($pObjName)
    {
        if (isset($this->inventoryByName[$pObjName]))
        {
            unset($this->inventoryByName[$pObjName]);
        }
    }
    
    public function hasObject($pObjName)
    {
        return (isset($this->inventoryByName[$pObjName]));
    }
    
    /**
        @return <array> List of object names in inventory
    */
    public function getInventory()
    {
        return array_keys($this->inventoryByName);
    }
}

