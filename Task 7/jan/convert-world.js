const fs = require('fs');
const worlds = require("./../worlds.json");

let level = 1;
worlds.map(world => {
  const worldBlockCount = world.world.length * world.world[0].length;
  const worldNameBuffer = Buffer.from(world.title);
  const worldBuffer = Buffer.alloc(3 + worldNameBuffer.length + worldBlockCount);
  worldBuffer.writeUInt8(world.world[0].length, 0);
  worldBuffer.writeUInt8(world.world.length, 1);
  worldBuffer.writeUInt8(worldNameBuffer.length, 2);
  worldNameBuffer.copy(worldBuffer, 3);
  let offset = worldNameBuffer.length + 3;
  world.world.map(row => {
    row.map(block => {
      switch (block) {
        case '1':
          worldBuffer.writeUInt8(1, offset);
          break;
        case '0':
          worldBuffer.writeUInt8(0, offset);
          break;
        case 'S':
          worldBuffer.writeUInt8(2, offset);
          break;
        case 'X':
          worldBuffer.writeUInt8(3, offset);
          break;
        default:
          worldBuffer.writeUInt8(0, offset);
      }
      offset++;
    });
  });
  fs.writeFileSync(`${__dirname}/assets/worlds/world_${level++}.wld`, worldBuffer);
  console.log(`World "${world.title}" has size ${world.world[0].length}x${world.world.length}\n`);
});