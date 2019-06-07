<template>
  <div class="wrapper">
    <div class="settings row left">
      <select v-model="worldId">
        <option value="0" disabled>Bitte wählen Sie eine Welt aus</option>
        <option v-for="(world, index) in worlds" :key="index" :value="world.id">{{ world.title }}</option>
      </select>
    </div>
    <div class="settings row right">
      <label>
        Kollisionserkennungstracking anzeigen
        <input
          type="checkbox"
          v-model="debug.collisionTracking"
        >
      </label>
      <br>
      <label>
        Kachel IDs anzeigen
        <input type="checkbox" v-model="debug.showTileIDs">
      </label>
      <br>
      <label>
        Koordinaten anzeigen
        <input type="checkbox" v-model="debug.showCoordinates">
      </label>
    </div>
    <div class="world-wrapper">
      <div class="parallax" :style="parallaxBackground">
        <div class="world-sun">
          <div
            class="row"
            v-if="selectedWorld"
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
                      <div v-if="shouldHighlight(row, col)" class="highlight"></div>
                    </transition>
                  </template>
                </div>
              </div>
            </template>
          </div>
        </div>
      </div>
      <transition name="finished-modal">
        <div v-if="finished" class="finished-wrapper">
          <div class="finished">
            <p class="headline">Das ist das Ende!</p>
            <p>Du kannst die Welt nun erneut starten oder in das nächste Level springen!</p>

            <div>
              <button @click="restart">Level neustarten</button>
              <div class="spacer">&nbsp;</div>
              <button @click="next">Nächstes Level spielen</button>
            </div>
          </div>
        </div>
      </transition>
    </div>
  </div>
</template>

<script>
import worlds from "./worlds.json";

export default {
  data() {
    return {
      worldId: "FlatWorld",
      store: [],
      swapPlayer: false,
      selectedWorld: null,
      flipPlayer: true,
      highlight: [],
      moving: true,
      keydown: false,
      debug: {
        collisionTracking: false,
        showTileIDs: false,
        showCoordinates: false
      },
      starting: true,
      showTutorial: true,
      shiftLeft: 0,
      finished: false
    };
  },
  watch: {
    worldId(value) {
      const { world } = this.worlds.find(world => world.id === value) || {
        world: null
      };

      if (!world) {
        this.selectedWorld = null;
      }

      this.selectedWorld = world.map(w => [...w]);
      this.starting = true;
    },
    selectedWorld() {
      if (this.finished) {
        this.shiftLeft = 0;
      }
    }
  },
  computed: {
    worlds() {
      return worlds;
    },
    worldMeta() {
      if (this.selectedWorld) {
        return {
          rows: this.selectedWorld.length,
          cols: this.selectedWorld[0].length
        };
      }
      return null;
    },
    playerPosition() {
      const y = this.selectedWorld.findIndex(row =>
        row.some(col => col === "S")
      );
      const x = this.selectedWorld[y].findIndex(col => col === "S");

      return { x, y };
    },
    parallaxBackground() {
      return {
        backgroundPositionX: `${this.shiftLeft}px, ${this.shiftLeft *
          2}px, ${this.shiftLeft * 3}px, ${this.shiftLeft * 5}px`
      };
    }
  },
  mounted() {
    document.addEventListener("keyup", () => {
      this.moving = true;
      this.keydown = false;
    });
    document.addEventListener("keydown", this.handleKeyDown);

    if (this.worldId) {
      this.worldId = this.worldId;
      this.selectedWorld = this.worlds
        .find(world => world.id === this.worldId)
        .world.map(w => [...w]);
    } else {
      const first = this.worlds[0];
      this.worldId = first.id;
      this.selectedWorld = first.world;
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
    restart() {
      this.selectedWorld = this.worlds
        .find(world => world.id === this.worldId)
        .world.map(w => [...w]);

      this.finished = false;
    },
    next() {
      const index = this.worlds.findIndex(world => world.id === this.worldId);

      if (index < this.worlds.length - 1) {
        this.worldId = this.worlds[index + 1].id;
      } else {
        this.worldId = this.worlds[0].id;
      }

      this.finished = false;
    },
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
        case "ArrowLeft": {
          this.movePlayer("left");
          this.shiftLeft += 1;
          break;
        }
        case "ArrowRight": {
          this.movePlayer("right");
          this.shiftLeft -= 1;
          break;
        }
      }
    },
    getValue(y, x) {
      y -= 1;
      x -= 1;

      if (this.selectedWorld) {
        if (this.selectedWorld[y]) {
          return this.selectedWorld[y][x];
        }
      }
      return 0;
    },
    movePlayer(direction) {
      const { x, y } = this.playerPosition;

      switch (direction) {
        case "left":
          this.flipPlayer = true;
          this.setPlayerPosition({ x: x - 1, y });
          break;
        case "right":
          this.flipPlayer = false;
          this.setPlayerPosition({ x: x + 1, y });
          break;
      }
    },
    setPlayerPosition(newPos) {
      const old = this.playerPosition;

      if (this.canMove(newPos)) {
        if (old.y !== newPos.y) {
          const row = this.selectedWorld[old.y].slice(0);
          const rowNew = this.selectedWorld[newPos.y].slice(0);

          row[old.x] = "0";
          rowNew[old.x] = "S";

          this.$set(this.selectedWorld, old.y, row);
          this.$set(this.selectedWorld, newPos.y, rowNew);
        }

        if (old.x !== newPos.x) {
          const row = this.selectedWorld[newPos.y].slice(0);
          row[old.x] = "0";
          row[newPos.x] = "S";
          this.$set(this.selectedWorld, newPos.y, row);
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
      const entity = this.getValue(newPos.y + 1, newPos.x + 1);

      this.highlight.push({
        x: newPos.x + 1,
        y: newPos.y + 1,
        dismiss: 500
      });

      if (entity === "X") {
        this.finished = true;
      }

      return entity === "0";
    },
    shouldHighlight(row, col) {
      return this.highlight.some(h => h.x === col && h.y == row);
    },
    tileSpecificClasses(row, col) {
      return {
        underlying:
          this.getValue(row - 1, col) === this.getValue(row, col) || row === 1,
        flip: this.getValue(row, col) === "S" && this.flipPlayer,
        moving: this.keydown,
        starting: this.starting
      };
    }
  }
};
</script>

<style lang="scss">
html {
  height: 100vh;
  width: 100vw;

  color: white;

  * {
    margin: 0;
    padding: 0;
  }
}

select {
  font-size: 18px;
}

div.wrapper {
  font-family: Helvetica, Courier, monospace;
  height: 100vh;
  width: 100vw;
  position: relative;
  background: black;

  > div.row {
    display: block;

    > div {
      display: inline-block;
    }

    &.settings {
      position: absolute;
      top: 10px;
      opacity: 0.7;

      z-index: 99;

      &.left {
        left: 10px;
      }

      &.right {
        right: 10px;
        text-align: right;
      }
    }
  }
}

div.world-wrapper {
  overflow: hidden;

  background-image: radial-gradient(
    circle 60px at 38% 50%,
    rgb(228, 195, 7),
    rgb(255, 242, 172),
    rgb(184, 220, 204)
  );

  position: absolute;
  top: 50%;
  transform: translateY(-50%);
  width: 100%;
  margin: auto;

  div.parallax {
    background-image: url("./images/bg.png"), url("./images/plx-4.png"),
      url("./images/plx-2.png"), url("./images/plx-3.png");
    transition: background-position 0.5s ease-in-out;
    background-size: auto 100%;
  }

  div.world-sun {
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
  }

  .finished-wrapper {
    position: absolute;
    z-index: 1000;
    display: flex;
    align-items: center;
    justify-content: center;
    top: 0;
    right: 0;
    bottom: 0;
    left: 0;

    .finished {
      background: rgb(184, 220, 204);

      padding: 10px;
      text-align: center;
      color: black;
      box-shadow: 5px 5px 10px 10px rgba(0, 0, 0, 0.5);

      p {
        &.headline {
          font-size: 25px;
        }

        padding: 10px 0;

        &:first-child {
          white-space: nowrap;
        }
      }

      > div {
        padding-top: 10px;
        display: flex;

        button {
          flex: 1;

          -webkit-appearance: none;
          outline: 0;

          font-size: 16px;
          padding: 10px 0;
          background: transparent;
          border: 1px solid black;
          cursor: pointer;

          &:hover {
            background: rgb(139, 185, 165);
          }
        }
      }
    }
  }
}

div.tile {
  &.tile- {
    &1 {
      background: url("./images/tileset.png") no-repeat;
      background-size: initial;
      background-position: -46px -30px;
      background-size: 1200px;

      &.underlying {
        background-position: -46px -78px;
      }
    }

    &S {
      background: url("./images/player_idle.gif") no-repeat center;
      background-size: contain;

      &.moving {
        background: url("./images/run.gif") no-repeat center;
        background-size: contain;
      }

      &.flip {
        transform: scaleX(-1);
      }

      &.starting {
        background: url("./images/midair.gif") no-repeat center;
        background-size: contain;
      }
    }

    &X {
      z-index: 99;
      position: relative;

      &:before {
        content: "";
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
        content: "";
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
      content: "";
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      left: 0;
      background-color: rgba(255, 0, 0, 0.15);
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
      content: "";

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

.finished-modal-enter-active,
.finished-modal-leave-active {
  transition: all 1s ease-in-out;
}

.finished-modal-enter,
.finished-modal-leave-to {
  transform: translateY(-100%);
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

@-webkit-keyframes pulse {
  0% {
    -webkit-box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
  }
  70% {
    -webkit-box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
  }
  100% {
    -webkit-box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
  }
}
@keyframes pulse {
  0% {
    -moz-box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
    box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
  }
  70% {
    -moz-box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
    box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
  }
  100% {
    -moz-box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
    box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
  }
}
</style>
