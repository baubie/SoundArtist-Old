function drawGradients() {
    var canvas = document.getElementById("gradient");
    var context = canvas.getContext("2d");
    var gradient = context.createLinearGradient(0, 0, 0, canvas.height);
    
    gradient.addColorStop(0, canvas.getAttribute("topColor"));
    gradient.addColorStop(1, canvas.getAttribute("bottomColor"));
    
    context.fillStyle = gradient;
    context.fill();
}
