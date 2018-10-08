const rowToByte = b => (b[0] << 6) | (b[1] << 4) | (b[2] << 2) | b[3];

module.exports = {
  checkGame(gameFieldMtx) {
    const counts = { A: 0, B: 0, C: 0 };
    const ALLOWED_VALUES = new Map([[null, 0x0], ['A', 0x1], ['B', 0x2]]);
    const flatMtx = gameFieldMtx.reduce((arr, row) => arr.concat(row), []).map(e => {
      if (!ALLOWED_VALUES.has(e)) {
        counts.C++;
        return 0;
      }
      if (e) counts[e]++;
      return ALLOWED_VALUES.get(e);
    });
    if (counts.C > 0) return 'Go away! Strange kid!';
    if (counts.B > counts.A) return 'Cheater! Cheater! Cheater!';
    for (let yOffset = 0; yOffset <= 14; yOffset += 7) {
      for (let xOffset = 0; xOffset <= 3; xOffset++) {
        const cOffset = yOffset + xOffset;
        const bytes = [cOffset, cOffset+7, cOffset+14, cOffset+21].map(offset => rowToByte(flatMtx.slice(offset, offset + 4)));
        const originalMat = parseInt(bytes.reduce((acc, byte) => `${acc}` + `00${byte.toString(16)}`.slice(-2), ''), 16);
        const vByte4 = (bytes[3] & 0xC0) | ((bytes[2] & 0xC0) >> 2) | ((bytes[1] & 0xC0) >> 4) | ((bytes[0] & 0xC0) >> 6);
        const vByte3 = ((bytes[3] & 0x30) << 2) | (bytes[2] & 0x30) | ((bytes[1] & 0x30) >> 2) | ((bytes[0] & 0x30) >> 4);
        const vByte2 = ((bytes[3] & 0x0C) << 4) | ((bytes[2] & 0x0C) << 2) | (bytes[1] & 0x0C) | ((bytes[0] & 0x0c) >> 2);
        const vByte1 = ((bytes[3] & 0x03) << 6) | ((bytes[2] & 0x03) << 4) | ((bytes[1] & 0x03) << 2) | (bytes[0] & 0x03);
        const rotMat = parseInt([vByte4, vByte3, vByte2, vByte1].reduce((acc, byte) => `${acc}` + `00${byte.toString(16)}`.slice(-2), ''), 16);
        const cheater = [vByte4, vByte3, vByte2, vByte1].map(e => e.toString(2).match(/10{3,}1/)).filter(m => m);
        if (cheater.length) return 'Cheater! Cheater! Cheater!';
        if ([originalMat, rotMat].map(w => (w & 0x80200802)>>>0).includes(0x80200802)) return 'Player B wins!';
        if (bytes.concat([vByte4, vByte3, vByte2, vByte1]).map(b => b & 0xAA).includes(0xAA)) return 'Player B wins!';
        if ([originalMat, rotMat].map(w => (w & 0x40100401)>>>0).includes(0x40100401)) return 'Player A wins!';
        if (bytes.concat([vByte4, vByte3, vByte2, vByte1]).map(b => b & 0x55).includes(0x55)) return 'Player A wins!';
      }
    }
  }
};
