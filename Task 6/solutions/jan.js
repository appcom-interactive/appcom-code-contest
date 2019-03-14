class Field {
  constructor() {
    this.id = null;
    this.links = new Map();
    this.dame = false;
    this.blocked = false;
  }

  resolveLinks(chessboard) {
    const row = Math.floor(this.id / 8);
    const column = this.id % 8;
    for (let i = 0; i < 8; i++) {
      const horizontal = chessboard[row * 8 + i];
      const vertical = chessboard[i * 8 + column];
      if (!this.links.has(horizontal.id) && this.id !== horizontal.id) {
        this.links.set(horizontal.id, horizontal);
      }
      if (!this.links.has(vertical.id) && this.id !== vertical.id) {
        this.links.set(vertical.id, vertical);
      }
    }
    let xr = column;
    let xl = column;
    let yr = row;
    let yl = row;

    /*while ((xr + 1) < 8 && (yr + 1) < 8) {
      xr++;
      yr++;
    }

    while ((xl - 1) >= 0 && (yl - 1) >= 0) {
      xl--;
      yl--;
    }*/

    for (let x = xr, y = yr; x >= 0 &&  y >= 0; x--, y--) {
      const diag = chessboard[y * 8 + x];
      if (!this.links.has(diag.id) && this.id !== diag.id) {
        this.links.set(diag.id, diag);
      }
    }
    for (let x = xr, y = yl; x >= 0 && y < 8; x--, y++) {
      const diag = chessboard[y * 8 + x];
      if (!this.links.has(diag.id) && this.id !== diag.id) {
        this.links.set(diag.id, diag);
      }
    }
    for (let x = xl, y = yr; x < 8 && y >= 0; x++, y--) {
      const diag = chessboard[y * 8 + x];
      if (!this.links.has(diag.id) && this.id !== diag.id) {
        this.links.set(diag.id, diag);
      }
    }
    for (let x = xl, y = yl; x < 8 && y < 8; x++, y++) {
      const diag = chessboard[y * 8 + x];
      if (!this.links.has(diag.id) && this.id !== diag.id) {
        this.links.set(diag.id, diag);
      }
    }
  }

  placeDame() {
    this.dame = true;
    this.blocked = true;
    this.links.forEach(link => (link.blocked = true));
  }
}

class Chessboard {
  constructor(path = []) {
    this.chessboard = [];
    for (let i = 0; i < 64; i++) {
      const field = new Field();
      field.id = i;
      this.chessboard.push(field);
    }

    for (let i = 0; i < 64; i++) {
      this.chessboard[i].resolveLinks(this.chessboard);
    }

    path.forEach(at => this.placeDame(at));
  }

  placeDame(at) {
    this.chessboard[at].placeDame();
  }

  getPossibleFields() {
    return this.chessboard
        .filter(x => !x.blocked)
        .reduce((acc, x) => {acc.push(x.id); return acc;}, []);
  }
}

let pathes = (new Chessboard()).getPossibleFields().map(v => ([v]));
for (let w = 1; w < 8; w++) {
  pathes.map((v, i, a) => {
    const cb = new Chessboard(v);
    const base = [...v];
    cb.getPossibleFields().forEach(f => {
      a.push(base.concat([f]));
    });
  });
  const aSet = new Set();
  pathes = pathes.filter(v => v.length > w);
  pathes.map((_, i, a) => {
    const stringify = a[i].sort((x, y) => (x < y ? -1 : 1)).join(';');
    aSet.add(stringify);
  });
  pathes = Array.from(aSet.entries()).map(v => (v[0].split(';').map(v => (v * 1))));
}

module.exports = {
  getPossibleMoves() {
    return pathes.length;
  },
  getBoards() {
    return pathes;
  }
};
