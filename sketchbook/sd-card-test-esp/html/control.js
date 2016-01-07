// var myVar = setInterval(promptupdate, 1000);

function onControlClick()
{
    var action_ids = [
        // print controle buttons..
        document.getElementById("loadfile"),
        document.getElementById("print"),
        document.getElementById("pause"),
        document.getElementById("stop"),
        document.getElementById("turnoff"),
        document.getElementById("motorsoff"),
        // machine actions.
        document.getElementById("link-homex"),
        document.getElementById("homey"),
        document.getElementById("homez"),
        document.getElementById("homex"),
        document.getElementById("r"),
        document.getElementById("rr"),
        document.getElementById("rrr"),
        document.getElementById("rrrr"),
        document.getElementById("l"),
        document.getElementById("ll"),
        document.getElementById("lll"),
        document.getElementById("llll"),
        document.getElementById("b"),
        document.getElementById("bb"),
        document.getElementById("bbb"),
        document.getElementById("bbbb"),
        document.getElementById("t"),
        document.getElementById("tt"),
        document.getElementById("ttt"),
        document.getElementById("tttt")
    ];

    // link the html-links to onclick and not load href.
    for(var i = 0; i < action_ids.length; i++)
    {
        // console.log(action_ids, i);
        var element_id = action_ids[i];
        var element_id_name = action_ids[i].id;
        // create functions for it so that the page isn't reloaded when clicked.
        // and sends back what element what clicked as to take the appropriate
        // action.
        element_id.named = element_id_name;
        element_id.onclick = function()
        {
            // future send the actual commands to /action?this.name=1
            console.log(this.named);
            return false;
        }
    }
}

function Prompt()
{
    this.text = "";
    this.outputarea = document.getElementById("outputarea");
    this.addoutput = function(input)
    {
        // append the text and make lines.
        this.text += input + "\n";
        // set the text in the area.
        this.outputarea.value = this.text;
        // always set area scrolled down to new input.
        this.outputarea.scrollTop = this.outputarea.scrollHeight;
    }
    this.clearoutput = function()
    {
        // set text to nothing and display/update.
        this.text = "";
        this.addoutput("");
    }
}

function DocumentReady()
{
    var prompt = new Prompt();
    prompt.addoutput("Page Ready!");
    prompt.addoutput("Test line!");
    onControlClick();
}

$(document).ready(DocumentReady);