<?php
    require_once('Tokenizer.class.inc');
    require_once('Parser.class.inc');
    require_once('Executive.class.inc');

    session_start();

    if (isset($_GET['reset'])) 
    {
        session_destroy();
        session_start();
    }

    $tokenizer = new Tokenizer(file_get_contents('main.game'));

    $parser = new Parser($tokenizer);
?>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<head>
    <link rel="stylesheet" href="main.css" type="text/css" />
	<title>The Game</title>
</head>

<div id="infobar">
    &nbsp;Location
    <div id="score">SCORE: 0</div>
    <div class="clear"></div>
</div>
<div id="main">&nbsp;<br/>&nbsp;<br/>

<?php




?>

</div>


<table style="width: 100%; margin: 0; border: 0 none; padding: 0;">
    <tr>
        <td style="width: 37px">
            <div id="prompt_arrows">&nbsp;&gt;&gt;&nbsp;</div>
        </td>
        <td>
            <input id="prompt" value="">
        </td>
        <td style="width: 8px"></td>
    </tr>
</table>


<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>

<script type="text/javascript" src="main.js"></script>