// const chiffre = {
//     /*
//         ERSTE AUFGABE:
//
//         Hier mal eine eigene Struktur überlegen,
//         wie man die Datenstruktur am besten abbildet. Alles ist erlaubt :)
//
//         Auch wenn der Polybios-Chiffre eigentlich aus zufällig zusammen-
//         gewürfelten Buchstaben besteht, würde ich hier sagen, bleiben wir
//         wie der Wikipedia Beitrag erst mal bei einem festen Aufbau:
//
//         https://de.wikipedia.org/wiki/Polybios-Chiffre
//
//             1	2	3	4	5
//         1	A	B	C	D	E
//         2	F	G	H  I/J	K
//         3	L	M	N	O	P
//         4	Q	R	S	T	U
//         5	V	W	X	Y	Z
//
//         J ist hier ein Sonderfall. Würde hier das J unter dem gleichen Wert wie I abspeichern
//     */
// };

const columnCount = 5;
const rowCount = 5;

// Easy method to get ascii numbers for alphabet -> [97, 98, ..., ]
const asciiAlphabet = Array.from(new Array(26).keys()).map(value => value + 'a'.charCodeAt(0));

// Convert ascii numbers to actual alphabet and customize special characters i and j to i/j
const alphabetArray = asciiAlphabet
    .map((value) => {
        const result = String.fromCharCode(value);

        if (result === 'i') {
            return 'i/j';
        } else if (result === 'j') {
            return null;
        }

        return result;
    })
    .filter(value => !!value);

module.exports = {
    encrypt(value) {
        if (value) {
            return value.split("").map((character) => {
                // Lower case character of word such that they match
                let lowerCharacter = character.toLowerCase();

                // Check for special characters i and j
                if (lowerCharacter === 'i' || lowerCharacter === 'j') {
                    lowerCharacter = 'i/j';
                }

                // get index where character is found (TODO what if character is not found?)
                const index = alphabetArray.indexOf(lowerCharacter);

                // get row from index
                const row = Math.trunc(index / rowCount) + 1;

                // get column from index
                const column = (index % columnCount) + 1;

                return `${row}${column}`;
            }).join(' ');
        }

        return '';
    }
};
