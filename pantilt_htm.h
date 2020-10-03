const char pantilt_htm[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport"
        content="width=device-width,
        initial-scale=1.0,
        user-scalable=no"/>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <title>Pan tilt servos</title>
  <style>
    #container {
      width: 400px;
      height: 400px;
      background-color: #333;
      display: flex;
      align-items: center;
      justify-content: center;
      overflow: hidden;
      border-radius: 7px;
      touch-action: none;
    }
    #item {
      width: 100px;
      height: 100px;
      background-color: rgb(99, 245, 210);
      border: 10px solid rgba(136, 136, 136, .5);
      border-radius: 50%;
      touch-action: none;
      user-select: none;
    }
    #item:active {
      background-color: rgba(168, 218, 220, 1.00);
    }
    #item:hover {
      cursor: pointer;
      border-width: 20px;
    }
  </style>
</head>
<body>
  <div id="outerContainer">
    <div id="container">
      <div id="item">
      </div>
    </div>
  </div>
<script>
    var dragItem = document.querySelector("#item");
    var container = document.querySelector("#container");
    var active = false;
    var currentX = 0;
    var currentY = 0;
    var initialX;
    var initialY;
    var servoXInit = 90;
    var servoYInit = 150;
    var servoPosX = 90;
    var servoPosY = 150;
    var tempX = 90;
    var tempY = 150;
    var xOffset = 0;
    var yOffset = 0;
    w = $('#container').width();
    h = $('#container').height();

    container.addEventListener("touchstart", dragStart, false);
    container.addEventListener("touchend", dragEnd, false);
    container.addEventListener("touchmove", drag, false);

    container.addEventListener("mousedown", dragStart, false);
    container.addEventListener("mouseup", dragEnd, false);
    container.addEventListener("mousemove", drag, false);

    function dragStart(e) {
      if (e.type === "touchstart") {
        initialX = e.touches[0].clientX - xOffset;
        initialY = e.touches[0].clientY - yOffset;
      } else {
        initialX = e.clientX - xOffset;
        initialY = e.clientY - yOffset;
      }
      if (e.target === dragItem) {
        active = true;
      }
    }

    function dragEnd(e) {
      initialX = currentX;
      initialY = currentY;
      active = false;
      moveServos(currentX, currentY);
    }

    function drag(e) {
      if (active) {
        e.preventDefault();
        if (e.type === "touchmove") {
          currentX = e.touches[0].clientX - initialX;
          currentY = e.touches[0].clientY - initialY;
        } else {
          currentX = e.clientX - initialX;
          currentY = e.clientY - initialY;
        }

        xOffset = currentX;
        yOffset = currentY;
        setTranslate(currentX, currentY, dragItem);
      }
    }

    function setTranslate(xPos, yPos, el) {
      el.style.transform = "translate3d(" + xPos + "px, " + yPos + "px, 0)";
    }

    function moveServos(xPos, yPos){   
      xPo = servoXInit-180*xPos/w;
      yPo = servoYInit+100*yPos/h;

      if (xPo > 150) {xPo = 150}
      if (xPo < 30) {xPo = 30}
      if (yPo > 170) {yPo = 170}
      if (yPo < 60) {yPo = 60}

      servo(xPo, yPo);
    }

    $.ajaxSetup({timeout:1000});
    function servo(xP, yP) {
      $.get("/?value="+Math.round(xP)+","+Math.round(yP));
    }
</script>
</body>
</html>)rawliteral";
