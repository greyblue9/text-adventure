<?php

class Area
{
    public $name;
	
    public $title;
	public $titleArticle; // not used for Area definition
    public $description;
	public $objects; // array of object names
	public $outlets; // array of outlet instances (AKA exits)
	
	function __construct()
	{
		$this->objects = array();
		$this->outlets = array();
	}
}