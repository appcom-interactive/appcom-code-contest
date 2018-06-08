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
        if (value === null) {
            return [];
        }
        
        // Create an empty array
        const array = [];

        // If value is a string
        if (typeof value === 'string') {
            // Create an empty array for a single splitted element
            const element = [];

            // Iterate all characters of string
            [...value].forEach((char, index) => {
                // If current char is a seperator, push the current element into the main array
                if (char === seperator) {
                    if (element.length > 0) {
                        array.push(element.join(''));
                    }

                    // Clear the single element array
                    element.length = 0;
                } else {
                    // Otherwise push the next char into the element array, as it is a part of the splitted value
                    element.push(char);
                }

                // Check if the end of the value has been reached and push the last element into the main array
                if (index === value.length - 1) {
                    array.push(element.join(''));
                }
            });

            // Push the whole value into the main array if the value could not be split
            if (array.length === 0 && value) {
                array.push(value);
            }
        // If value is an object
        } else if (typeof value === 'object') {
            // Iterate over all keys of the object
            Object.keys(value).forEach((key, index) => {
                const obj = {};
                obj[key] = value[key];
                array.push(obj);
            });
        }

        // Return the main array, containing the splitted values
        return array;
    }
};