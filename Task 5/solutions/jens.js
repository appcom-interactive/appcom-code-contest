const DRILL_TIMES = {
  E: 1.5,
  H: 0.5,
  K: 1,
  S: 2.5,
  L: 1.5,
  G: 5
};

const ALLOWED_TYPES = Object.keys(DRILL_TIMES);
const MAX_DRILL_TIME = 10;

const areAllLayersValid = layer =>
  !layer.some(plan =>
    plan
      .reduce((acc, val) => acc.concat(val), [])
      .some(element => !ALLOWED_TYPES.includes(element))
  );

const countDrillTimes = plan => {
  const drillTimes = plan[0].map(plan => plan.map(() => 0));

  plan.forEach(layer => {
    layer.forEach((row, z) => {
      row.forEach((type, x) => {
        drillTimes[z][x] += DRILL_TIMES[type.toUpperCase()];
      });
    });
  });

  const lowest = drillTimes.reduce((time, z) => {
    z.forEach(y => {
      if (y < time) time = y;
    });
    return time;
  }, MAX_DRILL_TIME);

  return drillTimes.reverse().reduce((pos, row, z) => {
    row.forEach((col, x) => {
      if (col === lowest) {
        pos.push({ x: x + 1, z: z + 1, t: col });
      }
    });

    return pos;
  }, []);
};

module.exports = {
  drillCheck(plan) {
    if (areAllLayersValid(plan)) {
      const positions = countDrillTimes(plan);
      switch (positions.length) {
        case 0:
          return "Unwirtschaftliches Gelände";
        case 1:
          return positions.pop();
        default:
          return positions.reverse();
      }
    } else {
      return "Unvollständige Geländedaten";
    }
  }
};
