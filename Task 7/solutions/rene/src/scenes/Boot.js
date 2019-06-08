import { Scene } from 'phaser';

export default class BootScene extends Scene {
  constructor() {
    super({ key: 'BootScene' });
  }

  preload() {
    const images = ['tile-single'];
    images.forEach(img => {
      this.load.image(img, `assets/img/${img}.png`);
    });
    this.load.image('coin', `assets/img/coin.png`);
    this.load.atlas('player', 'assets/player.png', 'assets/player.json');
  }

  create() {
    this.scene.start('PlayScene');
  }
}
