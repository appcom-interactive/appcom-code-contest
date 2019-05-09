<template>
  <div>
    <input type="text" v-model="tempSize" />
    <button @click="calculate">calculate</button>
    <div
      class="board"
      v-for="[index, board] in solutions"
      :key="'board' + index"
    >
      <span>{{index}}.)</span>
      <div class="row" v-for="(row, index) in board" :key="'row' + index">
        <div
          class="column"
          v-for="(column, index) in row"
          :key="'column' + index"
          :class="{ active: column }"
        ></div>
      </div>
    </div>
  </div>
</template>

<script>
import BoardClass from './index.js';

export default {
  data() {
    return {
      tempSize: 8,
      size: 8,
      solutions: [],
      board: null
    };
  },
  methods: {
    calculate() {
      this.size = this.tempSize;
      const board = new BoardClass(this.size);
      board.run();
      this.solutions = board.getSolutions();
    }
  },
  created() {
    const board = new BoardClass(this.size);
    board.run();
    this.solutions = board.getSolutions();
  }
};
</script>

<style>
.board {
  margin: 80px;
}

.row {
  display: grid;
  grid-auto-columns: 40px;
  grid-auto-rows: 40px;
  grid-template-columns: repeat(auto-fit, minmax(auto, 40px));
}
.column {
  border: 1px solid #ddd;
}

.column.active {
  background-color: #000;
}

span {
  font-size: 20px;
  margin-bottom: 10px;
  display: block;
}
</style>
