$(document).ready(function()
{
    $('#prompt').focus();
    
    $('#prompt').keyup(function(e) {
        if(e.keyCode == 13)
        {
			var command = $(this).val();
			$(this).val("");
			
            sendCommand(command);
        }
    });
    
    sendCommand('look', true);
    
});


function sendCommand(pCommand, pSilent)
{
    if (!pSilent)
    {
        $("#main").html($("#main").html() + "<br/><div class='command'>&nbsp;>>&nbsp;<span>" + pCommand + '</span></div>');
    }
    
    $.ajax({
        type: "POST",
        url: "command.php",
        data: 'command='+pCommand,
        success: commandDone
    });
}

function commandDone(data)
{
	
	$("#main").html($("#main").html() + data + "<br/>&nbsp;");

    $('html, body').animate({scrollTop: $("#main").height() + $("#prompt").height()}, 800, "linear");
}