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
        return [];
    }
};