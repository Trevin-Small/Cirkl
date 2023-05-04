import { initializeApp } from "firebase/app";

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDvn53F6_u2YxB5KQJXRbn3XJ-EwfjoVUo",
  authDomain: "cirkl-d3b3d.firebaseapp.com",
  projectId: "cirkl-d3b3d",
  storageBucket: "cirkl-d3b3d.appspot.com",
  messagingSenderId: "791478699120",
  appId: "1:791478699120:web:852c432fa603d42f9060b5",
  measurementId: "G-5S303FP0SY"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

const MAX_CANVAS_SIZE = 800;
const MIN_CANVAS_SIZE = 240;
const CANVAS_PX_SIZE = 480;

const Main = (() => {

  // Button Containers
  let edit = {};
  let upload = {};
  let manip = {};

  let croppedImg;
  let canvas;
  let ctx;
  let canvasDisplaySize;
  let img;
  let imgProperties = {
    x: 0,
    y: 0,
    width: 0,
    height: 0,
    aspectRatio: 1
  };

  function init() {
    edit = document.getElementById('edit-container');
    edit.cropBtn = document.getElementById("crop-btn");
    edit.cropBtn.addEventListener("click", cropImage, false);
    edit.removeBtn = document.getElementById("remove-btn");
    edit.removeBtn.addEventListener("click", removeImage, false);

    upload = document.getElementById('upload-container');
    upload.uploadBtn = document.getElementById("upload");
    upload.uploadBtn.addEventListener("change", uploadImage, false);

    manip = document.getElementById('manip-container');
    manip.zoomIn = document.getElementById("zoom-in");
    manip.zoomIn.addEventListener("click", () => {zoomImage(30)}, false);

    manip.zoomOut = document.getElementById("zoom-out");
    manip.zoomOut.addEventListener("click", () => {zoomImage(-30)}, false);

    manip.up = document.getElementById('up');
    manip.up.addEventListener("click", () => {moveImage(0, -15)}, false);

    manip.right = document.getElementById('right');
    manip.right.addEventListener("click", () => {moveImage(15, 0)}, false);

    manip.down = document.getElementById('down');
    manip.down.addEventListener("click", () => {moveImage(0, 15)}, false);

    manip.left = document.getElementById('left');
    manip.left.addEventListener("click", () => {moveImage(-15, 0)}, false);


    croppedImg = document.getElementById('cropped-img');

    canvas = document.getElementById("photo-canvas");
    ctx = canvas.getContext("2d");
    ctx.imageSmoothingEnabled = true;

    resize();
    window.addEventListener("resize", resize, false);
  }

  function resize() {
    let mainContainer = document.getElementById("main-container");
    let containerWidth = mainContainer.offsetWidth;
    let containerHeight = mainContainer.offsetHeight;

    let minDimension = containerWidth < containerHeight ? containerWidth : containerHeight;

    let canvasSize = Math.max(Math.min(0.5 * minDimension, MAX_CANVAS_SIZE), MIN_CANVAS_SIZE);
    canvas.style.width = canvas.style.height = canvasSize + "px";
  }

  function uploadImage(e) {
    if (upload.uploadBtn.value == "") return;

    canvas.style.display = "block";
    canvasDisplaySize = CANVAS_PX_SIZE;

    let reader = new FileReader();
    reader.onload = function(event) {
      img = new Image();
      img.onload = function() {
        imgProperties.x = imgProperties.y = 0;
        imgProperties.width = img.width;
        imgProperties.height = img.height;
        imgProperties.aspectRatio =  img.height / img.width;
        console.log(imgProperties);
        drawImage();
      }
      img.src = event.target.result;
    }
    reader.readAsDataURL(e.target.files[0]);
    edit.style.visibility = "visible";
    upload.style.display = "none";
    manip.style.display = "flex";
    upload.uploadBtn.value = "";
  }

  function moveImage(directionX, directionY) {
    //if (imgProperties.x + directionX > 0 ||
    //  imgProperties.x + directionX + imgProperties.width < canvasDisplaySize) return;
    //if (imgProperties.y + directionY > 0 ||
    //  imgProperties.y + directionY + imgProperties.height < canvasDisplaySize) return;

    imgProperties.x += directionX;
    imgProperties.y += directionY;
    drawImage();
  }

  function zoomImage(zoom) {
    canvasDisplaySize += zoom;
    imgProperties.x -= zoom / 2;
    imgProperties.y -= (zoom / 2);// * imgProperties.aspectRatio;
    drawImage();
  }

  function drawImage() {
    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, CANVAS_PX_SIZE, CANVAS_PX_SIZE);
    ctx.drawImage(img, imgProperties.x, imgProperties.y, canvasDisplaySize, canvasDisplaySize);
  }

  async function cropImage() {
    ctx.globalCompositeOperation = 'destination-in';
    ctx.beginPath();
    ctx.arc(CANVAS_PX_SIZE / 2, CANVAS_PX_SIZE / 2, CANVAS_PX_SIZE / 2, 0, 2 * Math.PI, true);
    ctx.fill();

    var element = document.createElement('a');
    element.setAttribute('href', canvas.toDataURL());
    element.setAttribute('download', Date.now().toString() + ".png");

    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);

  }

  function removeImage() {
    croppedImg.src = "";
    croppedImg.style.display = "none";
    canvas.style.display = "block";
    ctx.fillStyle = 'black';
    ctx.clearRect(0, 0, CANVAS_PX_SIZE, CANVAS_PX_SIZE);
    ctx.fillRect(0, 0, CANVAS_PX_SIZE, CANVAS_PX_SIZE);
    edit.style.visibility = "hidden";
    upload.style.display = "flex";
    manip.style.display = "none";
  }

/*
  async function buildImageBuffer() {
    let imageData = ctx.getImageData(0, 0, CANVAS_PX_SIZE, CANVAS_PX_SIZE, { colorSpace: "srgb" });
    let data = imageData.data;
    let imageBytes = new Uint8Array(data.length / 2);
    let j = 0;

    for (let i = 0; i < data.length; i += 4) {

      let red = (data[i] & 0b11111000) >> 3;
      let blue = (data[i + 1] & 0b11111000) >> 3;
      let green = (data[i + 2] & 0b11111100) >> 2;

      let bytes = colorBytes(red, green, blue);

      imageBytes[j] = bytes[0];
      imageBytes[j + 1] = bytes[1];
      j += 2;
    }

    console.log(imageBytes);

    const base64_arraybuffer = async (data) => {
      // Use a FileReader to generate a base64 data URI
      const base64url = await new Promise((r) => {
          const reader = new FileReader()
          reader.onload = () => r(reader.result)
          reader.readAsDataURL(new Blob([data]))
      })


      The result looks like 
      "data:application/octet-stream;base64,<your base64 data>", 
      so we split off the beginning:

      return base64url.split(",", 2)[1]
  }

  // example use:
    return await base64_arraybuffer(imageBytes);

    //let s = imageBytes.toString("base64");
    //return Buffer.from(imageBytes).toString("base64");
  }

  function colorMap(color, fromScale, toScale) {
    return Math.trunc((color / fromScale) * toScale);
  }

  function colorBytes(red, green, blue) {
    let val = 0;
    val = (blue << 11) | (green << 5) | (red);
    let upper = (val & 0xFF00);
    let lower = (val & 0x00FF);
    return [upper, lower];
  }

  function toCArray(arrayName, imgBuffer) {

    let output = "const uint8_t " + arrayName + "[] = {\n";

    let counter = 0;

    for (let i = 0; i < imgBuffer.length; i++) {
      output += "0x" + imgBuffer[i][0] + "," + "0x" + imgBuffer[i][1] + ",";
      counter += 2;

      if (counter == 16) {
        output += "\n";
        counter = 0;
      }
    }

    output += "\n}";

    console.log(output);
  }
*/

  return {
    init
  };

})();

export function init() { Main.init(); }