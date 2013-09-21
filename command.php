<?php
function debug($pVar)
{
    if (isset($_GET['debug']))
    {
        print("\n <pre> {debug_output} \n");
        if (gettype($pVar) == 'object' || gettype($pVar) == 'array')
            print_r($pVar);
        else
            var_dump($pVar);
        print("\n {debug_end} </pre> \n");
    }
}


require_once('Game.class.inc');
require_once('Player.class.inc');
require_once('Executive.class.inc');

$cmd = (isset($_POST['command']))? strtolower(trim($_POST['command'])): '';
if ($cmd == '')
{
	print('I beg your pardon?');
	exit;
}

session_start();

//    _______________________
//___/  Command processing   \______________________________________

/** @var Executive */
$executive = Executive::get();

$words = explode(' ', $cmd);

// Process first word (verb)
$verb = array_shift($words);

if ($verb == 'walk' || $verb == 'go')
{
	$verb = array_shift($words);
}
debug('Verb: '.$verb);
$response = 'What?';
switch ($verb)
{
	case 'look':
		$response = $executive->look();
		break;
	
	case 'north':
	case 'n':
		$response = $executive->go(Executive::NORTH);
		break;
        
	case 'south':
	case 's':
		$response = $executive->go(Executive::SOUTH);
		break;
        
	case 'west':
	case 'w':
		$response = $executive->go(Executive::WEST);
		break;
        
	case 'east':
	case 'e':
		$response = $executive->go(Executive::EAST);
		break;
        
    case 'down':
    case 'd':
        $response = $executive->go(Executive::DOWN);
        break;
        
    case 'up':
    case 'u':
        $response = $executive->go(Executive::UP);
        break;
        
    case 'left':
    case 'l':
        $response = $executive->go(Executive::LEFT);
        break;
        
    case 'right':
    case 'r':
        $response = $executive->go(Executive::RIGHT);
        break;

	case 'in':
	case 'through':
    case 'out':
        $response = $executive->go(Executive::DOOR);
        break;

	case 'die':
        $response = 'You are dead to me.';
        break;
        
    case 'take':
    case 'get':
    case 'carry':
    case 'pick':
        // $objWords: ['knife'], ['blue', 'ball'], ['red', 'brick'], etc.
        $objWords = $words;
        if (count($objWords))
        {
            $response = $executive->pickUp($objWords);
        }
        break;
        
    case 'drop':
        // $objWords: ['knife'], ['blue', 'ball'], ['red', 'brick'], etc.
        $objWords = $words;
        if (count($objWords))
        {
            $response = $executive->drop($objWords);
        }
        break;
        
    case 'inventory':
    case 'inv':
    case 'i':
        $response = $executive->showInventory();
        break;
	
	default:
		break;
}


print(nl2br($response));



