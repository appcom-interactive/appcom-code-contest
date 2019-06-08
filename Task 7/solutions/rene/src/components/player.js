export default class Player extends Phaser.Physics.Arcade.Sprite {
  constructor(scene, player) {
    super(scene, player.x, player.y, player.texture);
    this.ratio = 70 / this.height;
    this.setOrigin(0, 0);
    console.log(this);

    scene.add.existing(this);

    scene.physics.add.existing(this);

    this.body.setBounce(0.2);
    this.body.setCollideWorldBounds(true);

    this.setScale(this.ratio);

    scene.anims.create({
      key: 'walk',
      frames: scene.anims.generateFrameNames('player', {
        prefix: 'p1_walk',
        start: 1,
        end: 11,
        zeroPad: 2
      }),
      frameRate: 10,
      repeat: -1
    });

    scene.anims.create({
      key: 'idle',
      frames: [{ key: 'player', frame: 'p1_stand' }],
      frameRate: 10
    });

    scene.anims.create({
      key: 'jump',
      frames: [{ key: 'player', frame: 'p1_jump' }],
      frameRate: 10
    });
  }

  update(cursors) {
    if (cursors.left.isDown) {
      console.log(this.scene);
      this.body.setVelocityX(-200);
      this.setFlipX(true);
      if (this.body.onFloor()) {
        this.anims.play('walk', true);
      }
    } else if (cursors.right.isDown && this.body.onFloor()) {
      this.body.setVelocityX(200);
      this.setFlipX(false);
      if (this.body.onFloor()) {
        this.anims.play('walk', true);
      }
    } else if (this.body.onFloor()) {
      this.body.setVelocityX(0);
      this.anims.play('idle', true);
    }
    if ((cursors.space.isDown || cursors.up.isDown) && this.body.onFloor()) {
      this.body.setVelocityY(-300);
      this.anims.play('jump', true);
    }
  }
}
