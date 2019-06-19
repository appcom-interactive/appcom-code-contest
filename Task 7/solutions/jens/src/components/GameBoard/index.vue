<template>
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
        />
      </div>
    </template>
  </div>
</template>

<script>
import { mapState, mapGetters, mapMutations } from 'vuex';

import Tile from './Tile';

export default {
  components: {
    Tile
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
      showTutorial: true
    };
  },
  watch: {
    worldId() {
      this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
      this.starting = true;
    },
    reset(value) {
      if (value) {
        this.setReset(false);
        this.resetShiftLeft();

        this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
        this.starting = true;
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
      this.keydown = false;
    });
    document.addEventListener('keydown', this.handleKeyDown);

    this.$store.state.worlds = worlds;

    if (this.worldId) {
      this.copiedWorld = this.selectedWorld.world.map(w => [...w]);
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
    handleKeyDown(event) {
      if (this.finished) {
        return;
      }

      if ([32, 37, 38, 39, 40].indexOf(event.keyCode) > -1) {
        event.preventDefault();
        this.keydown = true;
        this.starting = false;
        this.showTutorial = false;
      }

      if (!this.moving) {
        return;
      }

      this.moving = false;

      setTimeout(() => {
        this.moving = true;
      }, 150);

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
      }
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
      return this.highlight.find(h => h.x === col && h.y == row);
    },
    tileSpecificClasses(row, col) {
      return {
        underlying: this.getValue(row - 1, col) === this.getValue(row, col) || row === 1,
        flip: this.getValue(row, col) === 'S' && this.flipPlayer,
        moving: this.keydown,
        starting: this.starting
      };
    },
    ...mapMutations(['increaseShiftLeft', 'decreaseShiftLeft', 'resetShiftLeft', 'setReset']),
    ...mapState(['worlds', 'shiftLeft'])
  }
};
</script>

<style lang="scss" scoped>
div#world {
  display: grid;
  justify-content: center;
  align-items: center;

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

.highlight-transition-enter-active {
  transition: opacity 0.2s;
}
.highlight-transition-leave-active {
  transition: opacity 0.5s;
}
.highlight-transition-enter, .highlight-transition-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
</style>
