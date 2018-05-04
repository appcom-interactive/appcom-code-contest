const chiffre = [
  ["A", "B", "C", "D", "E"],
  ["F", "G", "H", "I/J", "K"],
  ["L", "M", "N", "O", "P"],
  ["Q", "R", "S", "T", "U"],
  ["V", "W", "X", "Y", "Z"]
];

module.exports = {
  encrypt(value) {
    const onlyChars = new RegExp("[^A-Z]");
    let mappingList = {};

    /**
     * Methode zum prüfen von Sonderfällen in der mappingList
     * und versucht diese zu korrigieren
     *
     * @param {*} item
     * @param {*} x
     * @param {*} y
     */
    const sanitizeMappingList = (item, x, y) => {
      if (onlyChars.test(item)) {
        item.replace(onlyChars, "").split("");
        const sonderlinge = item.replace(onlyChars, "").split("");
        sonderlinge.map(sonderling => {
          mappingList[sonderling] = (y + 1).toString() + (x + 1).toString();
        });
      } else {
        mappingList[chiffre[y][x]] = (y + 1).toString() + (x + 1).toString();
      }
    };

    /**
     * erstellt ein Objekt mit den Zuordnungen
     *
     * @param {*} chiffre
     */
    const createMappingList = chiffre => {
      chiffre.map((rows, y) => {
        rows.map((cols, x) => {
          sanitizeMappingList(chiffre[y][x], x, y);
        });
      });
    };

    createMappingList(chiffre);

    /**
     * Erstellt die verschlüsselte Nachricht
     */
    const createEncryptedMessage = () => {
      let message = "";
      value.split("").map(char => {
        message += mappingList[char] + " ";
      });
      return message.trim();
    };

    return createEncryptedMessage();
  }
};
