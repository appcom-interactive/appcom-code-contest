export default class TilesSingle extends Phaser.Physics.Arcade.Sprite {
  constructor(scene, x, y, texture) {
    super(scene, x, y, texture);
    this.ratio = 70 / this.width;
    this.width = 70;
    this.height = 70;
    this.setOrigin(0, 0);

    scene.add.existing(this);

    scene.physics.add.existing(this, true);

    this.setScale(this.ratio);
  }
}
