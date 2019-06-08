import { Scene } from 'phaser';
import TilesGroup from '../components/tiles/tilesGroup';
import Player from '../components/player';
import Coin from '../components/coin';
import Map from '../components/map';

export default class PlayScene extends Scene {
  constructor() {
    super({ key: 'PlayScene' });
  }

  create({ level }) {
    this.level = level || 0;
    const map = new Map(this.level);
    console.log(map);

    this.cameras.main.setBackgroundColor('#ade6ff');
    this.cameras.main.fadeIn();

    this.cameras.main.setBounds(
      map.size.x,
      map.size.y,
      map.size.width,
      map.size.height
    );
    this.physics.world.setBounds(
      map.size.x,
      map.size.y,
      map.size.width,
      map.size.height
    );

    this.input.addPointer(1);
    this.cursors = this.input.keyboard.createCursorKeys();

    this.tilesGroup = new TilesGroup(
      this,
      map.info.filter(el => el.type === 'tile')
    );

    this.player = new Player(this, map.info.find(el => el.type === 'player'));
    this.coin = new Coin(this, map.info.find(el => el.type === 'coin'));

    this.cursors = this.input.keyboard.createCursorKeys();

    this.physics.add.collider(this.tilesGroup, this.player);
    this.physics.add.overlap(this.player, this.coin, (player, coin) => {
      coin.destroy();
      this.cameras.main.fadeOut();
      this.time.addEvent({
        delay: 2000,
        callback: () => {
          this.scene.restart({ level: this.level + 1 });
        }
      });
    });
  }
  update() {
    this.player.update(this.cursors);
  }
}
