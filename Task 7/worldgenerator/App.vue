<template>
  <div class="wrapper">
    <div class="row">
      <div id="settings">
        Rows: <br /><input type="number" v-model="rows" /> <br />
        Columns:<br /><input type="number" v-model="cols" /><br />
        <input type="checkbox" v-model="showGrid" /> Grid<br /><br />
      </div>
    </div>
    <div class="row">
      <div
        id="world"
        :style="{
          'grid-template-rows': `repeat(${rows}, 62px`,
          'grid-template-columns': `repeat(${cols}, 62px`
        }"
      >
        <template v-for="row in computedRows">
          <div
            v-for="col in computedCols"
            :key="`${row}-${col}`"
            class="block"
            :class="{ editing: getValue(row, col).editing, grid: showGrid }"
          >
            <div
              :class="[
                `tile-${getValue(row, col).value}`,
                getValue(row - 1, col).value === getValue(row, col).value
                  ? 'underlying'
                  : ''
              ]"
              class="tile"
            ></div>

            <div
              class="hoverable"
              @click="getValue(row, col).value = tileSelection"
            ></div>
          </div>
        </template>
      </div>
    </div>
    <div class="row">
      <div id="tileselector">
        <p>Tile selection</p>
        <div class="grid">
          <div
            v-for="(tile, index) in allowedChars"
            :key="index"
            class="tile"
            :class="[`tile-${tile}`, tileSelection === tile ? 'selected' : '']"
            @click="tileSelection = tile"
          ></div>
        </div>
      </div>
    </div>
    <div class="row">
      <div id="computedWorld">
        <p>World array</p>
        <pre @click="copy">{{ fieldString }}</pre>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      cols: 8,
      rows: 8,
      store: [],
      showGrid: false,
      tileSelection: undefined,
      swapPlayer: false
    };
  },
  computed: {
    computedCols() {
      return Number.parseInt(this.cols);
    },
    computedRows() {
      return Number.parseInt(this.rows);
    },
    computedField() {
      return [...new Array(this.computedRows)].map((r, row) => {
        return [...new Array(this.computedCols)].map((c, col) => {
          return this.getValue(row + 1, col + 1).value;
        });
      });
    },
    fieldString() {
      return JSON.stringify(this.computedField, undefined, 2);
    },
    allowedChars() {
      return ["0", "1", "X", "S", "P", ""];
    }
  },
  mounted() {
    document.addEventListener("keyup", this.handleKeyUp);
    document.addEventListener("keydown", e => {
      if ([32, 37, 38, 39, 40].indexOf(e.keyCode) > -1) {
        e.preventDefault();
      }
    });
  },
  methods: {
    handleKeyUp() {
      event.preventDefault();
      console.log(event.code);

      switch (event.code) {
        case "ArrowLeft": {
        }
        case "ArrowRight": {
        }
      }
    },
    getValue(x, y) {
      let field = this.store.find(field => field.x === x && field.y === y);

      if (!field) {
        field = {
          x,
          y,
          value: "0",
          editing: false
        };

        this.store.push(field);
      }

      return field;
    },
    upsert(x, y, value, focused) {
      if (!focused && value === "") {
        value = 0;
      }

      value = String(value).toUpperCase();

      if (this.allowedChars.includes(value)) {
        const field = this.getValue(x, y);
        field.value = value;

        if (!focused) {
          field.editing = false;
        }
      } else {
        alert(`Der Wert ${value} ist ungültig.`);
      }
    },
    copy() {
      navigator.clipboard.writeText(this.fieldString);
      alert("Spielwelt wurde in die Zwischenablage kopiert");
    },
    edit(x, y) {
      const field = this.getValue(x, y);
      field.editing = true;
      this.$nextTick(() => {
        this.$refs[`input-${x}-${y}`][0].focus();
      });
    }
  }
};
</script>

<style lang="scss" scoped>
div.wrapper {
  font-family: Helvetica, Courier, monospace;

  > div.row {
    display: block;

    > div {
      display: inline-block;
    }
  }
}

div div#world {
  display: inline-grid;
  background: url("./images/bg.png");
  background-size: auto 100%;

  div.block {
    position: relative;
    box-sizing: border-box;

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

div#tileselector {
  div.grid {
    display: grid;
    grid-template-columns: repeat(8, 1fr);

    div.tile {
      width: 62px;
      height: 62px;
      border: 2px solid lightgray;
      cursor: pointer;

      &.selected {
        border: 2px solid green;
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
    }

    &X {
      background: rgba(252, 156, 156, 0.479);
    }
  }
}

div#computedWorld pre {
  cursor: pointer;

  &:hover {
    text-decoration: underline;
  }
}
</style>
