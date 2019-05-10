<template>
  <div>
    <div id="settings">
      Rows: <br /><input type="number" v-model="rows" /> <br />
      Columns:<br /><input type="number" v-model="cols" /><br /><br />
    </div>
    <div
      id="world"
      :style="{
        'grid-template-rows': `repeat(${rows}, 1fr`,
        'grid-template-columns': `repeat(${cols}, 1fr`
      }"
    >
      <template v-for="row in computedRows">
        <div v-for="col in computedCols" :key="`${row}-${col}`" class="block">
          <input
            type="text"
            @keypress.enter="upsert(row, col, $event.target.value)"
            @blur="upsert(row, col, $event.target.value)"
            :value="
              getValue(row, col).value === '0' ? '' : getValue(row, col).value
            "
            size="1"
            @focus="upsert(row, col, '', true)"
          />
        </div>
      </template>
    </div>

    <div id="computedWorld">
      <p>Spielwelt</p>
      <pre @click="copy">{{ fieldString }}</pre>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      cols: 8,
      rows: 8,
      store: []
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
  methods: {
    getValue(x, y) {
      let field = this.store.find(field => field.x === x && field.y === y);

      if (!field) {
        field = {
          x,
          y,
          value: "0"
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
      } else {
        alert(`Der Wert ${value} ist ungültig.`);
      }
    },
    copy() {
      navigator.clipboard.writeText(this.fieldString);
      alert("Spielwelt wurde in die Zwischenablage kopiert");
    }
  }
};
</script>

<style lang="css" scoped>
div {
  font-family: Helvetica, Courier, monospace;
}

div div#world {
  display: grid;
}

div#world div.block {
  border: 1px solid black;
  height: 40px;
  position: relative;
  display: grid;
  align-items: center;
  justify-content: center;
}

div.block input {
  font-size: 22px;
  -webkit-appearance: none;
  border: 0;
}

div#computedWorld pre {
  cursor: pointer;
}

div#computedWorld pre:hover {
  text-decoration: underline;
}
</style>
