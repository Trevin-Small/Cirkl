export const ArrayBuilder = ( () => {

  const arrayHeader = "const uint8_t ";

  function toCArray(arrayName, imgBuffer) {

    let output = arrayHeader + arrayName + "[" + imgBuffer.length * 3 + "] = {\n";

    let counter = 0;

    for (let i = 0; i < imgBuffer.length; i++) {
      output += "0x" + imgBuffer[i].r + ", " + "0x" + imgBuffer[i].g + ", " + "0x" + imgBuffer[i].b + ",";
      counter += 3;

      if (counter == 15) {
        output += "\n";
        counter = 0;
      }
    }

    output += "\n}";

    console.log(output);
  }

  return {
    toCArray
  }

})();