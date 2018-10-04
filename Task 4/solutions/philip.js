const directions = {
  right: {
    x: 1,
    y: 0
  },
  left: {
    x: -1,
    y: 0
  },
  top: {
    x: 0,
    y: -1
  },
  bottom: {
    x: 0,
    y: 1
  },
  topright: {
    x: 1,
    y: -1
  },
  topleft: {
    x: -1,
    y: -1
  },
  bottomright: {
    x: 1,
    y: 1
  },
  bottomleft: {
    x: -1,
    y: 1
  }
};

module.exports = {
  checkGame(grid) {
    const countCoins = grid.reduce((acc, val) => acc.concat(val), []).reduce(
      (before, current) => ({
        A: current === 'A' ? (before.A += 1) : before.A,
        B: current === 'B' ? (before.B += 1) : before.B,
        C: current === 'C' ? (before.C += 1) : before.C
      }),
      { A: 0, B: 0, C: 0 }
    );

    if (countCoins.B > countCoins.A) {
      return 'Cheater! Cheater! Cheater!';
    } else if (countCoins.C > 0) {
      return 'Go away! Strange kid!';
    }

    function checkFieldsAround(direction, letter, rowIndex, lineIndex) {
      let count = 1;
      let isSame = true;

      let x = 0;
      let y = 0;

      while (isSame) {
        if (
          rowIndex + y < 5 &&
          lineIndex + x < 7 &&
          grid[rowIndex + y][lineIndex + x] === letter
        ) {
          count++;
        } else {
          isSame = false;
          if (grid[rowIndex + y][lineIndex + x] === null) {
            count = 0;
          }
        }

        x += directions[direction].x;
        y += directions[direction].y;
      }

      return {
        direction,
        letter,
        count
      };
    }

    for (let i = grid.length - 1; i >= 0; i--) {
      for (let k = grid[i].length - 1; k >= 0; k--) {
        if (grid[i][k] !== null) {
          const dir = Object.keys(directions).map(d =>
            checkFieldsAround(d, grid[i][k], i, k)
          );
          const result = dir.filter(e => e.count >= 4)[0];

          if (result) {
            return `Player ${result.letter} wins!`;
          } else if (dir.filter(e => e.count === 0).length >= 8) {
            return 'Cheater! Cheater! Cheater!';
          }
        }
      }
    }
  }
};
