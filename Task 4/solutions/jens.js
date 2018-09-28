const validate = (row, coin) =>
  row.reduce((count, value) => {
    if (count < 4) {
      value === coin ? count++ : (count = 0);
    }
    return count;
  }, 0);

const check = (board, x, y, coin) =>
  validate(board[y], coin) ||
  validate(board.map(row => row[x]), coin) ||
  diagonal(board.slice(y), x, coin);

const diagonal = (board, x, coin) => {
  const diagonal = board.reduce(
    (acc, row) => {
      acc.left.push(row[x - acc.left.length]);
      acc.right.push(row[x + acc.right.length]);
      return acc;
    },
    { left: [], right: [] }
  );

  return validate(diagonal.left, coin) || validate(diagonal.right, coin);
};

const countCoins = (board, coin) =>
  board.reduce(
    (coins, row) => (coins += row.filter(value => value === coin).length),
    0
  );

const messages = {
  cheater: "Cheater! Cheater! Cheater!",
  wrong_coin: "Go away! Strange kid!"
};

module.exports = {
  checkGame(board) {
    if (countCoins(board, "B") > countCoins(board, "A")) {
      return messages.cheater;
    }

    if (board.some(row => row.some(col => ![null, "A", "B"].includes(col)))) {
      return messages.wrong_coin;
    }

    const reversed = board.reverse();
    const winner = reversed.reduce(
      (result, row, y) =>
        row.find((coin, x) => coin && check(reversed, x, y, coin)) || result,
      undefined
    );

    return winner ? `Player ${winner} wins!` : messages.cheater;
  }
};
