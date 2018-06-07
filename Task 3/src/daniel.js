// @flow

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
  split(value: string | {}, seperator: string): Array<mixed> {
    if (seperator === null || value === null) {
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
