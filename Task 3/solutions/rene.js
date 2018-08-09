

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
    let newArray = [];
    if (!value) {
      return newArray;
    }
    if (value === 'undefined' || value === null) {
      newArray = [];
    } else if (typeof value === 'object') {
      newArray = Object.keys(value).map(key => {
        return {
          [key]: value[key]
        };
      });
    } else {
      while (value.length) {
        if (value.indexOf(seperator) > -1) {
          newArray.push(value.slice(0, value.indexOf(seperator)));
          if (value.indexOf(seperator) === value.length - 1) {
            newArray.push('');
          }
          value = value.slice(value.indexOf(seperator) + 1, value.length);
        } else {
          newArray.push(value);
          value = '';
        }
      }
    }

    console.log(newArray);
    return newArray;
  }
};