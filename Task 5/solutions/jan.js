const TIME_MAP = new Map([
  ['E', 15], ['H',  5],
  ['K', 10], ['S', 25],
  ['L', 15], ['G', 50]
]);

module.exports = {
  drillCheck(plan) {
    const maxZ = plan[0].length;
    const maxX = plan[0][0].length;
    const buffer = Buffer.alloc(maxX * maxZ * 2, 0);
    try {
      let offset = 0;
      plan.forEach((layer, y) => {
        layer.forEach(stripe => {
          stripe.forEach(field => {
            if (!TIME_MAP.has(field)) throw new Error('Unvollständige Geländedaten');
            let valueAt = buffer.readUInt16LE(offset - (y * maxX * maxZ * 2));
            valueAt += TIME_MAP.get(field);
            buffer.writeUInt16LE(valueAt, offset - (y * maxX * maxZ * 2));
            offset += 2;
          });
        });
      });
    } catch (e) {
      return e.message;
    }
    const jsArray = [];
    for (let i = 0; i < buffer.length / 2; i++) {
      jsArray.push(buffer.readUInt16LE(i * 2) / 10);
    }
    const result = jsArray.reduce(
      (prev, current, index) => {
        const x = Math.ceil(index / maxX);
        const z = maxZ - ((x * maxX) / maxZ) + 1;
        if (prev[0].t > current) {
          return [{ x, z, t: current }];
        }
        if (prev[0].t === current) {
          return prev.concat([{ x, z, t: current }]);
        }
        return prev;
      }, [{ x: 0, z: 0, t: 200 }]
    ).filter(current => current.t < 10);
    if (result.length === 0) return 'Unwirtschaftliches Gelände';
    return result.length === 1 ? result[0] : result;
  }
};
