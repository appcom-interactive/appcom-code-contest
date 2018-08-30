module.exports = {
  checkGame(value) {
    const checkHorizontal = (value, i, j, player) => {
      let correct = 1;
      try {
        for (let k = 1; k < 4; k++) {
          if (value[i][j + k] === player) {
            correct++;
          }
        }
        return correct === 4;
      } catch (e) {
        return false;
      }
    };
    const checkVertical = (value, i, j, player) => {
      let correct = 1;
      try {
        for (let k = 1; k < 4; k++) {
          if (value[i + k][j] === player) {
            correct++;
          }
        }
        return correct === 4;
      } catch (e) {
        return false;
      }
    };
    const checkDiagonal = (value, i, j, player) => {
      let correct = 1;
      try {
        for (let k = 1; k < 4; k++) {
          if (value[i + k][j - k] === player) {
            correct++;
          }
        }
        if (correct !== 4) {
          correct = 1;
          for (let k = 1; k < 4; k++) {
            if (value[i + k][j + k] === player) {
              correct++;
            }
          }
        }
        return correct === 4;
      } catch (e) {
        return false;
      }
    };

    let playerAcoins = 0;
    let playerBcoins = 0;
    for (let i = 0; i < value.length; i++) {
      for (let j = 0; j < value[i].length; j++) {
        const player = value[i][j];
        if (player === 'A') {
          playerAcoins++;
        }
        if (player === 'B') {
          playerBcoins++;
        }
      }
    }
    if (playerBcoins > playerAcoins) {
      return 'Cheater! Cheater! Cheater!';
    }

    for (let i = 0; i < value.length; i++) {
      for (let j = 0; j < value[i].length; j++) {
        const player = value[i][j];

        if (player !== null) {
          if (player !== 'A' && player !== 'B') {
            return 'Go away! Strange kid!';
          }
          if (i < value.length - 1 && value[i + 1][j] === null) {
            return 'Cheater! Cheater! Cheater!';
          }

          if (
            checkHorizontal(value, i, j, player) ||
            checkVertical(value, i, j, player) ||
            checkDiagonal(value, i, j, player)
          ) {
            return `Player ${player} wins!`;
          }
        }
      }
    }
  }
};
