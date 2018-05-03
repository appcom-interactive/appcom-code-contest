const chiffre = [
  ["A", "B", "C", "D", "E"],
  ["F", "G", "H", "I/J", "K"],
  ["L", "M", "N", "O", "P"],
  ["Q", "R", "S", "T", "U"],
  ["V", "W", "X", "Y", "Z"]
];

module.exports = {
  encrypt(value) {
    const valueArray = value.split("");
    let encryptetValue = "";

    const findCoords = char => {
      let result = "";
      chiffre.map((element, y) => {
          // :) naja ich versuche es noch raffinierter
        if (char === "J" || char === "I") {
          result = "24 ";
        }
        if (element.indexOf(char) !== -1) {
          let x = element.indexOf(char);
          y = y + 1;
          x = x + 1;
          result = y.toString() + x.toString() + " ";
        }
      });
      return result;
    };

    valueArray.map(char => {
      encryptetValue += findCoords(char);
    });

    return encryptetValue.trim();
  }
};
