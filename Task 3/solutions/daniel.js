/*
    DRITTE AUFGABE:

    Es geht weiter mit Strings.
    Schreibe eine eigene split('Hello World!', ' '); Implementation.
    Diese Funktion soll sowohl Strings als auch Objekte in Arrays umgewandeln.
    Es sollte natürlich keine Methode benutzt werden, die diese Funktionalität schon
    beinhaltet.

    In diesem Test setzen wir auf Typensicherheit mit der Library "flow" von facebook.

    Um deine Aufgabe zu builden gib einfach "npm run build" ein.
*/

module.exports = {
  split(value, seperator) {
    isInvalid =
      (seperator === null && value === null) ||
      value === null ||
      value === undefined;

    if (
      typeof value == "object" &&
      !isInvalid &&
      Object.keys(value).length !== 0
    ) {
      var arr = [];
      Object.keys(value).forEach(key => {
        arr.push({ [key]: value[key] });
      });
      return arr;
    }

    if (isInvalid) {
      return [];
    }

    let erg = [];
    let tmp = "";
    Array.from(value).map((item, i) => {
      if (item !== seperator) {
        tmp += item;
      } else {
        erg.push(tmp);
        tmp = "";
      }
      if (i === Array.from(value).length - 1) {
        erg.push(tmp);
      }
    });
    return erg;
  }
};
