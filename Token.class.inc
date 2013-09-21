<?php

define('GLOBAL_TOKEN', 'GlobalToken');
define('FIELD_TOKEN', 'FieldToken');
define('VALUE_TOKEN', 'ValueToken');
define('SEMANTIC_TOKEN', 'SemanticToken');
define('STRING_TOKEN', 'StringToken');
define('UNKNOWN_TOKEN', 'UnknownToken');

define('TK_VARIABLE', '@variable');
define('TK_AREA', '@area');
define('TK_OBJECT', '@object');
define('TK_GAME', '@game');

define('TK_NAME', '@name');
define('TK_TITLE', '@title');
define('TK_DESCRIPTION', '@description');
define('TK_OBJECTS', '@objects');
define('TK_EXITS', '@exits');
define('TK_TAGLINE', '@tagline');
define('TK_CARRY', '@carry?');
define('TK_START', '@start');
define('TK_NOUNS', '@nouns');
define('TK_ADJS', '@adjs');

define('TK_YES', '@yes');
define('TK_NO', '@no');

define('TK_A', '@a');
define('TK_AN', '@an');
define('TK_SOME', '@some');
define('TK_WEST', '@west');
define('TK_EAST', '@east');
define('TK_NORTH', '@north');
define('TK_SOUTH', '@south');
define('TK_LEFT', '@left');
define('TK_RIGHT', '@right');
define('TK_UP', '@up');
define('TK_DOWN', '@down');
define('TK_STAIRS', '@stairs');
define('TK_DOOR', '@door');

class Token
{
    public $name;
    public $isKeyword;
    public $type;
    public $lineNum;
    
    function __construct($pTokenText, $pLineNum)
    {
        $this->lineNum = $pLineNum;
        $this->name = $pTokenText;

        $this->isKeyword = ($this->name[0] == '@');
        
        if (!$this->isKeyword)
        {
            $this->type = STRING_TOKEN;
        }
        else
        {
            switch ($this->name)
            {
                case TK_VARIABLE:
                case TK_AREA:
                case TK_OBJECT:
				case TK_GAME:
                    $this->type = GLOBAL_TOKEN;
                    break;
                
                case TK_NAME:
                case TK_TITLE:
                case TK_DESCRIPTION:
                case TK_OBJECTS:
                case TK_EXITS:
                case TK_TAGLINE:
                case TK_CARRY:
				case TK_START:
                    $this->type = FIELD_TOKEN;
                    break;
                
                case TK_YES:
                case TK_NO:
                    $this->type = VALUE_TOKEN;
                    break;
                
                case TK_A:
                case TK_AN:
                case TK_SOME:
                case TK_WEST:
                case TK_EAST:
                case TK_NORTH:
                case TK_SOUTH:
                case TK_LEFT:
                case TK_RIGHT:
                case TK_UP:
                case TK_DOWN:
                case TK_STAIRS:
                case TK_DOOR:
                    $this->type = SEMANTIC_TOKEN;
                    break;
                
                default:
                    $this->type = UNKNOWN_TOKEN;
            }
        }

    }
    
    function __tostring()
    {
        switch ($this->type)
        {
            case GLOBAL_TOKEN: return "global token {$this->name}"; break;
            case FIELD_TOKEN: return "field token {$this->name}"; break;
            case VALUE_TOKEN: return "value token {$this->name}"; break;
            case SEMANTIC_TOKEN: return "semantic token {$this->name}"; break;
            case STRING_TOKEN: return "string"; break;
            case UNKNOWN_TOKEN: return "unknown token {$this->name}"; break;
        }
    }
}