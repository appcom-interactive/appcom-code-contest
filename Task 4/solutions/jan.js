const gameFieldToValue = (field) => {
  switch (field) {
    case null:
      return 0;
    case "A":
      return 1;
    case "B":
      return -1;
    default:
      throw new Error("Go away! Strange kid!");
  }
};

module.exports = {
  checkGame(value) {
    let mappedValue = null;
    try {
      mappedValue = value.map(layerA => layerA.map(gameFieldToValue));
    } catch (e) {
      return e.message;
    }
    const gameFieldValue = mappedValue.reduce((acc, layer) => acc + layer.reduce((accL, field) => accL + field, 0), 0);
    if (gameFieldValue < 0) {
      return "Cheater! Cheater! Cheater!";
    }
    let aCount, bCount = 0;

    for (let x = 0; x < 7; x++) {
      aCount = 0;
      bCount = 0;
      for (let y = 0; y < 6; y++) {
        const valAtXY = mappedValue[y][x];
        switch (valAtXY) {
          case 1:
            if (++aCount === 4) {
              return 'Player A wins!'
            }
            bCount = 0;
            break;
          case -1:
            if (++bCount === 4) {
              return 'Player B wins!'
            }
            aCount = 0;
            break;
          case 0:
            if ((y - 1) >= 0 && mappedValue[y - 1][x] !== 0) {
              return "Cheater! Cheater! Cheater!";
            }
        }
      }
    }

    for (let y = 0; y < 6; y++) {
      aCount = 0;
      bCount = 0;
      for (let x = 0; x < 7; x++) {
        const valAtXY = mappedValue[y][x];
        switch (valAtXY) {
          case 1:
            if (++aCount === 4) {
              return 'Player A wins!'
            }
            bCount = 0;
            break;
          case -1:
            if (++bCount === 4) {
              return 'Player B wins!'
            }
            aCount = 0;
            break;
        }
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 3, x = 0; y >= 0, x < 4; x++, y--) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 4, x = 0; y >= 0, x < 5; x++, y--) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 5, x = 0; y >= 0, x < 6; x++, y--) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 0, x = 6; y < 6, x >= 1; x--, y++) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 1, x = 6; y < 6, x >= 2; x--, y++) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }

    aCount = 0;
    bCount = 0;
    for (let y = 2, x = 6; y < 6, x >= 3; x--, y++) {
      const valAtXY = mappedValue[y][x];
      switch (valAtXY) {
        case 1:
          if (++aCount === 4) {
            return 'Player A wins!'
          }
          bCount = 0;
          break;
        case -1:
          if (++bCount === 4) {
            return 'Player B wins!'
          }
          aCount = 0;
          break;
      }
    }
  }
};
