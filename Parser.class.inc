<?php

/**
Grammar for recursive descent parser

[BEGIN]     -> [GLOBAL] [BEGIN0]

[BEGIN0]    -> [GLOBAL] [BEGIN0]
            -> <epsilon>

[GLOBAL]    -> [AREA]
            -> [OBJECT]
			-> [GAME]

[AREA]      -> @area [AREA0]

[AREA0]     -> @name id [AREA1]
            -> @title [TITLE] [AREA1]
            -> @description <string> [AREA1]
            -> @objects <csv id-string> [AREA1]
            -> @exits [EXITS] [AREA1]

[AREA1]     -> [AREA0]
            -> <epsilon>

[TITLE]     -> <string>
            -> [SEM] <string>

[EXITS]     -> [SEM] [EXITS0]

[EXITS0]    -> [EXITS]
            -> id [EXITS1]

[EXITS1]    -> [EXITS]
            -> <epsilon>

[SEM]       -> <semantic token>

[OBJECT]    -> @object [OBJECT0]

[OBJECT0]   -> @name id [OBJECT1]
            -> @title [TITLE] [OBJECT1]
            -> @description <string> [OBJECT1]
            -> @tagline <string> [OBJECT1]
            -> @carry? [VALUE] [CARRYSTR] [OBJECT1]

[CARRYSTR]  -> <string>
            -> <epsilon>

[OBJECT1]   -> [OBJECT0]
            -> <epsilon>
			
[GAME]      -> @game [GAME0]

[GAME0]     -> @start id [GAME1]

[GAME1]     -> [GAME0]
            -> <epsilon>

[VALUE]     -> <value token>
*/

require_once('Tokenizer.class.inc');
// Game data classes
require_once('Area.class.inc');
require_once('Object.class.inc');
require_once('Exit.class.inc');


class Parser
{
    private $tokenizer;
    private $token;
	
	public $areas;
	public $objects;
	public $metadata;
    
    function __construct($pTokenizer)
    {
        $this->tokenizer = $pTokenizer;
		$this->areas = array();
		$this->objects = array();
		$this->metadata = array();
    }
    
    function parse()
    {
        $this->tokenizer->goToStart();
        
        $this->getNextToken();
        return $this->BEGIN();
    }
    
    private function BEGIN()
    {
        return ($this->_GLOBAL() && $this->BEGIN0());
    }
    
    private function BEGIN0()
    {
        $this->getNextToken();
        if ($this->token === false)
        {
            return true;
        }
        else
        {
            return ($this->_GLOBAL() && $this->BEGIN0());
        }
    }
    
    private function _GLOBAL()
    {
        if ($this->token->name == TK_AREA)
        {
            $this->getNextToken();
            return $this->AREA();
        }
        else if ($this->token->name == TK_OBJECT)
        {
            $this->getNextToken();
            return $this->_OBJECT();
        }
        else if ($this->token->name == TK_GAME)
        {
            $this->getNextToken();
            return $this->GAME();
        }
        else
        {
            print("Expected global token on line {$this->token->lineNum}");
            print(" (found {$this->token})");
            return false;
        }
    }
    
    private function AREA()
    {
		$newArea = new Area();
		$this->areas[] = &$newArea;
        return $this->AREA0($newArea);
    }
    
    private function AREA0(&$pNewArea)
    {
        if ($this->token->name == TK_NAME
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$pNewArea->name = $this->token->name;
			return $this->AREA1($pNewArea);
        }
        else if ($this->token->name == TK_TITLE)
        {
            $this->getNextToken();
            return ($this->TITLE($pNewArea) && $this->AREA1($pNewArea));
        }
        else if ($this->token->name == TK_DESCRIPTION
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$pNewArea->description = $this->token->name;
			return $this->AREA1($pNewArea);
        }
        else if ($this->token->name == TK_OBJECTS
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$objectsCSV = $this->token->name;
			$objects = explode(',', $objectsCSV);
			foreach ($objects as $object)
			{
				$pNewArea->objects[] = trim($object);
			}
			return $this->AREA1($pNewArea);
        }
        else if ($this->token->name == TK_EXITS)
        {
            $this->getNextToken();
            return ($this->EXITS($pNewArea) && $this->AREA1($pNewArea));
        }
        else
        {
            print("Invalid area field token on line {$this->token->lineNum}");
            print(" (found {$this->token})");
            return false;
        }
    }
    
    private function AREA1(&$pNewArea)
    {
        $this->getNextToken();
        if ($this->token === false)
        {
            // reached end
            return true;
        }
        if ($this->token->type != GLOBAL_TOKEN)
        {
            // more area fieldset
            return $this->AREA0($pNewArea);
        }
        else
        {
            $this->tokenizer->backUpOne();
            return true;
        }
    }
    
	/**
		shared by @area and @object construction
		Note: Title "article" does not currently apply to @areas.
	*/
    private function TITLE(&$pNewItem)
    {
        if ($this->token->type == SEMANTIC_TOKEN)
        {
			$pNewItem->titleArticle = ($this->token->name == TK_AN)? 'an': 'a';
			$this->getNextToken();
        }
		
        if ($this->token->type == STRING_TOKEN)
        {
			$pNewItem->title = $this->token->name;
            return true;
        }
        else
        {
            print("Invalid token for title on line {$this->token->lineNum}");
            print(" (found {$this->token})");
            return false;
        }
    }
    
    /**
		exits are defined with tokens in this order:
		@sem [@sem..] string
		@sem [@sem..] string
	*/
    private function EXITS(&$pNewArea)
    {
        return ($this->SEM($pNewArea) && $this->EXITS0($pNewArea));
    }
    
    private function EXITS0(&$pNewArea)
    {
        $this->getNextToken();
        if ($this->token === false)
        {
            print("Incomplete exit on line {$this->token->lineNum}");
            return false;
        }
        else if ($this->token->type == STRING_TOKEN)
        {
			// Save destination area for last exit
			$exits = &$pNewArea->outlets;
			$exits[count($exits)-1]->destArea = $this->token->name;
			
            $this->getNextToken();
            return $this->EXITS1($pNewArea);
        }
        else
        {
            return $this->EXITS($pNewArea);
        }
    }
    
    private function EXITS1(&$pNewArea)
    {
        if ($this->token->type == SEMANTIC_TOKEN)
        {
            return $this->EXITS($pNewArea);
        }
        else
        {
            $this->tokenizer->backUpOne();
            return true;
        }
    }
    
    private function SEM(&$pNewArea)
    {
		if ($this->token->type == SEMANTIC_TOKEN)
		{
			if (count($pNewArea->outlets) == 0)
			{
				// Create first exit for area
				$newExit = new Outlet();
				$newExit->tags[] = $this->token->name;
				$pNewArea->outlets[] = $newExit;
			}
			else
			{
				// Existing exits
				$exits = &$pNewArea->outlets;
				$lastExitIndex = count($exits) - 1;
				if ($exits[$lastExitIndex]->destArea === false)
				{	// Last exit has no dest. area
					// Add tag to last exit
					$exits[$lastExitIndex]->tags[] = $this->token->name;
				}
				else
				{
					// create new exit
					$newExit = new Outlet();
					$newExit->tags[] = $this->token->name;
					$pNewArea->outlets[] = $newExit;
				}
			}
			return true;
		}
        return false;
    }

    private function _OBJECT()
    {
		$newObject = new Object();
		$this->objects[] = &$newObject;
        return $this->OBJECT0($newObject);
    }
    
    private function OBJECT0(&$pNewObject)
    {
        if ($this->token->name == TK_NAME
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$pNewObject->name = $this->token->name;
            return $this->OBJECT1($pNewObject);
        }
        else if ($this->token->name == TK_TITLE)
        {
            $this->getNextToken();
            return ($this->TITLE($pNewObject) && $this->OBJECT1($pNewObject));
        }
        else if ($this->token->name == TK_DESCRIPTION
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
            $pNewObject->description = $this->token->name;
			return $this->OBJECT1($pNewObject);
        }
        else if ($this->token->name == TK_TAGLINE
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
            $pNewObject->tagline = $this->token->name;
            return $this->OBJECT1($pNewObject);
        }
        else if ($this->token->name == TK_CARRY)
        {
            if ($this->getNextToken()->type == VALUE_TOKEN)
            {
				$pNewObject->canCarry = ($this->token->name == TK_YES)? true: false;
                $this->getNextToken();
                return ($this->CARRYSTR($pNewObject) && $this->OBJECT1($pNewObject));
            }
            else
            {
                print("Expected value token for carry on line {$this->token->lineNum}");
                print(" (found {$this->token})");
                return false;
            }
        }
        else if ($this->token->name == TK_NOUNS
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$nounsCSV = $this->token->name;
			$nouns = explode(',', $nounsCSV);
			foreach ($nouns as $noun)
			{
				$pNewObject->nouns[] = trim($noun);
			}
			return $this->OBJECT1($pNewObject);
        }
        else if ($this->token->name == TK_ADJS
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$adjsCSV = $this->token->name;
			$adjs = explode(',', $adjsCSV);
			foreach ($adjs as $adj)
			{
				$pNewObject->adjs[] = trim($adj);
			}
			return $this->OBJECT1($pNewObject);
        }
        else
        {
            print("Invalid object field token on line {$this->token->lineNum}");
            print(" (found {$this->token})");
            return false;
        }
    }
    
    private function OBJECT1(&$pNewObject)
    {
        $this->getNextToken();
        if ($this->token === false)
        {
            // reached end
            return true;
        }
        else if ($this->token->type != GLOBAL_TOKEN)
        {
            // more objects fieldset
            return $this->OBJECT0($pNewObject);
        }
        else
        {
            $this->tokenizer->backUpOne();
            return true;
        }
    }
    
    private function CARRYSTR(&$pNewObject)
    {
        if ($this->token->type == STRING_TOKEN)
        {
			$pNewObject->carryStr = $this->token->name;
            return true;
        }
        else
        {
            $this->tokenizer->backUpOne();
            return true;
        }
    }
	
    private function GAME()
    {
        return $this->GAME0();
    }
    
    private function GAME0()
    {
        if ($this->token->name == TK_START
			&& $this->getNextToken()->type == STRING_TOKEN)
        {
			$this->metadata['start'] = $this->token->name;
			return $this->GAME1();
        }
        else
        {
            print("Invalid game field token on line {$this->token->lineNum}");
            print(" (found {$this->token})");
            return false;
        }
    }
    
    private function GAME1()
    {
        $this->getNextToken();
        if ($this->token === false)
        {
            // reached end
            return true;
        }
        if ($this->token->type != GLOBAL_TOKEN)
        {
            // more game fieldset
            return $this->GAME0();
        }
        else
        {
            $this->tokenizer->backUpOne();
            return true;
        }
    }
    
    private function VALUE()
    {
        return ($this->token->type == VALUE_TOKEN);
    }
    
    private function getNextToken()
    {
        $this->token = $this->tokenizer->getNextToken();
        return $this->token;
    }
}