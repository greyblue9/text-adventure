<?php

session_start();

if (array_key_exists('comment', $_POST))
{
	processComment();
}

function processComment()
{
	if (isset($_SESSION['lastcomment']))
	{
		if (microtime(true) - $_SESSION['lastcomment'] < 60)
		{
			print("<i>Sorry, you need to wait longer before making another comment.</i>");
			return;
		}
	}
	
	$name = strip_tags(trim($_POST['name']));
	if ($name == "") $name = "Some anonymous crazy person";
	if (count($name) > 20) $name = substr($name, 0, 20);
	$comment = nl2br(strip_tags(trim($_POST['comment'])));
	if (count($comment) > 200) $comment = substr($comment, 0, 200);
	
	$newText = "<b>$name</b> said (on ".date("M j 'y")." at ".date("g:i A")."):
	<blockquote>$comment</blockquote>
	<hr/>";
	
	$_SESSION['lastcomment'] = microtime(true);
	
	file_put_contents("comments.txt", $newText, FILE_APPEND);
}




?>

<title>David's walk tracker</title>
<style type="text/css">
	body {
		font-family: sans-serif;
	}
	
	.walking {
		font-size: 16pt;
		padding: 5px;
		background: #80FF80;
		color: black;
		width: 200px;
	}
	
	.walking.not {
		background: #FF4040;
	}
</style>
<h3>David's (only slightly creepy) walk tracker</h3>


<div class="not walking" style="float: left">
	Not walking
</div>
<div style="color: #888; font-size: 12pt; float: left; margin-left: 10px; padding: 7px;">
Updated every 10 seconds
</div>
<div style="clear:both">
</div>

<iframe scrolling="no" style="border:0;padding:0;margin:10px;" src="http://www.gmap-track.com/user.php?user=greyblue9&output=embed&zoom=16&mt=m&w=640&h=480" width="640" height="480"></iframe>


<div style="background: #ccccff; padding: 6px 6px 2px 6px; width: 640px; font-weight: bold; border-bottom: 2px solid #9999ff">Leave a comment!</div>
<div style="padding-left: 32px; width: 610px">

	<form action="walk.php" method="post">
	<br/>Your name: <input type="text" name="name" />
	<br/>Comment: <textarea name="comment" rows="5" cols="50"></textarea>
	<br/><button type="submit">Post comment!</button>
	</form>
	
	<hr/>

	
<?php

if (file_exists("comments.txt"))
	print(file_get_contents("comments.txt"));
?>

</div>


<?php 

ob_start();
print_r($_SERVER);
$serverDump = ob_get_contents();
ob_end_clean();

file_put_contents('walklog.txt', "\n" . date(false) . ":\n$serverDump\n", FILE_APPEND);