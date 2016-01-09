// var myVar = setInterval(promptupdate, 1000);

var getImageData = function(imgtag, x, y)
{
    var imgsrc = imgtag.src;
    
    var canvas = document.createElement("canvas");
    canvas.width = imgtag.width;
    canvas.height = imgtag.height;
    
    var context = canvas.getContext('2d')
    context.drawImage(imgtag, 0, 0);
    return context.getImageData(x, y, 1, 1);
}

function getClickPosition(e, element) {
    var parentPosition = getPosition(element);
    var xPosition = e.clientX - parentPosition.x;
    var yPosition = e.clientY - parentPosition.y;

    return {x: xPosition, y: yPosition};
}
 
function getPosition(element) {
    var xPosition = 0;
    var yPosition = 0;
      
    while (element) {
        xPosition += (element.offsetLeft - element.scrollLeft + element.clientLeft);
        yPosition += (element.offsetTop - element.scrollTop + element.clientTop);
        element = element.offsetParent;
    }
    return { x: xPosition, y: yPosition };
}

var isTransparentUnderMouse = function (event, element) {
    var imgtag = element.getElementsByTagName("img")[0];
    // console.log(element.getElementsByTagName("img"));
    // console.log(imgtag, imgtag.src, imgtag.width, imgtag.height);

    var pos = getClickPosition(event, element);
    var mx = pos.x;
    var my = pos.y;
    // console.log("pos: ", mx, my);

    pixel_data = getImageData(imgtag, mx, my);

    // console.log(pixel_data);
    // console.log(pixel_data.data);
    if(pixel_data.data[3])
        return false;
    return true;
};

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
        document.getElementById("homex"),
        document.getElementById("homey"),
        document.getElementById("homez"),
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
        element_id.element = element_id
        element_id.onclick = function(e)
        {
            var target = e.target;
            var tagname = target.tagName;
            // for a image we check the underlying transparency.
            // we don't wana react to that!
            if(tagname === "IMG")
            {
                if (isTransparentUnderMouse(e, this.element))
                    return false;
            }
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