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
        <div class="tile">
          <div
            v-if="(getValue(row, col) === 'S' && !finished) || getValue(row, col) !== 'S'"
            class="tile"
            :class="[`tile-${getValue(row, col)}`, tileSpecificClasses(row, col)]"
          ></div>

          <div v-if="debug.showTileIDs" class="text">{{getValue(row, col)}}</div>
          <div v-else-if="debug.showCoordinates" class="coordinates">{{row}} / {{col}}</div>

          <transition name="highlight-transition">
            <div v-if="getValue(row, col) === 'S' && showTutorial" class="tutorial">
              <p>Willkommen zu Block Mate!</p>
              <br>
              <p>Benutze die Pfeiltasten um dich in der Spielwelt zu bewegen.</p>
            </div>
          </transition>

          <transition name="highlight-transition">
            <div v-if="getValue(row, col) === 'X' && showTutorial" class="tutorial">
              <p>Erreiche die Tür!</p>
            </div>
          </transition>

          <template v-if="debug.collisionTracking">
            <transition name="highlight-transition">
              <div
                v-if="shouldHighlight(row, col)"
                class="highlight"
                :class="{ allowed: highlightInformation(row, col).allowed}"
              ></div>
            </transition>
          </template>
        </div>
      </div>
    </template>
  </div>
</template>

<script>
import { mapState, mapGetters, mapMutations } from 'vuex';

import worlds from '../worlds.json';

export default {
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

    div.tile {
      width: 100%;
      height: 100%;
    }
  }
}

div.tile {
  &.tile- {
    &1 {
      background: url('../assets/tileset.png') no-repeat;
      background-size: initial;
      background-position: -46px -30px;
      background-size: 1200px;

      &.underlying {
        background-position: -46px -78px;
      }
    }

    &S {
      background: url('../assets/player_idle.gif') no-repeat center;
      background-size: contain;

      &.moving {
        background: url('../assets/run.gif') no-repeat center;
        background-size: contain;
      }

      &.flip {
        transform: scaleX(-1);
      }

      &.starting {
        background: url('../assets/midair.gif') no-repeat center;
        background-size: contain;
      }
    }

    &X {
      z-index: 99;
      position: relative;

      &:before {
        content: '';
        position: absolute;
        background: rgb(107, 46, 18);
        width: 50%;
        height: 100%;
        left: 50%;
        transform: translateX(-50%);
        border: 1px solid rgba(0, 0, 0, 0.3);
        box-shadow: 0 0 0 rgb(177, 60, 5);
        animation: pulse 2s infinite;

        z-index: 99;
      }

      &:after {
        content: '';
        position: absolute;
        left: 50%;
        top: 0;
        width: 1px;
        height: 100%;
        background: rgba(0, 0, 0, 0.3);
        transform: translateX(-50%);
        z-index: 100;
      }
    }
  }

  position: relative;

  .highlight {
    &:after {
      content: '';
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      left: 0;
      background-color: rgba(255, 0, 0, 0.15);
    }

    &.allowed:after {
      background-color: rgba(0, 255, 21, 0.15);
    }
  }

  .text,
  .coordinates {
    position: absolute;
    color: rgba(255, 255, 255, 0.6);
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
  }

  .text {
    font-size: 24px;
  }

  .coordinates {
    font-size: 12px;
  }

  .tutorial {
    position: absolute;
    bottom: calc(100% + 15px);
    left: 50%;
    transform: translateX(-50%);
    background: white;
    padding: 10px;
    border-radius: 3px;
    background-color: rgb(184, 220, 204);
    box-shadow: 2px 2px 5px 5px rgba(0, 0, 0, 0.3);
    color: black;

    p:first-child {
      white-space: nowrap;
    }

    &:after {
      position: absolute;
      content: '';

      width: 0;
      height: 0;
      left: 50%;
      bottom: -10px;
      transform: translateX(-50%);
      border-left: 10px solid transparent;
      border-right: 10px solid transparent;

      border-top: 10px solid rgb(184, 220, 204);
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
