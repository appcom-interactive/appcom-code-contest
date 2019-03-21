<template>
  <div>
    <div
      class="board"
      :style="gridStyle"
    >
      <template v-for="(row, rowIndex) in board">
        <div
          v-for="(item, itemIndex) in row"
          :key="`item-${rowIndex}-${itemIndex}`"
          :style="getItemStyle(rowIndex, itemIndex)"
        >
          <span
            v-if="item === 1"
            v-html="queen"
          ></span>
        </div>

      </template>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Board',
  props: {
    board: Array
  },
  computed: {
    gridStyle() {
      return {
        width: `${this.board.length * 75}px`,
        height: `${this.board.length * 75}px`,
        'grid-template-columns': `repeat(${this.board.length}, 75px)`,
        'grid-template-rows': `repeat(${this.board.length}, 75px)`
      };
    },
    queen() {
      return '&#x2655';
    }
  },
  methods: {
    getItemStyle(rowIndex, itemIndex) {
      return {
        'background-color':
          rowIndex % 2 === 0
            ? itemIndex % 2 === 0
              ? '#fff'
              : '#000'
            : itemIndex % 2 !== 0
            ? '#fff'
            : '#000',
        color:
          rowIndex % 2 === 0
            ? itemIndex % 2 === 0
              ? '#000'
              : '#fff'
            : itemIndex % 2 !== 0
            ? '#000'
            : '#fff'
      };
    }
  }
};
</script>

<style lang="scss" scoped>
.board {
  display: grid;

  border: 1px solid black;
  margin: 40px auto;
  div {
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 30pt;
  }
}
</style>
