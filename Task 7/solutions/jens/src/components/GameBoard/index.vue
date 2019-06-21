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
          />
        </div>
      </template>
      <GoalTile :position="goalTilePosition"/>
      <PlayerTile
        :flipPlayer="this.flipPlayer"
        :moving="keydown"
        @animation-finished="animationFinished"
      />
    </div>
  </div>
</template>

<script>
import { mapState, mapGetters, mapMutations } from 'vuex';

import Tile from './Tile';
import PlayerTile from './PlayerTile';
import GoalTile from './GoalTile';

import worlds from '../../worlds.json';

export default {
  components: {
    Tile,
    PlayerTile,
    GoalTile
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
    getValue(y, x) {
      y -= 1;
      x -= 1;

      if (this.copiedWorld) {
        if (this.copiedWorld[y]) {
          return this.copiedWorld[y][x];
        }
      }
      return 0;
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
      return {
        underlying: this.getValue(row - 1, col) === this.getValue(row, col) || row === 1,
        flip: this.getValue(row, col) === 'S' && this.flipPlayer,
        moving: this.keydown,
        starting: this.starting
      };
    },
    ...mapMutations(['increaseShiftLeft', 'decreaseShiftLeft', 'resetShiftLeft', 'setReset', 'setPlayer']),
    ...mapState(['worlds', 'shiftLeft'])
  }
};
</script>

<style lang="scss" scoped>
div.world-center-wrapper {
  position: relative;
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
