export default class Coin extends Phaser.Physics.Arcade.Sprite {
  constructor(scene, coin) {
    super(scene, coin.x, coin.y, coin.texture);
    this.ratio = 70 / this.height;
    this.setOrigin(0, 0);
    console.log(this);

    scene.add.existing(this);

    scene.physics.add.existing(this, true);

    this.setScale(this.ratio);
    this.setImmovable();

  }
}
