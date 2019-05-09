export default class Board {
  constructor(size) {
    this.size = size;
    this.board = this.createBoard(+this.size);
    this.count = 0;
    this.solutions = new Map();
  }

  getSolutions() {
    return this.solutions;
  }

  createBoard(size) {
    return new Array(size).fill('').map(() => new Array(size).fill(''));
  }

  drawChessBoard(board = this.board) {
    board.forEach((rows, rKey) => {
      let rowContent = '';
      let head = '';

      rows.forEach((row, cKey) => {
        if (rKey === 0) {
          head += cKey + 1 + '  ';
        }

        const current = board[rKey][cKey];

        rowContent += current ? ' D ' : ' | ';
      });

      if (rKey === 0) {
        console.log('\n\n    ' + head); // eslint-disable-line
        console.log('   -----------------------'); // eslint-disable-line
      }

      console.log(rKey + 1 + ' |' + rowContent); // eslint-disable-line
    });
  }

  ladyCanBePlaced(row, col) {
    // left
    for (let i = 0; i < col; i++) {
      if (this.board[row][i] === 1) {
        return false;
      }
    }

    // bottom left
    for (let r = row, c = col; c >= 0 && r < this.board.length; r++, c--) {
      if (this.board[r][c] === 1) {
        return false;
      }
    }

    // top left
    for (let r = row, c = col; r >= 0 && c >= 0; r--, c--) {
      if (this.board[r][c] === 1) {
        return false;
      }
    }

    return true;
  }

  run(column = 0) {
    if (column === this.board.length) {
      this.count += 1;
      this.solutions.set(this.count, this.board.map(m => m.map(k => k)));
      this.drawChessBoard();
    } else {
      // run each row
      for (let row = 0; row < this.board.length; row++) {
        if (this.ladyCanBePlaced(row, column)) {
          this.board[row][column] = 1;
          this.run(column + 1);
          this.board[row][column] = 0;
        }
      }
    }
  }
}
