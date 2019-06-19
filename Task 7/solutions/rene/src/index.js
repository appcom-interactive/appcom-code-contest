import Phaser from 'phaser';
import BootScene from './scenes/Boot';
// import MainMenuScene from './scenes/Mainmenu';
import PlayScene from './scenes/Play';

var config = {
  type: Phaser.AUTO,
  width: 840,
  height: 560,
  physics: {
    default: 'arcade',
    arcade: {
      gravity: { y: 500 },
      debug: false
    }
  },
  scene: [BootScene,/* MainMenuScene,*/ PlayScene]
};

var game = new Phaser.Game(config);
