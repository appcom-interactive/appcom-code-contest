const chiffre = [
    ['A', 'B', 'C', 'D', 'E'],
    ['F', 'G', 'H', 'I', 'J,K'],
    ['L', 'M', 'N', 'O', 'P'],
    ['Q', 'R', 'S', 'T', 'U'],
    ['V', 'W', 'X', 'Y', 'Z']
];

const searchChar = (char) => {
    for (let i = 0; i < chiffre.length; i++) {
        for (let j = 0; j < chiffre[i].length; j++) {
            if (chiffre[i][j].split(',').includes(char)) {
                return `${i+1}${j+1}`;
            }
        }
    }
};

module.exports = {
    encrypt: (value) => {
        return value.split('')
            .map(char => searchChar(char))
                .join(' ');
    }
};
