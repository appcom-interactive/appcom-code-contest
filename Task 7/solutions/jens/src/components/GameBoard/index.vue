  <template>
  <div class="world-center-wrapper">
    <div
      class="row"
      v-if="copiedWorld"
      id="world"
      :style="{
          'grid-template-rows': `repeat(${worldMeta.rows}, 62px)`,
          'grid-template-columns': `repeat(${worldMeta.cols}, 62px)`
        }"
    >
      <template v-for="row in worldMeta.rows">
        <div v-for="col in worldMeta.cols" :key="`${row}-${col}`" class="block">
          <Tile
            :value="getValue(row, col)"
            :coordinates="`${row} / ${col}`"
            :shouldHighlight="shouldHighlight(row, col)"
            :highlightAllowed="highlightInformation(row, col).allowed"
            :tileSpecificClasses="tileSpecificClasses(row, col)"
            :showTutorial="showTutorial"
            :hideTile="moveTile.y === row && moveTile.x === col"
          />
        </div>
      </template>
      <GoalTile :position="goalTilePosition"/>
      <PlayerTile
        :flipPlayer="this.flipPlayer"
        :moving="keydown"
        :pickBlock="pickBlock"
        @animation-finished="animationFinished"
      />
      <MoveableTile v-if="moveTile.y !== 0 && moveTile.x !== 0" :position="moveTile"/>
    </div>
  </div>
</template>

<script>
import { mapState, mapGetters, mapMutations } from 'vuex';

import Tile from './Tile';
import PlayerTile from './PlayerTile';
import GoalTile from './GoalTile';
import MoveableTile from './MoveableTile';

import worlds from '../../worlds.json';

export default {
  components: {
    Tile,
    PlayerTile,
    GoalTile,
    MoveableTile
  },
  data() {
    return {
      store: [],
      copiedWorld: null,
      swapPlayer: false,
      flipPlayer: true,
      highlight: [],
      moving: true,
      keydown: false,
      starting: true,
      showTutorial: true,
      pickBlock: null,
      moveTile: {
        x: 0,
        y: 0
      },
      goalTilePosition: {
        x: 0,
        y: 0
      }
    };
  },
  watch: {
    worldId() {
      this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
      this.starting = true;

      this.setPlayer(this.playerPosition);

      const y = this.copiedWorld.findIndex(row => row.some(col => col === 'X'));
      const x = this.copiedWorld[y].findIndex(col => col === 'X');

      this.goalTilePosition = { x, y };
    },
    reset(value) {
      if (value) {
        this.setReset(false);
        this.resetShiftLeft();

        this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
        this.setPlayer(this.playerPosition);

        this.starting = true;
        this.keydown = false;

        const y = this.copiedWorld.findIndex(row => row.some(col => col === 'X'));
        const x = this.copiedWorld[y].findIndex(col => col === 'X');

        this.goalTilePosition = { x, y };
      }
    }
  },
  computed: {
    worldMeta() {
      const world = this.copiedWorld;

      if (world) {
        return {
          rows: world.length,
          cols: world[0].length
        };
      }
      return null;
    },
    playerPosition() {
      const y = this.copiedWorld.findIndex(row => row.some(col => col === 'S'));
      const x = this.copiedWorld[y].findIndex(col => col === 'S');

      return { x, y };
    },
    finished: {
      get() {
        return this.$store.getters.finished;
      },
      set(value) {
        this.$store.commit('setFinished', value);
      }
    },
    ...mapGetters(['selectedWorld', 'worldId', 'debug', 'reset'])
  },
  mounted() {
    document.addEventListener('keyup', () => {
      this.moving = true;
    });
    document.addEventListener('keydown', this.handleKeyDown);

    this.$store.state.worlds = worlds;

    if (this.worldId) {
      this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
      this.setPlayer(this.playerPosition);

      const y = this.copiedWorld.findIndex(row => row.some(col => col === 'X'));
      const x = this.copiedWorld[y].findIndex(col => col === 'X');

      this.goalTilePosition = { x, y };
    }

    setInterval(() => {
      const removeIndexes = [];

      this.highlight.forEach((h, index) => {
        h.dismiss -= 100;

        if (h.dismiss <= 0) {
          removeIndexes.push(index);
        }
      });

      removeIndexes.forEach(i => {
        this.highlight.splice(i, 1);
      });
    }, 100);
  },
  methods: {
    animationFinished() {
      this.keydown = false;
    },
    isAnyBlockPickable() {
      const { x, y } = this.playerPosition;

      const left = this.getValue(y + 1, x, true);
      const right = this.getValue(y + 1, x + 2, true);

      if (this.pickBlock !== null) {
        this.moveTile.x = 0;
        this.moveTile.y = 0;
        return false;
      } else if (left === 'P') {
        this.pickBlock = 'left';
        this.moveTile.x = x;
        this.moveTile.y = y + 1;

        return true;
      } else if (right === 'P') {
        this.pickBlock = 'right';
        this.moveTile.x = x + 2;
        this.moveTile.y = y + 1;

        return true;
      }

      return false;
    },
    handleKeyDown(event) {
      if (this.finished || this.keydown) {
        return;
      }

      if ([37, 39].indexOf(event.keyCode) > -1) {
        event.preventDefault();
        this.keydown = true;
        this.starting = false;
        this.showTutorial = false;
      }

      if (event.keyCode === 32) {
        if (this.isAnyBlockPickable()) {
          this.setMessage('Du kannst den Block jetzt verschieben!');
        } else if (this.pickBlock !== null) {
          this.pickBlock = null;
          this.setMessage('Der Block wurde losgelassen!');
        } else {
          this.setMessage('Es ist kein verschiebbarer Block in der Nähe!');
        }
      }

      if (!this.moving) {
        this.keydown = false;
        return;
      }

      this.moving = false;

      setTimeout(() => {
        this.moving = true;
      }, 700);

      switch (event.code) {
        case 'ArrowLeft': {
          this.movePlayer('left');
          break;
        }
        case 'ArrowRight': {
          this.movePlayer('right');
          break;
        }
      }
    },
    getValue(y, x, check = false) {
      y -= 1;
      x -= 1;

      if (this.copiedWorld) {
        if (x < 0 || y < 0) {
          return '1';
        }

        if (x > this.copiedWorld.length + 2 || y > this.copiedWorld[0].length + 2) {
          return '1';
        }

        if (check) {
          this.highlight.push({
            x: x + 1,
            y: y + 1,
            allowed: false,
            dismiss: 1000
          });
        }

        if (this.copiedWorld[y]) {
          return this.copiedWorld[y][x];
        }
      }

      return '1';
    },
    movePlayer(direction) {
      const { x, y } = this.playerPosition;

      switch (direction) {
        case 'left':
          this.flipPlayer = true;
          this.setPlayerPosition({ x: x - 1, y });
          break;
        case 'right':
          this.flipPlayer = false;
          this.setPlayerPosition({ x: x + 1, y });
          break;
      }
    },
    setPlayerPosition(newPos) {
      const old = this.playerPosition;

      if (this.canMove(newPos)) {
        if (old.y !== newPos.y) {
          const row = this.copiedWorld[old.y].slice(0);
          const rowNew = this.copiedWorld[newPos.y].slice(0);

          row[old.x] = '0';
          rowNew[old.x] = 'S';

          console.log('moving player upwards');

          this.$set(this.copiedWorld, old.y, row);
          this.$set(this.copiedWorld, newPos.y, rowNew);
        }

        if (old.x !== newPos.x) {
          if (newPos.x < old.x) {
            this.increaseShiftLeft();
          } else {
            this.decreaseShiftLeft();
          }

          const row = this.copiedWorld[newPos.y].slice(0);

          if (row[newPos.x - 1] === 'P' && !this.pickBlock) {
            this.setMessage('Den Block links von dir kannst du bewegen ;)');
          } else if (row[newPos.x + 1] === 'P' && !this.pickBlock) {
            this.setMessage('Den Block rechts von dir kannst du bewegen ;)');
          }

          if (this.pickBlock) {
            if (this.pickBlock === 'left') {
              console.log('moving block to the left');

              row[newPos.x] = '0';
              row[newPos.x - 1] = 'P';

              this.moveTile.x = newPos.x;
              this.moveTile.y = newPos.y + 1;
            } else if (this.pickBlock === 'right') {
              console.log('moving block to the left');

              row[newPos.x] = '0';
              row[newPos.x + 1] = 'P';

              this.moveTile.x = newPos.x;
              this.moveTile.y = newPos.y + 1;
            }
          }

          row[old.x] = '0';
          row[newPos.x] = 'S';

          this.$set(this.copiedWorld, newPos.y, row);
        }

        this.pullPlayer();
      } else if (this.canMove({ x: newPos.x, y: newPos.y - 1 })) {
        this.setPlayerPosition({ x: newPos.x, y: newPos.y - 1 });
      } else {
        this.keydown = false;
      }

      this.setPlayer(this.playerPosition);
    },
    pullPlayer() {
      const { x, y } = this.playerPosition;
      if (this.canMove({ x, y: y + 1 })) {
        this.setPlayerPosition({ x, y: y + 1 });
      }
    },
    canMove(newPos) {
      const allowedEntities = ['X', '0'];

      if (this.pickBlock) {
        allowedEntities.push('P');
      }

      const entity = this.getValue(newPos.y + 1, newPos.x + 1);

      this.highlight.push({
        x: newPos.x + 1,
        y: newPos.y + 1,
        allowed: allowedEntities.includes(entity),
        dismiss: 500
      });

      if (entity === 'X') {
        this.finished = true;
      }

      return allowedEntities.includes(entity);
    },
    shouldHighlight(row, col) {
      return this.highlight.some(h => h.x === col && h.y == row);
    },
    highlightInformation(row, col) {
      return this.highlight.find(h => h.x === col && h.y == row) || { allowed: false };
    },
    tileSpecificClasses(row, col) {
      const data = {
        underlying: this.getValue(row - 1, col) === this.getValue(row, col) || row === 1,
        diagonalRight: this.getValue(row - 1, col + 1) !== '1',
        diagonalLeft: this.getValue(row - 1, col - 1) !== '1',
        airLeft: this.getValue(row, col - 1) !== '1',
        airRight: this.getValue(row, col + 1) !== '1',
        flip: this.getValue(row, col) === 'S' && this.flipPlayer,
        moving: this.keydown,
        starting: this.starting
      };

      if (data.airLeft) {
        delete data.diagonalLeft;
      }
      if (data.airRight) {
        delete data.diagonalRight;
      }

      return data;
    },
    ...mapMutations([
      'increaseShiftLeft',
      'decreaseShiftLeft',
      'resetShiftLeft',
      'setReset',
      'setPlayer',
      'setMessage'
    ]),
    ...mapState(['worlds', 'shiftLeft'])
  }
};
</script>

<style lang="scss" scoped>
div.world-center-wrapper {
  position: relative;
  z-index: 2;
  display: inline-block;
  left: 50%;
  transform: translateX(-50%);

  div#world {
    display: grid;

    div.block {
      position: relative;
      box-sizing: border-box;
      width: 100%;
      height: 100%;

      &.grid {
        border: 1px dotted lightgray;
      }

      &.editing {
        display: grid;
        align-items: center;
        justify-content: center;
      }

      &:hover {
        .hoverable {
          display: block;
        }
      }

      .hoverable {
        position: absolute;
        display: none;
        top: 0;
        right: 0;
        bottom: 0;
        left: 0;
        background: rgba(0, 0, 0, 0.3);
        cursor: crosshair;
      }

      input {
        font-size: 22px;
        -webkit-appearance: none;
        border: 0;
      }
    }
  }
}
</style>
