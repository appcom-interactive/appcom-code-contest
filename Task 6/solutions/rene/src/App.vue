<template>
  <div id="app">
    <label>
      n =
      <input
        v-model.number="numberofQueens"
      >
    </label>
    <button @click="initQueens" :disabled="numberofQueens <= 0">Start</button>

    <ol v-if="solvedBoards.length">
      <li
        v-for="(solvedBoard, boardIndex) in solvedBoards"
        :key="boardIndex"
      >
        <chess-board :board="solvedBoard"> </chess-board>
      </li>
    </ol>

    <div
      v-else-if="startedSolving"
      class="noSolution"
    > Für n = {{numberofQueens}} gibt es keine Lösung</div>

  </div>
</template>

<script>
import ChessBoard from './components/ChessBoard.vue';
export default {
  name: 'app',
  components: {
    ChessBoard
  },
  data() {
    return {
      numberofQueens: 1,
      board: null,
      solvedBoards: [],
      startedSolving: false
    };
  },
  methods: {
    initQueens() {
      this.startedSolving = true;
      this.solvedBoards = [];
      this.board = Array(this.numberofQueens)
        .fill()
        .map(() => Array(this.numberofQueens).fill(0));

      this.solveQueens();
    },
    canQueenBePlaced(row, column) {
      // links
      for (let i = 0; i < column; i++)
        if (this.board[row][i]) {
          return false;
        }

      // diagonal oben links
      for (let i = row, j = column; i >= 0 && j >= 0; i--, j--) {
        if (this.board[i][j]) {
          return false;
        }
      }

      // diagonal unten links
      for (let i = row, j = column; j >= 0 && i < this.board.length; i++, j--) {
        if (this.board[i][j]) {
          return false;
        }
      }

      return true;
    },
    solveQueens(col = 0) {
      // Wenn alle gesetzt wurden pushe sie ins solved Array und return
      // damit werden dann weitere lösungen gefunden
      if (col >= this.board.length) {
        this.solvedBoards.push(
          this.board.map(row => {
            return row.slice();
          })
        );
        return false;
      }

      // platziere Queen in Spalte
      for (let i = 0; i < this.board.length; i++) {
        // check ob die dort stehen kann
        if (this.canQueenBePlaced(i, col)) {
          // setze eine 1 ins Array falls ja
          this.$set(this.board[i], col, 1);

          // gehe rekursiv in die nächste Spalte, falls es eine Lösung gibt return true
          if (this.solveQueens(col + 1)) {
            return true;
          }
          // falls es keine Lösung gibt setze Queen wieder auf 0
          this.$set(this.board[i], col, 0);
        }
      }

      // falls keine mehr gesetzt werden kann return false
      return false;
    }
  }
};
</script>

<style lang="scss">
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
  .noSolution {
    margin: 30px 0;
    font-size: 30pt;
  }
}
</style>
