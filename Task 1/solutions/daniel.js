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
    let mappingList = {};

    // erstellt ein Objekt mit den Zuordnungen
    const createMappingList = chiffre => {
      chiffre.map((rows, y) => {
        rows.map((cols, x) => {
          // prüft ob non ALPHA CHARS vorhanden sind und und entfernt diese
          // dies könnte als eigene Funktion ausgelagert werden sanitizeMappingList() oä
          const pattern = new RegExp("[^A-Z]");
          if (pattern.test(chiffre[y][x])) {
            // wenn non alpha char dann entferne und teile die verbleibenden chars auf
            chiffre[y][x].replace(/[^A-Z]+/gi, "").split("");
            const sonderlinge = chiffre[y][x]
              .replace(/[^A-Z]+/gi, "")
              .split("");
            sonderlinge.map(sonderling => {
              mappingList[sonderling] = (y + 1).toString() + (x + 1).toString();
            });
          } else {
            mappingList[chiffre[y][x]] =
              (y + 1).toString() + (x + 1).toString();
          }
        });
      });
    };

    createMappingList(chiffre);

    // Erstellt verschlüsselte Nachricht
    const createEncryptedMessage = () => {
      let message = "";
      valueArray.map(char => {
        message += mappingList[char] + " ";
      });
      return message.trim();
    };

    return createEncryptedMessage();
  }
};
