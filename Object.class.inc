<?php

class Object
{
    public $name;
    public $title;
	public $titleArticle;
    public $description;
    public $tagline;
    public $canCarry;
    public $carryStr;
    
    public $nouns;
    public $adjs;
    
    function __construct()
    {
        $this->title = 'untitled object';
        $this->titleArticle = 'an';
        $this->description = 'The object has no description.';
        $this->tagline = '';
        $this->canCarry = 0;
        $this->carryStr = '';
        
        $this->nouns = array();
        $this->adjs = array();
    }
}