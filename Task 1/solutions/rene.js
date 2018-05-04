const chiffre = [
  ["A", "B", "C", "D", "E"],
  ["F", "G", "H", "I", "K"],
  ["L", "M", "N", "O", "P"],
  ["Q", "R", "S", "T", "U"],
  ["V", "W", "X", "Y", "Z"]
];

module.exports = {
  encrypt(value) {
    let chiffreText = value
      .toUpperCase()
      .replace("J", "I")
      .split("")
      .map(char => {
        let index = getIndex(chiffre, char);
        return `${String(index[0] + 1) + String(index[1] + 1)}`;
      });
    return chiffreText.join(" ");
  }
};

function getIndex(array, value) {
  for (let indexRow = 0; indexRow < array.length; indexRow++) {
    let indexColumn = array[indexRow].indexOf(value);
    if (indexColumn > -1) {
      return [indexRow, indexColumn];
    }
  }
}
