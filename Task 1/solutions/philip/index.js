class Polybios {
  /**
   * Creates an instance of Polybios.
   * @memberof Polybios
   */
  constructor(_gridCount = 5) {
    let count = 0;

    this.RANDOM_NUMBER = _gridCount;
    this.START_AT = 'A'.charCodeAt();

    this.dummyArray = [...new Array(this.RANDOM_NUMBER)];
    this.polybiosMap = this.dummyArray.map(() => {
      return this.dummyArray.map(() => {
        const letter = String.fromCodePoint(this.START_AT + count);

        if (letter !== 'J') {
          count++;
          return letter;
        } else {
          count += 2;
          return 'K';
        }
      });
    });
  }
  /**
   * Enrcrypt
   * @param {*} _text
   */
  encrypt(_text) {
    const text = _text;
    const str = [];

    for (let i = 0; i < text.length; i++) {
      this.polybiosMap.forEach((blank, number1) => {
        blank.forEach((char, number2) => {
          // special case
          if (
            text.charAt(i) === char ||
            (char === 'I' && text.charAt(i) === 'J')
          ) {
            str.push(number1 + 1 + '' + (number2 + 1));
          }
        });
      });
    }

    return str.join(' ');
  }
  /**
   * Decrypt
   * @param {*} _text
   */
  decrypt(_data) {
    const data = _data.split(' ');
    let str = '';

    data.forEach(el => {
      str += this.polybiosMap[el[0] - 1][el[1] - 1];
    });

    return str;
  }
}

module.exports = Polybios;
