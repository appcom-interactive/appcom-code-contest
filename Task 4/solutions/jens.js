const messages = {
  fail: {
    cheater: "Cheater! Cheater! Cheater!",
    wrong_coin: "Go away! Strange kid!"
  }
};

const allowedValues = [null, "A", "B"];

const countOccurrences = (value, coin) => {
  return value.reduce(
    (acc, val) => (acc += val.filter(v => v === coin).length),
    0
  );
};

const checkForInvalidCoins = board =>
  board.some(row => row.some(col => !allowedValues.includes(col)));

const invalidBoard = value => {
  const playerCoinsA = countOccurrences(value, "A");
  const playerCoinsB = countOccurrences(value, "B");

  if (playerCoinsB > playerCoinsA) {
    return messages.fail.cheater;
  }

  if (checkForInvalidCoins(value)) {
    return messages.fail.wrong_coin;
  }

  return false;
};

const validateArray = (row, coin) =>
  row.reduce((count, value) => {
    if (count < 4) {
      if (value === coin) {
        count++;
      } else {
        count = 0;
      }
    }
    return count;
  }, 0)
    ? coin
    : false;

const check = (value, x, y, coin) => {
  if (coin !== null) {
    return (
      validateArray(value[y], coin) || // Check horizontal
      vertical(value, x, y, coin) ||
      diagonal(value, x, y, coin)
    );
  }

  return false;
};

const vertical = (value, x, y, coin) => {
  const col = value
    .slice() // Copy array
    .splice(y) // Just get sub-array
    .reduce((acc, row) => {
      acc.push(row[x]);
      return acc;
    }, []); // Create a new row out of the col values

  return validateArray(col, coin);
};

const diagonal = (value, x, y, coin) => {
  const diagonal = value
    .slice()
    .splice(y)
    .reduce(
      (acc, val) => {
        acc.left.push(val[x - acc.left.length]);
        acc.right.push(val[x + acc.right.length]);
        return acc;
      },
      { left: [], right: [] }
    );

  return (
    validateArray(diagonal.left, coin) || validateArray(diagonal.right, coin)
  );
};

module.exports = {
  checkGame(value) {
    // Need to reverse the array to fix an failing test (line 52 onwards)
    const reversed = value.slice().reverse();

    if ((message = invalidBoard(reversed))) {
      return message;
    }

    const player = reversed.reduce((acc, row, y) => {
      if (acc) return acc;
      return row.find((coin, x) => check(reversed, x, y, coin));
    }, undefined);

    if (player) {
      return `Player ${player} wins!`;
    }

    return messages.fail.cheater;
  }
};
