import Levels from '../levels';

export default class Map {
  constructor(currentLevel) {
    this.currentLevel = currentLevel;
    this.map = Levels[this.currentLevel];

    const TILE_SIZE = 70;
    const config = {
      '1': {
        type: 'tile',
        texture: 'tile-single'
      },
      S: {
        type: 'player',
        texture: 'player'
      },
      X: {
        type: 'coin',
        texture: 'coin'
      }
    };

    this.size = {
      x: 0,
      y: 0,
      width: this.map[0].length * TILE_SIZE,
      height: this.map.length * TILE_SIZE
    };

    this.info = this.map.reduce((info, row, rowIndex) => {
      row.forEach((column, columnIndex) => {
        const tile = row[columnIndex];
        if (tile !== 0) {
          info.push({
            ...config[tile],
            x: columnIndex * TILE_SIZE,
            y: rowIndex * TILE_SIZE
          });
        }
      });
      return info;
    }, []);
  }
}
