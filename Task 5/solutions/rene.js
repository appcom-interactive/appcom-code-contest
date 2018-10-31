const DRILL_TIMES = {
  E: 1.5,
  H: 0.5,
  K: 1,
  S: 2.5,
  L: 1.5,
  G: 5
};

const times = plan => {
  const drillTimes = Array(plan[0].length)
    .fill()
    .map(() => Array(plan[0][0].length).fill(0));

  for (const layer of plan) {
    for (const [indexRow, row] of layer.entries()) {
      for (const [indexColumn, item] of row.entries()) {
        drillTimes[indexRow][indexColumn] += DRILL_TIMES[item.toUpperCase()];
      }
    }
  }

  var minRow = drillTimes.map(row => Math.min.apply(Math, row));
  var min = Math.min.apply(null, minRow);

  if (min > 10) {
    return 'Unwirtschaftliches Gelände';
  }

  const positions = [];

  drillTimes.forEach((row, indexRow) => {
    row.forEach((entry, indexColumn) => {
      if (drillTimes[indexRow][indexColumn] === min) {
        positions.push({
          x: indexColumn + 1,
          z: drillTimes.length - indexRow,
          t: min
        });
      }
    });
  });

  if (positions.length === 1) {
    return positions[0];
  } else if (positions.length > 1) {
    return positions;
  } else {
    return 'Unvollständige Geländedaten';
  }
};

module.exports = {
  drillCheck(plan) {
    return times(plan);
  }
};
