module.exports = {
  checkGame(value) {
    let isWinningRow,
      isWinningCol,
      isWinningDia,
      isWrongCoin,
      isInvalidBoard,
      aWins,
      bWins = false;

    // switch case alternative with object literals
    const showAnswer = type => {
      const types = {
        0: "nix geht hier",
        1: "Player A wins!",
        2: "Player B wins!",
        3: "Cheater! Cheater! Cheater!",
        4: "Go away! Strange kid!"
      };
      return types[type];
    };

    // flip array by 90 deg
    const flipArray = value[0].map(function(col, i) {
      return value.map(function(row) {
        return row[i];
      });
    });

    // check diagos
    const getDiagonals = m => {
      let s,
        x,
        y,
        d,
        o = [];
      for (s = 0; s < m.length; s++) {
        d = [];
        for (y = s, x = 0; y >= 0; y--, x++) d.push(m[y][x]);
        o.push(d.join(""));
      }
      for (s = 1; s < m[0].length; s++) {
        d = [];
        for (y = m.length - 1, x = s; x < m[0].length; y--, x++)
          d.push(m[y][x]);
        o.push(d.join(""));
      }
      return o;
    };

    const diagos = getDiagonals(value);

    if (diagos.includes("AAAA")) {
      isWinningDia = true;
      aWins = true;
    }
    if (diagos.includes("BBBB")) {
      isWinningDia = true;
      bWins = true;
    }

    // check wrong coins
    value.map(row => {
      row.map(col => {
        isWrongCoin = col === "B" || col === "A" || col === null;
      });
    });

    // check for holes
    let tmpCol = null;
    flipArray.map(row => {
      row.map(col => {
        // const isValidCoin = el => el === "B" || el === "A" || el === null;
        // if (!isValidCoin(col)) {
        //   isWrongCoin = true;
        // }
        if (tmpCol !== null && !(col !== null)) {
          isInvalidBoard = true;
        }
        tmpCol = col;
      });
    });

    // check for double coin toss
    let [countA, countB] = [0, 0];
    value.map(row => {
      row.map(col => {
        if (col === "A") {
          countA++;
        }
        if (col === "B") {
          countB++;
        }
      });
    });
    const isDoubleToss = countB > countA || countB + 1 < countA;

    // check winning row
    for (let index = 0; index < value.length; index++) {
      const el = value[index].join("");
      if (el.includes("AAAA")) {
        isWinningRow = true;
        aWins = true;
      }
      if (el.includes("BBBB")) {
        isWinningRow = true;
        bWins = true;
      }
    }

    // check winning col
    for (let index = 0; index < flipArray.length; index++) {
      const el = flipArray[index].join("");
      if (el.includes("AAAA")) {
        isWinningCol = true;
        aWins = true;
      }
      if (el.includes("BBBB")) {
        isWinningCol = true;
        bWins = true;
      }
    }

    // check for all states and decide answer
    if (isWinningRow || isWinningCol || isWinningDia) {
      return aWins ? showAnswer(1) : showAnswer(2);
    }
    if (isDoubleToss || isInvalidBoard) {
      return showAnswer(3);
    }
    if (isWrongCoin) {
      // check me
      return showAnswer(4);
    }
  }
};
