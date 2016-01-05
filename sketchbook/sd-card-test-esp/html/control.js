var myVar = setInterval(promptupdate, 1000);

function promptupdate() {
    var outputarea = document.getElementById("outputarea");
    var text = "HI!\nthing";
    
    outputarea.value = text;
    outputarea.scrollTop = outputarea.scrollHeight;
}

function DocumentReady()
{
    promptupdate();
}

$(document).ready(DocumentReady);