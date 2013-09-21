<?php

class Outlet
{
    public $destArea;
    public $tags;
    
	function __construct()
	{
		$this->tags = array();
		$this->destArea = false;
	}
}