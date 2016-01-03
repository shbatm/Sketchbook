var myVar = setInterval(promptupdate, 1000);

function promptupdate() {
    // var d = new Date();
    var outputarea = document.getElementById("outputarea");
    var text = "HI!\nthing";
    for(var i = 0; i < 1000; i++)
    {
        text += i + "\n";
    }
    $("#outputarea").text(text);
    outputarea.scrollTop = outputarea.scrollHeight;
}
function DocumentReady()
{
    promptupdate();
}
$(document).ready(DocumentReady);
