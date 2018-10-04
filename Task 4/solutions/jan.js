const gameFieldToValue = (field) => {
  switch (field) {
    case null:
      return 0;
    case 'A':
      return 1;
    case 'B':
      return -1;
    default:
      throw 'Go away! Strange kid!';
  }
};

module.exports = {
  checkGame(gameFieldMtx) {
    try {
      const mappedValue = gameFieldMtx.map(layerA => layerA.map(gameFieldToValue));
      const gameFieldValue = mappedValue.reduce((acc, layer) => acc + layer.reduce((accL, field) => accL + field, 0), 0);
      if (gameFieldValue < 0) {
        return 'Cheater! Cheater! Cheater!';
      }
      const cheaterRegex = /[AB]X[AB]/;
      const playerRegex = /(A{4}|B{4})/;
      const gameFieldTransposed = gameFieldMtx[0].map((_, idx) => gameFieldMtx.map(row => row[idx]));
      let result;
      gameFieldTransposed.forEach(layer => {
        const filledLayer = layer.map(e => !e ? 'X' : e).join('');
        if (cheaterRegex.test(filledLayer)) {
          throw 'Cheater! Cheater! Cheater!';
        }
        if ((result = filledLayer.match(playerRegex)) !== null) {
          throw `Player ${result[0].slice(-1)} wins!`;
        }
      });
      gameFieldMtx.forEach(layer => {
        const filledLayer = layer.map(e => !e ? 'X' : e).join('');
        if ((result = filledLayer.match(playerRegex)) !== null) {
          throw `Player ${result[0].slice(-1)} wins!`;
        }
      });
      const rotatedMatrix = [];
      for (let y = 0; y < gameFieldMtx.length; y++) {
        for (let x = 0; x < gameFieldMtx[y].length; x++) {
          if (!rotatedMatrix[x+y]) rotatedMatrix[x+y] = [];
          rotatedMatrix[x + y].push(!gameFieldMtx[y][x] ? 'X' : gameFieldMtx[y][x]);
        }
      }
      rotatedMatrix
        .filter(layer => layer.length >= 4)
        .map(layer => layer.join(''))
        .forEach(layer => {
          if ((result = layer.match(playerRegex)) !== null) {
            throw `Player ${result[0].slice(-1)} wins!`;
          }
        })
    } catch (e) {
      return e;
    }
  }
};
